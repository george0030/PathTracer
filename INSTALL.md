# Building

PathTracer has only been tested functional on 64-bit Linux.

## Requirements

* Cmake of version 3.0 or larger
* Eigen version 3.3 - Install with ```sudo apt-get install libeigen3-dev```
* SFML library of version 2.5 - Install with ```sudo apt-get install libsfml-dev```

Run the following commands from the project root directory

* ```cd build```
* ```cmake -DCMAKE_BUILD_TYPE=Release ..```
* ```make```

The executable will be found at build/PathTracer
