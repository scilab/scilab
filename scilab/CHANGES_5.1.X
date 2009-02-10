               Changes between version 5.0.X and 5.1 of Scilab
               ===============================================

New Features:
=============

* Added elementary functions: acosd acot actod acoth asecd asech asec asind
  acsc acscd acsch csch atand cosd cotd cscd csc secd sech sec sind tand

* Scicos documentation in Docbook format.
  PDF, PS, JAR and other versions now available.

* Graphics available on all platforms and OS using function usecanvas
  See help page of usecanvas function for more details.

* New function dae (for Differential Algebraic Equations) added.
  This function is providing the same features as dassl with ode 
  (ordinary differential equations) profile.
  For more information, see SEP #2.

* Request 564: completion add '\' or '/' at the end of a directory

* Added uigetfile (Functions xgetfile and tk_getfile are now obsolete).
  For more information, see SEP #8.

* Scilab is now supported natively under Mac OS X (See RELEASE_NOTES_5.1.X)

* Uses termios instead of termio in NW & NWNI modes

Enhancement:
============

* Graphics stabilty improved.

* Graphics speed improved.

* Menubar/toolbar update improved (particularly when (un)docking windows).

* Update of some output strings (standard, warning & error)

* Scilab uses Atlas 3.9.4 on Windows

* Directive FRENCH IMPOSED added to test_run() to force the test in french

* Errors occuring when getting or setting graphic/gui handles properties are
  now treated as any other Scilab errors.

* Now catch the exception when JOGL is crashing because native library 
  gluegen-rt is not available in the java.library.path (LD_LIBRARY_PATH) and 
  display a nice message

* Better error report in the documentation build process (docbook)

* Binary versions embed jre 1.6.0_11

* test_run() now accepts absolute paths as first input argument. It permits
  toolboxes to use this system.

* Under Linux/Unix, do not launch Scilab in background when called without argument

* icons are now installed into pkgdatadir/icons/

* Update of the example in the dynamic_link modules and in the toolbox skeleton
  (use the actual filename instead of the .o)

Tests:
======

* Tests added for functions: umf_luget, taucs_chget, pcg, removedir, rmdir, ones

Internationalization:
=====================

* Almost all the i/o functions (isdir, mkdir, cd, chdir, mopen, fileinfo, 
mclose, deletefile, findfiles, ls, listfiles, getshortpathname, 
getlongpathname, removedir...) now handle file and directory names containing
UTF-8 characters. This is the work of Yung-Jang Lee. Many thanks!
(Unitary tests provided) 


Misc:
=====

* Some dead code removed (SCI/scripts/, 
                          SCI/bin/{dold,scilink,Blatdoc,Blatdocs,f77-linux,minfopr} )


Compilation:
============

* libmatio is now detected thanks to pkg-config

* More warnings in C/C++/Fortran during the compilation process.
See http://lists.scilab.org/cgi-bin/ezmlm-browse?list=dev&cmd=showmsg&msgnum=684

* --enable-build-help is now the default behaviour
  (--disable-build-help removes the check of the dependencies)

* --with-fftw, --with-pvm and --with-umfpack are now the default behaviour 
  (--without-fftw, --without-pvm and --without-umfpack removes their builds)

* interpolation, signal_processing, statistics modules are loaded dynamically 
  by Scilab.

* cc and cc-* are not longer consider as GNU C compiler when detecting the 
  compiler (use gcc & gcc-*) because it was causing some issues under Solaris

* By default, under Linux/Unix/Macosx, static libraries are no longer built.

* Do not init TK when building the doc ("make doc")... in order to be able to 
build Scilab through ssh

* Giws has lost it .py extension

* Force the build for the french documentation.


Bug fixes:
==========

* bug 1582 fixed - If parametric 3d curve values are out of given ebox of 3d 
                   axes, this curve values are netherveless plotted.

* bug 1926 fixed - wavwrite doesn't manage single channel mode.

* bug 2175 fixed - "contour2d" prints level numbers partially outside the rect.

* bug 2255 fixed - plot function fails to verify that x and y data have the same
                   length in certain cases.

* bug 2265 fixed - Add a property to toggle grid position between background
                   and foreground.

* bug 2273 fixed - chdir didn't manage UNC path on Windows.

* bug 2357 fixed - The vertex-order of the plot3d2 and plor3d3 functions is 
                   different from the one given in the manual.
				   
* bug 2453, 3731, 3759, 3784, 3801 & 3802 fixed - Force all the numerical value
                                              to remain to the english notation.

