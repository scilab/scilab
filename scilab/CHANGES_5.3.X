            Changes between versions 5.3.1 and 5.3.2 of Scilab
            =================================================

Scilab:
=======

* The Mac OS X version is checked to avoid errors on unsupported versions
  of Mac OS X. As a reminder, Scilab is supported under Mac OS X 10.6.

* Under GNU/Linux, in case of an internal (C, C++ or Fortran) programming error
  of Scilab or an extension, the full error stack is displayed and Scilab no
  longer crashes.

Scinotes:
=========

* Tools 'Find Files' (CTRL+SHIFT+B under Windows and Linux / CMD+SHIFT+B under 
  Mac) and 'Find word in Files' (CTRL+SHIFT+F under Windows and Linux / 
  CMD+SHIFT+F under Mac) added.

* Added possibility to replace single quoted by double quoted strings.

* Added possibility to save and execute the code with F5 key.

* Added possibility to remove the comments when executing the code in the
  console.

* All occurrences of a search word are now highlighted in yellow in the 
  text editor.

* Bug 9012 fixed - Print preview triggered a Java exception.

* Bug 9047 fixed - edit('foo',12) now opens the function foo 12 lines after the
                   definition of foo.

* Bug 9132 fixed - When s = "abc // def" was evaluated WITH ECHO, only s="abc was
                   executed.

* Bug 9171 fixed - Encountered problem to enter Japanese chars under Windows 
                   with IME.

* Bug 9183 fixed - Bad display of selected text when horizontal wrapping was
                   disabled.

* Bug 9186 fixed - An exception was thrown when textfield got the focus in code
                   navigator.

* Bug 9189 fixed - Some keywords were not helpable.

* Bug 9257 fixed - Scinotes could not be started from NW / adv-cli mode.

* Bug 9340 fixed - Bad name was proposed at PDF export.

* Bug 9348 fixed - Miscolorization of a transposed field.


Xcos:
=====

* Some blocks parameters dialogs now support the translation system.

* A skeleton of an Xcos module has been introduced and should help the user
  to design his own palette.

* Documentation updated with examples of the blocks and their translations 
  into French:
  - OUT_f - Regular Output Port
  - IN_f - Regular Input Port
  - Modulo_Count - Modulo counter (0 to N counter)
  - OUTIMPL_f - Output implicit port
  - CLKINV_f - Input activation port
  - CLKOUTV_f - Output activation port
  - INIMPL_f - Implicit Input port
  - GENSIM_f - Sine wave generator
  - STEP_FUNCTION - Step Function
  - SAWTOOTH_f - Sawtooth generator
  - TEXT_f - Free annotation
  - Counter
  - RAMP
  - INTRPLBLK_f (english update only)
  - TKSCALE (english update only)
  - EXPRESSION (english update only)
  - DELAY_f
  - RFILE_f Read from input file

* Some duplicated tests have been removed.

* Bug 5425 fixed - Drag and drop of blocks between different Scilab versions
                   produced an exception.

* Bug 5605 fixed - REGISTER block was not listed in the discrete palette.

* Bug 6416 fixed - GENSQR_f block documentation was not complete.

* Bug 7479 fixed - The diagram did not update its origin on block moving.

* Bug 8630 fixed - Scopes crashed Scilab on simulation when they had the same
                   window id.

* Bug 8692 fixed - Null block parameters were not checked before encoding.

* Bug 8735 fixed - Mixed (Implicit-Explicit) superblocks I/O blocks produced
                   warnings.

* Bug 8931 fixed - Simulations errors were not reported under Linux and Mac.

* Bug 9024 fixed - Opening a non existing file led to palette closing issues.

* Bug 9077 fixed - The diagram in Automotive Suspension demo was indicated as 
                   modified on loading.

* Bug 9111 fixed - Zoom In/Zoom Out keyboard shortcuts did not work under Mac.

* Bug 9122 fixed - Java check failed on diagram validation.

* Bug 9160 fixed - Saving a diagram with the default name produced an exception.

* Bug 9175 fixed - Xcos: Sigbuilder, modify "Data bounds" in curve editor failed.

* Bug 9237 fixed - Labels were not editable after a block copy.

* Bug 9219 fixed - AFFICH_m format was not the parameterized one.

* Bug 9307 fixed - AFFICH_m block only printed the first column for an input
                   matrix.

* Bug 9346 fixed - FROMWS_c block had a functional regression since Scilab 5.3.1.

* Bug 9369 fixed - Diagrams with an origin did not pass validation.

* Bug 9386 fixed - Diagram root cell could be invalid on hierarchical diagrams.


Packaging:
==========

* Scilab's icons updated.

* In the source tree, Scilab's icons moved from icons/ to desktop/.

* In the source tree, scilab-adv-cli.desktop, scilab-cli.desktop,
  scilab.desktop can be found desktop/. This follows the FreeDesktop Standards:
  "Desktop Entry Specification".


toolbox_skeleton:
=================

* removal of temporary variables created on the stack by the build and load of
  the toolbox_skeleton.


Javasci:
========

* Javasci v2 FAQ updated.

* Bug 6823 fixed - In Javasci v1, the constructor
                   SciStringArray(String name, int r, int c) did not work.
                   Thanks to F. Legrand for providing the fix.

* Bug 9147 fixed - Javasci v1 was broken since the introduction of the
                   version 2.


Dynamic Link:
=============

* Under GNU/Linux and Mac OS X, the dynamic link is now creating a subdirectory
  of TMPDIR instead of doing everything in the same directory which could have
  led to unexpected side effects.

* Added 'ilib_include_path' macro to handle includes flag for platform
  compiler. See SEP 55.

* bug 7887 fixed - Under some GNU/Linux distributions (like Ubuntu or Mageia), 
                   the dynamic link process was using the libstdc++ embedded
                   into the Scilab distribution. Now, if the compiler is gcc,
                   the dynamic link guesses the libstdc++ path and use it 
                   uppermost.


Graphics:
=========

* JPG compression level is now set to 95 by default.

* Extends the xs2jpg function with a compression argument to set the quality of
  the exported JPG.
  See Bug 5573 and SEP 54.

* New functions 'comet' and 'comet3d' have been added.

* Bug 9428 fixed - A Java exception was displayed in some cases when using event handlers.


Localization:
=============

* Bug 8994 fixed - Some localization templates were not included and
                   translated.


Help:
=====

* Creation of a "complex" sub-section of "Elementary Functions".

* Bug 6975 partially-fixed - Beginning of the documentation of xlabel(),
                             ylabel() and zlabel() functions.

* Bug 7072 fixed - Fix various issues in the documentation.
                   Thanks to Yves Barois.

* Bug 7305 fixed - issparse was not documented.

* Bug 9362 fixed - An exception was thrown on the pdf doc generation.

* Bug 9439 fixed - There was no entry "Scilab Home" in help toc list.


Compilation:
============

* Option --without-scicos renamed to --without-xcos

* Option --without-ocaml renamed to --without-modelica

* Detection of Jgoodies / looks updated under GNU/Linux and Mac OS X.

* g77 is no longer supported. gfortran is replacing it.

* Explicit check Java version (at least 1.6).

* Bug 7049 fixed - JgraphX version was checked even if the --without-xcos
                   flag was set.

* Bug 9018 fixed - --without-xcos disabled the detection of jhdf5 but was still
                   building xcos.


Optimization:
=============

* semidef help page updated.

* Formatting of the leastsq help page updated.

* Bug 7723 fixed - The demonstrations of the nmplot functions generated
                   warnings.


Numerical Maths:
================

* Bug 7101 fixed - The roots had convergence problems for some polynomials.
                   The default algorithm has been changed to the eigenvalue
                   of the companion matrix.

* Bug 9350 fixed - abs(complex(%nan,0)) returned zero instead of nan.



CACSD Module:
============

* Bug 6744 fixed - p_margin() returned an erroneous result (new fix).

* Bug 9205 fixed - Optional argument added to nyquist to control the drawing of
                   the symetric part and a new function added for high
                   definition zoom.


Bug Fixes:
==========

* Bug 5687 fixed - The window title of a figure was not immediately updated when
                   figure_name was changed.

* Bug 6789 fixed - The page header was not printed with
                   toprint(["line 1" "line 2"],"Page header").

* Bug 6945 fixed - Conflicting options were confusing Scilab argument processing
                   (example: scilab-cli with the -nw option).

* Bug 7954 fixed - Using TCL_EvalStr randomly froze Scilab.

* Bug 7976 fixed - help/cleaner_help.sce was not called when cleaning a toolbox.

* Bug 8317 fixed - Examples of Matplot1 updated.

* Bug 8371 fixed - Each thread used to get the command line was not detached
                   from the JVM.

* Bug 8423 fixed - With certain options of the configure, the build process
                   failed.

* Bug 8595 fixed - When exporting with several windows opened, the Dialogbox options 
                   did not appear at the front.

* Bug 8675 fixed - The complex function was undocumented.

* Bug 8959 fixed - winqueryreg returned an incorrect value if the value name was
                   false.

* Bug 8971 fixed - The termination rule of optim was unclear.

* Bug 9070 fixed - The demonstration "Simulation/Wheel Simulation/Simulation
                   with ode" displayed a warning about an obsolete use of
                   ilib_for_link.

* Bug 9082 fixed - It was not possible to preview LaTeX with second LaTeX
                   string displayed.

* Bug 9083 fixed - Bad output with prettyprint and syslin.

* Bug 9113 fixed - setlookandfeel could led to an exception.

* Bug 9121 fixed - Doc generation did not fail when an invalid image was displayed.

* Bug 9125 fixed - Scilab could not start when called through a symbolic link.

* Bug 9126 fixed - Some new files of the module/toolbox skeleton introduced in
                   Scilab 5.3.1 were not installed.

* Bug 9128 fixed - In help page of dir(), infos about bytes field were missing.

* Bug 9139 fixed - Scilab 5.3.1 had an unexpected dependency on GLIBC_2.7
                   causing Scilab to crash under old RHEL / CentOS.

* Bug 9141 fixed - An extra empty line was present in the console.

* Bug 9159 fixed - grand(1,3,'uin',1,0) crashed Scilab.

* Bug 9168 fixed - Bad command line width when completion window was displayed
                   at the bottom of the console.

* Bug 9170 fixed - 'help clink' threw an exception.

* Bug 9195 fixed - Empty lines entered in Scilab console were displayed in
                   the history browser.

* Bug 9200 fixed - perl.sci script crashed under Windows with a wrong error
                   message.

* Bug 9211 fixed - set("userdata", list()) crashed Scilab.

* Bug 9232 fixed - Play the example in 'help about' in NW mode led to an
                   exception.

* Bug 9246 fixed - It was not possible to insert an integer in an integer array
                   with a different integer type.

* Bug 9250 fixed - mkdir used to display a wrong message when called with
                   two inputs arguments.

* Bug 9264 fixed - Important loss of performance on some string management
                   functions in api_scilab (example: on mputl).

* Bug 9270 fixed - The contour function was broken.

* Bug 9279 fixed - Add a version number in configuration.xml.

* Bug 9280 fixed - wavread failed to read PCM 32 bit float file format.

* Bug 9300 fixed - Scilab crashed when one tried to complete an empty mlist.

* Bug 9442 fixed - Sometimes, an exception could be thrown in the console.

* Bug 9457 fixed - set("figure_style", "new") returned a error message.


            Changes between versions 5.3.0 and 5.3.1 of Scilab
            =================================================


Three main improvements particularly deserve to be enlightened with Scilab 5.3.1:
* Improvements of the current features, mainly focusing on usability, better user
  experiences  and stabilization.
* Consolidation of the undertaken efforts with Scilab documentation (content,
  integration with Scilab and Xcos, etc).
* Integration of the Japanese documentation into the Scilab distribution.
  Thanks to Rui Hirokawa.


Console:
========

* Scilab provides an on-the-fly rendering when a LaTeX expression is being typed.

* The console can now be scrolled with Page Up/Down keyboard keys.
  See Bug #6251.

* Performance improvements on the display of long computation with a lot of
  output (the more text was entered in the console, the slower was the display).
  See Bug #4840.

* Evaluate Selection With Echo/No Echo Right click options added in
  the console.
  See Bug #8749.


Documentation:
==============

* Homepage added in the help browser to highlight the new features of Scilab
  and list the online available resources.

* Add the capability to increase or decrease font size.
  Shortcuts under Windows and Linux: CTRL + + / CTRL + - or CTRL + mouse wheel
  increases/decreases.
  Shortcuts under Mac OS X: CMD + + / CMD + - or CMD + mouse wheel
  increases/decreases.


* Add an incremental search bar to search for text in the browser.
  Shortcut under Windows and Linux: CTRL + F
  Shortcut under Mac OS X: CMD + F

* Enable the creation of links in between external modules with the following
  syntax:
<link type="scilab" linkend="mytoolbox/exec/macros/fooexec.sce">macros/fooexec.sce</link>

* Enable the creation of links between an external module and Scilab main documentation
  with the following syntax:
<link type="scilab" linkend="scilab/help/bvode">bvode</link>

* Enable the execution and the edition of any Scilab example.

* Enable the execution of demos with the following syntax:
<link type="scilab" linkend="scilab/demos/gui/demos/uicontrol_plot3d.dem">Uicontrol demo</link>

