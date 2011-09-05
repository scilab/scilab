            Changes between version 5.3.3 and 5.4.0 of Scilab
            =================================================


SciNotes:
=========

* Add marks in the vertical scrollbar to see the positions of matching words
  when an incremental search is done (via F3 or in selecting text).

* In the find/grep tools, add the possibility to use ~ as a joker for user home
  dir. and SCI for Scilab data base dir.

* Add an 'Save and Execute all files' action.

* A tab can be closed in using the middle mouse button.

* bug 9334 fixed - Extra lines were added when code was exported into PDF under
                   Windows.

* Bug 9358 fixed - A simple quoted string was not always correctly convert
                   into double quoted one.

* bug 9476 fixed - In find/grep tools, use the abbreviation 'sec' instead of
                   'seconds'.

* bug 9480 fixed - Highlighted words after selection were wrong under Windows.

* Bug 9484 fixed - An empty file was not removed when an existing file was
                   opened.

* Bug 9488 fixed - There was no popup menu on the tab to copy the file path.

* bug 9505 fixed - A LaTeX string in a comment was not considered as a comment.

* bug 9597 fixed - It was not possible to navigate across the tabs with
                   <CTRL>+(PAGE_UP | PAGE_DOWN).

* bug 9740 fixed - There was no way to have \n in replacement string.

* bug 9897 fixed - With Mac OS, eol was not replaced by \n when code was copyed
                   from Word to SciNotes.

* bug 9878 fixed - Add a shortcut "Correct indentation" in right click menu.


Graphical User Interface:
=========================

* The variable browser now refresh itself automatically (See bug 9446).


Xcos:
=====

* xcos_skeleton will not be loaded if you have not installed Xcos.

* Error reporting has been improved to print warnings on blocks into Xcos.

* bug 8421 fixed - CLKFROM and CLKGOTO were not part of the "Signal Routing"
                   palette.

* bug 8538 fixed - Counter block label was not reflecting parameters

* bug 9301 fixed - Exception when one tried to paste a column to replace a
                   column selection.

* bug 9336 fixed - SuperBlock with non connected ports produced an invalid
                   index error.

* bug 9390 fixed - FROMWSB_c block does not reset to the default values on
                   ReInitialization execution.

* bug 9478 fixed - Block structure was not checked while adding them to a new
                   palette.

* Bug 9501 fixed - With a localized TMPDIR, block edition could throw an HDF5 
                   exception.

* bug 9506 fixed - some libraries dependencies were not loaded for xcos_skeleton startup.

* bug 9532 fixed - Users defined blocks had no icons when saved

* bug 9547 fixed - The about box was not localized due to a multi-line string.

* bug 9590 fixed - xcos examples were not embeded in binary version on Windows.

* bug 9810 fixed - MATINV wrong simulation values errors has been improved.



Localization:
=============

* Czech localization by Zbyněk Schwarz


File input/output:
==================

* mgetl, exec, fscanfMat functions manage files encoded as UTF-8 with BOM.

* tempname function added (SEP 68).


New functions
=============

* The assert module provides a set of assertion functions.
  The goal of this module is to provide functions to check the
  behavior of some other functions, for example in unit tests.
  We emphasize the use of consistent tools for testing numerical
  issues, with the goal of testing numerical algorithms more easily.
  In particular, we provide a comparison function for two floating
  point numbers, which allows to check that two numbers are
  "numerically almost equal", i.e. that the relative error is small.
  This automatically fixes the bug #4381.

* The damp function in CACSD modules computes the natural pulsation
  and damping factor of linear dynamical systems.

Optimization:
=============

* neldermead: Added a demo - search for largest smallest polygon.

