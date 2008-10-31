                     Main Changes Scilab 3.0 -> 4.0
                     ==============================

Graphics:
=========

  - Graphical entities (objects) have been extended with a particular effort on:
    + The Axes entity with respect to change of coordinates (logscale enable, 
      axes inversion in 2D and 3D) and graduation display.
    + Versatile Title and labels entities in 2D and 3D.
    + 3D object merge and zoom.
    + Rotation of text entities.
    + Save and load of all graphical entities.

  - New functions have been defined to mimic their Matlab equivalent:
    + plot.
    + surf.
    + mesh.
    + bar, barh and barhomogenize.
    + pie.

  - Graphical Environment improved and extended:
    + Manipulation of the hierachy of the entities has been made easier thanks 
      to a hierachy browser.
    + Annotation edition is now possible with the Insert menu and toolbar. 
      Text, polylines, arrows, rectangles, circles, lines can be added.
    + Copy/Paste/Delete/Move Menus allow to customize graphics and Annotations.

  - Graphic window Events (mouse, keyboard,...) handling have been improved and 
    extended:
    + click, double_click, press, release ,move.
    + key press and release, with or without Shift and Ctrl modifiers.

  - xs2bmp xs2emf functions added to export graphics under bmp and EMF
    (Enhanced Meta File) formats. These functions are only available under
    Windows.

  - Colorbar function added. Thanks to Bruno Pincon.

  - Under Windows a toolbar  has been added to the graphic windows, the 
    function toolbar can be used to set or unset it.


Numerical computation:
======================

  - Sparse operations and functions like real, imag, matrix, spones revisited to 
    improve efficiency. Thanks to Bruno Pincon.

  - Bessel functions extended to work in the complex case (using Slatec routines)
    Incompatibilities: The semantics of besseli, besselj, besselk and bessely 
    functions has been changed and extended.
    The oldbesseli, oldbesselj, oldbesselk and oldbessely correspond to the old
    obsolete semantics.

  - New version of linpro and quapro. Thanks to Cecilia Pola.

  - bvodeS function added to solve differential equation with boundary value. 
    Thanks to Rainer Von Seggern.

  - detrend function added to remove constant, linear or piecewise linear trend 
    from a vector. Thanks to Bruno Pincon.

  - Interface with Excel (Functions to read Excel files).
  


Matlab to Scilab converter:
===========================

  - translatepaths function improved to allow conversion on an entire toolbox 
    agating inference throught toolbox functions.
    
  - The set of translated function has been extented in particular with the 
    basic graphic functions.
    
  - Scilab function sum, prod,... extented to the "first non singleton" matlab
    semantics to improve readability and efficiency of translated code.
    
  - Try catch construct added to Scilab for a better translation.


Scipad editor:
==============

  - A debugging tool is now available.

  - Drag'n'drop is now supported.

  - Split a Scipad window.

  - Print file from Scipad is now available.

  - Scipad is easily localized (See "Adding translations..." in the Scipad Help
    Menu). Today English, German, French, Swedish, Polish, Norwegian and Italian 
    languages are supported.

  - User settings and text colors are now configurable and save across editing 
    sessions.

  - Colorization of strings rewritten - now supports strings on continued lines.

  - Colorization of files launched in the background, with progressbar.

  - Miscellaneous file management improvements: readonly flag, 
    absolute pathnames to files, pruned pathnames display, revert to 
    saved feature, MRU (Most Recently Used) list.

  - Quick access in the file menu for recently opened/saved files.

  - Identification of Scilab predefined variables and library functions in 
    scilab scripts.

  - Keyword completion added, keyword list now completely dynamical.

  - Undo/Redo rewritten.

  - Go to... functions rewritten and expanded.

  - Find/Replace rewritten, includes find files, find in files, find in multiple 
    buffers, find in selection only, find full word.

  - Creation of XML help page templates and xmltohtml compilation available from 
    within Scipad.

Syntax:
=======

  - try-catch instruction added to improve programming with error control.


Other Improvements:
===================

  - Configure adapted to linux 64bit architectures.

  - Use tcltk 8.4.12 - TCL interface has been totally rewritten (for better 
    error detection and better data transfert). ScilabEval improve to handle 
    synchronism.

  - Memory improvements under Windows platforms (particularly the management 
    of virtual memory or swap file).
    
  - Exception management added under Windows version.

  - Windows platforms with:
    + Intel C Compiler 9.0.
    + Intel Fortran 9.0.

  - The source files have been updated to optimise the compiled version built
    with VC6 tool. Please note that the Windows binary version provided on 
    our Web site is built with .NET
    
  - Improvement of the integration of Visual Studio Compiler to the dynamic 
    links: findmsvccompiler() and configure_msvc() macros have been added.

  - Integration of the ATLAS library (specific Windows version).
    During the installation of Scilab, dynamic library (Atlas.dll) is 
    automatically chosen according to the CPU detected.
    See details in the Atlas.spec file under scilab\bin directory.
    
    Scilab can be used with Atlas library on all platforms by:
    
    + downloading Atlas library sources.
    + compiling Atlas sources.
    + compiling scilab sources with the following configure option : 
         --with-atlas-library=ATLAS_LIB_DIRECTORY
         
      (see <SCI>/configure --help).
    
  - Java interface written to allow calling Scilab computational engine from Java.

  - Font rotation is now possible under Xwindow.

  - SCI and HOME are redefined if not correct ( Bug 1488 Only for Windows ).

  - HOME variable (%HOME%) defined under Windows: c:/Documents and Setting/USER.

  - New environment variable SCIHOME:
    + Under Windows: %HOME%\scilab\SCILAB_VERSION
    + Under Linux: $HOME/USER/.scilab/SCILAB_VERSION

  - Function what() has been rewritten : alphabetical sort, print modified.

  - mkdir, rmdir, copyfile, toprint functions added for handling files and
    directories from Scilab.

  - getshortpathname, getlongpathname, winqueryreg (Registers), console, 
    mcisendstring (MCI device), clipboard, functions added for a better 
    interaction with Windows environment.

  - getos, setenv, getmemory, sleep, functions added for a better interaction 
    with the operating system (Unix and Windows).

  - The perl function as been added to allow execution of Perl scripts from 
    Scilab.

  - The calendar function has been added.


New primitives:
===============

  - clipboard (specific Windows)
  - perl
  - calendar
  - mkdir
  - rmdir
  - copyfile
  - sleep
  - getos
  - setenv
  - toprint("file")
  - getmemory()
  - getshortpathname (specific Windows)
  - getlongpathname (specific Windows)
  - toolbar (specific Windows)
  - hidetoolbar (specific Windows)
  - console (specific Windows)
  - mcisendstring (specific Windows)
  - banner
  - winqueryreg (specific Windows)
  - xls_open
  - xls_read
  - scicos_debug_count


Please note:
============

  - "xgetmouse", "eventhandler" event set extended to click, double-click, <Ctrl> Key. 
    See more details in xclick and xgetmouse on line help.
    Warning: Because of distinction of click events, some users' scripts can 
    have a different behavior.
    To make it work as before please check and modified test on events as follow:
    supposing that button contains the value returned by xclick (left most argument) 
    or xgetmouse (third componant of returned vector) then replace test like:
      button==0 by or(button==[0,3])
      button==1 by or(button==[1,4])
      button==2 by or(button==[2,5])


