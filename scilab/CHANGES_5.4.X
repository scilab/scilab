                    Changes between version 5.4.0 and 5.4.1
                    =======================================


Improvements
=============

* A warning has been added for 1./x and 1.\x syntax clarification.

* A new output argument has been added to regexp function to retrieve
  subpatterns matches. See SEP #86.

* New xmlName function added to retrieve the name of a XML element.

* clc(): New icon in the toolbar to clear the console. See bug #12049.

* test_run no longer takes into account skipped tests in the summary.
  See bug #12117.

* Two new commands added:
 - For SciNotes:
     scinotes modules/core/macros/whos.sci
       Opens a file.
       If the file does not exist, SciNotes proposes to create it.
     scinotes -indent modules/core/macros/whos.sci
       Allows to indent automatically a Scilab file.

 - For Xcos:
     xcos modules/xcos/examples/integral.xcos
       Opens the integral Xcos schema.
       If the file does not exist, Xcos proposes to create it.

* Search is now possible in the console. Shortcut F3 or CTRL+F enables it.
  See Bug #11841.

* editvar proposes direct save to the CSV format of the whole variable or the
  selection.

* Completion in Scilab is now case insensitive. See bug #6320.

* variance is now vectorized and allows the syntax variance(x,"*",w).
  See bug #12362.

* Console command history is now stored in SCIHOME/history file under all platforms.

* In Scilab documentation, 'scilab:needs-examples="no"' tag has been added to specify
  that the help page does not need example.



Xcos
=====

* New Flowmeter block in the thermohydraulics palette added to measure a
  thermohydraulics flow.

* Palette icons and some blocks rendering improved.

* xcosPalGenerateAllIcons function added as a Xcos palette utility function.

* New ODE solver added: LSodar. Automatically switches methods to efficiently
  solve both stiff and nonstiff problems. It includes a rootfinding feature.

* Two new fixed-size step ODE solvers added: Dormand-Prince 4(5) and Runge-Kutta 4(5).
  Added to the CVode package, they also benefit from the rootfinding feature.

* Implicit fixed-size step ODE solver added: Implicit Runge-Kutta 4(5).
  Added to the CVode package, it also benefits from the CVode rootfinding feature.

* Block parameter errors (types and sizes) reporting improved.

* Some scopes blink on simulation avoided.



Compilation
============

* Enforce a minimal version of Flexdock (1.2.3).

* Enforce a minimal version of JLaTeXMath (1.0.2).

* Enforce a minimal version of Scirenderer (1.1.0).

* Enforce a minimal version of XMLGraphics (1.4).



Obsolete functions
===================

* fit_dat() is obsolete and will be removed in Scilab 5.5.0.
  Please use datafit() instead. See bug #9306.

* getfont() is obsolete and will be removed in Scilab 5.5.0.
  Please use ged() instead.

* getlinestyle() is obsolete and will be removed in Scilab 5.5.0.
  Please use ged() instead.

* getmark() is obsolete and will be removed in Scilab 5.5.0.
  Please use ged() instead.

* getsymbol() is obsolete and will be removed in Scilab 5.5.0.
  Please use ged() instead.

* winclose() is obsolete and will be removed in Scilab 5.5.0.
  Please use close() instead.

* xsetm() is obsolete and will be removed in Scilab 5.5.0.
  Please use set(), get() and ged() instead.



Removed functions
==================

* chartoeom() and eomtochar() removed.

* config() removed. Please use preferences() instead.

* createpopup() removed. Please use uicontextmenu() instead.

* mtlb_conv removed. Please use conv() instead.

* mtlb_repmat() removed. Please use repmat() instead.

* neldermead_display, nmplot_display, optimbase_display and
  optimsimplex_print removed. Please use disp() instead.

* optimsimplex_tostring removed. Please use string() instead.

* ricc_old() removed. Please use ricc() instead.

* showalluimenushandles() removed.
  Please use set(get(0), "ShowHiddenHandles", "on") instead.

* with_pvm() removed. Please use getversion() instead.

* with_texmacs() removed. TeXmacs is now an ATOMS module.
  More details on: http://atoms.scilab.org/toolboxes/texmacs

* xbasr() removed. Automatically managed by Scilab now.

* xselect() removed. Please use show_window() instead.



Bug fixes
==========

Scilab
-------

* Bug #2547 fixed - Small size marks were not correctly exported.

* Bug #3313 fixed - In the documentation, the title of a link used the
                    XML id instead of the title of the page.

* Bug #4405 fixed - qr() was poorly documented.

* Bug #4504 fixed - sskf function did not work with two outputs (corresponding
                    help page updated).

* Bug #4946 fixed - Bad rendering of a big rotated label.

* Bug #5291 fixed - strtod returned 0 when no real number was detected.

* Bug #5478 fixed - Scilab could not be started with a non readable/writeable
                    SCIHOME.

* Bug #5548 fixed - Error messages in some examples of Simulated Annealing help pages fixed.

* Bug #5576 fixed - interp() help page has been improved.

* Bug #6583 fixed - jmat() was poorly documented.

* Bug #6890 fixed - Strings were exported as shapes rather than Postscript
                    strings in EPS/PS export.

* Bug #6975 fixed - xlabel(), ylabel() and zlabel() functions were not documented.

* Bug #7058 fixed - There was an error in a MathML formula in beta help page.

* Bug #7190 fixed - is_absolute_path was not documented.

* Bug #7410 fixed - %h_p() and sci2exp() failed to display polyline entity associated to datatips.

* Bug #7583 fixed - binomial function moved in the distribution function chapter.

* Bug #7698 fixed - The "Graphics\Animations\Riemann surface" demonstration background
                    was black (instead of white).

* Bug #7769 fixed - get_function_path did not have a unit test.

* Bug #7790 fixed - Line thickness was not taken into account when drawing marks.

* Bug #8213 fixed - geom3d help page had no examples.

* Bug #8395 fixed - dft has been deprecated to fft.

* Bug #8414 fixed - Some genetic algorithms functions had no examples.

* Bug #8484 fixed - syslin help page has been moved to CACSD chapter.

* Bug #8638 fixed - Examples given in gcd help page have been improved.

* Bug #8649 fixed - flag and flagexpand arguments were not documented in basename
                    and dirname help pages.

* Bug #8883 fixed - In some cases, Scilab built from sources was looking for
                    libraries at the wrong place.

* Bug #8899 fixed - splin2d help page was unclear about the size of
                    the arguments.

* Bug #8901 fixed - interp2d help page was unclear about the size of
                    the arguments.

* Bug #8964 fixed - The optim_{ga, moga, nsga, nsga2} functions did not always
                    create initial population.

* Bug #9005 fixed - bitset function did not have any tests.

* Bug #9196 fixed - The threshold level for conditioning in backslash and slash
                    was too small.

* Bug #9305 fixed - In optimization help pages, a new chapter untitled"Least Squares functions"
                    has been created.

* Bug #9584 fixed - grand returned non-empty matrix for negative size argument.

* Bug #9934 fixed - clear([["A","B"]) returned a non standard error message.

* Bug #9935 fixed - clear() did not clear all variables.

* Bug #10278 fixed - clear(1) returned a wrong error message.

* Bug #10415 fixed - Removing a session line in the history caused the removal
                     of the session node in the history browser.

* Bug #10546 fixed - fileinfo help page updated.

* Bug #10606 fixed - unix_g did not return standard output in somes cases.

* Bug #10661 fixed - xmltopdf and xmltopdf were failing in the binary version.

* Bug #10815 fixed - diff help page did not have a "See Also" section.

* Bug #11134 fixed - xgetmouse() prevented callback menu execution.

* Bug #11209 fixed - variance returned wrong results for complex arguments.

* Bug #11362 fixed - axes_properties help page did not explain that
                     mark_style=mark_size=0 drew a single dot.

* Bug #11436 fixed - With Matplot, color outside the colormap was replaced with
                     the last color in colormap.

