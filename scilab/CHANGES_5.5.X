                    Changes between version 5.4.X and 5.5.0
                    =======================================

New Features
=============

* New special functions:
 - erfi - The imaginary error function.
 - dawson - Compute the Dawson (scaled imaginary error) function.

* New functions introduced:
 - getURL - Download an URL (HTTP, HTTPS, FTP...)
 - splitURL - Split a URL (HTTP, HTTPS, FTP...)
 - cov - Covariance matrix. Deprecates mvvacov. See bug #11896 fixed.
 - ismatrix - Check if a variable is a matrix. See bug #10456 fixed.
 - isrow - Check if a variable is a row vector. See bug #10456 fixed.
 - iscolumn - Check if a variable is a column vector. See bug #10456 fixed.
 - issquare - Check if a variable is a square matrix. See bug #10456 fixed.
 - cross - Vector cross product. See bug #9941 fixed.

* Add a complete set of functions to read and write any HDF5 file from Scilab.

* New Solver:
 - daskr - differential-algebraic system solver with rootfinding 'daskr', using
BDF methods with direct and preconditioned Krylov linear solvers, based on ODEPACK.

* Based on JIMS external module, Scilab provides functions to interact
  with Java objects.

* erf, erfc, erfcx and calerf functions now support complex arguments.

* isnum has been redesigned in native code. Performance improvements Up to 130x.
  See bug #10404

* Allow the usage of the '$' keyword in part function.

* The histplot command can now be used with the option polygon=%t/%f to add the
  frequency polygon chart (thanks to Mehran Khorshidi).

* Multi level completion on mlist, struct, XML structures...

* Variable browser improvements:
 - The variable browser also shows the size of integers and the user type of
  the tlist/mlist.
  See bugs #12523 and #10409
 - It is now possible to delete variables from the variable browser.
  See bug #9447
 - A user can now plot variables from the variable browser (already available in
  editvar).

* Windows Solution updated to Visual Studio 2012.

* -keepconsole option added for Scilab Windows to facilitate debugging.
  Calling Scilab with this optin will leave the console box window opened at startup.

* License update: switch to the CeCILL 2.1.


Syntax Changes
===============

* Vector ^ scalar syntax declared obsolete. See bug #11524


Improvements
=============

* New calling sequence allowed for nicholschart: nicholschart(gains, phases, colors).
  See bug #7828.

* qp_solve can now take up to 5 output arguments. The last one is an error flag,
  if it is present, then the function will display a warning instead of an error.
  See bug #10269.

* graypolarplot has been improved in terms of performances and rendering.
  See bug #12641.

* nthroot is now vectorizable.
  See bug #12678.


Obsolete
=========

* Scilab 5.5.X family will be the latest family working under Windows XP/2003.

* The third argument of poly function will be more strict.
    Only the following strings are allowed: "roots", "coeff", "c", "r".

* Option and direction arguments of gsort will be more strict in all cases.
  Only the following strings are allowed: option: "r" "c" "g" "lr" "lc"
                                              direction : "d" "i"

* nfreq tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use tabul instead.

* IsAScalar tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use isscalar instead.

* chart tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use nicholschart instead.

* dft removed, please use fft instead.

* sscanf removed, please use msscanf instead.

* fscanf removed, please use mfscanf for files opened with mopen or read for files opened with file instead.

* fprintf removed, please use mfprintf for files opened with mopen or write for files opened with file instead.


Differential Equations
======================

* Netlib's Quadpack, used for definite integration, has been updated to match the upstream.


SciNotes
========

