               Changes between version 5.1.X and 5.2 of Scilab
               ===============================================


Compilation:
============

* Visual studio uses default runtime library /MD (msvcrt.lib) to build all libraries of scilab.

* modelicac (scicos) built with Ocaml 3.11.0 on windows

Source reorganisation:
======================

* Makefile.incl.mak (Windows, dynamic link) moved from SCI directory to SCI/modules/src/scripts

Input/Output functions:
=======================

 - Rewrite :
 	+ getenv

Bug fixes:
==========

* bug 1628 fixed - with long programs (several hours) I get a negative value for timer()

* bug 3884 fixed - chdir, getcwd, pwd moved from core to fileio module.

* bug 3953 fixed - delete() always sets the current entity to the parent of the deleted entity.

* bug 3992 fixed - remove some invalid dependencies on external libraries.

* bug 4090 fixed - draw does not work any more while inside drawlater/drawnow scope.