* Bug #11474 fixed - save_format help page was still refering to the old save
                     format. (SEP #64 content used to update this page).

* Bug #11477 fixed - There were memory leaks with textures.

* Bug #11497 fixed - EDT violations were detected by the
                     ThreadCheckingRepaintManager tool.

* Bug #11501 fixed - $$..$$ is now used for LaTeX in SciNotes comments to avoid
                     confusion with Scilab code.

* Bug #11549 fixed - Preferences window changed its dimension when hitting Default button.

* Bug #11567 fixed - Scilab failed to start when there was an accentuated character
                     in the pathname.

* Bug #11569 fixed - F5 to navigate in the dock was incompatible with SciNotes.
                     So F5 has been replaced by shift+F6 to go to the previous tab.

* Bug #11639 fixed - Uninstalling Scilab removed all Scilab registry information.

* Bug #11662 fixed - lattn help page had no example (resolved with bug #11663).

* Bug #11663 fixed - lattp and lattn help pages were incomplete.

* Bug #11664 fixed - lev help page had no example.

* Bug #11665 fixed - lindquist help page had no example.

* Bug #11668 fixed - rpem help page had no example.

* Bug #11669 fixed - kalm and srkf help pages had no example.

* Bug #11682 fixed - "resizefcn" and "closerequestfcn" properties description
                     was not precise enough in the figure properties help page.

* Bug #11711 fixed - xclick and xgetmouse did not work under Mac OS X.

* Bug #11778 fixed - Bad color_map dimensions did not return an error.

* Bug #11801 fixed - Bug with isoview="on" on subplots fixed.

* Bug #11804 fixed - Zoom messages were not translated.

* Bug #11811 fixed - Typo in SciNotes help page fixed.


* Bug #11814 fixed - Typo in one of the titles of CACSD fixed.

* Bug #11851 fixed - Default EOL under Mac is now LF.

* Bug #11860 fixed - wavwrite failed for 24-bit files due to an error in
                     savewave function.

* Bug #11924 fixed - winclose() was not tagged as obsolete.

* Bug #11931 fixed - In graphics, white space was not used as default thousand
                     separator.

* Bug #11939 fixed - ssprint formatting lacked accuracy.

* Bug #11941 fixed - show_margins() failed in 'nyquist' mode if the system was
                     in state-space form.

* Bug #11944 fixed - In SciNotes, untabify several lines did not work properly.

* Bug #11949 fixed - save() help page did not explain how to append data to a SOD file.

* Bug #11952 fixed - In SciNotes, a Java exception occurred when a text
                     containing a \E was selected.

* Bug #11954 fixed - export_to_hdf5 was not able to export the same variable twice.

* Bug #11958 fixed - axesflag=0 was not taken into account in plot2d3.

* Bug #11962 fixed - event_handler could be called with wrong figure_id.

* Bug #11965 fixed - Export to SVG converted texts in shapes.

* Bug #11969 fixed - Only the first line was executed when a code was piped under
                     Linux.

* Bug #11971 fixed - Canceling "Open File" action when launched from icon in the
                     toolbar opened SciNotes anyway.

* Bug #11976 fixed - csvTextScan third parameter validity was not checked.

* Bug #11980 fixed - csim returned an error message in some cases: "Submatrix
                     incorrectly defined".

* Bug #11981 fixed - smooth French help page had a wrong declaration
                     (colonnes => lignes).

* Bug #11982 fixed - Ticks computations were made when drawing.

* Bug #11986 fixed - Wrong xclick behaviour on keyboard input fixed.

* Bug #11991 fixed - Under Windows, some empty lines were added by csvWrite.

* Bug #11998 fixed - listvar_in_hdf5 did not close the hdf5 file.

* Bug #11999 fixed - Blank lines are now ignored by default by csvRead / read_csv.

* Bug #12003 fixed - Typo in spreadsheet module fixed.

* Bug #12004 fixed - "Configuring scaling factor" part added in karmarkar help page.

* Bug #12019 fixed - In eigs help page, NEV was unclear.

* Bug #12020 fixed - Default value of NCV parameter (eigs function) was not documented.

* Bug #12021 fixed - eigs help page improved.

* Bug #12022 fixed - ftuneq help page had an incorrect example.

* Bug #12028 fixed - The drawlater/drawnow functions did not work as expected.

* Bug #12033 fixed - assert_checkalmostequal was failing with two %inf values.

* Bug #12043 fixed - Typo in the atomsShow help page fixed.

* Bug #12050 fixed - Fix a typo: dimensionnal => dimensional.

* Bug #12059 fixed - Indices returned by gsort were wrong for sparse matrices.

* Bug #12062 fixed - getsystemmetrics() failed for some properties such as 'SM_CXMAXIMIZED'.

* Bug #12071 fixed - Printed information of a XML object was inconsistent with
                     available fields.

* Bug #12076 fixed - csvRead was not able to open CSV files when separators
                     were not spaces.

* Bug #12077 fixed - In Scilab documentation, the attribute "bgcolor" was not handled
                     in tables.

* Bug #12083 fixed - Loading multiple variables from a SOD file could failed.

* Bug #12087 fixed - profile help page did not mention add_profiling.

* Bug #12097 fixed - Scilab wrongly detected DLL platform information.

* Bug #12111 fixed - karmarkar function did not generate appropriate warnings.

* Bug #12115 fixed - phaseplot(frq,db,phi) did not work.

* Bug #12119 fixed - bar and barh had wrong x/y bounds.

* Bug #12120 fixed - eigs function was broken for complex sparse matrices.

* Bug #12122 fixed - lufact help page did not specify that lufact did not handle complex matrix.

* Bug #12133 fixed - parallel_run was not available.

* Bug #12134 fixed - Default stack size value was wrong in stacksize help page.

* Bug #12135 fixed - stacksize('max') failed without displaying error message.

* Bug #12137 fixed - eigs(A,B) returned incorrect results for sparse matrices.

* Bug #12139 fixed - eigs ran out of memory for sparse matrices.

* Bug #12140 fixed - csvRead failed when a comma was used as a decimal mark.

* Bug #12141 fixed - Calls to gettext in rubberbox help page examples have been removed.
                     Items will be translated in each available languages.

* Bug #12144 fixed - csvRead triggered some error messages when ranges were
                     wrong.

* Bug #12151 fixed - Axes did not pass through (0,0) when x_location and
                     y_location were set to 'origin'.

* Bug #12166 fixed - There was a bad label with drawaxis.

* Bug #12168 fixed - matfile_listvar crashed when listing variables of a closed MAT-file.

* Bug #12174 fixed - routh_t function gave incorrect output for all denominators
                     that included gain value "k".

* Bug #12179 fixed - Incompatibility with MPI version of HDF5 fixed.

* Bug #12184 fixed - Performances of derivat function improved.

* Bug #12186 fixed - Performances of horner function improved.

* Bug #12190 fixed - sprspn help page description updated.

* Bug #12196 fixed - There were no log-scales with Fec and Fac3d.

* Bug #12204 fixed - Typo in French localization fixed.

* Bug #12219 fixed - delete("all") did not recreate a basic axe within a cleaned
                     figure.

* Bug #12231 fixed - In SciNotes, a whole word search did not work properly.

* Bug #12232 fixed - There were several memory leaks in various fillers.

* Bug #12234 fixed - Graphics could crash with an invalid property.

* Bug #12235 fixed - Matplot did not update on color_map change.

* Bug #12238 fixed - [d, v] = eigs(A) was broken for sparse matrices.

* Bug #12239 fixed - Recent changes in eigs applied to feigs.

* Bug #12240 fixed - eigs function refactored.

* Bug #12243 fixed - parallel_run did not run all calculations on 8 core
                     processors.

* Bug #12246 fixed - An error message is now returned when reading invalid Excel files
                     (which have declared wrong column/row count).

* Bug #12247 fixed - Typo in some error messages fixed.

* Bug #12248 fixed - SciNotes toolbar was missing.

* Bug #12259 fixed - Wrong translation for "Filled" fixed in SciNotes.

* Bug #12260 fixed - Grid was not drawn with log scale and user-defined ticks.

* Bug #12270 fixed - When the console lost the focus between SciNotes and the console,
                     the caret could become visible in the console instead SciNotes.

* Bug #12272 fixed - Presentation of Cyrillic letters on sample images of Russian
                     help page for grand function fixed.

* Bug #12284 fixed - Completion in console could led to a crash (or deadlock).

* Bug #12285 fixed - export_to_hdf5 function did not manage any matrix in append mode.

* Bug #12289 fixed - sub_ticks property was ignored.

* Bug #12291 fixed - A list of handles could not be displayed as a handle userdata.

* Bug #12296 fixed - Calling getenv with a big environment variable
                     led to memory corruption.

* Bug #12304 fixed - Undocking, redocking and undocking a graphic figure led to an exception.

* Bug #12305 fixed - Legends could not be loaded from SOD files.

* Bug #12307 fixed - ftp addresses were not handled in the help browser.

* Bug #12317 fixed - eventhandler functions no more reported keyboard events.

* Bug #12321 fixed - Saving environment failed to save library variables. Now these variables
                     are ignored like in previous versions of save function.

* Bug #12333 fixed - Command history was restored after reopening Scilab even when cleaned.

* Bug #12335 fixed - strtod did not work with negative numbers.

* Bug #12343 fixed - SciNotes help page was pointing to the editor() function.

* Bug #12346 fixed - lsslist() help pages moved to CACSD chapter.

* Bug #12348 fixed - surf() disabled lines(n) if there was no input variable.

* Bug #12354 fixed - SciNotes wrongly detected an invalid function with a semicolon after args
                     declaration.

* Bug #12360 fixed - Format for purely real complex value was wrong in csvWrite.

* Bug #12361 fixed - Error in replot() help page examples fixed.

* Bug #12370 fixed - Example with marks added in xpoly help page.

* Bug #12373 fixed - pmodulo(n,m<0) returned a negative result.

* Bug #12374 fixed - Unexpected mark was drawn with xsegs.

* Bug #12377 fixed - chepol was located in the wrong help section (now in polynomial).

* Bug #12378 fixed - sdf() crashed Scilab.

* Bug #12383 fixed - Memory leaks fixed in csvRead.

* Bug #12386 fixed - Example given in sgrid help page improved.

* Bug #12389 fixed - Axes sizes were not taken into account in offscreen export.

* Bug #12391 fixed - findobj() could change current figure returned by gcf().

* Bug #12396 fixed - Example "Problem 2" was missing a variable in bvode help page.

* Bug #12399 fixed - Data bounds were badly computed with %inf and logarithmic scale.

* Bug #12405 fixed - Floating point errors could lead to an infinite loop in
                     ticks computation.

* Bug #12406 fixed - In SciNotes, whereami numbering failed when a variable name
                     started with endfunction.

* Bug #12418 fixed - Continuation was incorrectly supported in bvode.

* Bug #12442 fixed - pathconvert() help page was not clear about input arguments.

* Bug #12444 fixed - The fontname argument was buggy in the title function.

* Bug #12448 fixed - Double click on .sod file tried to open it in Scinotes
                     instead of loading it.

* Bug #12450 fixed - xlfont() did not return font names as specified in the documentation.

* Bug #12455 fixed - format_n property had no effect on "axis" handles.

* Bug #12457 fixed - drawlater() was not taken into account with xfpolys.


Xcos
-----

* Bug #5040 fixed - Fit diagram to view did not work with a single block.

* Bug #5162 fixed - "Export all" menu to export all diagram layers added.

* Bug #9074 fixed - Calling a function on the context gave a warning.

* Bug #10040 fixed - VARIABLE_DELAY help page did not fully describe the
                     initial value behavioral.

* Bug #10434 fixed - loadXcosLibs changed the value of predef.

* Bug #10435 fixed - Undo in a superdiagram did not update superblock ports
                     and links.

* Bug #10660 fixed - model2blk and callblk crashed Scilab under 64-bit platforms.

* Bug #11193 fixed - Lincos did not respect context values.

* Bug #11775 fixed - xcos.tst crashed under Linux and MacOS X.

* Bug #11812 fixed - Upper/lower case issues in Xcos documentation fixed.

* Bug #11821 fixed - Running xcosPalAdd example from help led to deadlock.

* Bug #11871 fixed - Connectors were misaligned after an Undo command.

* Bug #11913 fixed - Unconnected CLSS block with empty matrix parameters made
                     Scilab crash.

* Bug #11919 fixed - DLSS block with empty matrix parameters made Scilab crash.

* Bug #11945 fixed - When running simulation on large diagram, the grey filter
                     did not apply to the entire diagram.

* Bug #11963 fixed - Input ports in a superblock were corrupted.

* Bug #11988 fixed - Copying or dragging AFFICH_m block threw Java
                     NullPointerException.

* Bug #12014 fixed - dep_ut help page was wrongly detailed.

* Bug #12031 fixed - Changes in the context of a superblock did not always have
                     effect.

* Bug #12057 fixed - Scopes with big scope buffer size simulation were much
                     more slower than in Scilab 5.3.3.

* Bug #12101 fixed - Port ordering was not maintained when changing parameters.

* Bug #12162 fixed - tbx_build_blocks did not report errors.

* Bug #12167 fixed - Text of ports in superblocks was not updated.

* Bug #12213 fixed - The example given in lincos help page produced an undefined
                     variable warning.

* Bug #12265 fixed - The example diagrams given in xcos_simulate help page had
                     a badly rendering due to CLR blocks.

* Bug #12290 fixed - AFFICH_m block inside a superblock led to a Java
                     NullPointerException.

* Bug #12350 fixed - Under Windows Explorer, opening a xcos/zcos file
                     did not try to find an existing instance of Scilab.

* Bug #12351 fixed - The user could not save a diagram as a xcos file when using
                     the "Save as" menu, zcos format was forced.

* Bug #12352 fixed - Critical error when closing a scope windows during
                     simulation fixed.




                    Changes between version 5.3.3 and 5.4.0
                    =======================================

Scilab Desktop
===============

* Thanks to this new feature, Scilab confirms its usability and its strong
user-oriented approach. Scilab desktop is the new concept behind Scilab
windows management taking advantage of our docking system (advanced FlexDock features).
Out of the box, Scilab is now able to save all windows layouts including the
help browser, the variable browser, the command history, SciNotes and Xcos.
Through this major improvement, position and size of all windows are
saved from a session to another.

* A file browser component has been developed and fully
integrated into Scilab desktop and all other components have been improved
to provide the best user experience for Scilab users.

* Editvar has had important refreshment. It allows plot and subplot, manages
almost all Scilab variable types and is much more faster.

* Scilab desktop provides various predefined layouts:
 - Simple      The standard Scilab console
               => Scilab 5.3.3 (and previous versions) defaults.
 - Integrated  The variable browser, the history browser and the file browser
               are opened by default and combined around Scilab
               console to provide an ergonomic workspace.
               => Scilab 5.4.0 defaults

* Variable browser has been improved:
 - The dimension of the variable is shown by default.
 - The variable browser table is automatically resized when docked.
 - A tooltip has been added on the type column to display the name of the type.
 - A tooltip has been added on the size column to display the size in bytes
  (like in previous versions of Scilab).
 - Refresh is automatical (See Bug #9446).

* A unique Preferences window has been developed to configure Scilab and its modules.
This unique interface can be launched using preferences() function.


Graphics
=========

* Entire refactoring of Scilab graphic capabilities in order to solve the known issues
  of graphical cards compatibilities and many Scilab crashes.

* Based on SciRenderer (rendering library based on JoGL), Scilab graphics have
  been rewritten to provide new graphic functionalities with:
  - better performances,
  - better stability,
  - 2-D zoom using the mouse wheel,
  - new 3-D zoom capabilities,
  - rotation available through the mouse right-click.
  SciRenderer is a free and open source library developed by Scilab Enterprises
  for 2-D and 3-D rendering of scientific data.

* Improved graphic exports:
  - Off-screen export (Scilab will not open a window to export with driver).
  - New bitmap formats supported: PNG, JPEG and BMP in the driver function.
  - New vectorial formats supported: PDF and SVG in the driver function.
  - Many portability issues fixed.

* Obsolete value for line_style property:
  0 and 1 are equivalent values for SOLID rendering.
  A warning message will be displayed if the user tries to set this property to 0.
  Setting this property to 0 will no more be supported in Scilab 5.4.1.

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

* Bug #4289 fixed - Graphic exports were not working when the drawlater or the
                    pixmap modes were activated.

* Bug #4339 fixed - listvarinfile did not manage graphic handles.

* Bug #4818 fixed - Scilab could not export graphics which contained a lot of information.

* Bug #6582 fixed - A copy of a handle did not keep foreground property.

* Bug #6644 fixed - New graphic windows take care of default figure properties (uimenu).

* Bug #7412 fixed - Better management of datatips activation/deactivation.

* Bug #7419 fixed - Customization of datatips() is now available through a GUI.

* Bug #7998 fixed - %h_get failed on heterogeneous handle matrix.

* Bug #8239 fixed - Activation of datatips overwrote and destroyed any event_handler
                   formerly defined.

* Bug #8241 fixed - Graphical properties were not always inherited.

* Bug #8250, #10567, #10614 fixed - Artefacts dropped on
                                    vectorial exports (EPS/PDF/SVG).

* Bug #8705 fixed - Some specific graphics could not be saved.

* Bug #8916 fixed - Menu to export in PS format was missing in graphic figures.

* Bug #8965 fixed - copy function did not work for handles.

* Bug #9269 fixed - Specified graphic entities for legend were drawn in reverse order.

* Bug #9330 fixed - delete() called overloading instead of deleting the current
                   graphic entity.

* Bug #9455 fixed - The contourf and contourf 2 demonstrations displayed unwanted dots.

* Bug #9721 fixed - tight_limits help page improved. Thanks to Samuel
                    Gougeon.

* Bug #10121 fixed - polyline.interp_color_vector where not saved or recoverable
                    by xsave/xload. Thanks to Samuel Gougeon for the fix.

* Bug #10240 fixed - In a Scilab localized in French, SVG export could use "," instead
                     of "." for numerical separator.

* Bug #10290 fixed - Error message for bad assignement of interp_color_vector updated.

* Bug #10298 fixed - The example of datatipSetDisplay generated an error.

* Bug #10331 fixed - datatipCreate produced a warning and an error instead of
                    a warning if the curve user_data was not a struct.
* Bug #10360 fixed - Default format for export is now PNG instead of BMP.

* Bug #10365 fixed - To copy a handle to a figure (instead of an axes) crashed Scilab.

* Bug #10395 fixed - xset("clipping", [1,2,3,4]) returned an error.

* Bug #10417 fixed - xget("background") returned a wrong value.

* Bug #10637 fixed - xs2svg crashed when the first input argument was not a figure handle.
                     An error message is now displayed.


GUI
====

* Uicontrols tooltip can now be managed using the new "TooltipString" property.

* Figure closure can now be managed by the user (confirmation, ...) through the
  'CloseRequestFcn' property.

* F5 and F6 change the focus of the docked element.

* Improvement in the refresh of the docked elements.

* A 'Console' handle has been created and can be obtained by calling get(0):
  - uimenus now appear as 'Children' of this handle when they are added to the
    console,
  - the 'ShowHiddenHandles' property enables the user to make some hidden
    graphic objects visible.

* Scilab Console and graphic figure menus are now uimenus. They are hidden by
  default. To see them in the children list, use set(get(0), "ShowHiddenHandles", "on")

* New uicontrols added:
  - 'image' style uicontrol: display, transformation and various other
    operations on images.
  - 'table' style uicontrol: display data in a table form (read-only).

* The 'Value' property can now contain non-integer values and allows to take full
  advantage of 'slider' style uicontrols (See bug #7380).

* Figure resizing can now be managed by the user (uicontrol position setting, ...)
  through the 'resizefcn' property.


CLI Console (GNU/Linux and Mac OS X only)
==========================================

* Wide characters supported.

* Multiline command supported.

* Command line display improved.

* List of shortcuts in NW/NWNI mode:
  - CTRL+W removes the previous word.
  - CTRL+U removes everything from the current position to the beginning of
            the line.
  - UP or Ctrl+P recalls the previous line.
  - DOWN or Ctrl+N recalls the next line.
  - TAB for completion. Scilab displays a list of all names (functions, variables) that
    start with the characters already typed by the user.
  - Ctrl + A or HOME moves the cursor to the beginning of current line.
  - Ctrl + B or LEFT moves the cursor one character to the left.
  - Ctrl + D or DELETE deletes the current character.
  - Ctrl + E or END moves the cursor to the end of command line.
  - Ctrl + F or RIGHT moves the cursor one character to the right.
  - Ctrl + H or BACKSPACE deletes the previous character.
  - Ctrl + K kills the command line from cursor to the end.
  - Alt+B or Ctrl+LEFT moves one word left.
  - Alt+F or Ctrl+RIGHT moves one word right.

* Bug #3108 fixed - Some specific chars in the terminal were not correctly
                    removed by the backspace.

* Bug #3400 fixed - Long lines in the history broke the display.

* Bug #6804 fixed - Prompt was removed in CLI mode when playing with arrow keys.


Arnoldi Package
================

* New function:
  - eigs function added (See SEP #82).


Elementary Functions
====================

* New functions:
  - dec2base function added (See SEP #75).
  - nthroot function added (See SEP #78).

* linspace and logspace deprecate the use of non-integer values as third input argument.

* The toeplitz macro is now vectorized.


Sparse
======

* qmr function updated and modified.
   Calling qmr(F, Fp, x) where F is a function returning A*x
   and Fp a function returning A'*x is deprecated.
   Use qmr(F, x) instead where F prototype is
   function y = F(x, t)
   [...]
   endfunction
   where t is used to make F return A*x or A'*x.
   Same behavior has been implemented for qmr( ..., M1, M1p, M2, M2p).
   See qmr help page for examples and details.


Scilab
======

* save/load functions can export/import Scilab data to SOD format:
 - Scilab data are saved and loaded using a standard, HDF5-based, documented,
   and fully tested: Scilab Open Data format (SOD).
 - Graphic handles can be saved under the SOD format.
 - 'save' writes data to HDF5 format when using new calling sequence
   save("filename.sod", "a", "b", "c"). This format is Scilab 6 compatible.
 - 'save' displays a message when using old calling sequence
   save("filename", a, b, c) and writes data in old Scilab binary format.
   This format will not be supported by Scilab 6.
 - The 'matfile2sci' function now converts MAT-files to SOD files.

* Calling load with a file descriptor as first input argument is now obsolete.
  Please replace the file descriptor by the corresponding file name.
  This feature will be removed in Scilab 6.0.0.

* Update the default size of the Scilab stack to 100 million elements (instead of
  50M). See stacksize help page for more information.

* Scilab Desktop provides a button in dockbar to get the help about the component.

* External modules are managed out of the box by test_run function. See bug #9834.
Example: test_run('SCI/contrib/toolbox_skeleton')


XML Features
=============

* Version 5.4.0 introduces XML management features. It allows easy access to
XML files, validation and writing.

 - XMLObjects - Describes the properties of the different XML objects
 - xmlAddNs - Adds a namespace to a XML Element.
 - xmlClose - Closes a XML document.
 - xmlDTD - Creates a XML DTD object.
 - xmlDocument - Creates a new XML document.
 - xmlDump - Dumps a XML object.
 - xmlElement - Creates a new XML element.
 - xmlFormat - Formats a Scilab variable into XML.
 - xmlGetNsByHref - Gets a namespace by its href.
 - xmlGetNsByPrefix - Gets a namespace by prefix.
 - xmlGetOpenStreams - Gets all open XML documents or all open XML Validation files.
 - xmlNs - Creates a new XML Namespace.
 - xmlRead - Reads a XML stream from a local or distant file.
 - xmlReadStr - Reads a XML tree from a string.
 - xmlRelaxNG - Creates a XML Relax NG object.
 - xmlSchema - Creates a XML Schema object.
 - xmlValidate - Validates a document in using a DTD, a Relax NG or a Schema.
 - xmlWrite - Writes a XML document in a file.
 - xmlXPath - Makes an XPath query on a XML document.
 - xmlIsValidObject - Tests the existence of a XML object.

Those features have been developed in the context of OASIS R&D project.


CSV features
=============

* New functions:
  - csvRead - Reads a CSV file.
  - csvWrite - Writes data into a CSV file.
  - csvTextscan - Transforms a CSV line into a matrix.
  - csvDefault - Configures the CSV behavior.

* csvRead and cvsWrite provide the same features as read_csv and write_csv
  with major improvements in term of usability and performances:
  - Writing performances are improved to a 100x factor,
  - Reading performances are improved from 3x to 200x factor.

* Bug #8349 fixed - write_csv() failed to support the writing of complexes with
                    %nan or %inf parts.

* Bug #8653 fixed - csvRead now returns double values by default.
                    (read_csv still returns string values)

* Bug #8654 fixed - write_csv did not use the standard separator and decimal mark.

* Bug #8656 fixed - write_csv had accuracy issues.

* Bug #9969 fixed - Issues of performances of CSV functions for large files.

* Bug #10203 fixed - CSV function help pages did not detail the encoding
                     management.


SciNotes
=========

* Marks added in the vertical scrollbar to see the positions of matching words
  when an incremental search is done (via F3 or in selecting text).

* In the find/grep tools, possibility added to use ~ as an alias for user home
  dir. and SCI for Scilab data base dir.

* 'Save and Execute all files' action added.

* A tab can be closed in using the middle mouse button.

* Shortcuts META+(UP|DOWN) added to navigate between paragraphs.

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

* Bug #9897 fixed - Under Mac OS X, eol was not replaced by \n when code was copied
                   from Word to SciNotes.

* Bug #10157 fixed - Scilab froze when docking SciNotes due to a FlexDock bug in a
                    previous session.

* Bug #10252 fixed - The font could not be changed when SciNotes was docked with the
                    console.

* Bug #10354 fixed - Bad caret position after a selection action.

* Bug #10442 fixed - Sometimes a crash occurred at start-up when SciNotes was
                     restoring.

* Bug #10647 fixed - An error message did not respect Scilab localization standard.

* Bug #10889 fixed - Exception about block highlighters was thrown by SciNotes.


Call_Scilab
============

* CallScilab.h removed (was deprecated since Scilab 5.2.0).
  Please use call_scilab.h instead.

* 'int getLastErrorValue(void)' function added.
  It now returns the last error code generated by Scilab.

* Function 'const char* getLastErrorMessageSingle();' added.
  It now returns the last error message generated by Scilab.

* Deprecated functions in call_scilab.h removed : C2F(readmat), C2F(creadmat),
  C2F(creadbmat), C2F(creadcmat), C2F(readchain), C2F(creadchain),  C2F(creadchains),
  GetLengthStringMatrixByName, C2F(matptr), C2F(cmatptr), C2F(cmatcptr),  C2F(cmatsptr),
  C2F(cmatbptr), getlengthchain

* Bug #10006 fixed - Unrelevant C-comment in the example of ScilabHaveAGraph
                    removed.


Javasci
=======

* Javasci version 1 removed (deprecated in Scilab 5.3.0).

* New datatypes are now supported:
  - sparse and boolean sparse
  - tlists, mlists and lists
  - polynomial

* If Scilab fails to start because of a memory issue (stacksize), a
  Java exception is now displayed (InitializationException).

* Bug #10794 fixed - With Scilab built on an Ubuntu 11.10, javasci failed to
                     load with an undefined symbol


Dynamic Link
=============

* DEBUG_SCILAB_DYNAMIC_LINK was not available under GNU/Linux or Mac OS X.
  See bug #11142

* Bug #9432 fixed - Link a 64-bit dynamic library on a 32-bit platform returned
                   a wrong error message under Windows.

* Bug #9761 fixed - NDEBUG was not defined for dynamic link (by default) in
                   release mode under Windows.

* Bug #9930 fixed - ilib_build did not display all filenames used.

* Bug #10320 fixed - libname argument of ilib_for_link did not work.

* Bug #10344 fixed - Windows SDK 7.1 was not detected with VS Express 2010
                    under 64-bit platform.


Xcos
=====

* Major performance improvements on the simulation start-up. On important diagrams,
  the gain is very important (from minutes to milliseconds). See bug #8188.

* Performance improvements on diagram saving/loading.

* A compressed file format is now available (ZCOS) and allows more flexibility on
  data format.

* Ports styles and labels are now available to external modules.

* New examples are provided in the help pages of the following blocks:
  - ABS_VALUE - Absolute value
  - EVTDLY_c - Event delay
  - AFFICH_m - Display value
  - CLSS - Continuous state-space system
  - SELECT_m - Selector block
  - NEGTOPOS_f - Threshold negative to positive

* External modules needs tp be updated to use the new Scilab Open Data format
  instead of the previous HDF5 format.

* Help pages updated with examples and translated into French:
  - WFILE_f - Block to write to output file,
  - WRITEC_f - Block to write C binary file.

* xcos_toolbox_skeleton will not be loaded if Xcos is not installed.

* scilab -f file.xcos opens Xcos diagram in Xcos.

* Error reporting has been improved to print warnings on blocks into Xcos.

* The style of a block can be changed in the interface function.

* BARXY Block added to help 2-D Mechanics system visualization.

* Bug #6201 fixed - Block help menu had no shortcut.

* Bug #6733 fixed - Each call to Xcos created an empty diagram.

* Bug #7539 fixed - Interaction with some blocks was slowed down.

* Bug #7863 fixed - Default path for code generation was SCI instead of pwd() or
                    TMPDIR.

* Bug #7985 fixed - dmmul.c did not use dgemm routine optimized by BLAS.

* Bug #8188 fixed - Xcos to Scilab export took too much time.

* Bug #8295 fixed - Save compressed Xcos file set as default.

* Bug #8421 fixed - CLKFROM and CLKGOTO were not part of the "Signal Routing"
                   palette.

* Bug #8510 fixed - The message "Diagram contains implicit blocks..." stopped
                    all process.

* Bug #8538 fixed - Counter block label was not reflecting parameters.

* Bug #8600 fixed - Closing Xcos did not end the simulation.

* Bug #8742 fixed - On the Modelica initialization GUI, the error reporting did
                   not work.

* Bug #8743 fixed - On the Modelica initialization GUI, it was impossible to
                   re-fix the states after a derivative fix.

* Bug #8744 fixed - Solving the initialization problem more than once crashed
                    Scilab.

* Bug #9037 fixed - When connecting blocks, the green port blinked on a small mouse move.

* Bug #9075 fixed - The inverted pendulum demonstration in Xcos/Control systems
                    opened a useless graphic windows

* Bug #9131 fixed - PULSE_SC has been added to the Source palette.

* Bug #9290 fixed - HALT blocks displayed STOP.

* Bug #9301 fixed - Exception when one tried to paste a column to replace a
                   column selection.

* Bug #9336 fixed - SuperBlock with non-connected ports produced an invalid
                   index error.

* Bug #9390 fixed - FROMWSB_c block did not reset to the default values on
                   reinitialization execution.

* Bug #9478 fixed - Block structure was not checked while adding them to a new
                   palette.

* Bug #9496 fixed - Xcos flag 5 (ending) was not called when simulation was
                    stopped from the user interface.

* Bug #9501 fixed - With a localized TMPDIR, block edition could throw an HDF5
                   exception.

* Bug #9506 fixed - Some libraries dependencies were not loaded at
                   xcos_toolbox_skeleton start-up.

* Bug #9532 fixed - User-defined blocks had no icons when saved.

* Bug #9547 fixed - The 'about box' was not localized due to a multi-line string.

* Bug #9579 fixed - Duplicate content in scicos_math.h removed and replaced by
                   a cleaner call to math.h instead.

* Bug #9590 fixed - Xcos examples were not embedded in Windows binary version.

* Bug #9732 fixed - Code generation failed at compilation under Windows with
                   "unresolved external symbol dset_".

* Bug #9750 fixed - Minor error in scicos_debug help page fixed.

* Bug #9810 fixed - Errors dealing with MATINV wrong simulation values have been improved.

* Bug #9927 fixed - Xcos crashed at XcosPalAdd command execution.

* Bug #9947 fixed - VARIABLE_DELAY block could not be dragged to a diagram.

* Bug #9972 fixed - Error reporting with a wrong context produced an endless
                   loop.

* Bug #9988 fixed - CLKSOMV_f had a wrong interface function after loading from
                   a file.

* Bug #10003 fixed - The MAXMIN block did not update its display according to
                    the settings

* Bug #10041 fixed - WFILE_f block was not declared as deprecated.

* Bug #10048 fixed - Blocks which use super blocks now loop over their children
                    to find the parameters dialog to open.

* Bug #10069 fixed - AFFICH_m block did not update its display at
                    reinitialization.

* Bug #10081 fixed - Palette configuration saving could produce an invalid
                     file.

* Bug #10096 fixed - Set [1;-1;] on a product block thrown an exception.

* Bug #10107 fixed - Default value of the Integrator Absolute Tolerance was
                     too large. It has been reduced to 1e-6.

* Bug #10246 fixed - PROD_f block was broken.

* Bug #10302 fixed - LaTeX labels were not rendered.

* Bug #10333 fixed - Labeling I/O blocks on a super block produced wrong port
                     labeling when ports had not the same types.

* Bug #10340 fixed - Exception occurred while moving the palette split pane to
                     hide the block list and while using a palette diagram.

* Bug #10393 fixed - Block style was not updated on palette loading.

* Bug #10419 fixed - Exception occurred while refusing to create a new empty
                     file.

* Bug #10422 fixed - Exception occurred when exporting with an unspecified
                     format.

* Bug #10433 fixed - Xcos stop simulation button did not work properly.

* Bug #10461 fixed - Canceling a Quit operation did not work.

* Bug #10486 fixed - importXcosDiagram opened a tab.

* Bug #10496 fixed - Wrong error message fixed in xcosValidateBlockSet.

* Bug #10510 fixed - Unused variables were generated by Xcos at start-up.

* Bug #10533 fixed - Some action shortcuts were not disabled when editing a text
                     block.

* Bug #10657 fixed - CONSTRAINT_c had no icons on a graph.

* Bug #10679 fixed - blocks_link.tst failed.

* Bug #10697 fixed - post_xcos_simulate function did not work with a TOWS_c block
                     in a diagram.

* Bug #10781 fixed - DFlipflop incorrectly reported problem for port size or type.

* Bug #10800 fixed - xcosPalAddBlock help example did not work.

* Bug #10812 fixed - cos and cosf files loading did not use a direct handler.

* Bug #10848 fixed - The first Drag'n Drop of a block produced an exception.

* Bug #10943 fixed - Accented character in the file path broke file loading
                     in Xcos.

* Bug #11094 fixed - Image creation of blocks did not use xinit / xend.

* Bug #11132 fixed - Some help pages were missing: standard_origin, ...

* Bug #11494 fixed - Xcos PDE block could not open UIcontrol to set parameter.

* Bug #11518 fixed - When zooming on CLR block-text, the block was outside the box.

* Bug #11550 fixed - Closing the scope graph while running simulation led to a
                     segfault.

* Bug #11717 fixed - Setting recent opened file number in the preferences GUI
                     was not applied in Xcos menu.

* Bug #11735 fixed - Output sink name was not reported as figure name.

* Bug #11737 fixed - Incompatibility between a Xcos menu and its help page description.

* Bug #11746 fixed - Closing the palette window with the crossing box
                     did not update the checkbox in Xcos "View" menu.

* Bug #11747 fixed - Modelica block text was visible under the block image.


Localization
=============

* Czech localization by Zbynek Schwarz


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


Optimization
=============

* Demonstration added on neldermead: search for largest / smallest polygon.

* Clarity of examples improved in optimization module help pages:
  Avoid unnecessary calls to "deff",
  Avoid the "all statements in whole line" style.

* readmps - Created unit test.
            Bug fixed in error messages management.
            Bug fixed in a test file reading.
            Defined a printing system by overloading for MPS structure.
            Help page updated.

* Performances improved of genetic_algorithms (2 to 4 times faster).

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
                   Same changes applied to optimbase help page.

* Bug #8543 fixed - Wrong order for the options in neldermead help page.

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
                   Added examples of restarts with/without bounds in
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


Tests
======

* The assert module provides a set of assertion functions.
  The goal of this module is to provide functions to check the
  behavior of some other functions, for example in unitary tests.
  We emphasize the use of consistent tools for testing numerical
  issues, with the goal of testing numerical algorithms more easily.
  In particular, we provide a comparison function for two floating
  point numbers, which allows to check that two numbers are
  "numerically almost equal", i.e. that the relative error is small.
  This automatically fixes Bug #4381.

* A fourth argument added to test_run to export test results to XUnit format.

* A new function called example_run has been introduced, this function tests the
  examples given in Scilab help pages. See SEP #83.


Statistics
===========

* General improvements of the show_pca function (See bug #11500):
  - Data bounds fixed in the left graph,
  - Correlation circle graph improved,
  - Eigenvalues graph improved.

* Bug #9357 fixed - stdevf returned zero for row vector arguments.

* Bug #9846 fixed - stdevf gave zero result when a single row matrix was evaluated.

* Bug #7387 fixed - rand and grand functions help pages gave wrong advice.


Linear Algebra
===============

* cond function updated to take into account a second input argument (See SEP #85)

* Bug #9749 fixed - expm reported non convergence problem.


Sparse Matrices
================

* ordmmd help page and unit test created.

* Help pages organized into sub-sections.

* Bug #10126 fixed - Wrong (or failing) insertion in a sparse matrix.


Documentation
==============

* A new system to keep track of the translations of the main documentation
  has been developed. See http://help.scilab.org/status/fr_FR/,
  help.scilab.org/status/pt_BR/ and http://help.scilab.org/status/ja_JP/
  See Bug #5119.

* Categories have been created for the following modules help pages:
    - Scilab. See bug #11546.
    - Statistics. See bug #9108.
    - Genetic algorithms. See bug #11514.
    - Simulated annealing. See bug #11560.
    - Signal processing. See bug #11661.
    - Functions (libraries sub-category). See bug #11642.
    - Linear algebra. See bug #11562.
    - Special functions. See bug #11643.
    - Functions. See bug #11722.
    - Input/Output functions. See bug #11726.

* Tag <scilab:image> added. This tag will perform the graphic rendering of
  a Scilab code and include the result into the help page as a sample image.

* With the <scilab:image> tag, all graphic and trigonometric help pages
  now contain a sample image.

* Management of <varname>, <command>, <function> and <constant> Docbook tags.
  See bug #11459.

* Management of <note>, <caution>, <warning>, <important> and
 <tip> Docbook tags. See bug #10505.

* help_from_sci updated to manage recent changes in docbook Scilab files.

* Elementary functions module help pages have been better organized.
  See bug #11723.

* Automatic spelling corrections for Scilab Online help - http://help.scilab.org/

* <ALT>+<LEFT ARROW> / <ALT>+<RIGHT ARROW> allows browsing the history
  (See Bug #9474).

* The space key behaves like the page down key (See Bug #9491).

* Management of the <revhistory> and <revision> tag to provide information about
  the history of the function.

* Function help_skeleton extended to suggest the <revhistory>, <latex>, <note>,
  <caution>, <warning>, <important> and <tip> tags in examples.

* Scilab documentation is now available in Russian. Many thanks to Stanislav Kroter.

* Many errors have been fixed in Scilab documentation:
  - Wrong calling sequences
  - Errors in examples
  - Bad formatting
  - Typos

* Bug #3431 fixed - Some help tools functions were obsolete and not removed.

* Bug #9105 fixed - err output parameter of mopen function was not documented.

* Bug #9299 fixed - Fix a typo in cspect, pspect and window help pages
                   (Hanning changed to Hann).

* Bug #9320 fixed - analpf help page gave wrong frequency units of omega (Hz instead of rd/s).

* Bug #9417 fixed - In English api_scilab help pages, polynom updated to the word
                   polynomial.

* Bug #7133 fixed - help_from_sci function did not return the function template.

* Bug #7134 fixed - help_from_sci produced an invalid .xml file.

* Bug #9790 fixed - windows_tools help pages were not available for all platforms.

* Bug #9983 fixed - The xclick help page did not clearly specify the axes used for
                   coordinates.


CACSD & Signal Processing
==========================

* New functions:
  - conv - discrete 1-D convolution.
  - conv2 - discrete 2-D convolution.
  - convol2d - discrete 2-D convolution, using fft.
  - phaseplot - frequency phase plot.
  - xcorr - auto and cross correlation of signals (superseeds corr).
  - xcov - auto and cross correlationvariance of signals (superseeds corr).
  - arma2ss - conversion of an arma data structure into a state space one.
  - dct - Discrete cosine transform (based on FFTW library).
  - idct - Inverse discrete cosine transform (based on FFTW library).
  - dst - Discrete sine transform (based on FFTW library).
  - idst - Inverse discrete sine transform (based on FFTW library).

* fft function based on fftw updated and improved (See SEP#84):
    - Take data symmetry into account,
    - Include hard coded multivariate fft.

* The damp function in CACSD module computes the natural pulsation
  and damping factor of linear dynamical systems.

* 'mtlb_conv' compatibility function is now obsolete. Please use 'conv' instead.

* Bug #1635 fixed - iir filter frequency response might have been wrong.
                  "pole zero gain" representation added as possible iir function
                  returned value.

* Bug #7895 fixed - fft2() was broken under Windows with MKL wrapper.

* Bug #7901 fixed - besselh.tst failed under Windows.

* Bug #8684 fixed - wfir function produced an error message when used interactively.

* Bug #9098 fixed - CACSD demonstrations / Robust control failed on Windows 64-bit.

* Bug #9214 fixed - abaque replaced by nicholschart in the black.sci comments.

* Bug #9266 fixed - fftw crashed under Windows when using MKL wrapper.

* Bug #9321 fixed - Missing tests and help info for some CACSD functions.

* Bug #9647 fixed - At Scilab start-up, the fft variable was declared by default
                   while it should not have been the case.

* Bug #9675 fixed - iir generated bad results.

* Bug #9716 fixed - amell.tst execution froze under Windows with reference lapack library.

* Bug #9719 fixed - delip.tst execution froze under Windows.

* Bug #9725 fixed - syredi.tst execution froze under Windows.

* Bug #10008 fixed - Wrong unit in the datatip for bode function.

* Bug #10169 fixed - Bad clipping option in evans.

* Bug #10562 fixed - The "CACSD/Robust Control" demonstration failed.

* Bug #11092 fixed - Incorrect argument check in h_inf

* Bug #11340 fixed - bode, gainplot, nyquist and plzr without arguments did not show an example.



Windows version
================

* Scilab can now be installed without administration rights.
  In this case, the file association will not be configured.

* ATOMS module is now mandatory and is installed in the minimal installation.


Performances
=============

* Under GNU/Linux and Mac OS X, Scilab start-up speed of scilab-cli and
  scilab-adv-cli has been improved by 15 to 20 % (see Compilation for more
  information).

* SSE is now enabled by default under GNU/Linux with 32-bit CPU (it is already
  the case under 64-bit CPU).
  Pentium III class (or equivalent) with SSE instructions is required.

* Improved performances of pareto_filter function (Genetic algorithms).

* 'hank' function performances improved.

* Using api_scilab, construction of any type of lists is now much faster (300x in some cases).

* Bug #8608 fixed - dec2hex, hex2dec, dec2oct, oct2dec, base2dec code have been
                   simplified and optimized.


Compilation
============

* Under Windows, Scilab uses VS 2010 Pro and Express solution.

* Under GNU/Linux and Mac OS X, engine libraries have been merged into a single
  library named libscilab-cli.so. This greatly improves the start-up
  performances.
  Note that the --enable-maintainer-mode enables the previous behavior (simply
  the development process).

* Under GNU/Linux and Mac OS X, all modules build as standalone their algorithms
  as a static library (only available in the source tree).
  This will avoid an important dependency tree for unitary testing.

* Under GNU/Linux and Mac OS X, for each module, the list of
  explicit dependencies on the other libraries is no more needed.
  Use libscilab-cli.so or libscilab.so to link against Scilab.

* Scilab dependency on Java HDF has been removed.

* Enforce a minimal version of JRosetta (1.0.4).

* Enforce a minimal version of flexdock (1.2.0).

* Enforce a minimal version of GIWS (1.3.0).

* Enforce a minimal version of Scirenderer (1.0.2).

* Enforce a minimal version of JLaTeXMath (1.0.0).

* Enforce a minimal version of JOGL (2.0-rc9).

* Force the Java class version target to version 6 (or 1.6).

* Add Java 7 in the Debian & Ubuntu search path under GNU/Linux.

* BLAS_LIBS variable can now be set by the user (See bug #9824).

* Scilab now uses the ARPACK-ng package maintained by Scilab Enterprises.

* An option --without-arpack-ng has been added to disable the Arnoldi features.

* HDF5 is now a mandatory dependency of Scilab (even in a minimal build).

* Under GNU/Linux and Mac OS X, if ccache is installed, use it to build Scilab.

* Bug #9409 fixed - Warning "remark #8291" removed with Intel Fortran 2011 compiler.

* Bug #9500 fixed - Explicit the GUI module manifest class-path to facilitate Debian
                   checking tool process.

* Bug #9782 fixed - configure with --without-gui was failing.

* Bug #9793 fixed - --without-gui now also disables Xcos.

* Bug #9883 fixed - Build with --disable-build-help was failing.

* Bug #9991 fixed - ./configure --without-xcos failed to build.

* Bug #11060 fixed - ccache use can be disabled with --disable-ccache .

* Bug #10158 fixed - Configure used g77 even if gfortran was available.


History
========

* Default text in history for a starting session has been changed for:
"Begin Session: Tue Jun 21 14:35:45 2011" to "21/06/2011 14:35:45"


ATOMS
======

* 'atomsQuit' function added to allow to call .quit of modules loaded by ATOMS
  when user closes Scilab session. See SEP #77.

* tbx_build_loader generates also an unloader.sce to allow to call .quit file of
   an ATOMS module.

* ATOMS option 'offLine' has been renamed to 'offline'.

* Bug #5380 fixed - atomsShow() output was broken (separators were not aligned).

* Bug #8302 fixed - Wrong order when loading dependencies.

* Bug #8648 fixed - The tarball was not deleted when a module was removed (SEP #71).

* Bug #8664 fixed - Message error updated for non-supported OS.

* Bug #8763 fixed - ATOMS installation path was shared on Windows 64 with 32-bit.

* Bug #8942, #9805 fixed - atomsSystemUpdate() failed under some Windows.

* Bug #9078 fixed - atomsGui took time to start gui.

* Bug #9127 fixed - "Home" item renamed to "Installed Modules".

* Bug #9679 fixed - atomsSystemUpdate checks that Scilab is not in offLine mode, or writes a warning message if so.

* Bug #9798 fixed - .quit script of Atoms modules were not evaluated at Scilab quit.

* Bug #9967 fixed - Windows UAC was causing trouble with atomsRepositorySetOfl (internal function).

* Bug #10025 fixed - Wrong url in some help files examples.

* Bug #10031 fixed - atomsDisp did not work with "\n" in messages text.

* Bug #10494 fixed - atomsInstall and atomsLoad were not clearly distinguished
                     in the documentation.

* Bug #10500 fixed - ATOMS module without DESCRIPTION file returned a wrong
                     error message.

* Bug #10707 fixed - atomsInstall failed to use the path shortcut.

* Bug #10846 fixed - atomsSystemUpdate failed.
                     'downloadTimeout' help page added.

* Bug #10864 fixed - atomsRemove required a network connection to remove a package.

* Bug #10933 fixed - atomsGetConfig did not check the value of input arguments.

* Bug #10957 fixed - atomsInstall required a network connection to install
                     a local package.

* Bug #11062 fixed - atomsGetInstalledPath function moved as a public function.


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


API Scilab
==========

* Add functions to identify list datatypes:
  isListType, isNamedListType, isTListType, isNamedTListType, isMListType,
  isNamedMListType
  See bug #11106


Misc
=====

* The linspace function accepts column vectors as input argument for the two
  first input arguments. Thanks to Guillaume AZEMA for the implementation.

* The logspace function accepts column vectors as input argument for the two
  first input arguments.

* Change the buffering display mode.

* SEP #67: lines(0) by default at start-up.

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


Obsolete Functions
===================

* 'oemtochar' and 'chartooem' functions (windows_tools) are obsolete and will be
   removed in Scilab 5.4.1.

* 'showalluimenushandles' function (graphics) was undocumented. This function is
  obsolete and will be removed on 5.4.1.
  Use set(get(0), "ShowHiddenHandles", "on"); instead.

* 'code2str' and 'str2code' are obsolete and will be removed on 5.4.1, please use
  'ascii' instead.

* config function is now obsolete and will be removed in Scilab 5.4.1.
  Please use 'preferences' instead.

* dnaupd, dneupd, dsaupd, dseupd, znaupd and zneupd deprecated for eigs.

* createpopup and uipopup functions are now obsolete. Please use 'uicontextmenu' instead.


Removed Functions (previously declared as obsolete)
====================================================

* ilib_for_link, ilib_build no more accept files with ".o" or ".obj" extensions
  for source files.

* 'with_atlas' removed. Please use 'getdebuginfo' instead.

* C API function Error(x) removed. Please use SciError instead.

* 'MSDOS' removed. Please use 'getos' instead.


Known Incompatibilities
========================

* In previous versions, modulo(x,0) and pmodulo(x,0) returned x. But this solution is
  not correct. Indeed, the expression used to compute the modulo is x - y * int(x / y)
  for the modulo function and x - y * floor(x / y). Now, they return an error and
  %nan with ieee(2). See bug #7216.

* In previous versions, newest function could be called with a string vector
  as second (up to last) input argument. In these cases, only the first string of
  the vector was taken into account. These calls now return an error. See bug #11398.

* Setting the "foreground" property for a figure was possible even if it was not
  officially supported or documented. An error is now returned.
  Please use the axes "foreground" property instead. See bug #11369.

* getcallbackobject: Input argument must be an UID (String) and no more an index (Constant).


Known Issues
=============
* Some issues at start-up under Mac OS X 10.6.7 (and previous 10.6 versions)
  have been reported.
  If you have "Audio Hijack" installed on your Mac OS X, it prevents some
  applications to work including Scilab. See Bug #7012.
  Uninstall / disable Audio Hijack should fix this problem.
  Airfoil, also developed by Rogue Amoeba, has been reported as also breaking
  Scilab.


Bug Fixes
==========

* Bug #1604 fixed - List extraction failed.

* Bug #2479 fixed - Graphic editor could not be used when format was not format("v",18).

* Bug #2853 fixed - sd2sci.sci removed from sources.

* Bug #3291 fixed - SCI/modules/tclsci/tcl/BWidget-1.8.0/ moved as prerequirements.

* Bug #3235 fixed - In bezout function, wrong typo in the name of a variable
                    made the calpol unitary test failed.

* Bug #3244 fixed - Error messages in qpsolve and qp_solve functions updated.

* Bug #3787 fixed - Invalid calling sequences removed from schur help page.

* Bug #3811 fixed - The overloading charcode was wrong for libraries.

* Bug #4240 fixed - Some unexpected blank lines were displayed with the dir function.

* Bug #4337 fixed - who_user did not return the user's variable names but
                    only displayed them.

* Bug #4576 fixed - An example of ilib_build with "cflags" argument was missing.

* Bug #4806 fixed - If A was a sparse matrix, A^0 returned A and not the identity
                    matrix as expected.

* Bug #4813 fixed - Description of type 0 added in type help page.

* Bug #4833 fixed - msprintf() interpreted "\n" as a carriage return even if "\n"
                   was supplied by the 2nd input argument.

* Bug #4847 fixed - In uigetfile, there was no way to select files with a given
                   pattern.

* Bug #4978 fixed - Icons did not follow the Tango norm.

* Bug #5491 fixed - Clearly states that haveacompiler() only works under Windows.

* Bug #5675 fixed - Levitron demonstration was failing in some cases.

* Bug #5696 fixed - When SE Linux is enabled, error messages for
                    Scilab in the Scilab binary distribution disabled.

* Bug #6009 fixed - conv, conv2 and convol2d functions were missing.

* Bug #6154, #10850 fixed - Arnoldi package help pages updated.

* Bug #6408 fixed - prompt() removed trailing whites.

* Bug #6538 fixed - unobs ignored its 'tol' argument when it was given,
                    and always calculated its own, arbitrary, internal value.

* Bug #6702 fixed - max and min functions returned a scalar in k when the input
                    argument was the last column of an hypermatrix.

* Bug #6819 fixed - prompt() interpreted '<...>' as HTML.

* Bug #6875 fixed - test_run() could not run at once on the +3000 tests of
                    Scilab.

* Bug #7061 fixed - changelog.txt, readme.txt, version.xml removed from modules.

* Bug #7167 fixed - Typo fixed in pdiv help page.

* Bug #7216 fixed - modulo(x,0) and pmodulo(x,0) did not return an error message.
                    With ieee(2), these functions now return %nan.

* Bug #7249 fixed - 'string' function did not work as documented
                    with a macro as input argument.

* Bug #7251 fixed - Error fixed in qp_solve help page.

* Bug #7289 fixed - The handle of the menu was still valid after calling delmenu(console_menu_handle).

* Bug #7333 fixed - In BrowseVar, variables could have a weird type number.

* Bug #7358 fixed - type and typeof help pages have now links to the actual
                   description pages.

* Bug #7380 fixed - 'Value' property for uicontrol was rounded.

* Bug #7409 fixed - Some corrupted matrices could be created from api_scilab.

* Bug #7490 fixed - %inf had not always the same display.

* Bug #7674 fixed - In all Optimization and Simulation demonstrations, the source
                    code was displayed by default.
                    A messagebox now lets the choice to the user to display the
                    code or not.

* Bug #7700 fixed - Bad proposition in the console history when a char was added.

* Bug #7728 fixed - rubberbox(): Instantaneous pointer's coordinates are now displayed in the status bar.

* Bug #7745 fixed - add_demo moved in demo_tools module.

* Bug #7763 fixed - mtlb_max and mtlb_min functions did not manage complex
                    numbers with null imaginary part.

* Bug #7779 fixed - genlib did not check input arguments and did not use current
                    path as default.

* Bug #7815 fixed - tohome() function erased previous and future commands in Scilab console.

* Bug #7855 fixed - It was not possible to change event function while
                    the event handler was enabled.

* Bug #7859 fixed - Improvements of "exists" help page.

* Bug #7873 fixed - The lagr parameter in qld help page was wrong.

* Bug #7874 fixed - qp_solve help page did not give enough information
                    about iact.

* Bug #7881 fixed - Error in the condition which checks if the second input
                    argument in lattn function was scalar or not.

* Bug #8050 fixed - A weird error message was displayed in some cases.

* Bug #8086 fixed - Some functions displayed a wrong error message when called
                    without input argument.

* Bug #8101 fixed - median function with a sparse as a first argument did not
                    return the good result. Moreover, median(sparse, 'r'| 'c') was not
                    managed.

* Bug #8151 fixed - Under Mac OS X, "Scilab/Quit Scilab" and "File/Quit" items were
                    duplicates.

* Bug #8243 fixed - xgeom demonstration renamed to geom3d.

* Bug #8245 fixed - gsort() did not allow overloading with not managed types.

* Bug #8296 fixed - Asking for extra array dimension value with size crashed Scilab.

* Bug #8339 fixed - Default path for the Mac OS X version is now HOME instead
                    of "/".

* Bug #8351 fixed - integrate function did not manage properly the optional
                    input arguments.

* Bug #8420 fixed - lines crashed Scilab in some cases.

* Bug #8488 fixed - NaN values were propagated when min/max were performed between
                    several matrices.

* Bug #8592 fixed - Explicit error message added when macros are not available.

* Bug #8608 fixed - dec2hex, hex2dec, dec2oct, oct2dec, base2dec code have been
                    simplified and optimized.

* Bug #8640 fixed - histplot function failed when the data vector contained
                    the same values.

* Bug #8651 fixed - The error message of write_csv was wrong.

* Bug #8682 fixed - funcprot did not return the previous value when called with
                    an argument.

* Bug #8685 fixed - Lack of consistency in keyword help page.

* Bug #8688 fixed - fillMatrixOfString did not check available memory on stack.

* Bug #8724 fixed - A critical error with uiget(file|dir) could occur.

* Bug #8753 fixed - c_link help page did not describe input argument num.

* Bug #8768 fixed - GetRhsVar(..., STRING_DATATYPE, ...) did not check size of input argument.

* Bug #8785 fixed - modulo and pmodulo functions did not check input arguments.

* Bug #8797 fixed - Update of getdate('s') help pages to detail that
                   the returned value contains the leap seconds.

* Bug #8799 fixed - gsort did not work with hypermatrices.

* Bug #8859 fixed - Some comparison operators between hyper-matrices and 2-D arrays
                    were missing.

* Bug #8874 fixed - Display of complex numbers with Inf or NaN for real or imaginary
                   parts was broken.

* Bug #8963 fixed - mtlb_mode test failed under Windows.

* Bug #9002 fixed - bsplin3val, cshep2d, eval_cshep2d,interp, interp2d, interp3d,
                    linear_interp, splin, splin2d, splin3d functions ignored imaginary
                    part of input arguments without returning an error.

* Bug #9032 fixed - The rectangular contour of a 'Text' object did not fit the text
                    for huge font sizes.

* Bug #9045 fixed - plot2d did not support overloading.

* Bug #9056 fixed - A uicontrol could not be closed with <META>+W.

* Bug #9104 fixed - 'warning' used in a diary Scilab session added timestamp
                   twice in the diary.

* Bug #9129 fixed - Completion on field did not propose graphics handle fields
                   by default.

* Bug #9156 fixed - mgetl.c compilation failed under some Windows.

* Bug #9164 fixed - The unit tests now use the new assert functions.

* Bug #9197 fixed - Unitary test examples in toolbox_skeleton
                    and xcos_toolbox_skeleton did not use assert functions.

* Bug #9204 fixed - The Frobenius norm of a complex vector was numerically complex.

* Bug #9255 fixed - Correct bad calling sequence in list_double_writing_API help page.

* Bug #9268 fixed - Scilab was overriding a predefined variable JAVA_HOME.

* Bug #9277 fixed - Escape character '\' did not work with msprintf.

* Bug #9295 fixed - 'base2dec' ignored the fractional part of 'base' input argument.

* Bug #9328 fixed - Scilab randomly crached at start-up under Mac OS X when calling
                    functions returning no value (lines, modes, banner, ...).

* Bug #9339 fixed - tbx_build_help function did not manage languages properly.

* Bug #9354 fixed - savematfile could not save imbricate structures.

* Bug #9414 fixed - 'SCI' and 'SCIHOME' variables were not documented.

* Bug #9424 fixed - gettext("") did not return "".

* Bug #9452 fixed - It was impossible to download Intel MKL through a proxy.

* Bug #9461 fixed - The assert_checkalmostequal function did not manage
                   sparse matrices and hypermatrices.

* Bug #9464 fixed - contourf did not work if the "Matlab Compatibility functions
                    module" was not installed.

* Bug #9479 fixed - Typo fixed (intput => input).

* Bug #9498 fixed - A long history caused Scilab start-up to be very long.

* Bug #9502 fixed - 'toolbox' was badly used in assert help pages (replaced by 'module').

* Bug #9510 fixed - export_to_hdf5 did not return %f when trying to export
                    unmanaged types (function, macro, ...).

* Bug #9520 fixed - Missing named functions added in "API Scilab" help pages.

* Bug #9531 fixed - assert_checkequal failed with mlist.

* Bug #9540 fixed - MKL Pentium 4 target was not downloaded by installer under Windows.

* Bug #9542 fixed - Under Windows, during export operation, the file could be locked by
                    another process.

* Bug #9543 fixed - Scilab did not use the default DocumentBuilderFactory
                    to parse XML configuration files.

* Bug #9549 fixed - nthroot function added.

* Bug #9555 fixed - Typo error in deff help page.

* Bug #9571 fixed - "error(144)" returned a wrong variable name.

* Bug #9572 fixed - assert_checkerror did not check multi-lines errors.

* Bug #9573 fixed - getdebuginfo did not report multiple video cards under Windows.

* Bug #9581 fixed - cleaner.sce in toolbox_skeleton did not remove all generated files.

* Bug #9599 fixed - Error message "at line ... of function ... called by" was
                    printed directly on the same line.

* Bug #9616 fixed - In qp_solve help page, the input arguments in calling
                    sequence did not match those in arguments section.

* Bug #9635 fixed - Some temp. variables were not removed when test_run was used.

* Bug #9643 fixed - Genetic algorithms demonstration was not vectorized.

* Bug #9647 fixed - At Scilab start-up, the fft variable was declared by default
                   while it should not have been.

* Bug #9654 fixed - An example in optim help page was poorly implemented.

* Bug #9659 fixed - "error(246)" returned a bad message for overloading.

* Bug #9682 fixed - 3-D assignment into a 2-D array of integers issued an error.

* Bug #9693 fixed - The default value of epsg in optim help page was %eps.

* Bug #9696 fixed - An error in an if expression inside a try make Scilab froze.

* Bug #9699 fixed - A single $ as text in a ui text led to a segfault.

* Bug #9700 fixed - optim help page contained a mistake about bounds management.

* Bug #9707 fixed - createNamedXXX did not check variable name.

* Bug #9708 fixed - createNamedXXX did not check length of variable name.

* Bug #9712 fixed - StartScilab help page had an error about Stacksize argument type.

* Bug #9716 fixed - amell.tst execution froze under Windows with reference lapack library.

* Bug #9717 fixed - dllinfo.tst failed under Windows binary versions.

* Bug #9728 fixed - SCI/modules/special_functions/src/fortran/special_functions_f.def
                   was not ignored (generated file).

* Bug #9730 fixed - lines() returned an int32 and not a double value.

* Bug #9734 fixed - intsplin help page moved from interpolation to
                    elementary functions chapter.

* Bug #9735 fixed - Old stack api examples removed.

* Bug #9755 fixed - Completion on paths was not case insensitive under Windows.

* Bug #9756 fixed - polelm.tst unitary test failed under Windows 64-bit.

* Bug #9763 fixed - comet3d did not check input argument presence.

* Bug #9765 fixed - Remove ref to scicos_pal in sci_f_clear.f.

* Bug #9766 fixed - Scilab under Mac OS X 10.7 did not start.

* Bug #9791 fixed - Toolbar visible state change crashed Scilab in NW mode.

* Bug #9799 fixed - pol2str did not manage matrix of polynomials and input argument
                   type was not checked.

* Bug #9804 fixed - Starting Scilab with an external module emptied the
                   demonstrations list menu.

* Bug #9811 fixed - Output message of fminsearch was wrongly set.

* Bug #9824 fixed - BLAS_LIBS variable was overloaded by the configure.

* Bug #9825 fixed - The assert_computedigits function gave too large correct digits.

* Bug #9830 fixed - leastsq did not match the information given in optim help page.

* Bug #9833 fixed - percent was badly formatted by Scierror function.

* Bug #9836 fixed - repfreq failed due to singular problem.

* Bug #9838 fixed - evstr froze on some specific strings.

* Bug #9843 fixed - Colormaps with 2 colors could not be created.

* Bug #9858 fixed - length() could not be overloaded with mlist.

* Bug #9879 fixed - Fix some minor issues in isdir help pages.

* Bug #9880 fixed - Fix a page referencing itself in the help page.

* Bug #9881 fixed - Fix a mistake in call help page.

* Bug #9884 fixed - Possibility to add a default header on new files in SciNotes
                    added in Preferences.

* Bug #9885 fixed - User initialization scripts (scilab.ini, .scilab) did not work.

* Bug #9899 fixed - string(10*log10(1/10)) gave -0, instead of -10.

* Bug #9900 fixed - There was a memory leak in getAllocatedMatrixOfString,
                   getAllocatedMatrixOfWideString, getAllocatedNamedMatrixOfString,
                   and getAllocatedNamedMatrixOfWideString.

* Bug #9920 fixed - winqueryreg did not manage HKEY_CURRENT_CONFIG.

* Bug #9929 fixed - Improve integration with Gnome shell and Unity.

* Bug #9950 fixed - It was possible to select several texts in the console.

* Bug #9953 fixed - Documentation added to fre input argument of stdevf.

* Bug #9954 fixed - Example added in gmres help page.

* Bug #9957 fixed - msprintf("%d\r", 2) returned '2r'.

* Bug #9963 fixed - The assert_computedigits function was not vectorized.

* Bug #9966 fixed - The default value of comptype in assert_checkalmostequal
                   was wrongly chosen.

* Bug #9971 fixed - GLoad macro not used and not documented has been removed.

* Bug #9976 fixed - cdfbet function returned a wrong value instead of 0.

* Bug #9979 fixed - who_user() function printed a wrong message.

* Bug #9980 fixed - When one tried to complete an empty string, the console froze.

* Bug #9992 fixed - gcbo was not initialized in callbacks for uimenus in console.

* Bug #10001 fixed - %inf:%inf crashed Scilab.

* Bug #10016 fixed - The damp function unit test failed under Windows.

* Bug #10017 fixed - The unitary test for bdiag used a strange comparison
                     operator.

* Bug #10019 fixed - 'exec' crashed with error 113 for too long lines.

* Bug #10037 fixed - checkalmostequal.tst unitary test failed under Linux 64-bit.

* Bug #10044 fixed - wfir_gui function rewritten based on uicontrol and documented.

* Bug #10061 fixed - datatipCreate drew the tip in the current axes instead of curve
                     parent axes.

* Bug #10073 fixed - Use the new Scilab icon for GNU/Linux installations.
                    Icons renamed from XPM to PNG.

* Bug #10105 fixed - Focus was not given to the prompt when outputview had it.

* Bug #10119 fixed - assert_checkequal did not support sparse complex doubles.

* Bug #10124 fixed - strsplit crashed with wrong input arguments.

* Bug #10139 fixed - 'atomsTest' function did not manage specific test names.

* Bug #10150 fixed - completion help page moved to console chapter.

* Bug #10151 fixed - Wrong error message about polynomials was displayed.

* Bug #10178 fixed - norm function returned the message "division by zero" for
                     some sparse matrices.

* Bug #10179 fixed - Weird behavior with evstr.

* Bug #10184 fixed - In messagebox, focus was not set by default on "yes" or "ok"
                     button.

* Bug #10236 fixed - exit(-1) prompted for exiting Scilab.

* Bug #10241 fixed - A .scg file could not be opened when a non-ascii character was in the legend.

* Bug #10244 fixed - The install of call_scilab could fail in some cases under
                     GNU/Linux.

* Bug #10250 fixed - Bug in the detection of the Java path under GNU/Linux fixed.

* Bug #10267 fixed - The help browser did not use the default language at new session start-up.

* Bug #10272 fixed - Examples added in squeeze help page.

* Bug #10281 fixed - lyap function could crash.

* Bug #10282 fixed - repmat did not work for scalar input on dims greater than 2
                    neither for N-D arrays.

* Bug #10283 fixed - w and iw output arguments of ode function
                    are now directly read from work table used in sub-functions.

* Bug #10289 fixed - xload returns a more explicit error message when a file does
                    not exist.

* Bug #10292 fixed - Improved description of modulo help page.

* Bug #10293 fixed - Improved description of bvode help page.

* Bug #10303 fixed - test_run ignored "enable_lt" option.

* Bug #10304 fixed - test_run did not return an error if an invalid option was used.

* Bug #10306 fixed - 'isempty' could fail with empty elements in a list.

* Bug #10324 fixed - format(m) where m was a vector did not work but the
                    documentation stated it could be used.

* Bug #10330 fixed - 'error' did not manage vector of strings as input argument, but
                     'lasterror' returned vector of strings.

* Bug #10341 fixed - Font in help was bold when the font of a text uicontrol
                     was set to bold.

* Bug #10342 fixed - assert_checkerror help page had a wrong format.

* Bug #10345 fixed - uicontrols were not always drawn at the right place.

* Bug #10346 fixed - An uicontrol was visible during a few milliseconds even if
                     its "Visible" property was set to "off".

* Bug #10366 fixed - A warning about fun2string redefinition was displayed
                    when we used "Edit label generator" and click "Ok".

* Bug #10372 fixed - Dependencies of MALLOC.h removed on Windows.h and machine.h
                     under Windows.

* Bug #10379 fixed - lines did not manage a 1x2 vector [nc, nl] as input argument.

* Bug #10384 fixed - In exiting, "Close Console" was suggested rather than "Close
                     Scilab".

* Bug #10385 fixed - It was not possible to configure default web browser.

* Bug #10396 fixed - Display of a structure with large matrix took too much time.

* Bug #10402 fixed - FileBrowser threw an exception when a filter was set
                     and the tree was expanded.

* Bug #10408 fixed - In the history browser, the contents were scrolled even if the
                     knob was not at the bottom.

* Bug #10416 fixed - xmltoformat did not return at the root directory in case of error.

* Bug #10418 fixed - Playsound was failing under Mac OS X. Thanks to
                     Pierre-Henri Jondot for the patch.

* Bug #10423 fixed - browsehistory() function to open the history browser was missing.

* Bug #10431 fixed - Navigation in history using CTRL+P or CTRL+N was not available.

* Bug #10439 fixed - stacksize('max') crashed under Windows 64-bit.

* Bug #10440 fixed - 'wfir_gui' generated errors if the view checkbox was selected
                     and an erronous value was entered.

* Bug #10444 fixed - fullfile did not manage string matrix as last input argument.

* Bug #10449 fixed - At new session start-up, empty tabs could appear if the previous
                     session has not been properly exited.

* Bug #10450 fixed - Overloading for gamma, dlgamma and gammaln functions for list,
                     tlist, mlist and hypermatrices types was not allowed.

* Bug #10457 fixed - Reference in fftw help page pointed to a wrong version.

* Bug #10458 fixed - demo_gui did not clear its variables when closing the figure.

* Bug #10465 fixed - Typo fixed in isreal help page.

* Bug #10466 fixed - mgetl detected an error but it did not return an error
                     message in Scilab.

* Bug #10471 fixed - %helps_modules was not ignored by default by browsevar.

* Bug #10473 fixed - Error fixed in factorial function help page bibliography.

* Bug #10474 fixed - An exception could be thrown at the history browser
                     restoring.

* Bug #10475 fixed - Error fixed in perms function help page.

* Bug #10477 fixed - In ilib_for_link, the display of the error message generated
                     at configure time made mprintf fail.

* Bug #10479 fixed - max and min functions were incorrect for N-D arrays.

* Bug #10482 fixed - Wrong error message in format function fixed.

* Bug #10497 fixed - Some special characters were causing issues in phaseplot
                     localization.

* Bug #10499 fixed - The example in lincos help page returned a warning and an error.

* Bug #10503 fixed - error(999.666) did trunc error code without an error.

* Bug #10506 fixed - ssprint did not check input arguments.

* Bug #10518 fixed - odeoptions help page was inaccurate.

* Bug #10519 fixed - spec returned a wrong error message when an input argument
                     was not square.

* Bug #10520 fixed - The fact that ceil, floor, round or fix could manage complex
                     matrices was not clearly stated in the documentation.

* Bug #10521 fixed - frexp returned a wrong error message.

* Bug #10523 fixed - The fact that flipdim could manage complex matrices was
                     not clearly stated in the documentation.

* Bug #10526 fixed - A string was in French in the English help page of cumprod.

* Bug #10532 fixed - In some cases, browsevar cloud crash with some unknown
                     Scilab types.

* Bug #10536 fixed - A string was in French in the English help page of cumsum.

* Bug #10538 fixed - Typos fixed in prod and cumprod help pages.

* Bug #10543 fixed - %eps, %pi, %i, %nan, %inf were not documented.

* Bug #10544 fixed - debug did not return the previous value when called with
                     an argument.

* Bug #10551 fixed - Issues fixed in the Russian localization.

* Bug #10558 fixed - Tcl/Tk demonstrations did not start.

* Bug #10559 fixed - Spectral estimation demonstration failed if the graphic
                     figure was closed.

* Bug #10560 fixed - genetic algorithms demonstrations failed.

* Bug #10564 fixed - atan graphic demonstration was failing due to a too restrictive test
                     in the linspace function.

* Bug #10565 fixed - Simulation / bicycle simulation / unstable trajectory
                     demonstration failed.

* Bug #10574 fixed - Runge-Kutta-Method failed for vector [x, 1] with x > 9.

* Bug #10576 fixed - getdate (under Windows) did not manage dates after January 19, 2038

* Bug #10577 fixed - getdate did not check input arguments.

* Bug #10579 fixed - Wrong reference to and function in prod and cumprod help pages
                     fixed.

* Bug #10588 fixed - Update of waitbar help page to precise that the function returns
                     a handle.

* Bug #10595 fixed - In SciNotes, <TAB> did not go to the next tab position.

* Bug #10626 fixed - taucs_chdel(), umf_ludel() returned an error.

* Bug #10628 fixed - Reference to sci_types enum changed and "see also" section added.

* Bug #10631 fixed - The # sign was missing in some error cases.

* Bug #10632 fixed - Navigation buttons in the file browser did not change the
                     current Scilab working directory.

* Bug #10634 fixed - dsearch help page referenced an invalid input argument
                     name.

* Bug #10641 fixed - string(:) gave wrong results.

* Bug #10643 fixed - Bad result when using isreal function with a polynomial
                     input.

* Bug #10652 fixed - getdebuginfo() did not display TMPDIR value.

* Bug #10654 fixed - dgettext with "scilab" domain did not return the same result
                     as gettext.

* Bug #10656 fixed - Exception occurred when an empty help browser was closed.

* Bug #10658 fixed - Typos fixed in the bloc2exp help page.

* Bug #10662 fixed - xload did not support tlist for user_data property.

* Bug #10673 fixed - Typos fixed in ssrand help page.

* Bug #10677 fixed - %to_6=%to_e, crashed Scilab under Windows 64-bit version.

* Bug #10685 fixed - inttrap help page moved to differential equations module.

* Bug #10710 fixed - Wrong error message fixed in XML module.

* Bug #10711 fixed - The textbox of datatip was not displayed in graphics using
                     logarithm scale.

* Bug #10716 fixed - acos help page badly talked about vectors instead of matrices.

* Bug #10717 fixed - Typos fixed in acosd help page.

* Bug #10720 fixed - Typos fixed in acosh help page.

* Bug #10724 fixed - Missing links added in acot help page.

* Bug #10725 fixed - Typos fixed in acot help page.

* Bug #10726 fixed - Missing links added in acsc help page.

* Bug #10727 fixed - Missing links added in asech help page.

* Bug #10728 fixed - Wrong link fixed in asind help page.

* Bug #10729 fixed - Missing links added in asinh help page.

* Bug #10730 fixed - Missing links added in asinhm help page.

* Bug #10731 fixed - Typos fixed in atan help page.

* Bug #10732 fixed - Wrong link fixed in atand help page.

* Bug #10733 fixed - Missing link added in atanm help page.

* Bug #10734 fixed - Missing link added in cos help page.

* Bug #10735 fixed - Missing information added about input argument in cotd help page.

* Bug #10736 fixed - Missing "Arguments" section added in coth help page.

* Bug #10737 fixed - Missing "Arguments" section added in cothm help page.

* Bug #10738 fixed - Typos fixed in cscd help page.

* Bug #10739 fixed - Typos fixed in csgn help page.

* Bug #10740 fixed - Typos fixed in sec help page.

* Bug #10741 fixed - Typos fixed in secd help page.

* Bug #10742 fixed - Missing link added in sin help page.

* Bug #10743 fixed - Wrong link fixed in sind help page.

* Bug #10744 fixed - Typos fixed in sinh help page.

* Bug #10745 fixed - Typos fixed in tand help page.

* Bug #10769 fixed - Dead code removed from bsplin3val.

* Bug #10775 fixed - Scilab icons in FileBrowser had a wrong size.

* Bug #10782 fixed - 'iconvert' did not check input arguments.

* Bug #10797 fixed - contour, contour2d, contourf did not check input arguments.

* Bug #10807 fixed - It was not possible to overload fft and fftw for list,
                     tlist, mlist type.

* Bug #10809, #10810 fixed - error_table help page was not up-to-date.

* Bug #10820 fixed - datatipscurve generator was freezing Scilab.

* Bugs #10827, #10868 fixed - In SciNotes, strings containing LaTeX disappeared
                              when executed.

* Bug #10828 fixed - Developer arguments were not displayed in the help of
                     Scilab scripts.

* Bug #10831 fixed - set("auto_clear", "on") and set("auto_clear", "off") returned an error.

* Bug #10835 fixed - Typo that made the 'atanm' function unusable fixed.

* Bug #10836 fixed - resize_matrix with boolean failed.

* Bug #10837 fixed - Font modification in SciNotes preferences did not work.

* Bug #10839 fixed - dec2hex function was limited to 32-bits.

* Bug #10840 fixed - Keyboard arrows were disabled on 'Slider' uicontrols.

* Bug #10841 fixed - sliderStep property could not be set with "big" default value.

* Bug #10845 fixed - $ was not documented.

* Bug #10847 fixed - dec2base returned a wrong error message with a complex
                     number as input argument.

* Bug #10861 fixed - debug([]) returned a wrong error message.

* Bug #10867 fixed - Loading a .scg file failed when the legend contained accentuated characters.

* Bug #10879 fixed - Typo fixed in dec2base help page.

* Bug #10880 fixed - Default start-up directory moved to "My Documents folder"
                     under Windows for icon shortcut.

* Bug #10881 fixed - Typo fixed in ftest and ftuneq help pages.

* Bug #10885 fixed - Renamed "Exit" to "Exit SciNotes" in SciNotes.

* Bug #10904 fixed - Error messages of and() and or() builtin were not standard.

* Bug #10913 fixed - The "View code" button was missing in GUI demonstrations.

* Bug #10915 fixed - It was not possible to select several files when using
                     button "Open file" in console toolbar.

* Bug #10921 fixed - For ill-conditioned matrix, inv function displayed a
                     wrong value of rcond.

* Bug #10928 fixed - Examples of save updated with information to fix trouble under Windows.

* Bug #10966 fixed - linspace was producing some numerical errors on the last
                     element.

* Bug #10982 fixed - Completion failed when a space was used before the command.

* Bug #11010 fixed - Examples of preconditioning added in gmres help page.

* Bug #11029 fixed - Execution path to TMPDIR fixed in the example given in dae help page.

* Bug #11046 fixed - Sometimes, OverLoad(0) did not called %_xxx but %x_xxx.

* Bug #11048 fixed - In SciNotes, comment/uncomment actions did not work
                     properly.

* Bug #11049 fixed - Link to intg help page added in ode help page.

* Bug #11050 fixed - The output argument rd in ode and ode_root help pages was not
                     documented.

* Bug #11055 fixed - 'string' function did not manage implicit size type as
                     input argument.

* Bug #11069 fixed - An axe could be wrongly set as parent for an uicontrol.

* Bug #11070 fixed - The "Visible"/"Enable"/"Position" properties of a frame
                     uicontrol did not impact its children.

* Bug #11076 fixed -  mlist('r'), tlist('r') returned a wrong error message.

* Bug #11077 fixed - rlist() returned an error.

* Bug #11081 fixed -  movefile, copyfile did not support '~', 'TMPDIR',
                      shortcuts in filenames.

* Bug #11090 fixed - Typo fixed in event_handler_function help page.

* Bug #11093 fixed - isNamedVarExist() did not search in builtin functions names.

* Bug #11105 fixed - Crash with color_flag 3 fixed.

* Bug #11114 fixed - Using load(file_descriptor, ...) is now obsolete.

* Bug #11115 fixed - A graphic window was opened with two xend().

* Bug #11126 fixed - Error messages improved.

* Bug #11129 fixed - lapackscilab.sce could not be run outside its location.

* Bug #11138 fixed - a = list() crashed under Windows 64-bit.

* Bug #11154 fixed - Grid color hid the plots in Graphics/2D & 3D plots/plot2d demonstrations.

* Bug #11166 fixed - datatipRemove example did not work.

* Bug #11167 fixed - datatipRemoveAll example did not work.

* Bug #11175 fixed - xstringl example did not work.

* Bug #11186 fixed - 'typeof' of a type 130 returned an error.

* Bug #11194 fixed - A browsevar exception could occur at start-up.

* Bug #11203 fixed - Description for info output argument was wrong in lsqrsolve help page.

* Bug #11205 fixed - A rare exception at Scilab start-up could occur.

* Bug #11222 fixed - Links added between xdel, close and delete help pages.

* Bug #11223 fixed - Deletion of datatips.

* Bug #11244 fixed - Due to the variable browser, an error was displayed in the console when
                     deleting a global variable.

* Bug #11257 fixed - Memory leaks fixed in Scilab graphics.

* Bug #11265 fixed - msprintf crashed on a huge string.

* Bug #11281 fixed - References to x_left & y_top were false in reversed axes
                     cases in rubberbox help page.

* Bug #11289 fixed - swap_handles function modified the properties.

* Bug #11293 fixed - Line style was not taken into account.

* Bug #11298 fixed - Listboxes callback was executed several times for one action.

* Bug #11300 fixed - freson calculated wrong frequencies in Scilab 5.3.3.

* Bug #11318 fixed - Order of uimenus in a figure changed when adding submenus.

* Bug #11324 fixed - Restoring old files (< 5.4.0 format) could fail due to new
                     mechanism in SOD file loading.

* Bug #11326 fixed - set(h, "enable", "off") and set(h, "visible", "off") had no
                     effect after menu creation.

* Bug #11328 fixed - strsplit returned a wrong error message when the second input
                     argument contained wrong values.

* Bug #11330 fixed - oldplot function removed.

* Bug #11331 fixed - graypolarplot returned an error about an unknown property.

* Bug #11332 fixed - Default usage of contour2d was producing some errors.

* Bug #11333 fixed - Missing argument number checking in comet added.

* Bug #11343 fixed - The "isoview" figure property did not work when the axes
                     margins had been modified.

* Bug #11346 fixed - Error messages added in cosm function in case of bad
                     usage.

* Bug #11349 fixed - There was an error when the path to external editor contained
                     white spaces.

* Bug #11360 fixed - In some help pages, f in calling sequence was named mtx in
                     the parameter section.

* Bug #11369 fixed - colordef("black") returned an error about an unknown property.

* Bug #11372 fixed - Calling odeoptions returned an error.

* Bug #11385 fixed - length returned a wrong error message when called without
                     input argument.

* Bug #11388 fixed - Calling xsetech(frect=[0,0,100,100]) failed.

* Bug #11390 fixed - When cond function had one input argument, matrices could be
                     non square. Moreover, cond function has been updated to
                     give the possibility to use another norm than the one imposed
                     (by default 2-norm).

* Bug #11396 fixed - Overloading: toto([])=[] with toto being an mlist failed.

* Bug #11398 fixed - newest produced an incomprehensible error message when
                     called with a string vector as second input argument.

* Bug #11403 fixed - toprint and Print menu did not work.

* Bug #11409 fixed - xpoly crashed on polyline allocation.

* Bug #11411 fixed - save function used unsigned char to store string length.
                     Now it is an integer.

* Bug #11420 fixed - When using the event_handler of the default figure, events were not trapped.

* Bug #11422 fixed - datatips did not work when the figure contained uicontrols.

* Bug #11423 fixed - When the variable "a" was selected, all the a characters were
                     highlighted rather than only the "a" variable.

* Bug #11426 fixed - save function shew a warning message while saving user environment.

* Bug #11434 fixed - After having set the "auto_resize" default figure property
                     to "off", plot was no longer possible.

* Bug #11443 fixed - There was no way to set default language by the preferences under
                     Windows.

* Bug #11449 fixed - color function created a figure when no current figure
                     existed.

* Bug #11450 fixed - Logarithmic ticks were not sorted.

* Bug #11452 fixed - The use of diag optional argument in lsqrsolve produced an error.

* Bug #11457 fixed - gsort crashed when getting string data from stack after big stacksize.

* Bug #11459 fixed - varname, command, constant and option docbook tags were not
                     handled.

* Bug #11460 fixed - In the console, scrollbar was not updated if nothing was
                     entered at the prompt.

* Bug #11466 fixed - fscanf was not set as obsolete as it should have been.

* Bug #11469 fixed - The drawing, using xfpolys, was incremental instead of drawing
                     at once at the end.

* Bug #11472 fixed - Old menus were not removed before adding the new ones in default figure.

* Bug #11481 fixed - The title of axes was in front of plots.

* Bug #11482 fixed - The two plots drawn in Graphics/Complex elementary function/Custom
                     demonstration were too small.

* Bug #11485 fixed - delete function deleted axes identifier before using it.

* Bug #11486 & #11487 fixed - When a list was inserted into itself,
                              it produced Segfault or infinite loop.

* Bug #11488 fixed - Java exception led to a segfault in -ns mode when something
                     was typed in the console.

* Bug #11489 fixed - 'SliderStep' property was ignored for uicontrols.

* Bug #11490 fixed - comet produced wrong display with matrix inputs.

* Bug #11502 fixed - Some Java debug information was displayed when playing with
                     graphics.

* Bug #11506 fixed - "Rendu d'子uations math士atiques dans le graphique Scilab"
                     were missing some closure tags in the examples.

* Bug #11509 fixed - A progression bar was missing during ATOMS operations.

* Bug #11515 fixed - checkVarDimension help page added and getNbInputArgument and
                     getNbOutputArgument help pages updated.

* Bug #11517 fixed - Bad formatting fixed in the French version of getenv help page.

* Bug #11519 fixed - The colored lines in the legend of nyquist plot were inverted.

* Bug #11529 fixed - Only 512 chars could be entered in the console, now it is
                     set to 4096.

* Bug #11532 fixed - Update of mkdir English and French help pages.

* Bug #11533, #11652 and #11653 fixed - typos in messages of eigs function fixed,
                     eigs help page updated and optional booleans in eigs are boolean by
                     default to be consistent with Scilab's own data types.

* Bug #11534 fixed - getshortpathname and getlongpathname documentation updated to be clearer
                     about what happens on other OS than Windows.

* Bug #11535 fixed - listfiles used on multiple folders could return unusable results.

* Bug #11537 fixed - In SciNotes, it was not possible to modify font size with
                     CTRL+scroll.

* Bug #11538 fixed - xxxWideString functions (API Scilab) did not check to_wide_string
                     conversion (crashed under GNU/Linux & Mac OS X).

* Bug #11544 fixed - ilib_gen_cleaner did not add command to remove "Debug" and
                     "Release" folder under Windows.

* Bug #11547 fixed - _() moved into gettext() help page.

* Bug #11553 fixed - The uicontextmenu handles (and their children) were not
                     deleted at the end of execution.

* Bug #11554 fixed - After using xcosPalAdd, a message was displayed on 'quit'.

* Bug #11558 fixed - Typo in helptools module fixed.

* Bug #11563 fixed - Button was called "Open" (instead of "Save") in the GUI
                     used to select a file name for graphic export.

* Bug #11568 fixed - In SciNotes, only N-1 whites were used for a tab when N
                     was set.

* Bug #11578 fixed - dir function under Windows took into account the
                     case to sort the results.

* Bug #11582 fixed - Typo fixed in strchr help page.

* Bug #11592 fixed - The example in bessel help page failed.

* Bug #11597 fixed - Under Mac OS X, the Preferences window was not available
                     through the standard shortcut "CMD + ,".

* Bug #11604 fixed - Some items were not localized in "Scilab Preferences" window.

* Bug #11606 fixed - exit() was too slow in nw mode when driver("png") was set.

* Bug #11612 fixed - Index of the max value of a negative sparse was wrong.

* Bug #11618 fixed - A(:) returned wrong values when A was a complex sparse matrix.

* Bug #11619 fixed - titlepage("Text"): the font size was no longer maximized to fit the figure's sizes.

* Bug #11628 fixed - Clearing a shortcut did not work in Preferences.

* Bug #11640 fixed - Same behavior as in Scilab 5.3.3 plugged back at STDIN closure.

* Bug #11641 fixed - In xmltoformat, parsing of the CHAPTER files was too strict.

* Bug #11645 fixed - Short description was wrong in atomsGetLoadedPath help page.

* Bug #11647 fixed - The phrase "The system cannot find the file specified." in
                     rmdir was not localized.

* Bug #11654 fixed - Rendering of the title of genetic algorithms help pages improved.

* Bug #11655 fixed - external_object help chapter was empty.

* Bug #11657 fixed - Setting the callback_type property of an uicontrol was not
                     taken into account.

* Bug #11659 fixed - twinkle displayed warning messages about show_pixmap function.

* Bugs #11666, #11667 and #11670 to 11676 fixed - Add examples in signal processing help pages.

* Bug #11683 fixed - figure_properties help page did not explain what happens
                     when the event_handler and closerequestfcn properties were both set.

* Bug #11684 fixed - mfft failed due to MKL fftw library restrictions.

* Bug #11685 fixed - gcbo variable was not defined when executing functions set
                     for "closerequestfcn" and "resizefcn" properties.

* Bug #11686 fixed - An error occurred when clicking outside of an uicontextmenu area.

* Bug #11688 fixed - The example in example_run help page failed.

* Bug #11697 fixed - ATOMS modules are now listed in the "Toolboxes" menu when
                     they are not automatically loaded.

* Bug #11701 fixed - Under Windows, a carriage return was added in SciNotes each time the file
                     is saved.

* Bug #11703 fixed - Exception occurred when text was selected in SciNotes color preferences.

* Bug #11738 fixed - Rendering was of poor quality and very slow when usign xsave/xload.

* Bug #11745 fixed - Scilab export functions did not export in EPS but in PS.

* Bug #11760 fixed - Exceptions were thrown when trying to display invalid
                     LaTeX or MathML.

* Bug #11761 fixed - There was an exception in rendering MathML.

* Bug #11763 fixed - In SciNotes, number of recently opened files set in
                     preferences was not taken into account.

* Bug #11764 fixed - ispc (Matlab to Scilab Conversion Tips) help page was not updated regarding the
                     MSDOS removal.

* Bug #11774 fixed - Wrong description in now help page.

* Bug #11787 fixed - There was a problem with toolbar refreshing under Mac OS.

* Bug #11794 fixed - Empty lists could not be saved/loaded to/from SOD files.

* Bug #11802 fixed - mrfit function redefined the diff function.

* Bug #11810 fixed - In EditVar, undesired data were added when the edition
                     was cancelled.

* Bug #11810 fixed - In BrowseVar, contents of "visibility" column were left
                     aligned.

* Bug #11810 fixed - In Editvar, under Mac OS, there was no vertical/horizontal
                     lines.

* Bug #11834 fixed - New menu to set font or colors in SciNotes.

* Bug #11838 fixed - Maximized state of a window was not saved in quitting.

* Bug #11840 fixed - In Xcos, dialogs could not be closed on escape.

* Bug #11842 fixed - CTRL+X did not work in Scilab console.

* Bug #11861 fixed - An exception could occur when closing a figure.

* Bug #11865 fixed - There was no information about a LaTeX error in doc
                     generation.



            Changes between version 5.4.0-beta-3 and 5.4.0 of Scilab
            ========================================================


CSV features
============

* New functions added:
  - csvRead: Read a CSV file.
  - csvWrite: Write data into a CSV file.
  - csvTextscan: Transform a CSV line into a matrix.
  - csvDefault: Configure the CSV behavior.

* csvRead and cvsWrite provide the same features as read_csv and write_csv
  with major improvements in term of usability and performances:
  - Writing performances are improved to a 100x factor.
  - Reading performances are improved from 3x to 200x factor.

* Bug #8349 fixed - write_csv() failed to support the writing of complexes with
                    %nan or %inf parts.

* Bug #8653 fixed - csvRead now returns double values by default.
                    (read_csv still returns string values)

* Bug #8654 fixed - write_csv did not use the standard separator and decimal mark.

* Bug #8656 fixed - write_csv had accuracy issues.

* Bug #9969 fixed - Issues of performances of CSV functions for large files.

* Bug #10203 fixed - The help pages of CSV function did not detail the encoding
                     management.


Signal Processing
=================

* New functions:
  - dct - Discrete cosine transform (based on FFTW library).
  - idct - Inverse discrete cosine transform (based on FFTW library).
  - dst - Discrete sine transform (based on FFTW library).
  - idst - Inverse discrete sine transform (based on FFTW library).


Windows version
===============

* Scilab can now be installed without administration rights.
  In this case, the file association will not be configured.

* ATOMS module is now mandatory and is installed in the minimal installation.


Xcos
====

* Bug #9075 fixed - The inverted pendulum demonstration in Xcos/Control systems
                    opened a useless graphic windows

* Bug #10433 fixed - Xcos stop simulation button did not work properly.

* Bug #10943 fixed - Accented character in the file path broke file loading
                     in Xcos.

* Bug #11717 fixed - Setting recent opened file number in the preferences GUI
                     was not applied in Xcos menu.

* Bug #11747 fixed - Modelica block text was visible under the block image.


Compilation
===========

* Update to Scirenderer 1.0.2.

* Update to JLaTeXMath 1.0.0.


Bug Fixes
=========

* Bug #7874 fixed - qp_solve help page did not give enough information
                    about iact.

* Bug #8799 fixed - gsort did not work with hypermatrices.

* Bug #10346 fixed - An uicontrol was visible during a few milliseconds even if
                     its "Visible" property was set to "off".

* Bug #10559 fixed - Spectral estimation demonstration failed if the graphic
                     figure was closed.

* Bug #10840 fixed - Keyboard arrows were disabled on 'Slider' uicontrols.

* Bug #10885 fixed - Renamed "Exit" to "Exit Scinotes" in Scinotes.

* Bug #11257 fixed - Memory leaks fixed in Scilab graphics.

* Bug #11293 fixed - Line style was not taken into account.

* Bug #11298 fixed - Listboxes callback was executed several times for one action.

* Bug #11489 fixed - 'SliderStep' property was ignored for uicontrols.

* Bug #11558 fixed - Typo in helptools module fixed.

* Bug #11657 fixed - Setting the callback_type property of an uicontrol was not
                     taken into account.

* Bug #11684 fixed - mfft failed due to MKL fftw library restrictions.

* Bug #11688 fixed - The example in example_run help page failed.

* Bug #11794 fixed - Empty lists could not be saved/loaded to/from SOD files.

* Bug #11802 fixed - mrfit function redefined the diff function.

* Bug #11810 fixed - In EditVar, undesired data were added when the edition
                     was cancelled.

* Bug #11810 fixed - In BrowseVar, contents of "visibility" column were left
                     aligned.

* Bug #11810 fixed - In Editvar, under Mac OS, there was no vertical/horizontal
                     lines.

* Bug #11834 fixed - New menu to set font or colors in SciNotes.

* Bug #11838 fixed - Maximized state of a window was not saved in quitting.

* Bug #11840 fixed - In Xcos, dialogs could not be closed on escape.

* Bug #11842 fixed - CTRL+X did not work in Scilab console.

* Bug #11861 fixed - An exception could occur when closing a figure.

* Bug #11865 fixed - There was no information about a LaTeX error in doc
                     generation.


            Changes between version 5.4.0-beta-2 and 5.4.0-beta-3 of Scilab
            ===============================================================

Xcos
====

* Bug #10041 fixed - WFILE_f block was not declared as deprecated.

* Bug #10781 fixed - DFlipflop incorrectly reported problem for port size or type.

* Bug #11735 fixed - Output sink name was not reported as figure name.

* Bug #11737 fixed - Incompatibility between a Xcos menu and its help page description.

* Bug #11746 fixed - Closing the palette window with the crossing box
                     did not update the checkbox in Xcos "View" menu.


Obsolete Functions
==================

* config function is now obsolete and will be removed in Scilab 5.4.1.
  Please use 'preferences' instead.

* dnaupd, dneupd, dsaupd, dseupd, znaupd and zneupd deprecated for eigs.


Documentation
=============

* Categories have been created for the following modules help pages:
    - Signal processing. See bug #11661.
    - Functions (libraries sub-category). See bug #11642.
    - Linear algebra. See bug #11562.
    - Special functions. See bug #11643.
    - Functions. See bug #11722.
    - Input/Output functions. See bug #11726.

* Elementary functions module help pages have been better organized.
  See bug #11723.

* Automatic spelling corrections for Scilab Online help - http://help.scilab.org/


Compilation
===========

* Scilab dependency on Java HDF has been removed.

* Update to Scirenderer 0.5.7.

* Update to JOGL 2.0 rc9


Linear Algebra
==============

* cond function updated to take into account a second input argument (See SEP #85)


Bug Fixes
=========

* Bug #4337 fixed - who_user did not return user's variable names but
                    only displayed them.

* Bug #7674 fixed - In all Optimization and Simulation demonstrations, the source
                    code was displayed by default.
                    A messagebox now lets the choice to the user to display the
                    code or not.

* Bug #8151 fixed - Under Mac OS X, "Scilab/Quit Scilab" and "File/Quit" items were
                    duplicates.

* Bug #8488 fixed - NaN values were propagated when min/max were performed between
                    several matrices.

* Bug #9032 fixed - The rectangular contour of a 'Text' object did not fit the text
                    for huge font size.

* Bug #9204 fixed - The Frobenius norm of a complex vector was numerically complex.

* Bug #9616 fixed - In qp_solve help page, the input arguments in calling
                    sequence did not match those in arguments section.

* Bug #9693 fixed - The default value of epsg in optim help page was %eps.

* Bug #9976 fixed - cdfbet function returned wrong value instead of 0.

* Bug #10178 fixed - norm function returned the message "division by zero" for
                     some sparse matrices.

* Bug #10677 fixed - %to_6=%to_e, crashed Scilab under Windows x64 version.

* Bug #11390 fixed - When cond function had one input argument, matrices could be
                     non square. Moreover, cond function has been updated to
                     give the possibility to use another norm than the one imposed
                     (by default 2-norm).

* Bug #11411 fixed - save function used unsigned char to store string length.
                     Now it is an integer.

* Bug #11426 fixed - save function shew a warning message while saving user environment.

* Bug #11509 fixed - A progression bar was missing during Atoms operations.

* Bug #11533, #11652 and #11653 fixed - typos in messages of eigs function fixed,
                     eigs help page updated and optional booleans in eigs are boolean by
                     default to be consistent with Scilab's own data types.

* Bug #11554 fixed - After using xcosPalAdd, a message was displayed on 'quit'.

* Bug #11568 fixed - In SciNotes, only N-1 whites were used for a tab when N
                     was set.

* Bug #11592 fixed - In the documentation, example of bessel failed.

* Bug #11597 fixed - Under Mac OS X, the Preferences window was not available
                     through the standard shortcut "CMD + ,".

* Bug #11604 fixed - Some items were not localized in "Scilab Preferences" window.

* Bug #11606 fixed - exit() was too slow in nw mode when driver("png") was set.

* Bug #11612 fixed - Index of the max value of a negative sparse was wrong.

* Bug #11618 fixed - A(:) returned wrong values when A was a complex sparse matrix.

* Bug #11619 fixed - titlepage("Text"): the font size was no longer maximized to fit the figure's sizes.

* Bug #11628 fixed - Clearing a shortcut did not work is Preferences.

* Bug #11640 fixed - Same behavior as in Scilab 5.3.3 plugged back at STDIN closure.

* Bug #11641 fixed - In xmltoformat, parsing of the CHAPTER files was too strict.

* Bug #11645 fixed - Short description was wrong in atomsGetLoadedPath help page.

* Bug #11647 fixed - The phrase "The system cannot find the file specified." in
                     rmdir was not localized.

* Bug #11654 fixed - Rendering of the title of genetic algorithms help pages improved.

* Bug #11655 fixed - external_object help chapter was empty.

* Bug #11659 fixed - twinkle displayed warning messages about show_pixmap function.

* Bugs #11666, #11667 and #11670 to 11676 fixed - Add examples in signal processing help pages.

* Bug #11683 fixed - figure_properties help page did not explain what happens
                     when the event_handler and closerequestfcn properties were both set.

* Bug #11685 fixed - gcbo variable was not defined when executing functions set
                     for "closerequestfcn" and "resizefcn" properties.

* Bug #11686 fixed - An error occurred when clicking outside of an uicontextmenu area.

* Bug #11697 fixed - ATOMS modules are now listed in the "Toolboxes" menu when
                     they are not automatically loaded.

* Bug #11701 fixed - Under Windows a carriage return was added in SciNotes each time the file
                     is saved.

* Bug #11703 fixed - Exception occurred when text was selected in SciNotes color preferences.

* Bug #11738 fixed - Rendering was of poor quality and very slow when usign xsave/xload.

* Bug #11745 fixed - Scilab export functions did not export in EPS but in PS.

* Bug #11760 fixed - Exceptions were thrown when trying to display invalid
                     LaTeX or MathML.

* Bug #11761 fixed - There was an exception in rendering MathML.

* Bug #11763 fixed - In SciNotes, number of recently opened files set in
                     preferences was not taken into account.

* Bug #11764 fixed - ispc (Matlab to Scilab Conversion Tips) help page was not updated regarding the
                     MSDOS removal.

* Bug #11774 fixed - Wrong description in now help page.

* Bug #11787 fixed - There was a problem with toolbar refreshing under Mac OS.


            Changes between version 5.4.0-beta-1 and 5.4.0-beta-2 of Scilab
            ===============================================================


Xcos
====

* Help pages updated with examples and translated into French:
  - WFILE_f - Block to write to output file
  - WRITEC_f - Block to write C binary file

* Bug #8510 fixed - The message "Diagram contains implicit blocks..." stopped
                    all process.

* Bug #9290 fixed - HALT blocks displayed STOP.

* Bug #9496 fixed - Xcos flag 5 (ending) was not called when simulation was
                    stopped from the user interface.

* Bug #11132 fixed - Some help pages were missing: standard_origin, ...

* Bug #11494 fixed - Xcos PDE block could not open UIcontrol to set parameter.

* Bug #11518 fixed - When zooming on CLR block-text, the block was outside the box.

* Bug #11550 fixed - Closing the scope graph while running simulation led to a
                     segfault.


Preferences
===========

* preferences() function added to open the Preferences window.


Javasci
=======

* Javasci version 1 removed (deprecated in Scilab 5.3.0).


Documentation
=============

* Tag <scilab:image> added. This tag will perform the graphic rendering of
  a Scilab code and include the result into the help page as a sample image.

* With the <scilab:image> tag, all graphic and trigonometric help pages
  now contain a sample image.

* Management of <varname>, <command>, <function> and <constant> Docbook tags.
  See bug #11459.

* help_from_sci updated to manage recent changes in docbook Scilab files.

* Categories have been created for the following modules help pages:
    - Scilab. See bug #11546.
    - Statistics. See bug #9108.
    - Genetic algorithms. See bug #11514.
    - Simulated annealing. See bug #11560.


Signal Processing
=================

* fft function based on fftw updated and improved (See SEP#84):
    - Take data symmetry into account,
    - Include hard coded multivariate fft.


Compilation
===========

* Switch to flexdock 1.2.0.

* Switch to Scirenderer 0.5.5.

* BLAS_LIBS variable can now be set by the user (See bug #9824).


GUI
===

* Uicontrols tooltip can now be managed using the new "TooltipString" property.

* Figure closure can now be managed by the user (confirmation, ...) through the
  'CloseRequestFcn' property.

* F5 and F6 change the focus of the docked element.

* Improvement in the refresh of the docked elements.


Obsolete Functions
===================

* createpopup function is now obsolete. Please use 'uicontextmenu' instead.

* uipopup function is now obsolete. Please use 'uicontextmenu' instead.


Arnoldi Package
===============

* New function:
  - eigs function added (See SEP #82)


Unitary tests
=============

* A fourth argument added to test_run to export test results to XUnit format.

* A new function called example_run has been introduced, this function tests the
  examples given in Scilab help pages. See SEP #83.


Statistics
==========

* General improvements of the show_pca function (See bug #11500):
  - Data bounds fixed in the left graph,
  - Correlation circle graph improved,
  - Eigenvalues graph improved.


Bug Fixes
=========

* Bug #3235 fixed - In bezout function, wrong typo in the name of a variable
                    made the calpol unitary test failed.

* Bug #3244 fixed - Error messages in qpsolve and qp_solve functions updated.

* Bug #3787 fixed - Invalid calling sequences removed from schur help page.

* Bug #5675 fixed - Levitron demonstration was failing in some cases.

* Bug #6154, #10850 fixed - arnoldi package help pages updated.

* Bug #6538 fixed - unobs ignored its 'tol' argument when it was given,
                    and always calculated its own, arbitrary, internal value.

* Bug #6702 fixed - max and min functions returned a scalar in k when the input
                    argument was the last column of an hypermatrix.

* Bug #7216 fixed - modulo(x,0) and pmodulo(x,0) did not return an error message.
                    With ieee(2) these functions now return %nan.

* Bug #7251 fixed - Error fixed in qp_solve help page.

* Bug #7763 fixed - mtlb_max and mtlb_min functions did not manage complex
                    numbers with null imaginary part.

* Bug #7855 fixed - It was not possible to change event function while
                    the event handler was enabled.

* Bug #7873 fixed - The lagr parameter in qld help page was wrong.

* Bug #7881 fixed - Error in the condition which checks if the second input
                    argument in lattn function was scalar or not.

* Bug #8101 fixed - median function with a sparse as a first argument did not
                    return the good result. Moreover, median(sparse, 'r'| 'c') was not
                    managed.

* Bug #8351 fixed - integrate function did not manage properly the optional
                    input arguments.

* Bug #8399 fixed - setlanguage didn't work when only fr_FR.UTF-8 was available.

* Bug #8640 fixed - histplot function failed when the data vector contained
                    the same values.

* Bug #8785 fixed - modulo and pmodulo functions did not check input arguments.

* Bug #9464 fixed - contourf did not work if the "Matlab Compatibility functions
                    module" was not installed.

* Bug #9824 fixed - BLAS_LIBS variable was overloaded by the configure.

* Bug #9830 fixed - leastsq did not match the information given in optim help page.

* Bug #9884 fixed - Possibility to add a default header on new files in SciNotes
                    added in Preferences.

* Bug #10150 fixed - Completion help page moved to console chapter.

* Bug #10184 fixed - In messagebox, focus was not set by default on yes or ok
                     button.

* Bug #10385 fixed - It was not possible to configure default web browser.

* Bug #10479 fixed - max and min functions were incorrect for N-D arrays.

* Bug #10497 fixed - Some special characters were causing issues in phaseplot
                     localization.

* Bug #10595 fixed - In SciNotes, <TAB> did not go to the next tab position.

* Bug #10711 fixed - The textbox of datatip was not displayed in graphics using
                     logarithm scale.

* Bugs #10827, #10868 fixed - In SciNotes, strings containing LaTeX disappeared
                              when executed.

* Bug #10837 fixed - Font modification in SciNotes preferences did not work.

* Bug #10915 fixed - It was not possible to select several files when using
                     button "Open file" in console toolbar.

* Bug #10921 fixed - For ill-conditioned matrix, inv function displayed a
                     wrong value of rcond.

* Bug #11046 fixed - Sometimes, OverLoad(0) did not called %_xxx but %x_xxx.

* Bug #11048 fixed - In SciNotes, comment/uncomment actions did not work
                     properly.

* Bug #11049 fixed - Link to intg help page added in ode help page.

* Bug #11050 fixed - The output argument rd in ode and ode_root help pages was not
                     documented.

* Bug #11069 fixed - An axe could be wrongly set as parent for an uicontrol.

* Bug #11070 fixed - The "Visible"/"Enable"/"Position" properties of a frame
                     uicontrol did not impact its children.

* Bug #11203 fixed - Description for info output argument was wrong in lsqrsolve help page.

* Bug #11244 fixed - Due to the variable browser, an error was displayed in the console when
                     deleting a global variable.

* Bug #11300 fixed - freson calculated wrong frequencies in Scilab 5.3.3.

* Bug #11318 fixed - Order of uimenus in a figure changed when adding submenus.

* Bug #11324 fixed - Restoring old files (< 5.4.0 format) could fail due to new
                     mechanism in SOD file loading.

* Bug #11326 fixed - set(h, "enable", "off") and set(h, "visible", "off") had no
                     effect after menu creation.

* Bug #11328 fixed - strsplit returned a wrong error message when the second input
                     argument contained wrong values.

* Bug #11330 fixed - oldplot function removed.

* Bug #11331 fixed - graypolarplot returned an error about an unknown property.

* Bug #11332 fixed - Default usage of contour2d was producing some errors.

* Bug #11333 fixed - Missing argument number checking in comet added.

* Bug #11343 fixed - The "isoview" figure property did not work when the axes
                     margins had been modified.

* Bug #11346 fixed - Error messages added in cosm function in case of bad
                     usage.

* Bug #11349 fixed - There was an error when the path to external editor contained
                     white spaces.

* Bug #11360 fixed - In some help pages, f in calling sequence was named mtx in
                     the parameter section.

* Bug #11369 fixed - colordef("black") returned an error about an unknown property.

* Bug #11372 fixed - Calling odeoptions returned an error.

* Bug #11385 fixed - length returned a wrong error message when called without
                     input argument.

* Bug #11388 fixed - Calling xsetech(frect=[0,0,100,100]) failed.

* Bug #11396 fixed - Overloading: toto([])=[] with toto being an mlist failed.

* Bug #11398 fixed - newest produced an incomprehensible error message when
                     called with a string vector as second input argument.

* Bug #11403 fixed - toprint and Print menu did not work.

* Bug #11409 fixed - xpoly crashed on polyline allocation.

* Bug #11420 fixed - When using the event_handler of the default figure, events were not trapped.

* Bug #11422 fixed - datatips did not work when the figure contained uicontrols.

* Bug #11423 fixed - When the variable "a" was selected, all the a characters were
                     highlighted rather than only the "a" variable.

* Bug #11434 fixed - After having set the "auto_resize" default figure property
                     to "off", plot was no longer possible.

* Bug #11443 fixed - There was no way to set default language by the preferences under
                     Windows.

* Bug #11449 fixed - color function created a figure when no current figure
                     existed.

* Bug #11450 fixed - Logarithmic ticks were not sorted.

* Bug #11452 fixed - The use of diag optional argument in lsqrsolve produced an error.

* Bug #11457 fixed - gsort crashed when getting string data from stack after big stacksize.

* Bug #11459 fixed - varname, command, constant and option docbook tags were not
                     handled.

* Bug #11460 fixed - In the console, scrollbar was not updated if nothing was
                     entered at the prompt.

* Bug #11466 fixed - fscanf was not set as obsolete as it should have been.

* Bug #11469 fixed - The drawing, using xfpolys, was incremental instead of drawing
                     at once at the end.

* Bug #11472 fixed - Old menus were not removed before adding the new ones in default figure.

* Bug #11481 fixed - The title of axes was in front of plots.

* Bug #11482 fixed - The two plots drawn in "Graphics/Complex elementary function/Custom"
                     demonstration were too small.

* Bug #11485 fixed - delete function deleted axes identifier before using it.

* Bug #11486 & #11487 fixed - When a list was inserted into itself,
                              it produced Segfault or infinite loop.

* Bug #11488 fixed - Java exception led to a segfault in -ns mode when something
                     was typed in the console.

* Bug #11490 fixed - comet produced wrong display with matrix inputs.

* Bug #11502 fixed - Some Java debug information was displayed when playing with
                     graphics.

* Bug #11506 fixed - "Rendu d'équations mathématiques dans le graphique Scilab"
                     were missing some closure tags in the examples.

* Bug #11515 fixed - checkVarDimension help page added and getNbInputArgument and
                     getNbOutputArgument help pages updated.

* Bug #11517 fixed - Bad formatting fixed in the French version of getenv help page.

* Bug #11519 fixed - The colored lines in the legend of nyquist plot were inverted.

* Bug #11529 fixed - Only 512 chars could be entered in the console, now it is
                     set to 4096.

* Bug #11532 fixed - Update of mkdir English and French help pages.

* Bug #11534 fixed - getshortpathname and getlongpathname documentation updated to be clearer
                     about what happens on other OS than Windows.

* Bug #11535 fixed - listfiles used on multiple folders could return unusable results.

* Bug #11537 fixed - In SciNotes, it was not possible to modify font size with
                     CTRL+scroll.

* Bug #11538 fixed - xxxWideString functions (API Scilab) did not check to_wide_string
                     conversion (crashed under GNU/Linux & Mac OS X).

* Bug #11544 fixed - ilib_gen_cleaner did not add command to remove "Debug" and
                     "Release" folder under Windows.

* Bug #11547 fixed - _() moved into gettext() help page.

* Bug #11553 fixed - The uicontextmenu handles (and their children) were not
                     deleted at the end of execution.

* Bug #11563 fixed - Button was called "Open" (instead of "Save") in the GUI
                     used to select a file name for graphic export.

* Bug #11578 fixed - dir function under Windows took into account the
                     case to sort the results.

* Bug #11582 fixed - Typo fixed in strchr help page.


Known Incompatibilities
=======================

* In previous versions, modulo(x,0) and pmodulo(x,0) returned x. But this solution is
  not correct. Indeed, the expression used to compute the modulo is x - y * int(x / y)
  for the modulo function and x - y * floor(x / y). Now, they return an error and
  %nan with ieee(2). See bug #7216.

* In previous versions, newest function could be called with a string vector
  as second (up to last) input argument. In these cases, only the first string of
  the vector was taken into account. These calls now return an error. See bug #11398.

* Setting the "foreground" property for a figure was possible even if it was not
  officially supported or documented. An error is now returned.
  Please use the axes "foreground" property instead. See bug #11369.


            Changes between version 5.4.0-alpha-1 and 5.4.0-beta-1 of Scilab
            ================================================================


Graphics
========

* Entire refactoring of Scilab graphic capabilities in order to solve the known issues
  of graphical cards compatibilities and many Scilab crashes.

* Based on SciRenderer (rendering library based on JoGL), Scilab graphics have
  been rewritten to provide new graphic functionalities with:
  - better performances,
  - better stability,
  - 2-D zoom using the mouse wheel,
  - new 3-D zoom capabilities,
  - rotation available through the mouse right-click.
  SciRenderer is a free and open-source library developed by Scilab Enterprises
  for 2-D and 3-D rendering of scientific data.

* Improved graphic exports:
  - Off-screen export (Scilab will not open a window to export with driver).
  - New bitmap formats supported: PNG, JPEG and BMP in the driver function.
  - New vectorial formats supported: PDF and SVG in the driver function.
  - Many portability issues fixed.


* Bug #4289 fixed - Graphic exports were not working when the drawlater or the
                    pixmap modes were activated.

* Bug #4339 fixed - listvarinfile did not manage graphic handles.

* Bug #4818 fixed - Scilab could not export graphics which contained a lot of information.

* Bug #6582 fixed - A copy of a handle did not keep foreground property.

* Bug #6644 fixed - New graphic windows take care of default figure properties (uimenu).

* Bug #7998 fixed - %h_get failed on heterogeneous handle matrix.

* Bug #8241 fixed - Graphical properties were not always inherited.

* Bug #8250, #10567, #10614 fixed - Artefacts dropped on the
                                    vectorial (EPS/PDF/SVG) exports.

* Bug #8705 fixed - Some specific graphics could not be saved.

* Bug #8916 fixed - Menu to export in PS format was missing in graphic figures.

* Bug #9721 fixed - Documentation improved on tight_limits. Thanks to Samuel
                    Gougeon.

* Bug #10240 fixed - In a Scilab localized in French, SVG export could use "," instead
                     of "." for numerical separator.

* Bug #10290 fixed - Error message for bad assignement of interp_color_vector updated.

* Bug #10360 fixed - Default format for export is now PNG instead of BMP.

* Bug #10365 fixed - To copy a handle to a figure (instead of an axes) crashed Scilab.

* Bug #10395 fixed - xset("clipping", [1,2,3,4]) returned an error.

* Bug #10417 fixed - xget("background") returned a wrong value.

* Bug #10637 fixed - xs2svg crashed when first input argument was not a figure handle.
                     An error message is now displayed.


GUI
===

* A 'Console' handle has been created and can be obtained by calling get(0):
  - uimenus now appear as 'Children' of this handle when they are added to the
    console,
  - the 'ShowHiddenHandles' property enables the user to make some hidden
    graphic objects visible.

* Scilab Console and graphic figure menus are now uimenus. They are hidden by
  default. To see them in the children list, use set(get(0), "ShowHiddenHandles", "on")

* New uicontrols added:
  - 'image' style uicontrol: display, transformation and various other
    operations on images.
  - 'table' style uicontrol: display data in a table form (read-only in Scilab
    5.4.0-BETA-1 version).

* The 'Value' property can now contain non-integer values and allows to take full
  advantage of 'slider' style uicontrols (See bug #7380).

* Figure resizing can now be managed by the user (uicontrol position setting, ...)
  through the 'resizefcn' property.


Scilab
======

* save/load functions can export/import Scilab data to SOD format:
 - Scilab data are saved and loaded using a standard, HDF5-based, documented,
   and fully tested: Scilab Open Data format (SOD).
 - Graphic handles can be saved under the SOD format.
 - 'save' writes data to HDF5 format when using new calling sequence
   save("filename.sod", "a", "b", "c"). This format is Scilab 6 compatible.
 - 'save' displays a message when using old calling sequence
   save("filename", a, b, c) and writes data in old Scilab binary format.
   This format will not be supported by Scilab 6.
 - The 'matfile2sci' function now converts MAT-files to SOD files.

* Calling load with a file descriptor as first input argument is now obsolete.
  Please replace the file descriptor by the corresponding file name.
  This feature will be removed in Scilab 6.0.0

* Update the default size of the Scilab stack to 100 million elements (instead of
  50M). See stacksize help page for more information.

* Scilab Desktop provides a button in dockbar to get the help about the component.

* External modules are managed out of the box by test_run function. See bug #9834.
Example: test_run('SCI/contrib/toolbox_skeleton')


XML
===

* xmlClose renamed in xmlDelete and xmlGetOpenStreams in xmlGetOpenDocs.

* Bug #10185 fixed - Add xmlDelete("all") in xml.quit.

* Bug #10594 fixed - size could not be used with two arguments on XMLList.

* Bug #10630 fixed - Typo fixed in an error message.

* Bug #10670 fixed - Crash avoided when xmlRead had an argument with a bad size.

* Bug #11250 fixed - Avoid a crash when an error is met during xmlValidation.


Documentation
=============

* Management of Docbook tags <note>, <caution>, <warning>, <important> and
 <tip>. See bug #10505.

* Function help_skeleton extended to suggest the <revhistory>, <latex>, <note>,
  <caution>, <warning>, <important> and <tip> tags in examples.

* Scilab documentation is now available in Russian. Many thanks to Stanislav Kroter.

* Many errors have been fixed in Scilab documentation:
  - Wrong calling sequences
  - Errors in examples
  - Bad formatting
  - Typos


Dynamic Link
=============

* DEBUG_SCILAB_DYNAMIC_LINK was not available under GNU/Linux or Mac OS X.
  See bug #11142

Xcos
====

* Major performance improvements on the simulation start-up. On important diagrams,
  the gain is very important (from minutes to milliseconds). See bug #8188.

* Performance improvements on diagram saving/loading.

* A compressed file format is now available (ZCOS) and allows more flexibility on
  data format.

* Ports styles and labels are now available to external modules.

* New examples are provided in the help pages of the following blocks:
  - ABS_VALUE - Absolute value
  - EVTDLY_c - Event delay
  - AFFICH_m - Display value
  - CLSS - Continuous state-space system
  - SELECT_m - Selector block
  - NEGTOPOS_f - Threshold negative to positive

* External modules should be updated to use the new Scilab Open Data format
  instead of the previous HDF5 format.

* Bug #6201 fixed - Block help menu had no shortcut.

* Bug #6733 fixed - Each call to Xcos created an empty diagram.

* Bug #7539 fixed - Interaction with some blocks was slowed down.

* Bug #7863 fixed - Default path for code generation was SCI instead of pwd() or
                    TMPDIR.

* Bug #8188 fixed - Xcos to Scilab export took too much time.

* Bug #8295 fixed - Save compressed Xcos file set as default.

* Bug #8600 fixed - Closing Xcos did not end the simulation.

* Bug #8744 fixed - Solving the initialization problem more than once crashed
                    Scilab.

* Bug #9131 fixed - PULSE_SC has been added to the Source palette.

* Bug #10081 fixed - Palette configuration saving could produce an invalid
                     file.

* Bug #10107 fixed - Default value of the Integrator Absolute Tolerance was
                     too large. It has been reduced to 1e-6.

* Bug #10246 fixed - PROD_f block was broken.

* Bug #10333 fixed - Labeling I/O blocks on a super block produced wrong port
                     labeling when ports had not the same types.

* Bug #10340 fixed - Exception occurred while moving the palette split pane to
                     hide the block list and while using a palette diagram.

* Bug #10393 fixed - Block style was not updated on palette loading.

* Bug #10419 fixed - Exception occurred while refusing to create a new empty
                     file.

* Bug #10422 fixed - Exception occurred when exporting with an unspecified
                     format.

* Bug #10461 fixed - Canceling a Quit operation did not work.

* Bug #10486 fixed - importXcosDiagram opened a tab.

* Bug #10496 fixed - Wrong error message fixed in xcosValidateBlockSet.

* Bug #10510 fixed - Unused variables were generated by Xcos at start-up.

* Bug #10533 fixed - Some action shortcuts were not disabled when editing a text
                     block.

* Bug #10657 fixed - CONSTRAINT_c had no icons on a graph.

* Bug #10679 fixed - blocks_link.tst failed.

* Bug #10697 fixed - post_xcos_simulate function did not work with a TOWS_c block
                     in a diagram.

* Bug #10800 fixed - xcosPalAddBlock help example did not work.

* Bug #10812 fixed - cos and cosf files loading did not use a direct handler.

* Bug #10848 fixed - The first Drag'n Drop of a block produced an exception.

* Bug #11094 fixed - Image creation of blocks did not use xinit / xend.


Compilation
===========

* Scilab now uses the ARPACK-ng package maintained by Scilab Enterprises.

* An option --without-arpack-ng has been added to disable the Arnoldi features.

* HDF5 is now a mandatory dependency of Scilab (even in a minimal build)

* Bug #11060 fixed - ccache use can be disabled with --disable-ccache .


Elementary Functions
====================

* New functions:
  - dec2base function added (See SEP #75)
  - nthroot function added (See SEP #78)

* linspace and logspace deprecate the use of non-integer values as third input argument.

* The toeplitz macro is now vectorized.


Sparse
======

* qmr function updated and modified.
   Calling qmr(F, Fp, x) where F is a function returning A*x
   and Fp a function returning A'*x is deprecated.
   Use qmr(F, x) instead where F prototype is
   function y = F(x, t)
   [...]
   endfunction
   where t is used to make F return A*x or A'*x.
   Same behavior has been implemented for qmr( ..., M1, M1p, M2, M2p).
   See qmr help page for examples and details.


CLI Console (GNU/Linux and Mac OS X only)
=========================================

* Wide characters supported.

* Multiline command supported.

* Command line display improved.

* List of shortcuts in NW/NWNI mode:
  - CTRL+W removes the previous word.
  - CTRL+U removes everything from the current position to the beginning of
            the line.
  - UP or Ctrl+P recalls previous line.
  - DOWN or Ctrl+N recalls next line.
  - TAB for completion. Scilab displays a list of all names (functions, variables) that
    start with the characters already typed by the user.
  - Ctrl + A or HOME moves the cursor to beginning of current line.
  - Ctrl + B or LEFT moves the cursor one character to the left.
  - Ctrl + D or DELETE deletes the current character.
  - Ctrl + E or END moves the cursor to the end of command line.
  - Ctrl + F or RIGHT moves the cursor one character to the right.
  - Ctrl + H or BACKSPACE deletes the previous character.
  - Ctrl + K kills command line from cursor to the end.
  - Alt+B or Ctrl+LEFT moves left one word.
  - Alt+F or Ctrl+RIGHT moves right one word.

* Bug #3108 fixed - Some specific chars in the terminal were not correctly
                    removed by the backspace.

* Bug #3400 fixed - Long lines in the history broke the display.

* Bug #6804 fixed - Prompt was removed in cli mode when playing with arrow keys.


Scinotes
========

* Shortcuts META+(UP|DOWN) added to navigate between paragraphs.

* Bug #10442 fixed - Sometimes a crash occurred at start-up when Scinotes was
                     restoring.

* Bug #10647 fixed - An error message did not respect Scilab localization standard.

* Bug #10889 fixed - Exception about block highlighters was thrown by Scinotes.


Performances
============

* 'hank' function performances improved.

* Using api_scilab, construction of any type of lists is now much faster (300x in some cases).


Javasci
=======

* New datatypes are now supported:
  - sparse and boolean sparse
  - tlists, mlists and lists
  - polynomial

* If Scilab fails to start because of a memory issue (stacksize), an
  InitializationException Java exception is now displayed.

* Bug #10794 fixed - With Scilab built on an Ubuntu 11.10, javasci failed to
                     load with an undefined symbol


CACSD & Signal Processing
=========================

* New functions:
  - conv - discrete 1-D convolution.
  - conv2 - discrete 2-D convolution.
  - convol2d - discrete 2-D convolution, using fft.
  - phaseplot - frequency phase plot.
  - xcorr - auto and cross correlation of signals (superseeds corr).
  - xcov - auto and cross correlationvariance of signals (superseeds corr).
  - arma2ss - conversion of an arma data structure into a state space one.

* 'mtlb_conv' compatibility function is now obsolete. Please use 'conv' instead.

* Bug #10562 fixed - The "CACSD/Robust Control" demo failed.

* Bug #11340 fixed - bode, gainplot, nyquist and plzr without arguments did not show an example.


ATOMS
=======

* 'atomsQuit' function added to allow to call .quit of modules loaded by ATOMS
  when user closes Scilab session. See SEP #77.

* tbx_build_loader generates also an unloader.sce to allow to call .quit file of
   an ATOMS module.

* ATOMS option 'offLine' has been renamed to 'offline'.

* Bug #8302 fixed - wrong order when loading dependencies

* Bug #9078 fixed - atomsGui took time to start gui.

* Bug #9798 fixed - .quit script of Atoms modules were not evaluated at Scilab quit.

* Bug #10494 fixed - atomsInstall and atomsLoad were not clearly distinguished
                     in the documentation.

* Bug #10500 fixed - ATOMS module without DESCRIPTION file returned a wrong
                     error message.

* Bug #10707 fixed - atomsInstall failed to use the path shortcut.

* Bug #10846 fixed - atomsSystemUpdate failed.
                     documentation about 'downloadTimeout' added.

* Bug #10864 fixed - atomsRemove required a network connection to remove a package.

* Bug #10933 fixed - atomsGetConfig did not check the value of input arguments.

* Bug #10957 fixed - atomsInstall required a network connection to install
                     a local package.

* Bug #11062 fixed - atomsGetInstalledPath function moved as a public function.


Call_Scilab
===========

* CallScilab.h removed (was deprecated since Scilab 5.2.0).
  Please use call_scilab.h instead.

* 'int getLastErrorValue(void)' function added.
  It now returns the last error code generated by Scilab.

* Function 'const char* getLastErrorMessageSingle();' added.
  It now returns the last error message generated by Scilab.

* Deprecated functions in call_scilab.h removed : C2F(readmat), C2F(creadmat),
  C2F(creadbmat), C2F(creadcmat), C2F(readchain), C2F(creadchain),  C2F(creadchains),
  GetLengthStringMatrixByName, C2F(matptr), C2F(cmatptr), C2F(cmatcptr),  C2F(cmatsptr),
  C2F(cmatbptr), getlengthchain


Bug Fixes
=========

* Bug #2479 fixed - Graphic editor could not be used when format was not format("v",18).

* Bug #2853 fixed - sd2sci.sci removed from sources.

* Bug #3811 fixed - The overloading charcode was wrong for libraries.

* Bug #4806 fixed - If A was a sparse matrix, A^0 returned A and not the identity
                    matrix as expected.

* Bug #4813 fixed - Description of type 0 added in type help page.

* Bug #4978 fixed - Icons did not follow the Tango norm.

* Bug #6009 fixed - conv, conv2 and convol2d functions were missing.

* Bug #7167 fixed - Typo fixed in pdiv help page.

* Bug #7380 fixed - 'Value' property for uicontrol was rounded.

* Bug #7490 fixed - %inf had not always the same display.

* Bug #7289 fixed - The handle of the menu was still valid after calling delmenu(console_menu_handle).

* Bug #7728 fixed - rubberbox(): Instantaneous pointer's coordinates are now displayed in the status bar.

* Bug #7779 fixed - genlib did not check input arguments and did not use current
                    path as default.

* Bug #7859 fixed - Improvements of "exists" help page.

* Bug #8050 fixed - A weird error message was displayed in some cases.

* Bug #8339 fixed - Default path for the Mac OS X version is now HOME instead
                    of "/".

* Bug #8592 fixed - Explicit error message added when macros are not available.

* Bug #8608 fixed - dec2hex, hex2dec, dec2oct, oct2dec, base2dec code have been
                    simplified and optimized.

* Bug #8651 fixed - The error message of write_csv was wrong.

* Bug #8682 fixed - funcprot did not return the previous value when called with
                    an argument.

* Bug #8685 fixed - Lack of consistency in keyword help page.

* Bug #8724 fixed - A critical error with uiget(file|dir) could have occurred.

* Bug #8859 fixed - Some comparison operators between hyper-matrices and 2-D arrays
                    were missing.

* Bug #9002 fixed - bsplin3val, cshep2d, eval_cshep2d,interp, interp2d, interp3d,
                    linear_interp, splin, splin2d, splin3d functions ignored imaginary
                    part of input arguments without returning an error.

* Bug #9197 fixed - Unitary test examples in toolbox_skeleton
                    and xcos_toolbox_skeleton did not use assert functions.

* Bug #9255 fixed - Correct bad calling sequence in list_double_writing api help page.

* Bug #9295 fixed - 'base2dec' ignored the fractional part of 'base' input argument.

* Bug #9328 fixed - Scilab randomly crached at start-up under Mac OS X when calling
                    functions returning no value (lines, modes, banner, ...).

* Bug #9424 fixed - gettext("") did not return "".

* Bug #9502 fixed - 'toolbox' was badly used in assert help pages (replaced by 'module').

* Bug #9520 fixed - Missing named functions added in "API Scilab" help pages.

* Bug #9549 fixed - nthroot function added.

* Bug #9572 fixed - assert_checkerror did not check multi-lines errors.

* Bug #9643 fixed - genetic algorithms demo was not vectorized.

* Bug #9654 fixed - An example in optim help page was poorly implemented.

* Bug #9700 fixed - optim help page contained a mistake about bounds management.

* Bug #9734 fixed - intsplin help page moved from interpolation to
                    elementary functions chapter.

* Bug #9735 fixed - Old stack api examples removed.

* Bug #9811 fixed - Output message of fminsearch was wrongly set.

* Bug #9954 fixed - Example added in gmres help page.

* Bug #9953 fixed - Documentation added to the fre input argument of stdevf.

* Bug #9992 fixed - gcbo was not initialized in callbacks for uimenus in console.

* Bug #10017 fixed - The unitary test for bdiag used a "strange" comparison
                     operator.

* Bug #10044 fixed - wfir_gui function rewritten based on uicontrol and documented.

* Bug #10061 fixed - datatipCreate drew the tip in the current axes instead of curve
                     parent axes.

* Bug #10139 fixed - 'atomsTest' function did not manage specific test names.

* Bug #10236 fixed - exit(-1) prompted for exiting Scilab.

* Bug #10241 fixed - A .scg file could not be opened when a non-ascii character was in the legend.

* Bug #10244 fixed - The install of call_scilab could fail in some cases under
                     GNU/Linux.

* Bug #10250 fixed - Bug in the detection of the Java path under GNU/Linux fixed.

* Bug #10267 fixed - The help browser did not use the default language at new session start-up.

* Bug #10272 fixed - Examples added in squeeze help page.

* Bug #10281 fixed - lyap function could crash.

* Bug #10303 fixed - test_run ignored "enable_lt" option.

* Bug #10304 fixed - test_run did not return an error if an invalid option was used.

* Bug #10306 fixed - 'isempty' could fail with empty elements in a list.

* Bug #10330 fixed - 'error' did not manage vector of strings as input argument, but
                     'lasterror' returned vector of strings.

* Bug #10341 fixed - font in help was bold when the font of a text uicontrol
                     was set to bold.

* Bug #10342 fixed - assert_checkerror help file had a wrong format.

* Bug #10345 fixed - uicontrols were not always drawn at the right place.

* Bug #10372 fixed - Dependencies of MALLOC.h removed on Windows.h and machine.h
                     under Windows.

* Bug #10379 fixed - lines did not manage a 1x2 vector [nc, nl] as input argument.

* Bug #10384 fixed - In exiting, "Close Console" was suggested rather than "Close
                     Scilab".

* Bug #10396 fixed - Display of a structure with large matrix took too much time.

* Bug #10402 fixed - FileBrowser threw an exception when a filter was set
                     and the tree was expanded.

* Bug #10408 fixed - In the history browser, the contents were scrolled even if the
                     knob was not at the bottom.

* Bug #10416 fixed - xmltoformat did not return at the root directory in case of error.

* Bug #10418 fixed - Playsound was failing under Mac OS X. Thanks to
                     Pierre-Henri Jondot for the patch.

* Bug #10423 fixed - The function browsehistory() to open the history browser was missing.

* Bug #10431 fixed - Navigation in history using CTRL+P or CTRL+N was not available.

* Bug #10439 fixed - stacksize('max') crashed under Windows 64 bit.

* Bug #10440 fixed - 'wfir_gui' generated errors if the view checkbox was selected
                     and an erronous value was entered.

* Bug #10444 fixed - fullfile did not manage string matrix as last input argument.

* Bug #10449 fixed - At new session start-up, empty tabs could appear if the previous
                     session has not been properly exited.

* Bug #10450 fixed - Overloading for gamma, dlgamma and gammaln functions for list,
                     tlist, mlist and hypermatrices types was not allowed.

* Bug #10457 fixed - Reference in fftw help page pointed to a wrong version.

* Bug #10458 fixed - demo_gui did not clear its variables when closing the figure.

* Bug #10465 fixed - Typo fixed in isreal help page.

* Bug #10466 fixed - mgetl detected an error but it did not return an error
                     message in Scilab.

* Bug #10471 fixed - %helps_modules was not ignored by default by browsevar.

* Bug #10473 fixed - Error fixed in factorial function help page bibliography.

* Bug #10474 fixed - An exception could be thrown at the history browser
                     restoring.

* Bug #10475 fixed - Error fixed in perms function help page.

* Bug #10477 fixed - In ilib_for_link, the display of the error message generated
                     at configure time made mprintf fail.

* Bug #10482 fixed - Wrong error message in format function fixed.

* Bug #10499 fixed - lincos example in help file returned warning and error.

* Bug #10503 fixed - error(999.666) did trunc error code without an error.

* Bug #10506 fixed - ssprint did not check input arguments.

* Bug #10518 fixed - odeoptions help page was inaccurate.

* Bug #10519 fixed - spec returned a wrong error message when an input argument
                     was not square.

* Bug #10520 fixed - The fact that ceil, floor, round or fix could manage complex
                     matrices was not clearly stated in the documentation.

* Bug #10521 fixed - frexp returned a wrong error message.

* Bug #10523 fixed - The fact that flipdim could manage complex matrices was
                     not clearly stated in the documentation.

* Bug #10526 fixed - A string was in French in the English help page of cumprod.

* Bug #10532 fixed - In some cases, browsevar cloud crash with some unknown
                     Scilab types.

* Bug #10536 fixed - A string was in French in the English help page of cumsum.

* Bug #10538 fixed - Typos fixed in the help pages of prod and cumprod.

* Bug #10543 fixed - %eps, %pi, %i, %nan, %inf were not documented.

* Bug #10544 fixed - debug did not return the previous value when called with
                     an argument.

* Bug #10551 fixed - Issues fixed in the Russian localization.

* Bug #10558 fixed - Tcl/Tk demonstrations did not start.

* Bug #10560 fixed - genetic algorithms demonstrations failed.

* Bug #10564 fixed - atan graphic demonstration was failing due to a too restrictive test
                     in the linspace function.

* Bug #10565 fixed - The demonstration simulation --> bicycle simulation --> unstable
                     trajectory failed.

* Bug #10574 fixed - Runge-Kutta-Method failed for vector [x, 1] with x > 9.

* Bug #10576 fixed - getdate (under Windows) did not manage dates after January 19, 2038

* Bug #10577 fixed - getdate did not check input arguments.

* Bug #10579 fixed - Wrong reference to and function in the help page of prod
                     and cumprod fixed.

* Bug #10588 fixed - Update of waitbar help page to precise that the function returns
                     a handle.

* Bug #10626 fixed - taucs_chdel(), umf_ludel() returned an error.

* Bug #10628 fixed - Reference to sci_types enum changed and "see also" section added.

* Bug #10631 fixed - The # sign was missing in some error cases.

* Bug #10632 fixed - Navigation buttons in the file browser did not change the
                     current Scilab working directory.

* Bug #10634 fixed - dsearch help page referenced an invalid input argument
                     name.

* Bug #10641 fixed - string(:) gave wrong results.

* Bug #10643 fixed - Bad result when using isreal function with a polynomial
                     input.

* Bug #10652 fixed - getdebuginfo() did not display TMPDIR value.

* Bug #10654 fixed - dgettext with "scilab" domain did not return the same result
                     as gettext.

* Bug #10656 fixed - Exception occurred when an empty help browser was closed.

* Bug #10658 fixed - Typos fixed in the bloc2exp help page.

* Bug #10662 fixed - xload did not support tlist for user_data property.

* Bug #10673 fixed - Typos fixed in ssrand help page.

* Bug #10685 fixed - inttrap help page moved to differential equations module.

* Bug #10710 fixed - Wrong error message fixed in xml module.

* Bug #10716 fixed - The help page of acos badly talked about vectors instead of matrices.

* Bug #10717 fixed - Typos fixed in acosd help page.

* Bug #10720 fixed - Typos fixed in acosh help page.

* Bug #10724 fixed - Missing links added in acot help page.

* Bug #10725 fixed - Typos fixed in acot help page.

* Bug #10726 fixed - Missing links added in acsc help page.

* Bug #10727 fixed - Missing links added in asech help page.

* Bug #10728 fixed - Wrong link fixed in asind help page.

* Bug #10729 fixed - Missing links added in asinh help page.

* Bug #10730 fixed - Missing links added in asinhm help page.

* Bug #10731 fixed - Typos fixed in atan help page.

* Bug #10732 fixed - Wrong link fixed in atand help page.

* Bug #10733 fixed - Missing link added in atanm help page.

* Bug #10734 fixed - Missing link added in cos help page.

* Bug #10735 fixed - Missing information added about input argument in cotd help page.

* Bug #10736 fixed - Missing "Arguments" section added in coth help page.

* Bug #10737 fixed - Missing "Arguments" section added in cothm help page.

* Bug #10738 fixed - Typos fixed in cscd help page.

* Bug #10739 fixed - Typos fixed in csgn help page.

* Bug #10740 fixed - Typos fixed in sec help page.

* Bug #10741 fixed - Typos fixed in secd help page.

* Bug #10742 fixed - Missing link added in sin help page.

* Bug #10743 fixed - Wrong link fixed in sind help page.

* Bug #10744 fixed - Typos fixed in sinh help page.

* Bug #10745 fixed - Typos fixed in tand help page.

* Bug #10769 fixed - Dead code removed from bsplin3val.

* Bug #10775 fixed - Scilab icons in filebrowser had a wrong size.

* Bug #10782 fixed - 'iconvert' did not check input arguments.

* Bug #10797 fixed - contour, contour2d, contourf did not check input arguments.

* Bug #10807 fixed - It was not possible to overload fft and fftw for list,
                     tlist, mlist type.

* Bug #10809, #10810 fixed - error_table help page was not up-to-date.

* Bug #10820 fixed - datatipscurve generator was freezing Scilab.

* Bug #10828 fixed - Developer arguments were not displayed in the help of
                     Scilab scripts.

* Bug #10831 fixed - set("auto_clear", "on") and set("auto_clear", "off") returned an error.

* Bug #10835 fixed - Typo that made the 'atanm' function unusable fixed.

* Bug #10836 fixed - resize_matrix with boolean failed.

* Bug #10839 fixed - dec2hex function was limited to 32 bits.

* Bug #10841 fixed - sliderStep property could not be set with "big" default value.

* Bug #10845 fixed - $ was not documented.

* Bug #10847 fixed - dec2base returned a wrong error message with a complex
                     number as input argument.

* Bug #10861 fixed - debug([]) returned a wrong error message.

* Bug #10867 fixed - Loading a .scg file failed when the legend contained accentuated characters.

* Bug #10879 fixed - Typo fixed in dec2base help page.

* Bug #10880 fixed - Default start-up directory moved to "My Documents folder"
                     under Windows for icon shortcut.

* Bug #10881 fixed - Typo fixed in ftest and ftuneq help pages.

* Bug #10904 fixed - Error messages of and() and or() builtin were not standard.

* Bug #10913 fixed - The "View code" button was missing in GUI demos.

* Bug #10928 fixed - Examples of save updated with information to fix trouble under Windows.

* Bug #10966 fixed - linspace was producing some numerical errors on the last
                     element.

* Bug #10982 fixed - Completion failed when a space was used before the command.

* Bug #11010 fixed - Examples of preconditioning added in gmres help page.

* Bug #11029 fixed - Execution path to TMPDIR fixed in the example given in dae help page.

* Bug #11055 fixed - 'string' function did not manage implicit size type as
                     input argument.

* Bug #11076 fixed -  mlist('r'), tlist('r') returned a wrong error message.

* Bug #11077 fixed - rlist() returned an error.

* Bug #11081 fixed -  movefile, copyfile did not support '~', 'TMPDIR',
                      shortcuts in filenames.

* Bug #11090 fixed - Typo fixed in event_handler_function help page.

* Bug #11093 fixed - isNamedVarExist() did not search in builtin functions names.

* Bug #11105 fixed - Crash with color_flag 3 fixed.

* Bug #11114 fixed - Using load(file_descriptor, ...) is now obsolete.

* Bug #11115 fixed - A graphic window was opened with two xend().

* Bug #11126 fixed - Error messages improved.

* Bug #11129 fixed - lapackscilab.sce could not be run outside its location.

* Bug #11138 fixed - a = list() crashed under Windows 64 bits.

* Bug #11154 fixed - Grid color hid the plots in Graphics/2D & 3D plots/plot2d demonstrations.

* Bug #11166 fixed - datatipRemove example did not work.

* Bug #11167 fixed - datatipRemoveAll example did not work.

* Bug #11175 fixed - xstringl example did not work.

* Bug #11186 fixed - 'typeof' of a type 130 returned an error.

* Bug #11194 fixed - A browsevar exception could occur at start-up.

* Bug #11205 fixed - A rare exception at Scilab start-up could occur.

* Bug #11222 fixed - Links added between xdel, close and delete help pages.

* Bug #11223 fixed - Deletion of datatips.

* Bug #11265 fixed - msprintf crashed on a huge string.

* Bug #11281 fixed - References to x_left & y_top were false in reversed axes
                     cases in rubberbox help page.

* Bug #11289 fixed - swap_handles function modified the properties.


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
                   xcos_toolbox_skeleton start-up.

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

* Czech localization by ZbynÄk Schwarz


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
==================

* 'oemtochar' and 'chartooem' functions (windows_tools) are obsolete and will be
   removed in Scilab 5.4.1.

* 'showalluimenushandles' function (graphics) was undocumented. This function is
  obsolete and will be removed on 5.4.1.
  Use set(get(0), "ShowHiddenHandles", "on"); instead.

* 'code2str' and 'str2code' are obsolete and will be removed on 5.4.1, please use
  'ascii' instead.


Removed Functions (previously declared as obsolete)
====================================================

* ilib_for_link, ilib_build no more accept files with ".o" or ".obj" extensions
  for source files.

* 'with_atlas' removed. Please use 'getdebuginfo' instead.

* C API function Error(x) removed. Please use SciError instead.

* 'MSDOS' removed. Please use 'getos' instead.


Performance
============

* Under GNU/Linux and Mac OS X, Scilab start-up speed of scilab-cli and
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
  library named libscilab-cli.so. This greatly improves the start-up
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

* Obsolete value for line_style property:
  0 and 1 are equivalent values for SOLID rendering.
  A warning message will be displayed if the user tries to set this property to 0.
  Setting this property to 0 will no more be supported in Scilab 5.4.1.

* getcallbackobject: Input argument must be an UID (String) and no more an index (Constant).

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

* Bug #10223 fixed - Errors messages in atomsDownload must be clearer.


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


API Scilab
==========

* Add functions to identify list datatypes:
  isListType, isNamedListType, isTListType, isNamedTListType, isMListType,
  isNamedMListType
  See bug #11106


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

* Bug #9647 fixed - At Scilab start-up, the variable fft was declared by default
                   while it should not have been.

* Bug #9675 fixed - iir generated bad results.

* Bug #9716 fixed - amell.tst execution froze under Windows with reference lapack library.

* Bug #9719 fixed - delip.tst execution froze under Windows.

* Bug #9725 fixed - syredi.tst execution froze under Windows.

* Bug #10008 fixed - Wrong unit in the datatip for bode function.

* Bug #10169 fixed - Bad clipping option in evans.

* Bug #11092 fixed - Incorrect argument check in h_inf


Misc
=====

* The linspace function accepts column vectors as input argument for the two
  first input arguments. Thanks to Guillaume AZEMA for the implementation.

* The logspace function accepts column vectors as input argument for the two
  first input arguments.

* Change the buffering display mode.

* SEP #67: lines(0) by default at start-up.

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

* Bug #9354 fixed - savematfile could not save imbricate structures.

* Bug #9414 fixed - 'SCI' and 'SCIHOME' variables were not documented.

* Bug #9452 fixed - It was impossible to download Intel MKL through a proxy.

* Bug #9461 fixed - The assert_checkalmostequal function did not manage
                   sparse matrices and hypermatrices.

* Bug #9479 fixed - Typo fixed (intput => input).

* Bug #9498 fixed - A long history caused Scilab start-up to be very long.

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

* Bug #9647 fixed - At Scilab start-up, the variable fft was declared by default
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
* Random crash under Mac OS X at start-up.
  Some random crash can occur at first Scilab start-up. Try to restart Scilab.

* Some issues at start-up under Mac OS X 10.6.7 (and previous 10.6 versions)
  have been reported.
  If you have "Audio Hijack" installed on your Mac OS X, it prevents some
  applications to work including Scilab. See Bug #7012.
  Uninstall / disable Audio Hijack should fix this problem.
  Airfoil, also developed by Rogue Amoeba, has been reported as also breaking
  Scilab.
