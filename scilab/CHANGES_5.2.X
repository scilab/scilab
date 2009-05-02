               Changes between version 5.1.X and 5.2.0 of Scilab
               =================================================

Compilation:
============

* Visual studio uses default runtime library /MD (msvcrt.lib) to build all libraries of scilab.

* modelicac (scicos) built with Ocaml 3.11.0 on Windows

* blas, lapack, arpack libraries moved as pre-requirements (Windows)

* blas, lapack libraries moved as pre-requirements (Linux/Unix/MacOSX)

* lapack 3.2 used on Windows

Source reorganisation:
======================

* Makefile.incl.mak (Windows, dynamic link) moved from SCI directory to
  SCI/modules/src/scripts

Input/Output functions:
=======================

 - Input arguments of fprintf are checked 

 - Rewrite :
    + getenv
    + diary

Core functions:
===============

 - Request 505 : Add error code capability to exit (SEP #19).

 - Rewrite :
     + mode
     + predef

fileio functions:
=================
  - Input arguments of functions are checked 
    (mclearerr, mclose, merror, mget, mgetstr, meof, mopen, mput, mputstr, mseek, mtell)
    
  - fileinfo manages column vector of strings (SEP #25)

Dynamic link:
=============

 - Windows : temporary files (objects) are generated in a intermediate subdirectory (Release or Debug)
   ilib_for_link and ilib_gen_Make (with Visual studio) use a same makefile template.
   (SCI/modules/dynamic_link/src/scripts/TEMPLATE_MAKEFILE.VC)
  
 - ilib_for_link and ilib_gen_Make with lcc-win32 use a same makefile template.
   (SCI/modules/dynamic_link/src/scripts/TEMPLATE_MAKEFILE.LCC)

 - ilib_verbose (SEP #17) Set level of display returned by dynamic link functions.
 
 - add a example to call a scilab macro from a C interface
   (see SCI/modules/dynamic_link/examples/interfaces/call_scilab_function directory)


Online Help :
=============

 - Sub-Chapters are now allowed (See SEP #24)

char encoding bugs :
====================

* bug 3433 fixed - length with a UTF string returns a incorrect size

* bug 3472 fixed - Dropping files in the Scilab shell doesn't work when the path
             contains accented characters.

* bug 3666 fixed - 'input' returns accented characters.

* bug 3701 fixed - "Error during call to UTFToLocale: No such file or directory" on Windows.

* bug 3839 fixed - strrev() doesn't work correctly if the string to be reversed
                   includes some accentuated characters.

* bug 4003 fixed - setenv & getenv do not manage utf string on Windows.

* bug 4012 fixed - display of month and week are wrong in sessions starts and ends (history manager).

* bug 4019 fixed - with -nw or -nwni mode on Windows, console returns error about
                   localization.

* bug 4068 fixed - Accentuated names in directories not supported.

* bug 4145 fixed - Simplified Chinese file names can not shown correctly.

* bug 4233 fixed - exec command with a UTF filename fails.

* bug 4277 fixed - strsubst,regexp, strindex, grep return strange results if accented
                   characters and character class are associated in the pattern to match.

Bug fixes:
==========

* bug 1628 fixed - with long programs (several hours) I get a negative value for
                   timer()

* bug 3772 fixed - input argument of 'predef' was not correctly checked.

* bug 3773 fixed - 'ans' was protected with predef('all').

* bug 3884 fixed - chdir, getcwd, pwd moved from core to fileio module.

* bug 3953 fixed - delete() always sets the current entity to the parent of the
                   deleted entity.

* bug 3992 fixed - remove some invalid dependencies on external libraries.

* bug 4090 fixed - draw does not work any more while inside drawlater/drawnow scope.

* bug 4262 fixed - lines returns incorrect values.

* bug 4267 fixed - A non initilization of a lock under Netbsd was 
                   causing an error on startup

* bug 4307 fixed - Desactivate (in modules.xml) modules if its functionnalities
                   are not available.
                   
* bug 4307 fixed - %io has no help file.                   

* bug 4360 fixed - The reference of bug_3954.tst was missing.

* bug 4379 fixed - If the thickness property (of plot3d for example) is set to 0,
                   the lines/edges should disappear.

* bug 4380 fixed - diary(0) without a initialization of diary crashs scilab.

* bug 4380 fixed - merror error codes not documented.

* bug 4409 fixed - fortran optim subroutines use write (fortran) and not basout as
                   standard output.

* bug 4403 fixed - isinf was defined (when not provided by the system) with a return

* bug 4433 fixed - problem using optim. On some examples, the optim function makes scilab hangs.

* bug 4461 fixed - redefines prompt not displayed in a diary.

* bug 4467 fixed - getshortpathname,getlongpathname do not manage string matrix as input