* bug 2647 fixed - Bad display of some numbers in special cases due to a 
                   gfortran bug.

* bug 2682 fixed - chart() is inconsistent in the second argument, angle.
                   It might be treated as degrees or radian.
				   
* bug 2767 fixed - By default graphic windows are opened one above each others
				   whereas they should be tiled.
                   
* bug 2814 fixed - Ticks graduation produce some unwanted decimals.

* bug 2959 fixed - functions on F1, F2, F12 keys are not mapped as scilab 4.x

* bug 3070 fixed - Could not use non-simple character like °, µ, ø ....

* bug 3080 fixed - When we click many times on a button of the Graphic window 
                   the instruction will be done as much times we have clicked.

* bug 3089 fixed - chdir seems to have problems with directory names containing
                   nordic characters like æ ø and å.

* bug 3131 fixed - version 'GLIBC_2.4' not found with the binary version under 
                   some GNU/Linux distro

* bug 3314 fixed - linspace primitive - rev 27707 - removed use of "ones" and 
                   use colon (i.e. :) operator only

* bug 3322 fixed - trimmean primitive - rev 27627 - Added verbose option, added
                   unit tests, updated doc.

* bug 3326 fixed - ESC key clears the command line in the console. 

* bug 3407 fixed - Line number where error is reported changed in Scilab may be 
                   wrong.

* bug 3411 fixed - eye on a sparse is not explicit.

* bug 3462 fixed - The match string returned by regexp is not the right one if 
                   the subject string contains backslashes.

* bug 3470 fixed - plot2d(), followed by legends produce an unexpected error.

* bug 3500 fixed - [pcre] match position but no character = no match

* bug 3512 fixed - regexp could return incorrect results

* bug 3515 fixed - Erroneous submenu numbering for setmenu() and unsetmenu() in 
                   Scilab 5.0.1

* bug 3527 fixed - Graphic windows created by figure are unable to display plots.

* bug 3530 fixed - Check that X11/Xlib.h is available since the Tk header needs it.

* bug 3546 fixed - frmag is broken when called with hn given by a vector of 
                   coefficients instead of a polynomial.

* bug 3549 fixed - machine.h was including PATH_MAX.h, BOOL.h and PARAMS.h 
                   includes have been cleaned up.

* bug 3554 fixed - 'findobj' function does not find figures nor sub-menus and 
                   the help example does not work.

* bug 3556 fixed - derivative was not supporting non compiled functions.

* bug 3559 fixed - paths in examples.bat in SCI/modules/javasci/examples were 
                   wrong.

* bug 3561 fixed - 'call' crashs when we want to return a 'c' variable type.

* bug 3565 fixed - bad detection of the IBM JDK

* bug 3571 fixed - Only one message displayed when log(0) is computed with 
                   ieee(1) mode.

* bug 3578 fixed - the definition of NewSparse and FreeSparse have been added.

* bug 3581 fixed - Legends do not appear correctly when axes are plotted in log
                   scale.

* bug 3590 fixed - Error message when we want to see a figure which has as 
                   userdata a structure (struct).

* bug 3596 fixed - In the graphic export (bitmap) the exported file is stored in
                   vertical mirrored fashion.

* bug 3605 fixed - Better fix of the javasci freeze. A part of this bug was 
                   caused by a dead lock in the TCL thread (we were trying to 
                   get access to a TCL interpretor which was locked).

* bug 3608 fixed - Scilab freezes when executing a combination of 
                   "eventhandler", "xchange" and "plot"

* bug 3624 fixed - Nan returned when multiplying large zero vector by its transpose.

* bug 3624, 3640 fixed - Atlas 3.8.0 have a bug causing wrong answer for 
                         row-major gemm C=A*A' or A'A 

* bug 3626 fixed - lcc-win32 library (.lib) not correctly generated.

* bug 3627 fixed - variance did not manage complex matrices of data

* bug 3628 fixed - Problem with cdouble type arguments in intersci.

* bug 3630 fixed - functions added by 'addinter' are not visible by 'what'.

* bug 3635 fixed - popupmenu objects created using uicontrol does not work as 
                   expected (it is not possible to use the arrow keys on the 
                   keyboard to browse through the various options).

* bug 3645 fixed - SCI/contrib/toolbox_skeleton package does not work out of the
                   box, and crashes on help.

* bug 3652 fixed - wrong computation of generalized eigenvalues.

* bug 3653 fixed - Figure menus were deleted when deleting current axis.

* bug 3661 fixed - cotg and coth do not work.

