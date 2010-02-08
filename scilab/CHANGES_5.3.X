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

Tests:
======

* Mechanism of unitary tests for Java code based on testng added and used

Removed functions (previously declared as obsolete):
====================================================

* 'textprint', 'pol2tex' replaced by 'prettyprint'.

* 'tk_savefile' replaced by 'uigetfile'.

* 'tk_getdir' replaced by 'uigetdir'.

* 'sci2excel' replaced by 'write_csv'.

* 'excel2sci' replaced by 'read_csv'.

* 'x_message_modeless' replaced by 'messagebox'.

* 'sethomedirectory' replaced by 'SCIHOME'.

* 'getcwd' replaced by 'pwd'.

* 'xbasc' replaced by 'clf'.

* 'getf' replaced by 'exec'.
 
Bug fixes:
==========

* bug 6419 fixed - gstacksize() There was no way to retrieve minimum from scilab.

* bug 6484 fixed - file did not return filenale of a opened fortran file.

* bug 6511 fixed - mseek and mtell did not manage files > 2 Gbytes on 64 bits.

* bug 6543 fixed - neldermead issues for automatic restarts.

* bug 6549 fixed - genlib function had a behavior which was not consistent with the load function.

* bug 6550 fixed - lib(".") returned a relative path in library variable created.

* bug 6556 fixed - fprintf() returned an error if it's called 2 times (or more) on the same file.

