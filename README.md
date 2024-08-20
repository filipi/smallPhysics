<img width="300" align="left" style="float: left; margin: 0 10px 0 0;" alt="screenshot" src="https://github.com/filipi/smallPhysics/blob/master/images/eitsim.png">   

smallPhysics
============

Small Physics Engine for simple C++ experiments

Originaly designed for softbody simulations intented to be used as phantoms to Electrical Impedance Tomography

Building example applications
=============================

There are two example applications, eitsim and mantle

* eitsim is a attempt to sketch a eletrical impedance simulator
  for a simple conductive foam.

* mantle is an example of using 2D physcis for clothing simulation

Linux/MacOS
===========

# Installing dependecies (glut, lib-jpeg, libglew)

## In Debian derivatives (Ubuntu, etc)

```sudo apt install freeglut3-dev libjpeg-dev libglew-dev```

## In MacOS

```brew install glew```

To compile the examples:

To simulate a matress: 

```make eitsim```

To simulate a sheet of fabric

```make mantle```

Cross compiling windows executable on Linux
===========================================

 - To cross compile example applications for windows on a Linux
   environment, you will need:
 * wine
 * wxDevCpp no wine
And the Glut devpack must be installed at the wxdevCpp.
http://devpaks.org/details.php?devpak=277
