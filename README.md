# Welcome to P6
![One screenshot worth thousands words](/image/bridge.png)

## Features
- Simulates static solid constructions.
- Has nonlinear materials.
- Has possibility to read and write exact data in text format.
- Can save and import constructions.
- Can save images.
- Can be built on every platform supported by [wxWidgets](https://www.wxwidgets.org).
- Has automatic building, testing and documentation systems.

## Installation
- Linux:
  1. Build [wxWidgets](https://www.wxwidgets.org) or install it with your package manager.
  2. Download [Eigen](http://eigen.tuxfamily.org) and specify it's location in Makefile.
  3. (Optionally) Build [Google Test](https://github.com/google/googletest) or install it with your package manager.
  4. (Optionally) Install [Doxygen](https://www.doxygen.nl) with your package manager.
  5. Build project by typing in terminal:
  ```
  make
  ```
  6. Other available targets for Makefile are `run`, `test` (requires [Google Test](https://github.com/google/googletest)), `doc` (requires [Doxygen](https://www.doxygen.nl)) and `clean`.

- Windows:
  1. Build [wxWidgets](https://www.wxwidgets.org) or download it's header files, 64-bit development files and release DLLs. Specify their location in Makefile-nmake-64.
  2. Download [Eigen](http://eigen.tuxfamily.org) and specify it's location in Makefile-nmake-64.
  3. (Optionally) Build [Google Test](https://github.com/google/googletest) and specify it's location in Makefile-nmake-64.
  4. (Optionally) Download and install [Doxygen](https://www.doxygen.nl).
  5. Enter Visual Studio command prompt by calling `vsdevcmd.bat`. It is located in Visual Studio files, so you may need add it's folder to `PATH` or call it directly.
  6. Build project by typing:
  ```
  nmake /F Makefile-nmake-64
  ```
  7. Other available targets for Makefile are `run`, `test` (requires [Google Test](https://github.com/google/googletest)), `doc` (requires [Doxygen](https://www.doxygen.nl)) and `clean`.

Hints:
- If you are having issue with black icons, try building project with Visual Studio (better) or uncommenting `#-DICONS_SET_BACKGROUND` in Makefile-nmake-64 (worse).
- If you want to test P6 without having wxWidgets, delete `-D P6_FILE_WXWIDGETS` from Makefile or Makefile-nmake-64 respectively.

## Help
Application is developed to be simple, but there are some non-obvious moments:
1. For non-linear materials you can type arbitrary strain(stress) in "Formula" window. There is special variable "s" for "stress" and also "sin", "cos", "ln", "exp" functions.
2. Red point in "move" mode is the anchor. Selected elements are scaled and rotated around the anchor.
3. Do not forget to specify material and cross-sectional area of sticks.
4. Clicking on "node", "stick" or "force" tool makes selection contain only nodes, sticks or forces respectively.
5. Rotating mouse wheel with "force" tool activated makes forces be displayed bigger or smaller.
	

Created by Meta-chan (Kyrylo Sovailo)\
Feel free to contact me at [github](https://github.com/Meta-chan), [telegram](https://t.me/Meta_chan) and k.sovailo\@gmail.com