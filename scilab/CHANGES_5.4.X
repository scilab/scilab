            Changes between version 5.3.3 and 5.4.0 of Scilab
            =================================================


Scilab Desktop:
===============

* Thanks to this new feature, Scilab confirms its usuability and its strong
user-oriented approach. Scilab Desktop is the new concept behind Scilab
window management taking advantage of advanced Flexdock features.
Out of the box, Scilab is now able to save all windows layouts including the
help browser, the variable browser, the command history, Scinotes and Xcos.
Through this major improvement, position and size of all the windows will
be preserved from a session to an other.

In this context, a file browser component has been developped and fully
integrated in Scilab Desktop and all other components have been improved
to provide the best user experience for Scilab's users.

Scilab Desktop provides various predefined layouts:
 - Simple      The standard Scilab console
               => Scilab 5.3.3 (and previous versions) defaults.
 - Integrated  The variable browser, the history browser and the file browser
               the console are opened by default and combined around Scilab's
               Console to provide an ergonomic working environment
               => Scilab 5.4.0 defaults


XML features:
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
 - xmlIsValidObject - Test the existence of an XML object

These features have been developped in the context of the OASIS R&D project.


SciNotes:
=========

* Add marks in the vertical scrollbar to see the positions of matching words
  when an incremental search is done (via F3 or in selecting text).

* In the find/grep tools, add the possibility to use ~ as a joker for user home
  dir. and SCI for Scilab data base dir.

* Add an 'Save and Execute all files' action.

* A tab can be closed in using the middle mouse button.

* bug 9334 fixed - Extra lines were added when code was exported into PDF under
                   Windows.

* bug 9358 fixed - A simple quoted string was not always correctly convert
                   into double quoted one.

* bug 9476 fixed - In find/grep tools, use the abbreviation 'sec' instead of
                   'seconds'.

* bug 9480 fixed - Highlighted words after selection were wrong under Windows.

* bug 9484 fixed - An empty file was not removed when an existing file was
                   opened.

* Bug 9488 fixed - There was no popup menu on the tab to copy the file path.

* bug 9505 fixed - A LaTeX string in a comment was not considered as a comment.

* bug 9597 fixed - It was not possible to navigate across the tabs with
                   <CTRL>+(PAGE_UP | PAGE_DOWN).

* bug 9740 fixed - There was no way to have \n in replacement string.

* bug 9854 fixed - There was a miscolorization with strings containing several
                   dots: 'abc..'.

* bug 9897 fixed - With Mac OS, eol was not replaced by \n when code was copyed
                   from Word to SciNotes.

* bug 9878 fixed - Add a shortcut "Correct indentation" in right click menu.

* bug 10157 fixed - Scilab froze when docking SciNotes after a flexdock bug in a
                    previous session.

* bug 10252 fixed - Could not change the font when SciNotes was docked with the
                    console.

* bug 10283 fixed - The values write in w and iw output arguments of ode function
                    are directly got from work table used in lsod* function.


Graphical User Interface:
=========================

* The variable browser now refresh itself automatically (See bug 9446).


Call_Scilab:
============

* bug 10006 fixed - Unrelevant C-comment in the example of ScilabHaveAGraph
                    removed.


Xcos:
=====

* xcos_skeleton will not be loaded if you have not installed Xcos.

* scilab -f file.xcos opens xcos diagram in xcos.

* Error reporting has been improved to print warnings on blocks into Xcos.

* The style of the block can be changed on the interface function.

* BARXY Block added to help 2D Mechanics system visualization.

* bug 7985 fixed - dmmul.c did not use dgemm routine optimized by BLAS.

* bug 8421 fixed - CLKFROM and CLKGOTO were not part of the "Signal Routing"
                   palette.

* bug 8538 fixed - Counter block label was not reflecting parameters

* bug 8742 fixed - On the Modelica initialization GUI, the error reporting did
                   not work.

* bug 8743 fixed - On the Modelica initialization GUI, it was impossible to
                   re-fix the states after a derivatives fix.

* bug 9037 fixed - While linking, the green port blinked on a small mouse move.

* bug 9301 fixed - Exception when one tried to paste a column to replace a
                   column selection.

