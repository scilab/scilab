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
 - ismatrix - Check if a variable is a matrix. See bug #10456 fixed.
 - isrow - Check if a variable is a row vector. See bug #10456 fixed.
 - iscolumn - Check if a variable is a column vector. See bug #10456 fixed.
 - issquare - Check if a variable is a square matrix. See bug #10456 fixed.
 - cross - Vector cross product. See bug #9941 fixed.

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

* Update Windows Solution to Visual Studio 2012.

* The histplot command can now be used with the option polygon=%t/%f to add the
  frequency polygon chart (thanks to Mehran Khorshidi).

* Switch to the CeCILL 2.1 license


Syntax changes
===============

* Vector ^ scalar syntax declared obsolete. See bug #11524


Improvements
=============

* New calling sequence allowed for nicholschart: nicholschart(gains, phases, colors).
  See bug #7828.

* qp_solve can now take up to 5 output arguments. The last one is an error flag,
  if it is present, then the function will issue a warning instead of an error.
  See bug #10269.

* graypolarplot has been improved in term of performances and rendering.
  See bug #12641.

* nthroot() is now vectorizable.
  See bug #12678.


Obsolete
=========

* The third argument of poly function will be more strict.
    Only the following strings will be allowed : "roots", "coeff", "c", "r".

* Arguments option and direction of gsort will be more strict in all case.
  Only the following strings will be allowed : option : "r" "c" "g" "lr" "lc"
                                              direction : "d" "i"

* nfreq() tagged as obsolete. Will be removed for Scilab 5.5.1.
  Use tabul() instead.

* IsAScalar() tagged as obsolete. Will be removed for Scilab 5.5.1.
  Use isscalar() instead.

* chart() tagged as obsolete. Will be removed for Scilab 5.5.1.
  Use nicholschart() instead.

* sscanf is obsolete, use msscanf instead.

Differential_equations
======================

* Netlib's Quadpack, used for definite integration, has been updated to match the upstream.


SciNotes
========