* Enable the opening of a Xcos diagram from the help browser:
<link type="scilab" linkend="scilab.xcos/xcos/demos/bounce.xcos">Open bounce.xcos</link>

* Enable the opening of Scilab macros with Scinotes from the help browser:
<link type="scilab" linkend="scilab.scinotes/elementary_functions/macros/log2.sci">Edit log2.sci</link>

* "See also" links to other interesting functions now display the corresponding
  function description.

* In the documentation, Java, C, C++ and XML codes are now colorized if the role
  in the program listing is set (role='java', role='c', role='cpp' or role='xml').

* The categories "Bitwise", "Elementary matrices", "Matrix manipulation",
  "Matrix operations", "Set operations", "Signal processing", "Symbolic",
  "Search and sort" have been created for the elementary functions module.

* Help pages for the bitxor, bitcmp, bitget and bitset functions have been
  created.

* In documentation's examples, when an undocumented function is used,
  a warning is displayed and there is no link on that function name.

* Bug 3716 fixed - There was no "find text" option in the right area of
  the help window.

* Bug 5362 fixed - There were problems with the same xml:id when different
  cases were used.

* Bug 5511 fixed - printf_conversion help page was unclear.

* Bug 5796 fixed - No way to increase/decrease font size.

* Bug 6514 fixed - 'help %k' did not open the correct corresponding
  help file.

* Bug 7069 fixed - Former documentation about the documentation build process
  was removed.

* Bug 8463 fixed - scilab PDF (fr_FR and pt_BR) were not fully generated.

* Bug 8677 fixed - The factorial function was not documented.

* Bug 8690 fixed - Typo fixed in min French help page.

* Bug 8756 fixed - The file id was only expected in refentry which caused
  xmltojar not to process XML files.

* Bug 8915 fixed - ASCII art removed in histplot help page.

* Bug 8937 fixed - The tag <screenshot> was not handled.

* Bug 9019 fixed - dsearch help page was unaccurate.

* Improved the help page of ode.


Xcos:
=====

* Many improvements on Xcos English documentation (better description
  of the blocks, arguments, examples, etc).
  For now, functions such as EXTRACTBITS, DLATCH, DFLIPFLOP, INTMUL,
  SRFLIPFLOP, CONVERT, BITSET, BITCLEAR, etc have been fully documented
  and provided with examples of use.
  Many thanks to Bernard Dujardin.

* Capability added to display directly Xcos sample schemas from the help browser.

* Beginning of the French translation of Xcos documentation.
  Thanks to Bernard Dujardin.

* Improvements of error reporting.

* The demos have been updated to use the latest features.

* Most of the old user interface for Xcos/Scicos control functions are now
  deprecated and will be removed in Scilab 5.4.0.
  See Bug #7232.

* Bug 5346 fixed - Title was not used as default file name.

* Bug 5680 fixed - Labels were not visible by default.

* Bug 6705 fixed - When drag and drop was used, port alignment was not correct.

* Bug 7194 fixed - Block UUID could not be used on a block implementation.

* Bug 7363 fixed - Linking CBLOCKs implementation with an external library was
                   impossible.

* Bug 7424 fixed - Integer parameters could not be modified when loading a diagram.

* Bug 7639 fixed - Generating code for Scilab function block was failing with
                   a syntax error message.

* Bug 7953 fixed - No error message was displayed when the diagram was invalid.

* Bug 8042 fixed - After a region to superblock action, the ports were wrongly
                   positioned.

* Bug 8064 fixed - Without the grid, a SplitBlock could not be moved.

* Bug 8223 fixed - Xcos did not produce a visible error when Saxon was used as
                   XML DOM implementation.

* Bug 8235 fixed - Using masked superblock did not work when parameters were
                   defined in the inner-context.

* Bug 8440 fixed - The SVG background was not exported to SVG.

* Bug 8480 fixed - The DeBug block did not have a valid function type.

* Bug 8580 fixed - Mirror and flip icons were inverted.

* Bug 8639 fixed - Simulations did not stop at Xcos close.

* Bug 8642 fixed - The getDiagramVersion unit test failed.

* Bug 8644 fixed - Example added to define customed block.

* Bug 8658 fixed - Accentuated characters use on file name lead to a Modelica
                   compiler error.

* Bug 8674 fixed - xcosShowBlockWarning was not protected against arbitrary
                   arguments.

* Bug 8696 fixed - An exception was thrown while linking and deleting the
                   source block.

* Bug 8711 fixed - The "Cont. Plant-Hybrid Observer" demo contained a translated
                   origin point which led to strange links creation.

* Bug 8731 fixed - Copied superblocks lost some graphical settings.

* Bug 8737 fixed - Compilation failed when a diagram contained any hidden links
                   (wrong indexes issues).

* Bug 8740 fixed - In the Modelica initialization GUI, discrete parameters were
                   not added to the unknown sum.

* Bug 8741 fixed - In the Modelica initialization GUI, the "fixed" checkbox
                   disabled some fields.

* Bug 8743 fixed - In the Modelica initialization GUI, the fixing states did
                   not de-fix derivatives.

* Bug 8798 fixed - The context parameter of scicos_simulate was not taken into
                   account.

* Bug 8818 fixed - Generic Modelica blocks did not transform Scilab exponential
                   notation into Modelica one.

* Bug 8822 fixed - Remove an implicit link connected to a split block could lead
                   to an unconnectable port.

* Bug 8867 fixed - Unconnected links could not be deleted.

* Bug 8870 fixed - TKSCALE block did not work without TCL.

* Bug 8884 fixed - Using region to superblock with ports on the same position
                   was resulting to a non-reconnected link.

* Bug 8896 fixed - Under Windows, a multi-line context could not be edited twice.

* Bug 8945 fixed - Blocks did not update their shape on parameters update.

* Bug 8952 fixed - "lincos" did not produce a valid error message when the
                   diagram had no inputs nor outputs.


Scinotes:
=========

* Refactorization of the printing.

* Add the capability to export code to PDF, PS, EPS and RTF.

* Name of the current function and position (line, column) are now displayed
  in the status bar of Scinotes.

* Replace CTRL key by META key under Mac OS X.

* A rectangular selection tool is now available. CTRL+selection
  (under Windows and Linux) and CMD+selection (under Mac OS X) activates it.
  This improves the selection action (of data for example).

* "edit" function can now open a file at a given line number.
  See Bug #9047

* Content assist:
  * An identifier is now shown in the text editor when a wrong variable is typed.
  * When typing a closing char (e.g. ')',']',...), user do not need to insert it
    again (like in Eclipse).

* Bug 8095 fixed - With OpenJDK, the size of the dialog 'Set colors' was bad.

* Bug 8697 fixed - An exception was thrown when an empty document was reload.

* Bug 8709 fixed - Default font had no Japanese glyphs.

* Bug 8714 fixed - Problem was encountered with Incremental Search bar when
  Code Navigator was docked.

* Bugs 8715, 8716 fixed - Problems with dnd'ed tabs.

* Bug 8764 fixed - File permissions were not checked.

* Bug 8885 fixed - Anchors were removed on a Replace All action.

* Bug 9000 fixed - '1+..' was not correctly colorized.


New Functions
=============

* 'repmat' function added. This function replicates and tiles a matrix.
  See SEP #49.


Graphics:
=========

* Fix a critical Bug in the graphic rendering which was crashing Scilab.
  This Bug was occuring on some Buggy drivers (Intel HD graphics for example).
  See Bug #7526 for more information.

* Bug 2913 fixed - In the histplot function, the legend was displayed over the
                   graphic.

* Bug 4818 fixed - It was not possible to export a figure with a lot of circle
                   marks.

* Bug 7509 fixed - Update of the legend help page regarding some previous
                   changes.

* Bug 7526 fixed - Scilab crashed under Windows and Intel HD graphics cards
                   while executing a plot command.

* Bug 7836 fixed - The sub_tics property was not correctly tracking the number
                   of displayed sub-ticks. Thanks to Paul Griffiths for the patch.

* Bug 7990 fixed - Box thickness was badly exported in vectorial formats
                   (SVG/PDF/...). Thanks to Paul Griffiths for the patch.

* Bug 8163 fixed - datatipToggle could not be called without argument as
                   stated in the help page.

* Bug 8518 fixed - When graphic export failed, error messages were not
                   clear.

* Bug 8766 fixed - For small input matrices, the "Matplot1" function was not
                   correctly taking into account its rectangle argument.

* Bug 8786 fixed - When entering wrong input arguments in 'getcolor' function,
                   the error message had no sense.


GUI:
====

* user_data for a uimenu was not displayed.

* Bug 8203 fixed - It was not possible to close the font chooser or the
                   color chooser with the escape key.

* Bug 8717 fixed - Bad window's title just after creation.

* Bug 8793 fixed - 'waitbar' function crashed when the handle was not initialized.

* Bug 8826, 8827 fixed - userdata property for a menu crashed Scilab.

* Bug 8838 fixed - uimenu without input argument crashed Scilab.

* Bug 9044 fixed - Updating a LaTeXed label of uimenu() misworked.

Statistics:
==========

* Bug 8560 fixed - The grand function failed on 64 bits machines.


Optimization:
=============

* Improvement of the karmarkar linear optimization solver:
  * User can configure an output function.
  * The number of iterations can be put as an output argument.
  * x0 can be put as an optional argument.
  * Management of inequality constraints.
  * Management of bounds.
  * Computation of the exitflag.
  * Computation of the Lagrange multipliers.
  * Documentation provided for the rtolf and gam parameters.
  * Documentation provided for the stopping rule.
  * More examples provided.

* Bug 7164 fixed - The help for neldermead does not give examples of
                   constrained optimization.
                   Added an example of bound constrained optimization and
                   and example of nonlinearily constrained optimization.

* Bug 7165 fixed - The neldermead help page stated that the constraint
                   is negative, while it is positive.

* Bug 7166 fixed - The second example in neldermead's help was wrong.

* Bug 7193 fixed - The karmarkar help page did not document the
                   eps, gamma, and crit arguments.

* Bug 8719 fixed - The karmarkar function printed unwanted messages.

* Bug 8720 fixed - The karmarkar function stopped too early in the iterations.

* Bug 8726 fixed - The karmarkar function produced a division-by-zero error.

* Bug 8727 fixed - The karmarkar function required the initial guess x0.

* Bug 8775 fixed - The karmarkar function did not detect unbounded problems.

File I/O:
=========

* mkdir is now able to create recursively directories.
  See Bug #8529.


Obsolete Functions:
===================

* 'loadScicosLibs' is obsolete and will be removed on 5.4.0, please use
  'loadXcosLibs' instead.

* ilib_for_link, ilib_build will not accept files with ".o" or ".obj" extensions
  for source files in Scilab 5.4.0.


Removed Functions (previously declared as obsolete):
====================================================

* 'maxi' removed. Please use 'max' instead.

* 'mini' removed. Please use 'min' instead.

* 'VCtoLCCLib', 'configure_lcc', 'with_lcc', 'chooselcccompiler', 'findlcccompiler'
  are now available as ATOMS modules.
  See http://atoms.scilab.org/toolboxes/LCC-windows
  or type atomsInstall('lcc-windows') in Scilab.

* 'oldbesseli', 'oldbesselj', 'oldbesselk' and 'oldbessely' removed.
  Please use 'besseli', 'besselj', 'besselk' and 'bessely' instead.


Javasci:
========

* Bug 8807 fixed - Javadoc of javasci v2 was not installed under GNU/Linux and
                   Mac OS X.


Demonstration:
==============

* A demonstration (Non isothermal plug flow reactor model) of ode usage has been
  added. Thanks to Jakub Kopac.


Module Skeleton:
================

* New example added (function with copy instead of reference).


Localization:
=============

* Bug 8096 fixed - With Russian localization, the banner was not centered.


Installer:
==========

* Bug 8825 fixed - Mac OS X installer reported a wrong version of Scilab.


Quality Assurance:
==================

* C/C++/Fortran test coverage available on the Scilab source code.
  Add "--enable-code-coverage" to the configure under GNU/Linux and Mac OS X
  Start the unitary tests or launch Scilab
  "make code-coverage" creates the report into "/coverage/results/"

* Using the configure with the option "--enable-compilation-tests" launchs Java
  unitary tests (based on TestNG).

* Bug 6684 fixed - Some Xcos unitary tests encountered problems at native
                   libraries loading.

* Bug 6873 fixed - A jar file was not available when testng was called.


Compilation:
============

* -Wimplicit removed for Gfortran in the enable deBug mode

* Intel Compiler XE 2011 update 1 C++/Fortran used to build Scilab under Windows.

* Upgrade to autoconf 2.67

* Update of JLaTeXMath to version 0.9.4

* Bug 3196 fixed - Minimal version of pcre (7.2) was not checked by the
                   configure

* Bug 8053 fixed - For an unknown reason, libtool refused to set the Wl flag
                   when the Gfortran compiler was set as
                   ./configure F77=gfortran

* Bug 8961 fixed - Ubuntu 11.04 explicitly forced the as-needed option to the
                   linker. It has been disabled.


Dynamic link:
=============

* Intel Fortran Composer XE 2011 (12) used by default if it is detected.


Scilab:
=======

* Scilab startup script of the binary is now automatically synchronized with
  the one from the source tree.



