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

* The Variable Browser shows also the size of integers.
  See bug #12523

* isnum has been redesigned in native code. Up to 130x performance
  improvements. See bug #10404


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

* Bug #12384 fixed - Using a modelica part linked with explicit link to
                     another modelica part lead to an algebraic loop error.


Toolbox skeleton
=================

* Add some Xcos specific content to xcos_toolbox_skeleton help pages.


Bug fixes
==========

* paramfplot2d: When input argument theta was column vector, an error occurred.

* Bug #4731 fixed - lqr() failed when time domain of input was a number.

* Bug #5539 fixed - sylv() help page was wrong in the discrete-time case.

* Bug #6168 fixed - Help for zpbutt, zpch1, zpch2 and zpell were unclear. 

* Bug #6693 fixed - modulo did not accept polynomial inputs. Help page was not updated.

* Bug #7684 fixed - Introduction demo splitted in subsections.

* Bug #7927 fixed - Output "flag" in qmr function was not well documented.

* Bug #8667 fixed - Handling of %nan in functions min,max,median is not correctly documented.

* Bug #8680 fixed - Output argument "end" of the regexp function has been changed.

* Bug #8824 fixed - taucs_chfact returned a segfault (not the case in mode nwni).

* Bug #9059 fixed - tbx_build_macros and genlib do not stop on error.

* Bug #9691 fixed - "imp" option of optim was poorly documented.

* Bug #10862 fixed - Add a without internet connection installation
                     global configuration on the installer

* Bug #10995 fixed - Typo in grand() for Gamma law argument

* Bug #11308 fixed - Calling sequences in help page of dsearch were wrong.

* Bug #11343 fixed - The "isoview" figure property did not work when the axes
                     margins had been modified.

* Bug #12163 fixed - unzoom did not work with a single input argument.

* Bug #12415 fixed - PATH environment variable grew when using call_scilab in a loop.

* Bug #12426 fixed - By using addErrorMessage, random string error could be displayed.

* Bug #12443 fixed - Document behavior of mopen() in text file mode
                     on Windows.

* Bug #12463 fixed - Bad specification for rect=[x,y,w,h] argument in french help
                     page of xstringb.

* Bug #12473 fixed - Problems with "é" in french help page of mkdir.

* Bug #12475 fixed - csvRead() does not support double quoted fields.

* Bug #12490 fixed - plot did not have a clf(); in its demo.

* Bug #12506 fixed - In SciNotes, miscolorization when returned values list was broken.

* Bug #12518 fixed - Polynoms were not displayed by default in browsevar.

* Bug #12520 fixed - Improve the description of the size in browsevar.

* Bug #12527 fixed - Scilab user functions were not listed in browsevar.

* Bug #12548 fixed - Duplicated code in xmltoformat.
