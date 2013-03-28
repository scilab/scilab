                    Changes between version 5.4.X and 5.5.0
                    =======================================

New features
=============

* Special functions improved:
 - erfi - The imaginary error function.
 - dawson - Compute the Dawson (scaled imaginary error) function.

* Functions erf, erfc, erfcx and calerf support complex arguments.

* New Solver:
 - daskr - differential-algebraic system solver with rootfinding 'daskr', using
BDF methods with direct and preconditioned Krylov linear solvers, from ODEPACK.

* New functions introduced:
 - getURL - Download an URL (HTTP, HTTPS, FTP...)
 - splitURL - Split a URL (HTTP, HTTPS, FTP...)

* Add a complete set of functions to read and write any HDF5 file from Scilab.


Syntax changes
===============

* Vector ^ scalar syntax declared obsolete. See bug #11524


Obsolete
=========

* The third argument of poly function will be more strict.
    Only the following strings will be allowed : "roots", "coeff", "c", "r".

* Arguments option and direction of gsort will be more strict in all case.
  Only the following strings will be allowed : option : "r" "c" "g" "lr" "lc"
                                              direction : "d" "i"

Xcos
====

* Update Sundials to lastest "2.5.0", keeping our modifications

* Bug #12359 fixed - Xcos files has been converted to zcos to gain some space.


Bug fixes
==========

* Bug #10862 fixed - Add a without internet connection installation
                     global configuration on the installer

* Bug #10995 fixed - Typo in grand() for Gamma law argument

* Bug #11343 fixed - The "isoview" figure property did not work when the axes
                     margins had been modified.

* Bug #12163 fixed - unzoom did not work with a single input argument.

* Bug #12463 fixed - Bad specification for rect=[x,y,w,h] argument in french help
                     page of xstringb.

* Bug #12473 fixed - Problems with "é" in french help page of mkdir.