Bug Fixes:
==========

* Bug 476  fixed - find did not work when there were more output arguments
                   than matrix dimensions.

* Bug 4930 fixed - When the console was resized from 0-height to x-height,
                   lines() were not correctly updated.

* Bug 5551 fixed - With the function dir(), add a trailing filesep() for each
                   directory.

* Bug 6884 fixed - Issues with the buttmag help page fixed.

* Bug 7152 fixed - It was not possible to export LaTeX in SVG.

* Bug 7377 fixed - Thrownan failed on empty matrix.

* Bug 7691 fixed - Giving complex arguments to inverse degree trigonometric
                   functions produced inconsistent results.

* Bug 8007 fixed - Hit UP key when the message 'Continue display...' was
                   displayed could lead to a crash.

* Bug 8383 fixed - "void" and "undefined" items in list were not saved by
                   import and export hdf5 functions.

* Bug 8435 fixed - squarewave(t) failed for t<0. Thanks to Samuel Gougeon for
                   the fix.

* Bug 8478 fixed - A searched path was missing for docbook xsl under Gentoo.

* Bug 8492 fixed - When the documentation was generated, error messages were not
                   displayed in the console but in the terminal.

* Bug 8563 fixed - Scilab under GNU/Linux 64 bit was unable to load files bigger
                   than 2GB.

* Bug 8732 fixed - strchr and strrchr did not work with a non-ASCII character
                   needle.

* Bug 8765 fixed - A(1:N,j)=[] returned wrong values if N>size(A,1).

* Bug 8783 fixed - loadmatfile could not load hyper matrices of integer types.

* Bug 8789 fixed - read function crashed under Windows XP 64 bit platform.

* Bug 8809 fixed - Bad top link in the chm files.

* Bug 8820 fixed - squeeze function returned 2D hyper matrix instead of regular
                   2D array function also extended to all array types included
                   cells ans structs.

* Bug 8828 fixed - "savematfile" could no more save a structure.

* Bug 8830 fixed - In call_scilab, TerminateScilab did not clear the last
                   error.

* Bug 8836 fixed - fileparts crashed when a matrix of strings was given as
                   input argument.

* Bug 8837 fixed - fscanfMat did not read a malformatted text with many blanks
                   at the end of file.

* Bug 8845 fixed - An example in call_scilab module with Visual Studio has been
                   rewritten to use api_scilab.

* Bug 8854 fixed - warning about Bug 7575 disabled for new Nvidia drivers.

* Bug 8897 fixed - Typo fixed.

* Bug 8906 fixed - Bad links to javasci javadoc on the website.

* Bug 8912 fixed - fullpath did not return same result under Windows and Linux.

* Bug 8919 fixed - Typo fixed.

* Bug 8954 fixed - Print the console led to an exception.

* Bug 9034 fixed - atomsVersionCompare failed on version with '-0'.

* Bug 9096 fixed - Sometimes an exception was thrown at the Scilab's startup.


            Changes between versions 5.3.0-beta-5 and 5.3.0 of Scilab
            ========================================================


SciNotes:
=========

* Bug 8465 fixed - Under GNU/Linux, it was not possible to paste with the
                   middle click.

Xcos:
=====

* Bug 8348 fixed - Sometimes when openning the parameters of a block, its ports
                   changed their index (and position).

* Bug 8407 fixed - The blocks was always resized after a parameters edition.

* Bug 8441 fixed - "xcos_block" abstract block were not removed on the
                   compilation.

* Bug 8483 fixed - Multiplying a scalar and vector using the MATMUL block did not
                   work.

* Bug 8540 fixed - Fix a typo on the "highligthed" term.

* Bug 8556 fixed - Creating a split block from a link did not work.

* Bug 8569 fixed - Deleting a link with a split-block did not reconstruct any
                   valid link.


Bug Fixes:
==========

* Bug 4673 fixed - An example of "addinter" function call was added.

* Bug 4821 fixed - help file 'man' was obsolete.

* Bug 5686 fixed - Complex number manipulation could crash Scilab.

* Bug 6768 fixed - input did not return good value when RETURN was pressed
                   on Windows XP.

* Bug 7694 fixed - There were no key shortcuts in the command history browser.

* Bug 7695 fixed - Only the first selected line was deleted in command history
                   browser.

* Bug 7714 fixed - diag, triu, tril applyed to rational matrices  changed
                   the formal variable name

* Bug 7727 fixed - The help page of sp2adj was not correct.
                   Improved the help page of adj2sp.
                   Added unit tests for sp2adj and adj2sp.
                   Improved implementation by checking the input arguments.

* Bug 7806 fixed - [a, b] = dos("git 1>&2") returned a wrong error.

* Bug 7849 fixed - eqfir / remez functions crashed under MacOSX due to
                   a lapack Bug on slamch function.

* Bug 7900 fixed - inserting a rational in a rational hymermatrix  generated
                   invalid rationals entries (with zero denominators)

* Bug 8091 fixed - Scilab was unable to find fortran compiler under MacOSX.

* Bug 8175 fixed - roots function did not converge under MacOSX due to
                   a lapack Bug on slamch function.

* Bug 8254 fixed - The nearfloat function did not work properly for near
                   denormalized numbers.

* Bug 8279 fixed - error(144) returned a unexpected message.

* Bug 8390 fixed - a "Warning : redefining function: mputl" displayed if you
                   did not install fftw on Windows.

* Bug 8412 fixed - find returned incorrect values with big vector.

* Bug 8448 fixed - 'whereis' function with a string as input argument did not work
                   with ATOMS functions.

* Bug 8459 fixed - strsplit failed when second input argument was a "/"

* Bug 8462 fixed - bvodeS crashed on Windows and did not return a error.

* Bug 8468 fixed - size with 2 input arguments did not work for hypermatrices.

* Bug 8475 fixed - It was impossible to get the data of an entity through
                   the graphic editor.

* Bug 8481 fixed - Scilab crashed when display a rational containing a complex.

* Bug 8505 fixed - help was not generated on Windows, if there was a path name
                   with some spaces.

* Bug 8528 fixed - length returned a wrong size for malformed characters.

* Bug 8531 fixed - "edit" function failed to open a defined macro loaded on stack.

* Bug 8532 fixed - size of input arguments of read_csv was not verified.

* Bug 8540 fixed - Fix a typo in Xcos.

* Bug 8582 fixed - Under Linux & Mac OS the waitbar window was too small and we
                   could not see the bar inside it.

* Bug 8587 fixed - readNamedMatrixOfDouble (readCommonNamedMatrixOfDouble)
                   returned wrong values.

* Bug 8598 fixed - Fix a typo in the french help page of "return".

* Bug 8601 fixed - An help page was misdiplayed when the title contains html tag.

* Bug 8611 fixed - Path of few images in documentation was bad.



            Changes between versions 5.3.0-beta-4 and 5.3.0-beta-5 of Scilab
            ===============================================================

Due to an important change from Apple in the Java environment, a new beta
release of Scilab has been decided. This will allow further testing for
this architecture.


Documentation:
==============

Massive improvement of the documentation system:
* Improved navigation in the help browser and the web interface.
* Improved look and feel in the help browser and the web interface.
* Colorisation of the Scilab code.
* In the examples, functions can be clicked to see their help pages.
* Improved maintainability.
* Faster build process (30 to 60 minutes => less than 30 seconds).

In the meantime, a work of categorisation has been started in order to
improve the Scilab documentation. More updates will follow in the upcoming
versions of Scilab.

* Documentation of trigonometry functions moved in a dedicated documentation
  section.

* Documentation of profiling functions moved in a dedicated documentation
  section.

* Documentation of color management in graphics functions moved in a dedicated
  section. See Bug 7048.

* Documentation of floating point functions moved in a dedicated section.
  See Bug 7052.

* Documentation of Integer representation functions moved in a dedicated
  section. See Bug 8268.

* Documentation of adj2sp was not in the sparse help section. See Bug 8269.

* Documentation of discrete mathematics functions moved in a dedicated section.
  See Bug 8270.

* Bugs 8357, 8358 fixed - There was an encoding problem under Windows.

* Bug 8367 fixed - In synopsis function, &apos were displayed rather than '.


Javasci:
========

A new version of the Java => Scilab API called Javasci v2 is now included into
the Scilab distribution.
This version fixes most the issues and limitations of the previous version.
This API is automatically fully tested through unitary tests and code coverage.
Please, note that previous version of Javasci remains available and shipped
into the Scilab distribution.
Advanced documentation and examples will be provided in the stable release of
Scilab 5.3.0.


License:
========

The license of all demos and examples have been changed to the 3-clause BSD
license. The domain public license was not valid in some countries.


Security issue:
===============

* Fix a (minor) security issue. CVE-2010-3378
See: http://security-tracker.debian.org/tracker/CVE-2010-3378


Known incompatibilities:
========================

* Due to Bug 7364 fix, "Checked" uimenus have to be manually managed, these
  uimenus are no more (un)checked on mouse click. Moreover, they can now have
  a callback.

* The graph of the nyquist function (CACSD module) has been symetrized.


Xcos:
=====

* Loop link on a block produced some error when openning it.

* Bug 8081 fixed - The context was not checked before being set.

* Bug 8140 fixed - Masked suberblock became empty when copied and pasted
                   between two xcos schemes.

* Bug 8141 fixed - Superblock mask became empty when reopening a file.

* Bug 8186 fixed - Demos with Superblocks were not simulating.

* Bug 8195 fixed - Creating implicit links was not working.

* Bug 8220 fixed - Improve the link creation with split block adding.

* Bug 8291 fixed - Some blocks (PDE, SQRT) did not display any label.

* Bug 8322 fixed - Rotated shape had 2 different bounds (icon and shape).

* Bug 8326 fixed - Region to superblock action did not work with OUT_f blocks.

* Bug 8331 fixed - Block.doc was not cleared on superblock diagrams.

* Bug 8397 fixed - the simulation was blocked on a Modelica translator error.


* Bug 8400 fixed - start demos from Xcos menu (? --> Xcos demos) did not work.


* Bug 8417 fixed - An h5 diagram open a new diagram before the end of the
                   decoding.


Graphic export:
===============

* Decrease the amount of feedbacks when exporting a graphic.


Graphics:
=========

Thanks to Samuel Gougeon, many improvements on the polarplot function:
* Clipping was missing. See Bug #7007.
* In some common cases, values of radial labels were wrong with respect to the
plotted radial frame or/and were lacking of digits.
* When the plot was centered around an angle far from 45°, the radial labels
were completely clipped.
* Sampling of the angular frame is improved according to the number of drawn
quadrants.
* Improved H&V positionning of angular labels
* Polar frame drawn in grey instead of black for a better data visibility.
* if the basic radius is >= 10^4 or <=10^-4, LaTeX is used to display smart
$..10^p$ exponents instead of the D+/-## console style display.
* Finally, their positioning is still improved in order to avoid their mutual
overlaying for big values.

* Bug 7110 fixed - When inserting a structure in a handle, the handle was
  overwritten.

* Bug 7492 fixed - If its "data_mapping" property was set to "direct", the
  Grayplot object's data seemed to be transposed when displayed.

* Bug 8168 fixed - Adding a datatip in a figure that mix surface and curve
                   generated a zero divide error.


Completion:
===========

* Add a completion on the fields name of a mlist or a tlist.


SciNotes:
=========

* Incremental search added with the shortcut: the F2 key.

* Add a "Save file in..." menu which gives the capability to save a file
  in favorite directories.

* Bug 4399 fixed - It was not possible to distinguish lines from inner functions
                   and from normal functions.

* Bugs 5321, 7760 fixed - It was not easy to configure the tabulation.

* Bug 7263 fixed - Scilab code could not be copy as HTML to keep the formatting.

* Bug 7762 fixed - A keyword was not fully helpable depending on the caret
                   position.

* Bug 7806 fixed - Blocks elseif...end and else...end were not highlighted.

* Bug 8095 fixed - The Set Colors window was too height with OpenJDK.

* Bug 8135 fixed - An exception was thrown when restoring files on MacOS X.

* Bug 8166 fixed - All the checkboxes in SciNotes 'Preferences' menu were
                   inefficient.

* Bug 8227 fixed - The commands evaluated with "Evaluate selection" were not
                   stored in history.

* Bug 8229 fixed - There was a problem of selection after an indentation.

* Bug 8230 fixed - Save As action did not propose the current file name as
                   default.

* Bug 8267 fixed - The file encoding was not correctly detected and no default
                   encoding was set.

* Bug 8277 fixed - Under Windows, exceptions were thrown when a key was hit on
                   a keyboard in chinese mode.

* Bug 8304 fixed - A Java exception was thrown on Exit menu.

* Bug 8309 fixed - Bad display of the line number in whereami mode with broken
                   lines.

* Bug 8359 fixed - An exception was thrown on Save As action.

* Bug 8368 fixed - Incremental search did not correctly highlight the pattern
                   under Windows.

* Bug 8381 fixed - It was not logical to add favorite dirs only in "Open file
                   in" action.

Completion:
===========

* Add completion on fields of mlist, tlist, struct and graphic handles in console.


Compilation:
============

* Bug 7489 fixed - VPATH was failing on scicos_blocks generation headers.
                   Note that the VPATH build system is not fully available.

