            Changes between version 5.3.2 and 5.4.0 of Scilab
            =================================================


SciNotes:
=========

* In the find/grep tools, add the possibility to use ~ as a joker for user home
  dir. and SCI for Scilab data base dir.

* bug 9334 fixed - Extra lines was added when code was exported into PDF under
                   Windows.

* bug 9476 fixed - In find/grep tools, use the abbreviation 'sec' instead of
                   'seconds'.

* bug 9480 fixed - Highlighted words after selection were wrong under Windows.

* Bug 9488 fixed - There was no popup menu on the tab to copy the file path.

* bug 9505 fixed - A LaTeX string in a comment was not considered as a comment.


Graphical User Interface:
=========================

* The variable browser now refresh itself automatically (See bug 9446).


Xcos:
=====

* xcos_skeleton will not be loaded if you have not installed Xcos.

* bug 8421 fixed - CLKFROM and CLKGOTO were not part of the "Signal Routing"
                   palette.

* bug 8538 fixed - Counter block label was not reflecting parameters

* bug 9301 fixed - Exception when one tried to paste a column to replace a
                   column selection.

* bug 9336 fixed - SuperBlock with non connected ports produced an invalid
                   index error.

* bug 9478 fixed - Block structure was not checked while adding them to a new
                   palette.

* bug 9506 fixed - some libraries dependencies were not loaded for xcos_skeleton startup.

* bug 9547 fixed - The about box was not localized due to a multi-line string.

* bug 9590 fixed - xcos examples were not embeded in binary version on Windows.


File input/output:
==================

* mgetl, exec, fscanfMat functions manage files encoded as UTF-8 with BOM.


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

* Bug 7889 fixed - The tolerance on x for neldermead was too small.
                   Updated -tolxrelative default value from %eps to sqrt(%eps).

* Bug 8805 fixed - The 'neldermead' function, algorithm box, wrongly used
                   the output function.

* Bug 9567 fixed - The neldermead Overview had a TODO section.


Documentation:
==============

* <ALT>+<LEFT ARROW> / <ALT>+<RIGHT ARROW> allows the browse of the history.
  (see bug 9474)

* The space key behave like the page down key. (see bug 9491)

* Management of the revhistory / revision tag to provide information about
  the history of the function.

* Bug 9299 fixed - Fix a typo in the cspect, pspect and window help pages
                   (Hanning changed to Hann)


Removed functions (previously declared as obsolete):
====================================================

* ilib_for_link, ilib_build do not accept files with ".o" or ".obj" extensions
  for source files.

* 'with_atlas' removed. Please use 'getdebuginfo' instead.

* C API function Error(x) has been removed. Please use SciError.


Performance:
============

* SSE is now enabled by default under GNU/Linux with 32 bit CPU (it is already
  the case under 64 bit CPU).
  Pentium III class (or equivalent) with SSE instructions is required.

* bug 8608 - dec2hex, hex2dec, dec2oct, oct2dec, code have been simplified and optimized.


Compilation:
============

* Under Windows, Scilab uses VS 2010 Pro and Express solution.

* Under GNU/Linux and Mac OS X, all modules build as standalone its algorithms
  as a static library (only available in the source tree).
  This will avoid an important dependency tree for unitary testing.


Graphics:
=========

* bug 9330 fixed - delete() called overloading instead of deleting the current
                   graphic entity.

* bug 9455 fixed - The contourf and contourf 2 demos displayed unwanted dots.


Bug fixes:
==========

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

* bug 8608 fixed - dec2hex, hex2dec, dec2oct and oct2dec code have been simplified
                   and optimized.

* bug 8684 fixed - 'wfir' function produced an error message when used interactively.

* bug 8768 fixed - GetRhsVar(..., STRING_DATATYPE, ...) did not check size of input argument.

* bug 9129 fixed - Completion on field did not propose graphics handle fields
                   by default.

* bug 9156 fixed - mgetl.c compilation failed on some Windows.

* bug 9164 fixed - The unit tests now use the new assert functions.

* bug 9266 fixed - fftw crashed on Windows when using MKL wrapper.

* bug 9268 fixed - Scilab was overridding a predefined variable JAVA_HOME.

* bug 9277 fixed - escape character "\" did not work with msprintf.

* bug 9432 fixed - link a 64 bit dynamic library on a 32 bit platform returned
                   a wrong error message on Windows.

* bug 9479 fixed - Typo fixed (intput => input).

* bug 9510 fixed - export_to_hdf5 did not return %f when trying to export
                   unmanaged type (function, macro, ...)

* bug 9527 fixed - Problem when a text was exported in SVG from the gui.

* bug 9540 fixed - MKL Pentium 4 target was not downloaded by installer on Windows.

* bug 9543 fixed - Scilab did not use the default DocumentBuilderFactory
                   to parse XML configuration files.

* bug 9555 fixed - Typo in the 'deff' help page.