* bug 9336 fixed - SuperBlock with non connected ports produced an invalid
                   index error.

* bug 9390 fixed - FROMWSB_c block does not reset to the default values on
                   ReInitialization execution.

* bug 9478 fixed - Block structure was not checked while adding them to a new
                   palette.

* Bug 9501 fixed - With a localized TMPDIR, block edition could throw an HDF5
                   exception.

* bug 9506 fixed - some libraries dependencies were not loaded for
                   xcos_skeleton startup.

* bug 9532 fixed - Users defined blocks had no icons when saved.

* bug 9547 fixed - The about box was not localized due to a multi-line string.

* bug 9573 fixed - getdebuginfo did not report multiple video cards on Windows.

* bug 9590 fixed - xcos examples were not embeded in binary version on Windows.

* bug 9732 fixed - code generation failed on compilation on Windows with
                   "unresolved external symbol dset_".

* bug 9750 fixed - Minor error in the help page of scicos_debug fixed.

* bug 9810 fixed - MATINV wrong simulation values errors has been improved.

* bug 9927 fixed - Xcos crashed while executing 'XcosPalAdd' command.

* bug 9947 fixed - VARIABLE_DELAY block could not be dragged to a diagram.

* bug 9972 fixed - Error reporting with a wrong context produce an endless
                   loop.

* bug 9988 fixed - CLKSOMV_f has a wrong interface function after loading from
                   a file.

* bug 10003 fixed - The MAXMIN block did not update its display according to
                    the settings.

* bug 10048 fixed - Superblock based blocks index sub-blocks
                    parameters instead of looking for them.

* bug 10069 fixed - AFFICH_m block did not update its display at
                    re-initialization.

* bug 10096 fixed - Set [1;-1;] on a product block thrown an exception.

* bug 10302 fixed - Latex labels were not rendered.


Localization:
=============

* Czech localization by Zbyněk Schwarz

* bug 10032 fixed - Force the unset of the environnement variable LANGUAGE to
                    tackle some issues.

* bug 10032 fixed - Mac OS X locale detection did not take in account
                    the LANG variable.


File input/output:
==================

* mgetl, exec, fscanfMat functions manage files encoded as UTF-8 with BOM.

* tempname function added (SEP 68).

* fullpath did not manage matrix of string as input argument. (SEP 72)

* bug 9253 fixed - fscanfMat failed to read a file with some lines
                   of text at the end (backward compatibility).

* bug 9570 fixed - fprintfMat did not use the good format argument with nan and
                   inf in a matrix.

* bug 9869 fixed - fscanf did not check number of input arguments.

* bug 9917 fixed - 'mputl' function crashed with a wrong second parameter.

* bug 10055 fixed - findfiles had a wrong default filter under GNU/Linux
                    & Mac OS X

* bug 10300 fixed - fscanfMat did not free some memory.


New functions
=============

* The assert module provides a set of assertion functions.
  The goal of this module is to provide functions to check the
  behavior of some other functions, for example in unit tests.
  We emphasize the use of consistent tools for testing numerical
  issues, with the goal of testing numerical algorithms more easily.
  In particular, we provide a comparison function for two floating
  point numbers, which allows to check that two numbers are
  "numerically almost equal", i.e. that the relative error is small.
  This automatically fixes the bug #4381.

* The damp function in CACSD modules computes the natural pulsation
  and damping factor of linear dynamical systems.


Optimization:
=============

* neldermead: Added a demo - search for largest smallest polygon.

* Improved clarity of examples in the help pages of the
  optimization module.
  Avoid unnecessary calls to "deff".
  Avoid the "all statements in whole line" style.

* bug 7891 fixed - The neldermead component had too many options.
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

* bug 9566 fixed - The tolvarianceflag, tolabsolutevariance,
  tolrelativevariance options of neldermead are now obsolete.
  See the help page of neldermead for details to update your
  code, if required.

* readmps - Created unit test.
            Fixed bug in the management of error messages.
            Fixed bug in the reading of a test file.
            Defined a printing system by overloading for MPS structure.
            Updated the help page.

* genetic_algorithms - performances improved (2 to 4 times faster).

