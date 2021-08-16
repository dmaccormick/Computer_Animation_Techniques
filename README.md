# Computer Animation Techniques

## Introduction
This repository contains several small projects that demonstrate computer animation techniques. These projects were assignments for a 2nd year animation class. They are written in C++ and are integrated into a framework that uses immediate mode OpenGL for rendering. 

## The Different Projects

### A - Morph Targets
This project is designed to showcase the logic behind morphing between keyed poses. It loads the different poses as OBJ files and then allows the user to blend between them. NOTE: This is immediate mode OpenGL, and so the morphing is done on the CPU. In modern OpenGL, this could be done in the vertex shader.

### B - LERP 
This project showcases linear interpolation (LERP). The user is able to add points to a path and have an object move along them. The points can also be saved and loaded for later use.

### C - Keyframe Systems 
This contains a simple keyframe animation system with attributes such as position, rotation, and scale. The program includes keyframes necessary to replicate an animation made in Maya that is of a minigolf putter hitting a ball along a course.

### D - Paths and Curves
Similar to project B, this involves an object following a path. However, in this project, the user can also change the path to be a Bezier or Catmull-Rom curve.

### E - Steering Behaviours
This project showcases some of the basic steering behaviours, as defined by Craig Reynolds (https://www.red3d.com/cwr/steer/), such as Seek and Flee. 
