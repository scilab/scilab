            Changes between version 5.3.X and 5.4.0 of Scilab
            =================================================

SciNotes:
=========

* Add tools 'Find Files' (ctrl+shift+B) and 'Find word in Files' (ctrl+shift+F).


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

Bug Fixes:
==========

* bug 4833 fixed - msprintf() interpreted "\n" as a carriage return even if "\n"
                   is supplied by the 2nd input argument.

* bug 8608 - dec2hex, hex2dec, dec2oct and oct2dec code have been simplified
             and optimized.

* bug 8608 - dec2hex, hex2dec, dec2oct, oct2dec, code have been simplified and optimized.

* bug 9156 - mgetl.c compilation failed on some Windows.

* bug 9164 - The unit tests now use the new assert functions.


Removed functions (previously declared as obsolete):
====================================================

* ilib_for_link, ilib_build do not accept files with ".o" or ".obj" extensions
  for source files.


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
