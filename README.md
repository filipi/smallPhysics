smallPhysics
============

Small Physics Engine for simple C++ experiments


Building example applications
=============================

There are two example applications, eitsim and mantle

* eitsim is a attempt to sketch a eletrical impedance simulator
  for a simple conductive foam.

* mantle is an example of using 2D physcis for clothing simulation

Linux
=====
make eitsim
make mantle

Cross compiling windows executable on Linux
===========================================

 - To cross compile example applications for windows on a Linux
   environment, you will need:
 * wine
 * wxDevCpp no wine
And the Glut devpack must be installed at the wxdevCpp.
http://devpaks.org/details.php?devpak=277
