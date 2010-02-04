                Changes between version 5.2.X and 5.3.0 of Scilab
               =================================================


Core:
=====

* Faster access to primitive table (internal). This change improves the overall
speed of Scilab.

Xcos:
=====

* The default palettes can now be manipulated by the user.

Compilation:
============

* On Windows, Pentium III class (or equivalent) with SSE instructions is required.

* Windows 2000 is no more supported as operating system.

 
Bug fixes:
==========

* bug 6419 fixed - gstacksize() There was no way to retrieve minimum from scilab.

* bug 6484 fixed - file did not return filenale of a opened fortran file.

* bug 6511 fixed - mseek and mtell did not manage files > 2 Gbytes on 64 bits.
