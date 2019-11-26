## simple-glfw

Utility library for working with GLFW. 2 goals: simplify context initialization, simplify dependency management.

1. Provides RAII absraction for Window: Window mutating glfw functions are directly accessible as methods. Ctor initializes GLFW and OpenGL. Supports move semantics. 

2. Build scripts for building crossplatform. Platforms currently supported: MacOS, Windows, Linux, WebAssembly.

TODO: look into Android. I am aware of an Android port of GLFW but unaware of status.
TODO: CI scripts: living build instructions, unit test runners.
TODO: more construction time options, especially forward compat and vulk support.
