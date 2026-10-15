Welcome to Scilab 2027.0.0
==========================

This file details the changes between Scilab 2027.0.0 (this version) and the previous 2026.1.0 stable release.

For changelogs of earlier releases, please see [Scilab 2026.1.0][1].

Summary:

- New features
- Obsolete functions & features
- Removed functions & features
- Known incompatibilities
- Compilation & Packaging
- Fixed issues

Please report anything we may have missed on [Discourse][2] or [GitLab][3].

[1]: https://help.scilab.org/docs/2026.1.0/en_US/CHANGES.html
[2]: https://scilab.discourse.group/
[3]: https://gitlab.com/scilab/scilab/-/issues

New features
------------

For a high-level description of the main new features in this release, please consult the homepage of the embedded documentation, available by simply typing `doc` in the Scilab console.

### SciNotes

- SciNotes editor is now web-based, built on the editor of VS Code and on the Scilab language server: completion, hover help, signature help, go to definition, semantic coloring, syntax diagnostics, indentation formatting and LaTeX preview in comments & strings.
- A **debugger** is also available: breakpoints (with conditions and hit counts) set in the margin, step by step execution, values of the variables on hover, and the variables of the stopped scope in the Variable Browser. See `doc scinotes_debugger`.
- An **AI assistant** is available as well: a side panel where an AI coding agent of your choice (Claude, Codex, ...) writes Scilab code, **runs it in the session** and edits the file open in the editor, asking before any change. See `doc scinotes_assistant`.

### New functions

- `agglomerative()`: Agglomerative hierarchical clustering.
- `between()`: Calendar difference between two `datetime` values.
- `caldiff()`: Calendar successive differences.
- `linkage()`: Agglomerative hierarchical cluster tree.
- `silhouette()`: Silhouette values of a clustering.
- `sprkode()`: SUNDIALS ordinary differential equation symplectic Runge-Kutta solver.

### Features improvements

- The **console** is now web-based: LaTeX rendering of strings, icons in the completion window, files executed by drag & drop, and a pasted block of code executed at once instead of line by line.
- **Console**: warnings and errors are now visually distinct from regular output (dedicated colors, in both the console and the terminal where errors go to stderr). Colors and bold/italic styles can be customized in Preferences ⇒ Colors.
- **Console**: "See also" entries and `doc <page>` displayed by `help()` are clickable links. Their color can be customized in Preferences ⇒ Colors.
- The **help browser** is now web-based: highlighted code, LaTeX & MathML equations, improved search, and a menu on each page to jump to its sections and anchors.
- The help of a toolbox is also rendered from its XML pages: `tbx_build_help()` no longer builds a help jar, but pre-generates the images of the pages (`<scilab:image>` blocks and Xcos diagrams) into the `help/images` directory of the toolbox, for all the languages at once. `tbx_package()` ships them with the toolbox.
- `uiimport()` has been rewritten with the "browser" uicontrol and now supports XLSX files. Besides a `table` or a `timeseries`, data can now be imported as column vectors, a matrix of doubles, a matrix of strings or a `cell`.
- The news feed is now web-based, and file choosers have a more modern look on all operating systems.
- The localization of a toolbox (`tbx_build_localization()`) now also extracts the messages of the `classes/` directory.
- `assert_checkalmostequal()`, `assert_checkequal()`, `assert_checkfalse()`, `assert_checkfilesequal()` and `assert_checktrue()` accept an optional custom error message as last input argument.
- `compress()` and `decompress()` now support "zstd" compression.
- `parquetRead()` and `parquetWrite()` now manage `timeseries`, and `parquetWrite()` accepts an optional compression argument ("gzip", "snappy", "zstd" or "uncompressed").
- `pca()` now accepts an "Algorithm" option ("svd" or "eig").
- `summary()` now supports matrices of doubles, integers, booleans, strings, durations and datetimes.

### Language evolutions