* Bug 7093 fixed - The data structure generated by neldermead_new could not
                   be saved and reloaded.

* Bug 7889 fixed - The tolerance on x for neldermead was too small.
                   Updated -tolxrelative default value from %eps to sqrt(%eps).

* Bug 7892 fixed - The neldermead component did not provide a default
                   output function.
                   The new function neldermead_defaultoutput was created
                   to print a simple one-line output message by iteration.

* Bug 8805 fixed - The 'neldermead' function, algorithm box, wrongly used
                   the output function.

* Bug 9567 fixed - The neldermead Overview had a TODO section.

* Bug 8542 fixed - In neldermead, there were undocumented default options.
                   Same changes applied to the help of optimbase.

* Bug 8543 fixed - In neldermead, the help page presents a wrong order
                   for the options.

* Bug 8545 fixed - In neldermead, the printing system displays too long
                   messages.

* Bug 8546 fixed - The neldermead functions may call f outside the bounds.

* Bug 8547 fixed - The neldermead function called f outside the constraints.

* Bug 8549 fixed - The neldermead function did not help to debug
                   the cost function.

* Bug 8552 fixed - Improved fminsearch to provide Extra Parameters.
                   Help page updated with new section "The cost function".
                   Help page updated with new example "Passing extra parameters".

* Bug 8676 fixed - The pack/unpack functions had no help pages.

* Bug 8777 fixed - The neldermead function failed to restart with bounds.
                   Added examples of restarts with/without bounds in the
                   neldermead help page.

* Bug 9323 fixed - The messages in optimbase_checkbounds were not localized.
                   Same changes applied to optimbase_checkx0.

* Bug 9472 fixed - The 'ind' argument of optim was unconsistently documented.

* Bug 9576 fixed - The neldermead component did not check the bounds
                   of some input parameters.

* Bug 9578 fixed - The optimbase_configure function does not check the
                   values of input arguments.

* Bug 9607 fixed - The optimsimplex_new function poorly managed the
                   variable number of arguments.

* Bug 9612 fixed - The quapro help page has been removed.

* Bug 9698 fixed - The optim/"gc" algorithm does not support imp=-1

* Bug 9822 fixed - The value ind=1 of optim is not supported by
                   all algorithms.


Statistics:
==============

* bug 9357 fixed - stdevf returned zero for row vector arguments.

* bug 9846 fixed - stdevf() gives zero result when a single row matrix is evaluated.

* bug 7387 fixed - The help page of the rand function gave a wrong advice.
                   The help page of the grand function gave a wrong advice.


Linear Algebra:
==============

* bug 9749 fixed - expm reported non convergence problem.


Sparse matrices:
==============

* bug 10126 fixed - Wrong (or failing) insertion in a sparse matrix.

* Created an help page and unit test for ordmmd.


Sparse Matrices:
==============

* Organized help pages into sub-sections.


Documentation:
==============

* A new system to keep track of the translations of the main documentation
  has been developed. See HTTP:// @TODO@
  See bug 5119.

* <ALT>+<LEFT ARROW> / <ALT>+<RIGHT ARROW> allows the browse of the history.
  (see bug 9474)

* The space key behave like the page down key. (see bug 9491)

* Management of the revhistory / revision tag to provide information about
  the history of the function.

* Bug 3431 fixed - some helptools functions were obsoletes and not removed.

* Bug 9105 fixed - 'err' output parameter of 'mopen' function was not documented.

* Bug 9299 fixed - Fix a typo in the cspect, pspect and window help pages
                   (Hanning changed to Hann)

* Bug 9320 fixed - analpf help page give wrong frequency units of omega (Hz instead of rd/s)

* Bug 9417 fixed - In English, api_scilab polynom help pages updated to the word
                   polynomial

* Bug 7134 fixed - help_from_sci produced an invalid .xml file.

* Bug 7133 fixed - help_from_sci function did not return the function template.

* Bug 9790 fixed - windows_tools help were not available on all platforms.

* Bug 9983 fixed - The xclick help page did not clearly specify the axes used for
                   coordinates


Obsolete Functions:
===================

