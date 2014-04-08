                     Changes between version 5.4.1 and 5.5.0
                     =======================================

New Features
=============

* New special functions:
 - erfi - The imaginary error function.
 - dawson - Compute the Dawson function (scaled imaginary error).

* New functions introduced:
 - getURL - Download a URL (HTTP, HTTPS, FTP...)
 - splitURL - Split a URL (HTTP, HTTPS, FTP...)
 - cov - Covariance matrix. Deprecates mvvacov. See bug #11896.
 - ismatrix - Check if a variable is a matrix. See bug #10456.
 - isrow - Check if a variable is a row vector. See bug #10456.
 - iscolumn - Check if a variable is a column vector. See bug #10456.
 - issquare - Check if a variable is a square matrix. See bug #10456.
 - cross - Vector cross product. See bug #9941.
 - members - Number of occurrences and linear indexes of common values between
             two matrices of the same type. See bug #12705.
 - jcreatejar - Creates a Java archive (JAR) from a set of files / directories
 - ilib_build_jar - Builds Java packages from sources into a JAR file
 - ifftshift - Inverses FFT shift
 - unwrap - Unwraps/unfolds a Y(x) profile
 - getPreferencesValue/setPreferencesValue - Get or set values in the preferences file.
 - htmlDump/htmlRead/htmlReadStr/htmlWrite - Read and write data from/in HTML files.
 - numderivative - Approximate derivatives of a function (Jacobian or Hessian).

* Complete set of functions to read and write any HDF5 file from Scilab added.

* New Solver:
 - daskr - differential-algebraic system solver with rootfinding 'daskr', using
BDF methods with direct and preconditioned Krylov linear solvers, based on ODEPACK.

* Based on JIMS external module, Scilab provides functions to interact
  with Java objects.

* erf, erfc, erfcx and calerf functions now support complex arguments.

* isnum has been redesigned in native code. Performance improvements up to 130x.
  See bug #10404

* Usage of the '$' keyword in part function allowed.

* The histplot command can now be used with the option polygon=%t/%f to add the
  frequency polygon chart (Thanks to Mehran Khorshidi).

* Multi level completion on mlist, struct, XML structures...

* Variable browser improvements:
 - The variable browser also shows the size of integers and the user type of
  the tlist/mlist.
  See bugs #12523 and #10409.
 - It is now possible to delete variables from the variable browser.
  See bug #9447.
 - A user can now plot variables from the variable browser (this functionality was
  already available in the variable editor).

* Added lighting effect for plot of surfaces. Lighting can be enabled
  creating light objects or disabled by deleting them. The following function
  was introduced:
  - light - Creates a light graphic object.

* Localization:
 - Multiple domains in localization managed.
 - addlocalizationdomain function added for a new domain creation.
 - Optional parameter added to gettext to manage domains.
 - tbx_generate_pofile and tbx_build_localization added to create localization files for
   modules.

* Windows Solution updated to Visual Studio 2012.

* -keepconsole option added for Scilab Windows to facilitate debugging.
  Calling Scilab with this option will leave the console box window opened at startup.

* License update: switch to the CeCILL 2.1.


Improvements
=============

* New calling sequence allowed for nicholschart: nicholschart(gains, phases, colors).
  See bug #7828.

* qp_solve can now take up to 5 output arguments. The last one is an error flag,
  if it is present, then the function will display a warning instead of an error.
  See bug #10269.

* graypolarplot has been improved in terms of performances and rendering.
  See bug #12641.

* nthroot is now vectorizable.
  See bug #12678.

* New optional output argument for routh_t.
  See bug #12829.

