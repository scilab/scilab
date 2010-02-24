                Changes between version 5.2.X and 5.3.0 of Scilab
               =================================================


Core:
=====

* Faster access to primitive table (internal). This change improves the overall
speed of Scilab.


FFTW:
=====

* On Windows, Scilab can use FFTW3 wrapper of Intel Math Kernel Library.
  Select "Download FFTW3 wrapper optimized by Intel Math Kernel Library" during
  installation.

Documentation:
==============

* It is now possible to use LaTeX inline in the documentation. Thanks to 
 Calixte Denizet for this feature. (See bug #4329)

* bug 6630 fixed - JavaHelp does not update existing image files when building help.


Xcos:
=====

* modelica compiler 1.12.1

* bug 4156 fixed - scicos defines some temp. variables on stack without remove.

* bug 4705 fixed - scicos used a non existing file BEpsf

* The default palettes can now be manipulated by the user.

* LaTeX formulas can be used on any TextBlock using the Scilab tag based 
description.

* builnewblock.sci rewritten to use scilab dynamic link module tools.

* bug 6661 fixed - "Code Generation" menu failed.

Compilation:
============

* On Windows, Pentium III class (or equivalent) with SSE instructions is 
  required.

* Windows 2000 is no more supported as operating system.

* tclsci module is dynamically loaded by Scilab.

* pvm module is dynamically loaded by Scilab.

* fix many warnings detected by Intel Fortran 11.1.051 and more on Windows.

helptools:
==========

* %helps_modules (global variable in scilab) is defined only when 
  scilab build help and no more at scilab's startup.
  
Tests:
======

* Mechanism of unitary tests for Java code based on TestNG added and used

Text Rendering:
===============

* Antialiasing activated by default to render text

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

* 'NumTokens' replaced by 'tokens'.

* 'sort' replaced by 'gsort'.

* 'scilab_demos' replaced by 'demo_gui'.

* 'with_gtk' replaced by 'getversion'.
 
Bug fixes:
==========

* bug 6419 fixed - gstacksize() There was no way to retrieve minimum from Scilab.

* bug 6428 fixed - fscanfMat did not manage format parameter as fprintfMat.

* bug 6484 fixed - file did not return filename of a opened fortran file.

* bug 6511 fixed - mseek and mtell did not manage files > 2 Gbytes on 64 bits 
                   Windows.

* bug 6516 fixed - fullfile did not remove blanks and not create a valid path.

* bug 6543 fixed - neldermead issues for automatic restarts.

* bug 6549 fixed - genlib function had a behavior which was not consistent with the load function.

* bug 6550 fixed - lib(".") returned a relative path in library variable created.

* bug 6556 fixed - fprintf() returned an error if it's called 2 times (or more) on the same file.