* 'oemtochar' and 'chartooem' functions (windows_tools) are obsolete and will be
   removed on 5.4.1


Removed functions (previously declared as obsolete):
====================================================

* ilib_for_link, ilib_build do not accept files with ".o" or ".obj" extensions
  for source files.

* 'with_atlas' removed. Please use 'getdebuginfo' instead.

* C API function Error(x) has been removed. Please use SciError.

* 'MSDOS' removed. Please use 'getos' instead.


Performance:
============

* Under GNU/Linux and Mac OS X, Scilab startup speed of scilab-cli and
  scilab-adv-cli have been improved by 15 to 20 % (see Compilation for more
  information).

* SSE is now enabled by default under GNU/Linux with 32 bit CPU (it is already
  the case under 64 bit CPU).
  Pentium III class (or equivalent) with SSE instructions is required.

* bug 8608 fixed - dec2hex, hex2dec, dec2oct, oct2dec, code have been simplified
                   and optimized.

* Performances of function pareto_filter (Genetic algorithms) improved.


Compilation:
============

* Under Windows, Scilab uses VS 2010 Pro and Express solution.

* Under GNU/Linux and Mac OS X, engine libraries have been merged into a single
  libraries named libscilab-cli.so. This greatly improves the startup
  performances.
  Note that the --enable-maintainer-mode enables the previous behavior (simply
  the development process).

* Under GNU/Linux and Mac OS X, all modules build as standalone its algorithms
  as a static library (only available in the source tree).
  This will avoid an important dependency tree for unitary testing.

* Under GNU/Linux and Mac OS X, for each module, get ride of the list of
  the explicit dependencies on the other libraries. Use libscilab-cli.so or
  libscilab.so to link against Scilab.

* Enforce a minimal version of JRosetta (1.0.4).

* Enforce a minimal version of GIWS (1.3.0).

* Force the Java class version target to version 6 (or 1.6).

* Add Java 7 in the Debian & Ubuntu search path under GNU/Linux.

* Under GNU/Linux and Mac OS X, if ccache is installed, use it to build Scilab.

* bug 9409 fixed - warning "remark #8291" removed with Intel Fortran 2011 compiler.

* bug 9500 fixed - Explicit the GUI module manifest class-path to make Debian
                   checking tool happy.

* bug 9782 fixed - configure with --without-gui was failing.

* bug 9793 fixed - --without-gui disables also Xcos.

* bug 9883 fixed - Build with --disable-build-help was failing.

* bug 9991 fixed - ./configure --without-xcos failed to build

* bug 10158 fixed - Configure used g77 even if gfortran is available


Dynamic link:
=============

* bug 9432 fixed - link a 64 bit dynamic library on a 32 bit platform returned
                   a wrong error message on Windows.

* bug 9761 fixed - "NDEBUG" was not defined for dynamic link (by default) in
                   release mode on Windows.

* bug 9930 fixed - ilib_build did not display all filenames used.


Variable browser:
=================

* Shows the dimension of the variable by default.

* Automatically resize the variable browser table when docked.

* A tooltip has been added on the type column to display the name of the type.

* A tooltip has been added on the size column to display the size in Bytes
  (like in previous version of Scilab).


History:
========

* Default text in history for a starting session has been changed for:
"Begin Session : Tue Jun 21 14:35:45 2011" to "21/05/2011 14:11:19"


Graphics:
=========

* datatips tool improved on several aspects:
  - It is now possible to manage directional arrows as well as
    square marks markers.
  - The automatic label placement has been improved to avoid avoid the
    overlapping with the curve.
  - It is now possible to have  painted box or box free labels.
  - Markers can now be selected and moved using the arrow keys
    or deleted with the "Del" key.

* bug 2043 fixed - The graphic function copy() did not manage axes. Thanks to
                   Samuel Gougeon for the fix.

* bug 7412 fixed - Better management of datatips activation/deactivation.

* bug 7419 fixed - datatips(): customization is now available through a GUI.

* bug 8239 fixed - Activation of datatips overwrote and destroyed any event_handler
                   formerly defined.

* bug 8965 fixed - copy function did not work for handles.

* bug 9330 fixed - delete() called overloading instead of deleting the current
                   graphic entity.

