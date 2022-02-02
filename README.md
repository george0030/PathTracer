# PathTracer

PathTracer is a 3D rendering application that utilizes path tracing through single pixels.

## Authors:
* Katja Helenius
* Emil Lintunen
* Akseli Jaakkola

## Building

See INSTALL.md

## Pre-compiled executable

In case you have trouble building the executable, a pre-compiled executable can be found at ```build/PathTracer```.

## Usage

```PathTracer <Scene_file.xml> <Output_image.png>```
PathTracer takes in an XML file that describes a scene layout. The scene may contain spheres and triangle meshes consisting of multiple triangles. Each object has a color, as well as an emissivity value. The emissivity value is given as a color as well.

The Scene file XML layout can be found in ```doc/scene_sample.xml```

# Documentation

A Doxygen source file documentation for the project can be found at ```doc/html/index.html```
