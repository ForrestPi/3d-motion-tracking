# 3D Motion Track

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/c03c9cdb9f494244ab76268f809019d0)](https://www.codacy.com/app/tao/3d-motion-tracking?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=starcolon/3d-motion-tracking&amp;utm_campaign=Badge_Grade)

Detect and track object via webcam in (semi) 3D fashion.

---

## Prerequisites

Ensure you have following installed:

  - [x] [gcc](https://gcc.gnu.org/) with C++11 support.
  - [x] [OpenCV 3.x](http://opencv.org/downloads.html) **with contribution package**
  - [x] A webcamera !


## Compile and run

As simple as:

```
$ make
$ bin/track
```

The tracking program runs forever. Tab `Ctrl + C` inside your terminal 
window to terminate.


## Disclaimer

The build is configured to perfectly fit on MacOS (g++ as a gcc compiler). 
Your mileage may vary if you are compiling the code on a different OS. 

---

## My build environment

In case you face difficulty building on your own machine, 
following is the specifications of my environment which may be 
helpful for you to spot the difference to yours.

<b>OS</b>

```
OSX El Capitan 10.11.6
```

<b>gcc -v</b>

```
Apple LLVM version 8.0.0 (clang-800.0.42.1)
Target: x86_64-apple-darwin15.6.0
```

<b>OpenCV3</b>

OpenCV 3.2.0 built and installed via following [homebrew](https://brew.sh/) params:

```
--with-ffmpeg --without-numpy --without-test --with-contrib --with-python3
```

---

# Licence

FreeBSD