* Autosave feature can now use %date to append the current date to the backup filename (See bug #12712).


Xcos
====

* New DAE solver: DDaskr, using BDF methods with direct Newton and preconditioned Krylov linear solvers, which includes rootfinding.
  It is available from the dae() function.

* In Modelica initialization GUI, inputs (eg. sensor) were not handled.

* Sundials updated to the "2.5.0" version, keeping our modifications.

* JGraphX updated to the "2.1.0.7" version, updating our performance hooks.

* API changed in the scicos_block4 interface: the uid value is not available
  inside the simulation function.

* Bug #7350 fixed - The I/O ports numbering of a superblock was not updated
                    when a new port was dropped.

* Bug #8570 fixed - "Region to superblock" has been renamed to
                    "Selection to superblock".

* Bug #11518 fixed - CLR block-text was displayed out of the bounding box
                     when zooming.

* Bug #12359 fixed - Xcos files have been converted to ZCOS to gain some space.

* Bug #12384 fixed - Using a Modelica part linked with an explicit link to
                     another Modelica part led to an algebraic loop error.

* Bug #12387 fixed - The "Modelica initialize" setup menu option did not blur
                     during simulation.

* Bug #12414 fixed - SWITCH_m block had different behaviors
                     for different types of inputs.

* Bug #12424 fixed - Calling lincos on a derivative block made Scilab crash.

* Bug #12460 fixed - xcosPalGenerateAllIcons sometimes crashed Scilab.

* Bug #12461 fixed - Cancelling zoom out by zooming in did not fully work.

* Bug #12561 fixed - SELECT_m and RELAY_f did not behave as expected.

* Bug #12568 fixed - "Recent files" menu entry is now below the "Open" one.

* Bug #12590 fixed - Block shape style was removed on file loading.

* Bug #12603 fixed - ZCOS files could contain blocks with modified (eg. invalid)
                     parameters.

* Bug #12619 fixed - DLR discrete block did not display LaTeX formula
                     like CLR continuous block.

* Bug #12651 fixed - The 'nw' scicos_simulate option did not work while
                     passing a previous Info simulation status.

* Bug #12664 fixed - Inverted pendulum French localization in the Xcos demonstrations was inaccurate.

* Bug #12667 fixed - 'Recent Files' menu was not localized.

* Bug #12731 fixed - Code generation produced erroneous block.

* Bug #12732 fixed - Improper Copyright comments in the files generated by the code generation tool.

* Bug #12797 fixed - I/O blocks generated by "Selection to Superblocks" had a
                     wrong size.

* Bug #12868 fixed - There were several problems with PULSE_SC block.

* Bug #12873 fixed - Scicos_flat produced an unexpected error after a diagram
                     load.

* Bug #12874 fixed - CSCOPE marks sizes were too small.

* Bug #12877 fixed - Incorrect output port dimensions and types in GENSIN_f, GENSQR_f, STEP_FUNCTION and STEP blocks fixed.


Toolbox Skeleton
=================

* Some Xcos specific content added to xcos_toolbox_skeleton help pages.


Compilation
============

* Minimal version of Flexdock (1.2.4) enforced.


Bug Fixes
==========

* paramfplot2d: When theta input argument was a column vector, an error occurred.

* Bug #2267 fixed - Wrong error message when ticks locations and labels did not have the same sizes.

* Bug #2416 fixed - Particular case (string([]) returns []) has been added in string help page.

* Bug #3511 fixed - strindex did not return all occurrences in regexp mode.

* Bug #3928 fixed - An error was returned when a matrix was flipped along the third dimension.

* Bug #4042 fixed - squeeze returned a hypermatrix instead of a matrix when at least one
                    dimension was equal to 1.

* Bug #4085 fixed - num2cell help page added.

* Bug #4229 fixed - delip did not return an error if one element of its first input argument
                    was negative.

* Bug #4383 fixed - csim with "step" and "impuls" now works with direct feedthrough.

* Bug #4481 fixed - iir help page updated: frq can be a scalar.

* Bug #4731 fixed - lqr failed when the time domain of an input was a number.

* Bug #5073 fixed - New parameter added in strtod function (decimal separator).

* Bug #5207 fixed - grand can now return a hypermatrix.

* Bug #5365 fixed - makecell help page was in the "compatibility functions" directory
                    instead of being in "data_structures".

* Bug #5539 fixed - sylv help page was wrong in discrete-time cases.

* Bug #5616 fixed - typeof(uiCreateTree(...)) returned "Tree" instead of "uitree".

* Bug #5694 fixed - numdiff help page clarified.

* Bug #5779 fixed - xnumb number format was too small (+ unit test added).

* Bug #6037 fixed - macrovar help page improved.

* Bug #6168 fixed - zpbutt, zpch1, zpch2 and zpell help pages were unclear.

* Bug #6390 fixed - The "external" argument of odecd was not well documented and not tested.

* Bug #6427 fixed - full([%T %F]) returned an error message.

* Bug #6466 fixed - Example with vectorized input added in mprintf and msprintf help pages.

* Bug #6693 fixed - modulo did not accept polynomial inputs. Help page was not updated.

* Bug #6752 fixed - unit test scilab.tst has been split in different tests.

* Bug #6988 fixed - Error messages in modules/data_structures/src/c/hmops.c were not standard.

* Bug #7026 fixed - There was no unit test for plot2d.

* Bug #7080 fixed - Some graphic macros did not use standard error messages.

* Bug #7204 fixed - geomean applied to a hypermatrix gave wrong results.

* Bug #7206 fixed - If the second input argument of meanf function was a hypermatrix, this function
                    returned an error.

* Bug #7296 fixed - %nan, %inf and -%inf enabled for the cdf* functions.

* Bug #7411 fixed - clf forgot to turn off the datatip mode.

* Bug #7486 fixed - LAPACK versions of DGELSY and ZGELSY now used.

* Bug #7596 fixed - A same error happening in different places now displays the same error message.

* Bug #7648 fixed - CDF functions now display a warning for non integer "degrees of freedom" argument.

* Bug #7655 fixed - An example added in type help page, for type(X)=11 and type(X)=13.

* Bug #7684 fixed - Introduction demonstration splitted in subsections.

* Bug #7781 fixed - The second parameter of iqr function had not effect.

* Bug #7782 fixed - lcm and gcd help pages improved to tell the user how to use both functions.

* Bug #7824 fixed - title function properties did not support an indexed color.

* Bug #7826 fixed - chart tagged as obsolete.

* Bug #7828 fixed - Slight improvements in nicholschart.

* Bug #7848 fixed - The third argument of correl function is now optional.

* Bug #7877 fixed - iirgroup function fixed.

* Bug #7927 fixed - Output "flag" in qmr function was not well documented.

* Bug #7960 fixed - plzr could not produce pole zero plot for a simple transfer function.

* Bug #8098 fixed - cumsum could not be applied to rational matrices.

* Bug #8162 fixed - Area of stability of plzr was wrong for continuous systems (+unit test added).

* Bug #8211 fixed - 'parameters' module demonstration finalized.

* Bug #8234 fixed - strtod did not return an empty matrix when the input argument was an empty matrix.

* Bug #8264 fixed - Matlab to Scilab dictionary help page updated for atan2.

* Bug #8319 fixed - dbphi(hypermat) and phasemag(hypermat) returned a matrix instead of a hypermatrix.

* Bug #8373 fixed - clear can now handle a matrix of strings argument.

* Bug #8415 fixed - optim_moga, optim_nsga, optim_nsga2 can now take list as input arguments,
                    as explained in their help page.

* Bug #8462 fixed - bvodeS could make Scilab unstable.

* Bug #8470 fixed - bvode displayed some output in terminal window and not in Scilab console.

* Bug #8479 fixed - The latest Saxon version was not supported.

* Bug #8511 fixed - sprand now uses grand instead of rand and grand functions. Internal state of the random
                    generator is no more changed.

* Bug #8561 fixed - ddassl, ddasrt, ddaskr: abs and rel tolerance sizes are now checked.

* Bug #8597 fixed - grand/clcg4 could display uncontrolled messages as warning.

* Bug #8607 fixed - Some error messages in modules/overloading/macros were not standard and not localized.

* Bug #8614 fixed - Unit test for barhomogenize added.

* Bug #8636 fixed - roots help page updated (default algorithm value was wrong).

* Bug #8667 fixed - Handling of %nan in functions min, max, median was not properly documented.

* Bug #8680 fixed - "end" output argument of regexp function has been changed.

* Bug #8687 fixed - typeof function failed on uint8, depending on the format (+ unit test added).

* Bug #8695 fixed - optim_ga used old (initial) values instead of newly-computed ones.

* Bug #8778 fixed - Call_ScilabOpen, TerminateScilab could not be called more
                    than 80 times in a loop.

* Bug #8779 fixed - gsort did not preserve the order of equal elements, in lexicographic sort.

* Bug #8820 fixed - Squeeze did not return a matrix when the number of dimensions
                    of the result was less or equal to 2.

* Bug #8824 fixed - taucs_chfact returned a segfault (not the case in nwni mode).

* Bug #8840 fixed - fileparts did not manage matrix of strings.

* Bug #8856 fixed - Non regression test added for [k,l,m,...]=find(a==5).
                    Non regression test of bug #476 updated.

* Bug #8857 fixed - Non regression test of bug #477 updated.

* Bug #8858 fixed - Non regression test of bug #480 updated.

* Bug #8862 fixed - mget and mput could not read and write 64 bit data from
                    binary files.

* Bug #9004 fixed - bitcmp function called with one input argument returned an error.

* Bug #9020 fixed - exists function did not accept matrix as first input argument.

* Bug #9059 fixed - tbx_build_macros and genlib did not stop even if an error occurred.

* Bug #9109 fixed - nfreq tagged as obsolete.

* Bug #9208 fixed - Added three optional output arguments to optim(),
                    to retrieve #iterations, #evaluations and a termination indicator.

* Bug #9385 fixed - The type checking in trigonometric functions has been added.

* Bug #9394 fixed - is_params recognized "plist" as an existing field.

* Bug #9395 fixed - add_params did not check its input arguments.

* Bug #9396 fixed - add_params accepted duplicate keys.

* Bug #9459 fixed - Default values of the optional plot3d arguments were not documented.

* Bug #9537 fixed - optimbase_configure only allowed row vectors as initial value.
                    Column vectors now allowed by transposing them.

* Bug #9538 fixed - optimbase_checkshape only allowed row vectors as output arguments of cost function.
                    Column vectors are now allowed by transposing them.

* Bug #9577 fixed - Setting neldermead_configure("-numberofvariables") is now optional,
                    setting neldermead_configure("-x0") initializes -numberofvariables implicitly.

* Bug #9601 fixed - Cylinder demonstration fixed.

* Bug #9691 fixed - "imp" option in optim help page was poorly documented.

* Bug #9694 fixed - Example in optim help page fixed to display correct counters.

* Bug #9780 fixed - gmres solver did not run with complex systems.

* Bug #9821 fixed - getrelativefilename did not manage matrix of strings.

* Bug #9851 fixed - Error message occurred because of a cut-off frequency of 0.25Hz with irr.

* Bug #10146 fixed - In SciNotes, 'help on keyword' moved from bottom to top in the popup menu.

* Bug #10180 fixed - det was not defined for sparse matrices.

* Bug #10213 fixed - sci2exp help page updated to document the impact of format function.

* Bug #10216 fixed - Invalid syntaxes for zeros, ones, eye, rand, like zeros(2,:).

* Bug #10226 fixed - When a // <empty session> line was deleted, all sessions
                     histories were folded.

* Bug #10254 fixed - Slight improvements in ones help page.

* Bug #10269 fixed - qp_solve can now take up to 5 output arguments. The last one is an error flag,
                     if it is present, then the function will issue a warning instead of an error.

* Bug #10276 fixed - qp_solve segfaulted with large matrices.

* Bug #10287 fixed - Error message added for complex expression as input argument of integrate function.

* Bug #10305 fixed - Comparison of lists with empty items returned an error message.

* Bug #10596 fixed - exit(xxx) from Scilab failed.

* Bug #10818 fixed - hom, %e, %t, %f, %z and %s help pages added.

* Bug #10823 fixed - fullpath returned different results under Windows and Linux for non-existent file.

* Bug #10862 fixed - Add a without Internet connection installation
                     global configuration in the installer.

* Bug #10866 fixed - det was not equivalent to detr for rational matrices.

* Bug #10906 fixed - Typo fixed cls2dls help page.

* Bug #10930 fixed - The comments in armax function were in French.

* Bug #10995 fixed - Typo fixed in grand help page for Gamma law argument.

* Bug #11139 fixed - conj was not defined for sparse matrices (+ unit test added).

* Bug #11308 fixed - Calling sequences in dsearch help page were wrong.

* Bug #11343 fixed - The "isoview" figure property did not work when the axes
                     margins had been modified.

* Bug #11648 fixed - Copy graphic to clipboard did not work.

* Bug #11766 fixed - nthroot has been added to the m2sci help page.

* Bug #11792 fixed - Lists can be accessed with non integer indexes (help page updated).

* Bug #11869 fixed - "Environment" was not localized in preferences.

* Bug #11885 fixed - Each rand has been changed to grand in genetic algorithms and simulated annealing functions.

* Bug #11891 fixed - Fisher ratio could be inaccurate for one-way ANOVA.

* Bug #11997 fixed - In case of invalid variable name, save function now returns an error instead of a compatibility warning.

* Bug #12034 fixed - max function did not manage empty matrices.

* Bug #12045 fixed - repmat returned wrong results if the values of input matrix were not double.

* Bug #12070 fixed - Removing a module can now be done in an on-line mode even if
                     it has been installed in an off-line mode.

* Bug #12080 fixed - lsqrsolve always printed messages.

* Bug #12085 fixed - Under Windows csvWrite wrote wrong EOL.

* Bug #12114 fixed - libstdc++ is now compiled as static instead of using the
                     library from thirdparty.

* Bug #12118 fixed - ode could take complex externals.

* Bug #12143 fixed - "stop entity picker" (ged(11)) returned an error message.

* Bug #12150 fixed - datatipCreate failed with one single point.

* Bug #12163 fixed - unzoom did not work with a single input argument.

* Bug #12326 fixed - There was no way to set LaTeX font size in preview.

* Bug #12349 fixed - In SciNotes, when the view was splitted, removing a char made the other view jump.

* Bug #12376 fixed - Exec & edit buttons in the help pages examples were sometimes misplaced.

* Bug #12413 fixed - ATOMS packages could not be removed if ATOMS had never been on-line.

* Bug #12415 fixed - PATH environment variable grew when using call_scilab in a loop.

* Bug #12426 fixed - By using addErrorMessage, random string error could be displayed.

* Bug #12433 fixed - show_pixmap function was removed from Scilab but still used in some functions.

* Bug #12443 fixed - The behavior of mopen in text file mode has been documented
                     under Windows.

* Bug #12463 fixed - Wrong specification for rect=[x,y,w,h] argument in xstringb French help
                     page.

* Bug #12470 fixed - Variable browser was not refreshed after loading an environment.

* Bug #12473 fixed - Problems with "é" in mkdir French help page.

* Bug #12475 fixed - csvRead did not support double quoted fields.

* Bug #12490 fixed - plot did not have a clf; in its demonstration.

* Bug #12506 fixed - In SciNotes, a miscolorization could occurred when returned values list was broken.

* Bug #12508 fixed - Wrong error message in rand function fixed.

* Bug #12518 fixed - Polynoms were not displayed by default in the variable browser.

* Bug #12520 fixed - Improve the description of the size in the variable browser.

* Bug #12527 fixed - Scilab user functions were not listed in the variable browser.

* Bug #12547 fixed - In SciNotes, lines were wrongly numbered with splitted function declarations.

* Bug #12548 fixed - Duplicated code in xmltoformat removed.

* Bug #12550 fixed - Optimbase and optimsimplex help pages were not standard.

* Bug #12551 fixed - Stack problem with diary([],"pause") and diary([],"resume") fixed.

* Bug #12556 fixed - A fatal error occurred when calling set with wrong instructions.

* Bug #12557 fixed - Valid function names were not specified in function and functions help pages.

* Bug #12564 fixed - Javasci compile & run help page was not clear about needed packages.

* Bug #12581 fixed - isfield could not support mlist or tlist.

* Bug #12589 fixed - Call sequence for spzeros & spones were missing in the French help pages.

* Bug #12593 fixed - A wrong error message was returned when running genlib with an error in the sci file.

* Bug #12600 fixed - mput did not manage unsigned integer.

* Bug #12606 fixed - Overloads for grand were not standard.

* Bug #12613 fixed - gsort did not return correct results with %nan.

* Bug #12614 fixed - Helpbrowser was not launched in EDT.

* Bug #12615 fixed - Graphics seemed to be freezed after a call to plot/bar/barh with
                     a bad LineSpec argument.

* Bug #12624 fixed - In case of errors in Scilab macros, make sure that
                     "make check-TESTS" now fails correctly.

* Bug #12627 fixed - At restoration, a window could be out of the screen.

* Bug #12629 fixed - Last example of csim help page defined a function called
                     input (overwriting the Scilab one).

* Bug #12631 fixed - A "see also" link has been added from progressionbar to waitbar and vice versa.

* Bug #12634 fixed - ATOMS modules could not be installed from an archive file
                     already existing in SCI/contrib/archives in offline mode.

* Bug #12637 fixed - In some help pages, some signal processing functions were not in the correct
                     section.

* Bug #12639 fixed - justify([], position) returned an error instead of [].

* Bug #12641 fixed - graypolarplot has been fully vectorized as it was too slow.

* Bug #12657 fixed - Computation of v1.^v2 is now done without any memory allocation,
                     when v1 and v2 are real arrays, v1 >= 0 and v2 integer.

* Bug #12668 fixed - Undocking SciNotes led to an exception.

* Bug #12678 fixed - nthroot now accepts vector/matrix as second argument.

* Bug #12679 fixed - Argument type check added in gcd and lcm.

* Bug #12686 fixed - Error returned by diff fixed.

* Bug #12702 fixed - When no extra parameters were needed in the cost function, NDcost did not work.

* Bug #12703 fixed - In SciNotes, common shortcuts 'SHIFT DELETE', 'SHIFT INSERT' (and others) added.

* Bug #12705 fixed - members function added. It allows to find the number of occurrences and linear indexes
                     for common values between two matrices of the same type.

* Bug #12706 fixed - A wrong size of a matrix as input argument of cross function was not detected.

* Bug #12708 fixed - Incorrect display in SciNotes preferences fixed (onmouseover styles).

* Bug #12712 fixed - In SciNotes, autosaving can now use %date to append the current date to the
                     backup filename.

* Bug #12715 fixed - Variable cross in pspect and cspect has been renamed because of conflicts with cross function.

* Bug #12736 fixed - In SciNotes, the Completion window appeared only in first tab.

* Bug #12747 fixed - legendre now accepts the -1 and 1 values for third argument.

* Bug #12749 fixed - fscanfMat help page updated.

* Bug #12758 fixed - Focus issue with plot3d fixed.

* Bug #12761 fixed - The https:// protocol was not supported on ATOMS.

* Bug #12772 fixed - eigs failed when trying to solve a sparse matrix eigen value problem.

* Bug #12774 fixed - Various typos fixed.

* Bug #12775 fixed - Some related functions were not listed in "See also" section of routh_t help page.

* Bug #12778 fixed - Insertion of an empty matrix in an integer matrix led to a wrong result.

* Bug #12779 fixed - savewave had a miscoding in the internal function write_wavedat.

* Bug #12783 fixed - There were some inconsistent error messages in dsearch.

* Bug #12790 fixed - Links to ZCOS files in documentation were broken.

* Bug #12791 fixed - Give more information in case of failure during the ATOMS autoload step.

* Bug #12794 fixed - calfrq.sci code did not follow Scilab standard.

* Bug #12795 fixed - Typos fixed in CACSD help page.

* Bug #12800 fixed - Typo fixed in Polynomials help page.

* Bug #12807 fixed - Display of showprofile improved.

* Bug #12808 fixed - Add missing </td> in documentation generation (note, warning, ...).

* Bug #12813 fixed - flipdim function extended to any type of input data.

* Bug #12814 fixed - Improvements of pertrans help page.

* Bug #12815 fixed - levin redefined cov as a variable.

* Bug #12816 fixed - Numbers pasted in editvar were not parsed according to locale.

* Bug #12818 fixed - Segfault in set function with invalid property values dimension.

* Bug #12823 fixed - In help generation (toolbox) links were not correctly handled.

* Bug #12827 fixed - noisgen help page improved.

* Bug #12830 fixed - In SciNotes, it was not possible to execute a replace action from the caret position.

* Bug #12831 fixed - In SciNotes toolbar, there was no button to open code navigator.

* Bug #12833 fixed - In SciNotes, there was no autoscroll when searching a pattern.

* Bug #12836 fixed - Error fixed in strcmpi help page.

* Bug #12839 fixed - Typo fixed in getVariablesOnStack help page.

* Bug #12840 fixed - Typo fixed in number_properties.xml help page.

* Bug #12852 fixed - Visual Studio 2012 SDK configuration was incorrect.

* Bug #12858 fixed - Typo fixed in debug and pause French help pages.

* Bug #12859 fixed - Fixed optional argument in isdef help page.

* Bug #12863 fixed - size(state-space, "r") returned an error.

* Bug #12875 fixed - phasemag returned an error for input vector containing zeros.

* Bug #12888 fixed - sysdiag was not documented about block diagonal matrices build.

* Bug #12906 fixed - champ and champ1 help pages updated

* Bug #12909 fixed - Completion on (mt)list led to a crash.

* Bug #12911 fixed - Matlab to Scilab dictionary help page updated for eig.

* Bug #12916 fixed - power help page improved.