* bug 3664 fixed - an error occurs when a change directory operation implies a 
                   directory name with french accent letter ('é' for example).

* bug 3667 fixed - Function input() now accepts a \n in the text to be printed 
                   and colon is no more added.

* bug 3673 fixed - When using Scilab embedded in a Java application through 
                   javasci, the first call to javasci API reset the look and 
                   feel of the Java application.

* bug 3681 fixed - Disable automatic completion if there are only spaces in a line.

* bug 3687 fixed - 'format' doesn't check inputs arguments.

* bug 3697 fixed - setdefaultlanguage moved in localization module. (Windows) 

* bug 3700 fixed - Problem when displaying accents in the help browser using the 
                   fr language. (Windows)

* bug 3702 fixed - When I start scilex.exe (Windows) in a windows console, the 
                   windows lost the focus.

* bug 3714 fixed - fprintfMat and fscanfMat cannot cope with directories with 
                   accented characters.

* bug 3745 fixed - "make install" now install all <module>/demos/ 
                   and <module>/examples.

* bug 3749 fixed - Scilab error when building scilab help in HTML.

* bug 3753 fixed - drawaxis produces unreadable labels.

* bug 3755 fixed - print files on some Windows.

* bug 3756 fixed - undocumented input argument for prompt().

* bug 3757 fixed - completion works better on nw mode on Windows.

* bug 3767, 3827 fixed (windows) - Atlas 3.8.2 crashs scilab.

* bug 3769 fixed - Dynamic link doesn't detect intel fortran compiler version 11 
                   on Windows.

* bug 3770 fixed - "qld" returns a error (qld: The constraints are inconsistent).

* bug 3775 fixed - get_absolute_file_path(input_file) returns a error if 
                   input_file is not opened in Scilab.

* bug 3800 fixed - After using surf to plot giving a stacksize error, it is no
                   longer possible to plot in the same window.

* bug 3821 fixed - Problems with uicontrol scrolling when parent figure has 
                   property auto_resize set to "off".

* bug 3822 fixed - When a figure handle is saved and reloaded the figure_size as
                   well as the viewport properties are not saved nor restored.

* bug 3833 fixed - Problems in cell insertion (loss of data).

* bug 3844 fixed - Axes with a very high y-bounds/x-bounds ratio do not appear.

* bug 3860 fixed - Scilab was using the dev name of the fftw library (ie without
                   heading .3 in the lib name). Thanks to Andrea Florio for 
                   providing the patch

* bug 3867 fixed - On the branch 5.0 nightly build 64 bits, the libmatio is
                   defined with the GLIBC_2.7 as mandatory. There is no reason
                   for this and it prevents Scilab binary to run under Redhat
                   and Centos.
                   
* bug 3868 fixed - In the nightly 64 bits build of the branch 5.0, jogl & gluegen
                   dyn lib are 32bits (should be 64 bits).
                  
* bug 3869 fixed - In the binaries, we have some useless files in thirdparty/tk8.5/demos/
                   which are causing some issues with the LSB.
                   
* bug 3870 fixed - In the binaries, we incorporate checkstyle in thirdparty/checkstyle/
                   which are causing some issues with the LSB (btw, it is useless in the binary).
                   
* bug 3873 fixed - While editing ticks'label on Axes, mouse click outside box or
                   double clicks or using Tab key, program terminates.

* bug 3874 fixed - When building the macros, only replaces the extension
                   (.sci => .bin).

* bug 3875 fixed - No images in PDF documentation.

* bug 3877 fixed - No use of interp->result any more in the Tcl interface.
                   Compatible with Tcl/Tk 8.6.

* bug 3878 fixed - Under mandriva, the javahelp package is not called jhall but javahelp2.

* bug 3879 fixed - Under mandriva, the batik package is not called batik but batik-all.

* bug 3880 fixed - Bug with p_margin and g_margin fixed.

* bug 3886 fixed - strsubst with regular expression was working only on the first
                   element of the vector.

* bug 3887 fixed - missing installation of scisparse.h and doublecomplex.h.

* bug 3891 fixed - strsubst() replaces wrong characters with the regexp mode and
                   when the subject contains backslahes. 

* bug 3892 fixed - example about mfprintf added in help.

* bug 3894 fixed - missing information about status -2 returned by mkdir.

* bug 3895 fixed - example about mputl added in help.

* bug 3897 fixed - macros perturbs the behavior of mfprintf().

* bug 3898 fixed - There was no way to check if mputl() has done his job.
					mputl returns %t or %f to check if function has correctly 
					written on the file.

