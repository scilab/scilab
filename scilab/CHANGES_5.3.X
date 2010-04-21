               Changes between version 5.2.X and 5.3.0 of Scilab
               =================================================


Performance:
============

* Faster access to primitive table (internal). This change improves the overall
  speed of Scilab.


FFTW:
=====

* On Windows, Scilab can use FFTW3 wrapper of Intel Math Kernel Library.
  Select "Download FFTW3 wrapper optimized by Intel Math Kernel Library" during
  installation.

Graphics:
=========

* the set and get functions are now overloadable.

* bug 6959 fixed - axis.tics_labels did not accept $..$ LaTeXed labels

Documentation:
==============

* It is now possible to use LaTeX inline in the documentation. Thanks to 
  Calixte Denizet for this feature. (See bug #4329)

* bug 6630 fixed - JavaHelp does not update existing image files when 
                   building help.

* bug 6852 fixed - JavaHelp did not center the LaTeX formulas containing an
                   attribute align="center" 

Xcos:
=====

* modelica compiler 1.12.1

* bug 4156 fixed - scicos defines some temp. variables on stack without remove.

* bug 4705 fixed - scicos used a non existing file BEpsf

* The default palettes can now be manipulated by the user.

* LaTeX formulas can be used on any TextBlock using the Scilab tag based 
  description.

* builnewblock.sci rewritten to use scilab dynamic link module tools.

* bug 5161 fixed - The export to SVG did not work with images or html.

* bug 5596 fixed - It was not possible to change text block properties (font
  and color)

* bug 6661 fixed - "Code Generation" menu failed.

* bug 6672 fixed - "Code generation" failed if superblock had 
                   some gain blocks.

* bug 6674 fixed - modelica generic block did not work.

* bug 6668 fixed - Xcos modelica demos did not work with the new 
                   modelica compiler.

* bug 6903 fixed - modelicat calls ocamlrun (wrong dependency on Windows).                   

Compilation:
============

* On Windows, Pentium III class (or equivalent) with SSE instructions is 
  required.

* Windows 2000 is no more supported as operating system.

* Scilab Mac OS X distribution relies only on Java 1.6 and is therefor now
  only available under 64 bits OS (Most of 10.5 and all 10.6 Mac OS X versions)

* Under Mac OS X, uses /usr/libexec/java_home instead of hardcoded paths to
  retrieve JAVA_HOME

* tclsci module is dynamically loaded by Scilab.

* pvm module is dynamically loaded by Scilab.

* fix many warnings detected by Intel Fortran 11.1.051 and more on Windows.

helptools:
==========

* %helps_modules (global variable in scilab) is defined only when 
  scilab build help and no more at scilab's startup.

Matlab to Scilab conversion tools:
==================================

* Unitary tests added.

* Matlab-Scilab equivalents dictionnary added in documentation.

* In 'mfile2sci', option only_double is now set to %T as default.

* Conversion updated with new Scilab functions:
  - 'filesep' now converted by 'filesep' instead of 'pathconvert'.

Tests:
======

* Mechanism of unitary tests for Java code based on TestNG added and used.

Text Rendering:
===============

* Antialiasing activated by default to render text


Obsolete Functions:
===================

* 'MSDOS' obsolete see 'getos' (SEP #39).

Removed functions (previously declared as obsolete):
====================================================

* 'textprint', 'pol2tex' replaced by 'prettyprint'.

* 'tk_savefile' replaced by 'uigetfile'.

* 'tk_getdir' replaced by 'uigetdir'.

* 'tk_choose' replaced by 'x_choose'.

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

* 'readc_' replaced by 'input'.

Bug fixes:
==========

* bug 4672 fixed - Internal function AddFunctionInTable renamed to 
                   getFunctionByName to avoid confusions on the actual usage
                   of this function. Thanks to Yuvraj Tomar.

* bug 4985 fixed - mgetl did not use fgets (C function).

* bug 4996 fixed - mgetl() was not located in the right module. It has been
                   moved to "fileio" module

* bug 6419 fixed - gstacksize() There was no way to retrieve minimum 
                   from Scilab.

* bug 6428 fixed - fscanfMat did not manage format parameter as fprintfMat.

* bug 6435 fixed - error 4 returned a wrong message.

* bug 6484 fixed - file did not return filename of a opened fortran file.

* bug 6511 fixed - mseek and mtell did not manage files > 2 Gbytes on 64 bits 
                   Windows.

* bug 6516 fixed - fullfile did not remove blanks and not create a valid path.

* bug 6543 fixed - neldermead issues for automatic restarts.

* bug 6549 fixed - genlib function had a behavior which was not consistent with
                   the load function.

* bug 6550 fixed - lib(".") returned a relative path in library variable 
                   created.

* bug 6556 fixed - fprintf() returned an error if it's called 2 times (or more)
                   on the same file.

* bug 6635 fixed - Incorrectly drawn Bode plots (calfrq bug).

* bug 6682 fixed - mprintf, msprintf, mfprintf did not manage octal (%o) format.

* bug 6740 fixed - It was not possible to launch Scilab as Minimized 
                   or Maximized Window.

* bug 6830 fixed - C2F(scistring) in stack2.c was broken for macro names of 
                   exactly 2 characters.

* bug 6891 fixed - 'whos' did not display global variables.

* bug 6947 fixed - Some demos in "Optimization and Simulation" failed, 
                   without write rights in SCI subdirectory.
