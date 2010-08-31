         Changes between version 5.3.0-beta-2 and 5.3.0-beta-3 of Scilab
         ===============================================================

Scilab:
=======

Some long-standing usability bugs have been fixed in this release.
Contributions from the Scilab team have been done on the flexdock project
to fix these issues.

* bugs 1589, 3994 fixed - On Startup, Scilab console did not have the focus.
                   The user had to click in the main window to gain focus.

* bug 3614 fixed - Sub-applications could not have their own icons. This
                   problem was causing some issues with the "Alt-Tab" features
                   under some operating systems.

* bug 4247 fixed - The command xclick didn't work correctly.

* bug 4423 fixed - It was not possible to navigate in Scilab's menu with arrow
                   keys.

* bug 5708 fixed - A focus problem between the console and the editor
                   happened when SciNotes was started from the console.

* bug 7369 fixed - fun2string  produced incorrect code for parenthesis
                   exponentiation expressions.

* bug 7625 fixed - Some selection issues were appearing from time to time when
                   Scilab was losing the focus.

* 'whos' function updated, variables sorted by name (case insensitive).
  If a variable is global, a * appears after his type name.
  (Thanks to Samuel Gougeon).


SciNotes:
=========

Anchors can be added in the source code and one can navigate between them with
the Code Navigator.

* bugs 7276, 7726 fixed - There was a highlighting problem with default font.

* bug 7554 fixed - Auto-completion on typing was too general.

* bug 7618 fixed - The color background change generated an exception.

* bug 7621 fixed - The font change generated an exception.

* bug 7623 fixed - "Evaluate Selection" evaluated the selection with one added char.

* bug 7673 fixed - The extension ".dem" was not available in Open or Save actions.

* bug 7676 fixed - The SaveAs action did not remove "readonly" mode.

* bug 7677 fixed - User could not open files in his favorite directories.

* bug 7678 fixed - Help action did not give help on selection.

* bug 7682 fixed - The knob of the vertical scrollbar moved badly on resize.

* bug 7718 fixed - There was a bad detection of a function when the list of args
                   was broken.

* bug 7719 fixed - It was not possible to open several files at the
                   same time through the file chooser.

* bug 7719 fixed - It was not possible to open several files at the
                   same time.

* bug 7722 fixed - Several FindReplace windows could be opened.

* bug 7731 fixed - In Find/Replace window, buttons were not updated when an
                   item was chosen in the combobox Find.

* bug 7746 fixed - File streams were not closed. This problem was causing some
                   issues under Windows.

* bug 7759 fixed - Several Code Navigator instances were possible.

* bug 7764 fixed - The CR in comments was removed when executing from SciNotes.

* bug 7774 fixed - The default tabulation was \t.

* bug 7801 fixed - The FindReplace window could not be cancelled with ESC.

* bug 7807 fixed - The SaveAs or Save actions emitted an error message when cancelled.

* bug 7808 fixed - The RestoreSession window could not be cancelled with ESC.

* bug 7830 fixed - There was a problem when opening a file at a line number.

* bug 7842 fixed - There was not the filepath in the title bar.

Editvar/Browsevar:
==================

* bug 7324 fixed - editvar() did not work for string variables.

* bug 7610 fixed - browsevar(): The icons (left column) did not appear in the
                   binary version (Linux & MacOSX).

* bug 7617 fixed - editvar() window did not have any menu bar.

* bug 7627 fixed - When browsevar() window was docked, if browsevar() was called
                   again, an empty window poped up and could not be closed.

* bug 7629 fixed - browsevar(): Trying to edit a variable that no more exists
                   generated an error.

Compilation:
============

* -fwritable-strings C argument removed for all architectures under Linux
  (was causing some build issues under sparc, alpha and powerpc)

* A problem on startup with GNU/Linux sparc has been fixed.

* The Java "user.home" property has been set to SCIHOME.

* bug 7668 fixed - Scilab 5.3.0-beta-2 did not build with Visual Studio 2008
                   Express.

* Update Lapack to 3.2.2 on Windows (fix bugs 7754, 7485, 6477).

* Update JLaTeXMath to version 0.9.3

Documentation:
==============

* bug 7755 fixed - Some sparse help pages moved from elementary_functions to
                   the sparse chapter.

Windows Installer:
==================

* Add "Command Line - Minimal Installation (no gui)" Configuration.
  Interpreter in command line without the graphic, java or Tcl/Tk capabilities.
  Installation requires only 60 Mo on disk.

Graphics:
=========

Two additionnal Graphics demos about plotyy() and plotyyy() added. See bug #7687
Thanks to Samuel Gougeon.

* bug 6910 fixed - The font size in LaTeX labels was too big.

* bug 7137 fixed - In Text uicontrol, fontsize was rounded which was problematic when
                   fontUnits was 'normalized'.

* bug 7857 fixed - If an event_handler was previously assigned to a graphic window 
                   the datatips toogle tool icon had to be clicked twice to make it active.


Xcos:
=====

* A new initialization UI for partial differential equations settings has been added.

* bug 7256 fixed - Saving a diagram with a non-ASCII filename created a
                   corrupted file on Windows.

* bug 7498 fixed - SPLIT_f were not always deleted when one of its connection
                   were.

* bug 7622 fixed - When opening an Xcos diagram done on 5.2.2, an exception
                   could be thrown.

* bug 7628 fixed - When importing Scicos diagram, type each port as "explicit"
                   when not typed.

* bug 7631 fixed - On Xcos, using xcosPalAdd modified the user palatte.xml file.

* bug 7635 fixed - Xcos diagram with nested SuperBlocks did not contains a
                   valid container reference.

