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

* The Variable Browser shows also the size of integers and the user type of
  the tlist/mlist.
  See bug #12523 and #10409

* isnum has been redesigned in native code. Up to 130x performance
  improvements. See bug #10404

* The deletion of variables from the Variable Browser is now possible.
  See bug #9447

* In browsevar, allows a user to plot the variable (already available in
  editvar).


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

* nfreq() tagged as obsolete. Will be removed for Scilab 5.5.1.
  Use tabul() instead.


Xcos
====

* Update Sundials to lastest "2.5.0", keeping our modifications

* Bug #7350 fixed - The superblock I/O ports numbering was not updated
                    when a new port was dropped.

* Bug #12359 fixed - Xcos files has been converted to zcos to gain some space.

* Bug #12384 fixed - Using a modelica part linked with explicit link to
                     another modelica part lead to an algebraic loop error.

* Bug #12568 fixed - "Recent files" menu entry should be placed next to the 
                     "Open" one.

* Bug #12603 fixed - ZCOS files may contain blocs with modified (eg. invalid)
                     parameters.


Toolbox skeleton
=================

* Add some Xcos specific content to xcos_toolbox_skeleton help pages.


Bug fixes
==========

* paramfplot2d: When input argument theta was column vector, an error occurred.

* Bug #4731 fixed - lqr() failed when time domain of input was a number.

* Bug #5073 fixed - New parameter added at strtod function (decimal separator).

* Bug #5207 fixed - grand() can now return a hypermatrix.

* Bug #5365 fixed - makecell help page was in the "compatibility functions" directory
                    instead of being in "data_structures".

* Bug #5539 fixed - sylv() help page was wrong in the discrete-time case.

* Bug #6168 fixed - Help for zpbutt, zpch1, zpch2 and zpell were unclear.

* Bug #6693 fixed - modulo did not accept polynomial inputs. Help page was not updated.

* Bug #6752 fixed - unit test scilab.tst has been split in different tests.

* Bug #6988 fixed - Error messages in modules/data_structures/src/c/hmops.c were not standard.

* Bug #7026 fixed - There was no unit test for plot2d.

* Bug #7206 fixed - If the second input argument of meanf function was an hypermat, this function 
                    returned an error.

* Bug #7655 fixed - An example added in help page of type, for type(X)=11 and type(X)=13.

* Bug #7684 fixed - Introduction demo splitted in subsections.

* Bug #7828 fixed - Slight improvements in nicholschart.

* Bug #7781 fixed - The second parameter of iqr function had not effect.

* Bug #7927 fixed - Output "flag" in qmr function was not well documented.

* Bug #8162 fixed - Area of stability of plzr was wrong for continuous systems (+unit test added).

* Bug #8264 fixed - Documentation for Matlab's atan2() equivalent fixed.

* Bug #8319 fixed - dbphi(hypermat) and phasemag(hypermat) returned a matrix instead of a hypermatrix.

* Bug #8597 fixed - Uncontrolled message of grand/clcg4 should be displayed as warning.

* Bug #8607 fixed - Some error messages in modules/overloading/macros were not standard and not localized.

* Bug #8667 fixed - Handling of %nan in functions min,max,median is not correctly documented.

* Bug #8680 fixed - Output argument "end" of the regexp function has been changed.

* Bug #8687 fixed - typeof() function failed on uint8, depending on the format (+ unit test added).

* Bug #8824 fixed - taucs_chfact returned a segfault (not the case in mode nwni).

* Bug #8862 fixed -  mget and mput couldn't read and write 64 bit data from	78
                     binary file.

* Bug #9004 fixed - bitcmp function called with one input argument returned an error.

* Bug #9059 fixed - tbx_build_macros and genlib do not stop on error.

* Bug #9109 fixed - nfreq tagged as obsolete.

* Bug #9459 fixed - Default values of the optional plot3d arguments were not documented.

* Bug #9601 fixed - Cylinder demo failed with a bad value.

* Bug #9691 fixed - "imp" option of optim was poorly documented.

* Bug #9780 fixed - gmres solver did not run with complex systems.

* Bug #9851 fixed - Error message occurred because of a cut-off frequency of 0.25Hz with irr.

* Bug #10254 fixed - Slight improvements in help page of ones.

* Bug #10287 fixed - An error message added for complex expression as input argument of integrate function.

* Bug #10862 fixed - Add a without internet connection installation
                     global configuration on the installer

* Bug #10906 fixed - Typo in help page of cls2dls.

* Bug #10995 fixed - Typo in grand() for Gamma law argument

* Bug #10226 fixed - When a // <empty session> line is delete, all sessions
                     histories are folded.

* Bug #11308 fixed - Calling sequences in help page of dsearch were wrong.

* Bug #11343 fixed - The "isoview" figure property did not work when the axes
                     margins had been modified.

* Bug #11891 fixed - Fisher ratio could be inaccurate for one-way ANOVA.

* Bug #12045 fixed - repmat returned wrong result if numbers of input matrix were not double.

* Bug #12085 fixed - On Windows csvWrite wrote wrong EOL.

* Bug #12150 fixed - datatipCreate failed with one single point.

* Bug #12163 fixed - unzoom did not work with a single input argument.

* Bug #12376 fixed - Exec & edit buttons in examples in help were sometimes misplaced.

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

* Bug #12508 fixed - Wrong error message in the rand function fixed.

* Bug #12518 fixed - Polynoms were not displayed by default in browsevar.

* Bug #12520 fixed - Improve the description of the size in browsevar.

* Bug #12527 fixed - Scilab user functions were not listed in browsevar.

* Bug #12547 fixed - In SciNotes, lines were wrongly numbered with splitted function declarations.

* Bug #12548 fixed - Duplicated code in xmltoformat.

* Bug #12550 fixed - Optimbase and optimsimplex help pages were not standard.

* Bug #12551 fixed - Stack problem with diary([],"pause") and diary([],"resume") fixed.

* Bug #12556 fixed - A fatal error occurred when calling set() with wrong instructions.

* Bug #12564 fixed - Javasci compile & run help page is not clear about needed packages.

* Bug #12581 fixed - isfield could not support mlist or tlist.

* Bug #12589 fixed - Call sequence for spzeros & spones were missing in the french help pages.

* Bug #12606 fixed - Overloads for grand were not standard.

* Bug #12614 fixed - Helpbrowser was not launched in EDT.
