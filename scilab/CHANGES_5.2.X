               Changes between version 5.1.X and 5.2 of Scilab
               ===============================================


Compilation:
============

* Visual studio uses default runtime library /MD (msvcrt.lib) to build all libraries of scilab.

* modelicac (scicos) built with Ocaml 3.11.0 on Windows

* blas, lapack, arpack libraries moved as pre-requirements (Windows)

* blas, lapack libraries moved as pre-requirements (Linux/Unix/MacOSX)

* lapack 3.2 used on Windows

Source reorganisation:
======================

* Makefile.incl.mak (Windows, dynamic link) moved from SCI directory to SCI/modules/src/scripts

Input/Output functions:
=======================

 - Rewrite :
 	+ getenv
 	
Core functions:
===============

 - Rewrite :
 	+ mode
 	

Dynamic link:
=============
 - Windows : temporary files (objects) are generated in a intermediate subdirectory (Release or Debug)
   ilib_for_link and ilib_gen_Make (with Visual studio) use a same makefile template.
   (SCI/modules/dynamic_link/src/scripts/TEMPLATE_MAKEFILE.VC)

 - ilib_verbose (SEP #17) Set level of display returned by dynamic link functions.
 
 - add a example to call a scilab macro from a C interface
  (see SCI/modules/dynamic_link/examples/interfaces/call_scilab_function directory)

Bug fixes:
==========

* bug 1628 fixed - with long programs (several hours) I get a negative value for timer()

* bug 3884 fixed - chdir, getcwd, pwd moved from core to fileio module.

* bug 3953 fixed - delete() always sets the current entity to the parent of the deleted entity.

* bug 3992 fixed - remove some invalid dependencies on external libraries.

* bug 4090 fixed - draw does not work any more while inside drawlater/drawnow scope.
