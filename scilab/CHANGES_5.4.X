            Changes between version 5.3.X and 5.4.0 of Scilab
            =================================================

File input/output:
==================

* mgetl, exec, fscanfMat functions manage files encoded as UTF-8 with BOM.


Compilation:
============

* Under Windows, Scilab uses VS 2010 Pro and Express solution.


Bug Fixes:
==========

* bug 4833 fixed - msprintf() interpreted "\n" as a carriage return even if "\n"
                   is supplied by the 2nd input argument.

* bug 8608 - dec2hex, hex2dec, dec2oct and oct2dec code have been simplified
             and optimized.

* bug 8608 - dec2hex, hex2dec, dec2oct, oct2dec, code have been simplified and optimized.


Removed functions (previously declared as obsolete):
====================================================

* ilib_for_link, ilib_build do not accept files with ".o" or ".obj" extensions
  for source files.


Performance:
============

* SSE is now enabled by default under GNU/Linux with 32 bit CPU (it is already
  the case under 64 bit CPU). 
  Pentium III class (or equivalent) with SSE instructions is required.
