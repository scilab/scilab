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
 - cov - Covariance matrix. Deprecates 'mvvacov'. See bug #11896 fixed.

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

* paramfplot2d: When input argument theta was column vector, an error occurred.

* Bug #5539 fixed - sylv() help page was wrong in the discrete-time case.

* Bug #6693 fixed - modulo did not accept polynomial inputs. Help page was not updated.

* Bug #7684 fixed - Introduction demo splitted in subsections.

* Bug #8667 fixed - Handling of %nan in functions min,max,median is not correctly documented.

* Bug #8824 fixed - taucs_chfact returned a segfault (not the case in mode nwni)

* Bug #10862 fixed - Add a without internet connection installation
                     global configuration on the installer

* Bug #10995 fixed - Typo in grand() for Gamma law argument

* Bug #11343 fixed - The "isoview" figure property did not work when the axes
                     margins had been modified.

* Bug #11479 fixed - Under Ubuntu Unity, the color of the text menu was wrong.

* Bug #12163 fixed - unzoom did not work with a single input argument.

* Bug #12443 fixed - Document behavior of mopen() in text file mode
                     on Windows.

* Bug #12463 fixed - Bad specification for rect=[x,y,w,h] argument in french help
                     page of xstringb.

* Bug #12473 fixed - Problems with "é" in french help page of mkdir.

* Bug #12475 fixed - csvRead() does not support double quoted fields.
