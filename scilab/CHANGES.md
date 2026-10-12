Welcome to Scilab 2026.1.0
==========================

This file details the changes between Scilab 2026.1.0 (this version) and the previous 2025.1.0 stable release.

For changelogs of earlier releases, please see [Scilab 2025.1.0][1].

Summary:

- New features
- Obsolete functions & features
- Removed functions & features
- Known incompatibilities
- Compilation & Packaging
- Fixed issues

Please report anything we may have missed on [Discourse][2] or [GitLab][3].

[1]: https://help.scilab.org/docs/2025.1.0/en_US/CHANGES.html
[2]: https://scilab.discourse.group/
[3]: https://gitlab.com/scilab/scilab/-/issues

New features
------------

For a high-level description of the main new features in this release, please consult the homepage of the embedded documentation, available by simply typing `doc` in the Scilab console.

### Scilab 2027.0.0

#### Features improvements

- The help browser, now based on JCEF (Chromium Embedded Framework), replaces JavaHelp: it renders the XML help pages directly, with highlighted code and LaTeX & MathML equations rendered by MathJax. The images of the examples are pre-generated, or generated on demand when they are not.
- The help of a toolbox is also rendered from its XML pages: `tbx_build_help()` no longer builds a help jar, but pre-generates the images of the pages (`<scilab:image>` blocks and Xcos diagrams) into the `help/images` directory of the toolbox, for all the languages at once. `tbx_package()` ships them with the toolbox.
- SciNotes has an AI assistant: a side panel (toolbar button or `Window > Scilab Assistant` menu) where an AI coding agent writes Scilab code, runs it in the session, checks the figures it produces and edits the file open in the editor. It talks through the Agent Client Protocol (ACP) to an AI agent installed and logged in by the user: Claude, OpenAI Codex, GitHub Copilot, Cursor, Google Antigravity, Mistral Vibe, Gemini CLI, or any other ACP agent added in its settings. The agent asks before changing files, running commands or browsing the web. Domain skills (graphics, GUIs, data import, signal processing, control, simulation, optimization, debugging, toolboxes, classdef, arguments) are shipped, and users can add their own. See `doc scinotes_assistant`.
- The localization of a toolbox (`tbx_build_localization()`) now also extracts the messages of the `classes/` directory.

### Scilab 2026.1.0

#### New functions

- `check_help()`: Validate Scilab XML help files against the Scilab Relax NG schema.
- `help()`: Display an inline summary of a documentation page in Scilab console.
- `ishermitian()`: Check whether a matrix is hermitian.
- `issymmetric()`: Check whether a matrix is symmetric.
- `parquetRead()`: Read a Parquet or Arrow file and return a table.
- `parquetWrite()`: Write a table in a Parquet or Arrow file.
- `pdist()`: Pairwise distances between observations.
- `pdist2()`: Pairwise distances between two sets of observations.
- `squareform()`: Conversion between a distance vector and a symmetric distance matrix.
- `summary()`: Summary of `table` or `timeseries` variables (properties, statistics...).
- `xcosDiagramExport()`: Export an Xcos diagram to an image.
- `xlsxInfo()`: Get information about an Excel file.
- `xlsxRead()`: Read data from an Excel file and return a matrix or cell.
- `xlsxSheet()`: Manage sheets in an Excel file.
- `xlsxWrite()`: Write data (matrix or cell) to an Excel file.

#### Features improvements

