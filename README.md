[![Build Status](https://travis-ci.org/jfcameron/gdk-graphics.svg?branch=master)](https://travis-ci.org/jfcameron/gdk-graphics) [![Coverage Status](https://coveralls.io/repos/github/jfcameron/gdk-graphics/badge.svg?branch=master)](https://coveralls.io/github/jfcameron/gdk-graphics?branch=master) [![Documentation](https://img.shields.io/badge/documentation-doxygen-blue.svg)](https://jfcameron.github.io/gdk-graphics/)

## simple-glfw

Utility library for working with GLFW. 2 goals: simplify context initialization, simplify dependency management.

1. Provides RAII absraction for Window: Window mutating glfw functions are directly accessible as methods. Ctor initializes GLFW and OpenGL. Supports move semantics. 

2. Build scripts for building crossplatform. Platforms currently supported: MacOS, Windows, Linux, WebAssembly.

TODO: look into Android. I am aware of an Android port of GLFW but unaware of status.
TODO: CI scripts: living build instructions, unit test runners.
TODO: more construction time options, especially forward compat and vulk support.

## CI & Documentation

Documentation generated with doxygen ca be viewed online here: https://jfcameron.github.io/gdk-graphics/

Coverage calculated with gcov viewable here: https://coveralls.io/github/jfcameron/gdk-graphics

CI done using Travis CI. Build scripts cover Windows, Linux, Mac; Clang, GCC, MSVC, MinGW: https://travis-ci.org/jfcameron/gdk-graphics

catch2 unit tests available under `test/`.