* Bug 7606 fixed - Build option -Wold-style-cast removed for C++.

* Bug 8013 fixed - In some cases, the build of the compiler of modelica failed.

* Bug 8274 fixed - Improve detection of Java regarding Apple changes:
                   Java for Mac OS X 10.6 Update 3 and 10.5 Update 8 Release
                   Notes

Dynamic link:
=============

* Intel Fortran Composer XE 2011 (12) support on Windows x86 and x64 platform.


Bug Fixes:
==========

* Bug 3175 fixed - There were no shortcuts in the console menus.

* Bug 4738 fixed - The number of columns in the console was badly calculated.

* Bug 4827 fixed - The test_run function did not properly work with modules
                   begining with a "n".

* Bug 5486 fixed - It was not possible to precise the text of a link in
                   help_from_sci.

* Bug 6942 fixed - Explicit dependency on the libX11 on scilab-cli was existing.

* Bug 7242 fixed - API Scilab returned bad address for named variables
                   when name was provided by input arguments.

* Bug 7364 fixed - The "Checked" property misworked for uimenus and they could
                   not have a callback.

* Bug 7566 fixed - The handling of immediate_drawing had been broken in zgrid by
                   Bug 6395 fix in 5.3.0-beta-4 version

* Bug 7768 fixed - For cdfgam, the Scale parameter was, in fact, the Rate.

* Bug 7805 fixed - clear called inside an exec while in pause could
                   have unexpected results.

* Bug 7970 fixed - The eager loading of openmp support library was crashing
                   on Linux with unsupported configuration for openmp.

* Bug 7997 fixed - getfreememory function adapted for FreeBSD. Thanks to Rob
                   Farmer.

* Bug 8016 fixed - An error in a german translation gave an error message.

* Bug 8046 fixed - msprintf did not manage tabulation as format.

* Bug 8067 fixed - Nyquist unitary test failed due to a change of the nyquist
                   function. The graph has been symetrized.

* Bug 8068 fixed - Figures were not all closed at the end of a demonstration.

* Bug 8077 fixed - browsevar, scinotes, xcos functions were not disabled in
                   scilab-cli / -nwni mode on Linux and MacOS X.

* Bug 8079 fixed - When the max number of variable available has been reached,
                   it was not possible to leave Scilab.

* Bug 8087 fixed - prettyprint did not handle %inf and %nan.

* Bug 8107 fixed - sum(a,1), cumsum(a,1), .. made Scilab 5.0.3-beta-4 crash
                   in some particular context.

* Bug 8108 fixed - ATOMS: Modules that start by the letter "n" were not well
                   managed by ATOMS. The cause of the issue is the behaviour of
                   the msprintf function, which considers "\n" in file paths as
                   a new line insertion.

* Bug 8110 fixed - ATOMS: When removing a module, its main directory was not
                   deleted even if it was empty.

* Bug 8115 fixed - DisableInteractiveMode() (call_scilab) did not work.

* Bug 8122 fixed - help of write_csv said that it reads file and not that it
                   wrotes.

* Bug 8124 fixed - Some translations were not up to date.

* Bug 8126 fixed - Some demos from the GUI modules were hanging.

* Bug 8128 fixed - A warning was displayed on 'Save' action in a graphic window.

* Bug 8129 fixed - The error message was not cleared in tbx_builder_src if you
                   did not have a compiler.

* Bug 8134 fixed - The font attributes (FontName, FontSize, FontWeight) were not
                   taken into account for "text" uicontrols.

* Bug 8148 fixed - fscanfMat did not read matrix file with tabulation as
                   default separator.

* Bug 8154 fixed - It was not possible to cancel a messagebox with ESC key.

* Bug 8160 fixed - The derivative function did not check for input argument #1,
                   if it was a list.

* Bug 8164 fixed - Typo in the fileparts help page.

* Bug 8170 fixed - Loading HDF5 files in non-Scilab format caused crash
                   of application.

* Bug 8174 fixed - ss2tf produced bad results if applied to a complex
                   state space system.

* Bug 8177 fixed - Format mode was modified by some macros and not restored.

* Bug 8181 fixed - uicontrol did not handle color where composants were outside
                   the range 0-255.

* Bug 8202 fixed - Typo in the localization.

* Bug 8206 fixed - isnum did not manage blanks.

* Bug 8208 fixed - Error in creating the temporary directory in function
                   createScilabTMPDIR.

* Bug 8218 fixed - File association with filename with accentued characters
                   did not work.

* Bug 8232 fixed - Drag and drop of .scg files (on console) used "xload"
                   and not "load".

* Bug 8240 fixed - Demo of polarplot improved (Thanks to Samuel Gougeon).

* Bug 8257, 8258 fixed - fscanfMat did not read matrix file not formated by
                   fprintfMat.

* Bug 8283 fixed - File graphics/help/en_US/datatips datatipLookfor.xml removed.
                   datatipLookfor is an internal function that has not be used
                   directly.

* Bug 8294 fixed - A help page about "supported compilers" has been added.

* Bug 8296 fixed - size( M,3) crashed scilab 5.3.0-beta-4.

* Bug 8299 fixed - ilib_compile did not return enough feedback on Windows when
                   compilation failed.

* Bug 8300 fixed - a(48400, 48400) = 0 crashed scilab.

* Bug 8318 fixed - help window was not brought up to front when already opened.

* Bug 8321 fixed - In some cases, configure_msvc failed and returned a wrong
                   error message.

* Bug 8332 fixed - Some use of clipboard function crashed Scilab.

* Bug 8413 fixed - fscanfMat crashed when file included text lines at the
                   beginning.

         Changes between versions 5.3.0-beta-3 and 5.3.0-beta-4 of Scilab
         ===============================================================

Due to a critical Bug from an underlying library of Scilab, a new beta release
of Scilab has been decided. This will allow further testing and avoid potential
side effects in the future stable release.

Xcos:
=====

* Include all the old-scicos palettes in the binary version.

* Use a user-friendly error message on the palette management implementation.

* Update to the latest jgraphx version.

* Bug 6500 fixed - The ideal transformer block was unusable due to wrong port
                   positioning (update the Transfomer demo).

* Bug 7219 fixed - The old gain demo did not work.

* Bug 7445 fixed - The Kalman filter and the Cont-Plant-Hybrid-Observer demos
                   did not simulate.

* Bug 7716 fixed - Block inter-functions were not loaded after using the
                   xcosPal API and then starting Xcos.

* Bug 7770 fixed - It was not possible to compile a diagram with Modelica
                   blocks.

* Bug 7822 fixed - Xcos menu bar was larger than the Scilab one.

* Bug 7868 fixed - Resizing a super-block with a lot of inputs or outputs
                   sometimes changed the order of the ports.

* Bug 7922 fixed - The Automotive Suspension demo did not work.

* Bug 7923 fixed - The DSUPER block was not documented.

* Bugs 7931, 7933, 7934 fixed - Some artifacts were visible on diagrams such as
                   wrong links and block names.

* Bug 7942 fixed - The "Start" button was not disabled when the diagram was
                   empty.

* Bug 7944 fixed - "Fit diagram to view" was disabled after opening a diagram.

* Bug 7945 fixed - The diagram path was not displayed in the title bar.

* Bug 7995 fixed - The "export diagram" actions did not work with file without
                   extension.

* Bug 8014 fixed - The TEXT_f.png file was not present on the source tree.

* Bug 8025 fixed - The simulation parameter dialog window was modal.

* Bug 8052 fixed - The I/O block value was used for computational purpose.

* Bug 8071 fixed - The PROD_f block in the Fibonacci demo was not well
                   rendered.

* Bug 8073 fixed - The child context could not be used to specify masked
                   super-blocks parameters.

* Bug 8097 fixed - Scilab crashed when plotting on a figure with "Units"
                   property set to "pixels".


Statistics:
===========

* Examples added in the "cdfnor" help page.


SciNotes:
=========

* LaTeX formula in comments or in a string are now rendered when the mouse
  cursor is over.

* Block text (function, if, while...) are now easily selectable. See Bug #7546.

* Bug 7524 fixed - Tabulation representation had a bad size in whitespace mode.

* Bug 8008 fixed - The "Edition" menu had an incorrect name (renamed to "Edit").

* Bug 8024 fixed - No key Insert had no effect.

* Bug 8027 fixed - In a splitted view, a frame was not informed of a change in
                   the other frame.

* Bug 8056 fixed - Comments were badly colorized after a command without
                   parenthesis.

* Bug 8061 fixed - An unwanted message box was displayed when an unsaved file
                   was executed.

* Bug 8085 fixed - It was not possible to disable the horizontal line wrapping.


GUI creation:
=============

* Now, it is possible to use LaTeX or MathML as label for uimenu or pushbutton
in uicontrol. See Bug #7366.

* Bug 7082 fixed - Setting the "enable" property had no effect on "listbox" and
                   "text" uicontrols.

* Bug 7365 fixed - "delete" applied to a "Checked" uimenu destroyed the handle
                   without updating the display.

* Bug 7367 fixed - uimenu properties display was not consistent.

* Bug 7368 fixed - Setting the "Visible" property to "off" had no effect.

* Bug 7898 fixed - In a "popupmenu" style uicontrol, when the list in the
                   "String" parameter has several repeated values, the returned
                   selected value was always the first value in the list and not
                   the selected one.


Editvar:
========

Actions such as Copy, Cut, Paste, Undo and Redo have been added.

The integer matrices are now handled.

In a cell of editvar, expressions such as =2+2 are now accepted. See Bug #7319.

A refresh button to synchronise the variable from Scilab has been added.
See Bug #7321

* Bug 7325 fixed - An error message did not respect the standard format.

* Bug 7327 fixed - Memory problems could lead to a crash without warning.

* Bug 7338 fixed - %nan and %inf were not correctly displayed.

* Bug 7340 fixed - %t or %f were not correctly displayed.

* Bug 7839 fixed - A global variable could be modified.


Graphics:
=========

* The icons have been updated to the Tango Art libre icons and to the Inkscape
  themable icons.

* Bug 6395 fixed - evans display improved to take special care of the
                   root loci points where complex conjugates roots
                   collapsed in a pair of real roots. Datatips
                   customization added. Root locus grids "sgrid" and
                   "zgrid" superposition improved.

* Bug 6686 fixed - The automatic sub-ticking gave wrong results.
Thanks to Paul Griffiths for the fix.


Call_Scilab:
============

* In StartScilab function, "int *Stacksize" argument changed to "int Stacksize"


Compilation:
============

* Fix a problem in the detection of some ARM cpus.


Bug Fixes:
==========

* Bug 6514 fixed - "help %k" and "help %asn" pointed to the symbols help page.

* Bug 6767 fixed - The "uicontrol" help page said the "Style" property of an
                   uicontrol could be set after creation but it did not.

* Bug 7095 fixed - The margins unit_test failed.

* Bug 7163 fixed - tree2code wrongly replaced every (:) occurence in a function
                   definition with (eye()).

* Bug 7140 fixed - Modifying the stack with gstacksize() and stacksize()
                   is now forbidden in concurrent execution context.

* Bug 7231 fixed - mtlb_num2str did not manage second input argument.

* Bug 7569 fixed - The number of accurate digits during inversion of cdfbet,
                   cdfgam, cdfbin, cdfchi, cdfchin, cdff, cdffnc, cdfnbn, cdfpoi
                   was only 8. Changed to 13.

* Bug 7640 fixed - xs2pdf, xs2eps, xs2emf crashed if filename prefix had less
                   of three characters.

* Bug 7690 fixed - Long lines removed in the parallel_run help page.

* Bug 7748 fixed - The help page of rcond was wrong: only one output
                   argument is provided.

* Bug 7862 fixed - "savematfile" only saved the first string in the vector
                   (or matrix) when variable was a vector (or matrix) of strings.

* Bug 7872 fixed - Riemann Surface demo returned an error if window was closed by
                   the cross button.

* Bug 7897 fixed - configure failed under FreeBSD (bashim)

* Bug 7907 fixed -  ilib_build, ilib_for_link, ... did not check 'files' input
                    argument.

* Bug 7908 fixed - The number of values modified by scicos_getvalues could not
                   by more than 20. This value has been updated to 30
                   variables.

* Bug 7924 fixed - pathconvert failed conversion to cygwin format.

* Bug 7940 fixed - Typo fixed in the parallel_run documentation.

* Bug 7941 fixed - The demonstration CACSD/PID did not work.

* Bug 7943 fixed - On a GNU/Linux binary version, it was not possible to use
                   the fortran_block block.

* Bug 7946 fixed - error(0, "bye") crashed Scilab.

* Bug 7950 fixed - spec crashed on Windows.

* Bug 7951 fixed - strcat crashed with the row concatenation option on
                   GNU/Linux.

* Bug 7952 fixed - libraryinfo did not check size of the input argument.

* Bug 7959 fixed - fscanf did not manage file descriptor as input argument.

* Bug 7964 fixed - errclear(2) cleared all errors and not only error 2 as
                   written in help.

* Bug 7974 fixed - Customized xerbla.f subroutine did not return enough
                   information.

* Bug 7981 fixed - copyfile failed with localized filenames on GNU/Linux.

* Bug 7988 fixed - Inverted pendulum demo failed with f2c version on Windows.

* Bug 7994 fixed - xs2emf created empty files on Windows.