* bug 7636 fixed - The SUPER_f internal diagram was not coherent with the block
                   external view.

* bug 7644 fixed - The last user palette could not be removed from the palette
                   tree.

* bug 7651 fixed - scicos_math.h was missing in binary package.

* bug 7715 fixed - Performances of xcosDiagramToHDF5 degraded after several
                   subsequent calls to this function.

* bug 7778 fixed - The disabled buttons were badly colorized.

* bug 7809 fixed - The AFFICH_m block settings did not update the block view.

* bug 7834 fixed - The last argument of xcosPalAddBlock about the block style
                   could not be changed by the user.

* bug 7845 fixed - Code generation was broken.

call_scilab:
============

* in 'API' mode, history manager is disabled.

* bug 7601 fixed - call_scilab C functions did not check if engine was started.

* bug 7602 fixed - It was not possible to start/close a Scilab engine at any
                   time.

Scilab/LabView Gateway:
=======================

* bug 7791 fixed - It was not possible to restart anymore. VI already started.

Bug Fixes:
==========

* bug 4247 fixed - The command xclick did not work correctly.

* bug 5697 fixed - External hyperlinks were opened in Help Browser rather
                   than in Web Browser.

* bug 6477 fixed - in some case, schur returned wrong values on Windows.

* bug 6504 fixed - Due to a bug in Java, the <br> tag was not copied as a CR
                   in a messagebox.

* bug 6938 fixed - frexp returned a wrong error message with a complex as 
                   input argument.

* bug 7125 fixed - fscanfMat() did not read correctly %inf, -%inf and
                   %nan values respectively recorded by fprintfMat().

* bug 7208 fixed - The help for atomsGetLoaded() gave the wrong shape
                   for the loaded matrix.

* bug 7348 fixed - ATOMS: A module was loaded twice if it was installed both in
                   the "user" and the "allusers" section.

* bug 7405 fixed - The toolbar was not updated when an undocking occured.

* bug 7452 fixed - demos nmplot from Optimization and Simulation -> fminsearch
                   -> nmplot* did not work.

* bug 7453 fixed - newfun() was broken after clearfun().

* bug 7485 fixed - Division of matrices of complex numbers crashed on Windows.

* bug 7519 fixed - Help page of getcallbackobject() did not describe input argument.

* bug 7525 fixed - Scilab failed to assign 2-d matrices into 3-d matrices, on
                   Windows.

* bug 7599 fixed - Intersci-n and intersci did not create code from a .desc file
                   if it was not ended by an empty line.

* bug 7603 fixed - Fortran code generated by intersci did not build with a fortran
                   or F2C compiler on Windows.

* bug 7611 fixed - The cross to close a subwindow and the little arrow to undock
                   were not visible even if the subwindow was docked.

* bug 7615 fixed - When opening a SuperBlock, a non-blocking exception could be
                   thrown.

* bug 7616 fixed - fullfile did not work with only one input argument.

* bug 7619 fixed - get_function_path did not return all available paths.

* bug 7620 fixed - The Optimization and Simulation/Optimisation/Plot demo did
                   not plot anything.

* bug 7661 fixed - Add some usage examples for the function 'warning'.

* bug 7681 fixed - help_from_sci failed to process tabulated .sci files.

* bug 7685 fixed - Demo Graphics => More surfaces => Sphere did not appear
                   as spherical.

* bug 7688 fixed - set("color_map", hsvcolormap(128)) produced
                   a EXCEPTION_STACK_OVERFLOW.

* bug 7693 fixed - With a corrupted palettes.xml file, the error message was not
                   coherent with the recovering action.

* bug 7703 fixed - 'coding_ga_identity' help contained false "See Also".

* bug 7704 fixed - 'init_ga_default' help did not inform the default values of
                   the optional parameters.

* bug 7706 fixed - example in the 'optim_ga' help page did not work.

* bug 7733 fixed - strstr crashed scilab if the second input argument was longer
                   than the first one.

* bug 7737 fixed - Part 1 - ATOMS: when installing a module, the archive was
                   automatically downloaded before checking if the archive
                   already existed in the archive directory.

* bug 7740 fixed - edit() with a Scilab macro redefined failed.

* bug 7747 fixed - xclick(): When the graphic window was docked, keyboard
                   events were not detected.

* bug 7751 fixed - justify() did not work correctly if the subject embedded
                   leading or trailing spaces.

* bug 7754 fixed - umfpack did not work with Scilab 5.3.0-beta-2 on Windows.

* bug 7756 fixed - sprand did not produce normal numbers.

* bug 7766 fixed - cdff, cdffnc functions did not display %inf in error messages

* bug 7775 fixed - ged action 6 (selection and move of a graphic entity) was
                   broken.

* bug 7802 fixed - the graduate function complained about obsolete maxi and mini
                   functions.

* bug 7803 fixed - edit edit failed.

* bug 7821 fixed - In a messagebox, an exception was thrown when clicking on a
                   mal-formed url.

* bug 7831 fixed - Some reference of the old editvar UI were present in the
                   source directory tree.

* bug 7840 fixed - big lines were splitted by mgetl.

         Changes between version 5.3.0-beta-1 and 5.3.0-beta-2 of Scilab
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

* bug 6496 fixed - Add hyperlinks on macros in SciNotes.

* bugs 7462, 7463 fixed - The editor did not manage files with extensions *.tst,
                   *.start,*.quit.

* bug 7477 fixed - Bad detection of a function declaration when the line
                   ended with a comment.

* bug 7491 fixed - Broken lines with a .. were not correctly handled and caused
                   a freeze of SciNotes.

