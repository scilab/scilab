               Changes between version 5.2.0-beta1 and 5.2.0 of Scilab
               ==========================================================


Text editor:
============

* Mnemonic added on View ("V") and Document ("D") in the menu bar

* Refactoring of some large part of the source code

* Thanks to right button click, a popup menu displays the following features:
- Execute in Scilab
- Edit selection into a new text editor
- Copy
- Select all
- History
- Help on the selected text
See bug #5387

* bug 4873 fixed - "undo" did not work if the last action affected several lines

* bug 4883 fixed - "Menu->Document->Auto Indent" preference was not saved

* bug 4886 fixed - Performance enhancements for colorization
  (notable speed-up on opening large scripts)

* bug 5013 fixed - Find/Replace dialog box: Buttons were resized after the search

* bug 5103 fixed - File=>Save now opens the "Save" dialog box for
                   Untitled files even if they are not modified

* bug 5125 fixed - An infinite loop is no more got when using the 
                   Undo functionnality

* bug 5137 fixed - Drag & drop on editor was not enabled

* bug 5140 fixed - "Load into Scilab" was executed line by line

* bug 5171 fixed - Problem with the highlight in the undo manager 

* bug 5173 fixed - Better multi-thread correctness wrt Event Dispatching 
  Thread
  
* bug 5174 fixed - The undo did not work on tab

* bug 5189 fixed - No more error when a file selection
                   for saving is cancelled

* bug 5219 fixed - Rename of the item menu "Indent" => "Reformat Selection"

* bug 5220 fixed - In editor, "Colorize" action in "Document" menu is now 
                   implemented as check box

* bug 5224 fixed - Maj+Tab modified the text selection

* bug 5270 fixed - In editor, shortcut for the line numbering (CTRL+L) changed
                   and assigned to "Load into Scilab" in order to match the old
                   behavior

* bug 5290 fixed - Function editorHighlightLine removed, and editor updated in order
                   to have in argument files to open and lines to be highlighted
                   for each files

* bug 5308 fixed - Editor did not check input argument type

* bug 5317 fixed - Inserting a tab inside a line tabified the whole line instead
                   of only its queue

* bug 5318 fixed - When a "if" line took its "end" on the line, its indentation
                   was one level too high 

* bug 5319 fixed - Default filename proposed in 'Save As' dialog

* bug 5341 fixed - "Save All" functionality was missing

* bug 5386 fixed - Wrong title for auto indent action in menu

* bug 5460 fixed - 'Close All' and 'Close all but this' added menus in editor

* bug 5465 fixed - Better Undo/Redo support (works across "Save"/ "Save as..", 
                   undo / redo multiple edits on a region in one go)

* bug 5482 fixed - It was not possible to choose EOL type

* bug 5497 fixed - Selection drag was not available

* bug 5515 fixed - Encoding was not well managed
                   Now:
                   - default encoding is the system default one
                   - user defined encoding save in Editor configuration file

* bug 5519 fixed - Message asking the user if he wants to exit the 
                   editor when he clicks on the cross added



Help Browser:
=============

* Thanks to right button click, a popup menu displays the following features:
- Execute in Scilab
- Edit in the text editor
- Copy
- Select all
- History
- Help on the selected text

* In the console and the text editor, in the menu associated to the right button
click, the item "Help on the selected text" has been added.


Text Rendering:
===============

* New function "prettyprint" added
  This function provides the capability to represent a Scilab variable to LaTeX,
  TeX and Mathml. 
  See SEP #36 for more information

* pol2tex is now obsolete. Please use prettyprint instead

* texprint is now obsolete. Please use prettyprint instead

* bug 5303 fixed - pol2tex() could not be used straightforwardly with the new 
                   "$...$" LaTeX feature.

* bug 5304 fixed - texprint() could not be used with the new "$...$" LaTeX
                   feature.

ATOMS:
======

* An "offline" mode has been added. This mode allows the user to install modules
  from a local repository or a local package even if the network is unreachable :
  --> atomsSetConfig('offLine','True');
  --> atomsInstall("/home/johnsmith/module_lycee_1.1.bin.i686.linux.tar.gz");

Graphic Export:
===============

Refactoring of the graphic export for vectorial files (PDF, EPS, PS & SVG)
through FOP/Batik:

* Improved quality of the vectorial export

* Improved size of the PDF (PS & EPS are however bigger but this should be 
  fixed in Scilab > 5.2.0)

* Handles export of graphics with LaTeX/MathML text

* bug 4200 fixed - Export PDF was causing a "bad /BBox" with Acrobat


Localization:
=============