* Bug 8006 fixed - mgetl produced a memory allocation error.

* Bug 8032 fixed - cdfnor was able to fail silently.

* Bug 8055 fixed - lines() was wrongly updated when the console had the focus
                   back.


         Changes between versions 5.3.0-beta-2 and 5.3.0-beta-3 of Scilab
         ===============================================================

Scilab:
=======

Some long-standing usability Bugs have been fixed in this release.
Contributions from the Scilab team have been done on the flexdock project
to fix these issues.

* Bugs 1589, 3994 fixed - On Startup, Scilab console did not have the focus.
                   The user had to click in the main window to gain focus.

* Bug 3614 fixed - Sub-applications could not have their own icons. This
                   problem was causing some issues with the "Alt-Tab" features
                   under some operating systems.

* Bug 4247 fixed - The command xclick didn't work correctly.

* Bug 4423 fixed - It was not possible to navigate in Scilab's menu with arrow
                   keys.

* Bug 5708 fixed - A focus problem between the console and the editor
                   happened when SciNotes was started from the console.

* Bug 7369 fixed - fun2string  produced incorrect code for parenthesis
                   exponentiation expressions.

* Bug 7625 fixed - Some selection issues were appearing from time to time when
                   Scilab was losing the focus.

* 'whos' function updated, variables sorted by name (case insensitive).
  If a variable is global, a * appears after his type name.
  (Thanks to Samuel Gougeon).

Graphics:
========

It's now possible to use LaTeX or MathML as label for uimenu or Pushbutton and
Text in uicontrols.

SciNotes:
=========

Anchors can be added in the source code and one can navigate between them with
the Code Navigator.

* Bugs 7276, 7726 fixed - There was a highlighting problem with default font.

* Bug 7554 fixed - Auto-completion on typing was too general.

* Bug 7618 fixed - The color background change generated an exception.

* Bug 7621 fixed - The font change generated an exception.

* Bug 7623 fixed - "Evaluate Selection" evaluated the selection with one added char.

* Bug 7673 fixed - The extension ".dem" was not available in Open or Save actions.

* Bug 7676 fixed - The SaveAs action did not remove "readonly" mode.

* Bug 7677 fixed - User could not open files in his favorite directories.

* Bug 7678 fixed - Help action did not give help on selection.

* Bug 7682 fixed - The knob of the vertical scrollbar moved badly on resize.

* Bug 7718 fixed - There was a bad detection of a function when the list of args
                   was broken.

* Bug 7719 fixed - It was not possible to open several files at the
                   same time through the file chooser.

* Bug 7719 fixed - It was not possible to open several files at the
                   same time.

* Bug 7722 fixed - Several FindReplace windows could be opened.

* Bug 7731 fixed - In Find/Replace window, buttons were not updated when an
                   item was chosen in the combobox Find.

* Bug 7746 fixed - File streams were not closed. This problem was causing some
                   issues under Windows.

* Bug 7759 fixed - Several Code Navigator instances were possible.

* Bug 7764 fixed - The CR in comments was removed when executing from SciNotes.

* Bug 7774 fixed - The default tabulation was \t.

* Bug 7801 fixed - The FindReplace window could not be cancelled with ESC.

* Bug 7807 fixed - The SaveAs or Save actions emitted an error message when cancelled.

* Bug 7808 fixed - The RestoreSession window could not be cancelled with ESC.

* Bug 7830 fixed - There was a problem when opening a file at a line number.

* Bug 7842 fixed - There was not the filepath in the title bar.

Editvar/Browsevar:
==================

* Bug 7324 fixed - editvar() did not work for string variables.

* Bug 7610 fixed - browsevar(): The icons (left column) did not appear in the
                   binary version (Linux & MacOSX).

* Bug 7617 fixed - editvar() window did not have any menu bar.

* Bug 7627 fixed - When browsevar() window was docked, if browsevar() was called
                   again, an empty window poped up and could not be closed.

* Bug 7629 fixed - browsevar(): Trying to edit a variable that no more exists
                   generated an error.

Compilation:
============

* -fwritable-strings C argument removed for all architectures under Linux
  (was causing some build issues under sparc, alpha and powerpc)

* A problem on startup with GNU/Linux sparc has been fixed.

* The Java "user.home" property has been set to SCIHOME.

* Bug 7668 fixed - Scilab 5.3.0-beta-2 did not build with Visual Studio 2008
                   Express.

* Update Lapack to 3.2.2 on Windows (fix Bugs 7754, 7485, 6477).

* Update JLaTeXMath to version 0.9.3

Documentation:
==============

* Bug 7755 fixed - Some sparse help pages moved from elementary_functions to
                   the sparse chapter.

Windows Installer:
==================

* Add "Command Line - Minimal Installation (no gui)" Configuration.
  Interpreter in command line without the graphic, java or Tcl/Tk capabilities.
  Installation requires only 60 Mo on disk.

Graphics:
=========

Two additionnal Graphics demos about plotyy() and plotyyy() added. See Bug #7687
Thanks to Samuel Gougeon.

* Bug 6910 fixed - The font size in LaTeX labels was too big.

* Bug 7137 fixed - In Text uicontrol, fontsize was rounded which was
                   problematic when fontUnits was 'normalized'.

* Bug 7857 fixed - If an event_handler was previously assigned to a graphic
                   window the datatips toogle tool icon had to be clicked twice
                   to make it active.


Xcos:
=====

* A new initialization UI for partial differential equations settings has been added.

* Bug 7256 fixed - Saving a diagram with a non-ASCII filename created a
                   corrupted file on Windows.

* Bug 7498 fixed - SPLIT_f were not always deleted when one of its connection
                   were.

* Bug 7622 fixed - When opening an Xcos diagram done on 5.2.2, an exception
                   could be thrown.

* Bug 7628 fixed - When importing Scicos diagram, type each port as "explicit"
                   when not typed.

* Bug 7631 fixed - On Xcos, using xcosPalAdd modified the user palatte.xml file.

* Bug 7635 fixed - Xcos diagram with nested SuperBlocks did not contains a
                   valid container reference.

* Bug 7636 fixed - The SUPER_f internal diagram was not coherent with the block
                   external view.

* Bug 7644 fixed - The last user palette could not be removed from the palette
                   tree.

* Bug 7651 fixed - scicos_math.h was missing in binary package.

* Bug 7715 fixed - Performances of xcosDiagramToHDF5 degraded after several
                   subsequent calls to this function.

* Bug 7778 fixed - The disabled buttons were badly colorized.

* Bug 7809 fixed - The AFFICH_m block settings did not update the block view.

* Bug 7834 fixed - The last argument of xcosPalAddBlock about the block style
                   could not be changed by the user.

* Bug 7845 fixed - Code generation was broken.

call_scilab:
============

* in 'API' mode, history manager is disabled.

* Bug 7601 fixed - call_scilab C functions did not check if engine was started.

* Bug 7602 fixed - It was not possible to start/close a Scilab engine at any
                   time.

Scilab/LabView Gateway:
=======================

* Bug 7791 fixed - It was not possible to restart anymore. VI already started.

Bug Fixes:
==========

* Bug 4247 fixed - The command xclick did not work correctly.

* Bug 5697 fixed - External hyperlinks were opened in Help Browser rather
                   than in Web Browser.

* Bug 6477 fixed - in some case, schur returned wrong values on Windows.

* Bug 6504 fixed - Due to a Bug in Java, the <br> tag was not copied as a CR
                   in a messagebox.

* Bug 6938 fixed - frexp returned a wrong error message with a complex as
                   input argument.

* Bug 7125 fixed - fscanfMat() did not read correctly %inf, -%inf and
                   %nan values respectively recorded by fprintfMat().

* Bug 7208 fixed - The help for atomsGetLoaded() gave the wrong shape
                   for the loaded matrix.

* Bug 7348 fixed - ATOMS: A module was loaded twice if it was installed both in
                   the "user" and the "allusers" section.

* Bug 7405 fixed - The toolbar was not updated when an undocking occured.

* Bug 7452 fixed - demos nmplot from Optimization and Simulation -> fminsearch
                   -> nmplot* did not work.

* Bug 7453 fixed - newfun() was broken after clearfun().

* Bug 7485 fixed - Division of matrices of complex numbers crashed on Windows.

* Bug 7519 fixed - Help page of getcallbackobject() did not describe input argument.

* Bug 7525 fixed - Scilab failed to assign 2-d matrices into 3-d matrices, on
                   Windows.

* Bug 7599 fixed - Intersci-n and intersci did not create code from a .desc file
                   if it was not ended by an empty line.

* Bug 7603 fixed - Fortran code generated by intersci did not build with a fortran
                   or F2C compiler on Windows.

* Bug 7611 fixed - The cross to close a subwindow and the little arrow to undock
                   were not visible even if the subwindow was docked.

* Bug 7615 fixed - When opening a SuperBlock, a non-blocking exception could be
                   thrown.

* Bug 7616 fixed - fullfile did not work with only one input argument.

* Bug 7619 fixed - get_function_path did not return all available paths.

* Bug 7620 fixed - The Optimization and Simulation/Optimisation/Plot demo did
                   not plot anything.

* Bug 7661 fixed - Add some usage examples for the function 'warning'.

* Bug 7681 fixed - help_from_sci failed to process tabulated .sci files.

* Bug 7685 fixed - Demo Graphics => More surfaces => Sphere did not appear
                   as spherical.

* Bug 7688 fixed - set("color_map", hsvcolormap(128)) produced
                   a EXCEPTION_STACK_OVERFLOW.

* Bug 7693 fixed - With a corrupted palettes.xml file, the error message was not
                   coherent with the recovering action.

* Bug 7703 fixed - 'coding_ga_identity' help contained false "See Also".

* Bug 7704 fixed - 'init_ga_default' help did not inform the default values of
                   the optional parameters.

* Bug 7706 fixed - example in the 'optim_ga' help page did not work.

* Bug 7733 fixed - strstr crashed scilab if the second input argument was longer
                   than the first one.

* Bug 7737 fixed - Part 1 - ATOMS: when installing a module, the archive was
                   automatically downloaded before checking if the archive
                   already existed in the archive directory.

* Bug 7740 fixed - edit() with a Scilab macro redefined failed.

* Bug 7747 fixed - xclick(): When the graphic window was docked, keyboard
                   events were not detected.

* Bug 7751 fixed - justify() did not work correctly if the subject embedded
                   leading or trailing spaces.

* Bug 7754 fixed - umfpack did not work with Scilab 5.3.0-beta-2 on Windows.

* Bug 7756 fixed - sprand did not produce normal numbers.

* Bug 7766 fixed - cdff, cdffnc functions did not display %inf in error messages

* Bug 7775 fixed - ged action 6 (selection and move of a graphic entity) was
                   broken.

* Bug 7802 fixed - the graduate function complained about obsolete maxi and mini
                   functions.

* Bug 7803 fixed - edit edit failed.

* Bug 7821 fixed - In a messagebox, an exception was thrown when clicking on a
                   mal-formed url.

* Bug 7831 fixed - Some reference of the old editvar UI were present in the
                   source directory tree.

* Bug 7840 fixed - big lines were splitted by mgetl.


         Changes between versions 5.3.0-beta-1 and 5.3.0-beta-2 of Scilab
         ===============================================================

Obsolete Functions:
===================

* 'oldbesseli', 'oldbesselj', 'oldbesselk' and 'oldbessely' are obsolete.
Please use 'besseli', 'besselj', 'besselk' and 'bessely' instead.


History Manager:
================

* "// End Session" line removed from the history file.

* Begin session line identified by "// -- " and finished by " -- //"


SciNotes:
=========

* Add the possibility to restore the last session (thanks to Paul Griffiths).

* Add a close-button on the tabs and the possibility to drag&drop them.

* Bug 6496 fixed - Add hyperlinks on macros in SciNotes.

* Bugs 7462, 7463 fixed - The editor did not manage files with extensions *.tst,
                   *.start,*.quit.

* Bug 7477 fixed - Bad detection of a function declaration when the line
                   ended with a comment.

* Bug 7491 fixed - Broken lines with a .. were not correctly handled and caused
                   a freeze of SciNotes.

* Bug 7550 fixed - In SciNotes, the "case" keyword was misplaced in some cases.

* Bug 7557 fixed - SciNotes: unselected line was tabified.

* Bug 7558 fixed - SciNotes: global variables were reached before local ones.

* Bug 7564 fixed - In SciNotes, no message was displayed when an unsuccessful
                   search occurred.

* Bug 7586 fixed - ShortCut for 'Save' action was missing in SciNotes.

* Bug 7590 fixed - In SciNotes, there was a bad organization of
                   "Split..." and "Line Numbering..." menus.

* Bug 7595 fixed - In SciNotes, a string was broken if '..' was encountered.


File input/output:
==================

* Rewrite 'mputl' as a built-in function for speed improvements.


Window Management:
==================

* If a subwindow is not docked, it is not possible to close a subwindow with
its own "cross" anymore.

* Bugs 7572, 5519 fixed - The cross in the dockbar was confusing when a window
                          was not docked.

Editvar/Browsevar:
==================

* Bug 7328 fixed - editvar() window title did not contain the name of the
                   edited variable.

* Bug 7329 fixed - If editvar() window was docked, calling editvar another time
                   threw java exceptions and froze editvar().