* bug 9455 fixed - The contourf and contourf 2 demos displayed unwanted dots.

* bug 10121 fixed - polyline.interp_color_vector where not saved/recoverabled
                    by xsave/xload. Thanks to Samuel Gougeon for the fix.

* bug 10298 fixed - Example of datatipSetDisplay generated an error.

ATOMS:
======

* bug 5380 fixed - atomsShow() output was broken (separators were not aligned).

* bug 8648 fixed - We did not delete the tarball when removing a module (SEP 71).

* bug 8664 fixed - Message error updated for non supported OS.

* bug 8763 fixed - ATOMS installation path was shared on Windows 64 with 32 bits.

* bug 9127 fixed - "Home" item renamed to "Installed Modules".

* bug 9679 fixed - atomsSystemUpdate checks that Scilab is not in offLine mode, else writes a message.

* bug 8942, 9805 fixed - atomsSystemUpdate() failed on some Windows.

* bug 9805 fixed - atomsSystemUpdate() failed on some Windows.

* bug 9967 fixed - windows UAC causing trouble with atomsRepositorySetOfl (internal function)

* bug 10025 fixed - wrong url in some examples from help files

* bug 10031 fixed - atomsDisp did not work with Verbose option set to True and "\n" in messages.



Javasci:
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

* bug 9149 fixed - Calling a graphic function without activated the appropriate
                   mode did not triggered an exception

* bug 9467 fixed - unnecessary declarations in MAKE.BAT of javasci examples.

* bug 9544 fixed - Calling graphic with Javasci from a SWING application from
                   EDT was crashing the execution of a java program.



CACSD and Signal processing
==========================
* bug 1635 fixed - iir filter frequency response may be wrong. "pole zero gain"
                   representation added as possible iir function returned values.

* bug 7895 fixed - fft2() was broken on Windows with MKL wrapper.

* bug 7901 fixed - besselh.tst failed on Windows.

* bug 8684 fixed - 'wfir' function produced an error message when used interactively.

* bug 9098 fixed - demo CACSD --> Robust control failed on Windows 64 bit.

* bug 9214 fixed - abaque replaced by nicholschart in the black.sci comments

* bug 9266 fixed - fftw crashed on Windows when using MKL wrapper.

* bug 9647 fixed - On Scilab startup, the variable fft was declared by default
                   while it should not.

* bug 9675 fixed - iir generated bad results.

* bug 9716 fixed - amell.tst execution froze under Windows with reference lapack library.

* bug 9719 fixed - delip.tst execution froze under Windows.

* bug 9725 fixed - syredi.tst execution froze under Windows.

* bug 9321 fixed - missing tests and help info for some CACSD functions

* bug 10008 fixed - Wrong unit in the datatip for bode function.

* bug 10169 fixed - bad clipping option in evans.


Misc:
=====

* The linspace function accepts column vectors as input argument for the two
  first input arguments. Thanks to Guillaume AZEMA for the implementation.

* The logspace function accepts column vectors as input argument for the two
  first input arguments.

* Change the buffering display mode.

* SEP #67: lines(0) by default at startup.

* Options -disable-exception-catching added under GNU/Linux & Mac OS X.
  Disables the Scilab exception catching system.

* texmacs module moved as ATOMS module.
  atomsInstall('texmacs')

* PVM module moved as ATOMS module.
  atomsInstall('PVM')

* maple2scilab module moved as ATOMS module.
  atomsInstall('maple2scilab')

* SEP #69: predef('names') returns names of protected variables by predef.

* printf moved as alias on mprintf function.

* sprintf moved as alias on msprintf function.


Bug fixes:
==========

* bug 1604 fixed - list extraction failed.

* bug 3291 fixed - SCI/modules/tclsci/tcl/BWidget-1.8.0/ moved as prerequirements.

* bug 4240 fixed - Some unexpected blank lines was displayed with the "dir" function.

* bug 4576 fixed - An example of ilib_build with "cflags" argument was missing.

* bug 4833 fixed - msprintf() interpreted "\n" as a carriage return even if "\n"
                   is supplied by the 2nd input argument.

