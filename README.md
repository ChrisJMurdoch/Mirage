
# World Engine

C++ libraries focused on procedural world generation, simulation, and rendering.

![Hex](https://github.com/ChrisJMurdoch/WorldEngine/blob/master/images/Hex.PNG?raw=true)

## Features
- Hardware-accelerated rendering with options such as dynamic lighting, instancing, and post-processing effects.
- Built-in procedural generation library with 3D Perlin noise, domain warping, and fractal-layering.
- Utilities and tools for monitoring and analysing library features.
- Configurable multithreading and hardware acceleration.

![Planet](https://github.com/ChrisJMurdoch/WorldEngine/blob/master/images/PlanetCropped.gif?raw=true)

## Building from source

Builds of all necessary third-party libraries are already included under the vendor/ directory.  Feel free to swap them out for newer or custom-built versions.

### With CMake

Included at the root of the project is a CMakeLists.txt file that can be used to build the project and required libraries from source:

> cmake -B ./[BUILD_DIR]/

> cmake --build ./[BUILD_DIR]/

Then run the program with:

> .\[BUILD_DIR]\Debug\Redshift.exe

### With _

Alternatively, the project can be built and run using the [_ build script](https://github.com/ChrisJMurdoch/_).  After setting the APP_NAME to 'Redshift', the project can be built and run with:

> _ init build run

Subsequent runs can be started with:

> _ run
