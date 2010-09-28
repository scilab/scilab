            Changes between version 5.3.X and 5.4.0 of Scilab
            =================================================


Bug Fixes:
==========

* bug 8608 - dec2hex, hex2dec, dec2oct and oct2dec code have been simplified
             and optimized.


Removed functions (previously declared as obsolete):
====================================================

* ilib_for_link, ilib_build do not accept files with ".o" or ".obj" extensions
  for source files.


Performance:
============

* SSE is now enabled by default under GNU/Linux with 32 bit CPU (it is already
  the case under 64 bit CPU). 
  Pentium III class (or equivalent) with SSE instructions is required.