* modulo and pmodulo now support integers & hypermatrices (See bug #13002).

* test_run can now separate 32-bit systems from 64-bit ones.

* pol2str: now handles polynomials with complex coefficients and hypermatrices (See bug #13109).

* nanreglin: reglin with arguments containing NaNs (See bug #13208).

* New optional input argument added to matfile2sci (append or write the output file).


GUI Refactoring and Improvements
================================

* New uicontrols styles:
 - tab: component which enables the user to switch between sets of uicontrols by
        clicking on a tab.
        The children components are "frame" uicontrols.
        Two dedicated properties have been added to configure this component:
         - title_position: Position of the tabs
         - title_scroll: Indicates if tabs must all be displayed at a time or
                         managed with scroll features.
 - layer: Component which enables the user to make parts of a GUI visible/invisible
          programmatically.
 - spinner: Component which enables the user to select/edit a value between bounds
            with a fixed step.

* New uicontrols properties:
 - border: Used to set some decoration properties on "frame" uicontrols.
            These decorations can be created and initialized with the createBorder
            and the createBorderFont functions.
 - scrollable: Used to add scrolling capabilities on "frame" uicontrols.
 - groupname: Used to group "radiobutton" and "checkbox" uicontrols for an easier
              management.
 - icon: Add an icon to "pushbutton", "text" and "frame" uicontrols.
 - margin: Empty space around uicontrols.

* New uimenu properties:
 - icon: add an icon on the left of the menu label.

* "listbox" and "popupmenu" style uicontrols can now manage colors selection, icons, background
  and foreground colors when the "String" property is set to a matrix matching the format:
  ["#color1", "Item1", "#background1", "#foreground1"; "#color2", "Item2", ..., ...]
  ["icon1", "Item1", "#background1", "#foreground1"; "icon2", "Item2", ..., ...]
  with "#color1", "#background1" and "#foreground1" in HTML format #XXXXXX.
  Then the component will display a colored box or icon on the left of the associated string,
  and different background/foreground colors for items.

* New management of uicontrols positioning:
  In previous versions, uicontrols position was managed in an absolute way through
  their "Position" property and the "Resizefcn" property of their parent figure.
  Using the new layout management in figures and "frame" style uicontrols, position
  is now managed in an automatic way based on Java layouts.
  New dedicated properties have been added in figure and uicontrols:
   - layout: layout type.
   - layout_options: configuration of the layout.
                     Options can be initialized with the createLayoutOptions function.
  Type "help layouts" in Scilab for more information about available types and options.
  Uicontrols position is then managed through the "constraints" property.
  A new createConstraints function has been added to managed these contraints.

* New figures properties:
 - icon: Allows to customize the figure icon.
 - menubar: allows to create windows without any menu bar (default menus will not be created).
 - menubar_visible: Manages menu bar visibility.
 - toolbar: Allows to create windows without any toolbar.
 - toolbar_visible: Manages toolbar visibility.
 - infobar_visible: Manages infobar visibility.
 - resize: Allows to lock window size.
 - dockable: Allows to create dockable/standard figures.
 - default_axes: Allows to manage default axes creation in figures.

* The figure "visible" property management has evolved and offers new possibilities:
 - When the figure is docked, this property manages the visibility
   of components inside the figure (uicontrols, axes, ...).
   In previous releases, this same property setting only managed axes.
 - When the figure is not docked, this property manages the visibility
   of the whole figure including its decorations.

* New way to create GUI using XML files:
 - Created GUI using the figure/uicontrol/uimenu functions can now be saved in
   this new format using saveGui function.
 - XML files in this format can be loaded in Scilab using the loadGui function.
   This function aims at creating GUI in a more efficient way.

* New rendering for GUI/uicontrols:
  In this version, uicontrols use the defaults of the OS Look & Feel.
  Some properties default values are not set by Scilab and then can be different from an
  OS to another.
  To come back to the previous and deprecated behavior, you can use the related
  property on the "Console" handle:
  set(get(0), "UseDeprecatedSkin", "on");
  Note that this deprecated behavior will be removed in future versions.

* New way to access uicontrols using a path containing their "Tag" and their parent(s) "Tag" property.
  See the set and get functions help page for more details.

* New display for uicontrols handles properties. Only properties used for Java rendering are displayed.
  To display all available properties, use the "ShowHiddenProperties" "Console" property.


Graphics Evolutions
===================

* set function prototype has been modified to allow the user to set multiple
  properties at once: set(h, "Property1", Value1, "Property2", Value2, ...)

* newaxes function now allows to create axes in "frame" uicontrols.

* clf function now also works with "frame" uicontrols.

* New axes properties:
 - ticks_format: Format of the ticks labels.
 - ticks_st: Scales and translates factors applied to ticks position when formatting
             the ticks labels.
 - auto_margins: Activated by default, this property lets Scilab compute margins
                 needed to display axes decorations (titles, labels, ...).
 - grid_thickness: Thickness of the grid plotting.
 - grid_style: Style of the grid plotting.
 - label_font_style: Font style used to draw the labels.

* New legends properties:
 - line_width: Width of the drawn line.
 - mark_count: Number of the drawn marks.

* New polyline properties:
 - mark_offset: Offset before the first mark.
 - mark_stride: Step between consecutive marks.
 - display_function: Name of the function used to customize the information
                     displayed in all the datatips of this polyline.
                     This function can be overloaded by setting a display
                     function on each datatip handle.
 - display_function_data: Additional data needed for datatips display function.
 - datatips: Handles of the datatips of the polyline.

* New Matplot property added :
 - rect: specifies the rectangle where the Matplot will be drawn.

* Datatips properties have been renamed for better readability:
 - tip_data --> data
 - tip_box_mode --> box_mode
 - tip_label_mode --> label_mode
 - tip_orientation --> orientation
 - tip_3component --> z_component
 - tip_auto_orientation --> auto_orientation
 - tip_interp_mode --> interp_mode
 - tip_disp_function --> display_function


Differential Equations
======================

* Netlib's Quadpack, used for definite integration, has been updated to match the upstream.


SciNotes
========

* Autosave feature can now use %date to append the current date to the backup filename
  (See bug #12712).


Xcos
====

* New DAE solver: DDaskr, using BDF methods with direct Newton and preconditioned Krylov
  linear solvers, which includes rootfinding.
  It is available from dae function.

* In Modelica initialization GUI, inputs (eg. sensor) were not handled.

* Sundials updated to the "2.5.0" version, keeping our modifications.

* JGraphX updated to the "2.1.0.7" version, updating our hooks performance.

* API changed in the scicos_block4 interface: the uid value is not available
  inside the simulation function.

* API change: In scicos_block4.h, "Residute" renamed to "Jacobian".

* xcosAddToolsMenu added to let the user perform some actions on the graph.

* CBLOCK4 block added to the "User-Defined Functions"

* xcos_debug_gui function added for a simple use of the Debug block.

* tbx_build_pal_loader function added to ease external module creation (See SEP #120).

* Event management demonstrations added.


Toolbox Skeleton
=================

* Some Xcos specific content added to xcos_toolbox_skeleton help pages.


Compilation
============

* Minimal version of Flexdock (1.2.4) enforced.

* Minimal version of JoGL (2.1.4) enforced.

* Minimal version of JLaTeXMath (1.0.3) enforced.

* New dependencies to manage EMF export:
 - freehep-graphics2d.jar
 - freehep-graphicsio-emf.jar
 - freehep-graphicsio.jar
 - freehep-io.jar
 - freehep-util.jar
Note: --without-emf can be used to disable this feature.


Obsolete & Removed Functions
============================

* Scilab 5.5.X family will be the latest family working under Windows XP/2003.

* Vector ^ scalar syntax declared as obsolete. See bug #11524

* The third argument of poly function will be more strict.
    Only the following strings are allowed: "roots", "coeff", "c", "r".

* Option and direction arguments of gsort will be more strict in all cases.
  Only the following strings are allowed:
  - option: "r" "c" "g" "lr" "lc"
  - direction : "d" "i"

* nfreq tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use tabul instead.

* IsAScalar tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use isscalar instead.

* chart tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use nicholschart instead.

* Second output argument of add_param tagged as obsolete. Will be removed in Scilab 5.5.1.

* mvvacov tagged as obsolete. Will be removed in Scilab 6.0.0.
  Please use cov instead.

* mfft tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use fft instead.

* relocate_handle tagged as obsolete. Will be removed in Scilab 5.5.1.

* msd and st_deviation tagged as obsolete (See bug #7593). Will be removed in Scilab 5.5.1.
  Please use stdev instead.

* pcg tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use conjgrad instead.

* milk_drop tagged as obsolete. Will be removed in Scilab 5.5.1.

* datatipGetStruct tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use 'datatips' property instead.

* regress tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use reglin instead.

* %asn tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use delip instead.

* jmat tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use flipdim instead.

* perl tagged as obsolete. Will be removed in Scilab 6.0.0.

* numdiff and derivative tagged as obsolete. Will be removed in Scilab 6.0.0.
  Please use numderivative instead.

* xmltochm tagged as obsolete. Will be removed in Scilab 5.5.1.

* dft removed, please use fft instead.

* sscanf removed, please use msscanf instead.

* fscanf removed, please use mfscanf for files opened with mopen or read for files opened
  with file instead.

* fprintf removed, please use mfprintf for files opened with mopen or write for files
  opened with file instead.

* clear_pixmap removed, please use drawlater/drawnow instead.

* pixmap property removed for figures, please use drawlater/drawnow instead.

* demo_message and demo_mdialog removed.

* with_embedded_jre removed.

* draw removed.

* fit_dat removed. Please use datafit instead.

* winclose removed. Please use close instead.

* datatipInitStruct and datatipRedraw removed.

* getfont, getmark, getlinestyle and getsymbol removed. Please use ged instead.


Known incompatibilities
========================

* Using => or =< now returns an error. See bug #2345.

* or/and functions now fix reals value before working on them. See bug #7666.

* a*b*-c+d is now interpreted as (a*b*(-c))+d instead of a*b*(-c+d). See bug #13168.

* Extracting children from an empty matrix of handles now returns an error.
  For example: f=gcf(); f.children(1).children.children returns an error.

* Figure anti_aliasing property behavior changed:
  - "off": No anti-aliasing activated on the figure.
  - "2x", "4x", "8x", "16x": anti-aliasing activation will have no effect on most plateforms.


Scilab Bug Fixes
================

* paramfplot2d: When theta input argument was a column vector, an error occurred.

* Bug #1253 fixed - There was no possibility to draw only few marks on a polyline which
                    contained a lot of points.

* Bug #1751 fixed - Margins were not computed according to contents.

* Bug #2067 fixed - Scilab crashed when plot was called with a large numerical value.

* Bug #2267 fixed - Wrong error message when ticks locations and labels did not have the
                    same sizes.

* Bug #2345 fixed - Use of operators => and =< did not lead to an error.

* Bug #2416 fixed - Particular case (string([]) returns []) has been added in string help
                    page.

* Bug #2802 fixed - convstr did not convert non ascii chars.

* Bug #3511 fixed - strindex did not return all occurrences in regexp mode.

* Bug #3928 fixed - An error was returned when a matrix was flipped along the third
                    dimension.

* Bug #4042 fixed - squeeze returned a hypermatrix instead of a matrix when at least one
                    dimension was equal to 1.

* Bug #4083 fixed - The numdiff and derivative functions were duplicates and are
                    now tagged as obsolete and replaced by numderivative. See SEP #130.

* Bug #4085 fixed - num2cell help page added.

* Bug #4177 fixed - find function moved to the "elementary_functions" module.

* Bug #4229 fixed - delip did not return an error if one element of its first input
                    argument was negative.

* Bug #4383 fixed - csim with "step" and "impuls" now works with direct feedthrough.

* Bug #4401 fixed - isnum did not recognize all constants or some complex numbers.

* Bug #4481 fixed - iir help page updated to precise that frq can be a scalar.

* Bug #4490 fixed - Input argument of sinc function moved to rad.

* Bug #4649 fixed - License issue in xs2ppm help page fixed.

* Bug #4677 fixed - xclick did not return correct mouse position on keyboard event.

* Bug #4692 fixed - Export did not work while xgetmouse was waiting.

* Bug #4731 fixed - lqr failed when the time domain of an input was a number.

* Bug #4743 fixed - Graphics with too big or too small values did not work.

* Bug #4858 fixed - libintl.h was missing in binary versions but included in localization.h.

* Bug #4965 fixed - Setting links property for a handle of type legend did not work.

* Bug #5016 fixed - condestsp could return different results when repeated calls were performed.

* Bug #5017 fixed - norm rewritten to take benefit from the Lapack package performance.

* Bug #5073 fixed - New parameter added in strtod function (decimal separator).

* Bug #5205 fixed - permute was slow for large hypermatrices.

* Bug #5207 fixed - grand can now return a hypermatrix.

* Bug #5232 fixed - 'frame' uicontrols children now follow the frame displacement.

* Bug #5365 fixed - makecell help page was in the "compatibility functions" directory
                    instead of being in "data_structures".

* Bug #5539 fixed - sylv help page was wrong in the discrete-time case.

* Bug #5616 fixed - typeof(uiCreateTree(...)) returned "Tree" instead of "uitree".

* Bug #5694 fixed - numdiff help page clarified.

* Bug #5779 fixed - xnumb number format was too small (+ unit test added).

* Bug #5826 fixed - Graphic windows did not redirect key events in console.

* Bug #5844 fixed - grayplot did not have a logflag option.

* Bug #5886 fixed - There was no property labels_font_style on axis.

* Bug #6037 fixed - macrovar help page improved.

* Bug #6168 fixed - zpbutt, zpch1, zpch2 and zpell help pages were unclear.

* Bug #6191 fixed - It was not possible to set thickness and line_style properties for a grid.

* Bug #6305 fixed - dsearch function did not work with integers, strings and hypermatrices.

* Bug #6306 fixed - It is now possible to compute an histogram instead of plotting it
                    with new function histc(). Besides, histplot can now return the computed data.

* Bug #6363 fixed - It was not possible to create figures without menubar/toolbar.

* Bug #6390 fixed - The "external" argument of odedc was not well documented and
                    not tested.

* Bug #6404 fixed - xrects help page was not clear about argument specification.

* Bug #6427 fixed - full([%T %F]) returned an error message.

* Bug #6466 fixed - Example with vectorized input added in mprintf and msprintf help pages.

* Bug #6476 fixed - Matplot help did not indicate that the data field should be used to update data.

* Bug #6512 fixed - %asn function tagged as obsolete.

* Bug #6584 fixed - mfft tagged as obsolete.

* Bug #6615 fixed - ui(get|put)file did not center the file dialog on the last focused window.

* Bug #6638 fixed - The profiler output was incorrect by a factor of 1000 under Windows.

* Bug #6689 & #6690 fixed - grand now works with complexes, polynomials, booleans, integers, sparses and strings,
                            and can handle row vectors, matrices and hypermatrices of thoses types.

* Bug #6693 fixed - modulo did not accept polynomial inputs. Help page was not updated.

* Bug #6752 fixed - unit test scilab.tst has been split in different tests.

* Bug #6824 fixed - resize_matrix did not manage hypermatrices.

* Bug #6832 fixed - Matrices of rationals can now be transposed via the " .' " operator.

* Bug #6840 fixed - New line_style added.

* Bug #6859 fixed - xlabel and xtitle could overlapped.

* Bug #6930 fixed - Matplot handle had no rect property.

* Bug #6988 fixed - Error messages in modules/data_structures/src/c/hmops.c were not
                    standard.

* Bug #7026 fixed - There was no unit test for plot2d.

* Bug #7038 fixed - A toggle button now manages datatip mode.

* Bug #7040 fixed - Example and description for getMatrixOfIntegerPrecision modified in help page.

* Bug #7047 fixed - milk_drop is now obsolete. It will be removed in Scilab 5.5.1, but will be kept as a demonstration.

* Bug #7051 fixed - fieldnames help page updated.

* Bug #7080 fixed - Some graphic macros did not use standard error messages.

* Bug #7084 fixed - Old, not documented and deprecated gr_menu function removed.

* Bug #7085 fixed - 'edit' uicontrols did not support multiple line edition.

* Bug #7133 fixed - help_from_sci with no input argument now launches a full demonstration.

* Bug #7169 fixed - It was not possible to specify different option value (thickness, style, ...) for X and Y grids.

* Bug #7204 fixed - geomean applied to a hypermatrix gave wrong results.

* Bug #7205 fixed - length(H) applied to a non-string hypermat returned 3 instead of size(H,"*").

* Bug #7206 fixed - If the second input argument of meanf function was a hypermatrix,
                    this function returned an error.

* Bug #7244 fixed - Extraction from a struct array with a boolean vector had a strange behavior.

* Bug #7296 fixed - %nan, %inf and -%inf enabled for the cdf* functions.

* Bug #7304 fixed - exportUI did not switch filename extension when filter was changed.

* Bug #7411 fixed - clf forgot to turn off the datatip mode.

* Bug #7417 fixed - Auto-positioning of datatips did not take the curvature into account.

* Bug #7486 fixed - LAPACK versions of DGELSY and ZGELSY now used.

* Bug #7561 fixed - roots help page now explains that coefficients are used in the contrary order of poly.

* Bug #7570 fixed - The switch criterion on x and y is now explained in beta() help page.

* Bug #7585 fixed - psi.f moved from "elementary functions" to "special functions" module.

* Bug #7593 fixed - stdev now encompasses msd and st_deviation thanks to a new optional input argument.

* Bug #7596 fixed - A same error happening in different places now displays the same
                    error message.

* Bug #7648 fixed - CDF functions now display a warning for non integer
                    "degrees of freedom" argument.

* Bug #7650 fixed - isempty(tlist(...)) always returned false, even when all defined fields were empty.

* Bug #7655 fixed - An example added in type help page, for type(X)=11 and type(X)=13.

* Bug #7666 fixed - Inconsistencies between and/or and &/| fixed.

* Bug #7684 fixed - Introduction demonstration splitted into subsections.

* Bug #7705 fixed - "dimension", "minbounds" and "maxbounds" fields have been documented in Genetic algorithms help pages.

* Bug #7739 fixed - Axis position was invalid in log modes.

* Bug #7771 fixed - There was no item about arrow_size in champ_properties help page.

* Bug #7772 fixed - There was no description, no example about line_style property in polyline help page.

* Bug #7781 fixed - The second parameter of iqr function had no effect.

* Bug #7782 fixed - lcm and gcd help pages improved to tell the user how to use both
                    functions.

* Bug #7824 fixed - title function properties did not support an indexed color.

* Bug #7826 fixed - chart tagged as obsolete.

* Bug #7828 fixed - Slight improvements in nicholschart.

* Bug #7848 fixed - The third argument of correl function is now optional.

* Bug #7858 fixed - variance and variancef can now return the mean of the input
                    in a new output argument and take the a priori mean as input.

* Bug #7877 fixed - iirgroup function fixed.

* Bug #7879 fixed - string now accepts plist type, and printing a plist displays that string.

* Bug #7905 fixed - Figure icon can now be changed using dedicated property.

* Bug #7916 fixed - nansum([]) returned NaN value while this function had to ignore it.

* Bug #7927 fixed - Output "flag" in qmr function was not well documented.

* Bug #7960 fixed - plzr could not produce pole zero plot for a simple transfer function.

* Bug #7986 fixed - spec gateway renamed from sci_eig.c to sci_spec.c.

* Bug #7999 fixed - SwingScilabFileChooser.getFilterIndex() was unusable.

* Bug #8031 fixed - cdfgam error message fixed.

* Bug #8058 fixed - The user can now set the tolerances of intc function.

* Bug #8060 fixed - Improved list display in the variable browser.

* Bug #8098 fixed - cumsum could not be applied to rational matrices.

* Bug #8131 fixed - It was not possible to choose the number of marks and the line width in legends.

* Bug #8133 fixed - Ticks disappeared in planar 3-D view.

* Bug #8162 fixed - Area of stability of plzr was wrong for continuous systems
                    (+unit test added).

* Bug #8196 fixed - Error messages dealing with negative thickness were not standard.

* Bug #8211 fixed - Parameters module demonstration finalized.

* Bug #8231 fixed - xrect help page did not say that clipping property was inherited.

* Bug #8234 fixed - strtod did not return an empty matrix when the input argument was an
                    empty matrix.

* Bug #8247 fixed - regress tagged as obsolete.

* Bug #8264 fixed - Matlab to Scilab dictionary help page updated for atan2.

* Bug #8290 fixed - DELAYV_f block documentation fixed.

* Bug #8319 fixed - dbphi(hypermat) and phasemag(hypermat) returned a matrix instead of
                    a hypermatrix.

* Bug #8323 fixed - Scilab "About Box" did not hide Scilab main window.

* Bug #8337 fixed - mtlb_rand now uses the "uniform" rule, whatever the random rule set is.

* Bug #8373 fixed - clear can now handle a matrix of strings argument.

* Bug #8379 fixed - It was not possible to delete the selected datatip with DELETE or BACKSPACE.

* Bug #8415 fixed - optim_moga, optim_nsga, optim_nsga2 can now take list as input
                    arguments, as explained in their help pages.
                    (and others) added.

* Bug #8462 fixed - bvodeS could make Scilab unstable.

* Bug #8470 fixed - bvode displayed some output in terminal window and not in
                    Scilab console.

* Bug #8479 fixed - The latest Saxon version was not supported.

* Bug #8511 fixed - sprand now uses grand instead of rand and grand functions. Internal
                    state of the random generator is no more changed.

* Bug #8561 fixed - ddassl, ddasrt, ddaskr: abs and rel tolerance sizes are now checked.

* Bug #8597 fixed - grand/clcg4 could display uncontrolled messages as warning.

* Bug #8607 fixed - Some error messages in modules/overloading/macros were not standard
                    and not localized.

* Bug #8614 fixed - Unit test for barhomogenize added.

* Bug #8636 fixed - roots help page updated (default algorithm value was wrong).

* Bug #8667 fixed - The handling of %nan in min, max, median functions was not properly
                    documented.

* Bug #8680 fixed - "end" output argument of regexp function has been changed.

* Bug #8687 fixed - typeof function failed on uint8, depending on the format
                    (+ unit test added).

* Bug #8695 fixed - optim_ga used old (initial) values instead of newly-computed ones.

* Bug #8745 fixed - Extracting from an empty matrix automatically returned an empty matrix.

* Bug #8778 fixed - Call_ScilabOpen, TerminateScilab could not be called more
                    than 80 times in a loop.

* Bug #8779 fixed - gsort did not preserve the order of equal elements, in
                    lexicographic sort.

* Bug #8820 fixed - squeeze did not return a matrix when the number of dimensions
                    of the result was less or equal to 2.

* Bug #8824 fixed - taucs_chfact returned a segfault (not the case in nwni mode).

* Bug #8840 fixed - fileparts did not manage matrix of strings.

* Bug #8856 fixed - Non regression test added for [k,l,m,...]=find(a==5).
                    Non regression test of bug #476 updated.

* Bug #8857 fixed - Non regression test of bug #477 updated.

* Bug #8858 fixed - Non regression test of bug #480 updated.

* Bug #8862 fixed - mget and mput could not read and write 64 bit data from
                    binary files.

* Bug #8956 fixed - xpoly, xfpoly, xrect, xsegs and xarc did not update data_bounds property.

* Bug #9004 fixed - bitcmp function called with one input argument returned an error.

* Bug #9020 fixed - exists function did not accept matrix as first input argument.

* Bug #9031 fixed - Misalignment of text when using xstring with a matrix fixed.

* Bug #9033 fixed - auto_dimensioning property for text handles was not documented.

* Bug #9059 fixed - tbx_build_macros and genlib did not stop even if an error occurred.

* Bug #9109 fixed - nfreq tagged as obsolete.

* Bug #9110 fixed - Examples and references to other functions added in Statistics help pages.

* Bug #9158 fixed - zeros called with a big number returned an empty matrix instead
                    of an error.

* Bug #9208 fixed - Added three optional output arguments to optim,
                    to retrieve #iterations, #evaluations and a termination indicator.

* Bug #9309 fixed - comparison help page updated to document issue with empty matrix.

* Bug #9319 fixed - Huge polylines could not be exported in PS/EPS.

* Bug #9385 fixed - The type checking in trigonometric functions has been added.

* Bug #9394 fixed - is_param recognized "plist" as an existing field.

* Bug #9395 fixed - add_param did not check its input arguments.

* Bug #9396 fixed - add_param accepted duplicate keys.

* Bug #9444 fixed - with_embedded_jre function removed.

* Bug #9459 fixed - Default values of the optional plot3d arguments were not documented.

* Bug #9493 fixed - Title is now correctly set when starting Scilab and focus is set on Console.

* Bug #9537 fixed - optimbase_configure only allowed row vectors as initial value.
                    Column vectors now allowed by transposing them.

* Bug #9538 fixed - optimbase_checkshape only allowed row vectors as output arguments of
                    cost function. Column vectors are now allowed by transposing them.

* Bug #9577 fixed - Setting neldermead_configure("-numberofvariables") is now optional,
                    setting neldermead_configure("-x0") initializes -numberofvariables
                    implicitly.

* Bug #9601 fixed - Cylinder demonstration fixed.

* Bug #9627 fixed - Arguments checking added in optimsimplex_* functions.

* Bug #9688 fixed - optim could crash when "imp" option was < 0. It is now set to 0 in
                    that case.

* Bug #9690 fixed - The "imp" option for optim could crash Scilab and was not consistent
                    with the help page.

* Bug #9691 fixed - "imp" option in optim help page was poorly documented.

* Bug #9694 fixed - Example in optim help page fixed to display correct counters.


* Bug #9697 fixed - Displayed information for optim "qn" and "gc" with bounds and imp=1 fixed.

* Bug #9701 fixed - optim with "qn" option was failing for large problems.

* Bug #9702 fixed - Contrary to what optim help page stated, the "gc" algorithm does use
                    the "epsx" parameter.

* Bug #9780 fixed - gmres solver did not run with complex systems.

* Bug #9788 fixed - neldermead can now produce a warning when it fails to converge,
                    thanks to a new input argument "warn".

* Bug #9819 fixed - unwrap function did not exist in Scilab.

* Bug #9821 fixed - getrelativefilename did not manage matrix of strings.

* Bug #9840 fixed - Default G tolerance in lsqrsolve was too large.

* Bug #9851 fixed - Error message occurred because of a cut-off frequency of 0.25Hz
                   with irr.

* Bug #9859 fixed - It was not possible to draw arrows in 3-D using xarrows.

* Bug #10012 fixed - lmisolver and lmitool called with no input now produce errors.


* Bug #10083 fixed - plot3d could not be used with only one input argument.

* Bug #10122 fixed - replot could not be used in 3-D.

* Bug #10146 fixed - In SciNotes, 'help on keyword' moved from bottom to top in the popup
                    menu.

* Bug #10175 fixed - Clearer example added for sp2adj to adj2sp conversion (and backwards conversion).

* Bug #10180 fixed - det was not defined for sparse matrices.

* Bug #10213 fixed - sci2exp help page updated to document the impact of format function.

* Bug #10214 fixed - evstr help page updated to mention that input argument must not be composed of
                     continuation marks (..).

* Bug #10216 fixed - Invalid syntaxes for zeros, ones, eye, rand, like zeros(2,:).

* Bug #10221 fixed - ifftshift function did not exist in Scilab.

* Bug #10226 fixed - When a // <empty session> line was deleted, all sessions
                     histories were folded.

* Bug #10234 fixed - reglin function moved from CACSD to Statistics module.

* Bug #10243 fixed - fun2string(X) called X before returning its code.

* Bug #10254 fixed - Slight improvements in ones help page.

* Bug #10269 fixed - qp_solve can now take up to 5 output arguments. The last one is an
                    error flag, if it is present, then the function will issue a warning
                    instead of an error.

* Bug #10271 fixed - ordmmd now checks the consistency of the third input argument with the
                     input matrix defined by the first two input arguments.

* Bug #10273 fixed - spchol help page now displays an example showing how to use its output arguments.

* Bug #10276 fixed - qp_solve segfaulted with large matrices.

* Bug #10287 fixed - Error message added for complex expression as input argument of
                    integrate function.

* Bug #10305 fixed - Comparison of lists with empty items returned an error message.

* Bug #10391 fixed - Error when using completion after a global variable clear fixed.

* Bug #10428 fixed - Java based components of Scilab showed a library load error in CLI mode.

* Bug #10445 fixed - In SciNotes, CTRL+Drag&Drop moved text rather than copying it.

* Bug #10470 fixed - In SciNotes, split "Horizontally" or "Vertically" was meaningless.

* Bug #10596 fixed - exit(xxx) from Scilab failed.

* Bug #10621 fixed - Figure without docking/undocking capabilities can now be created.

* Bug #10645 fixed - File encoding could not be given as argument in xmlRead.

* Bug #10718 fixed - New "Resize" property added to figures.

* Bug #10805 fixed - Documentation on left bracket was superfluous and is now removed.
                     Refer to brackets for information on "[" and "]".

* Bug #10816 fixed - Typo in part error message fixed.

* Bug #10818 fixed - home, %e, %t, %f, %z and %s help pages added.

* Bug #10823 fixed - fullpath returned different results under Windows and Linux for
                    non-existent file.

* Bug #10830 fixed - Hypermatrix insertion with a negative index returned a wrong error message.

* Bug #10833 fixed - exists help page updated.

* Bug #10840 fixed - Keyboard arrows were disabled on 'slider' type uicontrols.

* Bug #10856 fixed - analpf did not return the right result.

* Bug #10862 fixed - Add a without Internet connection installation
                    global configuration in the installer.

* Bug #10866 fixed - det was not equivalent to detr for rational matrices.

* Bug #10906 fixed - Typo fixed in cls2dls help page.

* Bug #10930 fixed - The comments in armax function were in French.

* Bug #10932 fixed - Startup directory was not saved/restored in preferences.

* Bug #10936 fixed - Scilab hung with invalid strf in plot2d.

* Bug #10942 fixed - Function soundsec revised. Now soundsec can be used for non integer values of time.

* Bug #10995 fixed - Typo fixed in grand help page for Gamma law argument.

* Bug #10998 fixed - matrix*hypermatrix and hypermatrix*matrix operations failed.

* Bug #11001 fixed - exists and isdef did not work with primitives.

* Bug #11007, #11008 & #11009 fixed - New conjgrad function (Conjugate Gradient methods "pcg", "cgs", "bicg" and "bicgstab").

* Bug #11065 fixed - The second output argument of unique function contained a wrong result.

* Bug #11067 fixed - Display of ticks labels with closed associated values was wrong.

* Bug #11139 fixed - conj was not defined for sparse matrices (+ unit test added).

* Bug #11303 fixed - Exception while searching with multiple tabs in SciNotes fixed.
                     already existing in SCI/contrib/archives in "offline" mode.

* Bug #11305 fixed - Performances improved with a better way to update data in Graphics.

* Bug #11308 fixed - Calling sequences in dsearch help page were wrong.

* Bug #11343 fixed - The "isoview" figure property did not work when the axes
                     margins had been modified.

* Bug #11523 fixed - In SciNotes, 'whereami line numbering' was not clear enough.

* Bug #11571 fixed - x_mdialog did not let the Look&Feel select the window size.

* Bug #11575 fixed - There was no preview of GIF files in exportUI dialog.

* Bug #11576 fixed - exportUI did not propose gcf().figure_name as default file name.

* Bug #11616 fixed - Figure menubar could not be made invisible.

* Bug #11629 fixed - Interactive zoom did not work properly in datatip mode.

* Bug #11648 fixed - Copying graphic via the clipboard did not work.

* Bug #11680 fixed - GUI functions in Scilab 5.4.X were much slower than in Scilab 5.3.3.

* Bug #11714 fixed - help_from_sci sometimes failed when input function had "<imagedata>" comments.

* Bug #11766 fixed - nthroot has been added to m2sci help page.

* Bug #11779 fixed - Wrong variable type in the documentation of getNbInputArgument and getNbOutputArgument fixed.

* Bug #11789 fixed - Documentation was missing for nbInputArgument.

* Bug #11792 fixed - Lists can be accessed with non integer indexes (list help page
  updated).

* Bug #11814 fixed - Typo in CACSD help chapter fixed.

* Bug #11869 fixed - "Environment" was not localized in preferences.

* Bug #11876 fixed - ilib_include_flag now returns a string when called with a string column vector as input.

* Bug #11885 fixed - Each rand has been changed to grand in genetic algorithms and
                    simulated annealing functions.

* Bug #11891 fixed - Fisher ratio could be inaccurate for one-way ANOVA.

* Bug #11953 fixed - Scilab crashed when global("") was typed.

* Bug #11964 fixed - uicontrol coordinates system did not take figure resize into account.

* Bug #11996 fixed - eye extended to hypermatrix.

* Bug #11997 fixed - In case of invalid variable name, save function now returns an error
                    instead of a compatibility warning.

* Bug #12012 fixed - Misleading perl function should not be part of Scilab (tagged as obsolete).

* Bug #12034 fixed - max function did not manage empty matrices.

* Bug #12037 fixed - Simplified Chinese version of SciNotes displayed warnings at startup.

* Bug #12045 fixed - repmat returned wrong results if the values of an input matrix were
                    not double.

* Bug #12070 fixed - Removing a module can now be done in an on-line mode even if
                     it has been installed in an off-line mode.

* Bug #12073 fixed - Width of code examples has been decreased in XML help pages.

* Bug #12080 fixed - lsqrsolve always printed messages.

* Bug #12082 fixed - convstr function did not work with non-ASCII symbols.

* Bug #12085 fixed - Under Windows, csvWrite wrote wrong EOL.

* Bug #12114 fixed - libstdc++ is now compiled as static instead of using the
                     library from thirdparty.

* Bug #12118 fixed - ode could take complex externals.

* Bug #12121 fixed - inv function did not work for complex sparse matrices.

* Bug #12130 fixed - flipdim can now flip blocks, thus making jmat obsolete.

* Bug #12143 fixed - "stop entity picker" (ged(11)) returned an error message.

* Bug #12145 fixed - demo_mdialog internal function removed.

* Bug #12150 fixed - datatipCreate failed with one single point.

* Bug #12156 fixed - Closing a Scilab session in Javasci could led to a HDF5 error message.

* Bug #12163 fixed - unzoom did not work with a single input argument.

* Bug #12170 fixed - Calling matfile_listvar on an empty file returned an error.

* Bug #12212 fixed - Export of a polyline in 2-D broke it into several segments.

* Bug #12306 fixed - Invalid memory free on completion in NWNI mode fixed.

* Bug #12308 fixed - create_palette help page removed (function removed).

* Bug #12326 fixed - There was no way to set LaTeX font size in preview.

* Bug #12334 fixed - Mark color in legend was invalid.

* Bug #12349 fixed - In SciNotes, when the view was splitted, removing a char made the
                    other view jump.

* Bug #12376 fixed - Exec & edit buttons in the help pages examples were sometimes
                    misplaced.

* Bug #12412 fixed - Typo fixed in some error messages.

* Bug #12413 fixed - ATOMS packages could not be removed if ATOMS had never been on-line.

* Bug #12415 fixed - PATH environment variable grew when using call_scilab in a loop.

* Bug #12426 fixed - By using addErrorMessage, a random string error could be displayed.

* Bug #12433 fixed - show_pixmap function was removed from Scilab but still used in some
                    functions.

* Bug #12439 fixed - edit_error returned a wrong message when there was no recorded error.

* Bug #12440 fixed - Unitary test added for bitxor.

* Bug #12443 fixed - The behavior of mopen in text file mode has been documented
                     under Windows.

* Bug #12463 fixed - Wrong specification for rect=[x,y,w,h] argument in xstringb
                    French help page.

* Bug #12465 fixed - ATOMS Default categories names were not localized.

* Bug #12470 fixed - Variable browser was not refreshed after loading an environment.

* Bug #12472 fixed - grand and link error messages fixed.

* Bug #12473 fixed - Problems with "Ã©" in mkdir French help page.

* Bug #12475 fixed - csvRead did not support double quoted fields.

* Bug #12481 fixed - xlabel could not be used with Scilab property names.

* Bug #12485 fixed - xchange returned bad values with log scale.

* Bug #12490 fixed - plot did not call clf; in its help page example producing a
                    wrong behavior.

* Bug #12492 fixed - Exported EPS files were invalid when dash patterns were too long.

* Bug #12496 fixed - zoom_rect could led to a crash in log scale.

* Bug #12506 fixed - In SciNotes, a miscolorization could occurred when returned values
                    list was broken.

* Bug #12508 fixed - Wrong error message in rand function fixed.

* Bug #12518 fixed - Polynoms were not displayed by default in the variable browser.
                     and a new function bode_asymp() to draw the system asymptotes.

* Bug #12520 fixed - Improve the description of the size in the variable browser.

* Bug #12527 fixed - Scilab user functions were not listed in the variable browser.

* Bug #12535 fixed - In a French localized Scilab, comma was used as decimal separator when zooming (rather than point).

* Bug #12547 fixed - In SciNotes, lines were wrongly numbered with splitted function
                    declarations.

* Bug #12548 fixed - Duplicated code in xmltoformat removed.

* Bug #12550 fixed - optimbase and optimsimplex help pages were not standard.

* Bug #12551 fixed - Stack problem with diary([],"pause") and diary([],"resume") fixed.

* Bug #12556 fixed - A fatal error occurred when calling set with wrong instructions.

* Bug #12557 fixed - Valid function names were not specified in function and functions
                    help pages.

* Bug #12564 fixed - Compile and run javasci help page was not clear about needed packages.

* Bug #12567 fixed - Error messages fixed in ricc.

* Bug #12581 fixed - isfield could not support mlist or tlist.

* Bug #12589 fixed - Call sequence for spzeros & spones were missing in French
                    help pages.

* Bug #12592 fixed - Scilab hung with plot(-0).

* Bug #12593 fixed - A wrong error message was returned when running genlib with an
                    error in the sci file.

* Bug #12594 fixed - Invalid SciNotes configuration file avoided SciNotes startup.

* Bug #12600 fixed - mput did not manage unsigned integer.

* Bug #12606 fixed - Overloads for grand were not standard.

* Bug #12613 fixed - gsort did not return correct results with %nan.

* Bug #12614 fixed - Helpbrowser was not launched in EDT.

* Bug #12615 fixed - Graphics seemed to be freezed after a call to plot/bar/barh with
                     a bad LineSpec argument.

* Bug #12622 fixed - Various typos fixed in error messages.

* Bug #12624 fixed - In case of errors in Scilab macros, "make check-TESTS" did not fail
                    as expected.

* Bug #12627 fixed - At restoration, a window could be out of the screen.

* Bug #12629 fixed - The last example of csim help page defined a function called
                     input (overwriting the Scilab one).

* Bug #12631 fixed - A "see also" link has been added from progressionbar to waitbar
                    and vice versa.

* Bug #12634 fixed - ATOMS modules could not be installed from an archive file
                     already existing in SCI/contrib/archives in "offline" mode.

* Bug #12637 fixed - In some help pages, some signal processing functions were not in
                    the correct section.

* Bug #12639 fixed - justify([], position) returned an error instead of [].

* Bug #12641 fixed - graypolarplot has been fully vectorized as it was too slow.

* Bug #12657 fixed - Computation of v1.^v2 is now done without any memory allocation,
                     when v1 and v2 are real arrays, v1 >= 0 and v2 integer.

* Bug #12668 fixed - Undocking SciNotes led to an exception.

* Bug #12672 fixed - Ticks part of axes_property help page updated.

* Bug #12673 fixed - Ticks were drawn outside of axes area.

* Bug #12678 fixed - nthroot now accepts vector/matrix as second argument.

* Bug #12679 fixed - Argument type check added in gcd and lcm.

* Bug #12682 fixed - Key events were disabled after zooming.

* Bug #12683 fixed - proc_name(k, gwin) callback was badly managed depending on
                     input arguments of addmenu function.

* Bug #12686 fixed - Error returned by diff fixed.

* Bug #12702 fixed - When no extra parameters were needed in the cost function,
                    NDcost did not work.

* Bug #12703 fixed - In SciNotes, common shortcuts 'SHIFT DELETE', 'SHIFT INSERT'
                    (and others) added.

* Bug #12705 fixed - members function added. It allows to find the number of occurrences
                    and linear indexes for common values between two matrices of the

* Bug #12706 fixed - A wrong size of a matrix as input argument of cross function was
                    not detected.

* Bug #12708 fixed - Incorrect display in SciNotes preferences fixed (onmouseover styles).

* Bug #12712 fixed - In SciNotes, autosaving can now use %date to append the current
                    date to the backup filename.

* Bug #12714 fixed - csvDefault("decimal", ",") returned %f while this value was valid.

* Bug #12715 fixed - Variable cross in pspect and cspect has been renamed because of
                    conflicts with cross function.

* Bug #12716 fixed - In SciNotes, RTL languages were not correctly displayed.

* Bug #12725 fixed - Datatips did not work in logarithmic scale.

* Bug #12733 fixed - There was no way to direct graphs to nothing with driver function.

* Bug #12736 fixed - In SciNotes, the Completion window appeared only in first tab.

* Bug #12737 fixed - In SciNotes, autosave did not create directory if it did not exist.

* Bug #12747 fixed - legendre now accepts the -1 and 1 values for third argument.

* Bug #12749 fixed - fscanfMat help page updated.

* Bug #12758 fixed - Focus issue with plot3d fixed.

* Bug #12761 fixed - The https:// protocol was not supported on ATOMS.

* Bug #12763 fixed - Value of "listbox" style uicontrols was not updated when using arrow keys.

* Bug #12769 fixed - xset("window", 1) did not set the current axes.

* Bug #12772 fixed - eigs failed when trying to solve a sparse matrix eigen value problem.

* Bug #12774 fixed - Various typos fixed.

* Bug #12775 fixed - Some related functions were not listed in "See also" section of
                    routh_t help page.

* Bug #12778 fixed - Insertion of an empty matrix in an integer matrix led to a
                    wrong result.

* Bug #12779 fixed - savewave had a miscoding in the internal function write_wavedat.

* Bug #12783 fixed - There were some inconsistent error messages in dsearch.

* Bug #12784 fixed - Misleading error message in many functions when passing an integer
                     argument instead of double argument fixed.

* Bug #12785 fixed - plot did not allow int data as first argument.

* Bug #12790 fixed - Links to ZCOS files in documentation were broken.

* Bug #12791 fixed - More information is now given in case of failure during the ATOMS
                    autoload step.

* Bug #12793 fixed - Improved the bode() plots with a new option "rad" to convert plot into rad/s
                     and a new function bode_asymp() to draw the system asymptotes.

* Bug #12794 fixed - calfrq.sci code did not follow Scilab standard.

* Bug #12795 fixed - Typos fixed in CACSD help page.

* Bug #12800 fixed - Typo fixed in Polynomials help page.

* Bug #12803 fixed - warning(['foo','bar']) printed two 'WARNING: '.

* Bug #12804 fixed - Typos fixed in routh_t help page.

* Bug #12807 fixed - Display of showprofile improved.

* Bug #12808 fixed - Add missing </td> in documentation generation (note, warning, ...).

* Bug #12813 fixed - flipdim function extended to any type of input data.

* Bug #12814 fixed - Improvements of pertrans help page.

* Bug #12815 fixed - levin redefined cov as a variable.
                    menu.

* Bug #12816 fixed - Numbers pasted in editvar were not parsed according to locale.

* Bug #12818 fixed - Segfault in set function with invalid property values dimension.

* Bug #12819 fixed - Link to contributors website page fixed in the documentation.

* Bug #12823 fixed - In help generation (toolbox) links were not correctly handled.

* Bug #12826 fixed - <warning> and <note> tags were not managed in the documentation.

* Bug #12827 fixed - noisegen help page improved.

* Bug #12828 fixed - routh_t gave a wrong result if the first element of a row was zero.

* Bug #12829 fixed - New optional output argument added for routh_t function.

* Bug #12830 fixed - In SciNotes, it was not possible to execute a replace action
                    from the caret position.

* Bug #12831 fixed - In SciNotes toolbar, there was no button to open code navigator.

* Bug #12833 fixed - In SciNotes, there was no autoscroll when searching a pattern.

* Bug #12836 fixed - Error fixed in strcmpi help page.

* Bug #12839 fixed - Typo fixed in getVariablesOnStack help page.

* Bug #12840 fixed - Typo fixed in number_properties.xml help page.

* Bug #12852 fixed - Visual Studio 2012 SDK configuration was incorrect.

* Bug #12854 fixed - configure failed to detect custom installation of docbook.

* Bug #12858 fixed - Typo fixed in debug and pause French help pages.

* Bug #12859 fixed - Fixed optional argument in isdef help page.

* Bug #12860 fixed - Missing semicolon in style.css added.

* Bug #12863 fixed - size(state-space, "r") returned an error.

* Bug #12875 fixed - phasemag returned an error for input vector containing zeros.

* Bug #12880 fixed - A warning is displayed when transposing arguments of plot.

* Bug #12882 fixed - Some help pages were not clear.

* Bug #12887 fixed - Scilab hung with auto_clear set to on and log_flags set to true.

* Bug #12888 fixed - sysdiag was not documented about block diagonal matrices build.

* Bug #12896 fixed - Typos fixed in XML module error messages.

* Bug #12900 fixed - It is now possible to set proxy options in Preferences.

* Bug #12906 fixed - champ and champ1 help pages updated

* Bug #12909 fixed - Completion on (mt)list led to a crash.

* Bug #12910 fixed - Typos fixed in several help pages.

* Bug #12911 fixed - Matlab to Scilab dictionary help page updated for eig.

* Bug #12913 fixed - linspace returned an error if the third argument was an integer type
                    variable.

* Bug #12916 fixed - power help page improved.

* Bug #12919 fixed - Rotation on plots was disabled after using menus.

* Bug #12927 fixed - ones function could not take integer type input.

* Bug #12931 fixed - ATOMS redefined "message" keyword.

* Bug #12938 fixed - No Java compiler was available in Linux binary version.

* Bug #12943 fixed - Datatips did not work properly when 2-D plots were rotated.

* Bug #12945 fixed - Datatips could not be dragged properly in zoomed axes.

* Bug #12948 fixed - When host was not found, getURL caused a crash to desktop.

* Bug #12950 fixed - getURL ignored the proxy settings under Windows.

* Bug #12951 fixed - Interactive zoom was broken.

* Bug #12952 fixed - It was not always possible to search a word in a file with SciNotes.

* Bug #12956 fixed - splitURL with no protocol in URL provoked an access
                     violation exception.

* Bug #12957 fixed - splitURL and getURL were not declared as new 5.5 functions.

* Bug #12962 fixed - xinfo documentation was not clear.

* Bug #12963 fixed - drawaxis did not place the axis properly.

* Bug #12966 fixed - Rotation, tool tip and other Xcos plot actions were
                     disabled by default.

* Bug #12967 fixed - Data editor icon was not 16x16.

* Bug #12968 fixed - A variable named 'temp' could not be saved.

* Bug #12971 fixed - getURL downloaded file name was wrong.

* Bug #12973 fixed - Exception occurring when clicking on a figure at creation fixed.

* Bug #12976 fixed - getURL returned a file name instead of a file path.

* Bug #12978 fixed - exportUI returned an error when input argument was a figure handle.

* Bug #12979 fixed - exportUI did not work with vectorial export.

* Bug #12990 fixed - GED features conflicted with figure events.

* Bug #12992 fixed - The sigma value was always equal to "LM" in eigs.

* Bug #12993 fixed - stdev returned value depended on 'x' being defined in the environment.

* Bug #13000 fixed - []./int8(3) and on int8(3)./[] led to an endless recursive.

* Bug #13002 fixed - modulo and pmodulo did not support integers & hypermatrices.

* Bug #13003 fixed - String to enum converter added to Java external objects.

* Bug #13004 fixed - Debug infos were displayed in case of an error with eoj.

* Bug #13005 fixed - jcompile did not use the classpath to compile.

* Bug #13007 fixed - Compilation errors were not returned by jcompile.

* Bug #13008 fixed - 'help $' now opens the 'Symbols' help page.

* Bug #13010 fixed - Wrong class was returned by jcompile (with ecj).

* Bug #13011 fixed - ilib_compile failed under Mac OS X 10.9.

* Bug #13012 fixed - Vectorial export of rotated strings was wrong.

* Bug #13013 fixed - In SciNotes, the first proposed directory to save a file was not current working directory.

* Bug #13014 fixed - Update of the efficiency inner variable improved in optim_ga.

* Bug #13015 fixed - Computation of efficiency inner variable improved in optim_ga.

* Bug #13022 fixed - Vectorial export did not clip large segments.

* Bug #13023 fixed - xs2pdf crashed under Windows when the target file was already opened.
                    (and others) added.

* Bug #13027 fixed - There was no autowrap into array in JIMS.

* Bug #13031 fixed - Lorentz Butterfly demonstration fixed.

* Bug #13032 fixed - CMATVIEW help page example fixed and CMAT3D help page example created.

* Bug #13033 fixed - -1 could not be used as nax argument in plot2d.

* Bug #13036 fixed - The help page associated to the history browser was wrong.

* Bug #13041 fixed - Wrong result was returned for "integer scalar" minus "integer vector".

* Bug #13042 fixed - Texts in graphics were not properly centered.

* Bug #13047 fixed - jcompile did not allow class reloading.

* Bug #13049 fixed - New handled protocols added in links in SciNotes.

* Bug #13050 fixed - The result of mvvacov was not symmetric.

* Bug #13051 fixed - SciNotes restoration could block desktop one.

* Bug #13053 fixed - datatipCreate did not return datatip handle.

* Bug #13055 fixed - Array indexing did not follow Scilab convention in JIMS.

* Bug #13061 fixed - hdf5 demonstration failed with a read-only file.

* Bug #13063 fixed - Color selection in SciNotes preferences threw exceptions.

* Bug #13064 fixed - Deleting a link connected to a split led to a translated link.

* Bug #13066 fixed - data_bounds was not fully documented in axes_properties.

* Bug #13069 fixed - Documentation for %MODELICA_USER_LIBS updated.

* Bug #13080 fixed - Contextual menu was unavailable for datatips management.

* Bug #13082 fixed - Datatip mark style was not inherited from the parent polyline.

* Bug #13083 fixed - Datatip marks did not inherit colors from the parent polyline.

* Bug #13084 fixed - interp_mode property could not be set on all datatips of the same polyline.

* Bug #13085 fixed - A datatip could not be moved on a circle.

* Bug #13092 fixed - Optimizations now check the user function output (must be real).

* Bug #13093 fixed - Removed trailing "\n" in the head_comments error message.

* Bug #13101 fixed - When x-axis was in reverse position x_location='origin' failed.

* Bug #13102 fixed - savematfile did not support "-v7.3" option.

* Bug #13108 fixed - Time between ATOMS database updates is now a month (was a day) and is configurable.

* Bug #13109 fixed - pol2str now supports polynomials with complex coefficients and hypermatrices.

* Bug #13111 fixed - sqrt returned different results when imaginary part was -0 versus 0.

* Bug #13114 fixed - clear_pixmap/pixmap property should have been removed in Scilab 5.4.1.

* Bug #13116 fixed - qpsolve now respects upper-bounds constraints.

* Bug #13119 fixed - mget and mgetl now return an error when called with decimal values as number of lines.

* Bug #13121 fixed - ode "rk" option crashed Scilab when the user derivative function failed.

* Bug #13127 fixed - There were no subticks with user defined ticks.

* Bug #13132 fixed - There were missing graduations when data_bounds interval was too small.

* Bug #13134 fixed - User-defined ticks in log scale were invisible.

* Bug #13136 fixed - exists and isdef failed for input arguments longer than 1.

* Bug #13139 fixed - fft help page fixed.

* Bug #13140 fixed - Various typos fixed in help pages.

* Bug #13144 fixed - csvRead can now ignore header comments thanks to a new input argument.

* Bug #13146 fixed - profile failed when a comment was on the same line as a function declaration.

* Bug #13150 fixed - Vectorial export used too much memory for grayplot.

* Bug #13152 fixed - Typo fixed in syslin French help page.

* Bug #13164 fixed - Miscolorization in SciNotes colors preferences fixed.

* Bug #13165 fixed - Preferences reset did not show a confirmation pop-up.

* Bug #13168 fixed - Wrong interpretation of star followed by minus fixed.

* Bug #13170 fixed - Legends for plzr plots fixed.

* Bug #13174 fixed - Scilab crashed after XML element removal.

* Bug #13175 fixed - argn help page updated with specific cases.

* Bug #13177 fixed - Error when changing x_ticks.locations on axes fixed.

* Bug #13185 fixed - When the "checked" option of a uimenu was set to "on" for
                     the first time, the display order of other uimenus was changed.

* Bug #13186 fixed - csvRead freezed Scilab if separator was an empty string.

* Bug #13187 fixed - xmltoformat did not include "imageobjects" in output.

* Bug #13188 fixed - The output argument of eomday function was not pre-dimensioned.

* Bug #13191 fixed - isempty(rational) returned an error message.

* Bug #13192 fixed - horner returned an error message when the input arguments did not have the same size.

* Bug #13194 fixed - part help page improved.

* Bug #13199 fixed - There was a thin blue line around acknowledgements button in about box.

* Bug #13200 fixed - about() ACKNOWLEDGEMENTS did not render utf-8 characters.

* Bug #13201 fixed - x_mdialog entries had no margin.

* Bug #13202 fixed - kernel help page improved.

* Bug #13203 fixed - Typos fixed in some error messages.

* Bug #13205 fixed - group accepted continuous transfer functions.

* Bug #13208 fixed - New nanreglin function to handle NaNs for reglin().

* Bug #13210 fixed - Incorrect argument description in mseek help page.

* Bug #13213 fixed - User-defined margins were reset by auto computation of the margins.

* Bug #13215 fixed - clf(1001) returned an error.

* Bug #13218 fixed - Typos fixed in core module help pages.

* Bug #13226 fixed - Completion with accented chars could led to a crash.

* Bug #13227 fixed - Invalid windowsConfiguration.xml file could avoid Scilab startup.

* Bug #13233 fixed - Wrong simulation result of step response csim('step',t,H) fixed.

* Bug #13234 fixed - lmitool calling sequence clarified.

* Bug #13236 fixed - "parents" help page has been renamed to "parentheses".

* Bug #13238 fixed - Wrong legends display fixed.

* Bug #13243 fixed - optim with "gc" option failed when imp<0.

* Bug #13247 fixed - Hypermatrix in structure definition did not work properly.

* Bug #13252 fixed - Minor typos fixed in Graphics messages.

* Bug #13258 fixed - Bad number display in datatips fixed.

* Bug #13267 fixed - Implicit typecasts in scicos.c fixed.

* Bug #13271 fixed - plot2d with logarithmic scale and %nan value froze Scilab.

* Bug #13272 fixed - Error occurring when reading CSV files with comment option on a CSV file without comment fixed.

* Bug #13280 fixed - Axes were not always displayed properly after figure resize.


Xcos Bug Fixes
===============

* Bug #7350 fixed - The I/O ports numbering of a superblock was not updated
                    when a new port was dropped.

* Bug #8570 fixed - "Region to superblock" has been renamed to
                    "Selection to superblock".

* Bug #9995 fixed - LOGICAL_OP drew the parameters over the block.

* Bug #11518 fixed - CLR block-text was displayed out of the bounding box
                     when zooming.

* Bug #11776 fixed - CMSCOPE did not take into account label&Id parameter.

* Bug #11975 fixed - Inverted Pendulum demonstration did not compile.

* Bug #12359 fixed - Xcos files have been converted to ZCOS to gain some space.

* Bug #12384 fixed - Using a Modelica part linked with an explicit link to
                     another Modelica part led to an algebraic loop error.

* Bug #12387 fixed - The "Modelica initialize" setup menu option did not blur
                     during simulation.

* Bug #12414 fixed - SWITCH_m block had different behaviors
                     for different types of inputs.

* Bug #12423 fixed - Data types of SWITCH2_m were not documented.

* Bug #12424 fixed - Calling lincos on a derivative block made Scilab crash.

* Bug #12449 fixed - QUANT_f was not rounding/truncating/flooring/ceiling properly input signal.

* Bug #12460 fixed - xcosPalGenerateAllIcons sometimes crashed Scilab.

* Bug #12461 fixed - Cancelling zoom out by zooming in did not fully work.

* Bug #12561 fixed - SELECT_m and RELAY_f did not behave as expected.

* Bug #12568 fixed - "Recent files" menu entry is now below the "Open" one.

* Bug #12590 fixed - Block shape style was removed on file loading.

* Bug #12603 fixed - ZCOS files could contain blocks with modified (eg. invalid)
                     parameters.

* Bug #12619 fixed - DLR discrete block did not display LaTeX formula
                     like CLR continuous block.

* Bug #12651 fixed - The 'nw' scicos_simulate option did not work while
                     passing a previous Info simulation status.

* Bug #12664 fixed - Inverted pendulum French localization in the Xcos demonstrations was
                    inaccurate.

* Bug #12667 fixed - 'Recent Files' menu was not localized.

* Bug #12685 fixed - The lincos and steadycos functions did not load
                     the XcosLibs so all the blocks were unknown.

* Bug #12731 fixed - Code generation produced erroneous block.

* Bug #12732 fixed - Improper Copyright comments in the files were generated by the code
                    generation tool.

* Bug #12796 fixed - There was some mismatches between implicit
                     and explicit ports of superblocks.

* Bug #12797 fixed - I/O blocks generated by "Selection to Superblocks" had
                     wrong size.

* Bug #12868 fixed - There were several problems with PULSE_SC block.

* Bug #12869 fixed - min and max were not defined but used in Xcos generated code.

* Bug #12873 fixed - scicos_flat produced an unexpected error after a diagram
                     load.

* Bug #12874 fixed - CSCOPE marks sizes were too small.

* Bug #12877 fixed - Incorrect output port dimensions and types in GENSIN_f, GENSQR_f,
                    STEP_FUNCTION and STEP blocks fixed.

* Bug #12924 fixed - Blocks type 2004 was not handled as a valid block type.

* Bug #12934 fixed - Separate compilation of a superblock could fail due to under
                    determined signal sizes.
* Bug #12998 fixed - 'ans' was not ignored in the context results.

* Bug #13006 fixed - Selection to superblock (in_f and out_f) failed.

* Bug #13030 fixed - Selection to superblock did not reset the origin.

* Bug #13059 fixed - NaN propagated at startup made the simulation fail.

* Bug #13071 fixed - Three unused functions in Xcos macros now specified as not mandatory
                     to write new blocks.

* Bug #13172 fixed - Logic block help file had some typo errors.

* Bug #13239 fixed - Grid was not shown in zoomed log scale.

* Bug #13250 fixed - CLSS wrongly handled scalar values.



                 Changes between version 5.5.0-beta-1 and 5.5.0
                 ==============================================

GUI Refactoring and Improvements
================================

* New uicontrols styles:
 - tab: component which enables the user to switch between sets of uicontrols by
        clicking on a tab.
        The children components are "frame" uicontrols.
        Two dedicated properties have been added to configure this component:
         - title_position: Position of the tabs
         - title_scroll: Indicates if tabs must all be displayed at a time or
                         managed with scroll features.
 - layer: Component which enables the user to make parts of a GUI visible/invisible
          programmatically.
 - spinner: Component which enables the user to select/edit a value between bounds
            with a fixed step.

* New uicontrols properties:
 - border: Used to set some decoration properties on "frame" uicontrols.
            These decorations can be created and initialized with the createBorder
            and the createBorderFont functions.
 - scrollable: Used to add scrolling capabilities on "frame" uicontrols.
 - groupname: Used to group "radiobutton" and "checkbox" uicontrols for an easier
              management.
 - icon: Add an icon to "pushbutton", "text" and "frame" uicontrols.
 - margin: Empty space around uicontrols.

* New uimenu properties:
 - icon: add an icon on the left of the menu label.

* "listbox" and "popupmenu" style uicontrols can now manage colors selection, icons, background
  and foreground colors when the "String" property is set to a matrix matching the format:
  ["#color1", "Item1", "#background1", "#foreground1"; "#color2", "Item2", ..., ...]
  ["icon1", "Item1", "#background1", "#foreground1"; "icon2", "Item2", ..., ...]
  with "#color1", "#background1" and "#foreground1" in HTML format #XXXXXX.
  Then the component will display a colored box or icon on the left of the associated string,
  and different background/foreground colors for items.

* New management of uicontrols positioning:
  In previous versions, uicontrols position was managed in an absolute way through
  their "Position" property and the "Resizefcn" property of their parent figure.
  Using the new layout management in figures and "frame" style uicontrols, position
  is now managed in an automatic way based on Java layouts.
  New dedicated properties have been added in figure and uicontrols:
   - layout: layout type.
   - layout_options: configuration of the layout.
                     Options can be initialized with the createLayoutOptions function.
  Type "help layouts" in Scilab for more information about available types and options.
  Uicontrols position is then managed through the "constraints" property.
  A new createConstraints function has been added to managed these contraints.

* New figures properties:
 - icon: Allows to customize the figure icon.
 - menubar: allows to create windows without any menu bar (default menus will not be created).
 - menubar_visible: Manages menu bar visibility.
 - toolbar: Allows to create windows without any toolbar.
 - toolbar_visible: Manages toolbar visibility.
 - infobar_visible: Manages infobar visibility.
 - resize: Allows to lock window size.
 - dockable: Allows to create dockable/standard figures.
 - default_axes: Allows to manage default axes creation in figures.

* The figure "visible" property management has evolved and offers new possibilities:
 - When the figure is docked, this property manages the visibility
   of components inside the figure (uicontrols, axes, ...).
   In previous releases, this same property setting only managed axes.
 - When the figure is not docked, this property manages the visibility
   of the whole figure including its decorations.

* New way to create GUI using XML files:
 - Created GUI using the figure/uicontrol/uimenu functions can now be saved in
   this new format using saveGui function.
 - XML files in this format can be loaded in Scilab using the loadGui function.
   This function aims at creating GUI in a more efficient way.

* New rendering for GUI/uicontrols:
  In this version, uicontrols use the defaults of the OS Look & Feel.
  Some properties default values are not set by Scilab and then can be different from an
  OS to another.
  To come back to the previous and deprecated behavior, you can use the related
  property on the "Console" handle:
  set(get(0), "UseDeprecatedSkin", "on");
  Note that this deprecated behavior will be removed in future versions.

* New way to access uicontrols using a path containing their "Tag" and their parent(s) "Tag" property.
  See the set and get functions help page for more details.

* New display for uicontrols handles properties. Only properties used for Java rendering are displayed.
  To display all available properties, use the "ShowHiddenProperties" "Console" property.


Graphics Evolutions
===================

* set function prototype has been modified to allow the user to set multiple
  properties at once: set(h, "Property1", Value1, "Property2", Value2, ...)

* newaxes function now allows to create axes in "frame" uicontrols.

* clf function now also works with "frame" uicontrols.

* New axes properties:
 - ticks_format: Format of the ticks labels.
 - ticks_st: Scales and translates factors applied to ticks position when formatting
             the ticks labels.
 - auto_margins: Activated by default, this property lets Scilab compute margins
                 needed to display axes decorations (titles, labels, ...).
 - grid_thickness: Thickness of the grid plotting.
 - grid_style: Style of the grid plotting.
 - label_font_style: Font style used to draw the labels.

* New legends properties:
 - line_width: Width of the drawn line.
 - mark_count: Number of the drawn marks.

* New polyline properties:
 - mark_offset: Offset before the first mark.
 - mark_stride: Step between consecutive marks.
 - display_function: Name of the function used to customize the information
                     displayed in all the datatips of this polyline.
                     This function can be overloaded by setting a display
                     function on each datatip handle.
 - display_function_data: Additional data needed for datatips display function.
 - datatips: Handles of the datatips of the polyline.

* New Matplot property added :
 - rect: specifies the rectangle where the Matplot will be drawn.

* Datatips properties have been renamed for better readability:
 - tip_data --> data
 - tip_box_mode --> box_mode
 - tip_label_mode --> label_mode
 - tip_orientation --> orientation
 - tip_3component --> z_component
 - tip_auto_orientation --> auto_orientation
 - tip_interp_mode --> interp_mode
 - tip_disp_function --> display_function


Scilab
======

* New functions introduced:
 - jcreatejar - Creates a Java archive (JAR) from a set of files / directories
 - ilib_build_jar - Builds Java packages from sources into a JAR file
 - ifftshift - Inverses FFT shift
 - unwrap - Unwraps/unfolds a Y(x) profile
 - getPreferencesValue/setPreferencesValue - Get or set values in the preferences file.
 - htmlDump/htmlRead/htmlReadStr/htmlWrite - Read and write data from/in HTML files.
 - numderivative - Approximate derivatives of a function (Jacobian or Hessian).

* modulo and pmodulo now support integers & hypermatrices (See bug #13002).

* test_run can now separate 32-bit systems from 64-bit ones.

* pol2str: now handles polynomials with complex coefficients and hypermatrices (See bug #13109).

* nanreglin: reglin with arguments containing NaNs (See bug #13208).

* New optional input argument added to matfile2sci (append or write the output file).


Xcos
====

* xcos_debug_gui function added for a simple use of the Debug block.

* tbx_build_pal_loader function added to ease external module creation (See SEP #120).

* Event management demonstrations added.

* API change: In scicos_block4.h, "Residute" renamed to "Jacobian".


Obsolete & Removed Functions
============================

* relocate_handle tagged as obsolete. Will be removed in Scilab 5.5.1.

* msd and st_deviation tagged as obsolete (See bug #7593). Will be removed in Scilab 5.5.1.
  Please use stdev instead.

* pcg tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use conjgrad instead.

* milk_drop tagged as obsolete. Will be removed in Scilab 5.5.1.

* datatipGetStruct tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use 'datatips' property instead.

* regress tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use reglin instead.

* %asn tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use delip instead.

* jmat tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use flipdim instead.

* perl tagged as obsolete. Will be removed in Scilab 6.0.0.

* numdiff and derivative tagged as obsolete. Will be removed in Scilab 6.0.0.
  Please use numderivative instead.

* xmltochm tagged as obsolete. Will be removed in Scilab 5.5.1.

* clear_pixmap removed, please use drawlater/drawnow instead.

* pixmap property removed for figures, please use drawlater/drawnow instead.

* demo_message and demo_mdialog removed.

* with_embedded_jre removed.

* draw removed.

* fit_dat removed. Please use datafit instead.

* winclose removed. Please use close instead.

* datatipInitStruct and datatipRedraw removed.

* getfont, getmark, getlinestyle and getsymbol removed. Please use ged instead.


Compilation
============

* Minimal version of JoGL (2.1.4) enforced.

* Minimal version of JLaTeXMath (1.0.3) enforced.

* New dependencies to manage EMF export:
 - freehep-graphics2d.jar
 - freehep-graphicsio-emf.jar
 - freehep-graphicsio.jar
 - freehep-io.jar
 - freehep-util.jar
 Note: --without-emf can be used to disable this feature.


Known incompatibilities
========================

* Using => or =< now returns an error. See bug #2345.

* or/and functions now fix reals value before working on them. See bug #7666.

* a*b*-c+d is now interpreted as (a*b*(-c))+d instead of a*b*(-c+d). See bug #13168.

* Extracting children from an empty matrix of handles now returns an error.
  For example: f=gcf(); f.children(1).children.children returns an error.

* Figure anti_aliasing property behavior changed:
  - "off": No anti-aliasing activated on the figure.
  - "2x", "4x", "8x", "16x": anti-aliasing activation will have no effect on most plateforms.


Scilab Bug Fixes
================

* Bug #1253 fixed - There was no possibility to draw only few marks on a polyline which
                    contained a lot of points.

* Bug #1751 fixed - Margins were not computed according to contents.

* Bug #2067 fixed - Scilab crashed when plot was called with a large numerical value.

* Bug #2345 fixed - Use of operators => and =< did not lead to an error.

* Bug #2802 fixed - convstr did not convert non ascii chars.

* Bug #4083 fixed - The numdiff and derivative functions were duplicates and are
                    now tagged as obsolete and replaced by numderivative. See SEP #130.

* Bug #4177 fixed - find function moved to the "elementary_functions" module.

* Bug #4401 fixed - isnum did not recognize all constants or some complex numbers.

* Bug #4490 fixed - Input argument of sinc function moved to rad.

* Bug #4649 fixed - License issue in xs2ppm help page fixed.

* Bug #4677 fixed - xclick did not return correct mouse position on keyboard event.

* Bug #4692 fixed - Export did not work while xgetmouse was waiting.

* Bug #4858 fixed - libintl.h was missing in binary versions but included in localization.h.

* Bug #4965 fixed - Setting links property for a handle of type legend did not work.

* Bug #5016 fixed - condestsp could return different results when repeated calls were performed.

* Bug #5232 fixed - 'frame' uicontrols children now follow the frame displacement.

* Bug #5826 fixed - Graphic windows did not redirect key events in console.

* Bug #5844 fixed - grayplot did not have a logflag option.

* Bug #5886 fixed - There was no property labels_font_style on axis.

* Bug #6191 fixed - It was not possible to set thickness and line_style properties for a grid.

* Bug #6305 fixed - dsearch function did not work with integers, strings and hypermatrices.

* Bug #6306 fixed - It is now possible to compute an histogram instead of plotting it
                    with new function histc(). Besides, histplot can now return the computed data.

* Bug #6363 fixed - It was not possible to create figures without menubar/toolbar.

* Bug #6404 fixed - xrects help page was not clear about argument specification.

* Bug #6476 fixed - Matplot help did not indicate that the data field should be used to update data.

* Bug #6512 fixed - %asn function tagged as obsolete.

* Bug #6615 fixed - ui(get|put)file did not center the file dialog on the last focused window.

* Bug #6689 & #6690 fixed - grand now works with complexes, polynomials, booleans, integers, sparses and strings,
                            and can handle row vectors, matrices and hypermatrices of thoses types.

* Bug #6824 fixed - resize_matrix did not manage hypermatrices.

* Bug #6832 fixed - Matrices of rationals can now be transposed via the " .' " operator.

* Bug #6859 fixed - xlabel and xtitle could overlapped.

* Bug #6930 fixed - Matplot handle had no rect property.

* Bug #7038 fixed - A toggle button now manages datatip mode.

* Bug #7040 fixed - Example and description for getMatrixOfIntegerPrecision modified in help page.

* Bug #7047 fixed - milk_drop is now obsolete. It will be removed in Scilab 5.5.1, but will be kept as a demonstration.

* Bug #7051 fixed - fieldnames help page updated.

* Bug #7084 fixed - Old, not documented and deprecated gr_menu function removed.

* Bug #7085 fixed - 'edit' uicontrols did not support multiple line edition.

* Bug #7133 fixed - help_from_sci with no input argument now launches a full demonstration.

* Bug #7169 fixed - It was not possible to specify different option value (thickness, style, ...) for X and Y grids.

* Bug #7205 fixed - length(H) applied to a non-string hypermat returned 3 instead of size(H,"*").

* Bug #7244 fixed - Extraction from a struct array with a boolean vector had a strange behavior.

* Bug #7304 fixed - exportUI did not switch filename extension when filter was changed.

* Bug #7417 fixed - Auto-positioning of datatips did not take the curvature into account.

* Bug #7561 fixed - roots help page now explains that coefficients are used in the contrary order of poly.

* Bug #7570 fixed - The switch criterion on x and y is now explained in beta() help page.

* Bug #7585 fixed - psi.f moved from "elementary functions" to "special functions" module.

* Bug #7593 fixed - stdev now encompasses msd and st_deviation thanks to a new optional input argument.

* Bug #7650 fixed - isempty(tlist(...)) always returned false, even when all defined fields were empty.

* Bug #7666 fixed - Inconsistencies between and/or and &/| fixed.

* Bug #7705 fixed - "dimension", "minbounds" and "maxbounds" fields have been documented in Genetic algorithms help pages.

* Bug #7739 fixed - Axis position was invalid in log modes.

* Bug #7771 fixed - There was no item about arrow_size in champ_properties help page.

* Bug #7772 fixed - There was no description, no example about line_style property in polyline help page.

* Bug #7858 fixed - variance and variancef can now return the mean of the input
                    in a new output argument and take the a priori mean as input.

* Bug #7879 fixed - string now accepts plist type, and printing a plist displays that string.

* Bug #7905 fixed - Figure icon can now be changed using dedicated property.

* Bug #7916 fixed - nansum([]) returned NaN value while this function had to ignore it.

* Bug #7986 fixed - spec gateway renamed from sci_eig.c to sci_spec.c.

* Bug #7999 fixed - SwingScilabFileChooser.getFilterIndex() was unusable.

* Bug #8031 fixed - cdfgam error message fixed.

* Bug #8060 fixed - Improved list display in the variable browser.

* Bug #8131 fixed - It was not possible to choose the number of marks and the line width in legends.

* Bug #8133 fixed - Ticks disappeared in planar 3-D view.

* Bug #8196 fixed - Error messages dealing with negative thickness were not standard.

* Bug #8231 fixed - xrect help page did not say that clipping property was inherited.

* Bug #8247 fixed - regress tagged as obsolete.

* Bug #8290 fixed - DELAYV_f block documentation fixed.

* Bug #8323 fixed - Scilab "About Box" did not hide Scilab main window.

* Bug #8337 fixed - mtlb_rand now uses the "uniform" rule, whatever the random rule set is.

* Bug #8379 fixed - It was not possible to delete the selected datatip with DELETE or BACKSPACE.

* Bug #8745 fixed - Extracting from an empty matrix automatically returned an empty matrix.

* Bug #8956 fixed - xpoly, xfpoly, xrect, xsegs and xarc did not update data_bounds property.

* Bug #9031 fixed - Misalignment of text when using xstring with a matrix fixed.

* Bug #9033 fixed - auto_dimensioning property for text handles was not documented.

* Bug #9110 fixed - Examples and references to other functions added in Statistics help pages.

* Bug #9309 fixed - comparison help page updated to document issue with empty matrix.

* Bug #9319 fixed - Huge polylines could not be exported in PS/EPS.

* Bug #9444 fixed - with_embedded_jre function removed.

* Bug #9493 fixed - Title is now correctly set when starting Scilab and focus is set on Console.

* Bug #9627 fixed - Arguments checking added in optimsimplex_* functions.

* Bug #9697 fixed - Displayed information for optim "qn" and "gc" with bounds and imp=1 fixed.

* Bug #9701 fixed - optim with "qn" option was failing for large problems.

* Bug #9819 fixed - unwrap function did not exist in Scilab.

* Bug #9840 fixed - Default G tolerance in lsqrsolve was too large.

* Bug #10012 fixed - lmisolver and lmitool called with no input now produce errors.

* Bug #10083 fixed - plot3d could not be used with only one input argument.

* Bug #10122 fixed - replot could not be used in 3-D.

* Bug #10175 fixed - Clearer example added for sp2adj to adj2sp conversion (and backwards conversion).

* Bug #10214 fixed - evstr help page updated to mention that input argument must not be composed of
                     continuation marks (..).

* Bug #10221 fixed - ifftshift function did not exist in Scilab.

* Bug #10234 fixed - reglin function moved from CACSD to Statistics module.

* Bug #10243 fixed - fun2string(X) called X before returning its code.

* Bug #10271 fixed - ordmmd now checks the consistency of the third input argument with the
                     input matrix defined by the first two input arguments.

* Bug #10273 fixed - spchol help page now displays an example showing how to use its output arguments.

* Bug #10391 fixed - Error when using completion after a global variable clear fixed.

* Bug #10428 fixed - Java based components of Scilab showed a library load error in CLI mode.

* Bug #10445 fixed - In SciNotes, CTRL+Drag&Drop moved text rather than copying it.

* Bug #10470 fixed - In SciNotes, split "Horizontally" or "Vertically" was meaningless.

* Bug #10621 fixed - Figure without docking/undocking capabilities can now be created.

* Bug #10645 fixed - File encoding could not be given as argument in xmlRead.

* Bug #10718 fixed - New "Resize" property added to figures.

* Bug #10805 fixed - Documentation on left bracket was superfluous and is now removed.
                     Refer to brackets for information on "[" and "]".

* Bug #10816 fixed - Typo in part error message fixed.

* Bug #10830 fixed - Hypermatrix insertion with a negative index returned a wrong error message.

* Bug #10833 fixed - exists help page updated.

* Bug #10840 fixed - Keyboard arrows were disabled on 'slider' type uicontrols.

* Bug #10856 fixed - analpf did not return the right result.

* Bug #10932 fixed - Startup directory was not saved/restored in preferences.

* Bug #10936 fixed - Scilab hung with invalid strf in plot2d.

* Bug #10942 fixed - Function soundsec revised. Now soundsec can be used for non integer values of time.

* Bug #10998 fixed - matrix*hypermatrix and hypermatrix*matrix operations failed.

* Bug #11001 fixed - exists and isdef did not work with primitives.

* Bug #11007, #11008 & #11009 fixed - New conjgrad function (Conjugate Gradient methods "pcg", "cgs", "bicg" and "bicgstab").

* Bug #11303 fixed - Exception while searching with multiple tabs in SciNotes fixed.

* Bug #11305 fixed - Performances improved with a better way to update data in Graphics.

* Bug #11523 fixed - In SciNotes, 'whereami line numbering' was not clear enough.

* Bug #11571 fixed - x_mdialog did not let the Look&Feel select the window size.

* Bug #11575 fixed - There was no preview of GIF files in exportUI dialog.

* Bug #11576 fixed - exportUI did not propose gcf().figure_name as default file name.

* Bug #11616 fixed - Figure menubar could not be made invisible.

* Bug #11629 fixed - Interactive zoom did not work properly in datatip mode.

* Bug #11680 fixed - GUI functions in Scilab 5.4.X were much slower than in Scilab 5.3.3.

* Bug #11714 fixed - help_from_sci sometimes failed when input function had "<imagedata>" comments.

* Bug #11779 fixed - Wrong variable type in the documentation of getNbInputArgument and getNbOutputArgument fixed.

* Bug #11789 fixed - Documentation was missing for nbInputArgument.

* Bug #11814 fixed - Typo in CACSD help chapter fixed.

* Bug #11876 fixed - ilib_include_flag now returns a string when called with a string column vector as input.

* Bug #11953 fixed - Scilab crashed when global("") was typed.

* Bug #11964 fixed - uicontrol coordinates system did not take figure resize into account.

* Bug #11996 fixed - eye extended to hypermatrix.

* Bug #12012 fixed - Misleading perl function should not be part of Scilab (tagged as obsolete).

* Bug #12037 fixed - Simplified Chinese version of SciNotes displayed warnings at startup.

* Bug #12073 fixed - Width of code examples has been decreased in XML help pages.

* Bug #12082 fixed - convstr function did not work with non-ASCII symbols.

* Bug #12121 fixed - inv function did not work for complex sparse matrices.

* Bug #12130 fixed - flipdim can now flip blocks, thus making jmat obsolete.

* Bug #12145 fixed - demo_mdialog internal function removed.

* Bug #12156 fixed - Closing a Scilab session in Javasci could led to a HDF5 error message.

* Bug #12170 fixed - Calling matfile_listvar on an empty file returned an error.

* Bug #12306 fixed - Invalid memory free on completion in NWNI mode fixed.

* Bug #12308 fixed - create_palette help page removed (function removed).

* Bug #12334 fixed - Mark color in legend was invalid.

* Bug #12412 fixed - Typo fixed in some error messages.

* Bug #12439 fixed - edit_error returned a wrong message when there was no recorded error.

* Bug #12440 fixed - Unitary test added for bitxor.

* Bug #12465 fixed - ATOMS Default categories names were not localized.

* Bug #12472 fixed - grand and link error messages fixed.

* Bug #12481 fixed - xlabel could not be used with Scilab property names.

* Bug #12485 fixed - xchange returned bad values with log scale.

* Bug #12492 fixed - Exported EPS files were invalid when dash patterns were too long.

* Bug #12496 fixed - zoom_rect could led to a crash in log scale.

* Bug #12535 fixed - In a French localized Scilab, comma was used as decimal separator when zooming (rather than point).

* Bug #12567 fixed - Error messages fixed in ricc.

* Bug #12622 fixed - Various typos fixed in error messages.

* Bug #12672 fixed - Ticks part of axes_property help page updated.

* Bug #12673 fixed - Ticks were drawn outside of axes area.

* Bug #12682 fixed - Key events were disabled after zooming.

* Bug #12683 fixed - proc_name(k, gwin) callback was badly managed depending on
                     input arguments of addmenu function.

* Bug #12714 fixed - csvDefault("decimal", ",") returned %f while this value was valid.

* Bug #12716 fixed - In SciNotes, RTL languages were not correctly displayed.

* Bug #12725 fixed - Datatips did not work in logarithmic scale.

* Bug #12733 fixed - There was no way to direct graphs to nothing with driver function.

* Bug #12737 fixed - In SciNotes, autosave did not create directory if it did not exist.

* Bug #12763 fixed - Value of "listbox" style uicontrols was not updated when using arrow keys.

* Bug #12769 fixed - xset("window", 1) did not set the current axes.

* Bug #12784 fixed - Misleading error message in many functions when passing an integer
                     argument instead of double argument fixed.

* Bug #12785 fixed - plot did not allow int data as first argument.

* Bug #12803 fixed - warning(['foo','bar']) printed two 'WARNING: '.

* Bug #12819 fixed - Link to contributors website page fixed in the documentation.

* Bug #12826 fixed - <warning> and <note> tags were not managed in the documentation.

* Bug #12854 fixed - configure failed to detect custom installation of docbook.

* Bug #12860 fixed - Missing semicolon in style.css added.

* Bug #12880 fixed - A warning is displayed when transposing arguments of plot.

* Bug #12882 fixed - Some help pages were not clear.

* Bug #12896 fixed - Typos fixed in XML module error messages.

* Bug #12900 fixed - It is now possible to set proxy options in Preferences.

* Bug #12910 fixed - Typos fixed in several help pages.

* Bug #12938 fixed - No Java compiler was available in Linux binary version.

* Bug #12943 fixed - Datatips did not work properly when 2-D plots were rotated.

* Bug #12945 fixed - Datatips could not be dragged properly in zoomed axes.

* Bug #12948 fixed - When host was not found, getURL caused a crash to desktop.

* Bug #12950 fixed - getURL ignored the proxy settings under Windows.

* Bug #12951 fixed - Interactive zoom was broken.

* Bug #12952 fixed - It was not always possible to search a word in a file with SciNotes.

* Bug #12956 fixed - splitURL with no protocol in URL provoked an access
                     violation exception.

* Bug #12957 fixed - splitURL and getURL were not declared as new 5.5 functions.

* Bug #12962 fixed - xinfo documentation was not clear.

* Bug #12967 fixed - Data editor icon was not 16x16.

* Bug #12963 fixed - drawaxis did not place the axis properly.

* Bug #12966 fixed - Rotation, tool tip and other Xcos plot actions were
                     disabled by default.

* Bug #12968 fixed - A variable named 'temp' could not be saved.

* Bug #12971 fixed - getURL downloaded file name was wrong.

* Bug #12973 fixed - Exception occurring when clicking on a figure at creation fixed.

* Bug #12976 fixed - getURL returned a file name instead of a file path.

* Bug #12978 fixed - exportUI returned an error when input argument was a figure handle.

* Bug #12979 fixed - exportUI did not work with vectorial export.

* Bug #12990 fixed - GED features conflicted with figure events.

* Bug #12992 fixed - The sigma value was always equal to "LM" in eigs.

* Bug #12993 fixed - stdev returned value depended on 'x' being defined in the environment.

* Bug #13000 fixed - []./int8(3) and on int8(3)./[] led to an endless recursive.

* Bug #13002 fixed - modulo and pmodulo did not support integers & hypermatrices.

* Bug #13003 fixed - String to enum converter added to Java external objects.

* Bug #13004 fixed - Debug infos were displayed in case of an error with eoj.

* Bug #13005 fixed - jcompile did not use the classpath to compile.

* Bug #13007 fixed - Compilation errors were not returned by jcompile.

* Bug #13008 fixed - 'help $' now opens the 'Symbols' help page.

* Bug #13010 fixed - Wrong class was returned by jcompile (with ecj).

* Bug #13011 fixed - ilib_compile failed under Mac OS X 10.9.

* Bug #13012 fixed - Vectorial export of rotated strings was wrong.

* Bug #13013 fixed - In SciNotes, the first proposed directory to save a file was not current working directory.

* Bug #13014 fixed - Update of the efficiency inner variable improved in optim_ga.

* Bug #13015 fixed - Computation of efficiency inner variable improved in optim_ga.

* Bug #13022 fixed - Vectorial export did not clip large segments.

* Bug #13023 fixed - xs2pdf crashed under Windows when the target file was already opened.

* Bug #13027 fixed - There was no autowrap into array in JIMS.

* Bug #13031 fixed - Lorentz Butterfly demonstration fixed.

* Bug #13032 fixed - CMATVIEW help page example fixed and CMAT3D help page example created.

* Bug #13033 fixed - -1 could not be used as nax argument in plot2d.

* Bug #13036 fixed - The help page associated to the history browser was wrong.

* Bug #13041 fixed - Wrong result was returned for "integer scalar" minus "integer vector".

* Bug #13042 fixed - Texts in graphics were not properly centered.

* Bug #13047 fixed - jcompile did not allow class reloading.

* Bug #13049 fixed - New handled protocols added in links in SciNotes.

* Bug #13050 fixed - The result of mvvacov was not symmetric.

* Bug #13051 fixed - SciNotes restoration could block desktop one.

* Bug #13053 fixed - datatipCreate did not return datatip handle.

* Bug #13055 fixed - Array indexing did not follow Scilab convention in JIMS.

* Bug #13061 fixed - hdf5 demonstration failed with a read-only file.

* Bug #13063 fixed - Color selection in SciNotes preferences threw exceptions.

* Bug #13064 fixed - Deleting a link connected to a split led to a translated link.

* Bug #13069 fixed - Documentation for %MODELICA_USER_LIBS updated.

* Bug #13066 fixed - data_bounds was not fully documented in axes_properties.

* Bug #13080 fixed - Contextual menu was unavailable for datatips management.

* Bug #13082 fixed - Datatip mark style was not inherited from the parent polyline.

* Bug #13083 fixed - Datatip marks did not inherit colors from the parent polyline.

* Bug #13084 fixed - interp_mode property could not be set on all datatips of the same polyline.

* Bug #13085 fixed - A datatip could not be moved on a circle.

* Bug #13092 fixed - Optimizations now check the user function output (must be real).

* Bug #13093 fixed - Removed trailing "\n" in the head_comments error message.

* Bug #13101 fixed - When x-axis was in reverse position x_location='origin' failed.

* Bug #13102 fixed - savematfile did not support "-v7.3" option.

* Bug #13108 fixed - Time between ATOMS database updates is now a month (was a day) and is configurable.

* Bug #13109 fixed - pol2str now supports polynomials with complex coefficients and hypermatrices.

* Bug #13111 fixed - sqrt returned different results when imaginary part was -0 versus 0.

* Bug #13114 fixed - clear_pixmap/pixmap property should have been removed in Scilab 5.4.1.

* Bug #13116 fixed - qpsolve now respects upper-bounds constraints.

* Bug #13119 fixed - mget and mgetl now return an error when called with decimal values as number of lines.

* Bug #13121 fixed - ode "rk" option crashed Scilab when the user derivative function failed.

* Bug #13127 fixed - There were no subticks with user defined ticks.

* Bug #13132 fixed - There were missing graduations when data_bounds interval was too small.

* Bug #13134 fixed - User-defined ticks in log scale were invisible.

* Bug #13136 fixed - exists and isdef failed for input arguments longer than 1.

* Bug #13139 fixed - fft help page fixed.

* Bug #13140 fixed - Various typos fixed in help pages.

* Bug #13144 fixed - csvRead can now ignore header comments thanks to a new input argument.

* Bug #13146 fixed - profile failed when a comment was on the same line as a function declaration.

* Bug #13150 fixed - Vectorial export used too much memory for grayplot.

* Bug #13152 fixed - Typo fixed in syslin French help page.

* Bug #13164 fixed - Miscolorization in SciNotes colors preferences fixed.

* Bug #13165 fixed - Preferences reset did not show a confirmation pop-up.

* Bug #13168 fixed - Wrong interpretation of star followed by minus fixed.

* Bug #13170 fixed - Legends for plzr plots fixed.

* Bug #13174 fixed - Scilab crashed after XML element removal.

* Bug #13175 fixed - argn help page updated with specific cases.

* Bug #13177 fixed - Error when changing x_ticks.locations on axes fixed.

* Bug #13185 fixed - When the "checked" option of a uimenu was set to "on" for
                     the first time, the display order of other uimenus was changed.

* Bug #13186 fixed - csvRead freezed Scilab if separator was an empty string.

* Bug #13187 fixed - xmltoformat did not include "imageobjects" in output.

* Bug #13188 fixed - The output argument of eomday function was not pre-dimensioned.

* Bug #13191 fixed - isempty(rational) returned an error message.

* Bug #13192 fixed - horner returned an error message when the input arguments did not have the same size.

* Bug #13194 fixed - part help page improved.

* Bug #13199 fixed - There was a thin blue line around acknowledgements button in about box.

* Bug #13200 fixed - about() ACKNOWLEDGEMENTS did not render utf-8 characters.

* Bug #13201 fixed - x_mdialog entries had no margin.

* Bug #13202 fixed - kernel help page improved.

* Bug #13203 fixed - Typos fixed in some error messages.

* Bug #13205 fixed - group accepted continuous transfer functions.

* Bug #13208 fixed - New nanreglin function to handle NaNs for reglin().

* Bug #13210 fixed - Incorrect argument description in mseek help page.

* Bug #13213 fixed - User-defined margins were reset by auto computation of the margins.

* Bug #13215 fixed - clf(1001) returned an error.

* Bug #13218 fixed - Typos fixed in core module help pages.

* Bug #13226 fixed - Completion with accented chars could led to a crash.

* Bug #13227 fixed - Invalid windowsConfiguration.xml file could avoid Scilab startup.

* Bug #13233 fixed - Wrong simulation result of step response csim('step',t,H) fixed.

* Bug #13234 fixed - lmitool calling sequence clarified.

* Bug #13236 fixed - "parents" help page has been renamed to "parentheses".

* Bug #13238 fixed - Wrong legends display fixed.

* Bug #13243 fixed - optim with "gc" option failed when imp<0.

* Bug #13247 fixed - Hypermatrix in structure definition did not work properly.

* Bug #13252 fixed - Minor typos fixed in Graphics messages.

* Bug #13258 fixed - Bad number display in datatips fixed.

* Bug #13267 fixed - Implicit typecasts in scicos.c fixed.

* Bug #13271 fixed - plot2d with logarithmic scale and %nan value froze Scilab.

* Bug #13272 fixed - Error occurring when reading CSV files with comment option on a CSV file without comment fixed.

* Bug #13280 fixed - Axes were not always displayed properly after figure resize.


Xcos Bug Fixes
==============

* Bug #9995 fixed - LOGICAL_OP drew the parameters over the block.

* Bug #11975 fixed - Inverted Pendulum demonstration did not compile.

* Bug #12423 fixed - Data types of SWITCH2_m were not documented.

* Bug #12685 fixed - The lincos and steadycos functions did not load
                     the XcosLibs so all the blocks were unknown.

* Bug #12998 fixed - 'ans' was not ignored in the context results.

* Bug #13006 fixed - Selection to superblock (in_f and out_f) failed.

* Bug #13030 fixed - Selection to superblock did not reset the origin.

* Bug #13059 fixed - NaN propagated at startup made the simulation fail.

* Bug #13071 fixed - Three unused functions in Xcos macros now specified as not mandatory
                     to write new blocks.

* Bug #13172 fixed - Logic block help file had some typo errors.

* Bug #13239 fixed - Grid was not shown in zoomed log scale.

* Bug #13250 fixed - CLSS wrongly handled scalar values.




                 Changes between version 5.4.1 and 5.5.0-beta-1
                 ==============================================

New Features
=============

* New special functions:
 - erfi - The imaginary error function.
 - dawson - Compute the Dawson function (scaled imaginary error).

* New functions introduced:
 - getURL - Download a URL (HTTP, HTTPS, FTP...)
 - splitURL - Split a URL (HTTP, HTTPS, FTP...)
 - cov - Covariance matrix. Deprecates mvvacov. See bug #11896.
 - ismatrix - Check if a variable is a matrix. See bug #10456.
 - isrow - Check if a variable is a row vector. See bug #10456.
 - iscolumn - Check if a variable is a column vector. See bug #10456.
 - issquare - Check if a variable is a square matrix. See bug #10456.
 - cross - Vector cross product. See bug #9941.
 - members - Number of occurrences and linear indexes of common values between
             two matrices of the same type.See bug #12705.

* Complete set of functions to read and write any HDF5 file from Scilab added.

* New Solver:
 - daskr - differential-algebraic system solver with rootfinding 'daskr', using
BDF methods with direct and preconditioned Krylov linear solvers, based on ODEPACK.

* Based on JIMS external module, Scilab provides functions to interact
  with Java objects.

* erf, erfc, erfcx and calerf functions now support complex arguments.

* isnum has been redesigned in native code. Performance improvements up to 130x.
  See bug #10404.

* Usage of the '$' keyword in part function allowed.

* The histplot command can now be used with the option polygon=%t/%f to add the
  frequency polygon chart (Thanks to Mehran Khorshidi).

* Multi level completion on mlist, struct, XML structures...

* Variable browser improvements:
 - The variable browser also shows the size of integers and the user type of
  the tlist/mlist.
  See bugs #12523 and #10409.
 - It is now possible to delete variables from the variable browser.
  See bug #9447.
 - A user can now plot variables from the variable browser (this functionality was
  already available in the variable editor).

* Added lighting effect for plot of surfaces. Lighting can be enabled
  creating light objects or disabled by deleting them. The following function
  was introduced:
  - light - Creates a light graphic object.

* Localization:
 - Multiple domains in localization managed.
 - addlocalizationdomain function added for a new domain creation.
 - Optional parameter added to gettext to manage domains.
 - tbx_generate_pofile and tbx_build_localization added to create localization files for
   modules.

* Windows Solution updated to Visual Studio 2012.

* -keepconsole option added for Scilab Windows to facilitate debugging.
  Calling Scilab with this option will leave the console box window opened at startup.

* License update: switch to the CeCILL 2.1.


Improvements
=============

* New calling sequence allowed for nicholschart: nicholschart(gains, phases, colors).
  See bug #7828.

* qp_solve can now take up to 5 output arguments. The last one is an error flag,
  if it is present, then the function will display a warning instead of an error.
  See bug #10269.

* graypolarplot has been improved in terms of performances and rendering.
  See bug #12641.

* nthroot is now vectorizable.
  See bug #12678.

* New optional output argument for routh_t.
  See bug #12829.


Differential Equations
======================

* Netlib's Quadpack, used for definite integration, has been updated to match the upstream.


SciNotes
========

* Autosave feature can now use %date to append the current date to the backup filename
  (See bug #12712).


Xcos
====

* New DAE solver: DDaskr, using BDF methods with direct Newton and preconditioned Krylov
  linear solvers, which includes rootfinding.
  It is available from dae function.

* In Modelica initialization GUI, inputs (eg. sensor) were not handled.

* Sundials updated to the "2.5.0" version, keeping our modifications.

* JGraphX updated to the "2.1.0.7" version, updating our hooks performance.

* API changed in the scicos_block4 interface: the uid value is not available
  inside the simulation function.

* xcosAddToolsMenu added to let the user perform some actions on the graph.

* CBLOCK4 block added to the "User-Defined Functions"


Toolbox Skeleton
=================

* Some Xcos specific content added to xcos_toolbox_skeleton help pages.


Compilation
============

* Minimal version of Flexdock (1.2.4) enforced.


Obsolete & Removed Functions
============================

* Scilab 5.5.X family will be the latest family working under Windows XP/2003.

* Vector ^ scalar syntax declared as obsolete. See bug #11524

* The third argument of poly function will be more strict.
    Only the following strings are allowed: "roots", "coeff", "c", "r".

* Option and direction arguments of gsort will be more strict in all cases.
  Only the following strings are allowed:
  - option: "r" "c" "g" "lr" "lc"
  - direction : "d" "i"

* nfreq tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use tabul instead.

* IsAScalar tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use isscalar instead.

* chart tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use nicholschart instead.

* Second output argument of add_param tagged as obsolete. Will be removed in Scilab 5.5.1.

* mvvacov tagged as obsolete. Will be removed in Scilab 6.0.0.
  Please use cov instead.

* dft removed, please use fft instead.

* sscanf removed, please use msscanf instead.

* fscanf removed, please use mfscanf for files opened with mopen or read for files opened
  with file instead.

* fprintf removed, please use mfprintf for files opened with mopen or write for files
  opened with file instead.

* mfft tagged as obsolete. Will be removed in Scilab 5.5.1.
  Please use fft instead.


Scilab Bug Fixes
================

* paramfplot2d: When theta input argument was a column vector, an error occurred.

* Bug #2267 fixed - Wrong error message when ticks locations and labels did not have the
                    same sizes.

* Bug #2416 fixed - Particular case (string([]) returns []) has been added in string help
                    page.

* Bug #3511 fixed - strindex did not return all occurrences in regexp mode.

* Bug #3928 fixed - An error was returned when a matrix was flipped along the third
                    dimension.

* Bug #4042 fixed - squeeze returned a hypermatrix instead of a matrix when at least one
                    dimension was equal to 1.

* Bug #4085 fixed - num2cell help page added.

* Bug #4229 fixed - delip did not return an error if one element of its first input
                    argument was negative.

* Bug #4383 fixed - csim with "step" and "impuls" now works with direct feedthrough.

* Bug #4481 fixed - iir help page updated to precise that frq can be a scalar.

* Bug #4731 fixed - lqr failed when the time domain of an input was a number.

* Bug #4743 fixed - Graphics with too big or too small values did not work.

* Bug #5017 fixed - norm rewritten to take benefit from the Lapack package performance.

* Bug #5073 fixed - New parameter added in strtod function (decimal separator).

* Bug #5205 fixed - permute was slow for large hypermatrices.

* Bug #5207 fixed - grand can now return a hypermatrix.

* Bug #5365 fixed - makecell help page was in the "compatibility functions" directory
                    instead of being in "data_structures".

* Bug #5539 fixed - sylv help page was wrong in the discrete-time case.

* Bug #5616 fixed - typeof(uiCreateTree(...)) returned "Tree" instead of "uitree".

* Bug #5694 fixed - numdiff help page clarified.

* Bug #5779 fixed - xnumb number format was too small (+ unit test added).

* Bug #6037 fixed - macrovar help page improved.

* Bug #6168 fixed - zpbutt, zpch1, zpch2 and zpell help pages were unclear.

* Bug #6390 fixed - The "external" argument of odedc was not well documented and
                    not tested.

* Bug #6427 fixed - full([%T %F]) returned an error message.

* Bug #6466 fixed - Example with vectorized input added in mprintf and msprintf help pages.

* Bug #6584 fixed - mfft tagged as obsolete.

* Bug #6638 fixed - The profiler output was incorrect by a factor of 1000 under Windows.

* Bug #6693 fixed - modulo did not accept polynomial inputs. Help page was not updated.

* Bug #6752 fixed - unit test scilab.tst has been split in different tests.

* Bug #6840 fixed - New line_style added.

* Bug #6988 fixed - Error messages in modules/data_structures/src/c/hmops.c were not
                    standard.

* Bug #7026 fixed - There was no unit test for plot2d.

* Bug #7080 fixed - Some graphic macros did not use standard error messages.

* Bug #7204 fixed - geomean applied to a hypermatrix gave wrong results.

* Bug #7206 fixed - If the second input argument of meanf function was a hypermatrix,
                    this function returned an error.

* Bug #7296 fixed - %nan, %inf and -%inf enabled for the cdf* functions.

* Bug #7411 fixed - clf forgot to turn off the datatip mode.

* Bug #7486 fixed - LAPACK versions of DGELSY and ZGELSY now used.

* Bug #7596 fixed - A same error happening in different places now displays the same
                    error message.

* Bug #7648 fixed - CDF functions now display a warning for non integer
                    "degrees of freedom" argument.

* Bug #7655 fixed - An example added in type help page, for type(X)=11 and type(X)=13.

* Bug #7684 fixed - Introduction demonstration splitted into subsections.

* Bug #7781 fixed - The second parameter of iqr function had no effect.

* Bug #7782 fixed - lcm and gcd help pages improved to tell the user how to use both
                    functions.

* Bug #7824 fixed - title function properties did not support an indexed color.

* Bug #7826 fixed - chart tagged as obsolete.

* Bug #7828 fixed - Slight improvements in nicholschart.

* Bug #7848 fixed - The third argument of correl function is now optional.

* Bug #7877 fixed - iirgroup function fixed.

* Bug #7927 fixed - Output "flag" in qmr function was not well documented.

* Bug #7960 fixed - plzr could not produce pole zero plot for a simple transfer function.

* Bug #8058 fixed - The user can now set the tolerances of intc function.

* Bug #8098 fixed - cumsum could not be applied to rational matrices.

* Bug #8162 fixed - Area of stability of plzr was wrong for continuous systems
                    (+unit test added).

* Bug #8211 fixed - Parameters module demonstration finalized.

* Bug #8234 fixed - strtod did not return an empty matrix when the input argument was an
                    empty matrix.

* Bug #8264 fixed - Matlab to Scilab dictionary help page updated for atan2.

* Bug #8319 fixed - dbphi(hypermat) and phasemag(hypermat) returned a matrix instead of
                    a hypermatrix.

* Bug #8373 fixed - clear can now handle a matrix of strings argument.

* Bug #8415 fixed - optim_moga, optim_nsga, optim_nsga2 can now take list as input
                    arguments, as explained in their help pages.

* Bug #8462 fixed - bvodeS could make Scilab unstable.

* Bug #8470 fixed - bvode displayed some output in terminal window and not in
                    Scilab console.

* Bug #8479 fixed - The latest Saxon version was not supported.

* Bug #8511 fixed - sprand now uses grand instead of rand and grand functions. Internal
                    state of the random generator is no more changed.

* Bug #8561 fixed - ddassl, ddasrt, ddaskr: abs and rel tolerance sizes are now checked.

* Bug #8597 fixed - grand/clcg4 could display uncontrolled messages as warning.

* Bug #8607 fixed - Some error messages in modules/overloading/macros were not standard
                    and not localized.

* Bug #8614 fixed - Unit test for barhomogenize added.

* Bug #8636 fixed - roots help page updated (default algorithm value was wrong).

* Bug #8667 fixed - The handling of %nan in min, max, median functions was not properly
                    documented.

* Bug #8680 fixed - "end" output argument of regexp function has been changed.

* Bug #8687 fixed - typeof function failed on uint8, depending on the format
                    (+ unit test added).

* Bug #8695 fixed - optim_ga used old (initial) values instead of newly-computed ones.

* Bug #8778 fixed - Call_ScilabOpen, TerminateScilab could not be called more
                    than 80 times in a loop.

* Bug #8779 fixed - gsort did not preserve the order of equal elements, in
                    lexicographic sort.

* Bug #8820 fixed - squeeze did not return a matrix when the number of dimensions
                    of the result was less or equal to 2.

* Bug #8824 fixed - taucs_chfact returned a segfault (not the case in nwni mode).

* Bug #8840 fixed - fileparts did not manage matrix of strings.

* Bug #8856 fixed - Non regression test added for [k,l,m,...]=find(a==5).
                    Non regression test of bug #476 updated.

* Bug #8857 fixed - Non regression test of bug #477 updated.

* Bug #8858 fixed - Non regression test of bug #480 updated.

* Bug #8862 fixed - mget and mput could not read and write 64 bit data from
                    binary files.

* Bug #9004 fixed - bitcmp function called with one input argument returned an error.

* Bug #9020 fixed - exists function did not accept matrix as first input argument.

* Bug #9059 fixed - tbx_build_macros and genlib did not stop even if an error occurred.

* Bug #9109 fixed - nfreq tagged as obsolete.

* Bug #9158 fixed - zeros called with a big number returned an empty matrix instead
                    of an error.

* Bug #9208 fixed - Added three optional output arguments to optim,
                    to retrieve #iterations, #evaluations and a termination indicator.

* Bug #9385 fixed - The type checking in trigonometric functions has been added.

* Bug #9394 fixed - is_param recognized "plist" as an existing field.

* Bug #9395 fixed - add_param did not check its input arguments.

* Bug #9396 fixed - add_param accepted duplicate keys.

* Bug #9459 fixed - Default values of the optional plot3d arguments were not documented.

* Bug #9537 fixed - optimbase_configure only allowed row vectors as initial value.
                    Column vectors now allowed by transposing them.

* Bug #9538 fixed - optimbase_checkshape only allowed row vectors as output arguments of
                    cost function. Column vectors are now allowed by transposing them.

* Bug #9577 fixed - Setting neldermead_configure("-numberofvariables") is now optional,
                    setting neldermead_configure("-x0") initializes -numberofvariables
                    implicitly.

* Bug #9601 fixed - Cylinder demonstration fixed.

* Bug #9688 fixed - optim could crash when "imp" option was < 0. It is now set to 0 in
                    that case.

* Bug #9690 fixed - The "imp" option for optim could crash Scilab and was not consistent
                    with the help page.

* Bug #9691 fixed - "imp" option in optim help page was poorly documented.

* Bug #9694 fixed - Example in optim help page fixed to display correct counters.

* Bug #9702 fixed - Contrary to what optim help page stated, the "gc" algorithm does use
                    the "epsx" parameter.

* Bug #9780 fixed - gmres solver did not run with complex systems.

* Bug #9788 fixed - neldermead can now produce a warning when it fails to converge,
                    thanks to a new input argument "warn".

* Bug #9821 fixed - getrelativefilename did not manage matrix of strings.

* Bug #9851 fixed - Error message occurred because of a cut-off frequency of 0.25Hz
                   with irr.

* Bug #9859 fixed - It was not possible to draw arrows in 3-D using xarrows.

* Bug #10146 fixed - In SciNotes, 'help on keyword' moved from bottom to top in the popup
                    menu.

* Bug #10180 fixed - det was not defined for sparse matrices.

* Bug #10213 fixed - sci2exp help page updated to document the impact of format function.

* Bug #10216 fixed - Invalid syntaxes for zeros, ones, eye, rand, like zeros(2,:).

* Bug #10226 fixed - When a // <empty session> line was deleted, all sessions
                     histories were folded.

* Bug #10254 fixed - Slight improvements in ones help page.

* Bug #10269 fixed - qp_solve can now take up to 5 output arguments. The last one is an
                    error flag, if it is present, then the function will issue a warning
                    instead of an error.

* Bug #10276 fixed - qp_solve segfaulted with large matrices.

* Bug #10287 fixed - Error message added for complex expression as input argument of
                    integrate function.

* Bug #10305 fixed - Comparison of lists with empty items returned an error message.

* Bug #10596 fixed - exit(xxx) from Scilab failed.

* Bug #10818 fixed - home, %e, %t, %f, %z and %s help pages added.

* Bug #10823 fixed - fullpath returned different results under Windows and Linux for
                    non-existent file.

* Bug #10862 fixed - Add a without Internet connection installation
                    global configuration in the installer.

* Bug #10866 fixed - det was not equivalent to detr for rational matrices.

* Bug #10906 fixed - Typo fixed in cls2dls help page.

* Bug #10930 fixed - The comments in armax function were in French.

* Bug #10995 fixed - Typo fixed in grand help page for Gamma law argument.

* Bug #11065 fixed - The second output argument of unique function contained a wrong result.

* Bug #11067 fixed - Display of ticks labels with closed associated values was wrong.

* Bug #11139 fixed - conj was not defined for sparse matrices (+ unit test added).

* Bug #11308 fixed - Calling sequences in dsearch help page were wrong.

* Bug #11343 fixed - The "isoview" figure property did not work when the axes
                     margins had been modified.

* Bug #11648 fixed - Copying graphic via the clipboard did not work.

* Bug #11766 fixed - nthroot has been added to m2sci help page.

* Bug #11792 fixed - Lists can be accessed with non integer indexes (list help page
  updated).

* Bug #11869 fixed - "Environment" was not localized in preferences.

* Bug #11885 fixed - Each rand has been changed to grand in genetic algorithms and
                    simulated annealing functions.

* Bug #11891 fixed - Fisher ratio could be inaccurate for one-way ANOVA.

* Bug #11997 fixed - In case of invalid variable name, save function now returns an error
                    instead of a compatibility warning.

* Bug #12034 fixed - max function did not manage empty matrices.

* Bug #12045 fixed - repmat returned wrong results if the values of an input matrix were
                    not double.

* Bug #12070 fixed - Removing a module can now be done in an on-line mode even if
                     it has been installed in an off-line mode.

* Bug #12080 fixed - lsqrsolve always printed messages.

* Bug #12085 fixed - Under Windows, csvWrite wrote wrong EOL.

* Bug #12114 fixed - libstdc++ is now compiled as static instead of using the
                     library from thirdparty.

* Bug #12118 fixed - ode could take complex externals.

* Bug #12143 fixed - "stop entity picker" (ged(11)) returned an error message.

* Bug #12150 fixed - datatipCreate failed with one single point.

* Bug #12163 fixed - unzoom did not work with a single input argument.

* Bug #12212 fixed - Export of a polyline in 2-D broke it into several segments.

* Bug #12326 fixed - There was no way to set LaTeX font size in preview.

* Bug #12349 fixed - In SciNotes, when the view was splitted, removing a char made the
                    other view jump.

* Bug #12376 fixed - Exec & edit buttons in the help pages examples were sometimes
                    misplaced.

* Bug #12413 fixed - ATOMS packages could not be removed if ATOMS had never been on-line.

* Bug #12415 fixed - PATH environment variable grew when using call_scilab in a loop.

* Bug #12426 fixed - By using addErrorMessage, a random string error could be displayed.

* Bug #12433 fixed - show_pixmap function was removed from Scilab but still used in some
                    functions.

* Bug #12443 fixed - The behavior of mopen in text file mode has been documented
                     under Windows.

* Bug #12463 fixed - Wrong specification for rect=[x,y,w,h] argument in xstringb
                    French help page.

* Bug #12470 fixed - Variable browser was not refreshed after loading an environment.

* Bug #12473 fixed - Problems with "é" in mkdir French help page.

* Bug #12475 fixed - csvRead did not support double quoted fields.

* Bug #12490 fixed - plot did not call clf; in its help page example producing a
                    wrong behavior.

* Bug #12506 fixed - In SciNotes, a miscolorization could occurred when returned values
                    list was broken.

* Bug #12508 fixed - Wrong error message in rand function fixed.

* Bug #12518 fixed - Polynoms were not displayed by default in the variable browser.

* Bug #12520 fixed - Improve the description of the size in the variable browser.

* Bug #12527 fixed - Scilab user functions were not listed in the variable browser.

* Bug #12547 fixed - In SciNotes, lines were wrongly numbered with splitted function
                    declarations.

* Bug #12548 fixed - Duplicated code in xmltoformat removed.

* Bug #12550 fixed - optimbase and optimsimplex help pages were not standard.

* Bug #12551 fixed - Stack problem with diary([],"pause") and diary([],"resume") fixed.

* Bug #12556 fixed - A fatal error occurred when calling set with wrong instructions.

* Bug #12557 fixed - Valid function names were not specified in function and functions
                    help pages.

* Bug #12564 fixed - Compile and run javasci help page was not clear about needed packages.

* Bug #12581 fixed - isfield could not support mlist or tlist.

* Bug #12589 fixed - Call sequence for spzeros & spones were missing in French
                    help pages.

* Bug #12592 fixed - Scilab hung with plot(-0).

* Bug #12593 fixed - A wrong error message was returned when running genlib with an
                    error in the sci file.

* Bug #12594 fixed - Invalid SciNotes configuration file avoided SciNotes startup.

* Bug #12600 fixed - mput did not manage unsigned integer.

* Bug #12606 fixed - Overloads for grand were not standard.

* Bug #12613 fixed - gsort did not return correct results with %nan.

* Bug #12614 fixed - Helpbrowser was not launched in EDT.

* Bug #12615 fixed - Graphics seemed to be freezed after a call to plot/bar/barh with
                     a bad LineSpec argument.

* Bug #12624 fixed - In case of errors in Scilab macros, "make check-TESTS" did not fail
                    as expected.

* Bug #12627 fixed - At restoration, a window could be out of the screen.

* Bug #12629 fixed - The last example of csim help page defined a function called
                     input (overwriting the Scilab one).

* Bug #12631 fixed - A "see also" link has been added from progressionbar to waitbar
                    and vice versa.

* Bug #12634 fixed - ATOMS modules could not be installed from an archive file
                     already existing in SCI/contrib/archives in "offline" mode.

* Bug #12637 fixed - In some help pages, some signal processing functions were not in
                    the correct section.

* Bug #12639 fixed - justify([], position) returned an error instead of [].

* Bug #12641 fixed - graypolarplot has been fully vectorized as it was too slow.

* Bug #12657 fixed - Computation of v1.^v2 is now done without any memory allocation,
                     when v1 and v2 are real arrays, v1 >= 0 and v2 integer.

* Bug #12668 fixed - Undocking SciNotes led to an exception.

* Bug #12678 fixed - nthroot now accepts vector/matrix as second argument.

* Bug #12679 fixed - Argument type check added in gcd and lcm.

* Bug #12686 fixed - Error returned by diff fixed.

* Bug #12702 fixed - When no extra parameters were needed in the cost function,
                    NDcost did not work.

* Bug #12703 fixed - In SciNotes, common shortcuts 'SHIFT DELETE', 'SHIFT INSERT'
                    (and others) added.

* Bug #12705 fixed - members function added. It allows to find the number of occurrences
                    and linear indexes for common values between two matrices of the
                    same type.

* Bug #12706 fixed - A wrong size of a matrix as input argument of cross function was
                    not detected.

* Bug #12708 fixed - Incorrect display in SciNotes preferences fixed (onmouseover styles).

* Bug #12712 fixed - In SciNotes, autosaving can now use %date to append the current
                    date to the backup filename.

* Bug #12715 fixed - Variable cross in pspect and cspect has been renamed because of
                    conflicts with cross function.

* Bug #12736 fixed - In SciNotes, the Completion window appeared only in first tab.

* Bug #12747 fixed - legendre now accepts the -1 and 1 values for third argument.

* Bug #12749 fixed - fscanfMat help page updated.

* Bug #12758 fixed - Focus issue with plot3d fixed.

* Bug #12761 fixed - The https:// protocol was not supported on ATOMS.

* Bug #12772 fixed - eigs failed when trying to solve a sparse matrix eigen value problem.

* Bug #12774 fixed - Various typos fixed.

* Bug #12775 fixed - Some related functions were not listed in "See also" section of
                    routh_t help page.

* Bug #12778 fixed - Insertion of an empty matrix in an integer matrix led to a
                    wrong result.

* Bug #12779 fixed - savewave had a miscoding in the internal function write_wavedat.

* Bug #12783 fixed - There were some inconsistent error messages in dsearch.

* Bug #12790 fixed - Links to ZCOS files in documentation were broken.

* Bug #12791 fixed - More information is now given in case of failure during the ATOMS
                    autoload step.

* Bug #12793 fixed - Improved the bode() plots with a new option "rad" to convert plot into rad/s
                     and a new function bode_asymp() to draw the system asymptotes.

* Bug #12794 fixed - calfrq.sci code did not follow Scilab standard.

* Bug #12795 fixed - Typos fixed in CACSD help page.

* Bug #12800 fixed - Typo fixed in Polynomials help page.

* Bug #12804 fixed - Typos fixed in routh_t help page.

* Bug #12807 fixed - Display of showprofile improved.

* Bug #12808 fixed - Add missing </td> in documentation generation (note, warning, ...).

* Bug #12813 fixed - flipdim function extended to any type of input data.

* Bug #12814 fixed - Improvements of pertrans help page.

* Bug #12815 fixed - levin redefined cov as a variable.

* Bug #12816 fixed - Numbers pasted in editvar were not parsed according to locale.

* Bug #12818 fixed - Segfault in set function with invalid property values dimension.

* Bug #12823 fixed - In help generation (toolbox) links were not correctly handled.

* Bug #12827 fixed - noisegen help page improved.

* Bug #12828 fixed - routh_t gave a wrong result if the first element of a row was zero.

* Bug #12829 fixed - New optional output argument added for routh_t function.

* Bug #12830 fixed - In SciNotes, it was not possible to execute a replace action
                    from the caret position.

* Bug #12831 fixed - In SciNotes toolbar, there was no button to open code navigator.

* Bug #12833 fixed - In SciNotes, there was no autoscroll when searching a pattern.

* Bug #12836 fixed - Error fixed in strcmpi help page.

* Bug #12839 fixed - Typo fixed in getVariablesOnStack help page.

* Bug #12840 fixed - Typo fixed in number_properties.xml help page.

* Bug #12852 fixed - Visual Studio 2012 SDK configuration was incorrect.

* Bug #12858 fixed - Typo fixed in debug and pause French help pages.

* Bug #12859 fixed - Fixed optional argument in isdef help page.

* Bug #12863 fixed - size(state-space, "r") returned an error.

* Bug #12875 fixed - phasemag returned an error for input vector containing zeros.

* Bug #12887 fixed - Scilab hung with auto_clear set to on and log_flags set to true.

* Bug #12888 fixed - sysdiag was not documented about block diagonal matrices build.

* Bug #12906 fixed - champ and champ1 help pages updated

* Bug #12909 fixed - Completion on (mt)list led to a crash.

* Bug #12911 fixed - Matlab to Scilab dictionary help page updated for eig.

* Bug #12913 fixed - linspace returned an error if the third argument was an integer type
                    variable.

* Bug #12916 fixed - power help page improved.

* Bug #12919 fixed - Rotation on plots was disabled after using menus.

* Bug #12927 fixed - ones function could not take integer type input.

* Bug #12931 fixed - ATOMS redefined "message" keyword.


Xcos Bug Fixes
===============

* Bug #7350 fixed - The I/O ports numbering of a superblock was not updated
                    when a new port was dropped.

* Bug #8570 fixed - "Region to superblock" has been renamed to
                    "Selection to superblock".

* Bug #11518 fixed - CLR block-text was displayed out of the bounding box
                     when zooming.

* Bug #11776 fixed - CMSCOPE did not take into account label&Id parameter.

* Bug #12359 fixed - Xcos files have been converted to ZCOS to gain some space.

* Bug #12384 fixed - Using a Modelica part linked with an explicit link to
                     another Modelica part led to an algebraic loop error.

* Bug #12387 fixed - The "Modelica initialize" setup menu option did not blur
                     during simulation.

* Bug #12414 fixed - SWITCH_m block had different behaviors
                     for different types of inputs.

* Bug #12424 fixed - Calling lincos on a derivative block made Scilab crash.

* Bug #12449 fixed - QUANT_f was not rounding/truncating/flooring/ceiling properly input signal.

* Bug #12460 fixed - xcosPalGenerateAllIcons sometimes crashed Scilab.

* Bug #12461 fixed - Cancelling zoom out by zooming in did not fully work.

* Bug #12561 fixed - SELECT_m and RELAY_f did not behave as expected.

* Bug #12568 fixed - "Recent files" menu entry is now below the "Open" one.

* Bug #12590 fixed - Block shape style was removed on file loading.

* Bug #12603 fixed - ZCOS files could contain blocks with modified (eg. invalid)
                     parameters.

* Bug #12619 fixed - DLR discrete block did not display LaTeX formula
                     like CLR continuous block.

* Bug #12651 fixed - The 'nw' scicos_simulate option did not work while
                     passing a previous Info simulation status.

* Bug #12664 fixed - Inverted pendulum French localization in the Xcos demonstrations was
                    inaccurate.

* Bug #12667 fixed - 'Recent Files' menu was not localized.

* Bug #12731 fixed - Code generation produced erroneous block.

* Bug #12732 fixed - Improper Copyright comments in the files were generated by the code
                    generation tool.

* Bug #12796 fixed - There was some mismatches between implicit
                     and explicit ports of superblocks.

* Bug #12797 fixed - I/O blocks generated by "Selection to Superblocks" had
                     wrong size.

* Bug #12868 fixed - There were several problems with PULSE_SC block.

* Bug #12869 fixed - min and max were not defined but used in Xcos generated code.

* Bug #12873 fixed - scicos_flat produced an unexpected error after a diagram
                     load.

* Bug #12874 fixed - CSCOPE marks sizes were too small.

* Bug #12877 fixed - Incorrect output port dimensions and types in GENSIN_f, GENSQR_f,
                    STEP_FUNCTION and STEP blocks fixed.

* Bug #12924 fixed - Blocks type 2004 was not handled as a valid block type.

* Bug #12934 fixed - Separate compilation of a superblock could fail due to under
                    determined signal sizes.