- Comparison operators `<`, `<=`, `>` and `>=` and element-wise logical operators `&` and `|` now perform the same **automatic expansion** as arithmetic operators.
- `classdef`: a class name can be used as type of a function argument (`arguments` block) or of a property; it accepts an object of this class or of a derived one.
- `classdef`: validator parameters are evaluated at call time: a field of an input (`opts.max`), and in methods `this`, a property of the object or a static property.
- `classdef`: `&&` and `||` call the `and`/`or` methods of objects, and overloaded operators are searched in both operands. Redefining a class rebuilds the loaded classes derived from it.

### Graphics

- Transparency is managed with new `*_alpha` properties for Polyline, Surface (Plot3d & Fac3d), Fec, Grayplot, Matplot, Champ, Segs, Arc, Rectangle, Text, Label, Legend and Datatip entities, and with new "...Alpha" options in `plot()`, `surf()`, `scatter()` and `scatter3d()`.
- New `Root` and `Console` graphic objects: `get(0)` returns the `Root` object whose children are the figures and the `Console`.
- Champ entities have a new `arrow_style` property ("filled" or "vback").
- `grayplot()`, `Sgrayplot()` and `Sfgrayplot()` now accept non-rectangular (curvilinear) grids given as matrices of the same size.
- `stackedplot()` can be used with subplots, accepts an axes handle and returns the handles of the created axes.

### Xcos

- The former fmu-wrapper ATOMS toolbox is now the built-in `fmu` module: import of FMI 1/2/3 FMUs (`importFMU()`, `fmiSimulate()`, `fmi*()` functions) and `FMU_WRAPPER` block, used when loading SSP files.
- SSP files keep the names and order of ports and can be saved with any superblock.
- The Modelica initialization GUI works again (Solve, Fix states & Fix derivatives).

Obsolete functions or features
------------------------------

All these functions and features will be removed in Scilab 2027.1.0 (May 2028):

- `tbx_make(tbx_path, "help", languages)`: the `languages` option is obsolete, the images of the help pages are generated for all languages at once at build time, or on the fly when missing.
- `xmltojar()`: no jar is built anymore, it only pre-generates the images of the help pages. Use `tbx_build_help()` instead.

Removed Functions
-----------------

The following functions have been removed:

- `dos()`: obsolete since 2026.0.0, please use `host()` instead.
- `readmps()`: obsolete since 2026.1.0, please use `quapro` toolbox instead.
- `unix()`: obsolete since 2026.0.0, please use `host()` instead.
- `unix_g()`: obsolete since 2026.0.0, please use `host()` instead.
- `unix_s()`: obsolete since 2026.0.0, please use `host()` instead.
- `unix_w()`: obsolete since 2026.0.0, please use `host()` instead.
- `unix_x()`: obsolete since 2026.0.0, please use `host()` instead.
- `Param3d` and `Title` graphic entities documentation and remaining management: these entities were no more created since Scilab 5.

Known incompatibilities
-----------------------

- Help jars (`scilab_<lang>_help.jar`) are no longer built, neither for Scilab nor for toolboxes. A toolbox whose `etc/*.start` registers its help chapter only when its `jar` directory exists, as the former toolbox skeleton did, must now test its `help` directory, and can give its root directory to `add_help_chapter()`.
- `xmltoformat()` no longer supports the `"javaHelp"` format, and the JavaHelp library (`jhall.jar`) is no longer needed.
- Many warning messages formerly printed as regular output (near-singularity in `inv()`/`spec()`/`freq()`/`rtitr()`, solver diagnostics in `ode()`/`odedc()`/`intg()`/`int2d()`/`int3d()`, function redefinition, `mopen()`, `createdir()`, uicontrols...) now go through the standard warning channel: they are hidden by `warning("off")` and raise an error under `warning("stop")`.
- `classdef`: the constructor of a parent class is now called implicitly, without argument, when the constructor of the derived class does not call it (or when the derived class has no constructor). A parent constructor that needs arguments must be called explicitly, a class with a private constructor can no longer be derived, and `A()` is only allowed in the constructor of a direct subclass.
- `classdef`: protected members are visible to all derived classes (they were visible to direct subclasses only), and a method can access private members of other instances of its class. A constructor declared in a `methods (Static)` block is an error.
- `classdef`: unknown types in `arguments` blocks are reported at the first call instead of at definition.
- `findobj()` starts its search from the `Root` object: depths are shifted by one, `Root` and `Console` objects can be returned, and a property name given without value returns `[]` instead of an error.
- `calendarDuration` display uses "mo" for months (instead of "m", used for minutes): `1y 2mo 3d 12h 35m 30s`.
- `cutaxes()`: the continuous axis is now drawn by `Axes(1)` instead of `Axes($)`.
- `stackedplot()` no longer creates a new figure on each call.
- `xlsxWrite()` refuses to write into an existing file it cannot read (it formerly replaced its content by a blank workbook).
- A call with a single string argument followed by an assignment (e.g. `legend("a").font_size = 3`) is no more parsed as a dynamic field access `legend.a` when `legend` is a function.
- The `demo_tools` module has been merged into the `helptools` module.