* Bug 7330 fixed - browsevar() called editvar() even if the latter did not
                   handle the variable type.

* Bug 7332 fixed - browsevar(): When checked booleans were hidden and unchecked
                   booleans were displayed, the "boolean" menu was reversed.

* Bug 7333 fixed - browsevar(): The type of global variables had weird values.

* Bug 7334 fixed - browsevar(): Some columns had wrong names.

* Bug 7337 fixed - browsevar(): Some types could not be filtered
                   (Scilab intrinsic, Matlab sparse and implicit polynomial).

* Bug 7343 fixed - editvar could not edit empty matrices.


Xcos:
=====

* Bug 6968 fixed - Modelica Chaos Xcos demo was not simulating.

* Bug 7372 fixed - No message was displayed in the Xcos info bar during the
                   first drag and drop of a block (it may take time).

* Bug 7396 fixed - On I/O blocks used in SuperBlocks, empty index threw a
                   decoding exception.

* Bug 7480 fixed - Xcos conditional compilation produced a too long script on
                   Scilab queue.

* Bug 7483 fixed - A wrong id block could lead to unopenable files.

* Bug 7520 fixed - The Xcos context was not translated from parent diagram to
                   child one.

* Bug 7529 fixed - Using the Xcos block help from the palette froze Scilab.

* Bug 7530 fixed - When connecting two links together in a superblock, the split
                   block was not correctly positioned.

* Bug 7532 fixed - Old Scicos diagrams without diagram version numbers could not
                   be opened.

* Bug 7533 fixed - On Xcos, it was possible to drop a block into another one.

* Bug 7534 fixed - On a RegionToSuperBlock action, the exported block parent
                   field were not updated with the new parent diagram.

* Bug 7540 fixed - Xcos did not well serialized ScilabInteger types. This
                   problem produced Bugs on the LOGIC and BITSET blocks.

* Bug 7551 fixed - An Xcos block could not change the type of a port when it was
                   already present.

* Bug 7571 fixed - The mirror state of a block was not saved on a diagram.

* Bug 7574 fixed - The Xcos diagram configuration settings did not work with
                   exponential values.

* Bug 7588 fixed - When Xcos palettes had been installed in Scilab, they were not
                   removed at the end of the session.



Compilation:
============

* Fix "warning: deprecated conversion from string constant to 'char'"
  message when compiling a toolbox using Scierror(...) and sciprint(...)


Bug Fixes:
==========
* Bug 3842 fixed - "native" and "double" evaluation modes added to the sum,
                   cumsum, prod and cumprod functions
                   For more information, see the SEP #46.

* Bug 4370 fixed - Help page for alufunction and figure_properties were not
                   up-to-date and the help page for pixel_drawing_mode was
                   missing.

* Bug 5185 fixed - It was not possible to edit the name of a super block.

* Bug 5366 fixed - Help of modules_manager was not built.

* Bug 6639 fixed - "hypermat"  did not remove the higher dimensions equal to 1.

* Bug 6741 fixed - Some source files were useless.

* Bug 6849 fixed - add_profiling badly handled "else" which made plotprofile
                   function produced an incorrect report.

* Bug 7121 fixed - Help file about saveafterncommands(0) updated.

* Bug 7124 fixed - mputl returned a wrong error message if a file did not exist.

* Bug 7158 fixed - Help chapter on texmacs was empty.

* Bug 7182 fixed - Help file of link about the output argument updated.

* Bug 7183 fixed - Help file of ulink about the input argument updated.

* Bug 7228 fixed - Help file of strcat about the third input argument updated.

* Bug 7282 fixed - eval failed on some functions without output arguments.

* Bug 7286 fixed - On some platforms, eps export of NaN values was Buggy.

* Bug 7371 fixed - Documentation about cell did not precise that the value
                   returned by A.dims could not be used as a subscript.

* Bug 7374 fixed - If Scilab was launched via another Scilab, TMPDIR was badly
                   built.

* Bug 7388 fixed - A java exception was thrown when we clicked on a menu created
                   using uimenu because this menu had no child.

* Bug 7391 fixed - Links added between help files on evstr, eval, execstr.

* Bug 7432 fixed - fscanfMat and fprintfMat accepted wrong format.

* Bug 7437 fixed - If history file was invalid, history browser crashed.

* Bug 7441 fixed - The history browser crashed if the language was changed.

* Bug 7443 fixed - The number of arguments of the "title", "xlabel", "ylabel"
                   and "zlabel" macros were incorrectly checked.

* Bug 7457 fixed - Under GNU/Linux and Mac OS, the datatip mode icon was
                   missing in the graphic windows toolbar.

* Bug 7459 fixed - Character issues fixed on some Japanese systems.

* Bug 7481 fixed - 6th input argument of ilib_for_link did not allow to set
                   library name on Windows.

* Bug 7505 fixed - Last optional parameter of "legend" macros was not read.

* Bug 7507 fixed - There were some issues in the pvm module error messages
                   which made translation difficult.

* Bug 7481 fixed - 6th input argument of ilib_for_link did not allow to set
                   library name on Windows.

* Bug 7508 fixed - There was a wrong error message in gmres function.

* Bug 7514 fixed - Some demo files were broken due to changes in x_mdialog function.

* Bug 7566 fixed - The cacsd module graphic functions (bode, black, nyquist,...)
                   did not preserve the immediate_drawing property value.

* Bug 7568 fixed - In the completion window, an exception was thrown when an
                   item was selected.

* Bug 7594 fixed - Undefined variable error was not properly handled since Bug 6781 fix.

           Changes between versions 5.2.X and 5.3.0-beta1 of Scilab
           =======================================================


Performance:
============

* Faster access to primitive table (internal). This change improves the overall
  speed of Scilab.

* Improved access to the list of functions increases the performances.


Parallel execution:
===================

* Function "parallel_run" has been introduced for all supported architectures.
  This function allows parallel computations and leverages multicore
  architectures and their capacities.
  For more information, see the help page of parallel_run or SEP #43.


SciNotes (Text Editor):
=======================

* The Scilab consortium would like to express their congratulations to Calixte
  Denizet for his impressive work on the recent developments on SciNotes.

* Distributed with a new name: SciNotes

* Complete refactorization of the text editor. This change improves the speed,
  the stability of the editor and also improves many aspects of SciNotes.

* Add the possibility to complete on-the-fly typing.
  Ex: type '[' gives '[]') or keywords (e.g. type 'if ' gives 'if <CR>end')

* Add the possibility to have several instances of the editor and to dock
  them (or not).

* Add the possibility to remove trailing whitespaces.

* Improve the "help on keyword" from the popup menu.

* Bugs 4399, 4922 fixed - The line numbering could not be compatible with the
                          output of the whereami function.

* Bug 4790 fixed - A broken string (with .. operator) was badly colorized.

* Bug 4865 fixed - The buttons to undo and redo were not disabled when there
                   was nothing to undo or redo.

* Bug 4872 fixed - A binary file was opened like a text file.

* Bug 4907 fixed - The source code of a given macro could not be opened.

* Bug 4916 fixed - Problem with 'See the code' in the demos.

* Bug 4990 fixed - When the argument was a directory, an error was displayed.

* Bug 4992 fixed - There was no possibility to split a window with the same
                   contents.

* Bug 5087 fixed - Different keywords were not split in different categories.

* Bug 5102 fixed - It was not possible to choose the font weight or the font
                   style for a keyword category.

* Bug 5113 fixed - Bad refresh of the application title in some cases.

* Bug 5124 fixed - Editor's title was not updated when a file was saved as...

* Bugs 5126, 6519 fixed - Problems appeared after using undo a few times.

* Bug 5175 fixed - Ghost text appeared in the view.

* Bug 5178 fixed - The kind of tabulations (hard tabulations or spaces)
                   could not be specified.

* Bug 5204 fixed - Weird display of 'else' keyword.

* Bug 5261 fixed - Several instances of the editor could not be opened.

* Bug 5269 fixed - Help could not be reached on a keyword with CTRL+F1.

* Bug 5320 fixed - A correct name was not proposed on the 'Save as' action.

* Bug 5321 fixed - There was no possibility to choose the length of a
                   tabulation.

* Bug 5473 fixed - It was not possible to generate a function help template
                   using help_from_sci.

* Bug 5534 fixed - Auto-completion was not available.

* Bug 5632 fixed - A file in read-only mode could not be opened.

* Bug 5644 fixed - The DragN'Drop text was copied rather than moved.

* Bug 5645 fixed - Undo action deleted a line rather than making it visible.

* Bugs 5646, 5658 fixed - The undo/redo strategy was failing in some cases.

* Bug 5647 fixed - Bad line numbering when a file was just opened.

* Bug 5648 fixed - The editor let you think that you can change a filename just
                   in changing the case.

* Bug 5652 fixed - Bad line numbering when several files were opened.

* Bug 5654 fixed - The editor did not save the user preferences.

* Bug 5655 fixed - The selection was lost after an unsuccessful search.

* Bug 5656 fixed - When a part of text was selected, the search was done on the full
                   document.

* Bug 5657 fixed - Control+End did not move the cursor to the end of the file.

* Bug 5681 fixed - The search window was not put in front of the editor's one.

* Bug 5682 fixed - A problem occurred in some cases with the replace action.

* Bug 6479 fixed - A problem occurred in some cases when a '{' was used in
                   comments.

* Bug 6515 fixed - The DragN'Drop text was copied at the end of the file.

* Bug 6658 fixed - The position and size of the editor were not saved.

* Bug 6670 fixed - There was no shortcut for the "Evaluate Selection" action.

* Bug 6748 fixed - There was a time problem between local computer and
                   remote file system.

* Bug 6807 fixed - '.*' was not correctly highlighted.

* Bug 6980 fixed - When a replaced text contained a '\', the backslash was
                   considered as a special character.

* Bug 7032 fixed - There was a problem when a filename contained a ' or a ".

* Bug 7088 fixed - The editor freezed when a user copied/pasted several times.

* Bug 7112 fixed - Two transpositions gave a string (if a'==1 then a=b').

* Bug 7150 fixed - It was impossible to change the background color.

* Bug 7237 fixed - Whites and tabs were not visible in strings and in comments.

* Bug 7238 fixed - The dot/del key on the numeric pad added a 'suppr' and a dot.

* Bug 7239 fixed - Font style was lost on a selected text.

* Bug 7240 fixed - The menu "Open source file" was active even if nothing could
                   be opened.

* Bug 7262 fixed - An exception was thrown when a text was dragged and dropped.

* Bug 7272 fixed - No check on the lastModified property was performed when
                   a file was opened.

* Bug 7277 fixed - The "recent files" list was badly ordered.

* Bug 7281 fixed - The menu "Evaluate selection" did not change when nothing
                   was selected.