- Console: warnings and errors are now visually distinct from regular output (dedicated colors, in both the console and the terminal where errors go to stderr). Colors and bold/italic styles can be customized in Preferences ⇒ Colors.
- New web-based GUIs for ATOMS & Scilab Demonstration using "browser" uicontrols.
- Objects defined using `classdef` now support static properties and methods.
- `csvTextScan()` now manages a "substitute" option as `csvRead()` to enable replacement of particular strings in the read file.
- `dbscan()` now accepts two more input arguments to specify metric distances and parameters.
- `dbscan()`, `kmeans()` and `meanshift()` now use `pdist2()` for better performances.
- `format()` now supports engineering notation (similar to exponential notation but with an exponent value divisible by three).
- `gallery()` can now generate "minij" (Symmetric positive definite matrix) and "moler" (Moler symmetric positive definite matrix) test matrices.
- "use_vectorized" parameter is now available for `optim_ga()`, `optim_moga()`, `optim_nsga()`, `optim_nsga2()` to indicate that the cost function is vectorized (called with the whole population at once).

#### Language evolutions

- Arithmetic operators now perform **automatic expansion** with no memory overhead: a singleton dimension is automatically replicated to match the other operand, making manual expansion (using `.*.` or `repmat()`) unnecessary for most case. See `doc operator_expand`.
- Comparison operators `<`, `<=`, `>` and `>=` and element-wise logical operators `&` and `|` now perform the same **automatic expansion** as arithmetic operators.

### Scilab 2026.0.0

#### New functions

- `cdftnc()`: Cumulative distribution function of the non-central student's T distribution.
- `dbscan()`: Density-based clustering.
- `enumeration()`: Get the enumeration of a classdef or an object.
- `estimate_bandwidth()`: Estimate an appropriate bandwidth for mean shift clustering.
- `gallery()`: Generate test matrices.
- `gradient()`: Compute numerical gradient.
- `isa()`: Check variable type.
- `meanshift()`: Mean shift clustering algorithm.
- `methods()`: Get accessible methods of a classdef or an object.
- `properties()`: Get accessible properties of a classdef or an object.
- `sortrows()` : Sort rows of a vector, matrix, table, or timeseries.
- `spset()`: Set non-zero entries of a sparse matrix.

#### Features improvements

- `host()` has been rewritten and is now used as a backend for all other functions that perform system calls (`dos()`, `unix()`, `unix_g()`, `unix_s()`, `unix_w()`, and `unix_x()`) which are now obsolete.
- `intersect()` now manages `duration`, `datetime`, `table`, and `timeseries` types.
- `lib()` can now load a library without exposing its symbols (default behavior remains unchanged).
- `setdiff()` now manages `duration`, `datetime`, `table`, and `timeseries` types.
- `table()` and `timeseries()` now manage integers.
- `union()` now manages `duration`, `datetime`, `table`, and `timeseries` types.
- Options names are now case-insensitive for `duration()`, `calendarDuration()`, `datetime()`, `timeseries()`, and `table()` functions.

#### Language evolutions

- Classes/Objects can now be defined and used in Scilab based on the new `classdef`, `enumeration`, `properties` & `methods` keywords.
- The `end` keyword can now be used to specify the last row/column index of an array, similarly to `$`.

#### Graphics

- Axes handles can now have their own colormap. If not provided, then the parent figure colormap is used (as in previous versions).

#### Xcos

