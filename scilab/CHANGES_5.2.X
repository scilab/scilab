               Changes between version 5.1.X and 5.2.0 of Scilab
               =================================================


Xcos:
=====

TODO: add an explanation about xscos


ATOMS:
======

TODO: add an explanation about atoms


Text Editor:
============

TODO: add an explanation about the text editor

Graphics:
=========

Thanks to Calixte Denizet, it is now possible to annotate Scilab's graphics
in MathML or LaTeX.
See demos => Graphics => LaTeX/MathML

* New features
  - Add transparency to markdrawing.
  - Antialiasing on graphics
  TODO: update it with the various information, sep, etc

Introduction of new binaries:
=============================

The goal of these changes is to improve the capabilities to use Scilab
in different environnements and use cases.

* Scilab 5.2 introduces new binaries in the distribution:

  - classical - The GUI application (equivalent to Scilab Standard Mode).
  	This program is exactly the same in the previous version and all arguments
	are still available
  	Name: scilab

  - Advanced Command-Line Interpreter (ACLI) - Interpreter in command line with the graphic capabilities available (equivalent to Scilab NW Mode)
  	Name: scilab-adv-cli

  - Command-Line Interpreter (CLI) - Interpreter in command line without the graphic or Tcl/Tk capabilities (faster) (equivalent to Scilab NWNI Mode)
  	Name: scilab-cli

  See SEP #18 for more comments

* The pipe has been plugged back under all operating systems
    It is now possible to do:
    echo "disp(%pi)"|./bin/scilab-adv-cli
    echo "disp(%pi)"|./bin/scilab-cli
    echo "disp(%pi)"|./bin/scilab -nw
    echo "disp(%pi)"|./bin/scilab -nwni

