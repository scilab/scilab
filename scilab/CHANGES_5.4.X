            Changes between version 5.3.X and 5.4.0 of Scilab
            =================================================

SciNotes:
=========

* bug 9476 fixed - In find/grep tools, use the abbreviation 'sec' instead of
                   'seconds'.


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


Documentation:
==============

* <ALT>+<LEFT ARROW> / <ALT>+<RIGHT ARROW> allows the browse of the history.
  (see bug 9474)

* The space key behave like the page down key. (see bug 9491)


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

* bug 9156 fixed - mgetl.c compilation failed on some Windows.

* bug 9164 fixed - The unit tests now use the new assert functions.

* bug 9277 fixed - escape character "\" did not work with msprintf.

* bug 9479 fixed - Typo fixed (intput => input).