* bug 4847 fixed - In uigetfile, there was no way to select files with a given
                   pattern.

* bug 5491 fixed - Clearly states that haveacompiler() only works under Windows.

* bug 5696 fixed - When SE Linux is enable, disable the error messages for
                   Scilab in the Scilab binary distribution.

* bug 6875 fixed - test_run() could not run at once on the +3000 tests of
                   Scilab.

* bug 7061 fixed - changelog.txt, readme.txt, version.xml removed from modules.

* bug 7249 fixed - 'string' function did not work with a macro as input argument
                   as documented.

* bug 7333 fixed - In BrowseVar, variables could have a weird type number.

* bug 7358 fixed - Help pages of type and typeof have now links to the actual
                   description pages.

* bug 7409 fixed - Some corrupted matrices could be created from api_scilab.

* bug 7700 fixed - Bad proposition in the console history when a char is added

* bug 7745 fixed - add_demo moved in demo_tools module.

* bug 8086 fixed - Some functions displayed a wrong error message when called
                   without input argument.

* Bug 8243 fixed - xgeom demonstration renamed to geom3d.

* bug 8245 fixed - gsort() did not allow overloading with not managed types.

* bug 8420 fixed - 'lines' crashed scilab in some cases.

* bug 8608 fixed - dec2hex, hex2dec, dec2oct and oct2dec code have been simplified
                   and optimized.

* bug 8688 fixed - fillMatrixOfString did not check available memory on stack.

* bug 8753 fixed - help file of 'c_link' did not describe input argument 'num'.

* bug 8768 fixed - GetRhsVar(..., STRING_DATATYPE, ...) did not check size of input argument.

* bug 8874 fixed - Display of Complex numbers with Inf or Nan for real or Imaginary
                   parts was broken.

* bug 8963 fixed - mtlb_mode test failed on Windows.

* bug 8797 fixed - Update of the help pages of getdate('s') to detail that
                   the value returned contains the leap seconds.

* bug 9045 fixed - plot2d did not support overloading.

* bug 9056 fixed - An uicontrol could not be closed with <META>+W.

* bug 9104 fixed - 'warning' used in a diary scilab session added timestamp
                   twice in the diary.

* bug 9129 fixed - Completion on field did not propose graphics handle fields
                   by default.

* bug 9156 fixed - mgetl.c compilation failed on some Windows.

* bug 9164 fixed - The unit tests now use the new assert functions.

* bug 9268 fixed - Scilab was overridding a predefined variable JAVA_HOME.

* bug 9277 fixed - escape character "\" did not work with msprintf.

* bug 9339 fixed - 'tbx_build_help' function did not manage languages correctly.

* bug 9354 fixed - 'savematfile' could not save imbricated structures.

* bug 9414 fixed - 'SCI' and 'SCIHOME' variables were not documented.

* bug 9452 fixed - It was impossible to download Intel MKL throw a proxy.

* bug 9461 fixed - The assert_checkalmostequal function did not manage
                   sparse matrices.
                   The assert_checkalmostequal function did not manage
                   hypermatrices.

* bug 9479 fixed - Typo fixed (intput => input).

* bug 9498 fixed - A long history caused Scilab startup to be very long.

* bug 9510 fixed - export_to_hdf5 did not return %f when trying to export
                   unmanaged type (function, macro, ...)

* bug 9531 fixed - assert_checkequal failed with mlist

* bug 9540 fixed - MKL Pentium 4 target was not downloaded by installer on Windows.

* bug 9542 fixed - During export operation the file could be locked by an other
                   process (Windows bug).

* bug 9543 fixed - Scilab did not use the default DocumentBuilderFactory
                   to parse XML configuration files.

* bug 9555 fixed - Typo in the 'deff' help page.

* bug 9571 fixed - "error(144)" returned a wrong variable name.

* bug 9599 fixed - error message "at line ... of function ... called by" was
                   printed directly on the same line.

* bug 9635 fixed - some temp. variables were not removed when we used test_run.

* bug 9647 fixed - On Scilab startup, the variable fft was declared by default
                   while it should not.

* bug 9659 fixed - "error(246)" returned a bad message for overloading.

