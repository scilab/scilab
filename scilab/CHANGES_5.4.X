            Changes between version 5.4.0-alpha-1 and 5.4.0-alpha-2 of Scilab
            =================================================================


XML
===

* Rename xmlClose in xmlDelete and xmlGetOpenStreams in xmlGetOpenDocs.


Xcos
====

* New examples are provided in the documentation for the blocks:
  - ABS_VALUE - Absolute value
  - EVTDLY_c - Event delay
  - AFFICH_m - Display value
  - CLSS - Continuous state-space system
  - SELECT_m - Selector block
  - NEGTOPOS_f - Threshold negative to positive



Unitary testing
===============

* external modules are now managed by out of the box by test_run
Example: test_run('SCI/contrib/toolbox_skeleton')
(See bug #9834)


Graphical User Interface
========================

* Bug 4978 fixed - Icons did not follow the Tango norm.

Scinotes
========

* Add shortcuts META+(UP|DOWN) to navigate between paragraphs


Javasci
=======

* Javasci supports sparse datatypes retrieval (Javasci is able to retrieve 
a sparse but not to send it to Scilab).

* Javasci supports struct datatypes under the form of an MList  (Javasci is
able to retrieve a struct but not to send it to Scilab).


Xcos
====

* Bug #8744 fixed - Solving the initialization problem more than once crash
                    Scilab.

* Bug #10081 fixed - Palette configuration saving could produce an invalid
                     file.

* Bug #10107 fixed - Default value of the Integrator Absolute Tolerance was
                     too large. It has been reduced to 1e-6.

* Bug #10333 fixed - Labeling I/O blocks on a Super block produced wrong port
                     labeling when ports had not the same types.

* Bug #10340 fixed - Exception occured while moving the palette split pane to
                     hide the block list and while using a palette diagram.

* Bug #10419 fixed - Exception occured while refusing to create a new empty
                     file.

* Bug #10422 fixed - Exception occured when exporting with an unspecified
                     format.


Signal Processing
=================

* New functions:
  - conv - discrete 1-D convolution.
  - conv2 - discrete 2-D convolution.
  - convol2d - discrete 2-D convolution, using fft.

* Bug #6009 fixed - conv, conv2 and convol2d and conv functions were missing.


Compatibility functions
=======================

* 'mtlb_conv' obsolete. Please use 'conv' instead.


Bug fixes
=========

* Bug #4813 fixed - Description of type 0 added in type help page.

* Bug #8050 fixed - A weird error message was displayed in some cases.

* Bug #9811 fixed - Output message of fminsearch was wrongly set.

* Bug #10017 fixed - The unitary test for bdiag used a "strange" comparison
                     operator.

* Bug #10044 fixed - wfir_gui function rewritten based on uicontrol and documented.

* Bug #10236 fixed - exit(-1) prompted for exiting Scilab.

* Bug #10384 fixed - On exit, "Close Console" was proposed rather than "Close
                     Scilab".

* Bug #10402 fixed - FileBrowser threw an exception when a filter was set
                     and the tree was expanded.

* Bug #10408 fixed - In the history browser, the contents was scrolled even if the
                     knob was not at the bottom.

* Bug #10418 fixed - Playsound was failing under Mac OS X. Thanks to
                     Pierre-Henri Jondot for the patch.

* Bug #10431 fixed - The user could not navigate in history using CTRL+P or CTRL+N.

* Bug #10440 fixed - 'wfir_gui' generated errors if the view checkbox was selected
                     and an erronous value was entered.


            Changes between version 5.3.3 and 5.4.0-alpha-1 of Scilab
            =========================================================

Scilab Desktop
===============

* Thanks to this new feature, Scilab confirms its usability and its strong
user-oriented approach. Scilab desktop is the new concept behind Scilab
windows management taking advantage of our docking system (advanced Flexdock features).
Out of the box, Scilab is now able to save all windows layouts including the
help browser, the variable browser, the command history, SciNotes and Xcos.
Through this major improvement, position and size of all windows will
be saved from a session to another.

In this context, a file browser component has been developed and fully
integrated into Scilab desktop and all other components have been improved
to provide the best user experience for Scilab users.

Editvar has had important refreshment. It allows plot and subplot, manages
almost all Scilab variable types and is much more faster.

Scilab desktop provides various predefined layouts:
 - Simple      The standard Scilab console
               => Scilab 5.3.3 (and previous versions) defaults.
 - Integrated  The variable browser, the history browser and the file browser
               are opened by default and combined around Scilab
               console to provide an ergonomic workspace
               => Scilab 5.4.0 defaults


XML Features
=============

* Version 5.4.0 introduces XML management features. It allows easy access to
XML files, validation and writing.

 - XMLObjects - Describe the properties of the different XML objects
 - xmlAddNs - Add a namespace to a XML Element
 - xmlClose - Close a XML document
 - xmlDTD - Create a XML DTD object
 - xmlDocument - Create a new XML document
 - xmlDump - Dump a XML object
 - xmlElement - Create a new XML element
 - xmlFormat - Format a Scilab variable into XML
 - xmlGetNsByHref - Get a namespace by its href
 - xmlGetNsByPrefix - Get a namespace by prefix
 - xmlGetOpenStreams - Get all open XML documents or all open XML Validation files.
 - xmlNs - Create a new XML Namespace
 - xmlRead - Read a XML stream from a local or distant file
 - xmlReadStr - Read a XML tree from a string
 - xmlRelaxNG - Create a XML Relax NG object
 - xmlSchema - Create a XML Schema object
 - xmlValidate - Validate a document in using a DTD, a Relax NG or a Schema.
 - xmlWrite - Write a XML document in a file
 - xmlXPath - Make an XPath query on a XML document
 - xmlIsValidObject - Test the existence of a XML object

Those features have been developed in the context of the OASIS R&D project.


SciNotes
=========

* Marks added in the vertical scrollbar to see the positions of matching words
  when an incremental search is done (via F3 or in selecting text).

* In the find/grep tools, possibility added to use ~ as an alias for user home
  dir. and SCI for Scilab data base dir.

* 'Save and Execute all files' action added.

* A tab can be closed in using the middle mouse button.

* Bug #9334 fixed - Extra lines were added when code was exported into PDF under
                   Windows.

* Bug #9358 fixed - A simple quoted string was not always correctly converted
                   into double quoted ones.

* Bug #9476 fixed - In find/grep tools, the abbreviation 'sec' is now used instead of
                   'seconds'.

* Bug #9480 fixed - Words after selection were badly highlighted under Windows.

* Bug #9484 fixed - An empty file was not removed when an existing file was
                   opened.

* Bug #9488 fixed - There was no popup menu on the tab to copy the file path.

* Bug #9505 fixed - A LaTeX string in a comment was not considered as a comment.

* Bug #9597 fixed - It was not possible to navigate across the tabs with
                   <CTRL>+(PAGE_UP | PAGE_DOWN).

* Bug #9740 fixed - There was no way to have \n in replacement string.

* Bug #9854 fixed - There was a  wrong colorization with strings containing several
                   dots: 'abc..'.

* Bug #9878 fixed - Shortcut "Correct indentation" added in right click menu.

* Bug #9897 fixed - With Mac OS X, eol was not replaced by \n when code was copied
                   from Word to SciNotes.

* Bug #10157 fixed - Scilab froze when docking SciNotes after a flexdock bug in a
                    previous session.

* Bug #10252 fixed - The font could not changed when SciNotes was docked with the
                    console.

* Bug #10354 fixed - Bad caret position after a selection action.


Graphical User Interface
=========================

* The variable browser now automatically refreshes (See Bug #9446).


Call_Scilab
============

* Bug #10006 fixed - Unrelevant C-comment in the example of ScilabHaveAGraph
                    removed.


Xcos
=====

* xcos_toolbox_skeleton will not be loaded if Xcos is not installed.

* scilab -f file.xcos opens Xcos diagram in Xcos.

* Error reporting has been improved to print warnings on blocks into Xcos.

* The style of a block can be changed in the interface function.

* BARXY Block added to help 2-D Mechanics system visualization.

* Bug #7985 fixed - dmmul.c did not use dgemm routine optimized by BLAS.

* Bug #8421 fixed - CLKFROM and CLKGOTO were not part of the "Signal Routing"
                   palette.

* Bug #8538 fixed - Counter block label was not reflecting parameters.

* Bug #8742 fixed - On the Modelica initialization GUI, the error reporting did
                   not work.

* Bug #8743 fixed - On the Modelica initialization GUI, it was impossible to
                   re-fix the states after a derivative fix.

* Bug #9037 fixed - When connecting blocks, the green port blinked on a small mouse move.

* Bug #9301 fixed - Exception when one tried to paste a column to replace a
                   column selection.

* Bug #9336 fixed - SuperBlock with non-connected ports produced an invalid
                   index error.

* Bug #9390 fixed - FROMWSB_c block did not reset to the default values on
                   reinitialization execution.

* Bug #9478 fixed - Block structure was not checked while adding them to a new
                   palette.

* Bug #9501 fixed - With a localized TMPDIR, block edition could throw an HDF5
                   exception.

* Bug #9506 fixed - Some libraries dependencies were not loaded at
                   xcos_toolbox_skeleton startup.

* Bug #9532 fixed - User-defined blocks had no icons when saved.

* Bug #9547 fixed - The about box was not localized due to a multi-line string.

* Bug #9579 fixed - Duplicate content in scicos_math.h removed and replaced by
                   a cleaner call to math.h instead.

* Bug #9590 fixed - Xcos examples were not embedded into Windows binary version.

* Bug #9732 fixed - Code generation failed at compilation under Windows with
                   "unresolved external symbol dset_".

* Bug #9750 fixed - Minor error in the help page of scicos_debug fixed.

* Bug #9810 fixed - Errors dealing with MATINV wrong simulation values have been improved.

* Bug #9927 fixed - Xcos crashed at 'XcosPalAdd' command execution.

* Bug #9947 fixed - VARIABLE_DELAY block could not be dragged to a diagram.

* Bug #9972 fixed - Error reporting with a wrong context produced an endless
                   loop.

* Bug #9988 fixed - CLKSOMV_f had a wrong interface function after loading from
                   a file.

* Bug #10003 fixed - The MAXMIN block did not update its display according to

* Bug #10048 fixed - Blocks which use superblocks now loop over their children 
to find the parameters dialog to open.

* Bug #10069 fixed - AFFICH_m block did not update its display at
                    reinitialization.

* Bug #10096 fixed - Set [1;-1;] on a product block thrown an exception.

* Bug #10302 fixed - LaTeX labels were not rendered.



Localization
=============

* Czech localization by ZbynÄk Schwarz


* Bug #10032 fixed - Force the unset of the environment variable LANGUAGE to
                    tackle some issues. Mac OS X locale detection did not take into account
                    the LANG variable.


File input/output
==================

* mgetl, exec, fscanfMat functions manage files encoded as UTF-8 with BOM.

* tempname function added (SEP #68).

* fullpath did not manage matrix of string as input argument. (SEP #72)

* Bug #9253 fixed - fscanfMat failed to read a file with some lines
                   of text at the end (backward compatibility).

* Bug #9570 fixed - fprintfMat did not use the good format argument with nan and
                   inf in a matrix.

* Bug #9869 fixed - fscanf did not check the number of input arguments.

* Bug #9917 fixed - mputl function crashed with a wrong second parameter.

* Bug #10055 fixed - findfiles had a wrong default filter under GNU/Linux
                    & Mac OS X.

* Bug #10300 fixed - fscanfMat did not free some memory.

* Bug #10355 fixed - Error messages in fullfile function were not localized.


New Functions
=============

* The assert module provides a set of assertion functions.
  The goal of this module is to provide functions to check the
  behavior of some other functions, for example in unitary tests.
  We emphasize the use of consistent tools for testing numerical
  issues, with the goal of testing numerical algorithms more easily.
  In particular, we provide a comparison function for two floating
  point numbers, which allows to check that two numbers are
  "numerically almost equal", i.e. that the relative error is small.
  This automatically fixes Bug #4381.

* The damp function in CACSD module computes the natural pulsation
  and damping factor of linear dynamical systems.


Optimization
=============

* neldermead: Added a demonstration - search for largest / smallest polygon.

* Improved clarity of examples in the help pages of 
  optimization module.
  Avoid unnecessary calls to "deff".
  Avoid the "all statements in whole line" style.

* readmps - Created unit test.
            Fixed bug in error messages management.
            Fixed bug in a test file reading.
            Defined a printing system by overloading for MPS structure.
            Updated the help page.

* genetic_algorithms - performances improved (2 to 4 times faster).

* Bug #7093 fixed - The data structure generated by neldermead_new could not
                   be saved and reloaded.

* Bug #7889 fixed - The tolerance on x for neldermead was too small.
                   Updated -tolxrelative default value from %eps to sqrt(%eps).

* Bug #7891 fixed - The neldermead component had too many options.
  Tagged -costfargument option of optimbase as obsolete: will be
  maintained for backward compatibility until 5.4.1.
  The -fun option can now be a list, where the element #1 is a
  function, and the elements #2 to the end are automatically appended to
  the calling sequence.
  Tagged -outputcommandarg option of optimbase as obsolete: will be
  maintained for backward compatibility until 5.4.1.
  The -outputcommand option can now be a list, where the element #1 is
  a function, and the elements #2 to the end are automatically appended
  to the calling sequence.
  Tagged "outputfun(x,optimValues,state)" calling sequence of fminsearch
  as obsolete: will be maintained for backward compatibility until
  5.4.1.
  The new calling sequence is "stop=outputfun(x,optimValues,state)"
  Tagged "myoutputfun(state,data)" calling sequence of neldermead
  as obsolete: will be maintained for backward compatibility until
  5.4.1.
  The new calling sequence is "stop=myoutputfun(state,data)"
  Tagged "-myterminateflag" and "-myterminate" options as obsolete:
  will be maintained for backward compatibility until 5.4.1.
  See the help page of neldermead for details to update your
  code, if required.

* Bug #7892 fixed - The neldermead component did not provide a default
                   output function.
                   The new function neldermead_defaultoutput was created
                   to print a simple one line output message by iteration.

* Bug #8542 fixed - In neldermead, there were undocumented default options.
                   Same changes applied to the help of optimbase.

* Bug #8543 fixed - In neldermead, the help page presented a wrong order
                   for the options.

* Bug #8545 fixed - In neldermead, the printing system displayed too long
                   messages.

* Bug #8546 fixed - The neldermead functions might have called f outside 
                   the bounds.

* Bug #8547 fixed - The neldermead function called f outside the constraints.

* Bug #8549 fixed - The neldermead function did not help to debug
                   the cost function.

* Bug #8552 fixed - Improved fminsearch to provide Extra Parameters.
                   Help page updated with new section "The cost function" and new 
                   example "Passing extra parameters".

* Bug #8676 fixed - The pack/unpack functions had no help pages.

* Bug #8777 fixed - The neldermead function failed to restart with bounds.
                   Added examples of restarts with/without bounds in the
                   neldermead help page.

* Bug #8805 fixed - The neldermead function, algorithm box, wrongly used
                   the output function.

* Bug #9323 fixed - The messages in optimbase_checkbounds were not localized.
                   Same changes applied to optimbase_checkx0.

* Bug #9472 fixed - The ind argument of optim was inconsistently documented.

* Bug #9566 fixed - The tolvarianceflag, tolabsolutevariance,
  tolrelativevariance options of neldermead are now obsolete.
  See the help page of neldermead for details to update your
  code, if required.

* Bug #9567 fixed - The neldermead overview had a TODO section.

* Bug #9576 fixed - The neldermead component did not check the bounds
                   of some input parameters.

* Bug #9578 fixed - The optimbase_configure function did not check the
                   values of input arguments.

* Bug #9607 fixed - The optimsimplex_new function poorly managed the
                   variable number of arguments.

* Bug #9612 fixed - The quapro help page has been removed.

* Bug #9698 fixed - The optim/"gc" algorithm did not support imp=-1.

* Bug #9822 fixed - The value ind=1 of optim was not supported by
                   all algorithms.


Statistics
==========

* Bug #9357 fixed - stdevf returned zero for row vector arguments.

* Bug #9846 fixed - stdevf() gave zero result when a single row matrix was evaluated.

* Bug #7387 fixed - The help pages for rand and grand functions gave wrong advice.


Linear Algebra
==============

* Bug #9749 fixed - expm reported non convergence problem.


Sparse Matrices
===============

* ordmmd help page and unit test created.

* Organized help pages into sub-sections.

* Bug #10126 fixed - Wrong (or failing) insertion in a sparse matrix.


Documentation
=============

* A new system to keep track of the translations of the main documentation
  has been developed. See http://help.scilab.org/status/fr_FR/, 
  help.scilab.org/status/pt_BR/ and http://help.scilab.org/status/ja_JP/
  See Bug #5119.

* <ALT>+<LEFT ARROW> / <ALT>+<RIGHT ARROW> allows browsing the history
  (See Bug #9474).

* The space key behave like the page down key (See Bug #9491).

* Management of the revhistory / revision tag to provide information about
  the history of the function.

* Bug #3431 fixed - Some help tools functions were obsolete and not removed.

* Bug #9105 fixed - err output parameter of mopen function was not documented.

* Bug #9299 fixed - Fix a typo in the cspect, pspect and window help pages
                   (Hanning changed to Hann).

* Bug #9320 fixed - analpf help page gave wrong frequency units of omega (Hz instead of rd/s).

* Bug #9417 fixed - In English, api_scilab polynom help pages updated to the word
                   polynomial.

* Bug #7133 fixed - help_from_sci function did not return the function template.

* Bug #7134 fixed - help_from_sci produced an invalid .xml file.

* Bug #9790 fixed - windows_tools help were not available for all platforms.

* Bug #9983 fixed - The xclick help page did not clearly specify the axes used for
                   coordinates.


Obsolete Functions
===================

* 'oemtochar' and 'chartooem' functions (windows_tools) are obsolete and will be
   removed in Scilab 5.4.1.


Removed Functions (previously declared as obsolete)
====================================================

* ilib_for_link, ilib_build no more accept files with ".o" or ".obj" extensions
  for source files.

* 'with_atlas' removed. Please use 'getdebuginfo' instead.

* C API function Error(x) removed. Please use SciError instead.

* 'MSDOS' removed. Please use 'getos' instead.


Performance
============

* Under GNU/Linux and Mac OS X, Scilab startup speed of scilab-cli and
  scilab-adv-cli has been improved by 15 to 20 % (see Compilation for more
  information).

* SSE is now enabled by default under GNU/Linux with 32-bit CPU (it is already
  the case under 64-bit CPU).
  Pentium III class (or equivalent) with SSE instructions is required.

* Improved performances of function pareto_filter (Genetic algorithms).

* Bug #8608 fixed - dec2hex, hex2dec, dec2oct, oct2dec, base2dec code have been
                   simplified and optimized.


Compilation
============

* Under Windows, Scilab uses VS 2010 Pro and Express solution.

* Under GNU/Linux and Mac OS X, engine libraries have been merged into a single
  library named libscilab-cli.so. This greatly improves the startup
  performances.
  Note that the --enable-maintainer-mode enables the previous behavior (simply
  the development process).

* Under GNU/Linux and Mac OS X, all modules build as standalone their algorithms
  as a static library (only available in the source tree).
  This will avoid an important dependency tree for unitary testing.

* Under GNU/Linux and Mac OS X, for each module, the list of
  explicit dependencies on the other libraries is no more needed. 
  Use libscilab-cli.so or libscilab.so to link against Scilab.

* Enforce a minimal version of JRosetta (1.0.4).

* Enforce a minimal version of flexdock (1.0).

* Enforce a minimal version of GIWS (1.3.0).

* Force the Java class version target to version 6 (or 1.6).

* Add Java 7 in the Debian & Ubuntu search path under GNU/Linux.

* Under GNU/Linux and Mac OS X, if ccache is installed, use it to build Scilab.

* Bug #9409 fixed - Warning "remark #8291" removed with Intel Fortran 2011 compiler.

* Bug #9500 fixed - Explicit the GUI module manifest class-path to facilitate Debian
                   checking tool process.

* Bug #9782 fixed - configure with --without-gui was failing.

* Bug #9793 fixed - --without-gui now also disables Xcos.

* Bug #9883 fixed - Build with --disable-build-help was failing.

* Bug #9991 fixed - ./configure --without-xcos failed to build.

* Bug #10158 fixed - Configure used g77 even if gfortran was available.


Dynamic Link
=============

* Bug #9432 fixed - Link a 64-bit dynamic library on a 32-bit platform returned
                   a wrong error message under Windows.

* Bug #9761 fixed - NDEBUG was not defined for dynamic link (by default) in
                   release mode under Windows.

* Bug #9930 fixed - ilib_build did not display all filenames used.

* Bug #10320 fixed - libname argument of ilib_for_link did not work.

* Bug #10344 fixed - Windows SDK 7.1 was not detected with VS Express 2010 
                    under 64-bit platform.


Variable Browser
=================

* Shows the dimension of the variable by default.

* Automatically resize the variable browser table when docked.

* A tooltip has been added on the type column to display the name of the type.

* A tooltip has been added on the size column to display the size in bytes
  (like in previous versions of Scilab).


History
========

* Default text in history for a starting session has been changed for:
"Begin Session: Tue Jun 21 14:35:45 2011" to "21/06/2011 14:35:45"


Graphics
=========

* datatips tool improved on several aspects:
  - It is now possible to manage directional arrows as well as
    square mark markers.
  - The automatic label placement has been improved to avoid the
    overlapping with the curve.
  - It is now possible to have painted box or box free labels.
  - Markers can now be selected and moved using the arrow keys
    or deleted with the "Del" key.

* Bug #2043 fixed - The graphic function copy() did not manage axes. Thanks to
                   Samuel Gougeon for the fix.

* Bug #7412 fixed - Better management of datatips activation/deactivation.

* Bug #7419 fixed - datatips(): customization is now available through a GUI.

* Bug #8239 fixed - Activation of datatips overwrote and destroyed any event_handler
                   formerly defined.

* Bug #8965 fixed - copy function did not work for handles.

* Bug #9269 fixed - Specified graphic entities for legend were drawn in reverse order.

* Bug #9330 fixed - delete() called overloading instead of deleting the current
                   graphic entity.

* Bug #9455 fixed - The contourf and contourf 2 demonstrations displayed unwanted dots.

* Bug #10121 fixed - polyline.interp_color_vector where not saved or recoverable
                    by xsave/xload. Thanks to Samuel Gougeon for the fix.

* Bug #10298 fixed - Example of datatipSetDisplay generated an error.

* Bug #10331 fixed - datatipCreate produced a warning and an error instead of
                    a warning if the curve user_data was not a struct.

ATOMS
======

* Bug #5380 fixed - atomsShow() output was broken (separators were not aligned).

* Bug #8648 fixed - The tarball was not deleted when a module was removed (SEP #71).

* Bug #8664 fixed - Message error updated for non-supported OS.

* Bug #8763 fixed - ATOMS installation path was shared on Windows 64 with 32-bit.

* Bug #8942, #9805 fixed - atomsSystemUpdate() failed under some Windows.

* Bug #9127 fixed - "Home" item renamed to "Installed Modules".

* Bug #9679 fixed - atomsSystemUpdate checks that Scilab is not in offLine mode, or writes a warning message if so.

* Bug #9805, #8942 fixed - atomsSystemUpdate() failed under some Windows.

* Bug #9967 fixed - Windows UAC was causing trouble with atomsRepositorySetOfl (internal function).

* Bug #10025 fixed - Wrong url in some help files examples.

* Bug #10031 fixed - atomsDisp did not work with "\n" in messages text.


Javasci
========

* Java mapping of Scilab data has now a new method called getType() which
returns the value of the ScilabTypeEnum enum.

* New methods added:
  - Scilab.execException(String)
    Same as Scilab.exec(String) but returns an exception when a Scilab error occurs
  - Scilab.execException(String[])
    Same as Scilab.exec(String[]) but returns an exception when a Scilab error occurs
  - Scilab.execException(File)
    Same as Scilab.exec(File) but returns an exception when a Scilab error occurs

* Bug #9149 fixed - Calling a graphic function without activating the appropriate
                   mode did not triggered an exception.

* Bug #9467 fixed - Unnecessary declarations in MAKE.BAT of Javasci examples.

* Bug #9544 fixed - Calling graphic with Javasci from a SWING application from
                   EDT was crashing the execution of a Java program.


CACSD and Signal Processing
===========================

* Bug #1635 fixed - iir filter frequency response might have been wrong. 
                  "pole zero gain" representation added as possible iir function 
                  returned value.

* Bug #7895 fixed - fft2() was broken under Windows with MKL wrapper.

* Bug #7901 fixed - besselh.tst failed under Windows.

* Bug #8684 fixed - wfir function produced an error message when used interactively.

* Bug #9098 fixed - CACSD demonstrations --> Robust control failed on 64-bit Windows.

* Bug #9214 fixed - abaque replaced by nicholschart in the black.sci comments.

* Bug #9266 fixed - fftw crashed under Windows when using MKL wrapper.

* Bug #9321 fixed - Missing tests and help info for some CACSD functions.

* Bug #9647 fixed - At Scilab startup, the variable fft was declared by default
                   while it should not have been.

* Bug #9675 fixed - iir generated bad results.

* Bug #9716 fixed - amell.tst execution froze under Windows with reference lapack library.

* Bug #9719 fixed - delip.tst execution froze under Windows.

* Bug #9725 fixed - syredi.tst execution froze under Windows.

* Bug #10008 fixed - Wrong unit in the datatip for bode function.

* Bug #10169 fixed - Bad clipping option in evans.


Misc
=====

* The linspace function accepts column vectors as input argument for the two
  first input arguments. Thanks to Guillaume AZEMA for the implementation.

* The logspace function accepts column vectors as input argument for the two
  first input arguments.

* Change the buffering display mode.

* SEP #67: lines(0) by default at startup.

* Options -disable-exception-catching added under GNU/Linux & Mac OS X.
  Disables the Scilab exception catching system.

* texmacs module moved as an ATOMS module.
  atomsInstall('texmacs')

* PVM module moved as an ATOMS module.
  atomsInstall('PVM')

* maple2scilab module moved as an ATOMS module.
  atomsInstall('maple2scilab')

* SEP #69: predef('names') returns names of protected variables by predef.

* printf moved as alias on mprintf function.

* sprintf moved as alias on msprintf function.


Bug Fixes
==========

* Bug #1604 fixed - List extraction failed.

* Bug #3291 fixed - SCI/modules/tclsci/tcl/BWidget-1.8.0/ moved as prerequirements.

* Bug #4240 fixed - Some unexpected blank lines were displayed with the dir function.

* Bug #4576 fixed - An example of ilib_build with "cflags" argument was missing.

* Bug #4833 fixed - msprintf() interpreted "\n" as a carriage return even if "\n"
                   was supplied by the 2nd input argument.

* Bug #4847 fixed - In uigetfile, there was no way to select files with a given
                   pattern.

* Bug #5491 fixed - Clearly states that haveacompiler() only works under Windows.

* Bug #5696 fixed - When SE Linux is enabled, error messages for
                   Scilab in the Scilab binary distribution disabled.

* Bug #6408 fixed - prompt() removed trailing whites.

* Bug #6819 fixed - prompt() interpreted '<...>' as html.

* Bug #6875 fixed - test_run() could not run at once on the +3000 tests of
                   Scilab.

* Bug #7061 fixed - changelog.txt, readme.txt, version.xml removed from modules.

* Bug #7249 fixed - 'string' function did not work as documented
				with a macro as input argument.

* Bug #7333 fixed - In BrowseVar, variables could have a weird type number.

* Bug #7358 fixed - Help pages of type and typeof have now links to the actual
                   description pages.

* Bug #7409 fixed - Some corrupted matrices could be created from api_scilab.

* Bug #7700 fixed - Bad proposition in the console history when a char was added.

* Bug #7745 fixed - add_demo moved in demo_tools module.

* Bug #7815 fixed - tohome() function erased previous and future commands in Scilab console.

* Bug #8086 fixed - Some functions displayed a wrong error message when called
                    without input argument.

* Bug #8243 fixed - xgeom demonstration renamed to geom3d.

* Bug #8245 fixed - gsort() did not allow overloading with not managed types.

* Bug #8296 fixed - Asking for extra array dimension value with size crashed Scilab.

* Bug #8420 fixed - lines crashed Scilab in some cases.

* Bug #8688 fixed - fillMatrixOfString did not check available memory on stack.

* Bug #8753 fixed - Help file of c_link did not describe input argument num.

* Bug #8768 fixed - GetRhsVar(..., STRING_DATATYPE, ...) did not check size of input argument.

* Bug #8797 fixed - Update of the help pages of getdate('s') to detail that
                   the returned value contains the leap seconds.

* Bug #8874 fixed - Display of complex numbers with Inf or Nan for real or imaginary
                   parts was broken.

* Bug #8963 fixed - mtlb_mode test failed under Windows.

* Bug #9045 fixed - plot2d did not support overloading.

* Bug #9056 fixed - A uicontrol could not be closed with <META>+W.

* Bug #9104 fixed - 'warning' used in a diary Scilab session added timestamp
                   twice in the diary.

* Bug #9129 fixed - Completion on field did not propose graphics handle fields
                   by default.

* Bug #9156 fixed - mgetl.c compilation failed under some Windows.

* Bug #9164 fixed - The unit tests now use the new assert functions.

* Bug #9268 fixed - Scilab was overriding a predefined variable JAVA_HOME.

* Bug #9277 fixed - Escape character "\" did not work with msprintf.

* Bug #9339 fixed - tbx_build_help function did not manage languages correctly.

* Bug #9354 fixed - 'avematfile could not save imbricate structures.

* Bug #9414 fixed - 'SCI' and 'SCIHOME' variables were not documented.

* Bug #9452 fixed - It was impossible to download Intel MKL through a proxy.

* Bug #9461 fixed - The assert_checkalmostequal function did not manage
                   sparse matrices and hypermatrices.

* Bug #9479 fixed - Typo fixed (intput => input).

* Bug #9498 fixed - A long history caused Scilab startup to be very long.

* Bug #9510 fixed - export_to_hdf5 did not return %f when trying to export
                   unmanaged types (function, macro, ...).

* Bug #9531 fixed - assert_checkequal failed with mlist.

* Bug #9540 fixed - MKL Pentium 4 target was not downloaded by installer under Windows.

* Bug #9542 fixed - During export operation, the file could be locked by another
                   process (Windows Bug).

* Bug #9543 fixed - Scilab did not use the default DocumentBuilderFactory
                   to parse XML configuration files.

* Bug #9555 fixed - Typo error in the deff help page.

* Bug #9571 fixed - "error(144)" returned a wrong variable name.

* Bug #9573 fixed - getdebuginfo did not report multiple video cards under Windows.

* Bug #9581 fixed - cleaner.sce in toolbox_skeleton did not remove all generated files.

* Bug #9599 fixed - Error message "at line ... of function ... called by" was
                   printed directly on the same line.

* Bug #9635 fixed - Some temp. variables were not removed when test_run was used.

* Bug #9647 fixed - At Scilab startup, the variable fft was declared by default
                   while it should not have been.

* Bug #9659 fixed - "error(246)" returned a bad message for overloading.

* Bug #9682 fixed \D0 3-D assignment into a 2-D array of integers issued an error.

* Bug #9696 fixed - An error in an if expression inside a try make Scilab froze.

* Bug #9699 fixed - A single $ as text in a text ui led to a segfault.

* Bug #9707 fixed - createNamedXXX did not check variable name.

* Bug #9708 fixed - createNamedXXX did not check length of variable name.

* Bug #9712 fixed - Help of StartScilab had a error about Stacksize argument type.

* Bug #9716 fixed - amell.tst execution froze under Windows with reference lapack library.

* Bug #9717 fixed - dllinfo.tst failed under Windows binary versions.

* Bug #9728 fixed - SCI/modules/special_functions/src/fortran/special_functions_f.def
                   was not ignored (generated file).

* Bug #9730 fixed - lines() returned an int32 and not a double value.

* Bug #9755 fixed - Completion on paths was not case insensitive under Windows.

* Bug #9756 fixed - polelm.tst unitary test failed under 64-bit Windows.

* Bug #9763 fixed - comet3d did not check input argument presence.

* Bug #9765 fixed - Remove ref to scicos_pal in sci_f_clear.f.

* Bug #9766 fixed - Scilab under Mac OS X 10.7 did not start.

* Bug #9791 fixed - toolbar visible state change crashed Scilab in NW mode.

* Bug #9799 fixed - pol2str did not manage matrix of polynomials and input argument
                   type was not checked.

* Bug #9804 fixed - Starting Scilab with an external module emptied the
                   demonstrations list menu.

* Bug #9825 fixed - The assert_computedigits function gave too large correct digits.

* Bug #9833 fixed - percent was badly formatted by Scierror function.

* Bug #9836 fixed - repfreq failed due to singular problem.

* Bug #9838 fixed - evstr froze on some specific strings.

* Bug #9843 fixed - Colormaps with 2 colors could not be created.

* Bug #9858 fixed - length() could not be overloaded with mlist.

* Bug #9879 fixed - Fix some minor issues in the isdir help pages.

* Bug #9880 fixed - Fix a page referencing itself in the help page.

* Bug #9881 fixed - Fix a mistake in call help page.

* Bug #9885 fixed - User initialization scripts (scilab.ini, .scilab) did not work.

* Bug #9899 fixed - string(10*log10(1/10)) gave -0, instead of -10.

* Bug #9900 fixed - There was a memory leak in getAllocatedMatrixOfString,
                   getAllocatedMatrixOfWideString, getAllocatedNamedMatrixOfString,
                   and getAllocatedNamedMatrixOfWideString.

* Bug #9920 fixed - winqueryreg did not manage HKEY_CURRENT_CONFIG.

* Bug #9929 fixed - Improve integration with Gnome shell and Unity.

* Bug #9950 fixed - It was possible to select several texts in the console.

* Bug #9957 fixed - msprintf("%d\r", 2) returned '2r'.

* Bug #9963 fixed - The assert_computedigits function was not vectorized.

* Bug #9966 fixed - The default value of comptype in assert_checkalmostequal
                   was wrongly chosen.

* Bug #9971 fixed - GLoad macro not used and not documented has been removed.

* Bug #9979 fixed - The who_user() function printed a wrong message.

* Bug #9980 fixed - Frozen console when one tried to complete an empty string.

* Bug #10001 fixed - %inf:%inf crashed Scilab.

* Bug #10016 fixed - The damp function unit test failed under Windows.

* Bug #10019 fixed - 'exec' crashed with error 113 for too long lines.

* Bug #10037 fixed - checkalmostequal.tst unitary test failed under Linux x64.

* Bug #10073 fixed - Use the new Scilab icon for GNU/Linux installations.
                    Icons renamed from XPM to PNG.

* Bug #10105 fixed - Focus was not given to the prompt when outputview had it.

* Bug #10119 fixed - assert_checkequal did not support sparse complex doubles

* Bug #10124 fixed - strsplit crashed with wrong input arguments.

* Bug #10151 fixed - Wrong error message about polynomials was displayed.

* Bug #10179 fixed - Weird behavior with evstr.

* Bug #10282 fixed - repmat did not work for scalar input on dims greater than 2
                    neither for N-D arrays.

* Bug #10283 fixed - w and iw output arguments of ode function
                    are now directly read from work table used in sub-functions.

* Bug #10289 fixed - xload returns a more explicit error message when a file does
                    not exist.

* Bug #10292 fixed - Improved description of modulo help page.

* Bug #10293 fixed - Improved description of bvode help page.

* Bug #10324 fixed - format(m) where m is a vector did not work but the
                    documentation stated it could be used.

* Bug #10366 fixed - A warning about fun2string redefinition was displayed
                    when we used "Edit label generator" and click "Ok".


Known Issues
============
* Random crash under Mac OS X at startup.
  Some random crash can occur at first Scilab startup. Try to restart Scilab.

* Some issues at startup under Mac OS X 10.6.7 (and previous 10.6 versions)
  have been reported.
  If you have "Audio Hijack" installed on your Mac OS X, it prevents some
  applications to work including Scilab. See Bug #7012.
  Uninstall / disable Audio Hijack should fix this problem.
  Airfoil, also developed by Rogue Amoeba, has been reported as also breaking
  Scilab.