* Add error code capability to exit (SEP #19 and request 505).
  Example:
	echo "if 1<>2 then exit(99) end"|scilab -nwni; echo $?
	returns 99


Optimization:
=============
* New features
 - Created neldermead component for simplex-based direct search optimization
 - Created nmplot component for fast plots of neldermead optimization
 - Created optimbase component to build optimization algorithms
 - Created optimsimplex component to build direct search algorithms
 - Created fminsearch optimization function
 - Created optimget/optimset to configure fminsearch
 - Created optimplotfunccount, optimplotfval, optimplotx as plotting functions for fminsearch
* Improvements
 - Added examples in the help of optim for logging feature and optimization
   based on numerical derivatives


Extension capabilities of Scilab:
=================================

In the previous versions of Scilab, there were no clear and easy way to extend
Scilab or to use as an embedded application. 
Knowing these drawbacks, Scilab 5.2 tackles all the issues by prodiving a new
consitent, documented and easy to use API. 

* A new API called api_scilab is available. This API provides functions to
  read/write data from/to Scilab memory.
  It provides many advantages:
  - manages all Scilab data types
  - consistency over all data types
  - will be maintained over future versions of Scilab
  - error management
  - fully documented
  - fully tested by unitary tests

  Note that all other Scilab API (straight access to the Scilab stack, stack1,
  stack2 and stack3) will be maintained in the Scilab 5.X family but most of
  them will be dropped in the 6 family.
  Do not hesitate to submit a bug report if you would like to see new features
  and functions in API Scilab.

* In the enum sci_types which list all Scilab datatypes, the type 
  "sci_lufact_pointer" has been renamed to "sci_pointer".
  A define of "sci_lufact_pointer" on "sci_pointer" has been added to backward
  compatibilities.
  The rational is to improve the use in special case of the pointer datatype
  to avoid memory duplication.

TODO: talk about call scilab based on api_scilab

* Under all operating systems but Windows, all headers are not installed in a single directory in order to simplify the use
  of Scilab features in the dynamic link. See SEP #32 for more information

* From this version of Scilab, only a defined and documented number of headers
  are installed. 
  Thanks to this change, a module/toolbox developer knows exactly what are the
  available functions provided by Scilab.
  See SEP #35. 
  Do not hesitate to contact us if you have been using a header Scilab from
  Scilab which had been removed from Scilab 5.2.


TODO: add the information about the doxygen doc generation if we do it.

call_scilab module:
===================

Call Scilab is the API which allows Scilab to be called as computing engine or
embedded into a thirdparty application.
From this version, Call Scilab is based on API Scilab and handles all Scilab
datatypes (in previous versions, Double, Boolean, Complex Matrixes and String 
elements were managed).
The Call Scilab function have the same profile as API Scilab with 'Named' into 
the function name.
 
* CallScilab.h renamed to call_scilab.h (See SEP #34). CallScilab.h is still 
  available and points to call_scilab.h

* Documentation and examples about call_scilab (BooleanManagement call_scilab
  compile_and_run_call_scilab DisableInteractiveMode GetLastJob DoubleManagement
  ScilabHaveAGraph SendScilabJobs SendScilabJob StartScilab StringManagement
  TerminateScilab) added

* Some function profiles in stack3.h provided to manage named named variable
  have been moved in CallScilab.h (please note that we haven't changed the ABI
  and the symbols remain in the core library)
  The goal of this modification is to separate functions dealing with the
  call_scilab API and the internal API used in the gateways.
  Note that all the moved functions are considered as obsolete.

* In call_scilab, all previous functions available to read/write data from/to
  the Scilab engine are now obsolete but supported until Scilab 6:

  TODO  list all depecreated functions

  * C2F(readmat) is obsolete. Please use C2F(creadmat) instead (same profile)
  * C2F(readchain) is obsolete. Please use C2F(creadchain) instead
    (same profile)
  * C2F(matptr) is obsolete. Please use C2F(cmatptr) instead (same profile)

* "make check" now builds also call_scilab examples

* A example to call scilab from C# added.
   see SCI/modules/call_scilab/examples/call_scilab/NET/C# directory

* A example to call scilab from VB.NET added.
   see SCI/modules/call_scilab/examples/call_scilab/NET/VB.NET directory

Option configuration:
=====================

* The file jvm_options.xml has been moved into etc/jvm_options.xml in order to
facilitate the use of this configuration file.


Mac OS X:
=========

* Support of Snow Leopard

* Docking system enabled

* bug 4087 fixed - Under Mac OS X, a Apple + C shortcut (Ctrl + C) didn't work from the console.

* bug 4508 fixed - Can't create a file when saving/exporting a figure under
                   Mac OS X.

toolbox_skeleton:
=================

The toolbox skeleton is an example of standard toolbox which shows most of the
use cases.

* examples of gateways functions (c_sum, f_sum, cpp_find) rewritten with new API.

* cleaner added

Input/Output functions:
=======================

* Input arguments of fprintf are now checked 

* Rewrite:
    + getenv
    + diary - SEP 23

Core functions:
===============

* Rewrite:
   + mode
   + predef
   + type
   + typename
   + whereis
     
* sethomedirectory is obsolete (see home). 

* getf is obsolete (see exec).

fileio functions:
=================

* Input arguments of functions are now checked 
    (mclearerr, mclose, merror, mget, mgetstr, meof, mopen, mput, mputstr, mseek, mtell)
    
* fileinfo manages column vector of strings (SEP #25)
  
* basename uses fileparts
  
* pathconvert: If the flagtrail argument is set to FALSE and the trailing separator
                of the path is removed (if present of course). See Bug 4468.
                  
* getcwd is obsolete (see pwd). 

* isfile: checks if filename is a existing file.

* movefile: move files or directory (Request 661)

* Rewrite:
   + copyfile (primitive)
   + fileparts (primitive)
   + isdir manages matrix of strings as input argument
   + basename (primitive)
   + pathconvert (primitive)
   + cd, chdir (primitive)
  
Dynamic link:
=============

* Windows: temporary files (objects) are generated in a intermediate subdirectory (Release or Debug)
   ilib_for_link and ilib_gen_Make (with Visual studio) use a same makefile template.
   (SCI/modules/dynamic_link/src/scripts/TEMPLATE_MAKEFILE.VC)
  
* ilib_for_link and ilib_gen_Make with lcc-win32 use a same makefile template.
   (SCI/modules/dynamic_link/src/scripts/TEMPLATE_MAKEFILE.LCC)

* ilib_verbose (SEP #17) Set level of display returned by dynamic link functions.
 
* add a example to call a Scilab macro from a C interface
   (see SCI/modules/dynamic_link/examples/interfaces/call_scilab_function directory)
   
* a 'cleaner.sce' file is generated by 'ilib_for_link' and 'ilib_build'.
 
* addinter: each interface added can manage 999 functions (bug 4675). 

* ilib_gen_gateway can generated gateway without PutLhsVar (Managed by user, in this case)
  see help of 'ilib_gen_gateway' about this modification and 
  SCI/contrib/toolbox_skeleton/sci_gateway/c/builder_gateway_c.sce (example)

* ilib_gen_loader now generates loaders which can be executed outside the loader's directory

* bug 4726 fixed - Under !Windows, when the link dynamic was copying the files,
  in some cases, it was going to copy backup files (for example, sci_luinc.c.bak
  instead of sci_luinc.c)

* bug 4919 fixed - If the source file was stored in TMPDIR, the dynamic link
  was removing the content of this file.

Online Help:
============

* Sub-Chapters are now allowed (See SEP #24)

* xmltochm exports help to chm format (Windows)

* add_help_chapter now accepts string array as input argument: consequently,
   several online help chapters can be added at the same time.

* add_help_chapter now accepts to add help chapters with the same title.

* In the documentation, the font size of the equations has been increased 
  for a better readability.

* Javasci FAQ page added

* Online help XML files have been converted into UTF-8
 

Source reorganisation:
======================

* Makefile.incl.mak (Windows, dynamic link) moved from SCI directory to
  SCI/modules/src/scripts
  
* newest, get_absolute_file_path, getrelativefilename moved from io to fileio.

* bug 3668 fixed - formatnumber.f, dmdsp.f, ... moved in output_stream module.

* printf, msprintf, mprintf, print moved in output_stream module.

* disp moved in output_stream module.

* bug 3668 fixed - lib, deff, getf, exec, execstr moved in functions module.

* oldsave, oldload functions (only used with macros of scilab 2.4) are removed
  Replaces oldsave, oldload by save and load in your code.
  
* bug 3669 fixed - integrate, intc and intl moved in Differential equations
  interp1, interp2d and splin2d  moved in the Interpolation module.

* libs/MALLOC/src/malloc.c is no longer build (used to be built for 64 
bits archs). Note that if Scilab is built on a 64 bits and triggers a 
seg fault on startup, uncomment the related line in 
libs/MALLOC/Makefile.am and launch autoreconf.


Compilation:
============

* Visual studio uses default runtime library /MD (msvcrt.lib) to build all libraries of scilab.

* Scilab on Windows built with Intel Fortran 11.0.061 and Intel C++ 11.0.074

* Symbols in dlls are explicitly exported on Windows.
  In previous versions of Scilab all symbols were exported.

* modelicac (scicos) built with Ocaml 3.11.0 on Windows

* blas, lapack, arpack libraries moved as pre-requirements (Windows)

* blas, lapack libraries moved as pre-requirements (Linux/Unix/MacOSX)

* lapack 3.2 used on Windows

* functions module is loaded dynamically by Scilab.

* autoconf updated to version 2.64

* automake updated to version 1.11

* libtool updated from version 1.5.26 to 2.2.7a (Closes bug #4279) with some
  patches for Mac OS X/Darwin support

* --enable-debug-java add two options to the JVM args (-verbose:jni and
  -Xcheck:jni) in order to provide more feedbacks on debug
  Note that this should not be used into a binary since it is expensive

* C++ => Java connector updated (using GIWS - 
  http://www.scilab.org/giws/ version 1.0.3)
  There was a bug in GIWS:
	The use of a static method returning an int wasn't calling the right
	method.
	It did not seem to trigger any errors in the past but the latest 
	version of the JDK is now complaining.


* Patches applied (see bugs #4506, #4517 & #4588) by Dan McMahill which fixes 
  some compile time problems under NetBSD which also improves all but Windows
  other operating systems.
  		- adds some missing $host to the 64 bit list.  
		- added a needed compiler flag for alpha-*-netbsd
		- added autoconf check for term.h header
		- added autoconf checks for gettext.
		- added autoconf check for dlopen in -ldl 
		  (on some systems, we don't need -ldl because dlopen is in libc).  
		  Don't hardcode linking with -ldl but rely on this new check.
		- added correct getrlimit() argument for NetBSD and DragonFly
		- avoid pulling in an OSF1 specific header on NetBSD/alpha
		- added missing newline at the end of a file
		- added missing XML LIBS and CFLAGS
		- adds LTLIBINTL to the list of common objects needed for linking the top level binary.
		- adds checks for term.h and termcap.h headers.  
		- adds check for -lpthread
		- reworks the checks for curses a little bit to search for 
		  curses.h and -lcurses first followed by ncurses.h and 
		  -lncurses.   
		- use configure results to protect the inclusion of curses.h, 
		  ncurses.h, term.h, and termcap.h.
		- add __DragonFly__ to a few conditional lines.
		- remove the static declaration of BC to match what was found
		  in a header file.
		- adds missing enable_keypad_mode()
		- use configure results to protect the inclusion of curses.h, 
		  ncurses.h, term.h, and termcap.h.
		- Large amounts of cleanup of the PVM configure code (Closes: #4151)

* Improve detection of build/run environnement under GNU/Linux Debian under
  arm, armel, mips & mipsel

tests:
======

* test_run() now accepts Sub-Modules (See SEP #31)

Internationalisation:
=====================

* Portugues do Brazil Localization added (Thanks Daniel de Souza Grilo and others)

* Japanese localization added (Thanks Hiroshi Saito)

* More help pages translated to Portugues do Brazil (Thanks Daniel de Souza Grilo)

* Catalan localization added to Scipad (Thanks Oriol Gonzalez)

* Japanese localization added to Scipad (Thanks Hiroshi Saito)

Bugs:

* bug 2627 fixed - Scilab 5 console retrieve input strings in UTF-8 format and Scilab doesn't
                   manage this format but ISO-8859.

* bug 3228 fixed - ascii function broken for extended set (example: ascii('�')).

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

* bug 4617 fixed - Some items (Console pull down menu and help browser item) 
  	  	   		   were not translatable

* bug 4660 fixed - Error in the French localization (Edit instead of Edition)

* bug 4758 fixed - embedded version of autoconf libs.m4 were causing some issues

* bug 4768 fixed - strsplit returns strange results with accened characters

String manipulation:
====================

* Rewrite:
    + strsplit - SEP 26

* 'NumTokens' is obsolete. see 'tokens'

Java/Scilab API:
================

 - uses api_scilab functions to write & read data (internal for javasci)

 - SciInteger class added  

Functions removed:
==================

* lgfft (See bug #4600)

Linear algebra:
===============
 - Clarified documentation for lu function.

elementary functions:
=====================
 - 'sort' is obsolete. see 'gsort'

Bug fixes:
==========

* bug 336  fixed - pspect and cspec help pages improved

* bug 415 fixed - wrong order in the roots of a polynomial


* bug 667  fixed - scilab.start-file causes an initialization error when you 
                    use predef('all') in a user startup file.

* bug 1412 fixed - A(3,3)=%s,A(:)=%z causes scilab to hang

* bug 1451 fixed - set theta, alpha, leg, flags as optional named arguments for hist3d

* bug 1628 fixed - with long programs (several hours) I get a negative value for
                   timer()

* bug 1776 fixed - funcprot accepts non-integer arguments

* bug 1809 fixed - Add a warning message (figure_size) after an invalid graphical script

* bug 1885 fixed - histplot should start from %eps rather than 0 to avoid logarithmic trouble.

* bug 2042 fixed - copy function altered axes properties.

* bug 2043 fixed - The function copy applied to an axes handle does not work

* bug 2238 fixed - function returns a wrong error

* bug 2275 fixed - add the isvector help page

* bug 2347 fixed - "fun2string" indentation problem with select/case 
                    instruction.

* bug 2446 fixed - "while" documentation help page improved to explain 
                    that the key words do or then can be ommited .

* bug 2453 fixed - mfscanf and probably the other variants msscanf mscanf may crash Scilab when
                   first argument is -1 (any number of lines)

* bug 2509 fixed - execstr forgets to store the function name and line number when in the errcatch mode

* bug 2537 fixed - improve xstring feature

* bug 2570 fixed - LCC does not work with some "stk" ref on some old toolboxes.

* bug 2649 fixed - diary() function cannot be nested.

* bug 3131 fixed - a glibc >= 2.4 was mandatory (causing Scilab to fail under
                   Debian Etch, Centos 4.X and some other old operating 
                   systems)

* bug 3264 fixed - More explicit message when too many files have been
                    opened.

* bug 3184 fixed - ticks of colorbar have disappeared

* bug 3201 fixed - add the help page for the function del_help_chapter

* bug 3422 fixed - the keyword for history search was not updatable

* bug 3488 fixed - dynamic link functions can now handle Fortran 90 code.

* bug 3533 fixed - lib(dir_path) doesn't work if the final slash is missing in dir_path.

* bug 3537 fixed - Problem with examples in french help page for metanet functions. 
                    The old graph data structure was used.

* bug 3543 fixed - Wrong parenthesize in struct indices (code generated by tree2code).

* bug 3580 fixed - a gateway declaration containing a false primitive ID or gateway ID crashs scilab.

* bug 3612 fixed - history manager doesn't work at startup

* bug 3660 fixed - font_angle was lost on the axis label when exporting through xs2pdf - Thanks to Paul Griffiths for the patch

* bug 3772 fixed - input argument of 'predef' was not correctly checked.

* bug 3773 fixed - 'ans' was protected with predef('all').

* bug 3884 fixed - chdir, getcwd, pwd moved from core to fileio module.

* bug 3923 fixed - diary() function produces bad files when it try to record interactive commands.

* bug 3953 fixed - delete() always sets the current entity to the parent of the
                   deleted entity.

* bug 3980 fixed - Wrong line number in error message

* bug 3992 fixed - remove some invalid dependencies on external libraries.

* bug 4030 fixed - unix_w('dir /s') can display strange matrix on windows.

* bug 4054 fixed - load into scilab a file with editor can crash scilab.

* bug 4090 fixed - draw does not work any more while inside drawlater/drawnow scope.

* bug 4101 fixed - -ns option wasn't clearly explaining that it is 
                    disabling many Scilab features

* bug 4110 fixed - Demos order differs between Linux and Windows versions

* bug 4181 fixed - remove the AdCommunications help page

* bug 4184 fixed - style of the examples has been corrected

* bug 4217 fixed - gsort sort complex by magnitude only with 'g' and not support 'r', 'c', 'lr', 'lc'

* bug 4222 fixed - fromc and fromjava do not have any documentation page.

* bug 4232 fixed - Fix a problem in the Java detection with GNU/Linux under
                    PowerPC

* bug 4262 fixed - lines returns incorrect values.

* bug 4263 fixed - completion cleans current input line.

* bug 4267 fixed - A non initilization of a lock under Netbsd was 
                   causing an error on startup
                   
* bug 4277 fixed - force UTF8 management in pcre processing.

* bug 4278 fixed - typename() called with a wrong number of input arguments returns incoherent errors.

* bug 4298 fixed - ilib_for_link fails with lcc-win32 (Windows).

* bug 4307 fixed - Desactivate (in modules.xml) modules if its functionnalities
                   are not available.
                   
* bug 4307 fixed - %io has no help file.                   

* bug 4330 fixed - stacksize('max') + stacksize(required_mem) doesn't work

* bug 4347 fixed - messagebox(): buttons are displayed in reverse order with 
                   respect to their order in the vector specifying them (Windows).

* bug 4351 fixed - test about 'exit' does not work on linux.

* bug 4358 fixed - changing defaultstacksize in scilab.start doesn't work

* bug 4360 fixed - The reference of bug_3954.tst was missing.

* bug 4364 fixed - stacksize('max') + stacksize(required_mem) doesn't work

* bug 4365 fixed - completion fails on a wrong path.

* bug 4369 fixed - Help for text_properties: For the clip_state attribut,
                    the second possible value was 'clipgrf' instead of 'cliprf'.

* bug 4379 fixed - If the thickness property (of plot3d for example) is set to 0,
                   the lines/edges should disappear.

* bug 4380 fixed - diary(0) without a initialization of diary crashs scilab.

* bug 4380 fixed - help page of the printf command has a problem.

* bug 4380 fixed - merror error codes not documented.

* bug 4385 fixed - the param3d_properties page has been removed, but a lot of
                   links to this page exist in the documentation.

* bug 4386 fixed - Dynamic link: link('show') displays only one library if
                   we have only one symbol.

* bug 4391 fixed - make install-html (when the option --with-install-help-xml
                   is added to the configure) wasn't installing MathML sources

* bug 4394 fixed - Add 'origin' as position for X and Y axes

* bug 4395 fixed - incorrect descriptions of the axesflag and nax parameters
                   in the plot2d help page.

* bug 4396 fixed - In Plot2d, the nax option can receive -1 for number of ticks, it's mean autoticks

* bug 4403 fixed - ieeefp.h wasn't included under Solaris in evaluate_expr.c
  	  	   		   isinf was defined (when not provided by the system) with a return

* bug 4409 fixed - fortran optim subroutines use write (fortran) and not basout as
                   standard output.

* bug 4407 fixed - help file scilab_xx_XX_help.jar have another scilab_xx_XX_help.jar embedded
                   and not valid.

* bug 4414 fixed - Complex grayplots are not saved correctly in scilab 5.1

* bug 4421 fixed - completion fails on some relative path.

* bug 4426 fixed - uigetfile() - File name is cut when we are in the root directory.

* bug 4428 fixed - function compute_initial_temp cannot handle functions with
                    several arguments

* bug 4430 fixed - incorrect description of the margins property in the axes_properties help page.

* bug 4431 fixed - xsegs() accept z argument

* bug 4433 fixed - problem using optim. On some examples, the optim function makes scilab hangs.

* bug 4437 fixed - Exporting an empty figure crashes Scilab.

* bug 4438 fixed - completion does not support insert mode.

* bug 4439 fixed - Unexpected complex conjugates when using cat.

* bug 4459 fixed - grayplot does not always plot map containing %nan.

* bug 4461 fixed - redefines prompt not displayed in a diary.

* bug 4463 fixed - Creating an one-colored Sgrayplot generate some exceptions
                   and does not plot anything.

* bug 4466 fixed - .' operator defined for strings and booleans

* bug 4467 fixed - getshortpathname,getlongpathname do not manage string matrix as input

* bug 4468 fixed - pathconvert(<path>,%F) should remove last trailing separator.

* bug 4469 fixed - getmd5() & mopen hang if there are called with an existing
                    directory as input argument and not a filename (linux)

* bug 4474 fixed - help of 'fort' is obsolete.

* bug 4474 fixed - addinter help has no example.

* bug 4479 fixed - A few minor bugs were causing the configure to fail under
                   FreeBSD (Thanks to Otacílio de Araújo Ramos Neto)

* bug 4483 fixed - Handle FreeBSD java detection

* bug 4486 fixed - When a file has been opened for execution with "File/Execute..."
                   menu, its pathname is lost.

* bug 4489 fixed - oldsave and oldload removed.

* bug 4491 fixed - get() with wrong argument causes EXCEPTION_ACCESS_VIOLATION error.

* bug 4492 fixed - output_stream_gateway.xml was not include in windows binary.

* bug 4495 fixed - exec does not check second and third input arguments.

* bug 4496 fixed - in tbx_build_gateway help page, ismex parameter is not documented.

* bug 4497 fixed - xmltoformat gets stuck in directories beginning with '.'.

* bug 4498 fixed - merror() returns weird strings if called before opening files.

* bug 4499 fixed - basename returns a wrong value if file or a directory starts with a dot

* bug 4500 fixed - basename('') returns a error

* bug 4501 fixed - mput() no more allowed integer types as first input argument.

* bug 4507 fixed - Depending on the fortran runtime libraries a dummy main
                   definition may be needed.

* bug 4514 fixed - valgrind.supp wasn't installed

* bug 4516 fixed - ilib_for_link does not use ilib_gen_loader but a internal function.

* bug 4518 fixed - ilib_build with a column vector of strings as filenames does not work on linux

* bug 4522 fixed - The Tcl interpreter initialization code makes use of some unitialized variables.

* bug 4515 fixed - linking and loading some external functions already used crashs scilab.

* bug 4525 fixed - scilab.sln and scilab_f2c.sln do not apply same dependencies on libraries (windows).

* bug 4528 fixed - makefile (windows) generated by ilib_for_link forces to rebuild all files each time.

* bug 4530 fixed - leg labels curves are in reverse order

* bug 4533 fixed - link fails on some path (windows)

* bug 4539 fixed - some optional parameters where not set in hist3d

* bug 4542 fixed - A unexpected interpretor warning 

* bug 4546 fixed - Error message added for linpro function (moved to an external function).
 
* bug 4547 fixed - examples of javasci with graphics are no more interactive.

* bug 4548 fixed - with intel fortran, file unit opened in a external dll are not shared.

* bug 4551 fixed - menus can return a error in GUI mode (error 2 Invalid factor)

* bug 4552 fixed - c++ comments in stack3.h have been translated into c comments

* bug 4564 fixed - Unable to edit matrices using x_mdialog function.

* bug 4573 fixed - core.start tried to load add_module_help_chapter even if it not exists

* bug 4574 fixed - ilib_build does not check if library is already loaded in scilab.

* bug 4580 fixed - rpem documentation made more accurate

* bug 4584 fixed - On Windows , home value was wrong

* bug 4585 fixed - Scilab fails to build with intel C++ compiler 11.0.074 on Windows

* bug 4586 fixed - Function fieldnames added for getting tlist, mlist and struct field names (see SEP#28)

* bug 4592 fixed - The ged entity picker produces an error if a menu has been
                   added to the graphic window

* bug 4596 fixed - g_margin fails to return the gain margins in some cases.

* bug 4600 fixed - missing function inside lgfft (lgfft must be removed).

* bug 4601 fixed - hidden functions and missing help for vec2list list2vec aplat and recons.

* bug 4603 fixed - Unknown function lst2tree called by function mtlb_eval
                   (+ some other problems fixed in mtlb_eval).

* bug 4604 fixed - rmdir(<dir>,"s") doesn't work if it contains a hidden file
                   (starting with a dot).

* bug 4605 fixed - basename('') returns a error.

* bug 4606 fixed - example of 'link' in help does not work

* bug 4707 fixed - Fail to build with ./configure --without-gui

* bug 4608 fixed - get_function_path returns a non-consistent path.

* bug 4611 fixed - what function doesn't returns a complete list of functions.

* bug 4612 fixed - The graphic export used to consider .jpeg as an invalid
                   extension.

* bug 4614 fixed - On some (rare) archs/OS, File -> Export on a graphic could
                    lead to a crash 

* bug 4615 fixed - xs2jpg(0, '/tmp/myplop.jpeg'); was creating a file 
                   called /tmp/myplop.jpeg.jpg (Thanks to Guilherme 
                   Kunigami for the fix)

* bug 4616 fixed - Text strings in SVG output are not rotated. Thanks 
                   to Paul Griffiths for the patch!

* bug 4618 fixed - buttmag produces a warning because of redefining symbol sample.

* bug 4626 fixed - when using the CLI version, completion make Scilab crash

* bug 4627 fixed - The function matfile2sci does not manage 7.x format.

* bug 4630 fixed - Problem with the Scicos block affich.

* bug 4635 fixed - Bad title in the french help of the graphic module.

* bug 4636 fixed - Metanet on-line help: an accent was missing on the "e" in the french version 

* bug 4638 fixed - this example with optim freezes scilab

* bug 4639 fixed - Some graphics demos does not contain the menu item "show code"

* bug 4641 fixed - the parameter Q of the function derivative was not clear.

* bug 4645 fixed - french 'gsort' help had some wrong informations.

* bug 4646 fixed - Assigning a string to an element of a matrix of numbers does not give an error.

* bug 4650 fixed - SCI/modules/graphics/macros/Sfgrayplot.sci has not been modified for localization.

* bug 4654 fixed - max of an empty sparse matrix makes Scilab hangs

* bug 4656 fixed - part(input_matrix,[]) always returned a single string even if
                   input_matrix had a several dimensions

* bug 4658 fixed - For graphics macros from adddcolor to getfont and all colormap.

* bug 4666 fixed - SCIHOME was not well defined if scilab has been launched in another scilab session.

* bug 4676 fixed - diary() removes whitespaces characters from the output of mprintf()

* bug 4683 fixed - There is no example of the use of tlist in the dedicated page.

* bug 4684 fixed - printf(), mprintf, msprintf do not properly deal with -%inf.

* bug 4688 fixed - listfiles("/") returned an error.

* bug 4689 fixed - diary() do not report interactive commands into the output file

* bug 4695 fixed - some help pages + examples where missing in the arnoldi package

* bug 4702 fixed - on some case, dos(cmd) does not return results

* bug 4707 fixed - Can not save a Scicos diagram to a non existing file under MacOS

* bug 4709 fixed - The set function with 2 parameters and no handles makes Scilab hangs

* bug 4710 fixed - Error with C++ interface which uses the function CreateVar.

* bug 4712 fixed - by GUI handle affectation produce an error.

* bug 4719 fixed - fsolve may fail on some system of equations. Add an example in the documentation.

* bug 4724 fixed - when the f variation of a data set is too small, plot hangs

* bug 4727 fixed - The function 'length' does not return the same value in Scilab 5.1 and Scilab 4.1.2

* bug 4429 fixed - Use hidden_axes instead of hidden_axis

* bug 4728 fixed - The function 'strcat' does not return the same value in Scilab 5.1 and Scilab 4.1.2

* bug 4732 fixed - the help page of function ss2ss has some missing part

* bug 4737 fixed - completion fails on file extension search.

* bug 4744 fixed - whereis, librairieslist, libraryinfo primitives should be moved in "functions" modules.

* bug 4750 fixed - Error boolean catenation

* bug 4752 fixed - ci2exp produces erroneous code when applied to boolean matrices (; are missing)

* bug 4753 fixed - if you have a toolbox and you launch scilab-cli, you have a error message at startup. 

* bug 4759 fixed - We should provide a better error message in nlev when called with no input argument.

* bug 4770 fixed - modules::renderer::utils::ColorMap::isValidScilabIndex test was wrong.

* bug 4773 fixed - Using "axes_reverse" produce a switch between the front and back color of a figure.

* bug 4780 fixed - The insertion point goes to the end of the document at each newly entered char.
		   Once unchecked The Document/Colorize menu item is impossible to check again.
		   If there is no selected line and one execute the Execute/EvaluateSelection menu a java exeception is reported

* bug 4789 fixed - number of input arguments is not checked in ilib_functions (dynamic_link module)

* bug 4792 fixed - macro name too long

* bug 4795 fixed - CallScilab.h renamed call_scilab.h

* bug 4797 fixed - macro make_help_index.sci (internal) removed (not used in scilab 5.x)

* bug 4798 fixed - pathconvert macro should be rewritten as a primitive (many times used and not faster).

* bug 4807 fixed - strsubst does not check if second argument is a valid patter where fourth argument is 'r'.

* bug 4810 fixed - gsort does not manage sparse vectors as sort.

* bug 4811 fixed - for some figures, it is not possible to save and load the handle of the figure.

* bug 4812 fixed - xs2... function doesn't work if the first argument is a handle

* bug 4816 fixed - plot doesn't manage autoclear correctly.

* bug 4817 fixed - macro savedefaultbrowser (internal) removed (not used in scilab 5.x)

* bug 4826 fixed - The type of the 'dims' entry of an hypermat is not always the same.

* bug 4828 fixed - completion clears the content of some completed path.

* bug 4832 fixed - wavread returns a non explicit error if RIFF have some 'smpl' chunck.

* bug 4838 fixed - completion returns wrong value with '..' 

* bug 4846 fixed - mopen("","") crashs on windows.

* bug 4853 fixed - C,C++ dlls generated by scilab had a dependency on fortran runtime (windows).

* bug 4855 fixed - Use standard error messages when setting/getting values from graphics handles.

* bug 4872 fixed - editor(<binary file>) opens a blank file without any warning.

* bug 4917 fixed - fileinfo(SCI+'/') returns [] (on Windows)

* bug 4918 fixed - error message returned by 'get_absolute_file_path' is incorrect.

* bug 4925 fixed - verbose option of the optim function crashs on Windows.

* bug 4926 fixed - Can not save a structure in a MAT-file using savematfile.

* bug 4927 fixed - When the user try to plot some data in semilogx or semilogy, a blank plot windows is displayed.

* bug 4928 fixed - x_mdialog crashes when more default values than labels.

* bug 4943 fixed - execstr does not check input arguments.

* bug 4964 fixed - set or get 'format_n' property from an axes crash scilab.

* bug 4966 fixed - execstr([]) should return [].

* bug 4974 fixed - exec('') crashs scilab.

* bug 4983 fixed - when scilab is started in nw mode, the addmenu examples are failing with a wierd error message.

* bug 4991 fixed - No check if there is modification when doing "Execute Into Scilab" in the editor.