* bug 3899 fixed - The help for "startup" was incorrect.

* bug 3903 fixed - "xdel" of a non existent window crashes Scilab

* bug 3905 fixed - fscanf supports filename and file descriptor as in scilab 4.x.

* bug 3906 fixed - debug(4);exit kills scilab.

* bug 3907 fixed - msprintf with a incorrect format can kill Scilab.

* bug 3912 fixed - strsubst() used with regular expression may return very 
                   stange result.

* bug 3918 fixed - The french documentation fails to build when french locales are not available on the system

* bug 3919 fixed - scilab x64 doesn't display graphics from a remote desktop.

* bug 3924 fixed - I used uicontrol with bad parameters, but uicontrol makes Scilab hangs.

* bug 3931 fixed - fileparts returns invalid values with some special filenames.

* bug 3941 fixed - Error in function minreal when used with state-space linear system.

* bug 3947 fixed - format old syntax reintroduced.

* bug 3950 fixed - Invalid index in kpure function (Example in help page failed).

* bug 3951 fixed - krac2 did not work for rational input (Example in help page failed).

* bug 3967 fixed - When called with a tf, pfss should of course return a tf.

* bug 3987 fixed - The relation between the font_size property and the size in 
                   point is not clearly given in the help pages.

* bug 3988 fixed - The title help page is not properly formatted.

* bug 3989 fixed - xlfont help updated.

* bug 3991 fixed - It is not possible to set a font_style greater than 10 even 
                   if one has added a new font using xlfont.

* bug 3993 fixed - Scilab menu is not located in the "Apple menu bar" like other applications

* bug 3995 fixed - install -D does not work under Mac OS X
                   (used by the make install).

* bug 3996 fixed - tabulation included in a paste string may make scilab crash 
                   in -nw or -nwni mode.

* bug 3999 fixed - xbasc() doesn't clear the current graphics window.

* bug 4005 fixed - Only localization system is used (only native now).

* bug 4006 fixed - xlfont does not return the right index.

* bug 4009 fixed - savematfile crashed when could not open the file for writing
                   (error message added).

* bug 4014 fixed - Wrong positioning in console after its clearing.

* bug 4015 fixed - "Flip left to right" no longer worked on hypermatrices.

* bug 4021 fixed - make distclean is cleaning also some temporary help files.

* bug 4022 fixed - Having the source tree and packaged version of Scilab on the
                   same system caused the system library to be loaded first
                   (should be the source tree first).

* bug 4026 fixed - With the "unix" command under Windows, it is not possible to
                   run an external program which has a long path name.

* bug 4027 fixed - Impossible to create hypermatrices under Linux x64 
                   (actually a bug with int32 values).

* bug 4037 fixed - wavread detects broadcast file format and returns a error 
                   (format not yet managed).

* bug 4046 fixed - core/includes/scisparse.h wasn't installed.

* bug 4055 fixed - From time to time, when I plot a graphic, scilab displays a 
                   blank graphic windows. (Windows)

* Side effect in the setenvc function.

* In the binary, the path to the fortran include directory was wrong.

* Some bashims in the starting script were causing startup issues.

* uimenus and uicontrols callbacks were executed two times when their "enable" 
  property were set to "on" and they were already enabled.

* Docking bugs fixed (problems with menubar, toolbar, infobar updates).

* bug in the script in charge of the dispatch of localization files fixed
  (tools/localization/dispatchLocalizationFile.sh)

Obsolete functions removed :
============================

* mtlb_load()  (Please use loadmatfile() instead).

* mtlb_save()  (Please use savematfile() instead).

* xbasimp() (Please use toprint() for printing and by xs2ps() for the file export instead).

* xg2ps() (Please use xs2ps() instead).

* hidetoolbar() (Please use toolbar(,'off') instead).

* browsehelp() (Please use helpbrowser() instead).

* quapro() (Please use qpsolve() instead).

* %sp_eye() (Please use speye() instead).

* TCL_gcf() (Please use gcf() instead).

* TCL_scf() (Please use scf() instead).

* TK_EvalStr() (Please use TCL_EvalStr() instead).

* TK_GetVar() (Please use TCL_GetVar() instead).

* TK_SetVar() (Please use TCL_SetVar() instead).

* sciGUIhelp() (Please use help() instead).

* demoplay() (Please use scilab_demos() instead).

* buttondialog() (Please use messagebox() instead).

* tk_getvalue() (Please use getvalue() instead).