* bug 7891 fixed - The neldermead component had too many options.
  Tagged -costfargument option of optimbase as obsolete: will be
  maintained for backward compatibility until 5.4.1.
  The -fun option can now be a list, where the element #1 is a
  function, and the elements #2 to the end are automatically appended to
  the calling sequence.
  Tagged -outputcommandarg option of optimbase as obsolete: will be
  maintained for backward compatibility until 5.4.1.
  The -outputcommand option can now be a list, where the element #1 is
  a function, and the elements #2 to the end are automatically appended
  to the calling sequence.
  Tagged "outputfun(x,optimValues,state)" calling sequence of fminsearch
  as obsolete: will be maintained for backward compatibility until
  5.4.1.
  The new calling sequence is "stop=outputfun(x,optimValues,state)"
  Tagged "myoutputfun(state,data)" calling sequence of neldermead
  as obsolete: will be maintained for backward compatibility until
  5.4.1.
  The new calling sequence is "stop=myoutputfun(state,data)"
  Tagged "-myterminateflag" and "-myterminate" options as obsolete:
  will be maintained for backward compatibility until 5.4.1.
  See the help page of neldermead for details to update your
  code, if required.

* readmps - Created unit test.
            Fixed bug in the management of error messages.
            Fixed bug in the reading of a test file.
            Defined a printing system by overloading for MPS structure.
            Updated the help page.

* Bug 7093 fixed - The data structure generated by neldermead_new could not
                   be saved and reloaded.

* Bug 7889 fixed - The tolerance on x for neldermead was too small.
                   Updated -tolxrelative default value from %eps to sqrt(%eps).

* Bug 8243 fixed - xgeom renamed to geom3d.

* Bug 8805 fixed - The 'neldermead' function, algorithm box, wrongly used
                   the output function.

* Bug 9567 fixed - The neldermead Overview had a TODO section.

* Bug 8542 fixed - In neldermead, there were undocumented default options.
                   Same changes applied to the help of optimbase.

* Bug 8543 fixed - In neldermead, the help page presents a wrong order
                   for the options.

* Bug 8545 fixed - In neldermead, the printing system displays too long
                   messages.

* Bug 8546 fixed - The neldermead functions may call f outside the bounds.

* Bug 8547 fixed - The neldermead function called f outside the constraints.

* Bug 8552 fixed - Improved fminsearch to provide Extra Parameters.
                   Help page updated with new section "The cost function".
                   Help page updated with new example "Passing extra parameters".

* Bug 9323 fixed - The messages in optimbase_checkbounds were not localized.
                   Same changes applied to optimbase_checkx0.

* Bug 9576 fixed - The neldermead component did not check the bounds
                   of some input parameters.

* Bug 9578 fixed - The optimbase_configure function does not check the
                   values of input arguments.

* Bug 9607 fixed - The optimsimplex_new function poorly managed the
                   variable number of arguments.


Documentation:
==============

* A new system to keep track of the translations of the main documentation
  has been developed. See HTTP:// @TODO@
  See bug 5119.

* <ALT>+<LEFT ARROW> / <ALT>+<RIGHT ARROW> allows the browse of the history.
  (see bug 9474)

* The space key behave like the page down key. (see bug 9491)

* Management of the revhistory / revision tag to provide information about
  the history of the function.

* Bug 3431 fixed - some helptools functions were obsoletes and not removed.

* Bug 9299 fixed - Fix a typo in the cspect, pspect and window help pages
                   (Hanning changed to Hann)

* Bug 9417 fixed - In English, api_scilab polynom help pages updated to the word
                   polynomial

* Bug 7134 fixed - help_from_sci produced an invalid .xml file.

* Bug 7133 fixed - help_from_sci function did not return the function template.

* Bug 9790 fixed - windows_tools help were not available on all platforms.

Removed functions (previously declared as obsolete):
====================================================

* ilib_for_link, ilib_build do not accept files with ".o" or ".obj" extensions
  for source files.

* 'with_atlas' removed. Please use 'getdebuginfo' instead.

* C API function Error(x) has been removed. Please use SciError.

* 'MSDOS' removed. Please use 'getos' instead.


Performance:
============

* SSE is now enabled by default under GNU/Linux with 32 bit CPU (it is already
  the case under 64 bit CPU).
  Pentium III class (or equivalent) with SSE instructions is required.

* bug 8608 fixed - dec2hex, hex2dec, dec2oct, oct2dec, code have been simplified
                   and optimized.

* Performances of function pareto_filter (Genetic algorithms) improved.


Compilation:
============

