            Changes between version 5.3.2 and 5.4.0 of Scilab
            =================================================

SciNotes:
=========

* In the find/grep tools, add the possibility to use ~ as a joker for user home
  dir. and SCI for Scilab data base dir.

* bug 9476 fixed - In find/grep tools, use the abbreviation 'sec' instead of
                   'seconds'.

* bug 9505 fixed - A LaTeX string in a comment was not considered as a comment.


Graphical User Interface:
=========================

* The variable browser now refresh itself automatically (See bug 9446).


Xcos:
=====

* bug 9478 fixed - Block structure was not checked while adding them to a new
                   palette.


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
  
Xcos:
=====

* xcos_skeleton will not be loaded if you have not installed Xcos.

* bug 9506 fixed - some libraries dependencies were not loaded for xcos_skeleton startup.


Documentation:
==============

* <ALT>+<LEFT ARROW> / <ALT>+<RIGHT ARROW> allows the browse of the history.
  (see bug 9474)

* The space key behave like the page down key. (see bug 9491)

* Management of the revhistory / revision tag to provide information about
  the history of the function.


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


Bug fixes:
==========

* bug 4833 fixed - msprintf() interpreted "\n" as a carriage return even if "\n"
                   is supplied by the 2nd input argument.

* bug 4847 fixed - In uigetfile, there was no way to select files with a given
                   pattern.

* bug 6875 fixed - test_run() could not run at once on the +3000 tests of
                   Scilab.

* bug 7061 fixed - changelog.txt, readme.txt, version.xml removed from modules.

* bug 8608 fixed - dec2hex, hex2dec, dec2oct and oct2dec code have been simplified
                   and optimized.

* bug 8768 fixed - GetRhsVar(..., STRING_DATATYPE, ...) did not check size of input argument.

* bug 9129 fixed - Completion on field did not propose graphics handle fields
                   by default.

* bug 9156 fixed - mgetl.c compilation failed on some Windows.

* bug 9164 fixed - The unit tests now use the new assert functions.

* bug 9277 fixed - escape character "\" did not work with msprintf.

* bug 9432 fixed - link a 64 bit dynamic library on a 32 bit platform returned
                   a wrong error message on Windows.

* bug 9479 fixed - Typo fixed (intput => input).

* bug 9510 fixed - export_to_hdf5 did not return %f when trying to export
                   unmanaged type (function, macro, ...)