- Diagrams are saved as [SSP](https://ssp-standard.org/) (System Structure and Parametrization) files by default. This setting can be edited in user preferences.
- Users are now enabled to save their diagrams as COSF files.
- COS format is no more associated with Xcos under Windows.
- SSP files are associated with Scilab during installation under Windows.

Obsolete functions or features
------------------------------

### Scilab 2027.0.0

All these functions and features will be removed in Scilab 2027.1.0 (May 2028):

- `tbx_make(tbx_path, "help", languages)`: the `languages` option is obsolete, the images of the help pages are generated for all languages at once at build time, or on the fly when missing.
- `xmltojar()`: no jar is built anymore, it only pre-generates the images of the help pages. Use `tbx_build_help()` instead.

### Scilab 2026.1.0

All these functions and features will be removed in Scilab 2027.0.0 (October 2026):

- `readmps()`: Please use `quapro` toolbox instead.

### Scilab 2026.0.0

All these functions and features will be removed in Scilab 2026.1.0 (May 2026):

- `demo_begin()`: Please use `exec()` instead.
- `demo_choose()`: Please use `x_choose()` instead.
- `demo_compiler()`: Please use `haveacompiler()` instead.
- `demo_end()`: Please use `exec()` instead.
- `demo_file_choice()`: Please use `x_choose()` instead.
- `demo_function_choice()`: Please use `x_choose()` instead.
- `demo_run()`: Please use `exec()` instead.

All these functions and features will be removed in Scilab 2027.0.0 (October 2026):

- `dos()`: Please use `host()` instead.
- `unix()`: Please use `host()` instead.
- `unix_g()`: Please use `host()` instead.
- `unix_s()`: Please use `host()` instead.
- `unix_w()`: Please use `host()` instead.
- `unix_x()`: Please use `host()` instead.

Removed Functions
-----------------

### Scilab 2026.1.0

The following functions have been removed:

- `check_modules_xml()`: not used and undocumented.
- `demo_begin()`: obsolete since 2026.0.0, please use `exec()` instead.
- `demo_choose()`: obsolete since 2026.0.0, please use `x_choose()` instead.
- `demo_compiler()`: obsolete since 2026.0.0, please use `haveacompiler()` instead.
- `demo_end()`: obsolete since 2026.0.0, please use `exec()` instead.
- `demo_file_choice()`: obsolete since 2026.0.0, please use `x_choose()` instead.
- `demo_function_choice()`: obsolete since 2026.0.0, please use `x_choose()` instead.
- `demo_run()`: obsolete since 2026.0.0, please use `exec()` instead.

### Scilab 2026.0.0

The following functions have been removed:

- `demo_folder_choice()`: undocumented and not used, replaced by `x_choose()`.
- `lft(P, p, R, r)`: obsolete since 2025.0.0, no more supported.
- `h2norm(Sl [,tol])` (with `Sl` a matrix of doubles): obsolete since 2025.0.0, no more supported.
- `linf(g [,eps, tol])` (with `g` a matrix of doubles): obsolete since 2025.0.0, no more supported.
- `nicholschart(modules,, colors)` (syntax with skipped arguments): obsolete since 2025.0.0, no more supported.
- `st_ility(Sl [,tol])` (with `Sl` a matrix of doubles): obsolete since 2025.0.0, no more supported.
- `syssize(Sl)` (with `Sl` a matrix of doubles): obsolete since 2025.0.0, no more supported.
- `help()`: obsolete since 2025.0.0, please use `doc()` instead.
- `daskr()`: obsolete since 2024.1.0, please use `dae()` instead.
- `dasrt()`: obsolete since 2024.1.0, please use `dae()` instead.
- `dassl()`: obsolete since 2024.1.0, please use `dae()` instead.
- `impl()`: obsolete since 2025.0.0, please use `dae()` instead.
- `testmatrix()`: obsolete since 2025.0.0, please use `magic()`, `invhilb()` or `frank()` instead.
- `captions()`: obsolete since 2025.0.0, please use `legend()` instead.
- `figure_style` property: obsolete since 2025.1.0, no more supported.
- `princomp()`: obsolete since 2025.0.0, please use `pca()` instead.

Known incompatibilities
-----------------------

### Scilab 2027.0.0

- Help jars (`scilab_<lang>_help.jar`) are no longer built, neither for Scilab nor for toolboxes. A toolbox whose `etc/*.start` registers its help chapter only when its `jar` directory exists, as the former toolbox skeleton did, must now test its `help` directory, and can give its root directory to `add_help_chapter()`.
- `xmltoformat()` no longer supports the `"javaHelp"` format, and the JavaHelp library (`jhall.jar`) is no longer needed.

### Scilab 2026.1.0

- Many warning messages formerly printed as regular output (near-singularity in `inv()`/`spec()`/`freq()`/`rtitr()`, solver diagnostics in `ode()`/`odedc()`/`intg()`/`int2d()`/`int3d()`, function redefinition, `mopen()`, `createdir()`, uicontrols...) now go through the standard warning channel: they are hidden by `warning("off")` and raise an error under `warning("stop")`.
- `readtable()` & `readtimeseries()` now also return empty columns (columns with missing or undefined names).
- In toolboxes, when using custom "builder_<lang>.sce" files in "src" folder, `tbx_build_src_clean()` must be called to be sure "cleaner<lang>.sce" files are generated.

### Scilab 2026.0.0

- `genlib()` no more loads generated library (see [#15918](https://gitlab.com/scilab/scilab/-/issues/15918)). `lib()` function must be called to load the generated library.
- `host()` function now returns `0` (success) instead of `1` (error) when called with an empty character string as input.

Compilation
-----------

### Scilab 2026.1.0

- Windows ARM64 support added (no official binaries available).
- Under Linux/macOS, installation of documentation (.xml files) was optional before this version (using a dedicated configure option). This option has been removed as .xml files are now mandatory for the `help()` function.

If you are familiar with building Scilab from sources, these dependencies have been updated:

- New dependency: libxslt >= 1.1.35 (used by `help()`).
- New dependency: XLNT >= 1.6.1 (used by `xlsxInfo()`, `xlsxRead()`, `xlsxSheet()` & `xlsxWrite()`).
- New dependency: Apache Arrow 19.0.0 with Parquet format & ZLIB support (used by `parquetRead()` & `parquetWrite()`).

### Scilab 2026.0.0

- Windows: Migration to Intel® oneAPI HPC Toolkit 2025.2.
- Linux: GCC 15 is now supported.

If you are familiar with building Scilab from sources, the following dependencies have been updated.

- Required API version of JCEF updated to 130.1.9 (Scilab is packaged with version 135.0.20).
- PCRE2 10.43 (or more recent) is now required (instead of PCRE1).
- Scilab now uses SUNDIALS 7.4.
- Under Windows, Scilab now uses HDF5 1.14.3 instead of 1.14.4 (see [#17441](https://gitlab.com/scilab/scilab/-/issues/17441)).

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

### Scilab 2026.1.0

- [#9985](https://gitlab.com/scilab/scilab/-/issues/9985): `lsqrsolve()` produced weird messages when the objective function was complex.
- [#14387](https://gitlab.com/scilab/scilab/-/issues/14387): `string(cell_array)` returned wrong results (wrong sizes, wrong type, wrong contents).
- [#14776](https://gitlab.com/scilab/scilab/-/issues/14776): Scilab crashed when an `AFFICH_m` block had a wrong input size.
- [#15101](https://gitlab.com/scilab/scilab/-/issues/15101): `ascii()` returned inconsistent results for `0` value.
- [#15773](https://gitlab.com/scilab/scilab/-/issues/15773): `eigs()` could not compute all eigenvalues of singular matrices.
- [#16501](https://gitlab.com/scilab/scilab/-/issues/16501): UID of a Xcos block did not update after pasting a block into another super block.
- [#16739](https://gitlab.com/scilab/scilab/-/issues/16739): Xcos labels boxes were corrupted in Xcos diagram coming from Scilab 5.5.2.
- [#16744](https://gitlab.com/scilab/scilab/-/issues/16744): `SUPER_f` destroyed port names in a duplicated diagram.
- [#17334](https://gitlab.com/scilab/scilab/-/issues/17334): `loadToolboxInlineHelp()` (new in Scilab 2025.0.0) was not documented and broke compatibility with Scilab 2024.1.0 (`loader.sce`, ...).
- [#17359](https://gitlab.com/scilab/scilab/-/issues/17359): A copy-paste of the `SELF_SWITCH` component generated an empty square in Xcos diagram.
- [#17394](https://gitlab.com/scilab/scilab/-/issues/17394): `loadToolboxInlineHelp()` could not find file containing links.
- [#17451](https://gitlab.com/scilab/scilab/-/issues/17451): Accessing `.content` field of `xmlXPath()` output (and the `XMLSet` datatype) could lead to 0-sized datatypes.
- [#17483](https://gitlab.com/scilab/scilab/-/issues/17483): Xcos diagram saved to SSP format did not keep `IN_f`/`OUT_f` labels.
- [#17485](https://gitlab.com/scilab/scilab/-/issues/17485): Compiling against `mex.h` generated an error when using C++23.
- [#17486](https://gitlab.com/scilab/scilab/-/issues/17486): All example is `xml` module documentation overwrote the `doc` command.
- [#17487](https://gitlab.com/scilab/scilab/-/issues/17487): macOS arm64 version needed Rosetta at runtime.
- [#17489](https://gitlab.com/scilab/scilab/-/issues/17489): Display of structs with objects generated warnings.
- [#17494](https://gitlab.com/scilab/scilab/-/issues/17494): The first step of `arkode` was incorrect for Euler method.
- [#17495](https://gitlab.com/scilab/scilab/-/issues/17495): `getVsWhereInformation()` failed when `vswhere` command returned `[]`.
- [#17499](https://gitlab.com/scilab/scilab/-/issues/17499): Line value of XML elements was wrong for bug XML files.
- [#17501](https://gitlab.com/scilab/scilab/-/issues/17501): `strsplit()` was broken in some cases in Scilab 2026.0.0.
- [#17504](https://gitlab.com/scilab/scilab/-/issues/17504): Hypermatrix display was wrong when the number of lines was 100 or more.
- [#17505](https://gitlab.com/scilab/scilab/-/issues/17505): It was not possible to modify data Xcos boxes since Scilab 2025.0.
- [#17506](https://gitlab.com/scilab/scilab/-/issues/17506): `scatterplot()` always displayed black marks when using `0` as marker type.
- [#17509](https://gitlab.com/scilab/scilab/-/issues/17509): An error in the methods of `classdef` object returned an empty function name.
- [#17510](https://gitlab.com/scilab/scilab/-/issues/17510): `classdef` properties were not shown correctly.
- [#17512](https://gitlab.com/scilab/scilab/-/issues/17512): `NaT` was not always consistent with comparison operators.
- [#17514](https://gitlab.com/scilab/scilab/-/issues/17514): Xcos electrical block parameter values did not update the icon.
- [#17522](https://gitlab.com/scilab/scilab/-/issues/17522): `csvRead()`, `csvTextscan()` were not able to handle a mix of whitespaces and semi-column separators.
- [#17525](https://gitlab.com/scilab/scilab/-/issues/17525): There was a mistake on `TEXT_f` block help page.
- [#17526](https://gitlab.com/scilab/scilab/-/issues/17526): Scilab failed when reading "DATASPACE SCALAR" attribute or dataset in HDF5 file.
- [#17528](https://gitlab.com/scilab/scilab/-/issues/17528): `csim()` could give incorrect results when called with initial conditions.
- [#17530](https://gitlab.com/scilab/scilab/-/issues/17530): `ESELECT_f` documentation did not explain which output value was selected according to the input value.
- [#17532](https://gitlab.com/scilab/scilab/-/issues/17532): `LOGICAL_OP` block symbol did not change according to settings.
- [#17533](https://gitlab.com/scilab/scilab/-/issues/17533): Vertices of connections may exchange their position when reloading an Xcos diagram.
- [#17535](https://gitlab.com/scilab/scilab/-/issues/17535): `cvode()` did not honor `maxSteps` in some cases.
- [#17538](https://gitlab.com/scilab/scilab/-/issues/17538): Error was signalled on original superblock (instead of copy) when doing a copy in Xcos.
- [#17539](https://gitlab.com/scilab/scilab/-/issues/17539): `readtable()` did not return columns whose names were empty.
- [#17540](https://gitlab.com/scilab/scilab/-/issues/17540): `cvode()` froze Scilab when the time span was specified as a two-element vector.
- [#17541](https://gitlab.com/scilab/scilab/-/issues/17541): Documentation of `arguments` was confusing in some cases.
- [#17542](https://gitlab.com/scilab/scilab/-/issues/17542): Depending on operations, `classdef` objects behaved differently with other types.
- [#17543](https://gitlab.com/scilab/scilab/-/issues/17543): `slint()` shew copyright/advertising notice.
- [#17548](https://gitlab.com/scilab/scilab/-/issues/17548): There was a mispelling in the figure menus when translated into French.
- [#17550](https://gitlab.com/scilab/scilab/-/issues/17550): `tbx_build_src()` changed the order of files at compile time, which could lead to compilation errors when dealing with Fortran 90 files.

### Scilab 2026.0.0

- [#7113](https://gitlab.com/scilab/scilab/-/issues/7113): `demo_compiler()` was a useless wrapper for `haveacompiler()` and has been removed.
- [#7258](https://gitlab.com/scilab/scilab/-/issues/7258): There were 8 functions to run an operating-system command, all have been merged in new `host()` function.
- [#8212](https://gitlab.com/scilab/scilab/-/issues/8212): Some deprecated functions such as `demo_begin()` and `demo_end)` were no longer maintained; they are now tagged as obsolete.
- [#12955](https://gitlab.com/scilab/scilab/-/issues/12955): `Matplot()` extension to (#,#,3) true colors ND-arrays was not documented.
- [#13260](https://gitlab.com/scilab/scilab/-/issues/13260): There was no CDF for the non central student distribution.
- [#13875](https://gitlab.com/scilab/scilab/-/issues/13875): `spset(A, v)`, dual of `[ij, v]=spget(A)` was missing.
- [#14713](https://gitlab.com/scilab/scilab/-/issues/14713): `demo_run(file)` was useless and no longer maintained; it is now tagged as obsolete.
- [#14790](https://gitlab.com/scilab/scilab/-/issues/14790): The "Axes" `ticks_format` and `ticks_st properties` were no more taken into account.
- [#15214](https://gitlab.com/scilab/scilab/-/issues/15214): Colormaps can now be assigned to "Axes" handles.
- [#15442](https://gitlab.com/scilab/scilab/-/issues/15442): `printf()` did not handle "uint64" integers greater than 2^32-1.
- [#15918](https://gitlab.com/scilab/scilab/-/issues/15918): `mylib = lib(libdir)` registered functions in the default library instead of in the one given as output argument.
- [#16074](https://gitlab.com/scilab/scilab/-/issues/16074): `msprintf("%ld\n", i)` and `mprintf("%ld\n", i)` appended some "d" for "int64" or "uint64" inputs.
- [#16546](https://gitlab.com/scilab/scilab/-/issues/16546): `cdft()`, T-distribution, returned wrong values when used with a low degree of freedom.
- [#17089](https://gitlab.com/scilab/scilab/-/issues/17089): Scilab now uses PCRE2 for regular expression support (instead of deprecated PCRE 1.3 version).
- [#17240](https://gitlab.com/scilab/scilab/-/issues/17240): `unix_g()` did not read standard error output when exit code was 0 or 1 and did not read stdout when exit code was 2 or more.
- [#17242](https://gitlab.com/scilab/scilab/-/issues/17242): SciPowerlab toolbox did not work with Scilab 2024.0.0.
- [#17243](https://gitlab.com/scilab/scilab/-/issues/17243): Xcos sometimes displayed an error about port size or type.
- [#17357](https://gitlab.com/scilab/scilab/-/issues/17357): `demo_file_choice()` was no longer maintained; it is now tagged as obsolete.
- [#17378](https://gitlab.com/scilab/scilab/-/issues/17378): Variables returned by `jarray()` could not be used/initialized.
- [#17391](https://gitlab.com/scilab/scilab/-/issues/17391): `csvRead()` was extremely slow to detect errors in column structure.
- [#17410](https://gitlab.com/scilab/scilab/-/issues/17410): Most recent version of FORTRAN OneAPI can now be detected and used by Scilab.
- [#17432](https://gitlab.com/scilab/scilab/-/issues/17432): Simple `table()` Matlab example did not work in Scilab.
- [#17435](https://gitlab.com/scilab/scilab/-/issues/17435): `table()` creation did not support empty matrices.
- [#17436](https://gitlab.com/scilab/scilab/-/issues/17436): Context was empty in "Water tank" demonstration and now contains variables needed for simulation.
- [#17437](https://gitlab.com/scilab/scilab/-/issues/17437): If the "Find/Replace" window of SciNotes was opened when Scilab was closed, it could not be closed.
- [#17438](https://gitlab.com/scilab/scilab/-/issues/17438): Scilab could not be compiled using GCC 15.
- [#17439](https://gitlab.com/scilab/scilab/-/issues/17439): Annotations were not supported for Xcos links.
- [#17441](https://gitlab.com/scilab/scilab/-/issues/17441): `xsave()` and `save()` no more worked with accented letters in the filename since Scilab 2025.0.0 on Windows.
- [#17442](https://gitlab.com/scilab/scilab/-/issues/17442): Documentation example for installing an ATOMS module from a file did not work.
- [#17443](https://gitlab.com/scilab/scilab/-/issues/17443): Changing the input/output format of duration object failed when forcing 'HH' to 24h format.
- [#17445](https://gitlab.com/scilab/scilab/-/issues/17445): `copyfile()` (used by `tbx_package()`) ddid not preserve symbolic links.
- [#17446](https://gitlab.com/scilab/scilab/-/issues/17446): `isvector()` documentation was wrong for scalar case.
- [#17447](https://gitlab.com/scilab/scilab/-/issues/17447): Some `java.nio.file.AccessDeniedException` errors were displayed by FileBrowser.
- [#17452](https://gitlab.com/scilab/scilab/-/issues/17452): `genlib()` made Scilab crash when macro code contained extra parentheses.
- [#17458](https://gitlab.com/scilab/scilab/-/issues/17458): Scilab could not be built against recent versions of JCEF.
- [#17459](https://gitlab.com/scilab/scilab/-/issues/17459): `std::from_chars` is now replaced by `fast_float::from_chars` for FreeBSD & macOS.
- [#17460](https://gitlab.com/scilab/scilab/-/issues/17460): Reading JSON files with empty objects made Scilab crash.
- [#17462](https://gitlab.com/scilab/scilab/-/issues/17462): `call_scilab` examples could not be built on recent GCC versions.
- [#17464](https://gitlab.com/scilab/scilab/-/issues/17464): `demo_function_choice()` was no longer maintained; it is now tagged as obsolete.
- [#17466](https://gitlab.com/scilab/scilab/-/issues/17466): An empty figure was drawn by `plot()` for data with a varying `X` and a close-to-constant `Y`.
- [#17468](https://gitlab.com/scilab/scilab/-/issues/17468): Scilab could not be built against recent versions libXML2 (>=2.14).
- [#17473](https://gitlab.com/scilab/scilab/-/issues/17473): Under Windows, background launch of Scilab created zombies.
- [#17477](https://gitlab.com/scilab/scilab/-/issues/17477): Since Scilab 2024.1.0, error reporting was broken when no `DISPLAY` variable was set.
- [#17478](https://gitlab.com/scilab/scilab/-/issues/17478): Compilation failed after SUNDIALS update.
- [#17479](https://gitlab.com/scilab/scilab/-/issues/17479): Inline documentation failed for non existing language documentation.
- [#17482](https://gitlab.com/scilab/scilab/-/issues/17482): Legend processing was broken if not all curves were given a string.