* bug 7550 fixed - In SciNotes, the "case" keyword was misplaced in some cases.

* bug 7557 fixed - SciNotes: unselected line was tabified.

* bug 7558 fixed - SciNotes: global variables were reached before local ones.

* bug 7564 fixed - In SciNotes, no message was displayed when an unsuccessful
                   search occurred.

* bug 7586 fixed - ShortCut for 'Save' action was missing in SciNotes.

* bug 7590 fixed - In SciNotes, there was a bad organization of
                   "Split..." and "Line Numbering..." menus.

* bug 7595 fixed - In SciNotes, a string was broken if '..' was encountered.


File input/output:
==================

* Rewrite 'mputl' as a built-in function for speed improvements.


Window Management:
==================

* If a subwindow is not docked, it is not possible to close a subwindow with
its own "cross" anymore.

* bugs 7572, 5519 fixed - The cross in the dockbar was confusing when a window
                          was not docked.

Editvar/Browsevar:
==================

* bug 7328 fixed - editvar() window title did not contain the name of the
                   edited variable.

* bug 7329 fixed - If editvar() window was docked, calling editvar another time
                   threw java exceptions and froze editvar().

* bug 7330 fixed - browsevar() called editvar() even if the latter did not
                   handle the variable type.

* bug 7332 fixed - browsevar(): When checked booleans were hidden and unchecked
                   booleans were displayed, the "boolean" menu was reversed.

* bug 7333 fixed - browsevar(): The type of global variables had weird values.

* bug 7334 fixed - browsevar(): Some columns had wrong names.

* bug 7337 fixed - browsevar(): Some types could not be filtered
                   (Scilab intrinsic, Matlab sparse and implicit polynomial).

* bug 7343 fixed - editvar could not edit empty matrices.


Xcos:
=====

* bug 6968 fixed - Modelica Chaos Xcos demo was not simulating.

* bug 7372 fixed - No message was displayed in the Xcos info bar during the
                   first drag and drop of a block (it may take time).

* bug 7396 fixed - On I/O blocks used in SuperBlocks, empty index threw a
                   decoding exception.

* bug 7480 fixed - Xcos conditional compilation produced a too long script on
                   Scilab queue.

* bug 7483 fixed - A wrong id block could lead to unopenable files.

* bug 7520 fixed - The Xcos context was not translated from parent diagram to
                   child one.

* bug 7529 fixed - Using the Xcos block help from the palette froze Scilab.

* bug 7530 fixed - When connecting two links together in a superblock, the split
                   block was not correctly positioned.

* bug 7532 fixed - Old Scicos diagrams without diagram version numbers could not
                   be opened.

* bug 7533 fixed - On Xcos, it was possible to drop a block into another one.

* bug 7534 fixed - On a RegionToSuperBlock action, the exported block parent
                   field were not updated with the new parent diagram.

* bug 7540 fixed - Xcos did not well serialized ScilabInteger types. This
                   problem produced bugs on the LOGIC and BITSET blocks.

* bug 7551 fixed - An Xcos block could not change the type of a port when it was
                   already present.

* bug 7571 fixed - The mirror state of a block was not saved on a diagram.

* bug 7574 fixed - The Xcos diagram configuration settings did not work with
                   exponential values.

* bug 7588 fixed - When Xcos palettes had been installed in Scilab, they were not
                   removed at the end of the session.



Compilation:
============

* Fix "warning: deprecated conversion from string constant to 'char'"
  message when compiling a toolbox using Scierror(...) and sciprint(...)


Bug Fixes:
==========

* bug 3842 fixed - "native" and "double" evaluation modes added to the sum,
                   cumsum, prod and cumprod functions
                   For more information, see the SEP #46.

* bug 4370 fixed - Help page for alufunction and figure_properties were not
                   up-to-date and the help page for pixel_drawing_mode was
                   missing.

* bug 5185 fixed - It was not possible to edit the name of a super block.

* bug 5366 fixed - Help of modules_manager was not built.

* bug 6639 fixed - "hypermat"  did not remove the higher dimensions equal to 1.

* bug 6849 fixed - add_profiling badly handled "else" which made plotprofile
                   function produced an incorrect report.

* bug 7121 fixed - Help file about saveafterncommands(0) updated.

* bug 7124 fixed - mputl returned a wrong error message if a file did not exist.

* bug 7158 fixed - Help chapter on texmacs was empty.

* bug 7182 fixed - Help file of link about the output argument updated.

* bug 7183 fixed - Help file of ulink about the input argument updated.

* bug 7228 fixed - Help file of strcat about the third input argument updated.

* bug 7282 fixed - eval failed on some functions without output arguments.

* bug 7286 fixed - On some platforms, eps export of NaN values was buggy.

* bug 7371 fixed - Documentation about cell did not precise that the value
                   returned by A.dims could not be used as a subscript.

* bug 7374 fixed - If Scilab was launched via another Scilab, TMPDIR was badly
                   built.

* bug 7388 fixed - A java exception was thrown when we clicked on a menu created
                   using uimenu because this menu had no child.

* bug 7391 fixed - Links added between help files on evstr, eval, execstr.

* bug 7432 fixed - fscanfMat and fprintfMat accepted wrong format.

* bug 7437 fixed - If history file was invalid, history browser crashed.

* bug 7441 fixed - The history browser crashed if the language was changed.

* bug 7443 fixed - The number of arguments of the "title", "xlabel", "ylabel"
                   and "zlabel" macros were incorrectly checked.

