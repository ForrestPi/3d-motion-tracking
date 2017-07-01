#include "ParticleTracker.h"

ParticleTracker::ParticleTracker()
{
  cout << GREEN << "[Tracker]" << RESET << " Initialising SIFT particle tracker..." << endl;

  this->sift      = SIFT::create();
  this->alignment = new Alignment(_dist, this->maxDisplacement);
  this->grid      = nullptr;
  this->alignment->setVisualisation(true);
}

ParticleTracker::~ParticleTracker()
{
  cout << CYAN << "[Tracker]" << RESET << " Terminating SIFT particle tracker..." << endl;
  delete this->sift;
  delete this->alignment;
  delete this->grid;
}

void ParticleTracker::initialiseGrid(int w, int h)
{
  if (this->grid != nullptr)
  {
    cout << YELLOW << "[Tracker] Grid is already initialised. Skipping..." << RESET << endl;
  }
  else
  {
    cout << GREEN << "[Tracker] Initialising Grid" << RESET << endl;
    this->grid = new Grid(Size(w, h), maxDisplacement);
  }
}

tuple<vector<Point2f>,Mat> ParticleTracker::detectPoints(Mat &in)
{
  vector<Point2f> points;
  vector<KeyPoint> keypoints;
  Mat descriptors;
  this->sift->detectAndCompute(in, noArray(), keypoints, descriptors);
  KeyPoint::convert(keypoints, points);
  return make_tuple(points, descriptors);
}

function<void (Mat)> ParticleTracker::track()
{
  cout << "[Press Ctrl+c to escape]" << endl;
  auto pipe = [&](Mat im)
  {
    if (this->grid == nullptr)
      this->initialiseGrid(im.cols, im.rows);
    else
      this->grid->neutralise(true);
    
    trackFeatures(im);
  };

  return pipe;
}

void ParticleTracker::trackFeatures(Mat &im)
{
  auto pointsAndFeatures = detectPoints(im);
  auto points = get<0>(pointsAndFeatures);
  Mat features = get<1>(pointsAndFeatures);
  // Draw all detected points as RED
  #ifdef DRAW_ALL_POINTS
  DrawUtils::drawMarks(im, points, Scalar(0,50,255));
  #endif
  
  if (!this->prevPoints.empty())
  {
    auto pairs = alignment->align(prevPoints, points, prevFeatures, features);
    set<int> trackedPoints;

    for (auto pair : pairs)
    {
      // Draw connecting line between two aligned points
      int i = pair.first;
      int j = pair.second;
      trackedPoints.insert(j);
      if (_dist(prevPoints[i], points[j]) >= MIN_DISTANCE_TO_DRAW_TRAIL)
        line(im, prevPoints[i], points[j], Scalar(0,50,255), 1, CV_AA);
      
      #ifndef DRAW_ALL_POINTS
      DrawUtils::drawSpot(im, points[j], Scalar(0,50,255));
      #endif

      // Register the previous points as anchors in the [Grid]
      Point2i p = Point2i((int)prevPoints[i].x, (int)prevPoints[i].y);
      Point2d v = Point2d(points[j].x - prevPoints[i].x,
                          points[j].y - prevPoints[i].y);
      this->grid->setAnchor(p, v);
    }

    // Highlight new points with GREEN (without matched previous points)
    #ifdef DRAW_NEW_POINTS_GREEN
    for (int j=0; j<points.size(); j++)
    {
      if (trackedPoints.find(j) == trackedPoints.end())
        DrawUtils::drawSpot(im, points[j], Scalar(0,255,50));
    }
    #endif

    // Track the feature points with [Grid]
    auto vEmpty = vector<tuple<Point2i, Point2d>>();
    this->grid->renderVelocityMap("Grid", vEmpty);

    #ifdef DEBUG_ALIGNMENT
    cout << "... " << points.size() << " feature points (" 
         << trackedPoints.size() << " tracked)" << endl;
    #endif
  }

  imshow("sift", im);
  
  // Store the points
  this->prevPoints.swap(points);
  
  // Store the features
  features.copyTo(this->prevFeatures);
}