Compilation
-----------

- Windows: Migration to Visual Studio 2026, Intel® oneAPI HPC Toolkit 2026 and Inno Setup 7.
- Linux: binaries are built with GCC 12 to be compatible with Ubuntu 22.04 libstdc++.
- Windows ARM64 support added (no official binaries available).

If you are familiar with building Scilab from sources, these dependencies have been updated:

- Java 21 (or more recent) is now required (instead of Java 17).
- Scilab now uses Eigen 5.0.1.
- Scilab now uses SWIG 4.5.1.
- Scilab is packaged with JCEF/CEF based on Chromium 146.0.7680.179.
- MATIO 1.6.0 is now supported.
- The embedded copy of ezxml has been removed: libxml2 is used instead.
- JavaHelp (`jhall.jar`) and OpenXLSX are no longer needed.
- OCaml is now detected on Fedora.

Packaging & Supported Operating Systems
---------------------------------------

- To run or compile Scilab, you might need:
  - Windows (amd64):
    - Windows 11 (Desktop)
    - Windows 10 (Desktop)
  - macOS:
    - M1-based Mac running macOS 11+ (compile and run)
    - Intel-based Mac running macOS 11+ (compile and run)
  - Linux (amd64):
    - debian: 13
    - ubuntu: 22.04, 24.04, 25.04
    - fedora: 42

Issue Fixes
-----------

### Scilab 2027.0.0

