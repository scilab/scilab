Welcome to Scilab 6.0.2
=======================

This file details the changes between Scilab 6.0.2 (this release) and the old stable release 5.5.2 (including information from 6.0.1 and 6.0.0).
For changelogs of earlier releases, please see [Scilab 5.5.2](https://www.scilab.org/sites/default/files/Scilab5.5.2_ReleaseNotes.pdf).

This file is intended for the specific needs of advanced users, and describes:
- High-level new features,
- Changes in installation, compilation, and compatibility,
- Changes in the language,
- New and modified features, in each module,
- Changes in functions (removed/added/modified),
- Bug fixes.

[1]: http://mailinglists.scilab.org
[2]: http://bugzilla.scilab.org


Main new features
-----------------

For a high-level description of the main new features of this release, please consult the [embedded help](modules/helptools/data/pages/homepage-en_US.html). It is also available as the "What's new" page of the help, by simply typing `help` in Scilab console.

In summary, the main new features are:
* New language parser and interpreter, ensuring:
  - Support for bigger data sets, thanks to dynamic memory usage. No need for `stacksize` anymore.
  - Better performance for some objects (cells and structs).
  - Clearer, less ambiguous, language syntax.
  - Executor reuse of specialization and custom engines.
  - Xcos also uses the new re-written Scilab engine.
* New code productivity features: full-featured debugger, profiler/coverage tool, and "lint"-like commands.
* Newsfeed, providing a continuous feed of news, tips, and general communication from the community and from Scilab Enterprises.
* Licensing change: Scilab is now released under the terms of the GNU General Public License (GPL) v2.0
It is still also available under the terms of the CeCILL v2.1.
* A `tbx_make()` function is added to build a toolbox following the toolbox directory structure convention
* ATOMS builder functions are now less dependent on the script files in the macros, `help`, `etc`, `src` and `sci_gateway` directories. These functions will do nothing (but warn the user) if they find no target to build:
  - `tbx_builder_macros`: compiles the `.sci` files present in `macros/` directory into the toolbox library. If `buildmacros.sce` or `builder.sce` script in `macros/` are present, executes them instead;
  - `tbx_builder_help`: compiles the help for each language (`la_LA`) directories in `help/`. If `builder_help` script is present in help, executes it instead;
  - `tbx_build_loader`: builds a default loader that mimics the skeleton `.start` files. If a `.start` file is found in `etc/` builds a loader calling this script;
  - `tbx_builder_src`: scans the subdirectories under `src` for builder files and executes them
  - `tbx_builder_gateway`: scans the subdirectories under `sci_gateway` for builder files and executes them


Windows starting changes:

The options `-nw` and `-nwni` are no longer supported by binaries (WScilex.exe, WScilex-cli.exe and Scilex.exe). Each binary already matches an execution mode and batch file `Scilab.bat` has been added to select the right binary depending on the option flag:

 * `scilab` starts `WScilex.exe` : Scilab with GUI and full functionnalities.
 * `scilab -nw` starts `WScilex-cli.exe`: Scilab CLI with full functionnalities.
 * `scilab -nwni` starts `Scilex.exe`: Scilab CLI without Java, Graphics, Xcos, etc.

Others flags are forwarded to binaries as before.


Installation
------------

* Under Windows, MKL packages are now included in Scilab installer and are not more downloaded anymore while installing Scilab.


Compilation
-----------

To build Scilab from sources, or to build extensions code (Toolboxes):
* A C++11 compliant compiler is now needed.
* Java 8 is now required to build Java code (version switch to 1.8).
* Ant minimal version switched to 1.9.0 (for Java 8 compatibility).
* ecj minimal version switched to 4.4.0 (for Java 8 compatibility).
* `--without-xcos` now only disable Xcos compilation. Xcos graphical interface is disabled using `--without-gui`.


Dependencies
------------

* Apache xmlgraphics stack upgraded to the latest versions:
  - xmlgraphics-commons 2.0.1
  - Batik 1.8
  - FOP 2.0


Packaging & Supported Operating Systems
---------------------------------------

* Scilab embedded JVM has been upgraded to Java 1.8. To run or compile Scilab you need at least:
  - Windows:
     - Windows 10
     - Windows 8 (Desktop)
     - Windows 7
     - Windows Vista SP2
     - Windows Server 2008 R2 SP1 (64-bit)
     - Windows Server 2012 (64-bit)
  - Mac OS X:
     - Intel-based Mac running Mac OS X version 10.14 (Mojave)
  - Linux:
     - Red Hat Enterprise Linux 5.5+, 6.x (32-bit), 6.x (64-bit), 7.x (64-bit)
     - Ubuntu Linux 18.04 LTS

    For more information, please consult: [What are the system requirements for Java?](http://java.com/en/download/help/sysreq.xml)

* [SSE2](https://en.wikipedia.org/wiki/SSE2), Streaming SIMD Extensions 2 support is now mandatory to run Scilab on Linux i686.


Language changes
----------------

Some small changes have been done to the language syntax, aiming at more clarity and less ambiguity. Some changes are not compatible with 5.5; code written for Scilab 5.x has to be modified in order to work in Scilab 6.0.

See [the wiki page on porting code from 5.5 to 6.0](https://wiki.scilab.org/FromScilab5ToScilab6) for details and examples.

* The parser now fully supports names of variables and functions of any length or/and including some UTF-8 multilingual characters.
* The name of the symbolic variable of polynomials and rationals can now be longer than 4 characters, and can include UTF-8 extended characters.
* Declaration of a number on two lines is no longer allowed: `1.. \n 2`.
* Declaration of an unclosed string on two lines is no longer allowed: `"abc..\ndef"` now yields an error.
* Comment blocks on multiple lines `/* ...\n ... \n ... */` are now possible.
* `1./M` is now parsed as `1 ./ M` instead of `1. / M`.
* `~` has now a priority higher than comparisons `==  ~=  <  <=  =>  >` one.
* Declaring strings using non-homogenous delimiters ("string' or 'string") is no longer allowed.
* `(a=b)` executed as `a == b` is now deprecated and returns an error.
* Function definitions can finish with `end` instead of `endfunction`.
* `{}` and `[]` are now distinct operators. Matrices can no longer be defined using `{}` operators. These are reserved for cell definition.
* Addition, subtraction, and inequality comparisons `<` `<=` `>=` `>` with an empty matrix now returns an empty matrix.
* New `&&` and `||` boolean AND and OR operators are introduced. They are shortcut in expressions even out of conditional tests, like `A = [1 %pi] || det([1 2])`
* The colon `: ` is now such that `type(:)==type(1:$)==129`, `typeof(:)==typeof(1:$)=='implicitlist'`, and `size(:)` is now undefined instead of `[-1,-1]`. In addition, `%i:10` is deprecated: Only real scalars can be used.
* Control and block keywords `break` `case` `catch` `continue` `do` `else` `elseif` `end` endfunction` `for` `function` `if` `resume` `return` `select` `then` `try` and `while` can no longer be overloaded as regular assignable variables as in `end=1`. They are now reserved and protected.
* `else` is no longer supported in a `while ... end` control instruction.
* `end` can no longer be directly followed by an instruction on the line without a `,` or `;` separator.
* Transposed matrix can now use the extraction operator directly `A'(1, 2)`.
* Function without output argument cannot be called in assignation expression anymore:
	```
	function foo()
		//...
	endfunction

	val = foo() // now returns an error
	```
* `varargout` is now initialized to `list()` by default.


Feature changes and additions
-----------------------------

Graphics:
* `scatter/scatter3` plot with different mark colors is now available.
* `parulacolormap` is now available.
* `name2rgb` can now handle a single string and a matrix of strings.
* `isoview`, `isoview on`, `isoview off`, `isoview(idGraphics, "on"|"off")` are now supported.
* `twinkle` and `twinkle(n)` are now supported: by default, the current element `gce` blinks.
* `replot` has been upgraded:
  - a bound set to `%inf` now gets the position of the most marginal object,
  - `replot` can now be used to reframe axes to all their contents,
  - option `tigh_limits` added,
  - Any handle having some Axes as direct children -- as uicontrol-frame -- is now supported.
* `colorbar` is upgraded:
  - various ways to set default minimal umin and/or maximal umax data values to be mapped are now implemented.
  - colminmax can now be specified as fractions of the colormap size.
  - colminmax can now implicitly map the specified [umin, umax] interval over the colormap size.
  - improved color bar sub-ticking.
* Datatips:
  - The property `z_component = 'on|off'` is renamed to `display_components = 'xyz'`.
  It is now possible to choose which components to display, and in which order.
  The `.z_component` property will be ignored in former `SCG` files.
  - A new `detached_position` property is available to display the datatip away from but linked to its anchor on the curve.
  - A new `Polyline.datatip_display_mode` property now allows to display each datatip of the curve only on `mouseover` its anchor or only on `mouseclick`.
* Interactively setting a common zoom box on multiple neighboring or overlaying axes, and with bounds selected out of the axes areas is now restored, after the Scilab 5.4 regression.
* Scroll to zoom:
  - Scrolling over overlaying axes now zooms all of them together.
  - Pressing CTRL while scrolling now zooms all axes in the current figure.
* When `Axes.view=="2d"`, the rotation is now impossible.
* On a figure, the contextual menu now proposes an entry `Label -> Title` to interactively set the title of any axes.
* `plot(x, list(fun, params))`is now supported.
* `plot(.., 'color', colors)` now accepts `colors` as a vector, when plotting multiple curves.
* `plot()`, `surf()` and `mesh()`: It is now possible to specify the `foreground`, `facecolor`, `markforeground` and `markbackground` global properties with any color name of the full predefined colors list. Up to now, only the 9 main colors names were usable. Moreover, colors can now be specified with their "#RRGGBB" hexa code or their indices in the color map.
* `surf(x,y,fun)` and `surf(x,y,list(fun,params))` syntaxes are now supported, with a function identifier `fun`.
* `bode()` rendering of plots and datatips are improved.
* `polarplot()` grid and datatips are improved.
* `sgrid()` is improved.
* Warnings from `fplot3d1` called without an option were removed.
* Zoom with mouse scroll wheel is now centered at cursor location. In 3D the zoom box is centered
at the closest projection of cursor on 3D objects of the Axes.
* Figure editor's terms have been clarified to ease usage.

Other:
* `type(:)` now returns `129` instead of `1`.
* `typeof(:)` and `typeof(n:$)` now return `"implicitlist"` instead of respectively `"constant"` and `"size implicit"`.
* `typeof(.., "overload")` allows now to get the overloading type-code of an object
* `size` is now overloadable for `tlist` lists, as it already could for `mlist` lists.
* For arrays of cells or structures, `length` now returns their number of elements.
* `linspace(a, b, n<=0)` now returns `[]` instead of b.
* `resize_matrix`: Its conversion option is extended to the new `int64` and `uint64` integer types.
* `cat` has been rewritten. It is now fast and can process heavy arrays at high dimensions.
* `ndgrid(x)` now accepts only one input `x`, with by default `y=x`.
* `ifftshift()` has now a `'r'|'c'|dim` option.
* `ndgrid` can now work with any types of homogeneous data
* `permute` now supports arrays of rationals.
* `sign` can now handle a sparse matrix.
* `real`, `imag`, `conj` and `isreal` now accept rational fractions.
* A call stack limit has been introduced. Default maximum depth is set up to `1000`
and can be changed by `recursionlimit` or through the Preferences interface.
* The floating point exception mode `ieee` is now set to `2` by default: floating point exceptions now produce `Inf` or `Nan`, and do not cause any error. The previous behavior can be recalled by simply calling: `ieee(0)`.
* `householder` can now return the reflection matrix and has a demo.
* `amell` now:
  - checks if its parameters are real numbers,
  - throws an error if the second parameter is not a scalar.
* `impl`: Recall `impl` with the same parameters as in its previous stop is now available.
* `ode`: `y0` is restricted to a column vector.
* `pppdiv`: Returns a scalar of type 'constant' when the rank is 0.
* `pdiv`: Returns a matrix of type 'constant' when all the rank is 0.
* `strange([])` now returns `%nan` instead of `[]`, as all other functions for statistical dispersion.
* `stdev(x, dir>ndims(x))` now yields an error instead of returning `zeros(x)`.
* `mean` and `stdev` can now be overloaded.
* `bitor`, `bitxor` and `bitand` are upgraded:
   - positive signed encoded integers are now accepted.
   - inputs with new `int64` or `uint64` encodings are now accepted.
   - operands with mixed types or/and integer types are now accepted.
   - distributive input scalars as in `bit###(scalar, array)` or `bit###(array, scalar)` are now accepted.
   - results with decimal-encoded integers > 2^32 are now correct.
   - decimal-encoded integers > 2^52 are now supported up to the biggest 1.80D+308.
   - `bitxor` is now vectorized and fast.
* The `grand` non-free `fsultra` generator is no longer available.
* The original `rpoly` algorithm is removed in favor of a C++11 implementation
* The zero-pole-gain (zpk) representation is now available for linear dynamical systems.
* `lqe` has been upgraded: The process noise variance and the measurement noise variance and covariance can now be specified to the linear quadratic estimator.
* `lqg` has been upgraded: The linear quadradic compensator can now be applied to a nominal plant, with a weighting matrix and a covariance matrix. Then, a matrix weigthing integral terms, and the degrees of freedom of the controler can be specified as options.
* `lqi` function added to compute "linear quadratic integral compensator".
* `gcd` and `lcm`
   - now accept `int64` and `uint64` integers.
   - For input integers, the result is now always positive.
   - The input can now be an array instead of a row vector.
* `lcm` of some integers now returns decimal integers instead of int32.
* `gamma()` can now process an hypermatrix.
* `unique()` can now unduplicate a set of complex numbers.
* `setdiff()` can now work row-wise or column-wise.
* `min()` and `max()` are now fully enabled to work on sparse-encoded matrices.
* `cond([])` now yields 0 as `cond([],2)`, instead of 1.
* `rcond([])` now yields `Inf` instead of `[]`, consistently with `cond([])==0`.
* A new console `File => Go to Favorite directory` menu allows to go to a favorite directory selected in a dynamical list set from Scinotes favorite and most recent directories.
* The console `File => Open a file` menu allows now to open xcos, zcos, scg or lib files with the proper Scilab component, and other files with the proper OS application. All files were opened in Scinotes and could freeze it.
* Now displayed decimals after `format("e",25)` are the same on all platforms.
* The demo GUI can now be set as not-dockable through the Preferences => General => Demos.
* `atomsSetConfig` does not update cache.
* `MPI_Create_comm` creates a new communicator from MPI_COMM_WORLD using MPI world ranks.
* The use of I/O console is now allowed with the following functions: `mget`, `mgetl`, `mgetstr`, `mput`, `mputl` and `mputstr`.
* `sleep(..,'s')` allows now to specify the duration in seconds.
* `exec` of macro executes the body in the current scope, but the prototype must have zero input and output arguments.
* `xmlGetValues` can now read a tag having multiple occurrences, and accepts the path to a preference file instead of its XML handle.
* `%io` is now protected (read-only).
* `fileinfo` can now take a row vector as input.
* `deletefile` can delete multiple files at once.
* `mgetl` speed has been improved for files with a lot of lines.
* `write`: Writing string or string matrix in a file does not add blank space before each value.
* `save()` can now process lists with undefined components.
* `savematfile` has been upgraded:
   - `savematfile(File)` now saves only user variables instead of all current ones.
   - In formats 6, 7 and 7.3, `savematfile()` no longer breaks the saving when an unsupported type is encountered. A warning is now softly yielded.
   - Input variables can no longer be corrupted by internal values before being saved.
   - When an error occurs, the output file is now closed and unlocked.
   - Encoded integers can now be saved in `-v4` format.
* Compatibility functions `mtlb_int8` `mtlb_int16` `mtlb_int32` `mtlb_uint8` `mtlb_uint16` `mtlb_uint32` have been rewritten to actually comply with Matlab versions. `mtlb_int64` and `mtlb_uint64` have been added.
* The function `stripblanks` now supports an option to remove trailing or leading spaces or both.
* `msprintf` does not return an error message anymore when there are too many input arguments (more values that format needs).
* `sci2exp` now uses `%s` or `%z` in literal expressions of polynomials in `s` or `z`.
* `banner()` updated with ESI Group a 2017-2018 copyright.
* `whereis` has been upgraded:
  - It can now be used for built-in functions.
  - When the same function name is registered in several libraries, `whereis` now returns all of them instead of only the last loaded one.
  - For unregistered user-defined macros, `"script"` is now returned instead of `[]`.
* `mclearerr` now returns a flag indicating the file identifier validity.
* `error`: absolute error numbers are no longer supported. Providing an explicit error message is now mandatory.
* `bench_run` can now return its results and/or record them in a file
* `assert_checkalmostequal` can now work with polynomials.
* `test_run` can now take `[]` as argument to be used on console; for instance: `test_run string [] no_check_ref`.
* Valgrind error detection added to `test_run` (on Linux only).


Help pages:
-----------

* Pages fixed / improved:
  - Graphics: `bar`, `barhomogenize`, `colordef`, `evans`, `histplot`, `plot`, `plot2d`, `sdf`, `surf`, `xlabel`, `xget`, `xset`, `xstring`.
  - Properties: `axes_properties`, `console_properties`, `datatip_properties`, `Globalproperty`, `graphics_entities`, `Matplot_properties`, `polyline_properties`, `root_properties`, `surface_properties`, `text_properties`.
  - Files: `file`, `fileinfo`, `listvarinfile`, `matfile_listvar`, `matfile_varreadnext`, `pathconvert`.
  - Strings: `csvTextscan`, `evstr`, `part`, `sprintf`.
  - Data handling: `cat`, `empty`, `flipdim`, `fftshift`, `ifftshift`, `iconvert`, `ismatrix`, `linspace`, `makecell`, `matrix`, `meshgrid`, `permute`, `rand`, `sparse`, `speye`, `varn`.
  - Computation: `and_op`, `or_op`, `casc`, `coffg`, `cond`, `comparison`, `diag`, `histc`, `ieee`, `intdec`, `lqe`, `lqg`, `lqg2stan`, `lqi`, `lqr`,  `members`, `modulo`, `norm`, `ode`, `odedc`, `ode_optional_output`, `ode_root`, `pdiv`, `roots`, `slash`, `stdev`, `sysconv`, `unique`.
  - Other: `bloc2ss`, `colon`, `for`, `end`, `minus`, `plus`, `printf`, `recursionlimit`, `scilab`, `tbx_make`, `tbx_generate_pofile`, `tree2code`, `%helps`, `lines`, `uigetdir`, `uigetfile`, `uiputfile`, `uigetfont`, `unix_g`, `unix_s`, `unix_w`, `unix_x`, `dos`, `where`.
  - Xcos palettes pages: layout of all ~230 pages improved: cleaned Table of contents. Useless `Module` and `Palettes` sections removed. `Dialog box` section renamed `Parameters`. Useless LaTeX entries converted into Docbook ones to become zoomable. Cross-referencing improved. Some wrong formulae fixed.

* Pages created or rewritten:
  - Graphics: `anti_aliasing`, `clf`, `isoview`, `locate`, `param3d`, `param3d1`, `pixel_drawing_mode`, `sgrid`
  - GUI: `%onprompt`, `console`, `consolebox`, `mode`, `realtime`, `sleep`, `setlanguage`.
  - Data handling:  `brackets`, `clean`, `double`, `format`, `isglobal`, `isinf`, `null`, `poly`, `sci2exp`, `toeplitz`, `type`, `typeof`.
  - Computation: `and`, `&,&&`, `bitor`, `bitxor`, `bitand`, `geomean`, `gcd`, `lcm`, `householder`, `min`, `max`, `or`, `|,||`, `intersect`, `setdiff`.
  - Converter: `eigs`, `fgets`, `mtlb_int8`, `mtlb_int16`, `mtlb_int32`, `mtlb_int64`, `mtlb_uint8`, `mtlb_uint16`, `mtlb_uint32`, `mtlb_uint64`.
  - Other: `argn`, `error_table`, `macr2tree`, `whereis`, `gettext`, `host`, `powershell`, `lib`, `load`, `save`, `savematfile`, `xmlGetValues`.

* Pages reorganized:
  - `else`, `elseif`, `end`, `try`, `sciargs`, `global`, `halt`, `empty`, `power`, `numderivative`
  - `pixel_drawing_mode`, `show_window`, `twinkle`, `uigetcolor`, `winsid`, `xdel`, `xgrid`, `xname`, `xnumb`
  - `repmat`, `sign`, `nthroot`, `lstsize`, `cell2mat`, `cellstr`, `ind2sub`, `sub2ind`, `and`, `or`, `unwrap`, `members`
  - `mode` => console, `matfile2sci` => matio
  - CACSD and Signal Processing help pages have been sorted out.
  - Signal processing: new `Convolution - correlation` subsection. `wfir_gui`, `filt_sinc`, `hilb`, `fft2`, `fftshift`,
  `ifftshift`, `hilbert`, `cepstrum`, `conv`, `conv2`, `convol2d`, `xcor`, `corr`, `hank`, `mrfit`, `frfir` sorted out in existing subsections.
  `bilt`, `convol`, `intdec`, and `sincd` moved.
  - Cells subsection created: `cell`, `cell2mat`, `cellstr`, `iscell`, `iscellstr`, `makecell`, `num2cell` gathered.
  - Colormaps and GUI/Menus subsections created
  - `grand` moved to the `rand` section. Removal of its own main chapter.
  - `bool2s` moved toward the `boolean` page. `Boolean` main chapter removed.
  - `Data structures/types` subsection created: `boolean`, `bool2s`, `hypermat`, `hypermatrices`, `matrices`, `type`, `typename`, `typeof`
  - m2sci converter: `Internals` subsection created: `Contents`, `Cste`, `Equal`, `Funcall`, `Infer`, `Operation`, `Type`, `Variable`, `get_contents_infer`, `sci_files`
  - matrix-wise trigonometric pages gathered in a new subsection: `acoshm`, `acosm`, `asinhm`, `asinl`, `atanhm`, `atanm`, `coshm`, `cosm`, `cothm`, `sinhm`, `sinm`, `tanhm`, `tanm`.
  - modules manager: subsection created for partial builders: `tbx_build_blocks`, `tbx_build_cleaner`, `tbx_build_loader`, `tbx_build_localization`, `tbx_build_src`, `tbx_builder_gateway`, `tbx_builder_help`, `tbx_builder_macros`, `tbx_builder_src`.

* Translations added:
  - (fr): `format`, `typeof`, `isoview`, `ndgrid`, `bench_run`, `consolebox`, `harmean`, `sleep`, `strtod`, `permute`, `geomean`
  - (ru): homepage, `strtod`

* Pages generation:
  - `<imagedata>` : Support to `id`, `width`, `height`, `align` and `style` attributes added.
  - `<tr>` : Support to `id` and `valign` attributes added.
  - `<latex>` : Support to the `alt` attribute added, to provide console-compliant equivalent expressions.
  - Generating help pages of external modules for Right-to-Left languages is now supported.
  - Encoding : all Scilab native pages are now UTF-8 encoded.


Data Structures
---------------

* cells and structs are now native types, hence improving performances.
* cells:
  - insertion and extraction must be done via `()` or `{}.`
  - `.dims` and `.entries` fields have been removed, please use `size` and `()` instead.
  - `length()` of a cells array is now the number of its primary components (without recursive
     counting). It is equivalent to `size( ,"*")`. It was formerly always equal to 3. It is now consistent with the definition for all types of Scilab arrays.
* struct
  - the `.dims` field has been removed, please use `size()` instead.
  - `length()` of a structures array is now the number of its primary components (without recursive counting). It is equivalent to `size( ,"*")`. It was formerly equal to its number of fields + 2. It is now consistent with the definition for all types of Scilab arrays.
* hypermatrix:
  - hypermatrices are natively managed (without `mlist` overloading).
  - `typeof` now returns the actual data type like `constant`, `string`, ... instead of `hypermat`
  - `type` now returns the actual data type like `1, 10, ...` instead of `17` (`mlist`).
  - `.dims` and `.entries` fields have been removed. Please use `size` and `()` instead.


Xcos
----

* Major rewrite of the data structures, huge models should load and save faster.
The memory usage on diagram edition is also slightly reduced.
* ZCOS and XCOS file formats have evolved to reduce the duplicated information.
Scilab 5.5.2 is able to open the newly saved files, but the ports have to be repositioned manually.
* Implicit fixed-size step ODE solver added: Crank-Nicolson 2(3).
Added to the CVode package, it also benefits from the CVode root finding feature.
* Added a new link style (`Optimal`) for automatically finding the optimal route.
* Automatically reposition split blocks for better-looking layout.
* `EXPBLK_m (a^u)` and `POWBLK_f (u^a)` icons now show the value of the `a` parameter. The `AUTOMAT` icon now shows `Nmodes` and `Nstates` values. The `SineVoltage` icon now shows the frequency value. In the Electrical palette, the Resistor, Inductor, Capacitor, and ConstantVoltage icons now show the actual value of the parameter.
* Block modifications:
  - `INVBLK`: add a divide by zero parameter to ignore the error
  - `PRODUCT`: add a divide by zero parameter to ignore the error
* The palette browser has been improved. The following features were included:
  - search engine
  - history (go forward or backward)
  - drag and drop multiple blocks at once
  - navigate using the keyboard arrows
  - add blocks to the most recent diagram by using the ENTER key
  - dynamic palette with the last used blocks
  - zoom using CTRL(+), CTRL(-) and CTRL(mouse wheel)
  - load SVG icons
* For many blocks, the value of the main parameter is now displayed on the block's icon: Resistor, Inductor, Capacitor,  ConstanteVoltage, CONVERT, Automaton, EXPBLK_m, POWBLK_f, MAXMIN, TrigFun. Improved display of the EXPRESSION.
* When rotating blocks, ports are now rotated as well. This was an issue for non-square icons.
* Deleted obsolete `WFILE_f` block, please use `WRITEC_f` instead.


API modification
----------------

A new set of C APIs to write C or C++ extensions (toolboxes) to Scilab.
It allows defining native functions (commonly called "gateways"), getting input parameters for such functions, setting return parameters, accessing local variables, using common helper functions for accessing environment information (such as warning level), generate errors...
It also includes ways to overload existing Scilab functions to support additional parameter types (see `help scilab_overload`). Finally, you can call back Scilab functions (macros and built-in functions) from your gateway (see `help scilab_call`).
User-defined functions written in C or C++ (gateways) must now use a `void* pvApiCtx` name as a second parameter instead of any `unsigned long l`. This is now required for some macros, such as `Rhs`, to work.
For example: use `int sci_Levkov(char *fname, void* pvApiCtx)` instead of `int sci_Levkov(char *fname)` or `int sci_Levkov(char *fname, unsigned long l)`.


Obsolete functions or features
------------------------------

* `datatipToggle` is obsolete and will be removed in Scilab 6.1. Please use the `datatipManagerMode` instead.
* `denom` is obsolete and will be removed in Scilab 6.1. Please use the `.den` rational attribute instead.
* `dgettext` is obsolete and will be removed in Scilab 6.1. Please use `gettext(domain, text)` instead.
* `dirname` is obsolete and will be removed in Scilab 6.1. Please use `fileparts` instead.
* `eval` is obsolete and will be removed in Scilab 6.1. Please use `evstr` instead.
* `eval3d` will be removed from Scilab 6.1. Please use `ndgrid` instead.
* `getPreferencesValue` is obsolete and will be removed in Scilab 6.1. Please use `xmlGetValues` instead.
* `isoview(xmin,xmax,ymin,ymax)` is deprecated. Please use `isoview("on"), replot(..)` instead.
* `lstsize` is obsolete and will be removed in Scilab 6.1. Please use `size` instead.
* `maxfiles` is now obsolete.
* `nanmin` is obsolete and will be removed in Scilab 6.1. Please use `min` instead.
* `nanmax` is obsolete and will be removed in Scilab 6.1. Please use `max` instead.
* `numer` is obsolete and will be removed in Scilab 6.1. Please use the `.num` rational attribute instead.
* `square` will be removed from Scilab 6.1. Please use `gcf().axes_size` and `replot` instead.
* `strcmpi` is deprecated. Please use `strcmp(..,"i")` instead.
* `with_tk` is obsolete and will be removed in Scilab 6.1. Please use the `with_module` instead.
* `xgetech` is obsolete and will be removed in Scilab 6.1. Please use `gca` instead.
* `xinfo` is obsolete and will be removed in Scilab 6.1. Please use `gcf().info_message` instead.


Removed Functions
-----------------

* `comp` and its associated type `11` have been removed. All functions will have type `13`.
* `curblockc` has been removed. Please use `curblock` instead.
* `derivative` has been removed. Please use `numderivative` instead.
* `fcontour` was obsolete since Scilab 4 and has been removed. Please use `contour` instead.
* `fcontour2d` has been removed. Please use `contour2d` instead.
* `fort` has been removed. Please use `call` instead.
* `gschur` was obsolete already in Scilab 4 and is now removed. Please use `schur` instead.
* `gspec` was obsolete already in Scilab 4 and is now removed. Please use `spec` instead.
* `havewindow` has been removed. Please use `getscilabmode()=="STD"` instead
* `intersci` has been removed. Please use [swig](http://swig.org/) instead.
* `jconvMatrixMethod` was obsolete and is now removed. Please use `jautoTranspose` instead.
* `lex_sort` has been removed. Please use `gsort(..,"lr")` instead.
* `m_circle` was obsolete since Scilab 5.2.0. It is removed. Please use `hallchart` instead.
* `mtlb_mode` has been removed. Please use `oldEmptyBehavior` instead.
* `numdiff` has been removed. Please use `numderivative` instead.
* `perl` was obsolete since Scilab 5.5.0. It is removed.
* `plot2d1` has been removed. Please use `plot2d` instead.
* `rafiter` was obsolete since Scilab 5.1 and is now removed.
* `readgateway` has been removed.
* `xclear` has been removed. Please use `clf` instead.
* `xpause` has been removed. Please use `sleep` instead.
* Symbolic module functions have been removed: `addf`, `cmb_lin`, `ldivf`, `mulf`, `rdivf`, `solve`, `subf`, `trianfml`, `trisolve` and `bloc2exp`.
* Functionnalities based on former Scilab stack have been removed:
  - `comp`, `errcatch`, `iserror`, `fun2string`, `getvariablesonstack`, `gstacksize`, `macr2lst`, `stacksize`, `code2str` and `str2code`.
  - `-mem` launching option (used to set `stacksize` at startup).
* Former debugging functions have been removed: `setbpt`, `delbpt`, `dispbpt`. Please use `debug` instead.
* Former profiling functions have been removed: `add_profiling`, `reset_profiling`, `remove_profiling`, `profile`, `showprofile`, and `plotprofile`.


Known issues
------------

* This is one of the first releases of a completely rewritten interpreter engine. If you discover strange behaviors or unexpected results do not hesitate to [report](https://bugzilla.scilab.org) them.
* Toolboxes rebuild is in progress. Do not hesitate to submit patch or feature upgrade to the [development mailing list](dev@lists.scilab.org) for a particular toolbox.


### Bugs fixed in 6.0.2:
* [#2247](http://bugzilla.scilab.org/show_bug.cgi?id=2247): The axes_properties page did not show how to yield vertical x-tick labels. `.grid_style`, `.grid_thickness`, and `.axes_bounds` inside a uicontrol frame were not documented.
* [#2464](http://bugzilla.scilab.org/show_bug.cgi?id=2464): `null()` was poorly documented.
* [#4050](http://bugzilla.scilab.org/show_bug.cgi?id=4050): `ged()` did not support graphical texts with a font size > 5.
* [#4808](http://bugzilla.scilab.org/show_bug.cgi?id=4808):  The `colorbar` page was misleading about when the color bar must be created wrt the related axes.
* [#4953](http://bugzilla.scilab.org/show_bug.cgi?id=4953): The `clean` function did not work when the matrix contains %inf.
* [#5430](http://bugzilla.scilab.org/show_bug.cgi?id=5430): `clf` could erase the ATOMS GUI or the m2sci one.
* [#5521](http://bugzilla.scilab.org/show_bug.cgi?id=5521): `surf(X,Y,fun)` and `surf(X,Y,list(fun,params))` were not supported.
* [#5557](http://bugzilla.scilab.org/show_bug.cgi?id=5557): `locate()` allowed to click out of the current axes and then returned meaningless coordinates.
* [#5558](http://bugzilla.scilab.org/show_bug.cgi?id=5558): `locate()` drawn crosses at unexpected positions in the current axes and may dramatically resize it when clicking in an axes or figure not being current ones.
* [#5559](http://bugzilla.scilab.org/show_bug.cgi?id=5559): `locate()` inconsistently asked for left or right mouse clicks to select points, according to the given or unknown expected number of points.
* [#6434](http://bugzilla.scilab.org/show_bug.cgi?id=6434): Calls like libname.Macro(..) worked only with one input argument.
* [#6548](http://bugzilla.scilab.org/show_bug.cgi?id=6548): `gamma` did not accept an hypermatrix and could not be overloaded for complex numbers.
* [#6729](http://bugzilla.scilab.org/show_bug.cgi?id=6729): The compatibility functions `mtlb_int8`, `mtlb_int16`, `mtlb_int32`, `mtlb_uint8`, `mtlb_uint16` and `mtlb_uint32` were not actually Matlab-like. Their documentation was neither correct nor up-to-date. `mtlb_int64` and `mtlb_int64` were missing.
* [#6737](http://bugzilla.scilab.org/show_bug.cgi?id=6737): mouse events were not correctly reported to an event handler function.
* [#6777](http://bugzilla.scilab.org/show_bug.cgi?id=6777): `gcf().anti_aliasing` was poorly documented and was not illustrated.
* [#6939](http://bugzilla.scilab.org/show_bug.cgi?id=6939): `nearfloat("succ",rand(2,2,2))` returned a (2,2) matrix with no error.
* [#6990](http://bugzilla.scilab.org/show_bug.cgi?id=6990): `or(hm,"r")`, `or(hm,"c")`, `and(hm,"r")` and `and(hm,"c")` generated an error for any hypermatrix hm of booleans, encoded integers, or doubles.
* [#7107](http://bugzilla.scilab.org/show_bug.cgi?id=7107): The `graphics_entities` page missed listing the console_properties, root_properties, uicontrol_properties, and uimenu_properties. References to `getsystemmetrics` and to `setlookandfeel` were missing in the `See also` section.
* [#7151](http://bugzilla.scilab.org/show_bug.cgi?id=7151): `plot(x, fun)` could not be used to plot a function with input parameters.
* [#7215](http://bugzilla.scilab.org/show_bug.cgi?id=7215): Some Matlab-Scilab equivalences were missing/outdated (A,B,C chapters).
* [#7221](http://bugzilla.scilab.org/show_bug.cgi?id=7221): `modulo` was not indicated as the equivalent of Matlab's rem().
* [#7277](http://bugzilla.scilab.org/show_bug.cgi?id=7277): SciNotes `File>Open recent` menu was not updated after 5 items were added.
* [#7305](http://bugzilla.scilab.org/show_bug.cgi?id=7305): `issparse` returned 0 or 1 instead of %F or %T.
* [#7362](http://bugzilla.scilab.org/show_bug.cgi?id=7362): There were 10 unused macros in the scicos_blocks hydraulic directory.
* [#7641](http://bugzilla.scilab.org/show_bug.cgi?id=7641): `uint8(1):uint16(4)` produced an error instead of being parsed as `uint8(1):1:uint16(4)`, that passes.
* [#7659](http://bugzilla.scilab.org/show_bug.cgi?id=7659): The `error` page was not up-to-date for Scilab 6.
* [#7675](http://bugzilla.scilab.org/show_bug.cgi?id=7675): sparse() got a wrong result, when creation with many values in same position.
* [#8000](http://bugzilla.scilab.org/show_bug.cgi?id=8000): `get(gcf(), "%s %s %s..")` crashed Scilab.
* [#8017](http://bugzilla.scilab.org/show_bug.cgi?id=8017): `atomsInstall` yielded an error when a version without packaging subversion is specified.
* [#8127](http://bugzilla.scilab.org/show_bug.cgi?id=8127): When using `ode()`, some internal errors messages were in french.
* [#8143](http://bugzilla.scilab.org/show_bug.cgi?id=8143): Checking of `mtlb_qr` input and output arguments was incorrect.
* [#8767](http://bugzilla.scilab.org/show_bug.cgi?id=8767): `%F & ~%F | %T & ~%F` was `%F`.
* [#8779](http://bugzilla.scilab.org/show_bug.cgi?id=8779): gsort(x,'lr') did not preserve order of equal items in index output.
* [#8810](http://bugzilla.scilab.org/show_bug.cgi?id=8810): Insertions with an hypermatrix of indices failed.
* [#8995](http://bugzilla.scilab.org/show_bug.cgi?id=8995): Problem with row insertion in empty matrix.
* [#9063](http://bugzilla.scilab.org/show_bug.cgi?id=9063): `pt` was the alias of `pt_PT` instead of `pt_BR`.
* [#9231](http://bugzilla.scilab.org/show_bug.cgi?id=9231): `fsolve()` produced wrong info for certain initial values
* [#9539](http://bugzilla.scilab.org/show_bug.cgi?id=9539): For a big 1600x1600 input map, `contour2di` crashed Scilab.
* [#9958](http://bugzilla.scilab.org/show_bug.cgi?id=9958): `mprintf("%d\r", 12345)` printed `" 345"`
* [#10171](http://bugzilla.scilab.org/show_bug.cgi?id=10171): `loadmatfile()` did not load files containing a matlab cell structure.
* [#10190](http://bugzilla.scilab.org/show_bug.cgi?id=10190): Xcos blocks and labels positioning were not aligned to the grid on creation.
* [#10197](http://bugzilla.scilab.org/show_bug.cgi?id=10197): creating implicit vector of integers could fail.
* [#10411](http://bugzilla.scilab.org/show_bug.cgi?id=10411): Pivoting blocks drew out of bounds icons.
* [#10639](http://bugzilla.scilab.org/show_bug.cgi?id=10639): The page of `intersect` poorly described the arguments. Examples were not illustrated.
* [#10668](http://bugzilla.scilab.org/show_bug.cgi?id=10668): Some words in the `bloc2ss` page were inaccurate or misleading.
* [#10918](http://bugzilla.scilab.org/show_bug.cgi?id=10918): After `a=5`, `a([%t ; %f])` returned `[5 ; 2.1e-314]` instead of `5`.
* [#11240](http://bugzilla.scilab.org/show_bug.cgi?id=11240): `A=[]; clear B; [A($+1),B]=1;` crashed Scilab.
* [#11435](http://bugzilla.scilab.org/show_bug.cgi?id=11435): The demo `Simulation => Flow => Blackhole` reliability had to be checked. Its displayed speed value was truncated.
* [#11583](http://bugzilla.scilab.org/show_bug.cgi?id=11583): When in Scinotes preferences the default header is erased, the available variables were no longer documented.
* [#11605](http://bugzilla.scilab.org/show_bug.cgi?id=11605): `save(filename, ["a","b","c"..]` was not allowed.
* [#11765](http://bugzilla.scilab.org/show_bug.cgi?id=11765): `eigs` was not documented among Matlab-Scilab equivalences.
* [#11916](http://bugzilla.scilab.org/show_bug.cgi?id=11916): Loading a graphic file containing several figures restored them by superimposing them in the same window.
* [#11933](http://bugzilla.scilab.org/show_bug.cgi?id=11933): For polynomials in `s` or `z`, `sci2exp` did not use `%s` and `%z` to generate a valid expression. For other polynomials, no how-to was documented.
* [#12109](http://bugzilla.scilab.org/show_bug.cgi?id=12109): `execstr(.,"errcatch")` calling a macro returning before setting the argout crashed Scilab.
* [#12147](http://bugzilla.scilab.org/show_bug.cgi?id=12147): `mfile2sci()` produced an error when 1) the {function} block is closed with {end}; 2) the macro's name matches a variable existing in the environment.
* [#12198](http://bugzilla.scilab.org/show_bug.cgi?id=12198): Scilab control and block keywords `break case ... try while` and `clear()` could be overloaded as regular assignable variables.
* [#12275](http://bugzilla.scilab.org/show_bug.cgi?id=12275): `msprintf("%s",ascii(97*ones(1,4097)))` produced "An error occurred: Buffer too small."
* [#12345](http://bugzilla.scilab.org/show_bug.cgi?id=12345): `tree_show` is a list-specific function. Its page was expected in the data_structures chapter rather than in the general GUI chapter. It is moved.
* [#12402](http://bugzilla.scilab.org/show_bug.cgi?id=12402): The menu `Applications => Scinotes` could launch the external editor instead of Scinotes.
* [#12477](http://bugzilla.scilab.org/show_bug.cgi?id=12477): Any `atomsRemove` error in `scilab.start` made any forthcoming session unusable.
* [#12520](http://bugzilla.scilab.org/show_bug.cgi?id=12520): Variable browser did not display the size of the variables.
* [#12529](http://bugzilla.scilab.org/show_bug.cgi?id=12529): The `listvarinfile` help page needed to be updated.
* [#12534](http://bugzilla.scilab.org/show_bug.cgi?id=12534): Variable browser did not display the size of the variables.
* [#12566](http://bugzilla.scilab.org/show_bug.cgi?id=12566): `disp()` of a T-list with a single field set to an encoded integer value generated an error.
* [#12618](http://bugzilla.scilab.org/show_bug.cgi?id=12618): `mfile2sci()` failed converting `a=1i // ab` into `a=1*%i // ab`.
* [#12659](http://bugzilla.scilab.org/show_bug.cgi?id=12659): `modulo()` used for big inputs crashed Scilab.
* [#12711](http://bugzilla.scilab.org/show_bug.cgi?id=12711): Zoom with mouseWheel was always centered at the center of viewing box and was not using cursor position.
* [#12810](http://bugzilla.scilab.org/show_bug.cgi?id=12810): support added to build RTL help pages.
* [#12835](http://bugzilla.scilab.org/show_bug.cgi?id=12835): Calling without LHS a function returning `varargout` set to list() yielded an error.
* [#12926](http://bugzilla.scilab.org/show_bug.cgi?id=12926): `~%f==1` was parsed as `~(%f==1)` instead of `(~%f)==1`.
* [#13306](http://bugzilla.scilab.org/show_bug.cgi?id=13306): Evaluating a function set as a structure field could fail.
* [#13197](http://bugzilla.scilab.org/show_bug.cgi?id=13197): The documentation of `lib()` needed to be updated for Scilab 6.
* [#13231](http://bugzilla.scilab.org/show_bug.cgi?id=13231): `[X,D]=spec(rand(2000,2000))` yielded a segmentation fault.
* [#13374](http://bugzilla.scilab.org/show_bug.cgi?id=13374): When setting an erroneous Xcos context, the error message printed in console could easily be missed.
* [#13039](http://bugzilla.scilab.org/show_bug.cgi?id=13039): In Scinotes, the "File => Save file in" action opened the parent of the targeted directory.
* [#13466](http://bugzilla.scilab.org/show_bug.cgi?id=13466): Since Scilab 5.5.0, it was no longer possible to dock the demo GUI.
* [#13480](http://bugzilla.scilab.org/show_bug.cgi?id=13480): The default k value of `eigs` was wrong and made `eigs` failing forsmall full matrices.
* [#13490](http://bugzilla.scilab.org/show_bug.cgi?id=13490): `histc` normalization was wrong. In the help, its description was rong and inconsistent.
* [#13522](http://bugzilla.scilab.org/show_bug.cgi?id=13522): `savematfile` failed saving numerical hypermatrices.
* [#13612](http://bugzilla.scilab.org/show_bug.cgi?id=13612): Xcos "Selection to superblock" did not assing I/O blocks correctly.
* [#13632](http://bugzilla.scilab.org/show_bug.cgi?id=13632): The syntax `savematfile(filename, varnames)` was not documented. Other information was missing in the page.
* [#13697](http://bugzilla.scilab.org/show_bug.cgi?id=13697): Calling `chfact` caused Scilab crash under Mac OS X
* [#13778](http://bugzilla.scilab.org/show_bug.cgi?id=13778): The size of `repmat([], [1 1 3])` was [0 0 3] instead of [0 0].
* [#13785](http://bugzilla.scilab.org/show_bug.cgi?id=13785): `gettext` was poorly documented.
* [#13798](http://bugzilla.scilab.org/show_bug.cgi?id=13798): Datatips did not update when moving curve or changing data
* [#13852](http://bugzilla.scilab.org/show_bug.cgi?id=13852): Calling `bvode` caused Scilab crash under Mac OS X
* [#13857](http://bugzilla.scilab.org/show_bug.cgi?id=13857): `splin3d` crashed Scilab under Mac OS X
* [#13915](http://bugzilla.scilab.org/show_bug.cgi?id=13915): On Windows, reinstalling an ATOMS toolbox with an already present archive failed.
* [#13936](http://bugzilla.scilab.org/show_bug.cgi?id=13936): The exponentiation of a matrix was much slower than its multiple products.
* [#14010](http://bugzilla.scilab.org/show_bug.cgi?id=14010): Browsevar was not displaying dimensions > 2 of hypermatrix
* [#14122](http://bugzilla.scilab.org/show_bug.cgi?id=14122): Some SCI/.atoms/tmp_<unix_timestamp> could be created instead of in TMPDIR, and remained in case of installation error.
* [#14218](http://bugzilla.scilab.org/show_bug.cgi?id=14218): Karmarkar post-process fails on some problems.
* [#14282](http://bugzilla.scilab.org/show_bug.cgi?id=14282): `listvarinfile(fn)` and `load(fn)` yielded some memory leak.
* [#14296](http://bugzilla.scilab.org/show_bug.cgi?id=14296): Xcos labels moving after creating blocks using scripts crashed Scilab.
* [#14318](http://bugzilla.scilab.org/show_bug.cgi?id=14318): There was a memory leak when writing a HDF5 file.
* [#14355](http://bugzilla.scilab.org/show_bug.cgi?id=14355): `powershell` was poorly documented.
* [#14373](http://bugzilla.scilab.org/show_bug.cgi?id=14373): the `xcosAddToolMenu` callback did not work without selecting a block.
* [#14386](http://bugzilla.scilab.org/show_bug.cgi?id=14386): `string(macro)` commented some `end` (missing `\n`).
* [#14388](http://bugzilla.scilab.org/show_bug.cgi?id=14388): Feeding a cell component or clearing a cell array often crashed Scilab.
* [#14460](http://bugzilla.scilab.org/show_bug.cgi?id=14460): sparse boolean indices were not supported.
* [#14489](http://bugzilla.scilab.org/show_bug.cgi?id=14489): clicking the scinotes icon did not bring its window in the foreground.
* [#14506](http://bugzilla.scilab.org/show_bug.cgi?id=14506): An `atomsInstallList` error message had a wrong number of input arguments.
* [#14510](http://bugzilla.scilab.org/show_bug.cgi?id=14510): The example in the `atomsLoad` page could not run properly.
* [#14521](http://bugzilla.scilab.org/show_bug.cgi?id=14521): For the CONVERT Xcos block, types codes 2, 6, 7, and 8 were not documented.
* [#14522](http://bugzilla.scilab.org/show_bug.cgi?id=14522): `or(%nan)` was %F.
* [#14536](http://bugzilla.scilab.org/show_bug.cgi?id=14536): `xpoly` crashed Scilab when it is used in logarithmic mode for points with negative or null coordinates.
* [#14539](http://bugzilla.scilab.org/show_bug.cgi?id=14539): It was not possible to build Scilab with hdf5 1.10.0.
* [#14606](http://bugzilla.scilab.org/show_bug.cgi?id=14606): Memory used by variables returned by `[names,mem]=who()` was always zero.
* [#14670](http://bugzilla.scilab.org/show_bug.cgi?id=14670): Superblocks could be opened more than once.
* [#14701](http://bugzilla.scilab.org/show_bug.cgi?id=14701): `poly(c,v,"coeff")` did not ignore null high order coefficients.
* [#14704](http://bugzilla.scilab.org/show_bug.cgi?id=14704): In Scinotes, URL for the `https://`, `ftp://`, `ftps://`, and `file://` protocols were no longer hyperlinked.
* [#14708](http://bugzilla.scilab.org/show_bug.cgi?id=14708): polynomial operators `+` `-` `/` `./` `*` `.*` no longer simplified null high orders coefficients from result
* [#14840](http://bugzilla.scilab.org/show_bug.cgi?id=14840): `load()` is unable to load old data files saved with Scilab < 5.4.0 out of the SOD format.
* [#14854](http://bugzilla.scilab.org/show_bug.cgi?id=14854): The `error_table` page needed to be updated.
* [#14897](http://bugzilla.scilab.org/show_bug.cgi?id=14897): `xinfo` is poor and useless. It is declared obsolete.
* [#14902](http://bugzilla.scilab.org/show_bug.cgi?id=14902): `array(k) = []` dit not work for handles (Regression)
* [#14906](http://bugzilla.scilab.org/show_bug.cgi?id=14906): The `histplot` normalization was broken.
* [#14915](http://bugzilla.scilab.org/show_bug.cgi?id=14915): `ascii(10)` makes `mprintf()` stop printing (REGRESSION).
* [#14946](http://bugzilla.scilab.org/show_bug.cgi?id=14946): `slint()` expected the predefined variables `%s`, `%z` and `home` to be initialized..
* [#14988](http://bugzilla.scilab.org/show_bug.cgi?id=14988): The example of Xcos diagram on the DERIV help page was distorted.
* [#14989](http://bugzilla.scilab.org/show_bug.cgi?id=14989): The example of Xcos diagram on the INTEGRAL_m help page was distorted.
* [#14990](http://bugzilla.scilab.org/show_bug.cgi?id=14990): The example of Xcos diagram on the NEGTOPOS_f help page was distorted.
* [#14995](http://bugzilla.scilab.org/show_bug.cgi?id=14995): savematfile crashes Scilab
* [#15017](http://bugzilla.scilab.org/show_bug.cgi?id=15017): `lcm` did not support int64 and uint64 input integers. Input integers had to be in a row instead of any matrix. For input decimal integers, results were int32 encoded.
* [#15024](http://bugzilla.scilab.org/show_bug.cgi?id=15024): `A=10; A{1}=2` corrupted A and silently failed
* [#15033](http://bugzilla.scilab.org/show_bug.cgi?id=15033): `fileinfo` gave negative size if filesize bigger than 2^31
* [#15050](http://bugzilla.scilab.org/show_bug.cgi?id=15050): Xcos "Selection to superblock" did not relink the diagram after sub-system modification.
* [#15091](http://bugzilla.scilab.org/show_bug.cgi?id=15091): `translatepaths()` was KO.
* [#15092](http://bugzilla.scilab.org/show_bug.cgi?id=15092): Scalar variable was resized as a column vector instead of row.
* [#15095](http://bugzilla.scilab.org/show_bug.cgi?id=15095): The Matlab=>Scilab translator failed when working with non-ASCII paths.
* [#15115](http://bugzilla.scilab.org/show_bug.cgi?id=15115): Tabs order in SciNotes was incorrect when opening file and restoring session.
* [#15122](http://bugzilla.scilab.org/show_bug.cgi?id=15122): Xcos "Selection to superblock" modified the rendering and labeling of blocks.
* [#15157](http://bugzilla.scilab.org/show_bug.cgi?id=15157): min/max on tlist or mlist did not call the right overload.
* [#15158](http://bugzilla.scilab.org/show_bug.cgi?id=15158): `macr2tree()` used on a function including some recursive extraction statements could crash.
* [#15182](http://bugzilla.scilab.org/show_bug.cgi?id=15182): The result of `a.*.b` with mixed integer/decimal operands introduced in Scilab 6 returned some doubles instead of integers, inconsistently wrt the `*` and `.*` products.
* [#15187](http://bugzilla.scilab.org/show_bug.cgi?id=15187): Super block diagram title (on the window bar) was not set.
* [#15259](http://bugzilla.scilab.org/show_bug.cgi?id=15259): LaTeX images did not have alt='..' attribute providing the LaTeX formula as text.
* [#15263](http://bugzilla.scilab.org/show_bug.cgi?id=15263): Assignments in an array of structures often overwrote some untargeted elements.
* [#15279](http://bugzilla.scilab.org/show_bug.cgi?id=15279): `unique` could not be used on sets of complex numbers.
* [#15284](http://bugzilla.scilab.org/show_bug.cgi?id=15284): Port names are not set to the corresponding I/O block labels.
* [#15310](http://bugzilla.scilab.org/show_bug.cgi?id=15310): `isdef` considered void arguments as defined (regression)
* [#15346](http://bugzilla.scilab.org/show_bug.cgi?id=15346): In an array of rationals, there was no way to address components with their linearized indices.
* [#15403](http://bugzilla.scilab.org/show_bug.cgi?id=15403): `bar(..,"stacked")` could start from y<>0. Input arguments and possible syntaxes were poorly tested.
* [#15404](http://bugzilla.scilab.org/show_bug.cgi?id=15404): `surf()` and `mesh()` did not allow to specify `foreground`, `facecolor`, `markforeground` and `markbackground` global properties colors as a predefined named color out of a list of the 9 main color names. Colors specifications as "#RRGGBB" hexa code or Colors indices in the color map were nor allowed.
* [#15422](http://bugzilla.scilab.org/show_bug.cgi?id=15422): `strsubst("ab", "", "cd")` crashed Scilab.
* [#15423](http://bugzilla.scilab.org/show_bug.cgi?id=15423): `tbx_make(myModule,sections)` executed the existing builder (if any), instead of targeting only selected module sections. Otherwise, tbx_make(myModule, "help"|"macros") yielded an error, and tbx_make(myModule,"localization") never built it.
* [#15430](http://bugzilla.scilab.org/show_bug.cgi?id=15430): 47 `Scierror()` errors messages had mismatching input data.
* [#15433](http://bugzilla.scilab.org/show_bug.cgi?id=15433): `getPreferencesValue` is a general xml utility function and should be renamed and moved to the XML module.
* [#15434](http://bugzilla.scilab.org/show_bug.cgi?id=15434): Xcos "Selection to superblock" failed the simulation.
* [#15440](http://bugzilla.scilab.org/show_bug.cgi?id=15440): Scilab debug help page linked to Xcos debug.
* [#15441](http://bugzilla.scilab.org/show_bug.cgi?id=15441): `mprintf("%%")` crashed Scilab
* [#15442](http://bugzilla.scilab.org/show_bug.cgi?id=15442): `printf` could not handle uint64 integers greater than 2^32-1.
* [#15447](http://bugzilla.scilab.org/show_bug.cgi?id=15447): The scilab.appdata.xml file deserved some refreshment.
* [#15448](http://bugzilla.scilab.org/show_bug.cgi?id=15448): The Linux desktop files needed to be refreshed.
* [#15449](http://bugzilla.scilab.org/show_bug.cgi?id=15449): Some small changes were required to ease the compilation with gcc >= 7.
* [#15450](http://bugzilla.scilab.org/show_bug.cgi?id=15450): Scilab's compilation did not support the ppc64le architecture.
* [#15453](http://bugzilla.scilab.org/show_bug.cgi?id=15453): Scilab's compilation needed HOME to be writable.
* [#15454](http://bugzilla.scilab.org/show_bug.cgi?id=15454): Hundreds of typos fixed all over the code: occured => occured, formated => formatted, an other => another, setted => set, retrived => retrieved ..
* [#15455](http://bugzilla.scilab.org/show_bug.cgi?id=15455): OCaml build sometimes failed with Linenum being an unbound module.
* [#15457](http://bugzilla.scilab.org/show_bug.cgi?id=15457): The icon of the "Trigo functions" and "CONVERT" Xcos blocks was not updated according to actual input parameters.
* [#15458](http://bugzilla.scilab.org/show_bug.cgi?id=15458): `hist3d` rendering was wrong.
* [#15464](http://bugzilla.scilab.org/show_bug.cgi?id=15464): Xcos C-blocks could not link dlls on Windows.
* [#15468](http://bugzilla.scilab.org/show_bug.cgi?id=15468): Xcos saved file used non UTF-8 characters which lead to invalid file loading later.
* [#15472](http://bugzilla.scilab.org/show_bug.cgi?id=15472): `mdelete("test")` worked like `mdelete("test.*")` under Windows
* [#15476](http://bugzilla.scilab.org/show_bug.cgi?id=15476): In the Electrical palette, the Resistor, Inductor, Capacitor, and ConstantVoltage icons did not show the parameter's value.
* [#15484](http://bugzilla.scilab.org/show_bug.cgi?id=15484): On Windows, `isdir("w:")`, `isdir("w:/")`, and `isdir("w:\")` returned %T whenever the W: drive does not exist.
* [#15489](http://bugzilla.scilab.org/show_bug.cgi?id=15489): Help pages of `covStart`, `covStop`, `covWrite` and `covMerge` were not built for the fr, ja, pt and ru languages, and were unreachable from a session in non `en_US` language.
* [#15490](http://bugzilla.scilab.org/show_bug.cgi?id=15490): Error message of grand function was incoherent
* [#15495](http://bugzilla.scilab.org/show_bug.cgi?id=15495): SampleCLK simple demo failed to simulate.
* [#15496](http://bugzilla.scilab.org/show_bug.cgi?id=15496): Scinotes `File > Open recent` menu was not updated when it should.
* [#15497](http://bugzilla.scilab.org/show_bug.cgi?id=15497): After `s.a=list(1)`, `type(s("a"))` was `1`.
* [#15499](http://bugzilla.scilab.org/show_bug.cgi?id=15499): Scinotes menus were inactive after session restore under OSX.
* [#15501](http://bugzilla.scilab.org/show_bug.cgi?id=15501): listvarinfile() crashes Scilab when listing a cell array (regression).
* [#15502](http://bugzilla.scilab.org/show_bug.cgi?id=15502): listvarinfile() misses listing arrays of struct, and then locks the file.
* [#15508](http://bugzilla.scilab.org/show_bug.cgi?id=15508): `int8..uint64` and `iconvert` help pages had errors and were not up to date.
* [#15510](http://bugzilla.scilab.org/show_bug.cgi?id=15510): 'Initialization problem' error did not provide extra information.
* [#15511](http://bugzilla.scilab.org/show_bug.cgi?id=15511): Keyboard ctrl-left ctrl-right shortcuts could not be used for slider under OSX.
* [#15512](http://bugzilla.scilab.org/show_bug.cgi?id=15512): After aborting a running script or macro (with abort or other), console print was not working
* [#15524](http://bugzilla.scilab.org/show_bug.cgi?id=15524): Fix `grand()` symbol resolutionon Linux.
* [#15525](http://bugzilla.scilab.org/show_bug.cgi?id=15525): Memory allocation crashed with a simple Scilab script.
* [#15526](http://bugzilla.scilab.org/show_bug.cgi?id=15526): fscanfMat could fail with large text files.
* [#15527](http://bugzilla.scilab.org/show_bug.cgi?id=15527): `ode` gave incorrect results with %ODEOPTIONS(1)=2.
* [#15528](http://bugzilla.scilab.org/show_bug.cgi?id=15528): savewave wrote null signal if 24 bits resolution and more than 2 channels
* [#15529](http://bugzilla.scilab.org/show_bug.cgi?id=15529): In the `iir` page, the descriptions of z and p were inverted.
* [#15531](http://bugzilla.scilab.org/show_bug.cgi?id=15531): `[x,k]=gsort(%nan+ones(1000,1))` crashed Scilab
* [#15535](http://bugzilla.scilab.org/show_bug.cgi?id=15535): `sign(int8(0))` returned 1 instead of 0, as well with any other input encoded integer type.
* [#15539](http://bugzilla.scilab.org/show_bug.cgi?id=15539): zero step in integer implicit list crashed Scilab.
* [#15542](http://bugzilla.scilab.org/show_bug.cgi?id=15542): `whereis('my_user_defined_fun')` returned [] and expected a function handle
* [#15548](http://bugzilla.scilab.org/show_bug.cgi?id=15548): [%t %t]./[%f %f] crashed Scilab.
* [#15549](http://bugzilla.scilab.org/show_bug.cgi?id=15549): unary minus incorrectly acted as the negation operator on %t.
* [#15550](http://bugzilla.scilab.org/show_bug.cgi?id=15550): Help pages of `unix*`, `host`, `dos` and `consolebox` were poor and needed some improvements.
* [#15551](http://bugzilla.scilab.org/show_bug.cgi?id=15551): arithmetic operators gave wrong results with booleans.
* [#15553](http://bugzilla.scilab.org/show_bug.cgi?id=15553): TOWS_c block failed with 0 simulation time.
* [#15557](http://bugzilla.scilab.org/show_bug.cgi?id=15557): `mtlb_std(1:10)` yielded an error.
* [#15563](http://bugzilla.scilab.org/show_bug.cgi?id=15563): poly(A,v) was broken when A is a matrix.
* [#15566](http://bugzilla.scilab.org/show_bug.cgi?id=15566): matrix insertion checked overall size but not individual dimensions.
* [#15578](http://bugzilla.scilab.org/show_bug.cgi?id=15578): `mfile2sci` no longer processed correctly a comment appended to the function prototype, nor any m2scideclare() statement.
* [#15579](http://bugzilla.scilab.org/show_bug.cgi?id=15579): `cond([])` returned 1 instead of 0 as `cond([],2)`.
* [#15582](http://bugzilla.scilab.org/show_bug.cgi?id=15582): The default `mfile2sci`'s `only-double` option set to %T was misleading and too restrictive.
* [#15583](http://bugzilla.scilab.org/show_bug.cgi?id=15583): `color(["red" "blue"])` yielded a puzzling error message asking for String type instead of a scalar.
* [#15586](http://bugzilla.scilab.org/show_bug.cgi?id=15586): `tree2code()` added some leading `,` in the argins and argout lists.
* [#15589](http://bugzilla.scilab.org/show_bug.cgi?id=15589): `inv()` could fail on some matrices of polynomials with complex coefficients.
* [#15590](http://bugzilla.scilab.org/show_bug.cgi?id=15590): `pdiv`, `modulo` and `pmodulo` did not work with hypermatrices of polynomials. There were some mistakes in `modulo` help page. The code of `modulo()` and `pmodulo` could be simplified since hypermatrices are no longer mlist() in Scilab 6.
* [#15593](http://bugzilla.scilab.org/show_bug.cgi?id=15593): The `grand` page was alone in a main chapter instead of being in the `rand` section.
* [#15594](http://bugzilla.scilab.org/show_bug.cgi?id=15594): The `bool2s` page was alone in a main chapter instead of being with the `boolean` page.
* [#15598](http://bugzilla.scilab.org/show_bug.cgi?id=15598): `string(handle)` returned "" instead of calling `%h_string()`
* [#15599](http://bugzilla.scilab.org/show_bug.cgi?id=15599): degree of zero polynomial was 0 instead of -Inf.
* [#15600](http://bugzilla.scilab.org/show_bug.cgi?id=15600): `savematfile(File)` was fragile and had many pitfalls.
* [#15601](http://bugzilla.scilab.org/show_bug.cgi?id=15601): `tbx_generate_pofile` failed when the absolute toolbox path includes some spaces, or when the toolbox has no XML files.
* [#15605](http://bugzilla.scilab.org/show_bug.cgi?id=15605): CLR and DLR Xcos blocks did not accept expressions including "%" like "%i"
* [#15609](http://bugzilla.scilab.org/show_bug.cgi?id=15609): (1:1):2 crashed Scilab.
* [#15615](http://bugzilla.scilab.org/show_bug.cgi?id=15615): `datatipToggle()` was a duplicate of `datatipManagerMode()`.
* [#15617](http://bugzilla.scilab.org/show_bug.cgi?id=15617): `evstr(list(..))` did not check the number and types of list components. When an input component can't be evaluated, the error messages were not clear enough.
* [#15621](http://bugzilla.scilab.org/show_bug.cgi?id=15621): polynomial exponentiation silently truncated exponent (regression).
* [#15624](http://bugzilla.scilab.org/show_bug.cgi?id=15624): parser allowed `fun(,)` or `fun(x,)` for built-in functions.
* [#15625](http://bugzilla.scilab.org/show_bug.cgi?id=15625): `delip(1,1)=0` instead of `Inf`
* [#15626](http://bugzilla.scilab.org/show_bug.cgi?id=15626): clicking on its icon did not give focus to a docked Scinotes.
* [#15627](http://bugzilla.scilab.org/show_bug.cgi?id=15627): changing font in SciNotes window with ctrl-mouse wheel in splitted view messed other view.
* [#15628](http://bugzilla.scilab.org/show_bug.cgi?id=15628): `with_tk` was useless and should be tagged obsolete to be removed.
* [#15629](http://bugzilla.scilab.org/show_bug.cgi?id=15629): `int(1e20)` returned -9.223D+18  (regression)
* [#15630](http://bugzilla.scilab.org/show_bug.cgi?id=15630): `x(0:2)=1` did not raise an error and `x=1;x(0:1,1)` crashed Scilab.
* [#15632](http://bugzilla.scilab.org/show_bug.cgi?id=15632): `x=[];x()=1` crashed Scilab.
* [#15635](http://bugzilla.scilab.org/show_bug.cgi?id=15635): `dellip(1,4)` terminated with neither output nor error (regression)
* [#15636](http://bugzilla.scilab.org/show_bug.cgi?id=15636): Clicking on its icon did not always give focus to Help browser
* [#15638](http://bugzilla.scilab.org/show_bug.cgi?id=15638): `colorbar()`: Both colors on the bar tips spanned an interval twice smaller than for other colors. This made almost impossible to choose a convenient number of colors to share the whole values interval in a simple way.
* [#15642](http://bugzilla.scilab.org/show_bug.cgi?id=15642):A(:) gave incorrect display when A is sparse boolean (regression)
* [#15645](http://bugzilla.scilab.org/show_bug.cgi?id=15645): `deff('y=f(x)','z=x^2'),fsolve(1,f)` crashed scilab
* [#15647](http://bugzilla.scilab.org/show_bug.cgi?id=15647): `spzeros(-1,-1)` yielded a corrupted result
* [#15648](http://bugzilla.scilab.org/show_bug.cgi?id=15648): `sparse([1 1],1,[-1 -1])` crashed scilab
* [#15652](http://bugzilla.scilab.org/show_bug.cgi?id=15652): An appended comment // after a comma in an multiline literal array now generates an error (regression).
* [#15653](http://bugzilla.scilab.org/show_bug.cgi?id=15653): sparse - complex substraction was corrupted
* [#15655](http://bugzilla.scilab.org/show_bug.cgi?id=15655): `clear a; a(1:4,:,1) = (1:4)` raised an error  (regression).
* [#15658](http://bugzilla.scilab.org/show_bug.cgi?id=15658): `tbx_builder_src` required a C|C++|fortran compiler to compile only Java sources.
* [#15659](http://bugzilla.scilab.org/show_bug.cgi?id=15659): `plot(x,fun)` could be very slow and crash Scilab.
* [#15660](http://bugzilla.scilab.org/show_bug.cgi?id=15660): `atomsInstall` and `atomsRemove` displayed concatenated notifications instead of stacked ones.
* [#15670](http://bugzilla.scilab.org/show_bug.cgi?id=15670): `set(H, prop1,val1, prop2,val2, ..)`: Setting multiple properties for a vector of handles yielded an error.
* [#15675](http://bugzilla.scilab.org/show_bug.cgi?id=15675): `format("e")` broke dimensions and size display of `whos()`
* [#15685](http://bugzilla.scilab.org/show_bug.cgi?id=15685): The page of `file()` needed to be updated about error numbers, and improved.
* [#15692](http://bugzilla.scilab.org/show_bug.cgi?id=15692): listvarinfile() crashes when listing a boolean sparse matrix (regression)
* [#15693](http://bugzilla.scilab.org/show_bug.cgi?id=15693): listvarinfile() misses displaying the size of polynomial variables (regression)
* [#15698](http://bugzilla.scilab.org/show_bug.cgi?id=15698): `intg` raised a roundoff error with a trivial integral
* [#15710](http://bugzilla.scilab.org/show_bug.cgi?id=15710): The Xcos EXPRESSION block did not accept expressions including some `&` or `~` characters.
* [#15718](http://bugzilla.scilab.org/show_bug.cgi?id=15718): `bitand`, `bitxor` and `bitor` might miswork with some `uint64` inputs.
* [#15721](http://bugzilla.scilab.org/show_bug.cgi?id=15721): An error while using `save(File, "-append", ..)` deleted the existing file.
* [#15724](http://bugzilla.scilab.org/show_bug.cgi?id=15724): `[]^3` displayed error messages about xerbla lapack library (regression)
* [#15728](http://bugzilla.scilab.org/show_bug.cgi?id=15728): `evstr(s)` returns bad results (and errors) for s being "k" "tmp" "lhs" "rhs" "comm" "vars" "vals"..
* [#15733](http://bugzilla.scilab.org/show_bug.cgi?id=15733): Some fixes after `deg(0*%s)` => `-%inf`.
* [#15735](http://bugzilla.scilab.org/show_bug.cgi?id=15735): `interp1` yielded some []+n warnings.
* [#15736](http://bugzilla.scilab.org/show_bug.cgi?id=15736): `unique` failed with complex numbers.
* [#15740](http://bugzilla.scilab.org/show_bug.cgi?id=15740): now `(%s == %z) == %f` and `(%s ~= %z) == %t`
* [#15741](http://bugzilla.scilab.org/show_bug.cgi?id=15741):  Operations between 2 polynomials with distinct variables no longer called the corresponding overloads (Regression)
* [#15746](http://bugzilla.scilab.org/show_bug.cgi?id=15746): `1/[1 2 3]` and `[1 2 3]'\1` did not raise an error
* [#15747](http://bugzilla.scilab.org/show_bug.cgi?id=15747): no output in overloaded operator crashed Scilab
* [#15748](http://bugzilla.scilab.org/show_bug.cgi?id=15748): `min(Sparse,"r"|"c")` and `max(Sparse,"r"|"c")` always returned vectors of zeros. In addition, with sparse inputs,  `min` and `max` did not compute indices as second output.
* [#15751](http://bugzilla.scilab.org/show_bug.cgi?id=15751): `min(sparse([],[]))` returned 0 instead of []. `max()` as well.
* [#15752](http://bugzilla.scilab.org/show_bug.cgi?id=15752): `min` and `max` did not ignore `Nan` values in sparse inputs.
* [#15753](http://bugzilla.scilab.org/show_bug.cgi?id=15753): `issparse()` returned 0 instead of 1 for sparse booleans.
* [#15755](http://bugzilla.scilab.org/show_bug.cgi?id=15755): `min(A1,A2,..)` and `max(A1,A2,..)` did not allow mixing sparse with scalar inputs.
* [#15757](http://bugzilla.scilab.org/show_bug.cgi?id=15757): The public function `xcosPalAddBlock` did not load scicos_scicoslib when needed, and then yielded an error. When the block is specified as a .sod file and the file does not exist, the error message was obscur.
* [#15758](http://bugzilla.scilab.org/show_bug.cgi?id=15758): sparse([0 0],1) crashes scilab (regression)
* [#15762](http://bugzilla.scilab.org/show_bug.cgi?id=15762): `error()` refused multiline error messages.
* [#15770](http://bugzilla.scilab.org/show_bug.cgi?id=15770): zp2ss and zp2tf unitary tests were failing
* [#15774](http://bugzilla.scilab.org/show_bug.cgi?id=15774): `clean()` failed on sparse complex matrix.
* [#15776](http://bugzilla.scilab.org/show_bug.cgi?id=15776): `assert_checktrue` and `assert_checkfalse` did not accept a sparse boolean input. When failing, their error message did not indicate the index of the failing entry. The failing message of `assert_checkfalse` was wrong.
* [#15778](http://bugzilla.scilab.org/show_bug.cgi?id=15778): Deleting some rows or columns of a boolean sparse array with `= []` crashed Scilab
* [#15779](http://bugzilla.scilab.org/show_bug.cgi?id=15779): `test_run` did not accept the wildcard * in tests names.
* [#15796](http://bugzilla.scilab.org/show_bug.cgi?id=15796): Display after `mprintf("")` crashed Scilab (regression)
* [#15799](http://bugzilla.scilab.org/show_bug.cgi?id=15799): `ifftshift()` could not invert `fftshift(x, along)` directional actions.
* [#15800](http://bugzilla.scilab.org/show_bug.cgi?id=15800): Distributive assignments of a property values in a vector of handles were not supported.
* [#15805](http://bugzilla.scilab.org/show_bug.cgi?id=15805): `colorbar()` ticking was poor and most often irrelevant or puzzling, set to not simple values, with useless unreadable minor ticks.
* [#15806](http://bugzilla.scilab.org/show_bug.cgi?id=15806): `colorbar(..)` missed some simple syntaxes with default umin, umax and colminmax values, such as `colorbar()`.
* [#15808](http://bugzilla.scilab.org/show_bug.cgi?id=15808): `[5i]` was parsed as `[5,i]`
* [#15809](http://bugzilla.scilab.org/show_bug.cgi?id=15809): HDF5 load/save was super slow for nested lists.
* [#15811](http://bugzilla.scilab.org/show_bug.cgi?id=15811): comparison of containers embedding cells failed.
* [#15813](http://bugzilla.scilab.org/show_bug.cgi?id=15813): In polarplot mode, datatips displayed irrelevant cartesian coordinates instead of polar ones.
* [#15814](http://bugzilla.scilab.org/show_bug.cgi?id=15814): Selecting graphic children with booleans yielded an error.
* [#15815](http://bugzilla.scilab.org/show_bug.cgi?id=15815): After `polarplot()`, reversing axes or switching `gca().rotation_angles` shifted all angular and radial labels.
* [#15816](http://bugzilla.scilab.org/show_bug.cgi?id=15816): `polarplot()`, `pie()`, `mesh()` and  `contourf()` ignored / canceled any upstream `drawlater`.
* [#15817](http://bugzilla.scilab.org/show_bug.cgi?id=15817): `ascii(c)` with c>=128 crashed on OSX.
* [#15818](http://bugzilla.scilab.org/show_bug.cgi?id=15818): `polarplot()` example could not be subplotted. `polarplot`'s demo did not display 'View code' and too often prompted the user.
* [#15827](http://bugzilla.scilab.org/show_bug.cgi?id=15828): After `bode(..'rad')`, abscissae were titled 'Fréquence' instead of 'Pulsation' in french, and datatips still shew 'Hz' instead of 'rad/s'.
* [#15830](http://bugzilla.scilab.org/show_bug.cgi?id=15830): `linspace` was not reliable for series of encoded integers.
* [#15847](http://bugzilla.scilab.org/show_bug.cgi?id=15847): Scilab crashed at startup.
* [#15850](http://bugzilla.scilab.org/show_bug.cgi?id=15850): Adding a space after a string increases its size.
* [#15853](http://bugzilla.scilab.org/show_bug.cgi?id=15853): `mlist("cblock")` displayed endlessly "operation +: Warning adding..."
* [#15854](http://bugzilla.scilab.org/show_bug.cgi?id=15854): `horner(1:$,4)` yielded an error. Extractions from `cblock` tables were impacted.
* [#15858](http://bugzilla.scilab.org/show_bug.cgi?id=15858): `size(cblock,'r'|'c'|'*'|1|2)` did not take the option into account. `size()` of an empty cblock table yielded an error.
* [#15859](http://bugzilla.scilab.org/show_bug.cgi?id=15859): `isempty` could not be overloaded for custom mlist or tlist typeof.
* [#15860](http://bugzilla.scilab.org/show_bug.cgi?id=15860): Horizontal concatenations with cblock tables had troubles.
* [#15866](http://bugzilla.scilab.org/show_bug.cgi?id=15866): Unlike all other set functions, `setdiff` did not accept any "r" or "c" option.
* [#15867](http://bugzilla.scilab.org/show_bug.cgi?id=15867): For input encoded integers, `setdiff` returned selected elements in decreasing order.
* [#15873](http://bugzilla.scilab.org/show_bug.cgi?id=15873): In average, `sprand(100, 100, 0.8)` yielded ~8800 non-zero values instead of ~8000.
* [#15878](http://bugzilla.scilab.org/show_bug.cgi?id=15878): `sgrid` and `evans` were broken.
* [#15880](http://bugzilla.scilab.org/show_bug.cgi?id=15880): `sgrid` needed some improvements: Labeling was sometimes ambiguous ; large circles were not labeled ; data_bounds did not always take the input wn into account ; named and #RRGGBB colors specifications could not be used. `evans` needed some improvements: the block of legends hid data ; asymptotes were too visible.
* [#15881](http://bugzilla.scilab.org/show_bug.cgi?id=15881): Demos about stems, bars and histograms were spread in several subsections.
* [#15884](http://bugzilla.scilab.org/show_bug.cgi?id=15884): `bode` frequencies ticking and grid style were poor. With legends, the phase plot was vertically shrunk and some graphical space was lost below it.
* [#15886](http://bugzilla.scilab.org/show_bug.cgi?id=15886): Display of polynomials was broken.
* [#15890](http://bugzilla.scilab.org/show_bug.cgi?id=15890): `evstr` sometimes yielded some `+[]` warnings.
* [#15891](http://bugzilla.scilab.org/show_bug.cgi?id=15891): Help pages of matrix-wise trigonometrical functions deserved a dedicated subsection.
* [#15898](http://bugzilla.scilab.org/show_bug.cgi?id=15898): `edit` failed 1) for any user-defined macro  2) for a native macro recompiled from Scinotes  3) for a native macro addressed through an alias (as sine=sind)
* [#15899](http://bugzilla.scilab.org/show_bug.cgi?id=15899): `tree2code(tree, prettyprint=%t)` ignored the indentation width declared in Scinotes preferences.
* [#15907](http://bugzilla.scilab.org/show_bug.cgi?id=15907): `filter` was corrupting its input state array
* [#15920](http://bugzilla.scilab.org/show_bug.cgi?id=15920): genlib() did not regenerate a missing .bin if the .sci was unchanged (Scilab 6 regression)
* [#15922](http://bugzilla.scilab.org/show_bug.cgi?id=15922): `rcond([])` returned `[]` instead of `Inf` consistently with `cond([])==0`.
* [#15924](http://bugzilla.scilab.org/show_bug.cgi?id=15924): sparse inequality to 0 yielded a wrong result.


### Bugs fixed in 6.0.1:
* [#2441](http://bugzilla.scilab.org/show_bug.cgi?id=2441): `%onprompt()` could be defined only at the top execution level.
* [#3256](http://bugzilla.scilab.org/show_bug.cgi?id=3256): The cursor of slider uicontrol is not displayed if the slider step is not an integer value.
* [#4276](http://bugzilla.scilab.org/show_bug.cgi?id=4276): `strsubst` replaced the first occurence in regex mode.
* [#5248](http://bugzilla.scilab.org/show_bug.cgi?id=5248): The graphical entity picker was out of work.
* [#5278](http://bugzilla.scilab.org/show_bug.cgi?id=5278): obsolete `xset()` was still used in scripts, macros, tests and help pages.
* [#5381](http://bugzilla.scilab.org/show_bug.cgi?id=5381): For UTF-8 inputs, `msprintf`, `mprintf` and `mfprintf` shortened the output.
* [#5567](http://bugzilla.scilab.org/show_bug.cgi?id=5567): `bar(.., colors)` used with grouped or stacked subsets, only a unique common meaningless color could be specified. Only a restricted list of 10 colors names could be used. Some arguments checking were wrong.
* [#5602](http://bugzilla.scilab.org/show_bug.cgi?id=5602): Assigning a component of a list in a cells array replaced the whole list.
* [#5611](http://bugzilla.scilab.org/show_bug.cgi?id=5611): It was not possible to delete the row or column of a cells array.
* [#6155](http://bugzilla.scilab.org/show_bug.cgi?id=6155): Pages of `param3d()` and `param3d1()` deserved being fixed, improved and merged in a single page.
* [#6607](http://bugzilla.scilab.org/show_bug.cgi?id=6607): `clear S; S(1:2,1:3).a = 1` yielded an error.
* [#6608](http://bugzilla.scilab.org/show_bug.cgi?id=6608): Field and data insertion in an array of structures might fail.
* [#6813](http://bugzilla.scilab.org/show_bug.cgi?id=6813): `makecell` used to create a N>2-D hyperarray yielded an error.
* [#6911](http://bugzilla.scilab.org/show_bug.cgi?id=6911): `help_from_sci` did not accept numerical HTML entities like `&#969;` in heading comments.
* [#6973](http://bugzilla.scilab.org/show_bug.cgi?id=6973): It was impossible to view figures of several demos in a stable docked panel.
* [#7652](http://bugzilla.scilab.org/show_bug.cgi?id=7652): Inserting `list("")` in a cells array could be erroneous.
* [#8140](http://bugzilla.scilab.org/show_bug.cgi?id=8140): Editing parameters of the EXPRESSION Xcos block changed Scilab's IEEE mode.
* [#8297](http://bugzilla.scilab.org/show_bug.cgi?id=8297): `cat` slowness was exponential, crippling, and made it useless.
* [#8629](http://bugzilla.scilab.org/show_bug.cgi?id=8629): On the help page of the Xcos Sinks palette, blocks screenshots were stacked and not hyperlinked to their page.
* [#8669](http://bugzilla.scilab.org/show_bug.cgi?id=8669): After `A=rand(3,3)`, some legal insertions in `A(*,*,:)` failed. Non regression tests added.
* [#8842](http://bugzilla.scilab.org/show_bug.cgi?id=8842): Assigning the pointer of a builtin function to a new structure failed.
* [#9297](http://bugzilla.scilab.org/show_bug.cgi?id=9297): Assigning a mlist to a structure's field failed.
* [#9519](http://bugzilla.scilab.org/show_bug.cgi?id=9519): The console menu `File -> Open a file` opened any file in Scinotes. Binary files like .zcos blocked Scinotes.
* [#9617](http://bugzilla.scilab.org/show_bug.cgi?id=9617): Xcos run-time version check was too conservative.
* [#9838](http://bugzilla.scilab.org/show_bug.cgi?id=9838): `evstr` poorly supported simple comments in input expressions.
* [#10270](http://bugzilla.scilab.org/show_bug.cgi?id=10270): `execstr(["%val=[int16(1)";"]"])` needed entering twice <enter> to be actually performed.
* [#10407](http://bugzilla.scilab.org/show_bug.cgi?id=10407): The macro `acf()` of the CACSD module was used nowhere and not documented. It is removed.
* [#10467](http://bugzilla.scilab.org/show_bug.cgi?id=10467): `execstr` inside a `try/end` with a `catch` killed the stack.
* [#11196](http://bugzilla.scilab.org/show_bug.cgi?id=11196): Some recursive extractions from a mlist failed.
* [#11228](http://bugzilla.scilab.org/show_bug.cgi?id=11228): Some recursive extraction from a mlist was not done in the right order and yielded a stack problem.
* [#11251](http://bugzilla.scilab.org/show_bug.cgi?id=11251): `execstr` could kill the variables stack.
* [#11483](http://bugzilla.scilab.org/show_bug.cgi?id=11483): The `Graphics => Misc` demo had a hole, 2 duplicates, and other bugs.
* [#11530](http://bugzilla.scilab.org/show_bug.cgi?id=11530): Raise (help, scinotes, ...) windows when iconified ( recall or startup )
* [#11693](http://bugzilla.scilab.org/show_bug.cgi?id=11693): uicontrol edit fields needed <enter> after the input to actually record it.
* [#11756](http://bugzilla.scilab.org/show_bug.cgi?id=11756): In the categories of the ATOMS GUI, actually available modules might be not listed.
* [#11767](http://bugzilla.scilab.org/show_bug.cgi?id=11767): `execstr("A. 1","errcatch")` failed to catch the error.
* [#12195](http://bugzilla.scilab.org/show_bug.cgi?id=12195): `generateBlockImage` sometimes changed the current graphic driver.
* [#12356](http://bugzilla.scilab.org/show_bug.cgi?id=12356): `colordef` did not allow to reset colors to the default default ones.
* [#12372](http://bugzilla.scilab.org/show_bug.cgi?id=12372): The `mode` function was poorly documented.
* [#12586](http://bugzilla.scilab.org/show_bug.cgi?id=12586): An example showing how to get XML comments was missing in the `xmlXPath` help page.
* [#12771](http://bugzilla.scilab.org/show_bug.cgi?id=12771): xcosPalGenerateAllIcons help example was broken.
* [#12792](http://bugzilla.scilab.org/show_bug.cgi?id=12792): `save(filename, "undefinedVariable")` created an empty useless file.
* [#12862](http://bugzilla.scilab.org/show_bug.cgi?id=12862): `intsplin` no longer integrated the imaginary part of given ordinates.
* [#13014](http://bugzilla.scilab.org/show_bug.cgi?id=13014): `optim_ga`: update the Efficiency inner variable.
* [#13277](http://bugzilla.scilab.org/show_bug.cgi?id=13277): `execstr("clear")` did not clear any variables in the current scope.
* [#13581](http://bugzilla.scilab.org/show_bug.cgi?id=13581): The example of `inistate()` help page did not work on Windows 64bit.
* [#13592](http://bugzilla.scilab.org/show_bug.cgi?id=13592): In an axes in a uicontrol frame, setting a `legend` interactively might not follow the mouse accurately.
* [#13796](http://bugzilla.scilab.org/show_bug.cgi?id=13796): `tbx_generate_pofile` missed registering `_()` and `gettext()` references. In `en_US.po`, commented paths were absolute. Literal '' were copied in msgid without simplification.
* [#13900](http://bugzilla.scilab.org/show_bug.cgi?id=13900): `nanmin` and `nanmax` were useless duplicates of `min` and `max`
* [#13962](http://bugzilla.scilab.org/show_bug.cgi?id=13962): Preselecting a groupnamed radiobutton was impossible after having created it already once.
* [#13979](http://bugzilla.scilab.org/show_bug.cgi?id=13979): A variable whose name is longer than 24 characters could not be cleared.
* [#13984](http://bugzilla.scilab.org/show_bug.cgi?id=13984): For a set of curves, `plot(.., "color", colors..)` was not vectorized for colors specifications. Moreover, only 10 basic named colors were accepted, instead of the full list of predefined colors names.
* [#14221](http://bugzilla.scilab.org/show_bug.cgi?id=14221): `msprintf` and `mprintf` ignored row-wise processing with `%%`.
* [#14254](http://bugzilla.scilab.org/show_bug.cgi?id=14254): When installing an ATOMS module that is not locally registered and available, the error message was unclear.
* [#14305](http://bugzilla.scilab.org/show_bug.cgi?id=14305): unused `%i_d_i` was still distributed.
* [#14371](http://bugzilla.scilab.org/show_bug.cgi?id=14371): Scilab crashed when a block is edited.
* [#14376](http://bugzilla.scilab.org/show_bug.cgi?id=14376): `input()` was broken: \n was introduced before prompting; multiple prompts occurred; the output value could be missing; "%" "\n" "\t" were no longer supported in messages...
* [#14399](http://bugzilla.scilab.org/show_bug.cgi?id=14399): `whereami`: wrong information (line numbers).
* [#14424](http://bugzilla.scilab.org/show_bug.cgi?id=14424): There were some troubles with `input`.
* [#14490](http://bugzilla.scilab.org/show_bug.cgi?id=14490): `plot2d` crashed when leg parameter was a vector.
* [#14531](http://bugzilla.scilab.org/show_bug.cgi?id=14531): It was not possible to specify SCIHOME as a start-up option.
* [#14532](http://bugzilla.scilab.org/show_bug.cgi?id=14532): `test_run` failed for toolboxes that are not autoloaded.
* [#14544](http://bugzilla.scilab.org/show_bug.cgi?id=14544): `scatter` and `scatter3` canceled any upstream `drawlater` instruction.
* [#14551](http://bugzilla.scilab.org/show_bug.cgi?id=14551): In the icon of the Xcos CLR and DLR blocks, multichar exponents were badly rendered.
* [#14573](http://bugzilla.scilab.org/show_bug.cgi?id=14573): `varargout` misworked when it was used in addition to fixed output arguments.
* [#14588](http://bugzilla.scilab.org/show_bug.cgi?id=14588): Some cross usages of launching options `-nw` and `-nwni` with Wscilex.exe and Scilex.exe binaries were inconsistent.
* [#14584](http://bugzilla.scilab.org/show_bug.cgi?id=14584): The example of demo_choose help page was broken because of a wrong file path.
* [#14598](http://bugzilla.scilab.org/show_bug.cgi?id=14598): `fort` wasn't properly removed.
* [#14603](http://bugzilla.scilab.org/show_bug.cgi?id=14603): The icon of the PROD_f Xcos block was puzzled
* [#14623](http://bugzilla.scilab.org/show_bug.cgi?id=14623): Bad lhs in MList extraction overload.
* [#14629](http://bugzilla.scilab.org/show_bug.cgi?id=14629): In the Xcos EXPRESSION block, `<` could truncate the rendering of the expression in the icon.
* [#14636](http://bugzilla.scilab.org/show_bug.cgi?id=14636): Xcos model with modelica electrical blocks (created in 5.5.2) crashed Scilab 6.
* [#14637](http://bugzilla.scilab.org/show_bug.cgi?id=14367): Some Scilab 5.5.2 diagrams did not simulate properly in Xcos.
* [#14656](http://bugzilla.scilab.org/show_bug.cgi?id=14656): The `matrix` page missed a synopsis for hypermatrix and deserved some improvements.
* [#14680](http://bugzilla.scilab.org/show_bug.cgi?id=14680): The `<` char in expression was not rendered in the icon of Xcos EXPRESSION block.
* [#14703](http://bugzilla.scilab.org/show_bug.cgi?id=14703): `clear linspace, type(linspace)` returned 11 instead of 13.
* [#14749](http://bugzilla.scilab.org/show_bug.cgi?id=14749): The icon of the Xcos `POWBLK_f` and `EXPBLK_m` blocks displayed "u^a" and "a^u" instead of the actual a value.
* [#14883](http://bugzilla.scilab.org/show_bug.cgi?id=14883): `whereis` did not support builtin functions.
* [#14886](http://bugzilla.scilab.org/show_bug.cgi?id=14886): `Matplot` save/load failed.
* [#14896](http://bugzilla.scilab.org/show_bug.cgi?id=14896): Using `set` with multiple properties, only the first one was set.
* [#14901](http://bugzilla.scilab.org/show_bug.cgi?id=14901): WScilex.exe was stopped immediately when launched from Java.
* [#14903](http://bugzilla.scilab.org/show_bug.cgi?id=14903): `plot2d` crashed if a current figure has no axes.
* [#14910](http://bugzilla.scilab.org/show_bug.cgi?id=14910): The `plot()` example was displayed in overlay to the existing graphics.
* [#14922](http://bugzilla.scilab.org/show_bug.cgi?id=14922): The `%onprompt()` special function was not documented.
* [#14925](http://bugzilla.scilab.org/show_bug.cgi?id=14925): `jdeff`: `invoke_lu()` deleted/corrupted the called java method after an error occured.
* [#14981](http://bugzilla.scilab.org/show_bug.cgi?id=14981): Some vertical concatenations of cells and further cells extractions crash Scilab.
* [#14984](http://bugzilla.scilab.org/show_bug.cgi?id=14984): In palettes pages, blocks screenshots were not aligned nor hyperlinked.
* [#15006](http://bugzilla.scilab.org/show_bug.cgi?id=15006): `ode` help page still contained 'root' which has been replaced by 'roots'.
* [#15008](http://bugzilla.scilab.org/show_bug.cgi?id=15008): scilab crashed when using operator AND (&, &&) or OR (| ||) with a string.
* [#15010](http://bugzilla.scilab.org/show_bug.cgi?id=15010): Coselica did not simulate on Scilab 6.
* [#15015](http://bugzilla.scilab.org/show_bug.cgi?id=15015): Xcos blocks using the `ascii` didn't work
* [#15017](http://bugzilla.scilab.org/show_bug.cgi?id=15017): On numbers, `gcd` result was always of `int32` type instead of matching the input type.
* [#15018](http://bugzilla.scilab.org/show_bug.cgi?id=15018): `mfprintf`'\n' not working.
* [#15019](http://bugzilla.scilab.org/show_bug.cgi?id=15019): Add 'csci6' in the calling of ilib_build in 'Getting started with API_Scilab' help page.
* [#15020](http://bugzilla.scilab.org/show_bug.cgi?id=15020): Examples of ilib_build help page updated.
* [#15023](http://bugzilla.scilab.org/show_bug.cgi?id=15023): `clf()` wrongly reset `figure_id`.
* [#15024](http://bugzilla.scilab.org/show_bug.cgi?id=15024): Xcos labels were not preserved at diagram compilation.
* [#15028](http://bugzilla.scilab.org/show_bug.cgi?id=15028): `assert_checkalmostequal` could not work with polynomials.
* [#15034](http://bugzilla.scilab.org/show_bug.cgi?id=15034): It was no longer possible to create a structure through a mlist.
* [#15037](http://bugzilla.scilab.org/show_bug.cgi?id=15037): the `residu` function might crash Scilab.
* [#15039](http://bugzilla.scilab.org/show_bug.cgi?id=15039): New Xcos graphical features needed some demos.
* [#15046](http://bugzilla.scilab.org/show_bug.cgi?id=15046): `call` couldn't mix inputs and outputs.
* [#15052](http://bugzilla.scilab.org/show_bug.cgi?id=15052): `getpid` was no longer available.
* [#15053](http://bugzilla.scilab.org/show_bug.cgi?id=15053): `_str2code` was removed with no proper equivalence and made `mfile2sci` failing.
* [#15054](http://bugzilla.scilab.org/show_bug.cgi?id=15054): The callbacks of `wfir_gui()` were not prioritary.
* [#15057](http://bugzilla.scilab.org/show_bug.cgi?id=15057): `Matplot` `.data` assignation did not take care of >2 dimension
* [#15058](http://bugzilla.scilab.org/show_bug.cgi?id=15058): With integers, `gcd` and `lcm` could return a negative result, depending on the order of negative components.
* [#15060](http://bugzilla.scilab.org/show_bug.cgi?id=15060): `fplot3d` did not draw because of an addition with an empty matrix which now returns an empty matrix.
* [#15063](http://bugzilla.scilab.org/show_bug.cgi?id=15063): `fort` wasn't properly removed.
* [#15066](http://bugzilla.scilab.org/show_bug.cgi?id=15066): Undo did not update ports positions
* [#15070](http://bugzilla.scilab.org/show_bug.cgi?id=15070): `bitset` failed when the bit position is specified as encoded integer.
* [#15072](http://bugzilla.scilab.org/show_bug.cgi?id=15072): The context was stored as a root diagram attribute instead of being stored on each Superblock layer.
* [#15079](http://bugzilla.scilab.org/show_bug.cgi?id=15079): When all children of a graphic handle have not the same number of sub-children, any vectorized extraction or insertion in subchildren failed.
* [#15089](http://bugzilla.scilab.org/show_bug.cgi?id=15089): The `getcolor` GUI cleared and used the current figure.
* [#15106](http://bugzilla.scilab.org/show_bug.cgi?id=15106): Operation without operand crashed Scilab.
* [#15107](http://bugzilla.scilab.org/show_bug.cgi?id=15107): Scilab failed to build with ocaml 4.04.0.
* [#15111](http://bugzilla.scilab.org/show_bug.cgi?id=15111): Summation block symbol displayed minus sign instead of plus sign.
* [#15117](http://bugzilla.scilab.org/show_bug.cgi?id=15117): Imbricated calls to `optim` / `leastsq` / `lsqrsolve` / `fsolve` either crashed or returned errors.
* [#15119](http://bugzilla.scilab.org/show_bug.cgi?id=15119): Inequality comparisons between complex-encoded real numbers failed.
* [#15121](http://bugzilla.scilab.org/show_bug.cgi?id=15121): `getOptionals` fails to recognize some options.
* [#15137](http://bugzilla.scilab.org/show_bug.cgi?id=15137): `ilib_build` could not build a library with more than 999 function entries in Scilab 6
* [#15138](http://bugzilla.scilab.org/show_bug.cgi?id=15138): Scilab crashed at startup on Windows.
* [#15139](http://bugzilla.scilab.org/show_bug.cgi?id=15139): Fix return in `try` / `catch` expression
* [#15142](http://bugzilla.scilab.org/show_bug.cgi?id=15142): `mopen()` wrong err value
* [#15144](http://bugzilla.scilab.org/show_bug.cgi?id=15144): `mean` and `stdev` could not be overloaded.
* [#15146](http://bugzilla.scilab.org/show_bug.cgi?id=15146): `lasterror` crashed Scilab when last error message is empty.
* [#15148](http://bugzilla.scilab.org/show_bug.cgi?id=15148): `link` did not return any error for a wrong flag argument value.
* [#15149](http://bugzilla.scilab.org/show_bug.cgi?id=15149): Files saved as ZCOS were corrupted.
* [#15170](http://bugzilla.scilab.org/show_bug.cgi?id=15170): `getd` created and returned a variable `k` that could overwrite a loaded one.
* [#15183](http://bugzilla.scilab.org/show_bug.cgi?id=15183): `ndgrid(x)` with by default `y=x` was not accepted.
* [#15184](http://bugzilla.scilab.org/show_bug.cgi?id=15184): `(1+%i) < 2` crashed Scilab.
* [#15185](http://bugzilla.scilab.org/show_bug.cgi?id=15185): "Region to Superblock" did not restore broken links.
* [#15188](http://bugzilla.scilab.org/show_bug.cgi?id=15188): `sci_hdf5_is_file` was wrongly named in the hdf5 DLL.
* [#15199](http://bugzilla.scilab.org/show_bug.cgi?id=15199): Scilab crashed when using `fsolve` for functions containing other functions.
* [#15205](http://bugzilla.scilab.org/show_bug.cgi?id=15205): `get_scicos_version` returned the previous version.
* [#15206](http://bugzilla.scilab.org/show_bug.cgi?id=15206): Windows default `mopen` read/write mode was text instead of binary.
* [#15207](http://bugzilla.scilab.org/show_bug.cgi?id=15207): `xcos(scs_m)` did not work for sub-systems.
* [#15215](http://bugzilla.scilab.org/show_bug.cgi?id=15215): `strcat` did not work correctly with empty strings.
* [#15223](http://bugzilla.scilab.org/show_bug.cgi?id=15223): `call` crashed in some cases.
* [#15228](http://bugzilla.scilab.org/show_bug.cgi?id=15228): `get` error message fixed.
* [#15236](http://bugzilla.scilab.org/show_bug.cgi?id=15236): The `isglobal` help page was inaccurate. Examples were erroneous.
* [#15239](http://bugzilla.scilab.org/show_bug.cgi?id=15239): The console properties `.tag` and `.userdata` were not displayed nor documented.
* [#15243](http://bugzilla.scilab.org/show_bug.cgi?id=15243): `grand(nr, nc, "uin", vmin)` crashed Scilab.
* [#15246](http://bugzilla.scilab.org/show_bug.cgi?id=15246): `chsolve` called a `blkslv` missing function.
* [#15249](http://bugzilla.scilab.org/show_bug.cgi?id=15249): `findobj("toto")` yielded an error instead of returning [].
* [#15260](http://bugzilla.scilab.org/show_bug.cgi?id=15260): `sci2exp` was broken for cells, structures, and all types of hypermatrix.
* [#15261](http://bugzilla.scilab.org/show_bug.cgi?id=15261): Insertion in struct felt with wrong default value.
* [#15266](http://bugzilla.scilab.org/show_bug.cgi?id=15266): Wrong default out_mode in `linear_interpn`.
* [#15273](http://bugzilla.scilab.org/show_bug.cgi?id=15273): The documented order of `eval_cshep2d` second derivatives was wrong.
* [#15278](http://bugzilla.scilab.org/show_bug.cgi?id=15278): Moving Xcos blocks reverted rotation and flip/mirror properties
* [#15282](http://bugzilla.scilab.org/show_bug.cgi?id=15282): `help_from_sci` could set an xml:id starting with the forbidden `%` character.
* [#15285](http://bugzilla.scilab.org/show_bug.cgi?id=15285): `resize_matrix` did not manage conversion into `int64` and `uint64`.
* [#15300](http://bugzilla.scilab.org/show_bug.cgi?id=15300): Distributive assignments like `a=(1,2)` crashed Scilab.
* [#15301](http://bugzilla.scilab.org/show_bug.cgi?id=15301): Sequence constructor `:` failed when a bound was an outputless funtion call.
* [#15302](http://bugzilla.scilab.org/show_bug.cgi?id=15302): `load` crashed when inputs arguments are not strings.
* [#15303](http://bugzilla.scilab.org/show_bug.cgi?id=15303): Some properties written in capital letter in set function crashed Scilab.
* [#15306](http://bugzilla.scilab.org/show_bug.cgi?id=15306): After the behavior of `||` was changed, its help page was not up to date.
* [#15308](http://bugzilla.scilab.org/show_bug.cgi?id=15308): Unlike `evstr(["1 2";"3 4"])`, `evstr(["1;2" "3;4"])` was not accepted.
* [#15328](http://bugzilla.scilab.org/show_bug.cgi?id=15328): OpenJDK 9 was not supported for custom builds.
* [#15337](http://bugzilla.scilab.org/show_bug.cgi?id=15337): There was a mistake in the definition of the l_p norm of a vector in the `norm` page.
* [#15339](http://bugzilla.scilab.org/show_bug.cgi?id=15339): `uigetfont()` returned garbage on user cancellation.
* [#15340](http://bugzilla.scilab.org/show_bug.cgi?id=15340): Scilab did not build against lucene 6.1.0.
* [#15342](http://bugzilla.scilab.org/show_bug.cgi?id=15342): `replot()` and `Reframe to contents` failed on graphics with xstring objects.
* [#15347](http://bugzilla.scilab.org/show_bug.cgi?id=15347): `toeplitz` failed with rationals.
* [#15354](http://bugzilla.scilab.org/show_bug.cgi?id=15354): `invr(%s^2)` returned 0 instead of `1/%s^2`. For any scalar number, polynomial or rational `a`, `coffg(a)` returned `0` instead of `1`. `coffg([])` yielded an error. `invr` and `coffg` had no unitary tests. The `coffg` help page was inaccurate and unclear.
* [#15360](http://bugzilla.scilab.org/show_bug.cgi?id=15360): `numer()` and `denom()` were almost useless, unused, and with more handy replacements. They are declared obsolete to be removed in Scilab 6.1.0.
* [#15363](http://bugzilla.scilab.org/show_bug.cgi?id=15363): `h2norm()` could no longer be applied to undefined time domain systems, and made the residu() right example failing.
* [#15368](http://bugzilla.scilab.org/show_bug.cgi?id=15368): `freson()` wrongly returned [] (no peak frequency detected) for some continuous time linear systems.
* [#15370](http://bugzilla.scilab.org/show_bug.cgi?id=15370): `bezout()` mishandled its output arguments.
* [#15375](http://bugzilla.scilab.org/show_bug.cgi?id=15375): A .zcos file opened as a palette was greyed out.
* [#15379](http://bugzilla.scilab.org/show_bug.cgi?id=15379): `zeros(A)` was not documented as equivalent to `mtlb_zeros(size(A))`.
* [#15380](http://bugzilla.scilab.org/show_bug.cgi?id=15380): `argn()` documentation was somewhat unclear and uncomplete.
* [#15388](http://bugzilla.scilab.org/show_bug.cgi?id=15388): Dynamic link did not work on MACOSX 10,12.
* [#15389](http://bugzilla.scilab.org/show_bug.cgi?id=15389): `Wscilex -nw` failed and created a ghost process.
* [#15390](http://bugzilla.scilab.org/show_bug.cgi?id=15390): In online help pages, tooltips showing the code on images were no longer displayed.
* [#15395](http://bugzilla.scilab.org/show_bug.cgi?id=15395): `ones(2,3,2) / %z` yielded an error..
* [#15396](http://bugzilla.scilab.org/show_bug.cgi?id=15396): `[m,n,p] = size(hr)` yielded an error with an hypermatrix hr of rationals.
* [#15402](http://bugzilla.scilab.org/show_bug.cgi?id=15402): The `range()` page was not fixed against the bug 1904 for the french and portuguese versions.
* [#15405](http://bugzilla.scilab.org/show_bug.cgi?id=15405): Accessing int64 hdf5 values crashed scilab.
* [#15413](http://bugzilla.scilab.org/show_bug.cgi?id=15413): Deleting some ranges (rows, columns, pages, ..) in a structures array failed.
* [#15418](http://bugzilla.scilab.org/show_bug.cgi?id=15418): With `colordef('black')` and `colordef('none')`, black title and labels were invisible on the black background.
* [#15419](http://bugzilla.scilab.org/show_bug.cgi?id=15419): `tbx_build_localization` did nothing and silently returned when no `~/locales` directory was detected. `tbx_generate_pofile()` was not run, and the user was not warned and guided.
* [#15420](http://bugzilla.scilab.org/show_bug.cgi?id=15420): insertion at negative index did not trigger an error.
* [#15456](http://bugzilla.scilab.org/show_bug.cgi?id=15456): Xcos 5.5.x files did not load properly.
* [#15459](http://bugzilla.scilab.org/show_bug.cgi?id=15459): Generating code from an Xcos superblock generated warnings.


### Bugs fixed in 6.0.0:
* [#592](http://bugzilla.scilab.org/show_bug.cgi?id=592): `linspace(a, b, n<=0)` returned `b` instead of `[]`
* [#2835](http://bugzilla.scilab.org/show_bug.cgi?id=2835): On negative "initial event", EVTDLY_c took no notice of the input.
* [#2919](http://bugzilla.scilab.org/show_bug.cgi?id=2919): The `fchamp` example and demo were unclear and badly rendered
* [#4327](http://bugzilla.scilab.org/show_bug.cgi?id=4327): Overloading did not support custom types names longer than 8 characters
* [#5278](http://bugzilla.scilab.org/show_bug.cgi?id=5278): Most of the references to `xset` and `xget` in scripts, macros, help pages and tests were obsolete.
* [#5723](http://bugzilla.scilab.org/show_bug.cgi?id=5723): Cross-references were missing between `axis_properties` and `axes_properties` help pages
* [#6307](http://bugzilla.scilab.org/show_bug.cgi?id=6307): There were no  easy versions of `lqr`, `lqe`, and `lqg`
* [#7192](http://bugzilla.scilab.org/show_bug.cgi?id=7192): From `S=[]`, `S($+1,:) = some_row` inserted it in row#2 after a parasitic row#1.
* [#7649](http://bugzilla.scilab.org/show_bug.cgi?id=7649): `isempty` returned `%F` on `struct()`, `{}` or `list(,)` and was not shortcut
* [#7696](http://bugzilla.scilab.org/show_bug.cgi?id=7696): The `parallel_run` help page was poorly formatted
* [#7794](http://bugzilla.scilab.org/show_bug.cgi?id=7794): The example in the `findABCD` help page failed.
* [#7958](http://bugzilla.scilab.org/show_bug.cgi?id=7958): `mrfit`did not allow a fourth parameter as shown in the help page.
* [#8010](http://bugzilla.scilab.org/show_bug.cgi?id=8010): Permanent variables could be redefined through a syntax like `%i(1,1)=1`
* [#8190](http://bugzilla.scilab.org/show_bug.cgi?id=8190): Fixed ICSE demos of Optimization module.
* [#8356](http://bugzilla.scilab.org/show_bug.cgi?id=8356): `sci2exp` applied to lists, tlists or mlists having undefined fields yielded an error or a wrong result.
* [#8493](http://bugzilla.scilab.org/show_bug.cgi?id=8493): Some trivial simplifications of `p1./p2` with matrices of complex-encoded polynomials were not done.
* [#8841](http://bugzilla.scilab.org/show_bug.cgi?id=8841): After `s.a=list(4,7)`, `s.a` was not equal to `s(1).a`
* [#8938](http://bugzilla.scilab.org/show_bug.cgi?id=8938): In a boolean sparse matrix `sp`, distributive insertions like `sp(1,:)=%t`, `sp(1,1:$)=%t` or `sp(:,:)=%t` yielded an error.
* [#9008](http://bugzilla.scilab.org/show_bug.cgi?id=9008): `test_run` applied the `create_ref` option even for tests having the `<-- NO CHECK REF -->` flag.
* [#9153](http://bugzilla.scilab.org/show_bug.cgi?id=9153): The `isqualbitwise` help page was inaccurate and badly located
* [#9161](http://bugzilla.scilab.org/show_bug.cgi?id=9161): Multiple insertions at a repeated index in a sparse matrice wrongly updated it.
* [#9288](http://bugzilla.scilab.org/show_bug.cgi?id=9288): No palette dynamically built with the most used blocks was available
* [#9451](http://bugzilla.scilab.org/show_bug.cgi?id=9451): `test_run` output did not clearly distinguish heading lines of modules and tests lines
* [#9825](http://bugzilla.scilab.org/show_bug.cgi?id=9825): `assert_computedigits` returns too much digits
* [#9865](http://bugzilla.scilab.org/show_bug.cgi?id=9865): When making a plot with `point`(no line), no symbol was shown in the legend.
* [#9876](http://bugzilla.scilab.org/show_bug.cgi?id=9876): Creating a complex structure with multiple hierarchy level and size failed.
* [#9912](http://bugzilla.scilab.org/show_bug.cgi?id=9912): In case of missing translated help page, its default `en_US` version was sometimes ignored
* [#10116](http://bugzilla.scilab.org/show_bug.cgi?id=10116): `for h = H, .., end` could not be used when H is a vector of graphic handles
* [#10195](http://bugzilla.scilab.org/show_bug.cgi?id=10195): `execstr` interpreted ascii(0) to ascii(31) characters as the power `^` operator.
* [#10326](http://bugzilla.scilab.org/show_bug.cgi?id=10326): The palette browser didn't have any search engine.
* [#10981](http://bugzilla.scilab.org/show_bug.cgi?id=10981): It was possible to rotate a 2D axes, and hard to get it back to a 2D view.
* [#11375](http://bugzilla.scilab.org/show_bug.cgi?id=11375): When a localized help subdirectory has only a `CHAPTER` file specifying the section title, this one was ignored.
* [#11476](http://bugzilla.scilab.org/show_bug.cgi?id=11476): `clf("reset")` used on a docked figure resized and moved the whole docked block like the Scilab desktop.
* [#11692](http://bugzilla.scilab.org/show_bug.cgi?id=11692): The summary of a help section built from both default `en_US` and localized files was never sorted overall.
* [#11959](http://bugzilla.scilab.org/show_bug.cgi?id=11959): Selecting a zoom area starting on some axes borders was hard and tricky.
* [#12017](http://bugzilla.scilab.org/show_bug.cgi?id=12017): The on-screen rendering according to `figure.pixel_drawing_mode` was out of work since Scilab 5.4
* [#12110](http://bugzilla.scilab.org/show_bug.cgi?id=12110): Zooming multiple side-by-side or overlaying axes at once was out of work since Scilab 5.4
* [#12417](http://bugzilla.scilab.org/show_bug.cgi?id=12417): Set "All supported formats" as default selected on Xcos open.
* [#12431](http://bugzilla.scilab.org/show_bug.cgi?id=12431): The page describing the `%helps` variable needed clarification.
* [#12453](http://bugzilla.scilab.org/show_bug.cgi?id=12453): In the Xcos palette browser, enabling or disabling some category resized the left panel.
* [#12623](http://bugzilla.scilab.org/show_bug.cgi?id=12623): When `%onprompt()` is defined, variables defined in any callback of a console's menu were not accessible in the console.
* [#12897](http://bugzilla.scilab.org/show_bug.cgi?id=12897): Renamed `optim`'s `imp` argument to `"iprint"`.
* [#13217](http://bugzilla.scilab.org/show_bug.cgi?id=13217): `augment` was wrong when `flag2` was `"i"`
* [#13166](http://bugzilla.scilab.org/show_bug.cgi?id=13166): `l` and `b` endian flags used with `mget` and `mgeti` were sticky
* [#13375](http://bugzilla.scilab.org/show_bug.cgi?id=13375): For a uicontrol listbox, `.Max - .Min==1` prevented any multiple selection. The default value documented for uicontrol`.relief` was wrong.
* [#13401](http://bugzilla.scilab.org/show_bug.cgi?id=13401): Scilab became a ghost process when it was closed while an `input` or a `halt` instruction was being performed in a callback of an undockable figure.
* [#13583](http://bugzilla.scilab.org/show_bug.cgi?id=13583): `getd` loading a script including a `clear` instruction yielded an error.
* [#13597](http://bugzilla.scilab.org/show_bug.cgi?id=13597): `help format` claimed setting a number of digits instead of a number of characters.
* [#13613](http://bugzilla.scilab.org/show_bug.cgi?id=13613): `isdef(name, 'l')` produced wrong output.
* [#13620](http://bugzilla.scilab.org/show_bug.cgi?id=13620): `dos` called with a vector of OS instructions crashed Scilab.
* [#13651](http://bugzilla.scilab.org/show_bug.cgi?id=13651): It was not possible to `copy` an axes into an uicontrol frame.
* [#13757](http://bugzilla.scilab.org/show_bug.cgi?id=13757): The `Toolboxes` menu dit not load properly not autoloaded ATOMS modules.
* [#13759](http://bugzilla.scilab.org/show_bug.cgi?id=13759): At startup, sometimes autoloadable Atoms modules were not loaded, randomly.
* [#13794](http://bugzilla.scilab.org/show_bug.cgi?id=13794): It was not possible to toggle the display of a datatip just by clicking on its anchor or by overflying its anchor with the mouse pointer.
* [#13856](http://bugzilla.scilab.org/show_bug.cgi?id=13856): `messagebox` crashed under Windows in 5.5 Scilab version and updated in version 6.
* [#13877](http://bugzilla.scilab.org/show_bug.cgi?id=13877): In help pages, `<` characters included in `<screen>` areas were not rendered in the help browser.
* [#13878](http://bugzilla.scilab.org/show_bug.cgi?id=13878): `tokens([])` yielded an error instead of returning `[]`.
* [#13895](http://bugzilla.scilab.org/show_bug.cgi?id=13895): After `p.a.h = 1; p.b.h = 3;`, `p(:).h` crashed Scilab.
* [#13906](http://bugzilla.scilab.org/show_bug.cgi?id=13906): Arrows keys did not allow to navigate through the Palette browser.
* [#13990](http://bugzilla.scilab.org/show_bug.cgi?id=13990): `warning` with localization enabled some memory corruption.
* [#14171](http://bugzilla.scilab.org/show_bug.cgi?id=14171): Scinotes Favorite and most recently used directories could no be targeted through the console `File` menu.
* [#14192](http://bugzilla.scilab.org/show_bug.cgi?id=14192): `g_margin` error-ed for double integrator.
* [#14278](http://bugzilla.scilab.org/show_bug.cgi?id=14278): `ltitr` returned an incorrect xf output value.
* [#14306](http://bugzilla.scilab.org/show_bug.cgi?id=14306): `>` and `>=` operators could not be used to compare encoded integers of mismatching inttypes.
* [#14330](http://bugzilla.scilab.org/show_bug.cgi?id=14330): luget was really slow.
* [#14367](http://bugzilla.scilab.org/show_bug.cgi?id=14367): `edit_curv` failed opening due to a `[]+1` operation.
* [#14379](http://bugzilla.scilab.org/show_bug.cgi?id=14379): Problem with lists of functions having 2 arguments.
* [#14395](http://bugzilla.scilab.org/show_bug.cgi?id=14395): `dir` displayed a []+".." warning when no subdirectory exists.
* [#14405](http://bugzilla.scilab.org/show_bug.cgi?id=14405): `xcosPalAdd` did not work on Windows.
* [#14411](http://bugzilla.scilab.org/show_bug.cgi?id=14411): `abort` used in a `while`loop crashed Scilab.
* [#14437](http://bugzilla.scilab.org/show_bug.cgi?id=14437): Changing the field of a struct embedded in a list sometimes misworked.
* [#14448](http://bugzilla.scilab.org/show_bug.cgi?id=14448): `havewindow` is removed but was still documented.
* [#14461](http://bugzilla.scilab.org/show_bug.cgi?id=14461): Calling `grand(n, "markov", P, x0)` did not return all outputs.
* [#14470](http://bugzilla.scilab.org/show_bug.cgi?id=14470): `geomean` often overflowed for easily computable entries, and did not check input arguments.
* [#14483](http://bugzilla.scilab.org/show_bug.cgi?id=14483): The `figure.figure_name` property had no `figure.name` alias
* [#14513](http://bugzilla.scilab.org/show_bug.cgi?id=14513): `isqual` comparing two built-in functions yielded an error.
* [#14527](http://bugzilla.scilab.org/show_bug.cgi?id=14527): Calling `pathconvert` function without parameters crashed Scilab.
* [#14553](http://bugzilla.scilab.org/show_bug.cgi?id=14553): `find(a=b)` crashed Scilab.
* [#14557](http://bugzilla.scilab.org/show_bug.cgi?id=14557): `csim` failed when the system has no state.
* [#14558](http://bugzilla.scilab.org/show_bug.cgi?id=14558): `square` was poor, clumsy and too specific. It is tagged as obsolete.
* [#14564](http://bugzilla.scilab.org/show_bug.cgi?id=14564): `fieldnames` failed for empty structs, tlists & mlists
* [#14571](http://bugzilla.scilab.org/show_bug.cgi?id=14571): The types of input arguments of `figure()` were not checked.
* [#14578](http://bugzilla.scilab.org/show_bug.cgi?id=14578): LaTeX string used for text uicontrol was not updated.
* [#14582](http://bugzilla.scilab.org/show_bug.cgi?id=14582): `gettext` or it alias `_()` were sometimes applied to broken literal strings
* [#14586](http://bugzilla.scilab.org/show_bug.cgi?id=14586): The Stop button of Xcos simulation did not work.
* [#14587](http://bugzilla.scilab.org/show_bug.cgi?id=14587): Datatip textbox wrong clipping when loaded from `*.scg` file.
* [#14590](http://bugzilla.scilab.org/show_bug.cgi?id=14590): Many help pages in `pt_BR` version had a wrong xml:lang="en" tag.
* [#14591](http://bugzilla.scilab.org/show_bug.cgi?id=14591): `<=` and `>=` elementwise operators comparing 2 hypermatrices of decimal numbers or encoded integers were inverted.
* [#14593](http://bugzilla.scilab.org/show_bug.cgi?id=14593): Signs were no longer drawn in BIGSOM and PRODUCT components.
* [#14602](http://bugzilla.scilab.org/show_bug.cgi?id=14602): WRITEC_f block didn't work for x86 machines.
* [#14604](http://bugzilla.scilab.org/show_bug.cgi?id=14604): `emptystr()` was 40x slower than Scilab 5
* [#14609](http://bugzilla.scilab.org/show_bug.cgi?id=14609): "msscanf" crashes Scilab when 'niter' parameter is out of range.
* [#14632](http://bugzilla.scilab.org/show_bug.cgi?id=14632): Zooming moved offscreen any drawn axis
* [#14640](http://bugzilla.scilab.org/show_bug.cgi?id=14640): `median(int8([10 60 80 100]))` returned -58 instead of 70 due to overflow when interpolating (60+80)>128
* [#14645](http://bugzilla.scilab.org/show_bug.cgi?id=14645): Xcos Demos -> Control Systems -> Lorenz Butterfly didn't end at the expected time (30)
* [#14648](http://bugzilla.scilab.org/show_bug.cgi?id=14648): `isinf` returned `%F` for complex numbers with both real and imag infinite parts.
* [#14649](http://bugzilla.scilab.org/show_bug.cgi?id=14649): `isnan(complex(%inf, %inf))` returned `%F` while the phase is `NaN`.
* [#14654](http://bugzilla.scilab.org/show_bug.cgi?id=14654): `bitor`, `bitxor` and `bitand` did not accept positive inputs of type `int8`, `int16`, `int32`, `int64` or `uint64`
* [#14659](http://bugzilla.scilab.org/show_bug.cgi?id=14659): number of I/O ports of the superblock was not updated when adding or deleting I/O blocks inside a superblock.
* [#14662](http://bugzilla.scilab.org/show_bug.cgi?id=14662): Matrix of strings concatenation with single quote led to a parser error.
* [#14664](http://bugzilla.scilab.org/show_bug.cgi?id=14664): Deleted obsolete WFILE_f block. Regenerated some Xcos demos to work with Scilab 6.
* [#14667](http://bugzilla.scilab.org/show_bug.cgi?id=14667): Multi line string without final quote generated a non terminal parser state.
* [#14681](http://bugzilla.scilab.org/show_bug.cgi?id=14681): Short-circuited AND operation was not possible with double matrices in if and while clauses
* [#14689](http://bugzilla.scilab.org/show_bug.cgi?id=14689): `resize_matrix(rand(2,3),[0 2])` did not return `[]`. Usage of new sizes <0 to keep them unchanged was not documented.
* [#14690](http://bugzilla.scilab.org/show_bug.cgi?id=14690): The user startup files set in the working directory were not executed. When `SCIHOME` is not the working directory, `SCIHOME\scilab.ini` was executed twice.
* [#14692](http://bugzilla.scilab.org/show_bug.cgi?id=14692): `isequal` always returned `%T` for builtin functions
* [#14694](http://bugzilla.scilab.org/show_bug.cgi?id=14694): The list of named colors was misaligned and poorly rendered in `help color_list`
* [#14710](http://bugzilla.scilab.org/show_bug.cgi?id=14710): `fullpath(TMPDIR+...)` was bugged on MacOS
* [#14711](http://bugzilla.scilab.org/show_bug.cgi?id=14711): When current axes is an uicontrol frame, `colorbar` did not display anything.
* [#14714](http://bugzilla.scilab.org/show_bug.cgi?id=14714): Deleting a datatip made Scilab leaking or crashed.
* [#14731](http://bugzilla.scilab.org/show_bug.cgi?id=14731): The demos `Graphics => Complex functions` opened an empty figure#0. Rotation of Im+Re parts were not synchronized
* [#14743](http://bugzilla.scilab.org/show_bug.cgi?id=14743): `test_run(.., "show_error")` did not document "failed: Slave Scilab exited with error code #" errors.
* [#14758](http://bugzilla.scilab.org/show_bug.cgi?id=14758): `xstringb` opened a default figure.
* [#14761](http://bugzilla.scilab.org/show_bug.cgi?id=14761): `||` misworked when LHS is %f or zeros. `&&` misworked when LHS is %t or non-zeros
* [#14779](http://bugzilla.scilab.org/show_bug.cgi?id=14779): `xsegs` used in logarithmic scale with coordinates `<= 0` crashed Scilab.
* [#14784](http://bugzilla.scilab.org/show_bug.cgi?id=14784): Setting field of graphics handle using children($) failed.
* [#14796](http://bugzilla.scilab.org/show_bug.cgi?id=14796): `ind2sub([4,2], [])` returned `[4 0]` instead of `[]`.
* [#14775](http://bugzilla.scilab.org/show_bug.cgi?id=14775): Loading an empty (0 bytes) `.sod` file crashed scilab
* [#14801](http://bugzilla.scilab.org/show_bug.cgi?id=14801): The horizontal concatenation of cells arrays wrongly puzzled components.
* [#14808](http://bugzilla.scilab.org/show_bug.cgi?id=14808): After `E=['A' 'B' 'C' 'D' 'E']`, `E(0:0)` crashed Scilab
* [#14821](http://bugzilla.scilab.org/show_bug.cgi?id=14821): `getio` function was missing. An error on the diary file opened has been corrected.
* [#14824](http://bugzilla.scilab.org/show_bug.cgi?id=14824): `mfprintf(fd, "%d", [])` yielded an incorrect error message.
* [#14835](http://bugzilla.scilab.org/show_bug.cgi?id=14835): `AFFICH_m` block was not rendered correctly.
* [#14839](http://bugzilla.scilab.org/show_bug.cgi?id=14839): `plot2d2` crashed Scilab.
* [#14885](http://bugzilla.scilab.org/show_bug.cgi?id=14885): The `tag` property was not documented in the `Matplot_properties` help page.
* [#14887](http://bugzilla.scilab.org/show_bug.cgi?id=14887): For many types of graphic handles, the display of the `.tag` value missed `".."` delimiters
* [#14909](http://bugzilla.scilab.org/show_bug.cgi?id=14909): On Windows, `getlongpathname` and `getshortpathname` did not force the file separator to `"\"`
* [#14911](http://bugzilla.scilab.org/show_bug.cgi?id=14911): The entry "Label => Title" was missing in the graphic context menu on a figure.
* [#14941](http://bugzilla.scilab.org/show_bug.cgi?id=14941): `find` did not accept encoded integers
* [#14942](http://bugzilla.scilab.org/show_bug.cgi?id=14942): Keep the Tkscale block label if block already has label.
* [#14956](http://bugzilla.scilab.org/show_bug.cgi?id=14956): `clf("reset")` forgot resetting the `immediate_drawing`, `resize`, `resizefcn`, `closerequestfcn`, `toolbar_visible`, `menubar_visible`, `infobar_visible`, `default_axes`, and `icon` figure properties.
* [#14965](http://bugzilla.scilab.org/show_bug.cgi?id=14965): `getPreferencesValue` could not read a tag having multiple occurrences and did not accept the path to the preferences file.
* [#14976](http://bugzilla.scilab.org/show_bug.cgi?id=14976): `asciimat(colNum)` concatenated rows when colNum has a single column of ascii codes. With UTF-8 chars, `asciimat(asciimat("àéïôù"))` yielded an error.
* [#14978](http://bugzilla.scilab.org/show_bug.cgi?id=14978): `input(message)` interpreted an entered `x` as a literal string, and exited with the new prompt on the same line.


### Bugs fixed in 6.0.0 beta-2 and earlier 6.0.0 pre-releases:

* [#2104](http://bugzilla.scilab.org/show_bug.cgi?id=2104): `iw(1:9)` and `w(1:10)` `ode` output parameters were not documented
* [#2517](http://bugzilla.scilab.org/show_bug.cgi?id=2517): `"position"` property format was not accepted by `figure` despite what was said in help
* [#6057](http://bugzilla.scilab.org/show_bug.cgi?id=6057): trailing space after minus sign has been removed from the display of values
* [#6064](http://bugzilla.scilab.org/show_bug.cgi?id=6064): `scatter` did not exist in Scilab.
* [#6314](http://bugzilla.scilab.org/show_bug.cgi?id=6314): The identical code of `%p_m_r` and `%r_m_p` was not factorized
* [#7378](http://bugzilla.scilab.org/show_bug.cgi?id=7378): `quart` used with only `NaN`s yielded an error instead of returning `NaN`.
* [#7646](http://bugzilla.scilab.org/show_bug.cgi?id=7646): Extractions `A'(1,2)` and `A.'(1,2)` from a transposed matrix were not possible
* [#7884](http://bugzilla.scilab.org/show_bug.cgi?id=7884): `typeof` help page was poor, puzzled, and not up-to-date to Scilab 6:
  - new typeof `uint64`, `int64`, `void`, `deletelist`, `implicitlist ` were missing
  - former `hypermat` and `size implicit` typeof weren't removed
  - typeof names longer than 8-char were not documented.
* [#8210](http://bugzilla.scilab.org/show_bug.cgi?id=8210): UMFPACK demos were not well packaged and not numerous enough.
* [#8310](http://bugzilla.scilab.org/show_bug.cgi?id=8310): Non-convex plane or unplane polygons could be wrongly triangulated and badly rendered with extra facets.
* [#8990](http://bugzilla.scilab.org/show_bug.cgi?id=8990): `Reframe to contents` feature was missing on the graphics toolbar and `Tools` menu.
* [#9456](http://bugzilla.scilab.org/show_bug.cgi?id=9456): `bench_run` did not work on a path or in a toolbox
* [#9560](http://bugzilla.scilab.org/show_bug.cgi?id=9560): `1./M` was parsed as `1. / M` instead of `1 ./ M`
* [#9621](http://bugzilla.scilab.org/show_bug.cgi?id=9621): A `tlist` with undefined fields can now be saved.
* [#10082](http://bugzilla.scilab.org/show_bug.cgi?id=10082): `string(complex)` with `real(complex)>0` did not remove the leading space replacing `"+"`
* [#11511](http://bugzilla.scilab.org/show_bug.cgi?id=11511): `error` did not accept string matrix (non regression test added).
* [#11625](http://bugzilla.scilab.org/show_bug.cgi?id=11625): Uicontrol table did not update `.string` when values were modified interactively in the table.
* [#12044](http://bugzilla.scilab.org/show_bug.cgi?id=12044): Adding or substracting the empty matrix now return an empty matrix.
* [#12202](http://bugzilla.scilab.org/show_bug.cgi?id=12202): Mixing int8 and doubles with colon operator led to wrong results.
* [#12419](http://bugzilla.scilab.org/show_bug.cgi?id=12419): objects were cleared before the `scilab.quit` was called
* [#12559](http://bugzilla.scilab.org/show_bug.cgi?id=12559): FFTW had some memory leaks
* [#12872](http://bugzilla.scilab.org/show_bug.cgi?id=12872): Help pages of `else`, `elseif`, `end`, `try`, `sciargs`, global, halt, empty and power were in wrong help sections
* [#12928](http://bugzilla.scilab.org/show_bug.cgi?id=12928): `intXX` functions with `%nan` and `%inf` return wrong values.
* [#13154](http://bugzilla.scilab.org/show_bug.cgi?id=13154): In shellmode, completion now separates Files from Directories.
* [#13289](http://bugzilla.scilab.org/show_bug.cgi?id=13289): Using non-integer indexes for mlists made Scilab crash.
* [#13298](http://bugzilla.scilab.org/show_bug.cgi?id=13298): Static analysis bugs detected by PVS-Studio fixed.
* [#13308](http://bugzilla.scilab.org/show_bug.cgi?id=13308): Xcos had no Crank-Nicolson solver.
* [#13465](http://bugzilla.scilab.org/show_bug.cgi?id=13465): The display of polyline `.display_function` and `.display_function` properties was not conventional
* [#13468](http://bugzilla.scilab.org/show_bug.cgi?id=13468): Scilab hanged when incorrect format was used for file reading using `mfscanf`.
* [#13490](http://bugzilla.scilab.org/show_bug.cgi?id=13490): `histc` help page fixed to match the macro (by default, normalize the result).
* [#13517](http://bugzilla.scilab.org/show_bug.cgi?id=13517): `isdef` crashed Scilab when called with a vector of strings as input in a function and after a declaration of variable.
* [#13709](http://bugzilla.scilab.org/show_bug.cgi?id=13709): `unique` sometimes returned wrong index values.
* [#13725](http://bugzilla.scilab.org/show_bug.cgi?id=13725): Sometimes `xfpoly` polygon filling failed.
* [#13748](http://bugzilla.scilab.org/show_bug.cgi?id=13748): `printf`, `sprintf` (en,ja): short descriptions and obsolete flags were missing.
* [#13750](http://bugzilla.scilab.org/show_bug.cgi?id=13750): Calling `ss2ss` with `flag = 2` returned an error.
* [#13751](http://bugzilla.scilab.org/show_bug.cgi?id=13751): `lqg2stan` returned wrong (inverted) values.
* [#13769](http://bugzilla.scilab.org/show_bug.cgi?id=13769): `t = "abc..//ghi"` was parsed as a continued + comment
* [#13780](http://bugzilla.scilab.org/show_bug.cgi?id=13780): `size` with two input and output arguments did not return an error.
* [#13795](http://bugzilla.scilab.org/show_bug.cgi?id=13795): `grep` with regexp option did not match the empty string properly
* [#13807](http://bugzilla.scilab.org/show_bug.cgi?id=13807): Invalid margins were computed when figure was not visible.
* [#13810](http://bugzilla.scilab.org/show_bug.cgi?id=13810): `householder(v, k*v)` returned column of `Nan`. Input parameters were not checked. The Householder matrix could not be returned. Help pages were inaccurate and without examples. There was no `householder` demo.
* [#13816](http://bugzilla.scilab.org/show_bug.cgi?id=13816): `show_margins` caused a scilab crash
* [#13829](http://bugzilla.scilab.org/show_bug.cgi?id=13829): `mean` and `sum` returned wrong results for hypermatrices.
* [#13831](http://bugzilla.scilab.org/show_bug.cgi?id=13831): `ss2ss` did not update the initial state
* [#13834](http://bugzilla.scilab.org/show_bug.cgi?id=13834): Drawing a high number of strings in a figure generated a Java exception.
* [#13838](http://bugzilla.scilab.org/show_bug.cgi?id=13838): Sparse and complex substraction made Scilab crash.
* [#13839](http://bugzilla.scilab.org/show_bug.cgi?id=13839): `sign` could not be used with sparse matrices
* [#13843](http://bugzilla.scilab.org/show_bug.cgi?id=13843): Scilab crashed when `polarplot` and `plot2d` were called with wrong `strf` value.
* [#13853](http://bugzilla.scilab.org/show_bug.cgi?id=13853): `plzr` returned wrong results for discrete-time systems with a numeric time step.
* [#13854](http://bugzilla.scilab.org/show_bug.cgi?id=13854): Under some operating systems, SciNotes did not initialize a new document at startup.
* [#13862](http://bugzilla.scilab.org/show_bug.cgi?id=13862): There was no lazy evaluation of `or` operands in `if` tests.
* [#13864](http://bugzilla.scilab.org/show_bug.cgi?id=13864): `%l_isequal` was useless in Scilab 6.
* [#13866](http://bugzilla.scilab.org/show_bug.cgi?id=13866): There were some issues with FFTW3 library.
* [#13869](http://bugzilla.scilab.org/show_bug.cgi?id=13869): `bench_run` with option `nb_run=10` did not override the NB RUN tags
* [#13872](http://bugzilla.scilab.org/show_bug.cgi?id=13872): Non regression test added for `unique` (the indices returned were wrong)
* [#13873](http://bugzilla.scilab.org/show_bug.cgi?id=13873): `%hm_stdev(H,idim>2)` returned `zeros(H)`
* [#13881](http://bugzilla.scilab.org/show_bug.cgi?id=13881): `datatipRemoveAll` did not work.
* [#13890](http://bugzilla.scilab.org/show_bug.cgi?id=13890): `getd` did not return loaded symbols in previous scope.
* [#13893](http://bugzilla.scilab.org/show_bug.cgi?id=13893): `simp` did not set a rational denominator at `1` when numerator was equal to zero.
* [#13894](http://bugzilla.scilab.org/show_bug.cgi?id=13894): Default working directory of the previous session did not work.
* [#13897](http://bugzilla.scilab.org/show_bug.cgi?id=13897): Concatenating structures with same fields in mismatching orders failed
* [#13899](http://bugzilla.scilab.org/show_bug.cgi?id=13899): Syntax coloring was off in `scinotes`
* [#13903](http://bugzilla.scilab.org/show_bug.cgi?id=13903): `get_function_path` returned a path with a missing file separator.
* [#13907](http://bugzilla.scilab.org/show_bug.cgi?id=13907): Avoids the gray background on the right panel of the palette Browser.
* [#13908](http://bugzilla.scilab.org/show_bug.cgi?id=13908): `part(text, n:$)` was very slow.
* [#13918](http://bugzilla.scilab.org/show_bug.cgi?id=13918): Unmanaged operations on hypermatrix did not call overload functions.
* [#13919](http://bugzilla.scilab.org/show_bug.cgi?id=13919): Scilab parsed `hidden` as a reserved keyword but it is not used.
* [#13920](http://bugzilla.scilab.org/show_bug.cgi?id=13920): `getscilabkeywords` help page should be in the "Scilab keywords" section.
* [#13923](http://bugzilla.scilab.org/show_bug.cgi?id=13923): Changes of `typeof(:)` and `typeof(n:$)` were not documented.
* [#13924](http://bugzilla.scilab.org/show_bug.cgi?id=13924): rationals `r1==r2` and `r1~=r2` might sometimes be wrong.
* [#13925](http://bugzilla.scilab.org/show_bug.cgi?id=13925): SciNotes used the wrong paired bracket highlight style.
* [#13931](http://bugzilla.scilab.org/show_bug.cgi?id=13931): handle `aarch64` processor for some Linux distribution.
* [#13939](http://bugzilla.scilab.org/show_bug.cgi?id=13939): In HTML help pages, itemizedlist `<ul>` were rendered as numbered ones
* [#13941](http://bugzilla.scilab.org/show_bug.cgi?id=13941): Internal timestamps of HDF5 files prevented having a fixed hash for an unvarying set of saved objects.
* [#13942](http://bugzilla.scilab.org/show_bug.cgi?id=13942): the palette browser tree was always resized when expanded/collapsed.
* [#13944](http://bugzilla.scilab.org/show_bug.cgi?id=13944): The menu "Toolboxes" was missing.
* [#13965](http://bugzilla.scilab.org/show_bug.cgi?id=13965): The rendering of histograms with `histplot` was poor
* [#13966](http://bugzilla.scilab.org/show_bug.cgi?id=13966): `twinkle` and `twinkle(n)` were not supported
* [#13971](http://bugzilla.scilab.org/show_bug.cgi?id=13971): A space has been added between Scilab prompt and cursor.
* [#13972](http://bugzilla.scilab.org/show_bug.cgi?id=13972): Wildcard `*` was not managed in `printf` expressions.
* [#13974](http://bugzilla.scilab.org/show_bug.cgi?id=13974): `isoview(xmin, xmax, ymin, ymax)` was unhandy.
* [#13983](http://bugzilla.scilab.org/show_bug.cgi?id=13983): `who_user` returned wrong values.
* [#13986](http://bugzilla.scilab.org/show_bug.cgi?id=13986): `setdefaultlanguage` did not set value correctly in Windows registry.
* [#13990](http://bugzilla.scilab.org/show_bug.cgi?id=13990): `gettext` did not manage the added `_W` macro.
* [#13999](http://bugzilla.scilab.org/show_bug.cgi?id=13999): `editor` was modal. It locked the console using an external editor.
* [#14012](http://bugzilla.scilab.org/show_bug.cgi?id=14012): Function `stripblanks` did not allow to remove only leading spaces of a set of strings, or only trailing one. An option to do so was added.
* [#14020](http://bugzilla.scilab.org/show_bug.cgi?id=14020): Incorrect carriage return ascii code.
* [#14022](http://bugzilla.scilab.org/show_bug.cgi?id=14022): `getscilabkeywords` was KO (+gateway what() added).
* [#14023](http://bugzilla.scilab.org/show_bug.cgi?id=14023): It was not possible to concatenate cells.
* [#14024](http://bugzilla.scilab.org/show_bug.cgi?id=14024): Print of `macrofile` display a debug message instead of macro prototype.
* [#14025](http://bugzilla.scilab.org/show_bug.cgi?id=14025): `head_comments` did not take into account compiled functions.
* [#14028](http://bugzilla.scilab.org/show_bug.cgi?id=14028): force flag of `genlib` did not rebuild bin file.
* [#14030](http://bugzilla.scilab.org/show_bug.cgi?id=14030): Linear algebra demo crashed due to a bad delete in `schur` implementation.
* [#14035](http://bugzilla.scilab.org/show_bug.cgi?id=14035): `ndgrid` did not manage all homogeneous data type (booleans, integers, polynomials, rationals, strings, `[]`)
* [#14036](http://bugzilla.scilab.org/show_bug.cgi?id=14036): `.tag` and `.user_data` properties were not displayed and not documented for light entity.
* [#14038](http://bugzilla.scilab.org/show_bug.cgi?id=14038): Encoded integers were no longer accepted for list extraction.
* [#14040](http://bugzilla.scilab.org/show_bug.cgi?id=14040): graphic property setting fails when using array of handles
* [#14041](http://bugzilla.scilab.org/show_bug.cgi?id=14041): `genlib` crashed when the file is locked by another program.
* [#14043](http://bugzilla.scilab.org/show_bug.cgi?id=14043): Examples of API Scilab help pages had to be updated (`pvApiCtx` in gateway prototypes).
* [#14044](http://bugzilla.scilab.org/show_bug.cgi?id=14044): `MALLOC.h` is now renamed to `sci_malloc.h`.
* [#14047](http://bugzilla.scilab.org/show_bug.cgi?id=14047): wrong behaviour of `break` ( `continue` ) in `if` and outside of loop fixed.
* [#14049](http://bugzilla.scilab.org/show_bug.cgi?id=14049): `genlib` hang if an unexpected `endfunction` occurs.
* [#14055](http://bugzilla.scilab.org/show_bug.cgi?id=14055): overload on matrix concatenation was not called with `[]`.
* [#14057](http://bugzilla.scilab.org/show_bug.cgi?id=14057): `grand(m,n)` returned a wrong error and `grand(m,n,p)` called an overloading function instead of returning an error.
* [#14058](http://bugzilla.scilab.org/show_bug.cgi?id=14058): Scilab crashed with `file("close", file())` instruction
* [#14059](http://bugzilla.scilab.org/show_bug.cgi?id=14059): Lack of performance on deletion of matrix elements.
* [#14065](http://bugzilla.scilab.org/show_bug.cgi?id=14065): Change "java size" in points in graphics font help page.
* [#14067](http://bugzilla.scilab.org/show_bug.cgi?id=14067): 3rd argument of `fsolve` became mandatory
* [#14082](http://bugzilla.scilab.org/show_bug.cgi?id=14082): `m=1; m()=1;` made Scilab crash.
* [#14093](http://bugzilla.scilab.org/show_bug.cgi?id=14093): `atanh` returns NaN for values with an absolute value greater than 1
* [#14095](http://bugzilla.scilab.org/show_bug.cgi?id=14095): Scilab crashed when a .fig file was loaded with `loadmatfile` function.
* [#14096](http://bugzilla.scilab.org/show_bug.cgi?id=14096): Issue with `mscanf`.
* [#14097](http://bugzilla.scilab.org/show_bug.cgi?id=14097): `genlib` no more adds a separator at the end of the lib path if it is not given in the directory path.
* [#14099](http://bugzilla.scilab.org/show_bug.cgi?id=14099): `sci2exp` macro was fixed to avoid "a+[] Warning". string(polynomials|rationals) had badly formatted outputs and was not vectorized
* [#14105](http://bugzilla.scilab.org/show_bug.cgi?id=14105): New block comments `/*...*/` feature was not documented.
* [#14107](http://bugzilla.scilab.org/show_bug.cgi?id=14107): `lstcat` of a string and a list did not produce consistent results.
* [#14109](http://bugzilla.scilab.org/show_bug.cgi?id=14109): `lsq` crashed Scilab when Scilab version depended on MKL library.
* [#14111](http://bugzilla.scilab.org/show_bug.cgi?id=14111): In Scilab 6, `lib` loading a Scilab 5 library did not give a proper error message.
* [#14113](http://bugzilla.scilab.org/show_bug.cgi?id=14113): Scilab 6 did not detect infinite loop.
* [#14115](http://bugzilla.scilab.org/show_bug.cgi?id=14115): In Scinotes, the `switch` and `otherwise` keywords were no longer colorized.
* [#14116](http://bugzilla.scilab.org/show_bug.cgi?id=14116): Invalid exponent in case of complex exponents especially `0*%i`.
* [#14118](http://bugzilla.scilab.org/show_bug.cgi?id=14118): `real`, `imag`, `conj`, `isreal` did not accept rationals
* [#14135](http://bugzilla.scilab.org/show_bug.cgi?id=14135): crash when running "Graphics -> Matplot -> Java Image" demonstration.
* [#14141](http://bugzilla.scilab.org/show_bug.cgi?id=14141): `gcf().attribute=value` lead to "Wrong insertion: function or macro are not expected".
* [#14144](http://bugzilla.scilab.org/show_bug.cgi?id=14144): Scilab crashed with `int64(2^63)`.
* [#14149](http://bugzilla.scilab.org/show_bug.cgi?id=14149): HDF5 could not restore hypermatrix with good dimensions.
* [#14150](http://bugzilla.scilab.org/show_bug.cgi?id=14150): The Windows SDK was not found on Windows 8.1.
* [#14156](http://bugzilla.scilab.org/show_bug.cgi?id=14156): `mfscanf` returned an empty matrix when datafile contained a header.
* [#14157](http://bugzilla.scilab.org/show_bug.cgi?id=14157): Insert of big set in sparse was very slow.
* [#14159](http://bugzilla.scilab.org/show_bug.cgi?id=14159): `Matplot` crashed Scilab on boolean input.
* [#14178](http://bugzilla.scilab.org/show_bug.cgi?id=14178): Tcl/Tk unavailability on MacOS is now documented.
* [#14181](http://bugzilla.scilab.org/show_bug.cgi?id=14181): `intg` (or `integrate`) in a function that is being integrated failed.
* [#14187](http://bugzilla.scilab.org/show_bug.cgi?id=14187): `fscanfMat` failed to read format `%d`, `%i` and `%f`.
* [#14199](http://bugzilla.scilab.org/show_bug.cgi?id=14199): `sqrt` returned wrong dimension results on matrix with more than dimensions.
* [#14203](http://bugzilla.scilab.org/show_bug.cgi?id=14203): Improve some error messages, some help pages and some comments.
* [#14204](http://bugzilla.scilab.org/show_bug.cgi?id=14204): `dec2bin` ( `dec2base` ) must show a better error message for too large values.
* [#14205](http://bugzilla.scilab.org/show_bug.cgi?id=14205): Console crash when assigning uint32 numbers to double matrix.
* [#14209](http://bugzilla.scilab.org/show_bug.cgi?id=14209): `1:245` created infinite loop.
* [#14212](http://bugzilla.scilab.org/show_bug.cgi?id=14212): Scilab 6 did not load array of struct from Scilab 5.5 files correctly
* [#14219](http://bugzilla.scilab.org/show_bug.cgi?id=14219): As [bug #14203](http://bugzilla.scilab.org/show_bug.cgi?id=14203), improve some error messages, some help pages and some comments.
* [#14223](http://bugzilla.scilab.org/show_bug.cgi?id=14223): `det` returned an error when it is used with a singular matrix.
* [#14225](http://bugzilla.scilab.org/show_bug.cgi?id=14225): command-line option "-quit" should set the processs exit status
* [#14228](http://bugzilla.scilab.org/show_bug.cgi?id=14228): Setting `.rotation_angles` property to a matrix of any size did not return error message.
* [#14232](http://bugzilla.scilab.org/show_bug.cgi?id=14232): Typos fixed in Xcos.
* [#14245](http://bugzilla.scilab.org/show_bug.cgi?id=14245): Problem in recursive extraction using list with `struct`.
* [#14247](http://bugzilla.scilab.org/show_bug.cgi?id=14247): `sqrt` did not work on hypermatrices (non regression test added).
* [#14249](http://bugzilla.scilab.org/show_bug.cgi?id=14249): `Ctrl-C` can be used to stop writing control expression.
* [#14251](http://bugzilla.scilab.org/show_bug.cgi?id=14251): `spec` leaked some memory.
* [#14253](http://bugzilla.scilab.org/show_bug.cgi?id=14253): Insertion in a struct contained in a list fixed.
* [#14255](http://bugzilla.scilab.org/show_bug.cgi?id=14255): `fftw` without MKL leaked.
* [#14271](http://bugzilla.scilab.org/show_bug.cgi?id=14271): `conjgrad` displayed an incorrect error message about number of arguments.
* [#14297](http://bugzilla.scilab.org/show_bug.cgi?id=14297): `cumsum`'s output was badly documented.
* [#14300](http://bugzilla.scilab.org/show_bug.cgi?id=14300): Crash when playing with structures.
* [#14303](http://bugzilla.scilab.org/show_bug.cgi?id=14303): matrix display for large number was not correctly aligned
* [#14304](http://bugzilla.scilab.org/show_bug.cgi?id=14304): `find(x, nmax)` returned `[]` (non regression test added).
* [#14313](http://bugzilla.scilab.org/show_bug.cgi?id=14313): Parser did not create a column separator after spaces and `...` at the end of lines
* [#14316](http://bugzilla.scilab.org/show_bug.cgi?id=14316): Operation `scalar^matrix` was identical to `scalar.^matrix` instead of being `expm( log(scalar) * matrix )`
* [#14326](http://bugzilla.scilab.org/show_bug.cgi?id=14326): It was no longer possible to delete any part of a structure array with `[]`.
* [#14331](http://bugzilla.scilab.org/show_bug.cgi?id=14331): The third argument of `lsq` crashed Scilab.
* [#14347](http://bugzilla.scilab.org/show_bug.cgi?id=14347): `plot2d` crashed with multiple entries (non regression test added).
* [#14348](http://bugzilla.scilab.org/show_bug.cgi?id=14348): Scilab did not open sce/sci file from Windows explorer.
* [#14359](http://bugzilla.scilab.org/show_bug.cgi?id=14359): Black Hole demo updated. Stop and Clear buttons did not have priority tag set. `Callback_type` property has been added and set to `10`.
* [#14361](http://bugzilla.scilab.org/show_bug.cgi?id=14361): Parser did not manage -linebreak + blockcomment `... /* a comment */`
* [#14362](http://bugzilla.scilab.org/show_bug.cgi?id=14362): The `ode` Lotka demo had typo errors
* [#14366](http://bugzilla.scilab.org/show_bug.cgi?id=14366): `typeof(var,"overload")` was not documented
* [#14374](http://bugzilla.scilab.org/show_bug.cgi?id=14374): The parser did not manage comments properly in shellmode
* [#14375](http://bugzilla.scilab.org/show_bug.cgi?id=14375): Calling `input` with a argument of 64 characters or more crashed Scilab.
* [#14389](http://bugzilla.scilab.org/show_bug.cgi?id=14389): The new `int64` and `uint64` were not documented, and other help pages were not updated for them.
* [#14390](http://bugzilla.scilab.org/show_bug.cgi?id=14390): `double` help page had irrelevant syntaxes and was poor
* [#14396](http://bugzilla.scilab.org/show_bug.cgi?id=14396): Real number display was not proper for very wide decimal parts
* [#14398](http://bugzilla.scilab.org/show_bug.cgi?id=14398): Matrix extraction was mistakenly considered a function call in calling sequence
* [#14405](http://bugzilla.scilab.org/show_bug.cgi?id=14405): GetScilabVariableJNI symbol was not resolved on xcosPalAdd()
* [#14415](http://bugzilla.scilab.org/show_bug.cgi?id=14415): Some spelling errors were detected in ~20 help pages
* [#14416](http://bugzilla.scilab.org/show_bug.cgi?id=14416): The file extension filter in Scinotes "Save as" window did not re-used the active file's extension when applicable.
* [#14418](http://bugzilla.scilab.org/show_bug.cgi?id=14418): `saxon9he.jar` made scilab throw an XPathFactoryConfigurationException.
* [#14419](http://bugzilla.scilab.org/show_bug.cgi?id=14419): Scinotes's highlighting of the new `||` and `&&` operators was wrong.
* [#14423](http://bugzilla.scilab.org/show_bug.cgi?id=14423): `bench_run did` not have a return value, export file was not configurable
* [#14425](http://bugzilla.scilab.org/show_bug.cgi?id=14425): `xpause` was a duplicate of `sleep`. `sleep` did not propose "s" time unit.
* [#14429](http://bugzilla.scilab.org/show_bug.cgi?id=14429): Rationals `r+(-r)` and `r-r` did not simplify the denominator to 1 in `simp_mode(%t)`
* [#14432](http://bugzilla.scilab.org/show_bug.cgi?id=14432): Using an implicit list as scoped assignation to a variable in function call caused scilab to crash
* [#14433](http://bugzilla.scilab.org/show_bug.cgi?id=14433): `acoth` (which call `atanh`) crashed scilab
* [#14434](http://bugzilla.scilab.org/show_bug.cgi?id=14434): `PlotSparse` did not work anymore.
* [#14446](http://bugzilla.scilab.org/show_bug.cgi?id=14446): An error message generated by `save(..)` pointed a bad argument index.
* [#14450](http://bugzilla.scilab.org/show_bug.cgi?id=14450): `builder_help.sce` of a toolbox ignored some existing language directories
* [#14453](http://bugzilla.scilab.org/show_bug.cgi?id=14453): `strcat([])` returned an empty string instead of `[]`.
* [#14455](http://bugzilla.scilab.org/show_bug.cgi?id=14455): `macr2tree` crashed when passing a FieldExp.
* [#14468](http://bugzilla.scilab.org/show_bug.cgi?id=14468): Scinotes was unable to export to HTML.
* [#14471](http://bugzilla.scilab.org/show_bug.cgi?id=14471): `strange([])` returned `[]` instead of `Nan` as all other functions for statistical dispersion
* [#14476](http://bugzilla.scilab.org/show_bug.cgi?id=14476): Operation `.*` between polynomials and imaginary numbers was always returning `0`
* [#14493](http://bugzilla.scilab.org/show_bug.cgi?id=14493): `and` and `or` help pages were poor and inaccurate.
* [#14495](http://bugzilla.scilab.org/show_bug.cgi?id=14495): `consolebox` help page shew wrong syntaxes and was poor.
* [#14499](http://bugzilla.scilab.org/show_bug.cgi?id=14499): `getd` did not update already defined functions
* [#14500](http://bugzilla.scilab.org/show_bug.cgi?id=14500): Operator `.^` was broken for sparse matrices.
* [#14517](http://bugzilla.scilab.org/show_bug.cgi?id=14517): The second argument of part function accepted an index of 0 without exiting in error.
* [#14524](http://bugzilla.scilab.org/show_bug.cgi?id=14524): Numeric locales were not set to standard "C" by default at scilab startup
* [#14540](http://bugzilla.scilab.org/show_bug.cgi?id=14540): Datatips did not clip outside axes bounds
* [#14685](http://bugzilla.scilab.org/show_bug.cgi?id=14685): datavec produced an invalid index error.
* [#14980](http://bugzilla.scilab.org/show_bug.cgi?id=14980): The datatip display of the root locus arcs is broken.
* [#14992](http://bugzilla.scilab.org/show_bug.cgi?id=14992): `readgateway` has been removed, use `whereis` instead.