* Bug 7299 fixed - There was a bad matching by the right for the expression
                   n=size(x,"*"); r=(x*x')

* Bug 7352 fixed - No completion menu on the right click.

* Bug 7421 fixed - The source file of a selected name with right
                   click could not be opened.


editvar (variable editor):
==========================

* New variable editor
  Note that this editor will be improved in the next beta releases.


browsevar (variable browser):
=============================

* New variable browser
  Note that this browser will be improved in the next beta releases.


Graphics:
=========

* The set, get and uicontrol functions are now overloadable.

* Datatips tools added.
  For more information, see the help page of datatips or SEP #29.

* The frequency response plots ("bode", "black", "nyquist", "gainplot",...)
  support cutomized datatips.

* Bug 2651 fixed - "plot2d" was not working when the x argument was not given
                   and some optional arguments were given.

* Bug 3100 fixed - The "xselect" function has been made obsolete and replaced
                   by "show_window", in accordance with its help page.

* Bug 3975 fixed - The "xload" function was not able to load the contents of a
                   previously saved graphic figure into a different one.

* Bug 4531 fixed - "legend" macros were not displaying any error when no
                   'Polyline' existed.

* Bug 4658 fixed - When some graphic functions were called without any
                   arguments, the error was not explicit enough.

* Bug 5284 fixed - In some cases, the function "legend" drew line segments in
                   its box in a reverse order.

* Bug 5606 fixed - In certain cases, user-defined x-axis ticks were not
                   correctly aligned.

* Bug 6098 fixed - It was not possible to get values by clicking on a point
                   of Bode diagram.

* Bug 6394 fixed - The "black", "m_circle", "chart" functions has been
                   improved for a better rendering and datatips
                   customization. Two new functions created: "hallchart"
                   (superseed "m_circle") and "nicholschart" (superseed
                   "chart").

* Bug 6532 fixed - In some case text entity rendering order was wrong.

* Bug 6676 fixed - When "twinkle(gca())" was executed, data related to the
                   current axes was not twinkling.

* Bug 6786 fixed - The "polarplot" function did not correctly display the polar
                   coordinated grid when negative rho values were plotted.

* Bug 6835 fixed - Performing an interactive rotation after having executed the
                   "plot3d" function caused Scilab to freeze on 64-bit Windows.

* Bug 6848 fixed - The patterns of the lines were poorly exported.

* Bug 6958 fixed - The "tics_labels" property from "axes" rejected column vector
                   of labels.

* Bug 6959 fixed - axis.tics_labels did not accept $..$ LaTeX labels.

* Bug 7024 fixed - Error messages from "twinkle" were not standard.

* Bug 7028 fixed - When Scilab printed an "axis" graphic handle, string array
                   size for "tics_labels" was erroneous.

* Bug 7065 fixed - "getColorIndex" used some global variables prohibiting
                   user's calls.

* Bug 7067 fixed - The "isoview" macro created an unnecessary "plot" object.

* Bug 7075 fixed - When the "links" property was set for a handle of type
                   "legend", the property type was not checked.

* Bug 7079 fixed - The "mtlb_get" used a "h" global variable instead of the
                   given "H" argument.

* Bug 7172 fixed - Executing the "scaling" function caused a function
                   redefinition warning due to its "factor" input argument.

* Bug 7173 fixed - The equation in the "scaling" help page was incorrect.

* Bug 7187 fixed - Plotting a curve, zooming, and then plotting an additional
                   curve caused Scilab to freeze, depending on the zoomed-in
                   area.

* Bug 7188 fixed - Deleting a Label graphics entity caused Scilab to crash.

* Bug 7353 fixed - Setting a Polyline's "interp_color_mode" property to "on"
                   without having previously set its "interp_color_vector"
                   property caused Scilab to crash.


Localization:
=============

* Polish localization added. Thanks to Paweł Dąbrowski, Ernest Życiński,
  Sebastian Sławski, Kamil Dąbrowsk, Kamil Strojek, Kamil Dąbrowski,
  Michał Snoc, Piotr Seroka...

* Bug 3069 fixed - In some cases, the function gettext was returning \"

* Bug 7059 fixed - The tag X-Launchpad-Export-Date has been removed to avoid
                   huge and useless difference after a launchpad
                   synchronization.

* Bug 7220 fixed - On Windows, when you started Scilab with "WScilex -l ru_RU",
                   some menus were disabled.

Source reorganization:
======================

* 'tbx_builder_src', 'tbx_build_macros', 'tbx_builder_gateway', ... functions
   has been moved in modules_manager module.

API_Scilab:
===========

* Error(x) is now deprecated and will be removed in Scilab 5.4.
  Please consider SciError(x).

Call_Scilab:
============

* Function "sci_types getVariableType(char *varName)" added into call_scilab.h
This function provides an easy way to retrieve the type of a variable.

Documentation:
==============

* It is now possible to use LaTeX inlined in the documentation. Thanks to
  Calixte Denizet for this feature. (See Bug #4329)

* Bug 6630 fixed - JavaHelp did not update existing image files when
                   building help.

* Bug 6852 fixed - JavaHelp did not center the LaTeX formulas containing an
                   attribute align="center"

Xcos:
=====

* Modelica compiler 1.12.1

* New set of icons added (the previous set of icons will be available as an
  external module)

* The default palettes can now be manipulated by the user.

* LaTeX formulas can be used on any TextBlock using the Scilab tag based
  description.

* builnewblock.sci rewritten to use Scilab dynamic link module tools.

* Introduction of the variable %MODELICA_USER_LIBS
  Allows a user to set where are the Modelica files describing the user's
  Modelica blocks.

* Bug 4156 fixed - Xcos defined some temp. variables on stack without
                   removing them.

* Bug 4705 fixed - Xcos used a non-existing file BEpsf

* Bug 4898 fixed - The scifunction_block stored list() parameters on the ipar
                   field instead of the opar field.

* Bug 4977 fixed - Some icons were duplicated between Gui and Xcos.

* Bug 5022 fixed - When opening some demos, ArrayIndexOutOfBoundsException
                   were thrown by some Swing components.

* Bug 5161 fixed - The export to SVG did not work with images or html.

* Bug 5596 fixed - It was not possible to change text block properties (font
                   and color)

* Bug 5640 fixed - Toolbar and menubar were sometimes not visible at startup.

* Bug 6440 fixed - Xcos demos did not work and returned "Unable to
                   open file" error.

* Bug 6527 fixed - There was several instances of the same variable name
                   in the workspace when a data set was exported via a
                   "To Workspace" block.

* Bug 6661 fixed - "Code Generation" menu failed.

* Bug 6668 fixed - Xcos Modelica demos did not work with the new
                   Modelica compiler.

* Bug 6672 fixed - "Code generation" failed if superblock had
                   some gain blocks.

* Bug 6674 fixed - Modelica generic block did not work.

* Bug 6814 fixed - Link point positions (link.xx and link.yy) were not exported
                   as column vectors.

* Bug 6900 fixed - The block interfunction was not considered as Scilab
                   macros.

* Bug 6903 fixed - modelicat called ocamlrun (wrong dependency on Windows).

* Bug 6981 fixed - The Scilab 'exit' call was killing Xcos session without
                   prompting the user if modified diagrams must be saved before
                   exit.

* Bug 7053 fixed - Rotated saved blocks were not loaded with the right rotation
                   value.

* Bug 7089 fixed - Drawing links when creating superblock threw an encoding
                   exception.

* Bug 7099 fixed - xml2modelica called ocamlrun (wrong dependency on Windows).

* Bug 7129 fixed - Xcos hung at the opening of superblocks with context.

* Bug 7143 fixed - Simulation values were not valid when the exponential sign
                   was an 'e'.

* Bug 7175 fixed - Demo bridge rectifier did not build on Windows.

* Bug 7214 fixed - Modelica demos did not generate a .mo file in TMPDIR/modelica
                   directory.


ATOMS:
======

* ATOMS client: Categories are now managed.
  - atomsList(category) limits the list of modules to a category.
  - atomsCategoryList() lists available categories.

* ATOMS GUI:
  - Ergonomics have been improved.
  - Modules can be filtered by category (See request 5276).
  - When a module is not up-to-date, a warning is displayed on the
  corresponding sheet.
  - Release date of a selected module can now be displayed.

* Bug 5276 fixed - ATOMS GUI: It was not possible to filter modules by category.

* Bug 6596 fixed - When the ATOMS GUI started, nothing was displayed. Now, the
                   list of already installed modules appears on the home page.

* Bug 6842 fixed - The ATOMS GUI could tell that an installed version of a module
                   was the most recent one even if it was not the case.

* Bug 6976 fixed - ATOMS GUI: Webpages URLs linked with a module were missing
                   from the presentation frame.

* Bug 6977 fixed - ATOMS GUI: In the presentation frame, URLs and emails were
                   not hyperlinked.

* Bug 7149 fixed - An error message of atomsDESCRIPTIONread() was badly formated.


Matlab to Scilab Conversion Tools:
==================================

* Unitary tests added.

* Matlab-Scilab equivalents dictionary added in documentation.

* In 'mfile2sci', option only_double is now set to %T as default.

* Conversion updated with new Scilab functions:
  - 'filesep' now converted by 'filesep' instead of 'pathconvert'.


Dynamic Link:
=============

* Microsoft Visual Studio 2010 Express and Professional support on
  Windows x86 and x64 platform.

* Microsoft Visual Studio 2008 Express support on Windows x64 platform
  It requires to install Windows SDK to add x64 component on Visual Studio.

* lcc-win32 support moved as module in ATOMS.
  It will be easier to maintain with new versions of lcc-win32.

* Intel Fortran 12 (beta) support on Windows x86 and x64 platform.

* 5th argument of 'ilib_compile', 'ilib_for_link', 'ilib_gen_cleaner',
  'ilib_gen_Make', 'ilib_mex_build', 'ilib_build' is deprecated.
  default value of makefile name must be [].

* Bug 4265 fixed - The execution of the dynamic link functions (ilib_*)
                   continued even if a compiler was not available.

* Bug 5737 fixed - loader.sce generated by ilib_gen_loader did not detect
                   if it was a x86 dll or a x64 dll used with a scilab x64 or x86,
                   on Windows.


Optimization:
=============

* Simulated annealing: Added documentation for accept_func_default and
  accept_func_vfsa.

* fminsearch: updated printing of neldermead, optimbase and optimsimplex objects.

* fminsearch: added demo for dimensionality effect of the Nelder-Mead algorithm.


Incompatibilities:
==================

* The Bug 4583 fix makes the default element value in string array assignment
  be an empty string instead of " ".


FFTW:
=====

* On Windows, Scilab can use FFTW3 wrapper of Intel Math Kernel Library.
  Select "Download FFTW3 wrapper optimized by Intel Math Kernel Library" during
  installation.


API_Scilab:
===========

* Error(x) is now deprecated and will be removed in Scilab 5.4.
  Please use SciError(x) instead.


Source reorganization:
======================

* 'tbx_builder_src', 'tbx_build_macros', 'tbx_builder_gateway', ... functions
   has been moved in modules_manager module.


Compilation:
============

* On Windows, Pentium III class (or equivalent) with SSE instructions is
  required.

* Scilab_f2c.sln can be exported to VS 2010 format.

* Windows 2000 operating system is no more supported.

* Scilab Mac OS X distribution only relies on Java 1.6 and is therefore now
  only available under 64-bit OS (Most of 10.5 and all 10.6 Mac OS X versions)

* Under Mac OS X, uses /usr/libexec/java_home instead of hardcoded paths to
  retrieve JAVA_HOME

* special_functions module is dynamically loaded by Scilab.

* tclsci module is dynamically loaded by Scilab.

* pvm module is dynamically loaded by Scilab.

* Many warnings detected by Intel Fortran 11.1.051 and more on Windows fixed.

* Better detection system of the dependencies version.

* Add a check of the minimal installations of hdf5 (1.8.4), batik (1.7).

* /usr/share/*/lib added to the Java search path for OpenSuse.

* Option --enable-compilation-tests added to the configure. With this option,
  if TestNG has been found, tests will be launched at compilation time.

* Added /usr/lib to JNI search path. Gluegen-rt needed it on openSUSE.

* Bug 4300 fixed - Moving Command Queue management to core.

* Bug 6432 fixed - Checkstyle disabled on SWIG (Java/JNI) generated files.

* Bug 6643 fixed - --without-gui failed to disable Ocaml build (A Xcos
                   dependency).

* Bug 6857 fixed - make CXXFLAGS="-myflag" was removing some C++ arguments.


Help Tools:
==========

* %helps_modules (global variable in Scilab) is only defined when
  Scilab build help and no more at Scilab's startup.


Core:
=====

* Under GNU/Linux, Unix and Mac OS X, for security purposes, temporary
  directory name is now randomized and permissions are limited to the user
  (See Bug #3493).

* Under GNU/Linux, Unix and Mac OS X, if TMPDIR is set, Scilab will respect it
  to store its temporary files (See Bug #3493).

* Under non-Windows system, the name of the temporary directory has been changed
  from SD_* to SCI_TMP_* to match the current Windows behavior.

* Bug 6986 fixed - edit('toto.sce') created a file toto.sce.sci containing
                   a function named toto.sce

Tests:
======

* Mechanism of unitary tests for Java code based on TestNG added and used.

* test_run() now accepts ATOMS modules as input argument.


Text Rendering:
===============

* Antialiasing activated by default to render text


License:
========

* The demos files (modules/*/demos/**/*.dem.*) are now released in the public
  domain. (See Bug #4902)


Obsolete Functions:
===================

* 'MSDOS' obsolete. Please use 'getos' instead (SEP #39).

* 'with_lcc', 'VCtoLCCLib', 'chooselcccompiler', 'configure_lcc',
  'findlcccompiler' have been moved as modules in ATOMS
  See http://atoms.scilab.org/toolboxes/LCC-windows or atomsInstall('lcc-windows')
  from Scilab.

* 'maxi' obsolete see 'max'.

* 'maxi' obsolete. Please use 'max' instead.

* 'mini' obsolete. Please use 'min' instead.

* 'xselect' obsolete. Please use 'show_window' instead.

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


ATOMS:
======

* ATOMS client: Categories are now managed.
  - atomsList(category) limits the list of modules to a category.
  - atomsCategoryList() lists available categories.

* The ATOMS GUI ergonomics has been improved.

* ATOMS GUI: Modules can be filtered by category (See request 5276).

* ATOMS GUI: When a module is not up-to-date, a warning is displayed on the
  corresponding sheet.

* ATOMS GUI: Display the release date of the watched module.

* Bug 5276 fixed - ATOMS GUI: It was not possible to filter modules by category.

* Bug 6596 fixed - When the ATOMS GUI started, nothing was displayed. Now, the
                   list of already installed modules appears on the home page.

* Bug 6842 fixed - If the installed version of a module was not the most recent
                   one, the ATOMS GUI claimed that the newest version was
                   installed.

* Bug 6976 fixed - ATOMS GUI: Webpages URLs linked with a module were missing
                   from the presentation frame.

* Bug 6977 fixed - ATOMS GUI: In the presentation frame, URLs and emails were
                   not hyperlinked.

* Bug 7149 fixed - An error message of atomsDESCRIPTIONread() was badly formated.

Bug fixes:
==========

* Bug 1041 fixed - An error inside a loop was reported as if it occurred on
                   the "end" line.

* Bug 1304 fixed - macr2tree now works with a Scilab expression as input.

* Bug 1371 fixed - The representation of floating numbers with and exponent
                   larger than 100 or smaller than -100 was incorrect.

* Bug 2076 fixed - pspect did not use the last data window.

* Bug 2613 fixed - Incorrect stack recovery for catched errors in some
                   specific contexts (for loops, ...).

* Bug 2623 fixed - Non-regression test script fixed.

* Bug 2654 partially fixed - exec returned a wrong error if a line had more
                   than 4096 characters.

* Bug 2775 fixed - Multiplication of 0 times infinity did not work because of
                   ATLAS under Windows.

* Bug 3281 fixed - Incorrect error message #72 fixed.

* Bug 3311 fixed - Loop with a "for" over a vector of complex polynomials produced
                   wrong values.

* Bug 3490 fixed - "resume" inside an exec crashed Scilab, "resume" restrictions
                    of use removed.

* Bug 3688 fixed - The assignment using ":" subscript for a dimension that did not
                   exist, did not work for hypermatrices.

* Bug 3780 fixed - The loadmatfile function did not ignore comments found in
                   ASCII files while loading them, resulting in an error message.
                   For Matlab compatibility, when this function is called with
                   one input argument load(filename), it now looks for a binary
                   file (if filename has no extension or .mat extension) or for an
                   ASCII file if filename has another extension.

* Bug 3796 fixed - In some situation the "tf2ss()" function failed to
                   compute correctly the state space representation of
                   a transfer function.

* Bug 3810 fixed - Insertion in string array and polynomial array may be very slow.

* Bug 3811 fixed - Documention about "typeof" and overload prefixes was not
                   up-to-date (help overloading).

* Bug 3937 fixed - Buttons size in "x_choices" window was related to the labels one.

* Bug 4075 fixed - A basic uicontrol example crashed Scilab 5.0.3

* Bug 4117 fixed - Via the user interface, closing a graphics figure which had a
                   non-existing event handler attached did not destroy
                   its associated data structure.

* Bug 4224 fixed - uigetfile masks were case insensitive on Windows.

* Bug 4359 fixed - M(:,:)='anything' produced erroneous results.

* Bug 4362 fixed - test_run("gui","checkbox") no more freezes Scilab.

* Bug 4375 fixed - Interactive test SCI/modules/core/tests/unit_tests/breakpoints.tst
                   failed.

* Bug 4493 fixed - 'rat' moved in elementary_functions module.

* Bug 4505 fixed - A red vertical bar appeared for some continuous time systems.

* Bug 4572 fixed - Scilab did a SIGSEGV after stacksize('max') + stacksize().

* Bug 4583 fixed - Default value for string array assignment was " " instead
                   of an empty string "".

* Bug 4625 fixed - intersci-n was broken on Windows.

* Bug 4640 fixed - plot2d() put CPU utilization at 100% on Windows.

* Bug 4672 fixed - Internal function AddFunctionInTable renamed to
                   getFunctionByName to avoid confusions on the actual usage
                   of this function. Thanks to Yuvraj Tomar.

* Bug 4699 fixed - xml2test.pl: Paths were badly displayed in some cases.

* Bug 4783 fixed - Comparison between sparse complex matrix and a full one was
                   wrong when the sparse matrix contained a  zero row.

* Bug 4799 fixed - Sometimes the bode, nyquist , ... function reported a
                   "non convergence" error

* Bug 4841 fixed - Functions created by 'getd' were unknown by Scilab.

* Bug 4948 fixed - A simple Runge Kutta method for solving a second order
                   differential equation had a strange behavior.

* Bug 4857 fixed - Help file of 'gsort' updated with sort of complex numbers
                   and overloading.

* Bug 4985 fixed - mgetl did not use fgets (C function).

* Bug 4996 fixed - mgetl() was not located in the right module. It has been
                   moved to "fileio" module.

* Bug 5110 fixed - Incomplete mgetstr documentation.

* Bug 5123 fixed - Help about '%helps' was wrong.

* Bug 5136 fixed - Information about dependant platform display of
                   exponential form (%e) was missing in the prinft_conversion
                   documentation.

* Bug 5149 fixed - test_run(): Calls to exec() were no more logged by diary().

* Bug 5216 fixed - URLs are now active in uicontrols of style "text". This
                   feature is used in ATOMS GUI (modules description).

* Bug 5324 fixed - Under Windows, "File open" dialog box focused on parent
                   directory of current directory.

* Bug 5432 fixed - A link to http://www.tcl.tk/doc/ added in help files about
                   Tcl/Tk.

* Bug 5512 fixed - The display order of input arguments was not described in
                   the disp() documention.

* Bug 5541 fixed - In the API documentation, the information that data must be
                   allocated before calling API functions was missing.

* Bug 5575 fixed - Wrong German translation fixed (Standart => Standard).

* Bug 5583 fixed - exec() did not manage correctly UTF-8 and ANSI/ASCII files
                   encoded.

* Bug 5588 fixed - Assigning null() in a tlist or mlist field shifted all remaining
                   values with respect to field names.

* Bug 5609 fixed - Documentation stated incorrectly that fieldnames were working
                   on cells.

* Bug 5612 fixed - There was no way to delete a field in a struct.
                   It is now possible with null().

* Bug 5629 fixed - export_to_hdf5 to a file overwrote a directory
                   with the same name (now trigger an error).

* Bug 5630 fixed - export_to_hdf5 to a file with a same directory
                   name altered the permission of the directory.

* Bug 5724 fixed - erfinv and erf missed references to similar functions

* Bug 6332 fixed - stacksize had problems on Mac OS X and 64-bit platforms.

* Bug 6349 fixed - The callback of an 'edit' uicontrol was only executed when it lost focus.

* Bug 6419 fixed - There was no way to retrieve minimum global stack size
                   from Scilab.

* Bug 6428 fixed - fscanfMat did not manage format parameter as fprintfMat.

* Bug 6435 fixed - error 4 returned a wrong message.

* Bug 6447 fixed - unix_g() and dos() did not support properly neither UTF8
                   nor ANSI encoded output.

* Bug 6449 fixed - regexp returned a wrong value with a string returned by mgetl.

* Bug 6451 fixed - length returned a wrong value with a string returned by mgetl.

* Bug 6452 fixed - part returned a wrong value with a string returned by mgetl.

* Bug 6484 fixed - file did not return filename of an opened Fortran file.

* Bug 6504 fixed - Copy/paste of the contents of message boxes was not available.

* Bug 6510 fixed - modules/helptools/help_en_US.xsl and
                   modules/helptools/help_fr_FR.xsl were useless but not removed.

* Bug 6511 fixed - mseek and mtell did not manage files > 2 Gbytes on 64-bit
                   Windows.

* Bug 6516 fixed - fullfile did not remove blanks and not create a valid path.

* Bug 6535 fixed - The property 'ListboxTop' of the graphic user interface object
                   'listbox' had no effect.

* Bug 6543 fixed - neldermead issues for automatic restarts.

* Bug 6549 fixed - genlib function had a behavior which was not consistent with
                   the load function.

* Bug 6550 fixed - lib(".") returned a relative path in library variable
                   created.

* Bug 6556 fixed - fprintf() returned an error if it was called twice (or more)
                   on the same file.

* Bug 6635 fixed - Incorrectly drawn Bode plots (calfrq Bug).

* Bug 6666 fixed - emptystr() gave a wrong result instead of complaining
                   about hypermatrix.

* Bug 6682 fixed - mprintf, msprintf, mfprintf did not manage octal (%o) format.

* Bug 6726 fixed - Variables whose name starts with a $  could be retrieved in
                   libraries.

* Bug 6731 fixed - Information on the output argument was missing from the
                   lib() documentation.

* Bug 6740 fixed - It was not possible to launch Scilab as Minimized
                   or Maximized Window.

* Bug 6744 fixed - p_margin() returned an erroneous result.

* Bug 6745 fixed - g_margin() returned an erroneous result.

* Bug 6750 fixed - TMPDIR constant had no help page.

* Bug 6751 fixed - Completion added unwanted characters when some
                   string sequences occurred several times.

* Bug 6774 fixed - Calling get_param generated a "funcprot" warning when
                   the value of an option stored in a parameters list was
                   a function.

* Bug 6781 fixed - Defined variables in a library were not recognized in
                   compiled macros.

* Bug 6783 fixed - Solving a system of linear equation by backslash ("\")
                   caused a crash, when using complex numbers on Windows.

* Bug 6784 fixed - It was not possible to move a Scilab installation without
                   breaking the (previously installed) ATOMS packages load.

* Bug 6827 fixed - The error printed by spchol if the variable was complex
                   was wrong.

* Bug 6830 fixed - C2F(scistring) in stack2.c was broken for macro names of
                   exactly two characters.

* Bug 6831 fixed - in cli (=nwni) mode, tbx_build_help failed.

* Bug 6861 fixed - yulewalk example failed on Windows.

* Bug 6862 fixed - Help page for 'vectorfind' missed some explanations about
                   input arguments types. Moreover there were missing checks
                   about input arguments number and types in the function.

* Bug 6864 fixed - Some error messages were wrong in data_structures module.

* Bug 6867 fixed - Wrong error message when base2dec() was called with an invalid
                   base representation.

* Bug 6889 fixed - 'fscanfMat' crashed with wrong formatted files.

* Bug 6891 fixed - 'whos' did not display global variables.

* Bug 6893 fixed - Some files +~JF****.tmp were created outside of Scilab
                   temporary directory.

* Bug 6895 fixed - Completion crashed with -NW mode, on some Linux.

* Bug 6901 fixed - This script with 'exec' produced a weird warning.

* Bug 6923 fixed - Some localized messages were wrong in 'qmr'.

* Bug 6924 fixed - Some error messages were wrong in 'gmres' and 'qmr'.

* Bug 6932 fixed - Unitary test for function 'exit' failed in Linux binary
                   version.

* Bug 6933 fixed - Some temporary files (dict_*.tmp) were created when using
                   search engine of the help browser.

* Bug 6934 fixed - Denormalized floating point numbers were not present in
                   Scilab's master.

* Bug 6940 fixed - An error message was wrong in 'pvm_send'.

* Bug 6946 fixed - Information about optional input arguments 'column separator'
                   and 'decimal mark' were missing in the write_csv documentation.

* Bug 6947 fixed - Some demos in "Optimization and Simulation" failed,
                   without write rights in SCI subdirectory.

* Bug 6951 fixed - stacksize had a strange behavior.

* Bug 6952 fixed - stacksize('max') did not return maximum value.

* Bug 6962 fixed - bvode generated an error on Windows.

* Bug 6964 fixed - Scilab could detect a wrong SCI path on Windows.

* Bug 6971 fixed - After launching a module by loader.sce, variable v was
                   predefined.

* Bug 6990 fixed - "and" and "or" did not work on hypermatrices.

* Bug 7003 fixed - 'evstr' did not evaluate NaN and Inf as %nan and %inf.

* Bug 7006 fixed - exec removed spaces at the end of filename when it returned
                   an error.

* Bug 7014 fixed - ilib_verbose value was modified after toolbox_skeleton.

* Bug 7019 fixed - resume() produced an incorrect error message.

* Bug 7021 fixed - Scilab did not start and returned a system error (193)
                   on Windows.

* Bug 7033 fixed - newfun was randomly crashing (mostly on 32-bit) writing to
                   unallocated memory.

* Bug 7036 fixed - The plot2d document was incomplete (missing optional argument).

* Bug 7042 fixed - Insertion of an empty matrix into a struct (to remove a row or column)
                   was Buggy when multiple index was used.

* Bug 7044 fixed - calling sequence in help of 'strcat' was wrong.

* Bug 7057 fixed - isdigit and isalphanum crashed with a matrix of string
                   as input argument.

* Bug 7077 fixed - uigetfont crashed Scilab when the cancel button was clicked.

* Bug 7078 fixed - A wrong error message was displayed when setting the "value"
                   property of an "edit" uicontrol.

* Bug 7094 fixed - If badly called, ode made Scilab crashed.

* Bug 7108 fixed - It was not possible to set the callback_type property via uicontrol.

* Bug 7109 fixed - 'lu' function with some complex matrices crashed on Windows.

* Bug 7123 fixed - No completion was possible if the name of a variable was
                   preceded by ~ or @ or < or >

* Bug 7161 fixed - wavread failed to load some .wav files with error 44.

* Bug 7163 fixed - Assignment index ":" were translated into "eye()".

* Bug 7170 fixed - Scilab equivalent for Matlab "uigetdir" was "tk_getdir",
                   it has been replaced by "uigetdir".

* Bug 7180 fixed - xs2pdf was unable to export a graphic.

* Bug 7181 fixed - The display of a struct having no fields did not work.

* Bug 7189 fixed - In some cases the usage of resume led to a crash of Scilab.

* Bug 7223 fixed - 'unsetmenu' on a separator crashed Scilab.

* Bug 7224 fixed - Export empty matrix in HDF5 did not unlock file.

* Bug 7236 fixed - Due to Bug 1371 fix, sci2exp(-%inf) returned a wrong
                   value -nf and not -%inf.

* Bug 7250 fixed - In the console, the first character of a filename was doubled
                   when completed.

* Bug 7260 fixed - To read a malformed .csv file with fscanfMat crashed Scilab.

* Bug 7285 fixed - Open/Save dialog box did not display filenames without
                   extension with filter "*.*"

* Bug 7291 fixed - unix('') and dos('') returned a wrong messsage on Windows.

* Bug 7278 fixed - strsplit did not work with some particular inputs.

* Bug 7312 fixed - isnum() diplayed warnings when using some unusual parameters.