- [#790](https://gitlab.com/scilab/scilab/-/issues/790): `findobj()` did not manage wrong key/value sequences and returned its results in a wrong order.
- [#6053](https://gitlab.com/scilab/scilab/-/issues/6053): It was not possible to use transparent colors in graphics.
- [#7814](https://gitlab.com/scilab/scilab/-/issues/7814): `classmarkov()` called the removed `mat_2_graph()` function.
- [#14202](https://gitlab.com/scilab/scilab/-/issues/14202): Logarithmic scale made Scilab crash (infinite loop) when using extreme bounds.
- [#15872](https://gitlab.com/scilab/scilab/-/issues/15872): Scilab crashed when `HOME` was not set or when `HOME/.Scilab/` was write protected.
- [#16088](https://gitlab.com/scilab/scilab/-/issues/16088): `spec()` could return an error (or even crash) when called with complex matrices having imaginary part equal to zero.
- [#16610](https://gitlab.com/scilab/scilab/-/issues/16610): Lighting did not apply to Fac3d surfaces colored by values.
- [#16621](https://gitlab.com/scilab/scilab/-/issues/16621): `get(0, "children")` returned an error while `c = get(0); c.children` worked.
- [#16847](https://gitlab.com/scilab/scilab/-/issues/16847): Setting `mark_foreground` with a vector did not update individual marker colors.
- [#16856](https://gitlab.com/scilab/scilab/-/issues/16856): Matplot entries outside the colormap (including 0) were drawn.
- [#17148](https://gitlab.com/scilab/scilab/-/issues/17148): `timeseries` tab completion was misleading.
- [#17201](https://gitlab.com/scilab/scilab/-/issues/17201): An interrupted and resumed loop gave access to the interactive command prompt.
- [#17258](https://gitlab.com/scilab/scilab/-/issues/17258): `make -C modules/javasci check` failed.
- [#17367](https://gitlab.com/scilab/scilab/-/issues/17367): `axes_reverse` inverted the move orientation when zoomed.
- [#17402](https://gitlab.com/scilab/scilab/-/issues/17402): It was not possible to stop a script at a chosen line from SciNotes.
- [#17424](https://gitlab.com/scilab/scilab/-/issues/17424): Labels & title rendering was wrong in the fourth image of `cutaxes()` documentation.
- [#17498](https://gitlab.com/scilab/scilab/-/issues/17498): Scilab sometimes crashed when running code with multiple parser calls in a loop (Riemann surface demonstration).
- [#17516](https://gitlab.com/scilab/scilab/-/issues/17516): `abort` killed Scilab after a break with Ctrl+C in a simulation.
- [#17523](https://gitlab.com/scilab/scilab/-/issues/17523): `xcos(scs_m)` opened an empty diagram.
- [#17527](https://gitlab.com/scilab/scilab/-/issues/17527): The embedded copy of ezxml, which had security issues, has been replaced by libxml2.
- [#17554](https://gitlab.com/scilab/scilab/-/issues/17554): `legend(..).font_size = 3` returned an unexpected error or crashed.
- [#17555](https://gitlab.com/scilab/scilab/-/issues/17555): `figure()` produced an incorrect error message.
- [#17556](https://gitlab.com/scilab/scilab/-/issues/17556): Under Windows, Scilab 2026.1.0 console (and opened figures) size was reduced when opening the ATOMS GUI.
- [#17558](https://gitlab.com/scilab/scilab/-/issues/17558): `summary()` of a table with selected variables returned incoherent results.
- [#17559](https://gitlab.com/scilab/scilab/-/issues/17559): `table()` and `timeseries()` did not accept a column vector for "VariableNames".
- [#17561](https://gitlab.com/scilab/scilab/-/issues/17561): `summary()` did not skip `NaN` values when computing the sum of a table variable.
- [#17562](https://gitlab.com/scilab/scilab/-/issues/17562): Some values no more worked as "Marker" value when calling `plot()` since Scilab 2024.0.0.
- [#17563](https://gitlab.com/scilab/scilab/-/issues/17563): Displaying a cell containing strings longer than the console width raised an error.
- [#17567](https://gitlab.com/scilab/scilab/-/issues/17567): `kmeans()` sometimes returned a dimension mismatch error.
- [#17568](https://gitlab.com/scilab/scilab/-/issues/17568): Xcos scope window name lost its accented characters.
- [#17571](https://gitlab.com/scilab/scilab/-/issues/17571): Xcos block unique name lost digits and accents.
- [#17574](https://gitlab.com/scilab/scilab/-/issues/17574): `y1` attribute of `SystemGeometry` element was wrong in SSP files saved by Xcos.
- [#17575](https://gitlab.com/scilab/scilab/-/issues/17575): Loading an SSP file created by easySSP made Scilab crash.
- [#17577](https://gitlab.com/scilab/scilab/-/issues/17577): Scilab could not be compiled using MATIO 1.6.0.
- [#17578](https://gitlab.com/scilab/scilab/-/issues/17578): `xlsxInfo()` and `xlsxRead()` failed to read sheets whose relationships referenced drawing parts absent from the file.
- [#17581](https://gitlab.com/scilab/scilab/-/issues/17581): `CMSCOPE` label did not always set the name of the graphic window.
- [#17590](https://gitlab.com/scilab/scilab/-/issues/17590): `arguments`: vectors were not transposed for size specifications with a colon, e.g. `(:,1)` and `(1,:)`.
- [#17594](https://gitlab.com/scilab/scilab/-/issues/17594): Setting `log_flags` after `Matplot()` made Scilab crash.
- [#17600](https://gitlab.com/scilab/scilab/-/issues/17600): Xcos crashed when loading Scilab 5.5.2 diagrams with non-contiguous port ordering.
- [#17601](https://gitlab.com/scilab/scilab/-/issues/17601): `classdef` with static properties and methods mixed with other ones caused unexpected failures.