* Autosave feature can now use %date to append the current date to the backup filename (See bug #12712).


Xcos
====

* Updated Sundials to the "2.5.0" version, keeping our modifications.

* Updated JGraphX to the "2.1.0.7" version, updating our performance hooks.

* Added a new DAE solver : DDaskr, using BDF methods with direct Newton and preconditioned Krylov linear solvers, which includes rootfinding.

* In the Modelica initialization GUI, inputs (eg. sensor) were not handled.

* Bug #7350 fixed - The superblock I/O ports numbering was not updated
                    when a new port was dropped.

* Bug #8570 fixed - "Region to superblock" has been renamed to
                    "Selection to superblock".

* Bug #11518 fixed - CLR block-text was displayed out of the bounding box
                     when zooming.

* Bug #12359 fixed - Xcos files has been converted to zcos to gain some space.

* Bug #12384 fixed - Using a modelica part linked with explicit link to
                     another modelica part lead to an algebraic loop error.

* Bug #12387 fixed - Setup menu option "Modelica initialize" did not blur
                     during simulation.

* Bug #12414 fixed - SWITCH_m block had different behaviors
                     for different types of inputs.

* Bug #12424 fixed - Calling lincos on a Derivative block made scilab crash.

* Bug #12460 fixed - xcosPalGenerateAllIcons sometimes crashed Scilab.

* Bug #12461 fixed - Cancelling zoom out by zooming in did not fully work.

* Bug #12561 fixed - SELECT_m and RELAY_f no more behaved as expected.

* Bug #12568 fixed - "Recent files" menu entry should be placed next to the
                     "Open" one.

* Bug #12590 fixed - Block shape style was removed on file loading.

* Bug #12603 fixed - ZCOS files may contain blocs with modified (eg. invalid)
                     parameters.

* Bug #12619 fixed - The discrete block DLR was not displaying latex formula
                     like continuous CLR block.

* Bug #12651 fixed - The 'nw' scicos_simulate option did not work while
                     passing a previous Info simulation status.

* Bug #12664 fixed - Inverted pendulum french translation was wrong in the Xcos demo.

* Bug #12667 fixed - 'Recent Files' menu was not localized.

* Bug #12731 fixed - Code generation produces erroneous block.

* Bug #12732 fixed - Improper Copyright comments in the files generated by the code generation tool.

* Bug #12868 fixed - There were several problems with the PULSE_SC block.

* Bug #12874 fixed - CSCOPE marks size were too small.


Toolbox skeleton
=================

* Add some Xcos specific content to xcos_toolbox_skeleton help pages.


Compilation
============

* Enforce a minimal version of Flexdock (1.2.4).


Bug fixes
==========

* paramfplot2d: When input argument theta was column vector, an error occurred.

* Bug #2267 fixed - Wrong error message when ticks locations and labels did not have the same sizes.

* Bug #2416 fixed - Particular case (string([]) returns []) has been added in the string help page.

* Bug #3928 fixed - An error was returned when a matrix was flipped along the third dimension.

* Bug #4042 fixed - squeeze returned an hypermatrix instead of a matrix when at least one
                    dimension was equal to 1.

* Bug #4085 fixed - help page of num2cell added.

* Bug #4229 fixed - delip did not return an error if one element of its first input argument
                    was negative.

* Bug #4383 fixed - csim with "step" and "impuls" now works with direct feedthrough.

* Bug #4481 fixed - Help page of iir updated: frq can be a scalar.

* Bug #4731 fixed - lqr() failed when time domain of input was a number.

* Bug #5073 fixed - New parameter added at strtod function (decimal separator).

* Bug #5207 fixed - grand() can now return a hypermatrix.

* Bug #5365 fixed - makecell help page was in the "compatibility functions" directory
                    instead of being in "data_structures".

* Bug #5539 fixed - sylv() help page was wrong in the discrete-time case.

* Bug #5616 fixed - typeof(uiCreateTree(...)) returned "Tree" instead of "uitree".

* Bug #5694 fixed - numdiff() help page clarified.

* Bug #5779 fixed - xnumb number format was too small (+ unit test added).

* Bug #6037 fixed - Documentation of macrovar() improved.

* Bug #6168 fixed - Help for zpbutt, zpch1, zpch2 and zpell were unclear.

* Bug #6427 fixed - full([%T %F]) returned an error message.

* Bug #6466 fixed - Example with vectorized input added in help pages of mprintf and msprintf.

* Bug #6693 fixed - modulo did not accept polynomial inputs. Help page was not updated.

* Bug #6752 fixed - unit test scilab.tst has been split in different tests.

* Bug #6988 fixed - Error messages in modules/data_structures/src/c/hmops.c were not standard.

* Bug #7026 fixed - There was no unit test for plot2d.

* Bug #7080 fixed - Some graphics macros did not use standard error messages.

* Bug #7206 fixed - If the second input argument of meanf function was an hypermat, this function
                    returned an error.

* Bug #7296 fixed - Enabled %nan, %inf and -%inf for the cdf* functions.

* Bug #7486 fixed - DGELSY and ZGELSY closer to the upstream, to propose our modifications to LAPACK.

* Bug #7596 fixed - Same error happening in different places now yields the same error message.

* Bug #7648 fixed - CDF functions now issue a warning for non integer "degrees of freedom" argument.

* Bug #7655 fixed - An example added in help page of type, for type(X)=11 and type(X)=13.

* Bug #7684 fixed - Introduction demo splitted in subsections.

* Bug #7781 fixed - The second parameter of iqr function had not effect.

* Bug #7824 fixed - The properties of title function did not support an indexed color.

* Bug #7826 fixed - chart tagged as obsolete.

* Bug #7828 fixed - Slight improvements in nicholschart.

* Bug #7848 fixed - The third argument of correl function is now optional.

* Bug #7927 fixed - Output "flag" in qmr function was not well documented.

* Bug #7960 fixed - plzr() could not produce pole zero plot for a simple transfer function.

* Bug #8098 fixed - cumsum could not be applied to rational matrices.

* Bug #8162 fixed - Area of stability of plzr was wrong for continuous systems (+unit test added).

* Bug #8234 fixed - strtod did not return an empty matrix when input argument was an empty matrix.

* Bug #8264 fixed - Documentation for Matlab's atan2() equivalent fixed.

* Bug #8319 fixed - dbphi(hypermat) and phasemag(hypermat) returned a matrix instead of a hypermatrix.

* Bug #8373 fixed - clear() can now take a matrix of strings argument.

* Bug #8462 fixed - bvodeS() could make Scilab unstable.

* Bug #8470 fixed - bvode() displayed some output in terminal window and not in Scilab console.

* Bug #8479 fixed - the latest Saxon version was not supported.

* Bug #8511 fixed - sprand used rand and grand functions, and internal state of the random
                    generator was changed.

* Bug #8561 fixed - ddassl(), ddasrt(), ddaskr(): abs and rel tolerances sizes are now checked.

* Bug #8597 fixed - Uncontrolled message of grand/clcg4 should be displayed as warning.

* Bug #8607 fixed - Some error messages in modules/overloading/macros were not standard and not localized.

* Bug #8614 fixed - Unit test for barhomogenize added.

* Bug #8636 fixed - roots() help page updated (default algorithm value was wrong).

* Bug #8667 fixed - Handling of %nan in functions min,max,median is not correctly documented.

* Bug #8680 fixed - Output argument "end" of the regexp function has been changed.

* Bug #8687 fixed - typeof() function failed on uint8, depending on the format (+ unit test added).

* Bug #8778 fixed - Call_ScilabOpen, TerminateScilab can not be called more
                    than 80 times in a loop.

* Bug #8779 fixed - gsort() did not preserve order of equal elements, in lexicographic sort.

* Bug #8820 fixed - Squeeze did not return a matrix when the number of dimensions
                    of the result was less or equal to 2.

* Bug #8824 fixed - taucs_chfact returned a segfault (not the case in mode nwni).

* Bug #8840 fixed - fileparts did not manage matrix of strings.

* Bug #8856 fixed - Non regression test added for [k,l,m,...]=find(a==5).
                    Non regression test of bug #476 updated.

* Bug #8857 fixed - Non regression test of bug #477 updated.

* Bug #8858 fixed - Non regression test of bug #480 updated.

* Bug #8862 fixed - mget and mput couldn't read and write 64 bit data from
                    binary files.

* Bug #9004 fixed - bitcmp function called with one input argument returned an error.

* Bug #9020 fixed - exists function did not accept matrix as first input argument.

* Bug #9059 fixed - tbx_build_macros and genlib do not stop on error.

* Bug #9109 fixed - nfreq tagged as obsolete.

* Bug #9385 fixed - The type checking in trigonometric functions has been added.

* Bug #9459 fixed - Default values of the optional plot3d arguments were not documented.

* Bug #9601 fixed - Cylinder demo failed with a bad value.

* Bug #9691 fixed - "imp" option of optim was poorly documented.

* Bug #9780 fixed - gmres solver did not run with complex systems.

* Bug #9821 fixed - getrelativefilename did not manage matrix of strings.

* Bug #9851 fixed - Error message occurred because of a cut-off frequency of 0.25Hz with irr.

* Bug #10146 fixed - In SciNotes, 'help on keyword' was at the end of the popup menu.

* Bug #10180 fixed - det was not defined for sparse matrices.

* Bug #10213 fixed - sci2exp can be impacted by format which has been mentionned in its help page.

* Bug #10216 fixed - Invalid syntaxes for zeros, ones, eye, rand, like zeros(2,:).

* Bug #10226 fixed - When a // <empty session> line is delete, all sessions
                     histories are folded.

* Bug #10254 fixed - Slight improvements in help page of ones.

* Bug #10269 fixed - qp_solve can now take up to 5 output arguments. The last one is an error flag,
                     if it is present, then the function will issue a warning instead of an error.

* Bug #10276 fixed - qp_solve() segfaulted with large matrices.

* Bug #10287 fixed - Added an error message for complex expression as input argument of integrate function.

* Bug #10305 fixed - Comparison of lists with empty items returned an error message.

* Bug #10596 fixed - exit(xxx) from Scilab was failing.

* Bug #10818 fixed - The following help pages have been added: hom, %e, %t, %f, %z and %s.

* Bug #10862 fixed - Add a without internet connection installation
                     global configuration on the installer

* Bug #10866 fixed - det() was not equivalent to detr() for rational matrices.

* Bug #10906 fixed - Typo in help page of cls2dls.

* Bug #10930 fixed - The comments in armax function were in french.

* Bug #10995 fixed - Typo in grand() for Gamma law argument

* Bug #11139 fixed - conj was not defined for sparse matrices (+ unit test added).

* Bug #10823 fixed - fullpath returned different results on Windows and Linux for non-existent file.

* Bug #11308 fixed - Calling sequences in help page of dsearch were wrong.

* Bug #11343 fixed - The "isoview" figure property did not work when the axes
                     margins had been modified.

* Bug #11648 fixed - Copy graphic to clipboard did not work.

* Bug #11766 fixed - nthroot has been added to the m2sci documentation.

* Bug #11792 fixed - Lists can be accessed with non integer indexes (help page updated).

* Bug #11869 fixed - "Environment" was not localized in preferences.

* Bug #11885 fixed - Each rand has been changed to grand in genetic algorithms and simulated annealing functions.

* Bug #11891 fixed - Fisher ratio could be inaccurate for one-way ANOVA.

* Bug #12034 fixed - max function did not manage empty matrices.

* Bug #12045 fixed - repmat returned wrong result if numbers of input matrix were not double.

* Bug #12070 fixed - Removing a module can now be done in on-line mode even if
                     it installed in off-line mode.

* Bug #12080 fixed - lsqrsolve always printed messages.

* Bug #12085 fixed - On Windows csvWrite wrote wrong EOL.

* Bug #12118 fixed - ode() could take complex externals.

* Bug #12143 fixed - "stop entity picker" (ged(11)) returned an error message.

* Bug #12150 fixed - datatipCreate failed with one single point.

* Bug #12163 fixed - unzoom did not work with a single input argument.

* Bug #12326 fixed - There was no way to set LaTeX font size in preview.

* Bug #12349 fixed - In SciNotes, when the view was splitted, removing a char made the other view jump.

* Bug #12376 fixed - Exec & edit buttons in examples in help were sometimes misplaced.

* Bug #12413 fixed - ATOMS packages could not be removed if ATOMS had never been on-line.

* Bug #12415 fixed - PATH environment variable grew when using call_scilab in a loop.

* Bug #12426 fixed - By using addErrorMessage, random string error could be displayed.

* Bug #12433 fixed - show_pixmap function has been removed in Scilab files.

* Bug #12443 fixed - Document behavior of mopen() in text file mode
                     on Windows.

* Bug #12463 fixed - Bad specification for rect=[x,y,w,h] argument in french help
                     page of xstringb.

* Bug #12470 fixed - Variable browser is not refreshed after loading an environment.

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

* Bug #12557 fixed - Valid function names were not specified in help pages of function and functions.

* Bug #12564 fixed - Javasci compile & run help page is not clear about needed packages.

* Bug #12581 fixed - isfield could not support mlist or tlist.

* Bug #12589 fixed - Call sequence for spzeros & spones were missing in the french help pages.

* Bug #12593 fixed - A wrong error message was returned when running genlib with an error in the sci file.

* Bug #12606 fixed - Overloads for grand were not standard.

* Bug #12613 fixed - gsort did not return correct results with %nan.

* Bug #12614 fixed - Helpbrowser was not launched in EDT.

* Bug #12615 fixed - Graphics seemed to be dead after a call to plot/bar/barh with
                     a bad LineSpec argument.

* Bug #12624 fixed - In case of errors in Scilab macros, make sure that
                     "make check-TESTS" fails correctly.

* Bug #12627 fixed - At restoration a window could be out of the screen.

* Bug #12629 fixed - Last example of csim help page defined a function called
                     input() (overwriting the Scilab one).

* Bug #12631 fixed - A "see also" link has been added from progressionbar to waitbar and vice versa.

* Bug #12634 fixed - ATOMS modules could not be installed from an archive file
                     already existing in SCI/contrib/archives in offline mode.

* Bug #12637 fixed - Some signal processing functions were not in the correct
                     section of the help pages.

* Bug #12639 fixed - justify([], position) returned an error instead of [].

* Bug #12641 fixed - graypolarplot() was slow and has been fully vectorized.

* Bug #12657 fixed - Computation of v1.^v2 is now done in place,
                     when v1 and v2 are real arrays, v1 >= 0 and v2 integer.

* Bug #12668 fixed - Undocking SciNotes led to an exception.

* Bug #12678 fixed - nthroot() now accepts vector/matrix as second argument.

* Bug #12679 fixed - Argument type check added in gcd and lcm.

* Bug #12686 fixed - Error returned by diff() fixed.

* Bug #12702 fixed - When no extra parameters were needed in the cost function, NDcost did not work.

* Bug #12703 fixed - In SciNotes, common shortcuts 'SHIFT DELETE', 'SHIFT INSERT' (and others) added.

* Bug #12705 fixed - Function members() added. It allows to find number of occurrences and linear indexes
                     for common values between two matrices of the same type.

* Bug #12706 fixed - A wrong size matrix as input argument of cross function was not detected.

* Bug #12708 fixed - Incorrect display in SciNotes preferences fixed (onmouseover styles).

* Bug #12712 fixed - In SciNotes, autosaving can now use %date to append the current date to the
                     backup filename.

* Bug #12715 fixed - Variable cross in pspect and cspect has been renamed because of conflicts with cross function.

* Bug #12736 fixed - In SciNotes, the Completion window appeared only in first tab.

* Bug #12747 fixed - legendre() now accepts the -1 and 1 values for the third argument.

* Bug #12758 fixed - Focus issue with plot3d() fixed.

* Bug #12761 fixed - The https:// protocol was not supported on ATOMS.

* Bug #12772 fixed - eigs() failed when trying to solve a sparse matrix eigen value problem.

* Bug #12774 fixed - Various typos fixed.

* Bug #12775 fixed - Some related functions were not listed in "See also" section of routh_t help page.

* Bug #12778 fixed - Insertion of an empty matrix in an integer matrix led to a wrong result.

* Bug #12779 fixed - savewave() had a miscoding in the internal function write_wavedat().

* Bug #12783 fixed - There were some inconsistent error messages in dsearch().

* Bug #12790 fixed - Links to zcos files in doc were broken.

* Bug #12791 fixed - Give more information in case of failure during the ATOMS autoload step.

* Bug #12794 fixed - calfrq.sci code did not follow Scilab standard.

* Bug #12795 fixed - Typo fixes in CACSD doc.

* Bug #12800 fixed - Typo fix in Polynomials doc.

* Bug #12808 fixed - Add missing </td> in doc generation (note, warning, ...).

* Bug #12813 fixed - flipdim() function extended to any type of input data.

* Bug #12814 fixed - Elementary_functions: improvement of pertrans() doc.

* Bug #12815 fixed - levin() redefined cov() as a variable.

* Bug #12816 fixed - Numbers pasted in editvar were not parsed according to locale.

* Bug #12823 fixed - In help generation (toolbox) links were not correctly handled.

* Bug #12827 fixed - Cacsd doc: noisgen() improved.

* Bug #12830 fixed - In SciNotes, it was not possible to replace from the caret position.

* Bug #12831 fixed - In SciNotes toolbar, there was no button to open code navigator.

* Bug #12833 fixed - In SciNotes, there was no autoscroll when searching a pattern.

* Bug #12836 fixed - There was an error in strcmpi documentation.

* Bug #12839 fixed - Typo fixes in Core doc: getVariablesOnStack().

* Bug #12840 fixed - Fix typo in number_properties.xml.

* Bug #12852 fixed - Visual Studio 2012 SDK configuration is incorrect.

* Bug #12858 fixed - Typo in French doc for debug() and pause().

* Bug #12859 fixed - Core doc: fix optional argument of isdef().