* Under Windows, Scilab uses VS 2010 Pro and Express solution.

* Under GNU/Linux and Mac OS X, all modules build as standalone its algorithms
  as a static library (only available in the source tree).
  This will avoid an important dependency tree for unitary testing.

* Force the Java class version target to version 6 (or 1.6).

* Add Java 7 in the Debian & Ubuntu search path under GNU/Linux.

* bug 9500 fixed - Explicit the GUI module manifest class-path to make Debian
                   checking tool happy.

* bug 9782 fixed - configure with --without-gui was failing.


Graphics:
=========


* bug 9330 fixed - delete() called overloading instead of deleting the current
                   graphic entity.

* bug 9455 fixed - The contourf and contourf 2 demos displayed unwanted dots.


ATOMS:
======

* bug 8664 fixed - Message error updated for non supported OS.

* bug 9127 fixed - "Home" item renamed to "Installed Modules".

* bug 9805 fixed - atomsSystemUpdate() failed on some Windows.


* bug 8763 fixed - ATOMS installation path was shared on Windows 64 with 32 bits.


Javasci:
========

* Java mapping of Scilab data has now a new method called getType() which
returns the value of the ScilabTypeEnum enum.

* bug 9467 fixed - unnecessary declarations in MAKE.BAT of javasci examples.

* bug 9544 fixed - Calling graphic with Javasci from a SWING application from
                   EDT was crashing the execution of a java program.


* bug 9679 fixed - atomsSystemUpdate checks that Scilab is not in offLine mode, else writes a message.

Misc:
=====

* Change the buffering display mode.

* SEP 67: lines(0) by default at startup.

* Options -disable-exception-catching added under GNU/Linux & Mac OS X.
  Disables the Scilab exception catching system.

* PVM module moved as ATOMS module.
  atomsInstall('PVM')


Bug fixes:
==========

* bug 1604 fixed - list extraction failed.

* bug 3291 fixed - SCI/modules/tclsci/tcl/BWidget-1.8.0/ moved as prerequirements.

* bug 4576 fixed - An example of ilib_build with "cflags" argument was missing.

* bug 4833 fixed - msprintf() interpreted "\n" as a carriage return even if "\n"
                   is supplied by the 2nd input argument.

* bug 4847 fixed - In uigetfile, there was no way to select files with a given
                   pattern.

* bug 6875 fixed - test_run() could not run at once on the +3000 tests of
                   Scilab.

* bug 7061 fixed - changelog.txt, readme.txt, version.xml removed from modules.

* bug 7358 fixed - Help pages of type and typeof have now links to the actual
                   description pages.

* bug 7895 fixed - fft2() was broken on Windows with MKL wrapper.

* bug 7901 fixed - besselh.tst failed on Windows.

* bug 8245 fixed - gsort() did not allow overloading with not managed types.

* bug 8608 fixed - dec2hex, hex2dec, dec2oct and oct2dec code have been simplified
                   and optimized.

* bug 8684 fixed - 'wfir' function produced an error message when used interactively.

* bug 8688 fixed - fillMatrixOfString did not check available memory on stack.

* bug 8768 fixed - GetRhsVar(..., STRING_DATATYPE, ...) did not check size of input argument.

* bug 8963 fixed - mtlb_mode test failed on Windows.

* bug 9045 fixed - plot2d did not support overloading.

* bug 9098 fixed - demo CACSD --> Robust control failed on Windows 64 bit.

* bug 9056 fixed - An uicontrol could not be closed with <META>+W.

* bug 9104 fixed - 'warning' used in a diary scilab session added timestamp
                   twice in the diary.

* bug 9129 fixed - Completion on field did not propose graphics handle fields
                   by default.

* bug 9156 fixed - mgetl.c compilation failed on some Windows.

* bug 9164 fixed - The unit tests now use the new assert functions.

* bug 9214 fixed - abaque replaced by nicholschart in the black.sci comments

* bug 9253 fixed - fscanfMat failed to read a file with some lines
                   of text at the end (backward compatibility).

* bug 9266 fixed - fftw crashed on Windows when using MKL wrapper.