* bug 7457 fixed - Under GNU/Linux and Mac OS, the datatip mode icon was
                   missing in the graphic windows toolbar.

* bug 7459 fixed - Character issues fixed on some Japanese systems.

* bug 7481 fixed - 6th input argument of ilib_for_link did not allow to set
                   library name on Windows.

* bug 7505 fixed - Last optional parameter of "legend" macros was not read.

* bug 7507 fixed - There were some issues in the pvm module error messages
                   which made translation difficult.

* bug 7481 fixed - 6th input argument of ilib_for_link did not allow to set
                   library name on Windows.

* bug 7508 fixed - There was a wrong error message in gmres function.

* bug 7514 fixed - Some demo files were broken due to changes in x_mdialog function.

* bug 7566 fixed - The cacsd module graphic functions (bode, black, nyquist,...)
                   did not preserve the immediate_drawing property value.

* bug 7568 fixed - In the completion window, an exception was thrown when an
                   item was selected.

* bug 7594 fixed - Undefined variable error was not properly handled since bug 6781 fix.

           Changes between version 5.2.X and 5.3.0-beta1 of Scilab
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

* bugs 4399, 4922 fixed - The line numbering could not be compatible with the
                          output of the whereami function.

* bug 4790 fixed - A broken string (with .. operator) was badly colorized.

* bug 4865 fixed - The buttons to undo and redo were not disabled when there
                   was nothing to undo or redo.

* bug 4872 fixed - A binary file was opened like a text file.

* bug 4907 fixed - The source code of a given macro could not be opened.

* bug 4916 fixed - Problem with 'See the code' in the demos.

* bug 4990 fixed - When the argument was a directory, an error was displayed.

* bug 4992 fixed - There was no possibility to split a window with the same
                   contents.

* bug 5087 fixed - Different keywords were not split in different categories.

* bug 5102 fixed - It was not possible to choose the font weight or the font
                   style for a keyword category.

* bug 5113 fixed - Bad refresh of the application title in some cases.

* bug 5124 fixed - Editor's title was not updated when a file was saved as...

* bugs 5126, 6519 fixed - Problems appeared after using undo a few times.

* bug 5175 fixed - Ghost text appeared in the view.

* bug 5178 fixed - The kind of tabulations (hard tabulations or spaces)
                   could not be specified.

* bug 5204 fixed - Weird display of 'else' keyword.

* bug 5261 fixed - Several instances of the editor could not be opened.

* bug 5269 fixed - Help could not be reached on a keyword with CTRL+F1.

* bug 5320 fixed - A correct name was not proposed on the 'Save as' action.

* bug 5321 fixed - There was no possibility to choose the length of a
                   tabulation.

* bug 5473 fixed - It was not possible to generate a function help template
                   using help_from_sci.

* bug 5534 fixed - Auto-completion was not available.

* bug 5632 fixed - A file in read-only mode could not be opened.

* bug 5644 fixed - The DragN'Drop text was copied rather than moved.

* bug 5645 fixed - Undo action deleted a line rather than making it visible.

* bugs 5646, 5658 fixed - The undo/redo strategy was failing in some cases.

* bug 5647 fixed - Bad line numbering when a file was just opened.

* bug 5648 fixed - The editor let you think that you can change a filename just
                   in changing the case.

* bug 5652 fixed - Bad line numbering when several files were opened.

* bug 5654 fixed - The editor did not save the user preferences.

* bug 5655 fixed - The selection was lost after an unsuccessful search.

* bug 5656 fixed - When a part of text was selected, the search was done on the full
                   document.

* bug 5657 fixed - Control+End did not move the cursor to the end of the file.

* bug 5681 fixed - The search window was not put in front of the editor's one.

* bug 5682 fixed - A problem occurred in some cases with the replace action.

* bug 6479 fixed - A problem occurred in some cases when a '{' was used in
                   comments.

* bug 6515 fixed - The DragN'Drop text was copied at the end of the file.

* bug 6658 fixed - The position and size of the editor were not saved.

* bug 6670 fixed - There was no shortcut for the "Evaluate Selection" action.

* bug 6748 fixed - There was a time problem between local computer and
                   remote file system.

* bug 6807 fixed - '.*' was not correctly highlighted.

* bug 6980 fixed - When a replaced text contained a '\', the backslash was
                   considered as a special character.

* bug 7032 fixed - There was a problem when a filename contained a ' or a ".

* bug 7088 fixed - The editor freezed when a user copied/pasted several times.

* bug 7112 fixed - Two transpositions gave a string (if a'==1 then a=b').

* bug 7150 fixed - It was impossible to change the background color.

* bug 7237 fixed - Whites and tabs were not visible in strings and in comments.

* bug 7238 fixed - The dot/del key on the numeric pad added a 'suppr' and a dot.

* bug 7239 fixed - Font style was lost on a selected text.

* bug 7240 fixed - The menu "Open source file" was active even if nothing could
                   be opened.

* bug 7262 fixed - An exception was thrown when a text was dragged and dropped.

* bug 7272 fixed - No check on the lastModified property was performed when
                   a file was opened.

* bug 7277 fixed - The "recent files" list was badly ordered.

* bug 7281 fixed - The menu "Evaluate selection" did not change when nothing
                   was selected.

