Welcome to Scilab 6.0.0 beta-2
==============================

This file details the changes between Scilab 6.0.0-beta-2, this release, and the previous release 5.5.2. For changelogs with earlier release, please see [Scilab 5.5.2](https://www.scilab.org/en/content/download/3332/24658/file/Scilab5.5.2_ReleaseNotes.pdf).

This file is intended for the specific needs of advanced users, and describes:
- New and modified features, in each module,
- Changes in functions (removed/added/modified),
- Changes in the language,
- Bug fixes.

This changelog is most likely incomplete, as an enormous amount of code has changed between 5.5.2 and 6. Please report any thing we could have missed, on the [mailing lists][1] or on the [bug tracker][2], and we will correct it before the final Scilab 6.0.0 release.

[1]: http://mailinglists.scilab.org
[2]: http://bugzilla.scilab.org


Main new features
-----------------

For high-level description of the main new features of this release, please consult the [embedded help](modules/helptools/data/pages/homepage-en_US.html). It is also available as the "What's new" page of the help, by simply typing `help` in Scilab console.

In summary, the main new features are:
* New language parser and interpreter, ensuring:
  - Support for bigger data sets: dynamic memory usage. No need for `stacksize` anymore.
  - Xcos also uses the new re-written Scilab engine.
* New code productivity features: full-featured debugger, profiler / coverage tool, and "lint"-like commands.
* Newsfeed, providing a continuous feed of news, tips, and general communication from the community and from Scilab Enterprises.
* Licensing change: Scilab is now released under the terms of the GNU General Public License (GPL) v2.0 (but continues to be available under the terms of the CeCILL v2.1).


Installation
------------

* Under Windows, MKL packages are now included in Scilab installer and are not more downloaded anymore while installing Scilab.


Compilation
-----------

* A C++11 compliant compiler is now needed.
* Java 8 is now required to build Java code (version switch to 1.8).
* Ant minimal version switched to 1.9.0 (for Java 8 compatibility).
* ecj minimal version switched to 4.4.0 (for Java 8 compatibility).
* `--without-xcos` now only disable Xcos compilation. Xcos graphical interface is disabled using `--without-gui`.


Dependencies
------------

* Apache xmlgraphics stack upgraded to the latest versions :
  - xmlgraphics-commons 2.0.1
  - Batik 1.8
  - FOP 2.0


Packaging & Supported Operating Systems
---------------------------------------

* Scilab embedded JVM has been upgraded to Java 1.8. To run or compile Scilab 6.0.0 you need at least:
  - Windows:
     - Windows 8 (Desktop)
     - Windows 7
     - Windows Vista SP2
     - Windows Server 2008 R2 SP1 (64-bit)
     - Windows Server 2012 (64-bit)
  - Mac OS X:
     - Intel-based Mac running Mac OS X 10.8.3+, 10.9+
  - Linux:
     - Red Hat Enterprise Linux 5.5+, 6.x (32-bit), 6.x (64-bit), 7.x (64-bit)
     - Oracle Linux 5.5+, 6.x (32-bit), 6.x (64-bit), 7.x (64-bit)
     - Ubuntu Linux 12.04 LTS, 13.x
     - Suse Linux Enterprise Server 10 SP2+, 11.x

    For more information, please consult: [What are the system requirements for Java?](http://java.com/en/download/help/sysreq.xml)

* [SSE2](https://en.wikipedia.org/wiki/SSE2), Streaming SIMD Extensions 2 support is now mandatory to run Scilab on Linux i686.


Syntax Modifications
--------------------

* Multiline blocs of comments `/* ...\n ... \n ... */` are now possible.
* Declaration of a number on two lines is no longer allowed: `1.. \n 2`
* `1./M` is now parsed as `1 ./ M` instead of `1. / M`
* `{}` and `[]` are now distinct operators. `{}` are now used to build cells.
* Declaring non-homogenous strings ("string' or 'string") is no longer allowed.
* New shortcut operators have been introduced: && and ||.
* Syntax `%i:10` is now deprecated (only real scalars can be used).
* `(a=b)` executed like `a == b` is now deprecated and returns an error.
* `while ... else ... end` control instruction is no more supported.
* Transposed matrix can now use the extraction operator directly `A'(1, 2)`
* Function without output argument cannot be call in assignation expression
	```
	function foo()
		//...
	endfunction

	val = foo() //return error
	```


Feature changes and additions
-----------------------------

* Addition or subtraction with an empty matrix now return an empty matrix.
* scatter/scatter3 plot with different mark colors is now available.
* parulacolormap is now available.
* name2rgb can now handle a single string and a matrix of strings.
* `isoview`, `isoview on`, `isoview off`, `isoview(idGraphics, "on"|"off")` are now supported
* `twinkle` and `twinkle(n)` are now supported: by default, the current element `gce` blinks.
* `householder` can now return the reflection matrix, and has a demo.
* `ndgrid` can now works with any types of homogeneous data
* `bench_run` can now return its results and/or record them in a file
* `typeof(.., "overload")` allows now to get the overloading type-code of an object
* `sign` can now handle a sparse matrix.
* `sleep(..,'s')` allows now to specify the duration in seconds.
* `real`, `imag`, `conj` and `isreal` now accept rational fractions
* A call stack limit has been introduced. Default maximum depth is setup to 1000 and can be changed by `recursionlimit` or through the Preferences interface.
* The floating point exception mode `ieee` is now set to `2` by default: floating point exceptions now produce `Inf` or `Nan`, and do not cause any error. The previous behavior can be recalled by simply calling: `ieee(0)`
* The graphics entity "Datatip" has its property `z_component = 'on|off'` changed to `display_components = 'xyz'`, now it is possible to choose which components to display and the order. Loading old `*.scg` files containing datatips with "z_component" property may not display the z component.
* Valgrind error detection added to `test_run` (on Linux only)
* `amell` now
  - checks if its parameters are real numbers,
  - throws an error if the second parameter is not a scalar.
* The use of I/O console is now allowed with the following functions: `mget`, `mgetl`, `mgetstr`, `mput`, `mputl` and `mputstr`.
* `mclearerr` now returns a flag indicating the file identifier validity.
* `fileinfo` can now take a row vector as input.
* `msprintf` no more returns an error message when there are too many input arguments (more values that format needs).
* `deletefile` can delete multiple files at once.
* `exec` of macro executes the body in the current scope but the prototype must have zero input and output arguments.
* `error`: an error number in input is deprecated.
* `impl`: Recall impl with the same parameters as in its previous stop is now available.
* `ode`: `y0` is restricted to a column vector.
* `pppdiv`: Return a scalar of type 'constant' when the rank is 0.
* `pdiv`: Return a matrix of type 'constant' when all the rank are 0.
* `test_run` can now take "[]" as argument to be used on console; for instance: test_run string [] no_check_ref
* `typeof(:)` and `typeof(n:$)` now return `"implicitlist"` instead of respectively `"constant"` and `"size implicit"`.
* `strange([])` now returns `%nan` instead of `[]`, as all other functions for statistical dispersion
* `stdev(x, dir>ndims(x))` now yields an error instead of returning `zeros(x)`
* `write`: Writing string or string matrix in a file does not add blank space before each value
* The zoom rubber box now can start/finish from points lying outside the axes bounds.
* The zoom rubber box now can select multiple and overlying axes at once. Changed behavior of scroll zoom, scrolling over overlying axes will zoom all of them together, using the CTRL key while scrolling will zoom all axes in the current figure.


Help pages:
-----------

* fixed / improved:  `members`, `part`, `ode`, `ode_optional_output`, `ode_roots`, `roots`,
  `printf`, `sprintf`, `iconvert`, `stdev`, `xlabel`, `and_op`, `or_op`
* rewritten: `consolebox`, `double`, `isoview`, `householder`, `or`, `and`, `format`, `typeof`, `brackets`, `setlanguage`, `sleep`, `isinf`
* reorganized:
  - `else`, `elseif`, `end`, `try`, `sciargs`, `global`, `halt`, `empty`, `power`
  - CACSD and Signal Processing help pages have been sorted up.
  - Colormaps and GUI/Menus subsections created
* translations added:
  - (fr): `format`, `typeof`, `isoview`, `ndgrid`, `bench_run`, `consolebox`, `harmean`, `sleep`
  - (ru): homepage


Data Structures
---------------

* cells and structs are now native types, hence improving performances.
* cells:
  - insertion and extraction must be done via `()` or `{}.`
  - `.dims` and `.entries` fields have been removed, please use `size` and `()` instead.
* struct
  - `.dims` field has been removed, please use size instead.
* hypermatrix:
  - hypermatrices are natively managed (without `mlist` overloading).
  - typeof function now returns real type like `constant`, `string`, ... instead of `hypermat`
  - type function returns real type like `1, 10, ...` instead of `17` (`mlist`).
  - `.dims` and `.entries` fields have been removed, please use `size` and `()` instead.


Xcos
----

* Major rewrite of the data structures, huge models should load and save faster. The memory usage on diagram edition is also slightly reduced.
* Implicit fixed-size step ODE solver added: Crank-Nicolson 2(3). Added to the CVode package, it also benefits from the CVode rootfinding feature.


API modification
----------------

A new set of C APIs to write C or C++ extensions (toolboxes) to Scilab. It allows defining native functions (commonly called "gateways"), getting input parameters for such functions, setting return parameters, accessing local variables, using common helper functions for accessing environment information (such as warning level), generate errors... It also includes ways to overload existing Scilab functions to support additional parameter types (see `help scilab_overload`). Finally, you can call back Scilab functions (macros and built-in functions) from your gateway (see `help scilab_call`).

User-defined functions written in C or C++ (gateways) must now use a `void* pvApiCtx` name as a second parameter instead of any `unsigned long l`. This is now required for some macros, such as `Rhs`, to work.

For example: use `int sci_Levkov(char *fname, void* pvApiCtx)` instead of `int sci_Levkov(char *fname)` or `int sci_Levkov(char *fname, unsigned long l)`.


Obsolete functions or features
------------------------------

* `maxfiles` is now obsolete.
* `isoview(xmin,xmax,ymin,ymax)` is deprecated. Please use `isoview("on"), replot(..)` instead.
* `eval3d` will be removed from Scilab 6.1. Please use `ndgrid` instead.
* `strcmpi` is deprecated. Please use `strcmp(..,"i")`instead.


Removed Functions
-----------------

* `intersci` has been removed. Please use [swig](http://swig.org/) instead.
* `numdiff` has been removed. Please use `numderivative` instead.
* `derivative` has been removed. Please use `numderivative` instead.
* `curblockc` has been removed. Please use `curblock` instead.
* `extract_help_examples` has been removed.
* `xpause` has been removed. Please use `sleep` instead.
* `xclear` has been removed. Please use `clf` instead.
* `fcontour2d` has been removed. Please use `contour2d` instead.
* `plot2d1` has been removed. Please use `plot2d` instead.
* `lex_sort` has been removed. Please use `gsort(..,"lr")` instead.
* `gspec` was obsolete already in Scilab 4 and is now removed. Please use `spec` instead.
* `gschur` was obsolete already in Scilab 4 and is now removed. Please use `schur` instead.
* `havewindow` has been removed. Please use `getscilabmode()=="STD"` instead
* `rafiter` was obsolete since Scilab 5.1 and is now removed.
* `jconvMatrixMethod` was obsolete and is now removed. Please use `jautoTranspose` instead.
* `fcontour` was obsolete since Scilab 4 and has been removed. Please use `contour` instead.

* Symbolic module functions have been removed: `addf`, `cmb_lin`, `ldivf`, `mulf`, `rdivf`, `solve`, `subf`, `trianfml`, `trisolve` and `block2exp`.
* Functionnalities based on former Scilab stack have been removed:
  - `comp`, `errcatch`, `iserror`, `fun2string`, `getvariablesonstack`, `gstacksize`, `macr2lst`, `stacksize`, `code2str` and `str2code`.
  - `-mem` launching option (used to set `stacksize` at startup).


Known issues
------------

* Scilab 6 is still in a beta stage and likely to contain a number of known or unknown bugs. Do not hesitate to [report](https://bugzilla.scilab.org) them.
* Toolboxes rebuild is in progress. Do not hesitate to submit patch or feature upgrade to the [development mailing list](dev@lists.scilab.org) for a particular toolbox.


Bug Fixes
---------

### In 6.0.0:
* [Bug #2919](http://bugzilla.scilab.org/show_bug.cgi?id=2919) fixed - The `fchamp` example and demo were unclear and badly rendered
* [Bug #5723](http://bugzilla.scilab.org/show_bug.cgi?id=5723) fixed   - Cross-references were missing between axis_properties and axes_properties help pages
* [Bug #7696](http://bugzilla.scilab.org/show_bug.cgi?id=7696) fixed   - The `parallel_run` help page was poorly formated
* [Bug #9912](http://bugzilla.scilab.org/show_bug.cgi?id=9912) fixed   - In case of missing translated help page, the default en_US was sometimes ignored
* [Bug #9153](http://bugzilla.scilab.org/9153) fixed                   - The `isqualbitwise` help page was inaccurate and badly located
* [Bug #11959](http://bugzilla.scilab.org/show_bug.cgi?id=11959) fixed - Allow "Zoom Area" to be clicked out of axes
* [Bug #12110](http://bugzilla.scilab.org/show_bug.cgi?id=12110) fixed - Unable to zoom multiple axes at once
* [Bug #13597](http://bugzilla.scilab.org/show_bug.cgi?id=13597) fixed - `help format` claimed setting a number of digits instead of characters
* [Bug #13757](http://bugzilla.scilab.org/show_bug.cgi?id=13757) fixed - The toolbox menu dit not load properly ATOMS modules not autoloaded.
* [Bug #13990](http://bugzilla.scilab.org/show_bug.cgi?id=13990) fixed - `warning` with localization enabled some memory corruption
* [Bug #14192](http://bugzilla.scilab.org/show_bug.cgi?id=14192) fixed - `g_margin` error-ed for double integrator.
* [Bug #14367](http://bugzilla.scilab.org/show_bug.cgi?id=14367) fixed - `edit_curv` failed opening due to a `[]+1` operation.
* [Bug #14448](http://bugzilla.scilab.org/show_bug.cgi?id=14448) fixed - removed havewindow() was still documented
* [Bug #14557](http://bugzilla.scilab.org/show_bug.cgi?id=14557) fixed - `csim` failed when the system has no state.
* [Bug #14582](http://bugzilla.scilab.org/14582) fixed                 - `gettext`or it alias `_()` were sometimes applied to broken literal strings
* [Bug #14586](http://bugzilla.scilab.org/show_bug.cgi?id=14586) fixed - Xcos simulation stop button did not work.
* [Bug #14587](http://bugzilla.scilab.org/show_bug.cgi?id=14587) fixed - Datatip textbox wrong clipping when loaded from `*.scg` file.
* [Bug #14590](http://bugzilla.scilab.org/show_bug.cgi?id=14590) fixed - Help pages in pt_BR directories had a wrong xml:lang="en" tag.
* [Bug #14593](http://bugzilla.scilab.org/show_bug.cgi?id=14593) fixed - Signs are no more drawn in BIGSOM and PRODUCT components.
* [Bug #14602](http://bugzilla.scilab.org/show_bug.cgi?id=14602) fixed - WRITEC_f block didn't work for x86 machines.
* [Bug #14648](http://bugzilla.scilab.org/show_bug.cgi?id=14648) fixed - `isinf` returned `%F` for complex numbers with both real and imag infinite parts.
* [Bug #14662](http://bugzilla.scilab.org/show_bug.cgi?id=14662) fixed - Matrix of strings concatenation with single quote led to a parser error.
* [Bug #14681](http://bugzilla.scilab.org/show_bug.cgi?id=14681) fixed - Short-circuited AND operation was not possible with double matrices in if and while clauses

### In 6.0.0 beta-2 and earlier:

* [Bug #2104](http://bugzilla.scilab.org/show_bug.cgi?id=2104) fixed   - `iw(1:9)` and `w(1:10)` `ode` output parameters were not documented
* [Bug #2517](http://bugzilla.scilab.org/show_bug.cgi?id=2517) fixed   - `"position"` property format was not accepted by `figure` despite what was said in help
* [Bug #6057](http://bugzilla.scilab.org/show_bug.cgi?id=6057) fixed   - trailing space after minus sign has been removed from the display of values
* [Bug #6064](http://bugzilla.scilab.org/show_bug.cgi?id=6064) fixed   - `scatter` did not exist in Scilab.
* [Bug #6314](http://bugzilla.scilab.org/show_bug.cgi?id=6314) fixed   - The identical code of `%p_m_r` and `%r_m_p` was not factorized
* [Bug #7378](http://bugzilla.scilab.org/show_bug.cgi?id=7378) fixed   - `quart` used with only `NaN`s yielded an error instead of returning `NaN`.
* [Bug #7646](http://bugzilla.scilab.org/show_bug.cgi?id=7646) fixed   - Extractions `A'(1,2)` and `A.'(1,2)` from a transposed matrix were not possible
* [Bug #7884](http://bugzilla.scilab.org/show_bug.cgi?id=7884) fixed   - `typeof` help page was poor, puzzled, and not up-to-date to Scilab 6:
  - new typeof `uint64`, `int64`, `void`, `deletelist`, `implicitlist ` were missing
  - former `hypermat` and `size implicit` typeof weren't removed
  - typeof names longer than 8-char were not documented.
* [Bug #8210](http://bugzilla.scilab.org/show_bug.cgi?id=8210) fixed   - UMFPACK demos were not well packaged and not numerous enough.
* [Bug #8310](http://bugzilla.scilab.org/show_bug.cgi?id=8310)         - Non-convex plane or unplane polygons could be wrongly triangulated and badly rendered with extra facets.
* [Bug #8990](http://bugzilla.scilab.org/show_bug.cgi?id=8990) fixed   - `.zoom_auto` feature was missing on the `plot` toolbar.
* [Bug #9456](http://bugzilla.scilab.org/show_bug.cgi?id=9456) fixed   - `bench_run` did not work on a path or in a toolbox
* [Bug #9560](http://bugzilla.scilab.org/show_bug.cgi?id=9560) fixed   - `1./M` was parsed as `1. / M` instead of `1 ./ M`
* [Bug #9621](http://bugzilla.scilab.org/show_bug.cgi?id=9621) fixed   - A `tlist` with undefined fields can now be saved.
* [Bug #10082](http://bugzilla.scilab.org/show_bug.cgi?id=10082) fixed - `string(complex)` with `real(complex)>0` did not remove the leading space replacing `"+"`
* [Bug #11511](http://bugzilla.scilab.org/show_bug.cgi?id=11511) fixed - `error` did not accept string matrix (non regression test added).
* [Bug #11625](http://bugzilla.scilab.org/show_bug.cgi?id=11625) fixed - Uicontrol table did not update `.string` when values were modified interactively in the table.
* [Bug #12044](http://bugzilla.scilab.org/show_bug.cgi?id=12044) fixed - Adding or substracting the empty matrix now return an empty matrix.
* [Bug #12202](http://bugzilla.scilab.org/show_bug.cgi?id=12202) fixed - Mixing int8 and doubles with colon operator led to wrong results.
* [Bug #12419](http://bugzilla.scilab.org/show_bug.cgi?id=12419) fixed - objects were cleared before the `scilab.quit` was called
* [Bug #12559](http://bugzilla.scilab.org/show_bug.cgi?id=12559)       - FFTW had some memory leaks
* [Bug #12872](http://bugzilla.scilab.org/show_bug.cgi?id=12872)       - Help pages of `else`, `elseif`, `end`, `try`, `sciargs`, global, halt, empty and power were in wrong help sections
* [Bug #12928](http://bugzilla.scilab.org/show_bug.cgi?id=12928) fixed - `intXX` functions with `%nan` and `%inf` return wrong values.
* [Bug #13154](http://bugzilla.scilab.org/show_bug.cgi?id=13154)       - In shellmode, completion now separates Files from Directories.
* [Bug #13289](http://bugzilla.scilab.org/show_bug.cgi?id=13289) fixed - Using non-integer indexes for mlists made Scilab crash.
* [Bug #13298](http://bugzilla.scilab.org/show_bug.cgi?id=13298) fixed - Static analysis bugs detected by PVS-Studio fixed.
* [Bug #13308](http://bugzilla.scilab.org/show_bug.cgi?id=13308) fixed - Xcos had no Crank-Nicolson solver.
* [Bug #13465](http://bugzilla.scilab.org/show_bug.cgi?id=13465) fixed - The display of polyline `.display_function` and `.display_function` properties was not conventional
* [Bug #13468](http://bugzilla.scilab.org/show_bug.cgi?id=13468) fixed - Scilab hanged when incorrect format was used for file reading using `mfscanf`.
* [Bug #13490](http://bugzilla.scilab.org/show_bug.cgi?id=13490) fixed - `histc` help page fixed to match the macro (by default, normalize the result).
* [Bug #13517](http://bugzilla.scilab.org/show_bug.cgi?id=13517) fixed - `isdef` crashed Scilab when called with a vector of strings as input in a function and after a declaration of variable.
* [Bug #13709](http://bugzilla.scilab.org/show_bug.cgi?id=13709) fixed - `unique` sometimes returned wrong index values.
* [Bug #13725](http://bugzilla.scilab.org/show_bug.cgi?id=13725) fixed - Sometimes `xfpoly` polygon filling failed.
* [Bug #13748](http://bugzilla.scilab.org/show_bug.cgi?id=13748) fixed - `printf`, `sprintf` (en,ja): short descriptions and obsolete flags were missing.
* [Bug #13750](http://bugzilla.scilab.org/show_bug.cgi?id=13750) fixed - Calling `ss2ss` with `flag = 2` returned an error.
* [Bug #13751](http://bugzilla.scilab.org/show_bug.cgi?id=13751) fixed - `lqg2stan` returned wrong (inverted) values.
* [Bug #13769](http://bugzilla.scilab.org/show_bug.cgi?id=13769) fixed - `t = "abc..//ghi"` was parsed as a continued + comment
* [Bug #13780](http://bugzilla.scilab.org/show_bug.cgi?id=13780) fixed - `size` with two input and output arguments did not return an error.
* [Bug #13795](http://bugzilla.scilab.org/show_bug.cgi?id=13795) fixed - `grep` with regexp option did not match the empty string properly
* [Bug #13807](http://bugzilla.scilab.org/show_bug.cgi?id=13807) fixed - Invalid margins were computed when figure was not visible.
* [Bug #13810](http://bugzilla.scilab.org/show_bug.cgi?id=13810) fixed - `householder(v, k*v)` returned column of `Nan`. Input parameters were not checked. The Householder matrix could not be returned. Help pages were inaccurate and without examples. There was no `householder` demo.
* [Bug #13816](http://bugzilla.scilab.org/show_bug.cgi?id=13816) fixed - `show_margins` caused a scilab crash
* [Bug #13829](http://bugzilla.scilab.org/show_bug.cgi?id=13829) fixed - `mean` and `sum` returned wrong results for hypermatrices.
* [Bug #13831](http://bugzilla.scilab.org/show_bug.cgi?id=13831) fixed - `ss2ss` did not update the initial state
* [Bug #13834](http://bugzilla.scilab.org/show_bug.cgi?id=13834) fixed - Drawing a high number of strings in a figure generated a Java exception.
* [Bug #13838](http://bugzilla.scilab.org/show_bug.cgi?id=13838) fixed - Sparse and complex substraction made Scilab crash.
* [Bug #13839](http://bugzilla.scilab.org/show_bug.cgi?id=13839) fixed - `sign` could not be used with sparse matrices
* [Bug #13843](http://bugzilla.scilab.org/show_bug.cgi?id=13843) fixed - Scilab crashed when `polarplot` and `plot2d` were called with wrong `strf` value.
* [Bug #13853](http://bugzilla.scilab.org/show_bug.cgi?id=13853) fixed - `plzr` returned wrong results for discrete-time systems with a numeric time step.
* [Bug #13854](http://bugzilla.scilab.org/show_bug.cgi?id=13854) fixed - Under some operating systems, SciNotes did not initialize a new document at startup.
* [Bug #13862](http://bugzilla.scilab.org/show_bug.cgi?id=13862) fixed - There was no lazy evaluation of `or` operands in `if` tests.
* [Bug #13864](http://bugzilla.scilab.org/show_bug.cgi?id=13864) fixed - `%l_isequal` was useless in Scilab 6.
* [Bug #13866](http://bugzilla.scilab.org/show_bug.cgi?id=13866) fixed - There were some issues with FFTW3 library.
* [Bug #13869](http://bugzilla.scilab.org/show_bug.cgi?id=13869) fixed - `bench_run` with option `nb_run=10` did not override the NB RUN tags
* [Bug #13872](http://bugzilla.scilab.org/show_bug.cgi?id=13872) fixed - Non regression test added for `unique` (the indices returned were wrong)
* [Bug #13873](http://bugzilla.scilab.org/show_bug.cgi?id=13873) fixed - `%hm_stdev(H,idim>2)` returned `zeros(H)`
* [Bug #13881](http://bugzilla.scilab.org/show_bug.cgi?id=13881) fixed - `datatipRemoveAll` did not work.
* [Bug #13890](http://bugzilla.scilab.org/show_bug.cgi?id=13890) fixed - `getd` did not return loaded symbols in previous scope.
* [Bug #13893](http://bugzilla.scilab.org/show_bug.cgi?id=13893) fixed - `simp` did not set a rational denominator at `1` when numerator was equal to zero.
* [Bug #13894](http://bugzilla.scilab.org/show_bug.cgi?id=13894) fixed - Default working directory of the previous session did not work.
* [Bug #13897](http://bugzilla.scilab.org/show_bug.cgi?id=13897) fixed - Concatenating structures with same fields in mismatching orders failed
* [Bug #13899](http://bugzilla.scilab.org/show_bug.cgi?id=13899) fixed - Syntax coloring was off in `scinotes`
* [Bug #13903](http://bugzilla.scilab.org/show_bug.cgi?id=13903) fixed - `get_function_path` returned a path with a missing file separator.
* [Bug #13907](http://bugzilla.scilab.org/show_bug.cgi?id=13907) fixed - Avoids the gray background on the right panel of the palette Browser.
* [Bug #13908](http://bugzilla.scilab.org/show_bug.cgi?id=13908) fixed - `part(text, n:$)` was very slow.
* [Bug #13918](http://bugzilla.scilab.org/show_bug.cgi?id=13918) fixed - Unmanaged operations on hypermatrix did not call overload functions.
* [Bug #13919](http://bugzilla.scilab.org/show_bug.cgi?id=13919) fixed - Scilab parsed `hidden` as a reserved keyword but it is not used.
* [Bug #13920](http://bugzilla.scilab.org/show_bug.cgi?id=13920) fixed - `getscilabkeywords` help page should be in the "Scilab keywords" section.
* [Bug #13923](http://bugzilla.scilab.org/show_bug.cgi?id=13923) fixed - Changes of `typeof(:)` and `typeof(n:$)` were not documented.
* [Bug #13924](http://bugzilla.scilab.org/show_bug.cgi?id=13924) fixed - rationals `r1==r2` and `r1~=r2` might sometimes be wrong.
* [Bug #13925](http://bugzilla.scilab.org/show_bug.cgi?id=13925) fixed - SciNotes used the wrong paired bracket highlight style.
* [Bug #13931](http://bugzilla.scilab.org/show_bug.cgi?id=13931) fixed - handle `aarch64` processor for some Linux distribution.
* [Bug #13939](http://bugzilla.scilab.org/show_bug.cgi?id=13939) fixed - In HTML help pages, itemizedlist `<ul>` were rendered as numbered ones
* [Bug #13941](http://bugzilla.scilab.org/show_bug.cgi?id=13941) fixed - Internal timestamps of HDF5 files prevented having a fixed hash for an unvarying set of saved objects.
* [Bug #13942](http://bugzilla.scilab.org/show_bug.cgi?id=13942) fixed - the palette browser tree was always resized when expanded/collapsed.
* [Bug #13944](http://bugzilla.scilab.org/show_bug.cgi?id=13944) fixed - The menu "Toolboxes" was missing.
* [Bug #13965](http://bugzilla.scilab.org/show_bug.cgi?id=13965) fixed - The rendering of histograms with `histplot` was poor
* [Bug #13966](http://bugzilla.scilab.org/show_bug.cgi?id=13966) fixed - `twinkle` and `twinkle(n)` were not supported
* [Bug #13971](http://bugzilla.scilab.org/show_bug.cgi?id=13971) fixed - A space has been added between Scilab prompt and cursor.
* [Bug #13972](http://bugzilla.scilab.org/show_bug.cgi?id=13972) fixed - Wildcard `*` was not managed in `printf` expressions.
* [Bug #13974](http://bugzilla.scilab.org/show_bug.cgi?id=13974) fixed - `isoview(xmin, xmax, ymin, ymax)` was unhandy.
* [Bug #13983](http://bugzilla.scilab.org/show_bug.cgi?id=13983) fixed - `who_user` returned wrong values.
* [Bug #13986](http://bugzilla.scilab.org/show_bug.cgi?id=13986) fixed - `setdefaultlanguage` did not set value correctly in Windows registry.
* [Bug #13990](http://bugzilla.scilab.org/show_bug.cgi?id=13990) fixed - `gettext` did not manage the added `_W` macro.
* [Bug #13999](http://bugzilla.scilab.org/show_bug.cgi?id=13999) fixed - `editor` was modal. It locked the console using an external editor.
* [Bug #14020](http://bugzilla.scilab.org/show_bug.cgi?id=14020) fixed - Incorrect carriage return ascii code.
* [Bug #14022](http://bugzilla.scilab.org/show_bug.cgi?id=14022) fixed - `getscilabkeywords` was KO (+gateway what() added).
* [Bug #14023](http://bugzilla.scilab.org/show_bug.cgi?id=14023) fixed - It was not possible to concatenate cells.
* [Bug #14024](http://bugzilla.scilab.org/show_bug.cgi?id=14024) fixed - Print of `macrofile` display a debug message instead of macro prototype.
* [Bug #14025](http://bugzilla.scilab.org/show_bug.cgi?id=14025) fixed - `head_comments` did not take into account compiled functions.
* [Bug #14028](http://bugzilla.scilab.org/show_bug.cgi?id=14028) fixed - force flag of `genlib` did not rebuild bin file.
* [Bug #14030](http://bugzilla.scilab.org/show_bug.cgi?id=14030) fixed - Linear algebra demo crashed due to a bad delete in `schur` implementation.
* [Bug #14035](http://bugzilla.scilab.org/show_bug.cgi?id=14035) fixed - `ndgrid` did not manage all homogeneous data type (booleans, integers, polynomials, rationals, strings, `[]`)
* [Bug #14036](http://bugzilla.scilab.org/show_bug.cgi?id=14036) fixed - `.tag` and `.user_data` properties were not displayed and not documented for light entity.
* [Bug #14038](http://bugzilla.scilab.org/show_bug.cgi?id=14038) fixed - Encoded integers were no longer accepted for list extraction.
* [Bug #14040](http://bugzilla.scilab.org/show_bug.cgi?id=14040) fixed - graphic property setting fails when using array of handles
* [Bug #14041](http://bugzilla.scilab.org/show_bug.cgi?id=14041) fixed - `genlib` crashed when the file is locked by another program.
* [Bug #14043](http://bugzilla.scilab.org/show_bug.cgi?id=14043) fixed - Examples of API Scilab help pages had to be updated (`pvApiCtx` in gateway prototypes).
* [Bug #14044](http://bugzilla.scilab.org/show_bug.cgi?id=14044) fixed - `MALLOC.h` is now renamed to `sci_malloc.h`.
* [Bug #14047](http://bugzilla.scilab.org/show_bug.cgi?id=14047) fixed - wrong behaviour of `break` ( `continue` ) in `if` and outside of loop fixed.
* [Bug #14049](http://bugzilla.scilab.org/show_bug.cgi?id=14049) fixed - `genlib` hang if an unexpected `endfunction` occurs.
* [Bug #14055](http://bugzilla.scilab.org/show_bug.cgi?id=14055) fixed - overload on matrix concatenation was not called with `[]`.
* [Bug #14057](http://bugzilla.scilab.org/show_bug.cgi?id=14057) fixed - `grand(m,n)` returned a wrong error and `grand(m,n,p)` called an overloading function instead of returning an error.
* [Bug #14058](http://bugzilla.scilab.org/show_bug.cgi?id=14058) fixed - Scilab crashed with `file("close", file())` instruction
* [Bug #14059](http://bugzilla.scilab.org/show_bug.cgi?id=14059) fixed - Lack of performance on deletion of matrix elements.
* [Bug #14065](http://bugzilla.scilab.org/show_bug.cgi?id=14065) fixed - Change "java size" in points in graphics font help page.
* [Bug #14067](http://bugzilla.scilab.org/show_bug.cgi?id=14067) fixed - 3rd argument of `fsolve` became mandatory
* [Bug #14082](http://bugzilla.scilab.org/show_bug.cgi?id=14082) fixed - `m=1; m()=1;` made Scilab crash.
* [Bug #14093](http://bugzilla.scilab.org/show_bug.cgi?id=14093) fixed - `atanh` returns NaN for values with an absolute value greater than 1
* [Bug #14095](http://bugzilla.scilab.org/show_bug.cgi?id=14095) fixed - Scilab crashed when a .fig file was loaded with `loadmatfile` function.
* [Bug #14096](http://bugzilla.scilab.org/show_bug.cgi?id=14096) fixed - Issue with `mscanf`.
* [Bug #14097](http://bugzilla.scilab.org/show_bug.cgi?id=14097) fixed - `genlib` no more adds a separator at the end of the lib path if it is not given in the directory path.
* [Bug #14099](http://bugzilla.scilab.org/show_bug.cgi?id=14099) fixed - `sci2exp` macro was fixed to avoid "a+[] Warning". string(polynomials|rationals) had badly formated outputs and was not vectorized
* [Bug #14105](http://bugzilla.scilab.org/show_bug.cgi?id=14105) fixed - New block comments `/*...*/` feature was not documented.
* [Bug #14107](http://bugzilla.scilab.org/show_bug.cgi?id=14107) fixed - `lstcat` of a string and a list did not produce consistent results.
* [Bug #14109](http://bugzilla.scilab.org/show_bug.cgi?id=14109) fixed - `lsq` crashed Scilab when Scilab version depended on MKL library.
* [Bug #14111](http://bugzilla.scilab.org/show_bug.cgi?id=14111) fixed - In Scilab 6, `lib` loading a Scilab 5 library did not give a proper error message.
* [Bug #14113](http://bugzilla.scilab.org/show_bug.cgi?id=14113) fixed - Scilab 6 did not detect infinite loop.
* [Bug #14115](http://bugzilla.scilab.org/show_bug.cgi?id=14115) fixed - In Scinotes, the `switch` and `otherwise` keywords were no longer colorized.
* [Bug #14116](http://bugzilla.scilab.org/show_bug.cgi?id=14116) fixed - Invalid exponent in case of complex exponents especially `0*%i`.
* [Bug #14118](http://bugzilla.scilab.org/show_bug.cgi?id=14118) fixed - `real`, `imag`, `conj`, `isreal` did not accept rationals
* [Bug #14135](http://bugzilla.scilab.org/show_bug.cgi?id=14135) fixed - crash when running "Graphics -> Matplot -> Java Image" demonstration.
* [Bug #14141](http://bugzilla.scilab.org/show_bug.cgi?id=14141) fixed - `gcf().attribute=value` lead to "Wrong insertion : function or macro are not expected".
* [Bug #14144](http://bugzilla.scilab.org/show_bug.cgi?id=14144) fixed - Scilab crashed with `int64(2^63)`.
* [Bug #14149](http://bugzilla.scilab.org/show_bug.cgi?id=14149) fixed - HDF5 could not restore hypermatrix with good dimensions.
* [Bug #14150](http://bugzilla.scilab.org/show_bug.cgi?id=14150) fixed - The Windows SDK was not found on Windows 8.1.
* [Bug #14156](http://bugzilla.scilab.org/show_bug.cgi?id=14156) fixed - `mfscanf` returned an empty matrix when datafile contained a header.
* [Bug #14159](http://bugzilla.scilab.org/show_bug.cgi?id=14159) fixed - `Matplot` crashed Scilab on boolean input.
* [Bug #14178](http://bugzilla.scilab.org/show_bug.cgi?id=14178) fixed - Tcl/Tk unavailability on MacOS is now documented.
* [Bug #14181](http://bugzilla.scilab.org/show_bug.cgi?id=14181) fixed - `intg` (or `integrate`) in a function that is being integrated failed.
* [Bug #14187](http://bugzilla.scilab.org/show_bug.cgi?id=14187) fixed - `fscanfMat` failed to read format `%d`, `%i` and `%f`.
* [Bug #14199](http://bugzilla.scilab.org/show_bug.cgi?id=14199) fixed - `sqrt` returned wrong dimension results on matrix with more than dimensions.
* [Bug #14203](http://bugzilla.scilab.org/show_bug.cgi?id=14203) fixed - Improve some error messages, some help pages and some comments.
* [Bug #14204](http://bugzilla.scilab.org/show_bug.cgi?id=14204) fixed - `dec2bin` ( `dec2base` ) must show a better error message for too large values.
* [Bug #14205](http://bugzilla.scilab.org/show_bug.cgi?id=14205) fixed - Console crash when assigning uint32 numbers to double matrix.
* [Bug #14209](http://bugzilla.scilab.org/show_bug.cgi?id=14209) fixed - `1:245` created infinite loop.
* [Bug #14212](http://bugzilla.scilab.org/show_bug.cgi?id=14212) fixed - Scilab 6 did not load array of struct from Scilab 5.5 files correctly
* [Bug #14219](http://bugzilla.scilab.org/show_bug.cgi?id=14219) fixed - As [bug #14203](http://bugzilla.scilab.org/show_bug.cgi?id=14203), improve some error messages, some help pages and some comments.
* [Bug #14223](http://bugzilla.scilab.org/show_bug.cgi?id=14223) fixed - `det` returned an error when it is used with a singular matrix.
* [Bug #14225](http://bugzilla.scilab.org/show_bug.cgi?id=14225) fixed - command-line option "-quit" should set the processs exit status
* [Bug #14228](http://bugzilla.scilab.org/show_bug.cgi?id=14228) fixed - Setting `.rotation_angles` property to a matrix of any size did not return error message.
* [Bug #14232](http://bugzilla.scilab.org/show_bug.cgi?id=14232) fixed - Typos fixed in Xcos.
* [Bug #14245](http://bugzilla.scilab.org/show_bug.cgi?id=14245) fixed - Problem in recursive extraction using list with `struct`.
* [Bug #14247](http://bugzilla.scilab.org/show_bug.cgi?id=14247) fixed - `sqrt` did not work on hypermatrices (non regression test added).
* [Bug #14249](http://bugzilla.scilab.org/show_bug.cgi?id=14249) fixed - `Ctrl-C` can be used to stop writing control expression.
* [Bug #14251](http://bugzilla.scilab.org/show_bug.cgi?id=14251) fixed - `spec` leaked some memory.
* [Bug #14253](http://bugzilla.scilab.org/show_bug.cgi?id=14253) fixed - Insertion in a struct contained in a list fixed.
* [Bug #14255](http://bugzilla.scilab.org/show_bug.cgi?id=14255) fixed - `fftw` without MKL leaked.
* [Bug #14271](http://bugzilla.scilab.org/show_bug.cgi?id=14271) fixed - `conjgrad` displayed an incorrect error message about number of arguments.
* [Bug #14297](http://bugzilla.scilab.org/show_bug.cgi?id=14297) fixed - `cumsum`'s output was badly documented.
* [Bug #14300](http://bugzilla.scilab.org/show_bug.cgi?id=14300) fixed - Crash when playing with structures.
* [Bug #14303](http://bugzilla.scilab.org/show_bug.cgi?id=14303) fixed - matrix display for large number was not correctly aligned
* [Bug #14304](http://bugzilla.scilab.org/show_bug.cgi?id=14304) fixed - `find(x, nmax)` returned `[]` (non regression test added).
* [Bug #14313](http://bugzilla.scilab.org/show_bug.cgi?id=14313) fixed - Parser did not create a column separator after spaces and `...` at the end of lines
* [Bug #14316](http://bugzilla.scilab.org/show_bug.cgi?id=14316) fixed - Operation `scalar^matrix` was identical to `scalar.^matrix` instead of being `expm( log(scalar) * matrix )`
* [Bug #14326](http://bugzilla.scilab.org/show_bug.cgi?id=14326) fixed - It was no longer possible to delete any part of a structure array with `[]`.
* [Bug #14331](http://bugzilla.scilab.org/show_bug.cgi?id=14331) fixed - The third argument of `lsq` crashed Scilab.
* [Bug #14347](http://bugzilla.scilab.org/show_bug.cgi?id=14347) fixed - `plot2d` crashed with multiple entries (non regression test added).
* [Bug #14348](http://bugzilla.scilab.org/show_bug.cgi?id=14348) fixed - Scilab did not open sce/sci file from Windows explorer.
* [Bug #14359](http://bugzilla.scilab.org/show_bug.cgi?id=14359) fixed - Black Hole demo updated. Stop and Clear buttons did not have priority tag set. `Callback_type` property has been added and set to `10`.
* [Bug #14361](http://bugzilla.scilab.org/show_bug.cgi?id=14361) fixed - Parser did not manage -linebreak + blockcomment `... /* a comment */`
* [Bug #14362](http://bugzilla.scilab.org/show_bug.cgi?id=14362) fixed - The `ode` Lotka demo had typo errors
* [Bug #14366](http://bugzilla.scilab.org/show_bug.cgi?id=14366) fixed - `typeof(var,"overload")` was not documented
* [Bug #14374](http://bugzilla.scilab.org/show_bug.cgi?id=14374) fixed - The parser did not manage comments properly in shellmode
* [Bug #14375](http://bugzilla.scilab.org/show_bug.cgi?id=14375) fixed - Calling `input` with a argument of 64 characters or more crashed Scilab.
* [Bug #14389](http://bugzilla.scilab.org/show_bug.cgi?id=14389) fixed - The new `int64` and `uint64` were not documented, and other help pages were not updated for them.
* [Bug #14390](http://bugzilla.scilab.org/show_bug.cgi?id=14390) fixed - `double` help page had irrelevant syntaxes and was poor
* [Bug #14396](http://bugzilla.scilab.org/show_bug.cgi?id=14396) fixed - Real number display was not proper for very wide decimal parts
* [Bug #14398](http://bugzilla.scilab.org/show_bug.cgi?id=14398) fixed - Matrix extraction was mistakenly considered a function call in calling sequence
* [Bug #14405](http://bugzilla.scilab.org/show_bug.cgi?id=14405) fixed - GetScilabVariableJNI symbol was not resolved on xcosPalAdd()
* [Bug #14415](http://bugzilla.scilab.org/show_bug.cgi?id=14415) fixed - Some spelling errors were detected in ~20 help pages
* [Bug #14416](http://bugzilla.scilab.org/show_bug.cgi?id=14416) fixed - The file extension filter in Scinotes "Save as" window did not re-used the active file's extension when applicable.
* [Bug #14418](http://bugzilla.scilab.org/show_bug.cgi?id=14418) fixed - `saxon9he.jar` made scilab throw an XPathFactoryConfigurationException.
* [Bug #14419](http://bugzilla.scilab.org/show_bug.cgi?id=14419) fixed - Scinotes's highlighting of the new `||` and `&&` operators was wrong.
* [Bug #14423](http://bugzilla.scilab.org/show_bug.cgi?id=14423) fixed - `bench_run did` not have a return value, export file was not configurable
* [Bug #14425](http://bugzilla.scilab.org/show_bug.cgi?id=14425) fixed - `xpause` was a duplicate of `sleep`. `sleep` did not propose "s" time unit.
* [Bug #14429](http://bugzilla.scilab.org/show_bug.cgi?id=14429) fixed - Rationals `r+(-r)` and `r-r` did not simplify the denominator to 1 in simp_mode(%t)
* [Bug #14432](http://bugzilla.scilab.org/show_bug.cgi?id=14432) fixed - Using an implicit list as scoped assignation to a variable in function call caused scilab to crash
* [Bug #14433](http://bugzilla.scilab.org/show_bug.cgi?id=14433) fixed - `acoth` (which call `atanh`) crashed scilab
* [Bug #14434](http://bugzilla.scilab.org/show_bug.cgi?id=14434) fixed - `PlotSparse` did not work anymore.
* [Bug #14446](http://bugzilla.scilab.org/show_bug.cgi?id=14446) fixed - An error message generated by `save(..)` pointed a bad argument index.
* [Bug #14450](http://bugzilla.scilab.org/show_bug.cgi?id=14450) fixed - `builder_help.sce` of a toolbox ignored some existing language directories
* [Bug #14453](http://bugzilla.scilab.org/show_bug.cgi?id=14453) fixed - `strcat([])` returned an empty string instead of `[]`.
* [Bug #14455](http://bugzilla.scilab.org/show_bug.cgi?id=14455)       - `macr2tree` crashed when passing a FieldExp.
* [Bug #14468](http://bugzilla.scilab.org/show_bug.cgi?id=14468) fixed - Scinotes was unable to export to HTML.
* [Bug #14471](http://bugzilla.scilab.org/show_bug.cgi?id=14471) fixed - `strange([])` returned `[]` instead of `Nan` as all other functions for statistical dispersion
* [Bug #14476](http://bugzilla.scilab.org/show_bug.cgi?id=14476) fixed - Operation `.*` between polynomials and imaginary numbers was always returning `0`
* [Bug #14493](http://bugzilla.scilab.org/show_bug.cgi?id=14493) fixed - `and` and `or` help pages were poor and inaccurate.
* [Bug #14495](http://bugzilla.scilab.org/show_bug.cgi?id=14495) fixed - `consolebox` help page shew wrong syntaxes and was poor.
* [Bug #14499](http://bugzilla.scilab.org/show_bug.cgi?id=14499) fixed - `getd` did not update already defined functions
* [Bug #14500](http://bugzilla.scilab.org/show_bug.cgi?id=14500) fixed - Operator `.^` was broken for sparse matrices.
* [Bug #14517](http://bugzilla.scilab.org/show_bug.cgi?id=14517) fixed - The second argument of part function accepted an index of 0 without exiting in error.
* [Bug #14524](http://bugzilla.scilab.org/show_bug.cgi?id=14524) fixed - Numeric locales were not set to standard "C" by default at scilab startup
* [Bug #14540](http://bugzilla.scilab.org/show_bug.cgi?id=14540) fixed - Datatips did not clip outside axes bounds