* bug 9682 fixed - 3D assignment into a 2D array of integers issued an error.

* bug 9696 fixed - An error in an if expression inside a try make Scilab froze

* bug 9699 fixed - A single $ as text in a text ui led to a segfault.

* bug 9707 fixed - createNamedXXX did not check variable name.

* bug 9708 fixed - createNamedXXX did not check length of variable name.

* bug 9712 fixed - help of StartScilab had a error about Stacksize argument type.

* bug 9716 fixed - amell.tst execution froze under Windows with reference lapack library.

* bug 9717 fixed - dllinfo.tst failed with Windows binary versions.

* bug 9728 fixed - SCI/modules/special_functions/src/fortran/special_functions_f.def
                   was not ignored (generated file)

* bug 9730 fixed - lines() returned an int32 and not a double value.

* bug 9755 fixed - Completion on paths was not case insensitive on Windows.

* bug 9756 fixed - polelm.tst unitary test failed on Windows 64 bit.

* bug 9763 fixed - comet3d did not check input argument presence.

* bug 9765 fixed - remove ref to scicos_pal in sci_f_clear.f

* bug 9766 fixed - Scilab on Mac OS X 10.7 did not start.

* bug 9791 fixed - toolbar visible state change crash Scilab in NW mode.

* bug 9799 fixed - pol2str did not manage matrix of polynomials and input argument
                   type was not checked.

* bug 9804 fixed - Starting Scilab with a external module emptied the
                   demo list menu.

* bug 9833 fixed - percent was badly formatted by Scierror function.

* bug 9836 fixed - repfreq failed due to "singular problem".

* bug 9838 fixed - evstr froze on some specific strings.

* bug 9843 fixed - colormaps with 2 colors could not be created.

* bug 9858 fixed - length() could not be overloaded with mlist.

* bug 9879 fixed - Fix some minor issues in the isdir help pages.

* bug 9880 fixed - Fix in the help page a page referencing itself.

* bug 9881 fixed - Fix a mistake in the help page of call.

* bug 9885 fixed - user initialization scripts (scilab.ini, .scilab) did not work.

* bug 9899 fixed - string(10*log10(1/10)) gave -0, instead of -10

* bug 9900 fixed - There was a memory leak in getAllocatedMatrixOfString,
                   getAllocatedMatrixOfWideString, getAllocatedNamedMatrixOfString,
                   and getAllocatedNamedMatrixOfWideString.

* bug 9920 fixed - winqueryreg did not manage HKEY_CURRENT_CONFIG.

* bug 9929 fixed - Improve integration with Gnome shell and Unity.

* bug 9950 fixed - It was possible to select several texts in the console.

* bug 9957 fixed - msprintf("%d\r", 2) returned '2r'.

* bug 9825 fixed - The assert_computedigits function generously
                   gives too large correct digits.

* bug 9963 fixed - The assert_computedigits function was not vectorized.

* bug 9966 fixed - The default value of comptype in assert_checkalmostequal
                   was wrongly chosen.

* bug 9971 fixed - GLoad macro not used and not documented is removed.

* bug 9979 fixed - The who_user() function printed a wrong message.

* bug 9980 fixed - Frozen console when one tryes to complete an empty string.

* bug 10001 fixed - %inf:%inf crashed Scilab.

* bug 10016 fixed - The damp function unit test failed under windows.

* bug 10019 fixed - 'exec' crashed with error 113 for too long lines.

* bug 10037 fixed - checkalmostequal.tst unitary test failed under Linux x64.

* bug 10073 fixed - Use the new Scilab icon for GNU/Linux installations.
                    Icons renamed from XPM to PNG.

* bug 10105 fixed - Focus was not given to the prompt when outputview had it.

* bug 10119 fixed - assert_checkequal did not support sparse complex doubles

* bug 10179 fixed - Weird behaviour with evstr.

* bug 10282 fixed - repmat did not work for scalar input on dims greater than 2 
                    neither for N-D arrays.

* bug 10289 fixed - xload returns a better error message when a file does
                    not exist.

* bug 10292 fixed - Improved description of the help page of modulo.

* bug 10293 fixed - Improved description of the help page of bvode.