* bug 7299 fixed - There was a bad matching by the right for the expression
                   n=size(x,"*"); r=(x*x')

* bug 7352 fixed - No completion menu on the right click.

* bug 7421 fixed - The source file of a selected name with right
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

* bug 2651 fixed - "plot2d" was not working when the x argument was not given
                   and some optional arguments were given.

* bug 3100 fixed - The "xselect" function has been made obsolete and replaced
                   by "show_window", in accordance with its help page.

* bug 3975 fixed - The "xload" function was not able to load the contents of a
                   previously saved graphic figure into a different one.

* bug 4531 fixed - "legend" macros were not displaying any error when no
                   'Polyline' existed.

* bug 4658 fixed - When some graphic functions were called without any
                   arguments, the error was not explicit enough.

* bug 5284 fixed - In some cases, the function "legend" drew line segments in
                   its box in a reverse order.

* bug 5606 fixed - In certain cases, user-defined x-axis ticks were not
                   correctly aligned.

* bug 6098 fixed - It was not possible to get values by clicking on a point
                   of Bode diagram.

* bug 6394 fixed - The "black", "m_circle", "chart" functions has been
                   improved for a better rendering and datatips
                   customization. Two new functions created: "hallchart"
                   (superseed "m_circle") and "nicholschart" (superseed
                   "chart").

* bug 6532 fixed - In some cases, the text entity rendering order was wrong.

* bug 6676 fixed - When "twinkle(gca())" was executed, data related to the
                   current axes was not twinkling.

* bug 6786 fixed - The "polarplot" function did not correctly display the polar
                   coordinated grid when negative rho values were plotted.

* bug 6835 fixed - Performing an interactive rotation after having executed the
                   "plot3d" function caused Scilab to freeze on 64-bit Windows.

* bug 6848 fixed - The patterns of the lines were poorly exported.

* bug 6958 fixed - The "tics_labels" property from "axes" rejected column vector
                   of labels.

* bug 6959 fixed - axis.tics_labels did not accept $..$ LaTeX labels.

* bug 7024 fixed - Error messages from "twinkle" were not standard.

* bug 7028 fixed - When Scilab printed an "axis" graphic handle, string array
                   size for "tics_labels" was erroneous.

* bug 7065 fixed - "getColorIndex" used some global variables prohibiting
                   user's calls.

* bug 7067 fixed - The "isoview" macro created an unnecessary "plot" object.

* bug 7075 fixed - When the "links" property was set for a handle of type
                   "legend", the property type was not checked.

* bug 7079 fixed - The "mtlb_get" used a "h" global variable instead of the
                   given "H" argument.

* bug 7172 fixed - Executing the "scaling" function caused a function
                   redefinition warning due to its "factor" input argument.

* bug 7173 fixed - The equation in the "scaling" help page was incorrect.

* bug 7187 fixed - Plotting a curve, zooming, and then plotting an additional
                   curve caused Scilab to freeze, depending on the zoomed-in
                   area.

* bug 7188 fixed - Deleting a Label graphics entity caused Scilab to crash.

* bug 7353 fixed - Setting a Polyline's "interp_color_mode" property to "on"
                   without having previously set its "interp_color_vector"
                   property caused Scilab to crash.


Localization:
=============

* Polish localization added. Thanks to Paweł Dąbrowski, Ernest Życiński,
  Sebastian Sławski, Kamil Dąbrowsk, Kamil Strojek, Kamil Dąbrowski,
  Michał Snoc, Piotr Seroka...

* bug 3069 fixed - In some cases, the function gettext was returning \"

* bug 7059 fixed - The tag X-Launchpad-Export-Date has been removed to avoid
                   huge and useless difference after a launchpad
                   synchronization.

* bug 7220 fixed - On Windows, when you started Scilab with "WScilex -l ru_RU",
                   some menus were disabled.


Documentation:
==============

* It is now possible to use LaTeX inlined in the documentation. Thanks to
  Calixte Denizet for this feature. (See bug #4329)

* bug 6630 fixed - JavaHelp did not update existing image files when
                   building help.

* bug 6852 fixed - JavaHelp did not center the LaTeX formulas containing an
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

* bug 4156 fixed - Xcos defined some temp. variables on stack without
                   removing them.

* bug 4705 fixed - Xcos used a non-existing file BEpsf

* bug 4898 fixed - The scifunction_block stored list() parameters on the ipar
                   field instead of the opar field.

* bug 4977 fixed - Some icons were duplicated between Gui and Xcos.

* bug 5022 fixed - When opening some demos, ArrayIndexOutOfBoundsException
                   were thrown by some Swing components.

* bug 5161 fixed - The export to SVG did not work with images or html.

* bug 5596 fixed - It was not possible to change text block properties (font
                   and color)

* bug 5640 fixed - Toolbar and menubar were sometimes not visible at startup.

* bug 6440 fixed - Xcos demos did not work and returned "Unable to
                   open file" error.

* bug 6527 fixed - There was several instances of the same variable name
                   in the workspace when a data set was exported via a
                   "To Workspace" block.

* bug 6661 fixed - "Code Generation" menu failed.

* bug 6668 fixed - Xcos Modelica demos did not work with the new
                   Modelica compiler.

* bug 6672 fixed - "Code generation" failed if superblock had
                   some gain blocks.

* bug 6674 fixed - Modelica generic block did not work.

* bug 6814 fixed - Link point positions (link.xx and link.yy) were not exported
                   as column vectors.

* bug 6900 fixed - The block interfunction was not considered as Scilab
                   macros.

* bug 6903 fixed - modelicat called ocamlrun (wrong dependency on Windows).

* bug 6981 fixed - The Scilab 'exit' call was killing Xcos session without
                   prompting the user if modified diagrams must be saved before
                   exit.

* bug 7053 fixed - Rotated saved blocks were not loaded with the right rotation
                   value.

* bug 7089 fixed - Drawing links when creating superblock threw an encoding
                   exception.

* bug 7099 fixed - xml2modelica called ocamlrun (wrong dependency on Windows).

* bug 7129 fixed - Xcos hung at the opening of superblocks with context.

* bug 7143 fixed - Simulation values were not valid when the exponential sign
                   was an 'e'.

* bug 7175 fixed - Demo bridge rectifier did not build on Windows.

* bug 7214 fixed - Modelica demos did not generate a .mo file in TMPDIR/modelica
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

* bug 5276 fixed - ATOMS GUI: It was not possible to filter modules by category.

* bug 6596 fixed - When the ATOMS GUI started, nothing was displayed. Now, the
                   list of already installed modules appears on the home page.

* bug 6842 fixed - The ATOMS GUI could tell that an installed version of a module
                   was the most recent one even if it was not the case.

* bug 6976 fixed - ATOMS GUI: Webpages URLs linked with a module were missing
                   from the presentation frame.

* bug 6977 fixed - ATOMS GUI: In the presentation frame, URLs and emails were
                   not hyperlinked.

* bug 7149 fixed - An error message of atomsDESCRIPTIONread() was badly formated.


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

* bug 4265 fixed - The execution of the dynamic link functions (ilib_*)
                   continued even if a compiler was not available.

* bug 5737 fixed - loader.sce generated by ilib_gen_loader did not detect
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

* The bug 4583 fix makes the default element value in string array assignment
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

* bug 4300 fixed - Moving Command Queue management to core.

* bug 6432 fixed - Checkstyle disabled on SWIG (Java/JNI) generated files.

* bug 6643 fixed - --without-gui failed to disable Ocaml build (A Xcos
                   dependency).

* bug 6857 fixed - make CXXFLAGS="-myflag" was removing some C++ arguments.


Help Tools:
==========

* %helps_modules (global variable in Scilab) is only defined when
  Scilab build help and no more at Scilab's startup.


Core:
=====

* Under GNU/Linux, Unix and Mac OS X, for security purposes, temporary
  directory name is now randomized and permissions are limited to the user
  (See bug #3493).

* Under GNU/Linux, Unix and Mac OS X, if TMPDIR is set, Scilab will respect it
  to store its temporary files (See bug #3493).

* Under non-Windows system, the name of the temporary directory has been changed
  from SD_* to SCI_TMP_* to match the current Windows behavior.

* bug 6986 fixed - edit('toto.sce') created a file toto.sce.sci containing
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
  domain. (See bug #4902)


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

* bug 5276 fixed - ATOMS GUI: It was not possible to filter modules by category.

* bug 6596 fixed - When the ATOMS GUI started, nothing was displayed. Now, the
                   list of already installed modules appears on the home page.

* bug 6842 fixed - If the installed version of a module was not the most recent
                   one, the ATOMS GUI claimed that the newest version was
                   installed.

* bug 6976 fixed - ATOMS GUI: Webpages URLs linked with a module were missing
                   from the presentation frame.

* bug 6977 fixed - ATOMS GUI: In the presentation frame, URLs and emails were
                   not hyperlinked.

* bug 7149 fixed - An error message of atomsDESCRIPTIONread() was badly formated.

Bug fixes:
==========

* bug 1041 fixed - An error inside a loop was reported as if it occurred on
                   the "end" line.

* bug 1304 fixed - macr2tree now works with a Scilab expression as input.

* bug 1371 fixed - The representation of floating numbers with and exponent
                   larger than 100 or smaller than -100 was incorrect.

* bug 2076 fixed - pspect did not use the last data window.

* bug 2613 fixed - Incorrect stack recovery for catched errors in some
                   specific contexts (for loops, ...).

* bug 2623 fixed - Non-regression test script fixed.

* bug 2654 partially fixed - exec returned a wrong error if a line had more
                   than 4096 characters.

* bug 2775 fixed - Multiplication of 0 times infinity did not work because of
                   ATLAS under Windows.

* bug 3281 fixed - Incorrect error message #72 fixed.

* bug 3311 fixed - Loop with a "for" over a vector of complex polynomials produced
                   wrong values.

* bug 3490 fixed - "resume" inside an exec crashed Scilab, "resume" restrictions
                    of use removed.

* bug 3688 fixed - The assignment using ":" subscript for a dimension that did not
                   exist, did not work for hypermatrices.

* bug 3780 fixed - The loadmatfile function did not ignore comments found in
                   ASCII files while loading them, resulting in an error message.
                   For Matlab compatibility, when this function is called with
                   one input argument load(filename), it now looks for a binary
                   file (if filename has no extension or .mat extension) or for an
                   ASCII file if filename has another extension.

* bug 3796 fixed - In some situation the "tf2ss()" function failed to
                   compute correctly the state space representation of
                   a transfer function.

* bug 3810 fixed - Insertion in string array and polynomial array may be very slow.

* bug 3811 fixed - Documention about "typeof" and overload prefixes was not
                   up-to-date (help overloading).

* bug 3937 fixed - Buttons size in "x_choices" window was related to the labels one.

* bug 4075 fixed - A basic uicontrol example crashed Scilab 5.0.3

* bug 4117 fixed - Via the user interface, closing a graphics figure which had a
                   non-existing event handler attached did not destroy
                   its associated data structure.

* bug 4224 fixed - uigetfile masks were case insensitive on Windows.

* bug 4359 fixed - M(:,:)='anything' produced erroneous results.

* bug 4362 fixed - test_run("gui","checkbox") no more freezes Scilab.

* bug 4375 fixed - Interactive test SCI/modules/core/tests/unit_tests/breakpoints.tst
                   failed.

* bug 4493 fixed - 'rat' moved in elementary_functions module.

* bug 4505 fixed - A red vertical bar appeared for some continuous time systems.

* bug 4572 fixed - Scilab did a SIGSEGV after stacksize('max') + stacksize().

* bug 4583 fixed - Default value for string array assignment was " " instead
                   of an empty string "".

* bug 4625 fixed - intersci-n was broken on Windows.

* bug 4640 fixed - plot2d() put CPU utilization at 100% on Windows.

* bug 4672 fixed - Internal function AddFunctionInTable renamed to
                   getFunctionByName to avoid confusions on the actual usage
                   of this function. Thanks to Yuvraj Tomar.

* bug 4699 fixed - xml2test.pl: Paths were badly displayed in some cases.

* bug 4783 fixed - Comparison between sparse complex matrix and a full one was
                   wrong when the sparse matrix contained a  zero row.

* bug 4799 fixed - Sometimes the bode, nyquist , ... function reported a
                   "non convergence" error

* bug 4841 fixed - Functions created by 'getd' were unknown by Scilab.

* bug 4948 fixed - A simple Runge Kutta method for solving a second order
                   differential equation had a strange behavior.

* bug 4857 fixed - Help file of 'gsort' updated with sort of complex numbers
                   and overloading.

* bug 4985 fixed - mgetl did not use fgets (C function).

* bug 4996 fixed - mgetl() was not located in the right module. It has been
                   moved to "fileio" module.

* bug 5110 fixed - Incomplete mgetstr documentation.

* bug 5123 fixed - Help about '%helps' was wrong.

* bug 5136 fixed - Information about dependant platform display of
                   exponential form (%e) was missing in the prinft_conversion
                   documentation.

* bug 5149 fixed - test_run(): Calls to exec() were no more logged by diary().

* bug 5216 fixed - URLs are now active in uicontrols of style "text". This
                   feature is used in ATOMS GUI (modules description).

* bug 5324 fixed - Under Windows, "File open" dialog box focused on parent
                   directory of current directory.

* bug 5432 fixed - A link to http://www.tcl.tk/doc/ added in help files about
                   Tcl/Tk.

* bug 5512 fixed - The display order of input arguments was not described in
                   the disp() documention.

* bug 5541 fixed - In the API documentation, the information that data must be
                   allocated before calling API functions was missing.

* bug 5575 fixed - Wrong German translation fixed (Standart => Standard).

* bug 5583 fixed - exec() did not manage correctly UTF-8 and ANSI/ASCII files
                   encoded.

* bug 5588 fixed - Assigning null() in a tlist or mlist field shifted all remaining
                   values with respect to field names.

* bug 5609 fixed - Documentation stated incorrectly that fieldnames were working
                   on cells.

* bug 5612 fixed - There was no way to delete a field in a struct.
                   It is now possible with null().

* bug 5629 fixed - export_to_hdf5 to a file overwrote a directory
                   with the same name (now trigger an error).

* bug 5630 fixed - export_to_hdf5 to a file with a same directory
                   name altered the permission of the directory.

* bug 5724 fixed - erfinv and erf missed references to similar functions

* bug 6332 fixed - stacksize had problems on Mac OS X and 64-bit platforms.

* bug 6349 fixed - The callback of an 'edit' uicontrol was only executed when it lost focus.

* bug 6419 fixed - There was no way to retrieve minimum global stack size
                   from Scilab.

* bug 6428 fixed - fscanfMat did not manage format parameter as fprintfMat.

* bug 6435 fixed - error 4 returned a wrong message.

* bug 6447 fixed - unix_g() and dos() did not support properly neither UTF8
                   nor ANSI encoded output.

* bug 6449 fixed - regexp returned a wrong value with a string returned by mgetl.

* bug 6451 fixed - length returned a wrong value with a string returned by mgetl.

* bug 6452 fixed - part returned a wrong value with a string returned by mgetl.

* bug 6484 fixed - file did not return filename of an opened Fortran file.

* bug 6504 fixed - Copy/paste of the contents of message boxes was not available.

* bug 6510 fixed - modules/helptools/help_en_US.xsl and
                   modules/helptools/help_fr_FR.xsl were useless but not removed.

* bug 6511 fixed - mseek and mtell did not manage files > 2 Gbytes on 64-bit
                   Windows.

* bug 6516 fixed - fullfile did not remove blanks and not create a valid path.

* bug 6535 fixed - The property 'ListboxTop' of the graphic user interface object
                   'listbox' had no effect.

* bug 6543 fixed - neldermead issues for automatic restarts.

* bug 6549 fixed - genlib function had a behavior which was not consistent with
                   the load function.

* bug 6550 fixed - lib(".") returned a relative path in library variable
                   created.

* bug 6556 fixed - fprintf() returned an error if it was called twice (or more)
                   on the same file.

* bug 6635 fixed - Incorrectly drawn Bode plots (calfrq bug).

* bug 6666 fixed - emptystr() gave a wrong result instead of complaining
                   about hypermatrix.

* bug 6682 fixed - mprintf, msprintf, mfprintf did not manage octal (%o) format.

* bug 6726 fixed - Variables whose name starts with a $  could be retrieved in
                   libraries.

* bug 6731 fixed - Information on the output argument was missing from the
                   lib() documentation.

* bug 6740 fixed - It was not possible to launch Scilab as Minimized
                   or Maximized Window.

* bug 6744 fixed - p_margin() returned an erroneous result.

* bug 6745 fixed - g_margin() returned an erroneous result.

* bug 6750 fixed - TMPDIR constant had no help page.

* bug 6751 fixed - Completion added unwanted characters when some
                   string sequences occurred several times.

* bug 6774 fixed - Calling get_param generated a "funcprot" warning when
                   the value of an option stored in a parameters list was
                   a function.

* bug 6781 fixed - Defined variables in a library were not recognized in
                   compiled macros.

* bug 6783 fixed - Solving a system of linear equation by backslash ("\")
                   caused a crash, when using complex numbers on Windows.

* bug 6784 fixed - It was not possible to move a Scilab installation without
                   breaking the (previously installed) ATOMS packages load.

* bug 6827 fixed - The error printed by spchol if the variable was complex
                   was wrong.

* bug 6830 fixed - C2F(scistring) in stack2.c was broken for macro names of
                   exactly two characters.

* bug 6831 fixed - in cli (=nwni) mode, tbx_build_help failed.

* bug 6861 fixed - yulewalk example failed on Windows.

* bug 6862 fixed - Help page for 'vectorfind' missed some explanations about
                   input arguments types. Moreover there were missing checks
                   about input arguments number and types in the function.

* bug 6864 fixed - Some error messages were wrong in data_structures module.

* bug 6867 fixed - Wrong error message when base2dec() was called with an invalid
                   base representation.

* bug 6889 fixed - 'fscanfMat' crashed with wrong formatted files.

* bug 6891 fixed - 'whos' did not display global variables.

* bug 6893 fixed - Some files +~JF****.tmp were created outside of Scilab
                   temporary directory.

* bug 6895 fixed - Completion crashed with -NW mode, on some Linux.

* bug 6901 fixed - This script with 'exec' produced a weird warning.

* bug 6923 fixed - Some localized messages were wrong in 'qmr'.

* bug 6924 fixed - Some error messages were wrong in 'gmres' and 'qmr'.

* bug 6932 fixed - Unitary test for function 'exit' failed in Linux binary
                   version.

* bug 6933 fixed - Some temporary files (dict_*.tmp) were created when using
                   search engine of the help browser.

* bug 6934 fixed - Denormalized floating point numbers were not present in
                   Scilab's master.

* bug 6940 fixed - An error message was wrong in 'pvm_send'.

* bug 6946 fixed - Information about optional input arguments 'column separator'
                   and 'decimal mark' were missing in the write_csv documentation.

* bug 6947 fixed - Some demos in "Optimization and Simulation" failed,
                   without write rights in SCI subdirectory.

* bug 6951 fixed - stacksize had a strange behavior.

* bug 6952 fixed - stacksize('max') did not return maximum value.

* bug 6962 fixed - bvode generated an error on Windows.

* bug 6964 fixed - Scilab could detect a wrong SCI path on Windows.

* bug 6971 fixed - After launching a module by loader.sce, variable v was
                   predefined.

* bug 6990 fixed - "and" and "or" did not work on hypermatrices.

* bug 7003 fixed - 'evstr' did not evaluate NaN and Inf as %nan and %inf.

* bug 7006 fixed - exec removed spaces at the end of filename when it returned
                   an error.

* bug 7014 fixed - ilib_verbose value was modified after toolbox_skeleton.

* bug 7019 fixed - resume() produced an incorrect error message.

* bug 7021 fixed - Scilab did not start and returned a system error (193)
                   on Windows.

* bug 7033 fixed - newfun was randomly crashing (mostly on 32-bit) writing to
                   unallocated memory.

* bug 7036 fixed - The plot2d document was incomplete (missing optional argument).

* bug 7042 fixed - Insertion of an empty matrix into a struct (to remove a row or column)
                   was buggy when multiple index was used.

* bug 7044 fixed - calling sequence in help of 'strcat' was wrong.

* bug 7057 fixed - isdigit and isalphanum crashed with a matrix of string
                   as input argument.

* bug 7077 fixed - uigetfont crashed Scilab when the cancel button was clicked.

* bug 7078 fixed - A wrong error message was displayed when setting the "value"
                   property of an "edit" uicontrol.

* bug 7094 fixed - If badly called, ode made Scilab crashed.

* bug 7108 fixed - It was not possible to set the callback_type property via uicontrol.

* bug 7109 fixed - 'lu' function with some complex matrices crashed on Windows.

* bug 7123 fixed - No completion was possible if the name of a variable was
                   preceded by ~ or @ or < or >

* bug 7161 fixed - wavread failed to load some .wav files with error 44.

* bug 7163 fixed - Assignment index ":" were translated into "eye()".

* bug 7170 fixed - Scilab equivalent for Matlab "uigetdir" was "tk_getdir",
                   it has been replaced by "uigetdir".

* bug 7180 fixed - xs2pdf was unable to export a graphic.

* bug 7181 fixed - The display of a struct having no fields did not work.

* bug 7189 fixed - In some cases the usage of resume led to a crash of Scilab.

* bug 7223 fixed - 'unsetmenu' on a separator crashed Scilab.

* bug 7224 fixed - Export empty matrix in HDF5 did not unlock file.

* bug 7236 fixed - Due to bug 1371 fix, sci2exp(-%inf) returned a wrong
                   value -nf and not -%inf.

* bug 7250 fixed - In the console, the first character of a filename was doubled
                   when completed.

* bug 7260 fixed - To read a malformed .csv file with fscanfMat crashed Scilab.

* bug 7285 fixed - Open/Save dialog box did not display filenames without
                   extension with filter "*.*"

* bug 7291 fixed - unix('') and dos('') returned a wrong messsage on Windows.

* bug 7278 fixed - strsplit did not work with some particular inputs.

* bug 7312 fixed - isnum() diplayed warnings when using some unusual parameters.