* bug 9268 fixed - Scilab was overridding a predefined variable JAVA_HOME.

* bug 9277 fixed - escape character "\" did not work with msprintf.

* bug 9354 fixed - 'savematfile' could not save imbricated structures.

* bug 9432 fixed - link a 64 bit dynamic library on a 32 bit platform returned
                   a wrong error message on Windows.

* bug 9452 fixed - It was impossible to download Intel MKL throw a proxy.

* bug 9479 fixed - Typo fixed (intput => input).

* bug 9498 fixed - A long history caused Scilab startup to be very long.

* bug 9510 fixed - export_to_hdf5 did not return %f when trying to export
                   unmanaged type (function, macro, ...)

* bug 9540 fixed - MKL Pentium 4 target was not downloaded by installer on Windows.

* bug 9542 fixed - During export operation the file could be locked by an other
                   process (Windows bug).

* bug 9543 fixed - Scilab did not use the default DocumentBuilderFactory
                   to parse XML configuration files.

* bug 9555 fixed - Typo in the 'deff' help page.

* bug 9570 fixed - fprintfMat did not use the good format argument with nan and
                   inf in a matrix.

* bug 9571 fixed - "error(144)" returned a wrong variable name.

* bug 9599 fixed - error message "at line ... of function ... called by" was
                   printed directly on the same line.

* bug 9635 fixed - some temp. variables were not removed when we used test_run.

* bug 9647 fixed - On Scilab startup, the variable fft was declared by default
                   while it should not.

* bug 9659 fixed - "error(246)" returned a bad message for overloading.

* bug 9675 fixed - iir generated bad results.

* bug 9682 fixed - 3D assignment into a 2D array of integers issued an error.

* bug 9696 fixed - An error in an if expression inside a try make Scilab froze

* bug 9707 fixed - createNamedXXX did not check variable name.

* bug 9708 fixed - createNamedXXX did not check length of variable name.

* bug 9716 fixed - amell.tst execution froze under Windows with reference lapack library.

* bug 9717 fixed - dllinfo.tst failed with Windows binary versions.

* bug 9719 fixed - delip.tst execution froze under Windows.

* bug 9725 fixed - syredi.tst execution froze under Windows.

* bug 9728 fixed - SCI/modules/special_functions/src/fortran/special_functions_f.def
                   was not ignored (generated file)

* bug 9730 fixed - lines() returned an int32 and not a double value.

* bug 9755 fixed - Completion on paths was not case insensitive on Windows.

* bug 9756 fixed - polelm.tst unitary test failed on Windows 64 bit.

* bug 9761 fixed - "NDEBUG" was not defined for dynamic link (by default) in
                   release mode on Windows.

* bug 9763 fixed - comet3d did not check input argument presence.

* bug 9765 fixed - remove ref to scicos_pal in sci_f_clear.f

* bug 9766 fixed - Scilab on Mac OS X 10.7 did not start.

* bug 9791 fixed - toolbar visible state change crash Scilab in NW mode.

* bug 9799 fixed - pol2str did not manage matrix of polynomials and input argument
                   type was not checked.

* bug 9804 fixed - Starting Scilab with a external module emptied the
                   demo list menu.

* bug 9833 fixed - percent was badly formatted by Scierror function.

* bug 9836 fixed - repfreq failed due to "singular problem".

* bug 9838 fixed - evstr froze on some specific strings.

* bug 9843 fixed - colormaps with 2 colors could not be created.

* bug 9858 fixed - length() could not be overloaded with mlist.

* bug 9869 fixed - fscanf did not check number of input arguments.

* bug 9879 fixed - Fix some minor issues in the isdir help pages.

* bug 9880 fixed - Fix in the help page a page referencing itself.

* bug 9881 fixed - Fix a mistake in the help page of call.

* bug 9900 fixed - There was a memory leak in getAllocatedMatrixOfString,
                   getAllocatedMatrixOfWideString, getAllocatedNamedMatrixOfString,
                   and getAllocatedNamedMatrixOfWideString.

* bug 9920 fixed - winqueryreg did not manage HKEY_CURRENT_CONFIG.