* Some strings were untranslatable/untranslated (Bug #4124)

* Some typos fixed


Documentation Improvements:
===========================

* More examples with LaTeX use-cases (colors, etc)


Compilation:
============

* Rebuild some C++/Java wrappers with a newest version of SWIG

* In some cases under Windows, the detection of libjogl.* could fail
  because of a missing explicit link against libGL.*


Xcos:
=====

* Icon added in the main Scilab tool bar

* bug 4997 fixed - Now Scilab editor and Xcos ask to save unsaved documents
                   when user closes Scilab

* bug 5026 fixed - Now palette browser appears only with a new diagram or by the
                   menu View->Palette browser

* bug 5052 fixed - The context variables defined at an upper level were not known
                   at a lower one

* bug 5053 fixed - Layout of in/out port of a block when modified, updated

* bug 5054 fixed - Remove ports on a block remove the connected links

* bug 5083 fixed - If lines are selected and user call the "Find/replace" dialog
                   box, the radiobox "Selected lines" in the frame "Scope" is
                   checked by default

* bug 5099 fixed - Diagram was not visible after a "Fit diagram to view"

* bug 5104 fixed - The block view was not updated on undo/redo

* bug 5111 fixed - "Save As" default path was not the current file path

* bug 5158 fixed - Region to SuperBlock now keeps links breaks inside and outside
                   SuperBlock

* bug 5159 fixed - The region to superblock action could not be undone/redone

* bug 5167 fixed - xcos*.h5 and xcos*.h5 were not deleted

* bug 5179 fixed - The accented letters used in text in Scicos diagram were not 
                   visible with Xcos at least under Linux

* bug 5180 fixed - TEXT_f was not selectable on the palette

* bug 5182 fixed - In some cases, the main diagram dialog no more responded
                   neither to the "validate" nor "cancel" menu

* bug 5183 fixed - Simulation menu was not disabled on a superblock diagram

* bug 5184 fixed - Setup menu values were not localized

* bug 5188 fixed - A function to reopen a parent diagram was added in the pop up
                   menu of the diagram

* bug 5196 fixed - Opening a SuperBlock modified some positions

* bug 5202 fixed - The description of a module is now scrollable

* bug 5209 fixed - Bad French translation for "Optimisation and Simulation"

* bug 5218 fixed - Uicontrols of style 'text' are now scrollable

* bug 5221 fixed - libhdf5 is defined with the GLIBC_2.7 as mandatory

* bug 5235 fixed - Error message added when launching Scicos

* bug 5267 fixed - Scilab exited when closing Xcos setup window using the upper
                   right cross

* bug 5281 fixed - The menu "View->"Palette Browser" is correctly checked at Xcos
                   start

* bug 5299 fixed - When saving a Xcos diagram, when the selected file was
                   already existing and did not contain the .xcos extension
                   (~/foo), the 'Replace file' dialog appeared but did not
                   overwrite the file

* bug 5301 fixed - When a superblock was created with the region to superblock,
                   selected internal links were not included into the
                   superblocks but were also not considered as IN_f OUT_F

* bug 5305 fixed - Display of filenames used by Xcos was wrong (under Windows)

* bug 5310 fixed - Closing a Xcos diagram closed all of Xcos

* bug 5312 fixed - 'Compile' information was not displayed

* bug 5313 fixed - Clicking repeatedly and quickly on a block in a diagram
                   made Scilab quit

* bug 5327 fixed - When indenting a block, the selection area was shifted one 
                   line down with respect to the previously selected one

* bug 5328 fixed - The RegionToSuperblock action produced a poorly rendered 
                   diagram

* bug 5336 fixed - Xcos palette tabs now load much faster and use less
                   memory

* bug 5343 fixed - The DLSS_f block was transparent when dropped in the working area

* bug 5344 fixed - Status of a modified file did not change on 'Save As'

* bug 5347 fixed - The 'Region to superblock' action crashed Xcos 

* bug 5348 fixed - Opening a Superblock crashed Xcos

* bug 5349 fixed - The file predator.cos could not be opened

* bug 5351 fixed - Remove ports on a block remove the connected links 

* bug 5353 fixed - Bad French localization for "set context"

* bug 5383 fixed - FROMWSB from workspace block was not working 

* bug 5388 fixed - Editor dialogs was not centered on editor main window

* bug 5389 fixed - If there was no C compiler detected on Windows, Xcos 
                   tried to call dynamic link functions

* bug 5395 fixed - The Xcos block AFFICH_m did not bother with the max
                   number of digits

* bug 5397 fixed - If there was a unconnected block in a superblock, the 
                   simulation did not start and the "Simulate/start" menu
                   remained disabled

* bug 5403 fixed - A graphic window is created only when needed

* bug 5404 fixed - Open & close Editor created a java exception

* bug 5405 fixed - The whole diagram is now updated when a superblock 
                   is modified

* bug 5407 fixed - It was impossible to use the arrow keys to move a block

* bug 5410 fixed - Html4.0 specific symbols were used

* bug 5412 fixed - Context modifications were not saved for Superblocks

* bug 5416 fixed - Blocks modifications were not saved for Superblocks

* bug 5417 fixed - AUTOMAT and PDE blocks were in demo blocks

* bug 5418 fixed - DEBUG_SCICOS block was not in the palette

* bug 5419 fixed - French translation issues

* bug 5422 fixed - A file was not a unique diagram instance (opened twice)

* bug 5423 fixed - French translation issues

* bug 5424 fixed - French translation issues

* bug 5426 fixed - The icon of modelica blocks Resistor Inductor was not 
                   rotating while the block was

* bug 5428 fixed - Connecting Implicit I/O port to Link was not allowed

* bug 5437 fixed - Show error message during Xcos simulation initialization

* bug 5440 fixed - OUT* blocks were not in the sinks palette 

* bug 5441 fixed - Xcos files (XML files) contained their own paths

* bug 5442 fixed - Undo a Mirror Action did not work as expected

* bug 5444 fixed - When the Xcos compiler failed to find the size of the data
                   on some links it asked user for it into a bad window

* bug 5445 fixed - The modified state was not updated on undo/redo

* bug 5449 fixed - The palette tab position was modified even if invisible

* bug 5450 fixed - The Signal Builder demos did not work
                   (xcos/demos/Signal_Builder.xcos does not exist)

* bug 5459 fixed - Editor "block" Scilab with example

* bug 5464 fixed - Actions were not disabled on file loading

* bug 5474 fixed - The demo Standard demos --> Scilab block was not working

* bug 5475 fixed - drag & drop .xcos file on console did not open Xcos diagram

* bug 5480 fixed - When creating a Superblock, input and output links were removed
                   and input and output ports were not created

* bug 5528 fixed - ENDBLK and ENC_c blocks (Sinks palette) did not have icons

* bug 5529 fixed - The parent diagram was not updated when performing undo/redo
                   on a SuperBlockDiagram

* bug 5530 fixed - IN_f, OUT_f and similars blocks did not accept expression as
                   parameter

* bug 5531 fixed - Setup values were not strongly typed (use of spinner)

* bug 5533 fixed - SetupAction did not valid inputs

* bug 5592 fixed - The Xcos startup operations were performed on the Scilab
                   execution thread

* bug 5594 fixed - The links did not move on arrow key typing

* bug 5598 fixed - The CLKOUT_f block was not an EventOutBlock

* bug 5634 fixed - The export functionnality did not work.

Tree functions :
================

* Rename tree functions:
  - insertNode      => uiInsertNode
  - getParentNode   => uiGetParentNode
  - getNodePosition => uiGetNodePosition
  - getChildrenNode => uiGetChildrenNode
  - findNode        => uiFindNode
  - equalsTree      => uiEqualsTree
  - dumpTree        => uiDumpTree
  - deleteNode      => uiDeleteNode
  - createTree      => uiCreateTree
  - createNode      => uiCreateNode
  - concatTree      => uiConcatTree
  - displaytree     => uiDisplayTree

* bug 5170 fixed - displaytree() name was not homogeneous with other 
                   tree functions.

Bug fixes:
==========
* bug 2588 fixed - Calling a function with no input argument contained in an 
                   mlist generated an error

* bug 3200 fixed - The add_help_chapter function returned a boolean

* bug 3478 fixed - file function returned a wrong error message when file did
                   not exist

* bug 3593 fixed - Macros to read and write csv file format updated

* bug 3675 fixed - Issues when saving/loading gui (uicontrols,figure...) fixed

* bug 3849 fixed - Path selected for graphics export is now memorized and used
                   as default for next export file selection

* bug 4401 fixed - isnum returned wrong values

* bug 4435 fixed - bench_run is now documented

* bug 4565 fixed - getvalue macro was defined twice

* bug 4670 fixed - ilib_build examples updated with api_scilab

* bug 4767 fixed - Typo in minreal was causing errors in some cases

* bug 4844 fixed - The help page for the deletefile function did not specify 
                   what was the meaning of the output variable

* bug 5025 fixed - The Tabbed palette manager became a Tree palette
                   manager

* bug 5037 fixed - The link style menu did not have mnemonics

* bug 5077 fixed - demo neldermead/output Command failed under Windows (Release)

* bug 5106 fixed - mopen help page did not contain any example

* bug 5107 fixed - mgetstr help page did not contain any example

* bug 5163 fixed - Incorrectly resized text after having performed
                   magnification+demagnification

* bug 5190 fixed - Context menus are now available under MacOS

* bug 5195 fixed - "get" crashed with "Lycee" module

* bug 5200 fixed - Adding a code generation error message when not applicable

* bug 5211 fixed - A non reg test was failing with the binary of Scilab

* bug 5213 fixed - ATOMS GUI: Module's author(s) was not displayed

* bug 5214 fixed - ATOMS GUI: The download size was not displayed

* bug 5233 fixed - The Windows installer bugged in some cases

* bug 5237 fixed - ATOMS: The load of an ATOMS module could produce an error.
                   The error is now catched.

* bug 5249 fixed - Using the help or apropos functions, some words (stop word) 
                   were not taken into account during the search

* bug 5251 fixed - getversion help page updated

* bug 5258 fixed - diary help page had some erroneous timestamp prefix formats

* bug 5259 fixed - root_properties help page displayed mis-encoded characters

* bug 5263 fixed - diary returned incorrect path for automatically numbered
                   filenames

* bug 5266 fixed - Modelica demos made available in MacOS binary version

* bug 5271 fixed - ATOMS GUI: Frames title was not emphasized

* bug 5283 fixed - Intel Fortran 11 compiler did not require to import some
                   settings in Visual Studio

* bug 5284 fixed - Function "legend" drew line segments in its box in a
                   reverse order

* bug 5286 fixed - Calling the 'lib' function with the wrong path made Scilab
                   hung

* bug 5292 fixed - There was a conflict between bool and Scilab header mex.h
                   (with Visual Studio and mex files)

* bug 5293 fixed - Undocumented features with function string(x)

* bug 5294 fixed - In mex.h, we had a bad type declaration

* bug 5295 fixed - configure: LibGL.so included at link time when
                   linking with libjogl.so

* bug 5298 fixed - Obsolete Makefile in the mexlib module

* bug 5307 fixed - In Neldermead, inconsistent shape for the input argument x
                   of the cost function

* bug 5325 fixed - Wrong default path on FileChooser

* bug 5326 fixed - graphic_export module was not loaded dynamically

* bug 5330 fixed - "help str1 str2" returned an error

* bug 5331 fixed - There was no example in the help page of matfile_* 
                   loadmatfile or savematfile

* bug 5339 fixed - Some macros had multiple definitions

* bug 5340 fixed - There was no example in the help of arma 

* bug 5355 fixed - There was no automatic indentation in the editor when 
                   commands such as "try" or "catch" were entered

* bug 5359 fixed - ATOMS: Path of the non-existing loader file was not well
                   displayed at Scilab startup

* bug 5360 fixed - "sciargs" documentation help page updated

* bug 5367 fixed - Incorrect %nan multiplication with vectors on 64-bit Vista

* bug 5368 fixed - input() behaved weird on carriage return response to prompt
                   on 64-bit

* bug 5373 fixed - isalphanum() did not support non ascii chars

* bug 5375 fixed - isdigit() did not support non ascii chars

* bug 5376 fixed - tool_skeleton.iss had not been updated

* bug 5384 fixed - get_absolute_file_path was case sensitive with filename on 
                   Windows

* bug 5391 fixed - Bad warning/error messages in ATOMS

* bug 5402 fixed - Update status bar contents after saving canceled

* bug 5413 fixed - help_from_sci: An error was triggered when < or > where used
                   in the comments

* bug 5431 fixed - When network is unreachable, ATOMS now displays an explicit 
                   warning and return softly

* bug 5433 fixed - LCC-Win32 did not work with Scicos

* bug 5452 fixed - legend reversed the order of plotted curves

* bug 5453 fixed - ATOMS: Sizes were displayed with French units

* bug 5456 fixed - TCL_UpVar(source,alias) returns %T even if the TCL variable
                   source did not exist and then aliasing had not been 
                   performed

* bug 5457 fixed - Scilab could not start with Korean Win XP

* bug 5460 fixed - Added menus 'Close All' and 'Close all but this' in editor

* bug 5461 fixed - The savematfile did not work when a format argument was given

* bug 5462 fixed - savematfile() did not save correctly sparse matrices
                   
* bug 5469 fixed - Force origin presence on axes with location property set 
                   to 'origin'

* bug 5470 fixed - The addcolor function help page did not contain example

* bug 5484 fixed - LCC-Win32 failed to generate makefile.lcc with this example

* bug 5487 fixed - help_from_sci was taking the first line of code as author
                   when no empty line was provided

* bug 5488 fixed - When the documentation was extracted from a .sci file using
                   help_from_sci function, the docbook refsynopsisdiv section
                   was not correct

* bug 5489 fixed - When the documentation was extracted from a .sci file using
                   help_from_sci function, the docbook Author refsection
                   section was not correct

* bug 5494 fixed - On Windows 2000, stacksize with the argument 'max' set 
                   stack to minimum size if it failed

* bug 5499 fixed - tbx_build_loader() automatically added a test about
                   version in the loader.sce even if the module has been 100% 
                   written in Scilab code

* bug 5505 fixed - getdebuginfo() output in French was partially translated,
                   with few errors

* bug 5506 fixed - There were some memory leak with getMatrixOfWideString

* bug 5507 fixed - whereis(bin2dec) returned an error

* bug 5511 fixed - printf_conversion help page was not clear

* bug 5513 fixed - input("message") did not restore the original prompt after
                   the input job was done

* bug 5532 fixed - added a "Save All" menu to save all modified files by editor

* bug 5546 fixed - Initialization trouble of the variable "compilerpath"

* bug 5577 fixed - help_skeleton macros encoded the generated xml file in 
                   ISO-8859-1 and not in UTF-8
 
* bug 5585 fixed - sysdiag(): Both examples were duplicated

* bug 5586 fixed - getscilabkeyworgs was broken

* bug 5604 fixed - Fminsearch: optimsimplex_new did not check for consistency 
                   of x0 and len for "axes" simplex

* bug 5607 fixed - 'mtlb_mode' did not work on Windows

* bug 5615 fixed - xls_open failed if there was a trailing space after the 
                   file extension

* bug 5619 fixed - bitset() could not set hypermatrix bits

* bug 5620 fixed - dec2bin() could not convert hypermatrices

               Changes between version 5.1.1 and 5.2.0-beta1 of Scilab
               =======================================================


Xcos:
=====

Xcos is developed simultaneously and together with Scilab, and replaces Scicos
in Scilab distributions. With a new ergonomic and efficient block diagrams 
editor, Xcos provides functionalities for modeling mechanical systems and control
systems, and is particularly suitable for industrial and academic needs.

This new version provides numerous new features:
* Several diagrams can be edited at the same time
* Scilab can be used during the diagram construction
* Diagram edition is available even during simulation
* Drag and drop from palettes to diagram and between diagrams
* History (Undo/Redo) with an unlimited number of actions memorized 
* Viewport to have an overall view of your diagram
* Palette window with new palettes organization
* Improved stability
* Better usability of menus
* Tool tips on block
* Big diagram managements
* Better integration of the demonstrations
* Standard shortcuts (Copy/Paste/Undo/Redo/...)

ATOMS:
======

ATOMS is the packaging system for Scilab's modules. Thanks to ATOMS, any user
running Scilab on a supported operating system is able to
install any modules from Scilab or third-party repositories with only three 
clicks or with just one Scilab command.

For example, the following command will install plotlib:
atomsInstall("plotlib")
This command will:
* download the binary package of plotlib from the Scilab repository
* unpack the package
* configure the package
* load the package

Many packages are already available and more are going to be offered
on Scilab's repositories.

This system is fully open to contributors. Anyone can manage his own toolbox
on Scilab ATOMS website (http://atoms.scilab.org) or run his own 
repository.

Note that ATOMS website replaces the current toolboxes/modules web interface.


Text Editor:
============

A new text editor is implemented in this version of Scilab. The editor now
follows the look and feel of Scilab and is integrated into the docking system.

The following features are available:
* Syntax colorization
* Automatic indentation
* Line numbering
* Execute into Scilab
* Comment/uncomment of blocks
* Go to line

Graphics:
=========

* Text rendering with LaTeX / MathML

The annotation of Scilab's graphics in MathML or LaTeX is now possible. 
It enables the integration of mathematical equations, greek variables, matrix 
displays... 
This is available in any cases where text can be set in the graphic (label, 
tick, title, xstring...).
For example:
x = 1:0.25:10;y1 = 1 + sin(4*%pi*x / 10);plot(x,y1,'k-');
xstring(6,1.4,'$\leftarrow f \left( x \right) = 1 + \sin \left( \frac{x \cdot 4 \cdot \pi}{10} \right)$');
See demos => Graphics => LaTeX/MathML
Thanks to Calixte Denizet for this feature and the underlying API for LaTeX rendering (project JLaTeXMath)

* Antialiasing on graphics
  This new feature improves the quality of graphic rendering.
  It can be activated with the following commands: 
  df = gdf(); df.anti_aliasing = "8x"; plot3d();
  The rational and performance tests are available in SEP #16 or can be
  seen in Scilab bugtracker as bug #2984.

* Add transparency to markdrawing.

* 'xbasc' is obsolete. See 'clf'

Introduction of new binaries:
=============================

The goal of these changes is to improve the capabilities to use Scilab
in different environments and use cases.

* Scilab 5.2 introduces new binaries in the distribution:

  - classical - The GUI application (equivalent to Scilab Standard Mode).
    This program is exactly the same as in the previous version and all arguments
    are still available
    Name: scilab

  - Advanced Command-Line Interpreter (ACLI) - Interpreter in command line with the graphic capabilities available 
    (equivalent to Scilab NW Mode)
    Name: scilab-adv-cli

  - Command-Line Interpreter (CLI) - Interpreter in command line without the graphic or Tcl/Tk capabilities (faster)
    (equivalent to Scilab NWNI Mode)
    Name: scilab-cli

  See SEP #18 for more details

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


Extension capabilities of Scilab (API Scilab):
==============================================

In the previous versions of Scilab, there was no clear or easy way to extend
Scilab or to use it as an embedded application. 
Knowing these drawbacks, Scilab 5.2 tackles all those issues by providing a new
consistent, documented and easy way to use API. 

* A new API called api_scilab is available. This API provides functions to
  read/write data from/to Scilab memory.
  It provides many advantages:
  - management of all Scilab data types
  - consistency over all data types
  - error management
  - fully documented
  - fully tested by unitary tests
  - and it will be maintained over future versions of Scilab

  Note that all other Scilab API (straight access to the Scilab stack, stack1,
  stack2 and stack3) will be maintained in the Scilab 5.X family but most of
  them will be dropped in the 6 family.
  Do not hesitate to submit a bug report if you would like to see new features
  and functions in Scilab API.

* In the enum sci_types which lists all Scilab data types, the type 
  "sci_lufact_pointer" has been renamed "sci_pointer".
  A define of "sci_lufact_pointer" on "sci_pointer" has been added to backward
  compatibilities.
  The rationale is to improve the use in special case of the pointer data type
  to avoid memory duplication.

* Under all operating systems but Windows, all headers are not installed in a single directory 
  in order to simplify the use of Scilab features in the dynamic link.
  See SEP #32 for more information

* From this version of Scilab, only a defined and documented number of headers
  are installed. 
  Thanks to this change, a module/toolbox developer knows exactly what are the
  available functions provided by Scilab.
  See SEP #35. 
  Do not hesitate to contact us if you have been using a Scilab header which has been removed from Scilab 5.2.


call_scilab module:
===================

Call Scilab is the API which allows Scilab to be called as computing engine or
embedded into a third-party application.
From this version, Call Scilab is based on Scilab API and handles all Scilab
data types (in previous versions, Double, Boolean, Complex Matrices and String 
elements were managed).
The Call Scilab function has the same profile as Scilab API with 'Named' into 
the function name.
 
* CallScilab.h renamed call_scilab.h (See SEP #34). CallScilab.h is still 
  available and points to call_scilab.h

* Documentation and examples about call_scilab added (BooleanManagement call_scilab
  compile_and_run_call_scilab DisableInteractiveMode GetLastJob DoubleManagement
  ScilabHaveAGraph SendScilabJobs SendScilabJob StartScilab StringManagement
  TerminateScilab)

* Some function profiles in stack3.h provided to manage named variable
  have been moved in CallScilab.h (please note that we have not changed the ABI
  and the symbols remain in the core library)
  The goal of this modification is to separate functions dealing with the
  call_scilab API and the internal API used in the gateways.
  Note that all moved functions are considered as obsolete.

* In call_scilab, all previous functions available to read/write data from/to
  the Scilab engine are now obsolete but supported until Scilab 6:

  * C2F(readmat) is obsolete. Please use readNamedMatrix in api_double.h
  * C2F(readchain) is obsolete. Please use readNamedMatrixOfString or readNamedMatrixOfWideString in api_string.h
  * C2F(matptr) is obsolete. Please use readNamedMatrixOfDouble in api_double.h
  * C2F(creadmat) is obsolete. Please use readNamedMatrix in api_double.h
  * C2F(creadbmat) is obsolete. Please use readNamedMatrixOfBoolean in api_boolean.h
  * C2F(creadcmat) is obsolete. Please use readNamedComplexMatrixOfDouble in api_double.h
  * C2F(creadchain) is obsolete. Please use readNamedMatrixOfString or readNamedMatrixOfWideString in api_string.h
  * C2F(creadchains) is obsolete. Please use readNamedMatrixOfString or readNamedMatrixOfWideString in api_string.h in api_string.h
  * C2F(cmatptr) is obsolete. Please use readNamedMatrixOfDouble in api_double.h
  * C2F(cmatcptr) is obsolete. Please use readNamedComplexMatrixOfDouble in api_double.h
  * C2F(cmatsptr) is obsolete. Please use createNamedMatrixOfString or createNamedMatrixOfWideString in api_string.h
  * C2F(cmatbptr) is obsolete. Please use createNamedMatrixOfBoolean in api_boolean.h
  * C2F(cwritemat) is obsolete. Please use createNamedMatrixOfDouble in api_double.h
  * C2F(cwritecmat) is obsolete. Please use createNamedComplexMatrixOfDouble in api_double.h
  * C2F(cwritechain) is obsolete. Please use createNamedMatrixOfString or createNamedMatrixOfWideString in api_string.h
  * C2F(cwritebmat) is obsolete. Please use createNamedMatrixOfBoolean in api_boolean.h

* "make check" now also builds call_scilab examples

* An example to call Scilab from C# added.
   see SCI/modules/call_scilab/examples/call_scilab/NET/C# directory

* An example to call Scilab from VB.NET added.
   see SCI/modules/call_scilab/examples/call_scilab/NET/VB.NET directory

Option configuration:
=====================

* The file jvm_options.xml has been moved into etc/jvm_options.xml in order to
facilitate the use of this configuration file.


Mac OS X:
=========

* Support of Snow Leopard

* Docking system enabled

* bug 4087 fixed - Under Mac OS X, an Apple + C shortcut (Ctrl + C) did not work from the console.

* bug 4508 fixed - When saving/exporting a figure under
                   Mac OS X, a file could not be created.

* bug 4393 fixed - When launching Scilab from the puffin icon, it did not detect the locale and switched to English.

* bug 4949 fixed - Using Xcos/Scicos in the source tree environment, with
                   schema using sundials library, the application had problems
                   to load the dynamic library.

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
     
* 'sethomedirectory' is obsolete. See 'home'.

* 'getf' is obsolete. See 'exec'.

* toolboxes is obsolete.

fileio functions:
=================

* Input arguments of functions are now checked 
    (mclearerr, mclose, merror, mget, mgetstr, meof, mopen, mput, mputstr, mseek, mtell)
    
* fileinfo manages column vector of strings (SEP #25)
  
* basename uses fileparts
  
* pathconvert: If the flagtrail argument is set to FALSE and the trailing separator
                of the path is removed (if present of course). See Bug 4468.
                  
* 'getcwd' is obsolete. See 'pwd'. 

* isfile: checks if filename is an existing file.

* movefile: moves files or directory (Request 661)

* Rewrite:
   + copyfile (primitive)
   + fileparts (primitive)
   + isdir manages matrix of strings as input argument
   + basename (primitive)
   + pathconvert (primitive)
   + cd, chdir (primitive)
  
Dynamic link:
=============

* Windows: temporary files (objects) are generated in an intermediate subdirectory (Release or Debug)
   ilib_for_link and ilib_gen_Make (with Visual studio) use a same makefile template.
   (SCI/modules/dynamic_link/src/scripts/TEMPLATE_MAKEFILE.VC)
  
* ilib_for_link and ilib_gen_Make with lcc-win32 use a same makefile template.
   (SCI/modules/dynamic_link/src/scripts/TEMPLATE_MAKEFILE.LCC)

* ilib_verbose (SEP #17) Set level of display returned by dynamic link functions.
 
* add an example to call a Scilab macro from a C interface
   (see SCI/modules/dynamic_link/examples/interfaces/call_scilab_function directory)
   
* a 'cleaner.sce' file is generated by 'ilib_for_link' and 'ilib_build'.
 
* addinter: each interface added can manage 999 functions (see bug 4675). 

* ilib_gen_gateway can generate gateway without PutLhsVar (Managed by user, in this case)
  see help of 'ilib_gen_gateway' about this modification and 
  SCI/contrib/toolbox_skeleton/sci_gateway/c/builder_gateway_c.sce (example)

* ilib_gen_loader now generates loaders which can be executed outside the loader's directory

* bug 4726 fixed - Under Windows, when the dynamic link was copying the files,
  in some cases, it was going to copy backup files (for example, sci_luinc.c.bak
  instead of sci_luinc.c)

* bug 4919 fixed - If the source file was stored in TMPDIR, the dynamic link
  was removing the content of this file.

Online Help:
============

* Sub-chapters are now allowed (See SEP #24)

* xmltochm exports help to chm format (Windows)

* add_help_chapter now accepts string array as input argument: consequently,
   several online help chapters can be added at the same time.

* add_help_chapter now accepts to add help chapters with the same title.

* In the documentation, the font size of the equations has been increased 
  for a better readability.

* Javasci FAQ page added

* Online help XML files have been converted into UTF-8
 

Source reorganization:
======================

* Makefile.incl.mak (Windows, dynamic link) moved from SCI directory to
  SCI/modules/src/scripts
  
* newest, get_absolute_file_path, getrelativefilename moved from io to fileio.

* bug 3668 fixed - formatnumber.f, dmdsp.f, ... moved in output_stream module.

* printf, msprintf, mprintf, print moved in output_stream module.

* disp moved in output_stream module.

* bug 3668 fixed - lib, deff, getf, exec, execstr moved in functions module.

* oldsave, oldload functions (only used with macros of Scilab 2.4) are removed
  Replaces oldsave, oldload by save and load in your code.
  
* bug 3669 fixed - integrate, intc and intl moved in Differential equations
  interp1, interp2d and splin2d  moved in the Interpolation module.

* libs/MALLOC/src/malloc.c is no longer build (used to be built for 64 
bits archs). Note that if Scilab is built on a 64 bits and triggers a 
seg fault on startup, uncomment the related line in 
libs/MALLOC/Makefile.am and launch autoreconf.


Compilation:
============

* Visual studio uses default runtime library /MD (msvcrt.lib) to build all libraries of Scilab.

* Scilab on Windows built with Intel Fortran 11.0.061 and Intel C++ 11.0.074

* Symbols in dlls are explicitly exported on Windows.
  In previous versions of Scilab all symbols were exported.

* modelicac (Xcos/Scicos) built with Ocaml 3.11.0 on Windows

* blas, lapack, arpack libraries moved as pre-requirements (Windows)

* blas, lapack libraries moved as pre-requirements (Linux/Unix/Mac OS X)

* lapack 3.2 used on Windows

* functions module is dynamically loaded by Scilab.

* autoconf updated to version 2.64

* automake updated to version 1.11

* libtool updated from version 1.5.26 to 2.2.7a (Closes bug #4279) with some
  patches for Mac OS X/Darwin support

* --enable-debug-java add two options to the JVM args (-verbose:jni and
  -Xcheck:jni) in order to provide more feedbacks on debug
  Note that this should not be used into a binary since it is expensive

* C++ => Java connector updated (using GIWS - 
  http://www.scilab.org/giws/ version 1.0.5)
  There was a bug in GIWS:
  The use of a static method returning an int was not calling the right
  method.
  It did not seem to trigger any errors in the past but the latest 
  version of the JDK is now complaining.


* Patches applied (see bugs #4506, #4517 & #4588) by Dan McMahill. They fix 
  some compile time problems under NetBSD which also improve all operating systems but Windows.
    - adds some missing $host to the 64 bit list.  
    - adds a needed compiler flag for alpha-*-netbsd
    - adds autoconf check for term.h header
    - adds autoconf checks for gettext
    - adds autoconf check for dlopen in -ldl 
      (on some systems, we do not need -ldl because dlopen is in libc).  
      Do not hardcode linking with -ldl but rely on this new check.
    - adds correct getrlimit() argument for NetBSD and DragonFly
    - avoids pulling in an OSF1 specific header on NetBSD/alpha
    - adds missing newline at the end of a file
    - adds missing XML LIBS and CFLAGS
    - adds LTLIBINTL to the list of common objects needed for linking the top level binary.
    - adds checks for term.h and termcap.h headers.  
    - adds check for -lpthread
    - reworks the checks for curses a little bit to search for 
      curses.h and -lcurses first followed by ncurses.h and -lncurses.   
    - uses configure results to protect the inclusion of curses.h, 
      ncurses.h, term.h, and termcap.h.
    - adds __DragonFly__ to a few conditional lines.
    - removes the static declaration of BC to match what was found
      in a header file.
    - adds missing enable_keypad_mode()
    - uses configure results to protect the inclusion of curses.h, 
      ncurses.h, term.h, and termcap.h.
    - Large amounts of cleanup of the PVM configure code (Closes: #4151)

* Improve detection of build/run environment under GNU/Linux Debian under
  arm, armel, mips & mipsel

tests:
======

* test_run() now accepts Sub-Modules (See SEP #31)

Internationalization and Characters Encoding:
=============================================

* Portugues do Brazil localization added (Thanks Daniel de Souza Grilo and others)

* Japanese localization added (Thanks Hiroshi Saito)

* Italian localization added (Thanks Carmelo Leggio)

* More help pages translated to Portugues do Brazil (Thanks Daniel de Souza Grilo)

* Catalan localization added to Scipad (Thanks Oriol Gonzalez)

* Japanese localization added to Scipad (Thanks Hiroshi Saito)

* Characters encoding:

  Strings manipulations functions updated to use wide characters.
  This feature add ability to manipulate any language on any system regardless of the system code page.

  Internal C functions added: readNamedMatrixOfWideString, createNamedMatrixOfWideString,
                              getMatrixOfWideString, createMatrixOfWideString in api_string.h
                              to manage wide characters with scilab API.

  input/output files functions also updated to use wide characters.
  Default file format encoding used by Scilab is UTF-8.

Bugs:

* bug 2627 fixed - Scilab 5 console retrieved input strings in UTF-8 format and Scilab did not
                   manage this format but ISO-8859.

* bug 3228 fixed - ascii function was broken for extended set (example: ascii('é')).

* bug 3433 fixed - Length with a UTF string returned an incorrect size

* bug 3472 fixed - Dropping files into Scilab shell did not work when the path
                   contained accented characters.

* bug 3666 fixed - 'input' returned accented characters.

* bug 3701 fixed - "Error during call to UTFToLocale: No such file or directory" on Windows.

* bug 3839 fixed - strrev() did not work correctly if the string to be reversed
                   included some accented characters.

* bug 4003 fixed - setenv & getenv did not manage utf string on Windows.

* bug 4012 fixed - Display of month and week were wrong in sessions starts and ends (history manager).

* bug 4019 fixed - With -nw or -nwni mode on Windows, console returned error about
                   localization.

* bug 4068 fixed - Accented names in directories were not supported.

* bug 4145 fixed - Simplified Chinese file names were not shown properly.

* bug 4233 fixed - exec command with a UTF filename failed.

* bug 4277 fixed - strsubst,regexp, strindex, grep returned strange results if accented
                   characters and character class were associated in the pattern to match.

* bug 4617 fixed - Some items (Console pull down menu and help browser item) 
                 were not translatable

* bug 4660 fixed - Error in the French localization (Edit instead of Edition)

* bug 4758 fixed - Embedded version of autoconf libs.m4 were causing some issues

* bug 4768 fixed - strsplit returned strange results with accented characters

* bug 4915 fixed - The default font in some of the uicontrols applications in
                   Scilab were not handling correctly for the Japanese and
                   others languages. This was happening with m2sci and the
                   demos browser.

* bug 5071 fixed - isletter("é") returned [%F %F] instead of %F.

* bug 5072 fixed - strspn("aéio","éa") returned 3 instead of 2.

String manipulation:
====================

* Rewrite:
    + strsplit - SEP #26

* 'NumTokens' is obsolete. See 'tokens'.

Java/Scilab API:
================

 - uses api_scilab functions to write & read data (internal for javasci)

 - SciInteger class added  

Functions removed:
==================

* lgfft (See bug #4600)

Linear algebra:
===============
* Clarified documentation for lu function.

Elementary functions:
=====================
* 'sort' is obsolete. See 'gsort'.

TCLSCI interface:
=================
* 'tk_getdir' is obsolete. See 'uigetdir'.
* 'tk_savefile' is obsolete. See 'uiputfile'.

GUI interface:
==============
* 'x_message_modeless' is obsolete. See 'messagebox'.

Bug fixes:
==========

* bug 336  fixed - pspect and cspec help pages improved

* bug 415 fixed - Order in the roots of a polynomial fixed


* bug 667  fixed - scilab.start-file caused an initialization error when you 
                    used predef('all') in a user startup file.

* bug 1412 fixed - A(3,3)=%s,A(:)=%z caused Scilab to hang

* bug 1451 fixed - theta, alpha, leg, flags as optional named arguments for hist3d set

* bug 1628 fixed - With long programs (several hours) a negative value for
                   timer() was get

* bug 1776 fixed - funcprot accepted non-integer arguments

* bug 1809 fixed - Warning message (figure_size) after an invalid graphical script added

* bug 1885 fixed - histplot starts from %eps rather than 0 to avoid logarithmic trouble.

* bug 2042 fixed - copy function altered axes properties.

* bug 2043 fixed - The function copy applied to an axis handle did not work

* bug 2238 fixed - Function returned a wrong error

* bug 2275 fixed - isvector help page added

* bug 2347 fixed - "fun2string" indentation problem with select/case 
                    instruction.

* bug 2446 fixed - "while" documentation help page improved to explain 
                    that the keywords do or then can be omitted.

* bug 2453 fixed - mfscanf and probably the other variants msscanf mscanf could crash Scilab when
                   first argument was -1 (any number of lines)

* bug 2509 fixed - execstr forgot to store the function name and line number when in the errcatch mode

* bug 2537 fixed - xstring feature improved

* bug 2570 fixed - LCC did not work with some "stk" ref on some old toolboxes.

* bug 2649 fixed - diary() function could not be nested.

* bug 3131 fixed - a glibc >= 2.4 was mandatory (causing Scilab to fail under
                   Debian Etch, Centos 4.X and some other old operating 
                   systems)

* bug 3184 fixed - Ticks of colorbar have disappeared

* bug 3200 fixed - add_help_chapter returns a boolean result which was
                   not explained in the help page.

* bug 3201 fixed - Help page for the function del_help_chapter added

* bug 3264 fixed - More explicit message when too many files have been
                    opened added.

* bug 3422 fixed - The keyword for history search was not updatetable

* bug 3488 fixed - Dynamic link functions can now handle Fortran 90 code.

* bug 3533 fixed - lib(dir_path) did not work if the final slash was missing in dir_path.

* bug 3537 fixed - Problem with examples in French help page for metanet functions. 
                    The old graph data structure was used.

* bug 3543 fixed - Wrong parenthesize in struct indices (code generated by tree2code).

* bug 3580 fixed - A gateway declaration containing a false primitive ID or gateway ID crashed Scilab.

* bug 3612 fixed - History manager did not work at startup

* bug 3660 fixed - font_angle was lost on the axis label when exporting through xs2pdf - Thanks to Paul Griffiths for the patch

* bug 3772 fixed - Input argument of 'predef' was not correctly checked.

* bug 3773 fixed - 'ans' was protected with predef('all').

* bug 3884 fixed - chdir, getcwd, pwd moved from core to fileio module.

* bug 3923 fixed - diary() function produced bad files when it tried to record interactive commands.

* bug 3953 fixed - delete() always set the current entity to the parent of the
                   deleted entity.

* bug 3980 fixed - Wrong line number in error message

* bug 3992 fixed - Some invalid dependencies on external libraries removed.

* bug 4030 fixed - unix_w('dir /s') could display strange matrix on Windows.

* bug 4090 fixed - Draw did not work any more while inside drawlater/drawnow scope.

* bug 4101 fixed - -ns option was not clearly explaining that it is 
                    disabling many Scilab features

* bug 4110 fixed - Demos order were different between Linux and Windows versions

* bug 4181 fixed - AdCommunications help page removed

* bug 4183 fixed - the function readmps has been moved to the
                   optimization folder

* bug 4184 fixed - Style of the examples corrected

* bug 4217 fixed - gsort sorted complex by magnitude only with 'g' and not support 'r', 'c', 'lr', 'lc'

* bug 4222 fixed - Documentation page on fromc and fromjava added

* bug 4232 fixed - Problem in the Java detection with GNU/Linux under
                    PowerPC fixed

* bug 4262 fixed - Lines returned incorrect values.

* bug 4263 fixed - Completion cleaned current input line.

* bug 4267 fixed - A non-initialization of a lock under Netbsd was 
                   causing an error on startup
                   
* bug 4277 fixed - Force UTF8 management in pcre processing.

* bug 4278 fixed - typename() called with a wrong number of input arguments returned incoherent errors.

* bug 4298 fixed - ilib_for_link failed with lcc-win32 (Windows).

* bug 4304 fixed - Fail to build with ./configure --without-gui

* bug 4307 fixed - Desactivate (in modules.xml) modules if their functionalities
                   are not available.
                   
* bug 4307 fixed - Help file for %io added                  

* bug 4330 fixed - stacksize('max') + stacksize(required_mem) did not work

* bug 4347 fixed - messagebox(): buttons were displayed in reverse order with 
                   respect to their order in the vector specifying them (Windows).

* bug 4351 fixed - Test about 'exit' did not work on Linux.

* bug 4358 fixed - Changing defaultstacksize in scilab.start did not work

* bug 4360 fixed - The reference of bug_3954.tst was missing

* bug 4364 fixed - stacksize('max') + stacksize(required_mem) did not work

* bug 4365 fixed - Completion failed on a wrong path

* bug 4366 fixed - The test dsaupd.tst was not working

* bug 4369 fixed - Help for text_properties: For the clip_state attribut,
                   the second possible value was 'clipgrf' instead of 'cliprf'

* bug 4378 fixed - The gamma function did not work properly with large numbers under Linux 64 bits

* bug 4379 fixed - If the thickness property (of plot3d for example) was set to 0,
                   the lines/edges could disappear

* bug 4380 fixed - diary(0) without an initialization of diary crashed Scilab

* bug 4380 fixed - Help page of the printf command had a problem

* bug 4380 fixed - Merror error codes documented

* bug 4385 fixed - the param3d_properties page has been removed, but a lot of
                   links to this page existed in the documentation.

* bug 4386 fixed - Dynamic link: link('show') displayed only one library if
                   we had only one symbol.

* bug 4391 fixed - make install-html (when the option --with-install-help-xml
                   was added to the configure) was not installing MathML sources

* bug 4394 fixed - 'origin' as position for X and Y axes added

* bug 4395 fixed - Incorrect descriptions of the axesflag and nax parameters
                   in the plot2d help page.

* bug 4396 fixed - In Plot2d, the nax option could receive -1 for number of ticks, which meant autoticks

* bug 4403 fixed - ieeefp.h was not included under Solaris in evaluate_expr.c
                   isinf was defined (when not provided by the system) with a return

* bug 4407 fixed - Help file scilab_xx_XX_help.jar had another scilab_xx_XX_help.jar embedded
                   and not valid.

* bug 4409 fixed - Fortran optim subroutines used fwrite (fortran) and not basout as
                   standard output

* bug 4414 fixed - Complex grayplots were not saved correctly in Scilab 5.1

* bug 4421 fixed - Completion failed on some relative path.

* bug 4426 fixed - uigetfile() - File name was cut when we were in the root directory.

* bug 4428 fixed - Function compute_initial_temp could not handle functions with
                    several arguments

* bug 4429 fixed - Use hidden_axes instead of hidden_axis

* bug 4430 fixed - Incorrect description of the margins property in the axes_properties help page.

* bug 4431 fixed - xsegs() accepted z argument

* bug 4433 fixed - Problem using optim. In some cases, the optim function made Scilab hung.

* bug 4437 fixed - Exporting an empty figure crashed Scilab.

* bug 4438 fixed - Completion did not support insert mode.

* bug 4439 fixed - Unexpected complex conjugates when using cat.

* bug 4459 fixed - grayplot did not always plot map containing %nan.

* bug 4461 fixed - Redefined prompt not displayed in a diary.

* bug 4463 fixed - Creating an one-colored Sgrayplot generated some exceptions
                   and did not plot anything.

* bug 4466 fixed - .' operator defined for strings and booleans

* bug 4467 fixed - getshortpathname,getlongpathname did not manage string matrix as input

* bug 4468 fixed - pathconvert(<path>,%F) could remove last trailing separator.

* bug 4469 fixed - getmd5() & mopen hung if there were called with an existing
                    directory as input argument and not a filename (Linux)

* bug 4474 fixed - Help of 'fort' was obsolete.

* bug 4474 fixed - addinter help had no example.

* bug 4479 fixed - A few minor bugs were causing the configure to fail under
                   FreeBSD (Thanks to Otacílio de Araújo Ramos Neto)

* bug 4483 fixed - Handle FreeBSD Java detection

* bug 4486 fixed - When a file had been opened for execution with "File/Execute..."
                   menu, its pathname was lost.

* bug 4489 fixed - oldsave and oldload removed.

* bug 4491 fixed - get() with wrong argument causes EXCEPTION_ACCESS_VIOLATION error.

* bug 4492 fixed - output_stream_gateway.xml was not include in Windows binary.

* bug 4495 fixed - exec did not check second and third input arguments.

* bug 4496 fixed - in tbx_build_gateway help page, ismex parameter was not documented.

* bug 4497 fixed - xmltoformat got stuck in directories beginning with '.'.

* bug 4498 fixed - merror() returned weird strings if called before opening files.

* bug 4499 fixed - basename returned a wrong value if file or a directory started with a dot

* bug 4500 fixed - basename('') returned a error

* bug 4501 fixed - mput() no more allowed integer types as first input argument.

* bug 4507 fixed - Depending on the Fortran runtime libraries a dummy main
                   definition was needed.

* bug 4514 fixed - valgrind.supp was not installed

* bug 4515 fixed - Linking and loading some external functions already used crashed Scilab

* bug 4516 fixed - ilib_for_link did not use ilib_gen_loader but an internal function.

* bug 4518 fixed - ilib_build with a column vector of strings as filenames did not work on Linux

* bug 4522 fixed - The Tcl interpreter initialization code made use of some uninitialized variables

* bug 4525 fixed - scilab.sln and scilab_f2c.sln did not apply same dependencies on libraries (Windows)

* bug 4528 fixed - makefile (Windows) generated by ilib_for_link forced to rebuild all files each time

* bug 4530 fixed - Leg labels curves were in reverse order

* bug 4533 fixed - Link failed on some path (Windows)

* bug 4539 fixed - Some optional parameters were not set in hist3d

* bug 4542 fixed - Unexpected interpreter warning 

* bug 4546 fixed - Error message added for linpro function (moved to an external function)
 
* bug 4547 fixed - Examples of javasci with graphics were no more interactive

* bug 4548 fixed - With Intel Fortran, file unit opened in a external dll were not shared

* bug 4551 fixed - Menus could return an error in GUI mode (error 2 Invalid factor)

* bug 4552 fixed - c++ comments in stack3.h had been translated into c comments

* bug 4564 fixed - Unable to edit matrices using x_mdialog function

* bug 4573 fixed - core.start tried to load add_module_help_chapter even if it did not exist

* bug 4574 fixed - ilib_build did not check if library was already loaded in Scilab

* bug 4580 fixed - rpem documentation made more accurate

* bug 4584 fixed - Under Windows, home value was wrong

* bug 4585 fixed - Scilab failed to build with Intel C++ compiler 11.0.074 on Windows

* bug 4586 fixed - Function fieldnames added for getting tlist, mlist and struct field names (see SEP #28)

* bug 4592 fixed - The ged entity picker produced an error if a menu had been
                   added to the graphic window

* bug 4596 fixed - g_margin failed to return the gain margins in some cases

* bug 4600 fixed - Missing function inside lgfft (lgfft needed be removed)

* bug 4601 fixed - Hidden functions and missing help for vec2list list2vec aplat and recons

* bug 4603 fixed - Unknown function lst2tree called by function mtlb_eval
                   (+ some other problems fixed in mtlb_eval)

* bug 4604 fixed - rmdir(<dir>,"s") did not work if it contained a hidden file
                   (starting with a dot)

* bug 4605 fixed - basename('') returned a error

* bug 4606 fixed - Example of 'link' in help did not work

* bug 4608 fixed - get_function_path returned a non-consistent path

* bug 4611 fixed - what function did not return a complete list of functions

* bug 4612 fixed - The graphic export used to consider .jpeg as an invalid
                   extension

* bug 4614 fixed - On some (rare) archs/OS, File -> Export on a graphic could
                    led to a crash 

* bug 4615 fixed - xs2jpg(0, '/tmp/myplop.jpeg'); was creating a file 
                   called /tmp/myplop.jpeg.jpg (Thanks to Guilherme 
                   Kunigami for the fix)

* bug 4616 fixed - Text strings in SVG output were not rotated. Thanks 
                   to Paul Griffiths for the patch!

* bug 4618 fixed - buttmag produced a warning because of redefining symbol sample

* bug 4620 fixed - the setenv function made Scilab hung.

* bug 4626 fixed - When using the CLI version, completion made Scilab crashed

* bug 4627 fixed - The function matfile2sci did not manage 7.x format

* bug 4630 fixed - Problem with the Xcos/Scicos block affich

* bug 4634 fixed - The expression "old graphic" has been removed from
                   the graphics help pages

* bug 4635 fixed - Bad title in the French help of the graphic module

* bug 4636 fixed - Metanet on-line help: an accent was missing on the "e" in the French version 

* bug 4638 fixed - This example with optim froze Scilab

* bug 4639 fixed - Some graphics demos did not contain the menu item "show code"

* bug 4641 fixed - The parameter Q of the function derivative was not clear

* bug 4645 fixed - French 'gsort' help had some wrong information

* bug 4646 fixed - Assigning a string to an element of a matrix of numbers did not give an error

* bug 4650 fixed - SCI/modules/graphics/macros/Sfgrayplot.sci had not been modified for localization

* bug 4654 fixed - Max of an empty sparse matrix made Scilab hung

* bug 4656 fixed - part(input_matrix,[]) always returned a single string even if
                   input_matrix had a several dimensions

* bug 4658 fixed - For graphics macros from adddcolor to getfont and all colormap

* bug 4666 fixed - SCIHOME was not well defined if Scilab had been launched in another Scilab session

* bug 4676 fixed - diary() removed whitespaces characters from the output of mprintf()

* bug 4683 fixed - There was no example of the use of tlist in the dedicated page

* bug 4684 fixed - printf(), mprintf, msprintf did not properly deal with -%inf

* bug 4688 fixed - listfiles("/") returned an error

* bug 4689 fixed - diary() did not report interactive commands into the output file

* bug 4695 fixed - Some help pages + examples were missing in the arnoldi package

* bug 4702 fixed - On some cases, dos(cmd) did not return results

* bug 4707 fixed - Could not save a Xcos/Scicos diagram to a non-existing file under Mac OS

* bug 4709 fixed - The set function with 2 parameters and no handles made Scilab hung

* bug 4710 fixed - Error with C++ interface which uses the function CreateVar

* bug 4712 fixed - By GUI handle affectation produced an error

* bug 4719 fixed - fsolve might failed on some system of equations. An example in the documentation was added

* bug 4724 fixed - When the f variation of a data set was too small, plot hung

* bug 4727 fixed - The function 'length' did not return the same value in Scilab 5.1 and Scilab 4.1.2

* bug 4728 fixed - The function 'strcat' did not return the same value in Scilab 5.1 and Scilab 4.1.2

* bug 4732 fixed - The help page of function ss2ss had some missing part

* bug 4737 fixed - Completion failed on file extension search

* bug 4744 fixed - whereis, librairieslist, libraryinfo primitives moved in "functions" modules

* bug 4746 fixed - Some ID for constraint linkend were missing.

* bug 4750 fixed - Error boolean catenation

* bug 4752 fixed - ci2exp produced erroneous code when applied to boolean matrices (; are missing)

* bug 4753 fixed - If you had a toolbox and you launched scilab-cli, you had an error message at startup

* bug 4759 fixed - Better error message in nlev when called with no input argument provided.

* bug 4770 fixed - modules::renderer::utils::ColorMap::isValidScilabIndex test was wrong

* bug 4773 fixed - Using "axes_reverse" produced a switch between the front and back color of a figure

* bug 4789 fixed - Number of input arguments was not checked in ilib_functions (dynamic_link module)

* bug 4792 fixed - Macro name too long

* bug 4795 fixed - CallScilab.h renamed call_scilab.h

* bug 4797 fixed - macro make_help_index.sci (internal) removed (not used in Scilab 5.x)

* bug 4798 fixed - pathconvert macro rewritten as a primitive (many times used and not faster)

* bug 4807 fixed - strsubst did not check if second argument was a valid patter where fourth argument was 'r'

* bug 4810 fixed - gsort did not manage sparse vectors as sort

* bug 4811 fixed - For some figures, it was not possible to save and load the handle of the figure

* bug 4812 fixed - xs2... function did not work if the first argument was a handle

* bug 4816 fixed - plot did not manage autoclear correctly

* bug 4817 fixed - macro savedefaultbrowser (internal) removed (not used in Scilab 5.x)

* bug 4820 fixed - the help page for the function csgn has been added

* bug 4826 fixed - The type of the 'dims' entry of an hypermat was not always the same

* bug 4828 fixed - Completion cleared the content of some completed path

* bug 4832 fixed - wavread returned a non explicit error if RIFF had some 'smpl' chunck

* bug 4838 fixed - Completion returned wrong value with '..' 

* bug 4846 fixed - mopen("","") crashed on Windows

* bug 4853 fixed - C,C++ dlls generated by Scilab had a dependency on fortran runtime (Windows)

* bug 4855 fixed - Standard error messages added when setting/getting values from graphics handles

* bug 4880 fixed - Query-replace with a regular expression doesn't work in editor

* bug 4904 fixed - wrong localized fr_FR message

* bug 4917 fixed - fileinfo(SCI+'/') returned [] (under Windows)

* bug 4918 fixed - Error message returned by 'get_absolute_file_path' was incorrect

* bug 4925 fixed - Verbose option of the optim function crashed on Windows

* bug 4926 fixed - Could not save a structure in a MAT-file using savematfile

* bug 4927 fixed - When the user tried to plot some data in semilogx or semilogy, a blank plot windows was displayed

* bug 4928 fixed - x_mdialog crashed when more default valued than labels

* bug 4943 fixed - execstr did not check input arguments

* bug 4944 fixed - If the extension is not provided, some functions adds the extension, some others don't.

* bug 4961 fixed - missing some field when printing some graphics Handle

* bug 4964 fixed - set or get 'format_n' property from an axis crashed Scilab

* bug 4966 fixed - execstr([]) return []

* bug 4970 fixed - Some link ID's were not correct in the doc of the
                   current master

* bug 4974 fixed - exec('') crashed Scilab

* bug 4983 fixed - When Scilab was started in nw mode, the addmenu examples were failing with a weird error message

* bug 4991 fixed - No check if there was modification when doing "Execute Into Scilab" in the editor

* bug 5033 fixed - The Demo CACSD / Inverted pendulum was broken.

* bug 5042 fixed - (1|[1,0,1;1,1,0]) returned a wrong result.

* bug 5074 fixed - French help star showed mis-encoded characters.

* bug 5081 fixed - demo "Signal Processing" --> "Spectral Estimation" failed (on Windows)

* bug 5105 fixed - completion: cd "..\L<tab>  did not work.

* bug 5121 fixed - Gui associated to PDE block has been localized

* bug 5138 fixed - ilib_build: if the "makefile name" argument is an empty matrix, the generated Makefile is ".mak"

* bug 5148 fixed - Setting {x,y,z}_ticks with empty value work.

* bug 5155 fixed - Scilab windows 32 bits version required CPU SSE 2 instructions.

* bug 5160 fixed - When a graphic has exported by the 'File -> export' menu, and did not specify any 'filter', scilab crashed.

* bug 5210 fixed - editor("SCI/etc/scilab.start") freezes Scilab.
