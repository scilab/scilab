Welcome to Scilab 6.0.0 beta-2
==============================

This file details the changes between Scilab 6.0.0 (this release), and the previous release 5.5.2.
For changelogs of earlier releases, please see [Scilab 5.5.2](https://www.scilab.org/en/content/download/3332/24658/file/Scilab5.5.2_ReleaseNotes.pdf).

This file is intended for the specific needs of advanced users, and describes:
- High-level new features,
- Changes in installation, compilation, and compatibility,
- Changes in the language,
- New and modified features, in each module,
- Changes in functions (removed/added/modified),
- Bug fixes.

This changelog is most likely incomplete, as an enormous amount of code has changed between 5.5.2 and 6.
Please report any thing we could have missed, on the [mailing lists][1] or on the [bug tracker][2], 
and we will correct it before the final Scilab 6.0.0 release.

[1]: http://mailinglists.scilab.org
[2]: http://bugzilla.scilab.org


Main new features
-----------------

For high-level description of the main new features of this release, please 
consult the [embedded help](modules/helptools/data/pages/homepage-en_US.html). It is also available 
as the "What's new" page of the help, by simply typing `help` in Scilab console.

In summary, the main new features are:
* New language parser and interpreter, ensuring:
  - Support for bigger data sets, thanks to dynamic memory usage. No need for `stacksize` anymore.
  - Better performance for some objects (cells and structs).
  - Clearer, less ambiguous, language syntax.
  - Xcos also uses the new re-written Scilab engine.
* New code productivity features: full-featured debugger, profiler / coverage tool, and "lint"-like commands.
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


Language changes
----------------

Some small changes have been done to the language syntax, aiming at more clarity and less
ambiguity. Some changes are not compatible with 5.5; code written for Scilab 5.x
has to be modified in order to work in Scilab 6.0. 

See [the wiki page on porting code from 5.5 to 6.0](https://wiki.scilab.org/FromScilab5ToScilab6)
for details and examples.

* Declaration of a number on two lines is no longer allowed: `1.. \n 2`.
* Comment blocks on multiple lines `/* ...\n ... \n ... */` are now possible.
* `1./M` is now parsed as `1 ./ M` instead of `1. / M`.
* Declaring strings using non-homogenous delimiters ("string' or 'string") is no longer allowed.
* `(a=b)` executed as `a == b` is now deprecated and returns an error.
* Function definitions can finish with `end` instead of `endfunction`.
* `{}` and `[]` are now distinct operators. Matrices can no longer be defined usind `{}` operators. 
These are reserved for cell definition.
* Shortcut and element-wise boolean operators are now distinct. `&&` and `||` are new shortcut boolean AND and OR,
while `&` and `|` are element-wise operations and will not shortcut one of the operand. 
Both shortcut and element-wise operators are evaluated from left to right.
* Syntax `%i:10` is now deprecated (only real scalars can be used).
* `while ... else ... end` control instruction is not supported anymore.
* Transposed matrix can now use the extraction operator directly `A'(1, 2)`.
* Function without output argument cannot be called in assignation expression anymore:
	```
	function foo()
		//...
	endfunction

	val = foo() // now returns an error
	```


Feature changes and additions
-----------------------------

* Addition or subtraction with an empty matrix now returns an empty matrix.
* scatter/scatter3 plot with different mark colors is now available.
* parulacolormap is now available.
* name2rgb can now handle a single string and a matrix of strings.
* `isoview`, `isoview on`, `isoview off`, `isoview(idGraphics, "on"|"off")` are now supported.
* `twinkle` and `twinkle(n)` are now supported: by default, the current element `gce` blinks.
* `replot` has been upgraded:
  - a bound set to %inf now gets the position of the most marginal object,
  - replot() can now be used to reframe axes to all their contents,
  - option "tigh_limits" added,
  - Any handle having some Axes as direct children -- as uicontrol-frame -- is now supported.
* `householder` can now return the reflection matrix, and has a demo.
* `ndgrid` can now works with any types of homogeneous data.
* `bench_run` can now return its results and/or record them in a file.
* `typeof(.., "overload")` allows now to get the overloading type-code of an object.
* `sign` can now handle a sparse matrix.
* `sleep(..,'s')` allows now to specify the duration in seconds.
* `real`, `imag`, `conj` and `isreal` now accept rational fractions.
* A call stack limit has been introduced. Default maximum depth is setup to 1000 
and can be changed by `recursionlimit` or through the Preferences interface.
* The floating point exception mode `ieee` is now set to `2` by default: floating 
point exceptions now produce `Inf` or `Nan`, and do not cause any error.
The previous behavior can be recalled by simply calling: `ieee(0)`.
* The graphics entity "Datatip" has its property `z_component = 'on|off'` changed to 
`display_components = 'xyz'`; it is now possible to choose which components to display,
and in which order. Loading old `*.scg` files containing datatips with "z_component"
property may not display the z component.
* Valgrind error detection added to `test_run` (on Linux only).
* `amell` now:
  - checks if its parameters are real numbers,
  - throws an error if the second parameter is not a scalar.
* The use of I/O console is now allowed with the following functions: `mget`, 
`mgetl`, `mgetstr`, `mput`, `mputl` and `mputstr`.
* `mclearerr` now returns a flag indicating the file identifier validity.
* `fileinfo` can now take a row vector as input.
* `msprintf` does not return an error message anymore when there are too many
input arguments (more values that format needs).
* `deletefile` can delete multiple files at once.
* `exec` of macro executes the body in the current scope, but the prototype must have zero
input and output arguments.
* `error`: an error number in input is deprecated.
* `impl`: Recall impl with the same parameters as in its previous stop is now available.
* `ode`: `y0` is restricted to a column vector.
* `pppdiv`: Return a scalar of type 'constant' when the rank is 0.
* `pdiv`: Return a matrix of type 'constant' when all the rank are 0.
* `test_run` can now take "[]" as argument to be used on console; for instance: `test_run string [] no_check_ref`.
* `typeof(:)` and `typeof(n:$)` now return `"implicitlist"` instead of respectively `"constant"` and `"size implicit"`.
* `strange([])` now returns `%nan` instead of `[]`, as all other functions for statistical dispersion.
* `stdev(x, dir>ndims(x))` now yields an error instead of returning `zeros(x)`.
* `write`: Writing string or string matrix in a file does not add blank space before each value.
* `bitor`, `bitxor` and `bitand` are upgraded:
   - positive signed encoded integers are now accepted.
   - inputs with new `int64` or `uint64` encodings are now accepted.
   - operands with mixed types or/and inttypes are now accepted.
   - distributive input scalars as in `bit###(scalar, array)` or `bit###(array, scalar)` are now accepted.
   - results with decimal-encoded integers > 2^32 are now correct.
   - decimal-encoded integers > 2^52 are now supported up to the biggest 1.80D+308.
   - `bitxor` is now vectorized and fast.
* The zoom rubber box now can start/finish from points lying outside the axes bounds.
* The zoom rubber box now can select multiple and overlying axes at once. Changed behavior of scroll zoom,
scrolling over overlying axes will zoom all of them together, using the CTRL key while scrolling 
will zoom all axes in the current figure.
* The graphics entity "Datatip" has a new property `detached_position` which accepts `[]` 
or a 3-components vector to set the position in axes coordinates to draw the datatip text box.
* `MPI_Create_comm` create a new communicator from MPI_COMM_WORLD using MPI world ranks.

Help pages:
-----------

* fixed / improved:  `members`, `part`, `ode`, `ode_optional_output`, `ode_roots`, `roots`,
  `printf`, `sprintf`, `iconvert`, `stdev`, `xlabel`, `and_op`, `or_op`
* rewritten: `consolebox`, `double`, `isoview`, `householder`, `or`, `and`, `format`, `typeof`,
`brackets`, `setlanguage`, `sleep`, `isinf`, `bitor`, `bitxor`, `bitand`
* reorganized:
  - `else`, `elseif`, `end`, `try`, `sciargs`, `global`, `halt`, `empty`, `power`
  - CACSD and Signal Processing help pages have been sorted up.
  - Colormaps and GUI/Menus subsections created
* translations added:
  - (fr): `format`, `typeof`, `isoview`, `ndgrid`, `bench_run`, `consolebox`, `harmean`, `sleep`, `strtod`
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

* Major rewrite of the data structures, huge models should load and save faster.
The memory usage on diagram edition is also slightly reduced.
* ZCOS and XCOS file formats have evolved to reduce the duplicated information. 
Scilab 5.5.2 is able to open the newly saved files, but the ports have to be repositioned manually.
* Implicit fixed-size step ODE solver added: Crank-Nicolson 2(3). 
Added to the CVode package, it also benefits from the CVode rootfinding feature.
* Block modifications :
  - `INVBLK`: add a divide by zero parameter to ignore the error
  - `PRODUCT`: add a divide by zero parameter to ignore the error


API modification
----------------

A new set of C APIs to write C or C++ extensions (toolboxes) to Scilab. 
It allows defining native functions (commonly called "gateways"), getting input parameters
for such functions, setting return parameters, accessing local variables, using common helper
functions for accessing environment information (such as warning level), generate errors... 

It also includes ways to overload existing Scilab functions to support additional parameter
types (see `help scilab_overload`). Finally, you can call back Scilab functions 
(macros and built-in functions) from your gateway (see `help scilab_call`).

User-defined functions written in C or C++ (gateways) must now use a `void* pvApiCtx` name
as a second parameter instead of any `unsigned long l`. This is now required for some macros, such as `Rhs`, to work.

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

* Scilab 6 is still in a beta stage and likely to contain a number of known or unknown bugs.
Do not hesitate to [report](https://bugzilla.scilab.org) them.
* Toolboxes rebuild is in progress. Do not hesitate to submit patch or feature upgrade to 
the [development mailing list](dev@lists.scilab.org) for a particular toolbox.


Bug Fixes
---------

### Bugs fixed in 6.0.0:
* [#2919](http://bugzilla.scilab.org/show_bug.cgi?id=2919): The `fchamp` example and demo were unclear and badly rendered
* [#4327](http://bugzilla.scilab.org/show_bug.cgi?id=4327): Overloading did not support custom types names longer than 8 characters
* [#5723](http://bugzilla.scilab.org/show_bug.cgi?id=5723): Cross-references were missing between axis_properties and axes_properties help pages
* [#7192](http://bugzilla.scilab.org/show_bug.cgi?id=7192): From S=[], S($+1,:) = some_row inserted it in row#2 after a parasitic row#1.
* [#7649](http://bugzilla.scilab.org/show_bug.cgi?id=7649): `isempty` returned `%F` on `struct()`, `{}` or `list(,)` and was not shortcut
* [#7696](http://bugzilla.scilab.org/show_bug.cgi?id=7696): The `parallel_run` help page was poorly formated
* [#8010](http://bugzilla.scilab.org/show_bug.cgi?id=8010): Permanent variables could be redefined through a syntax like `%i(1,1)=1`
* [#8356](http://bugzilla.scilab.org/show_bug.cgi?id=8356): `sci2exp` applied to lists, tlists or mlists having undefined fields yielded an error or a wrong result.
* [#8493](http://bugzilla.scilab.org/show_bug.cgi?id=8493): Some trivial simplifications of `p1./p2` with matrices of complex-encoded polynomials were not done.
* [#8938](http://bugzilla.scilab.org/show_bug.cgi?id=8938): In a boolean sparse matrix `sp`, distributive insertions like `sp(1,:)=%t`, `sp(1,1:$)=%t` or `sp(:,:)=%t` yielded an error.
* [#9153](http://bugzilla.scilab.org/show_bug.cgi?id=9153): The `isqualbitwise` help page was inaccurate and badly located
* [#9161](http://bugzilla.scilab.org/show_bug.cgi?id=9161): Multiple insertions at a repeated index in a sparse matrice wrongly updated it.
* [#9912](http://bugzilla.scilab.org/show_bug.cgi?id=9912): In case of missing translated help page, the default en_US was sometimes ignored
* [#10116](http://bugzilla.scilab.org/show_bug.cgi?id=10116): `for h = H, .., end` could not be used when H is a vector of graphic handles
* [#11375](http://bugzilla.scilab.org/show_bug.cgi?id=11375): When a localized help subdirectory has only a CHAPTER file specifying the section title, it was ignored.
* [#11692](http://bugzilla.scilab.org/show_bug.cgi?id=11692): The summary of a help section built from both default en_US and localized files was never sorted overall.
* [#11959](http://bugzilla.scilab.org/show_bug.cgi?id=11959): Allow "Zoom Area" to be clicked out of axes
* [#12110](http://bugzilla.scilab.org/show_bug.cgi?id=12110): Unable to zoom multiple axes at once
* [#13166](http://bugzilla.scilab.org/show_bug.cgi?id=13166): `l` and `b` endian flags used with `mget` and `mgeti` were sticky
* [#13375](http://bugzilla.scilab.org/show_bug.cgi?id=13375): If (Max - Min) == 1, then the multiple selection is not allowed.
* [#13583](http://bugzilla.scilab.org/show_bug.cgi?id=13583): `getd` loading a script including a `clear` instruction yielded an error
* [#13597](http://bugzilla.scilab.org/show_bug.cgi?id=13597): `help format` claimed setting a number of digits instead of characters
* [#13613](http://bugzilla.scilab.org/show_bug.cgi?id=13613): `isdef(name, 'l')` produced wrong output.
* [#13757](http://bugzilla.scilab.org/show_bug.cgi?id=13757): The toolbox menu dit not load properly ATOMS modules not autoloaded.
* [#13990](http://bugzilla.scilab.org/show_bug.cgi?id=13990): `warning` with localization enabled some memory corruption
* [#14192](http://bugzilla.scilab.org/show_bug.cgi?id=14192): `g_margin` error-ed for double integrator.
* [#14367](http://bugzilla.scilab.org/show_bug.cgi?id=14367): `edit_curv` failed opening due to a `[]+1` operation.
* [#14395](http://bugzilla.scilab.org/show_bug.cgi?id=14395): `dir` displayed a []+".." warning when no subdirectory exists.
* [#14448](http://bugzilla.scilab.org/show_bug.cgi?id=14448): removed havewindow() was still documented
* [#14513](http://bugzilla.scilab.org/show_bug.cgi?id=14513): `isqual` comparing two built-in functions yielded an error.
* [#14527](http://bugzilla.scilab.org/show_bug.cgi?id=14527): Calling pathconvert function without parameters crashed Scilab.
* [#14557](http://bugzilla.scilab.org/show_bug.cgi?id=14557): `csim` failed when the system has no state.
* [#14582](http://bugzilla.scilab.org/show_bug.cgi?id=14582): `gettext`or it alias `_()` were sometimes applied to broken literal strings
* [#14586](http://bugzilla.scilab.org/show_bug.cgi?id=14586): Xcos simulation stop button did not work.
* [#14587](http://bugzilla.scilab.org/show_bug.cgi?id=14587): Datatip textbox wrong clipping when loaded from `*.scg` file.
* [#14590](http://bugzilla.scilab.org/show_bug.cgi?id=14590): Help pages in pt_BR directories had a wrong xml:lang="en" tag.
* [#14591](http://bugzilla.scilab.org/show_bug.cgi?id=14591): `<=` and `>=` elementwise operators comparing 2 hypermatrices of decimal numbers or encoded integers were inverted.
* [#14593](http://bugzilla.scilab.org/show_bug.cgi?id=14593): Signs are no more drawn in BIGSOM and PRODUCT components.
* [#14602](http://bugzilla.scilab.org/show_bug.cgi?id=14602): WRITEC_f block didn't work for x86 machines.
* [#14632](http://bugzilla.scilab.org/show_bug.cgi?id=14632): Zooming moves drawn axis offscreen
* [#14640](http://bugzilla.scilab.org/show_bug.cgi?id=14640): `median(int8([10 60 80 100]))` returned -58 instead of 70 due to overflow when interpolating (60+80)>128
* [#14648](http://bugzilla.scilab.org/show_bug.cgi?id=14648): `isinf` returned `%F` for complex numbers with both real and imag infinite parts.
* [#14649](http://bugzilla.scilab.org/show_bug.cgi?id=14649): `isnan(complex(%inf, %inf))` returned `%F` while the phase is `NaN`.
* [#14654](http://bugzilla.scilab.org/show_bug.cgi?id=14654): `bitor`, `bitxor` and `bitand` did not accept positive inputs of type `int8`, `int16`, `int32`, `int64` or `uint64`
* [#14662](http://bugzilla.scilab.org/show_bug.cgi?id=14662): Matrix of strings concatenation with single quote led to a parser error.
* [#14667](http://bugzilla.scilab.org/show_bug.cgi?id=14667): Multi line string without final quote generated a non terminal parser state.
* [#14681](http://bugzilla.scilab.org/show_bug.cgi?id=14681): Short-circuited AND operation was not possible with double matrices in if and while clauses
* [#14690](http://bugzilla.scilab.org/show_bug.cgi?id=14690): The user's startup files set in the working directory were not executed. When `SCIHOME` is not the working directory, `SCIHOME\scilab.ini` was executed twice.
* [#14692](http://bugzilla.scilab.org/show_bug.cgi?id=14692): isequal() was always returning true for builtin functions
* [#14694](http://bugzilla.scilab.org/show_bug.cgi?id=14694): The list of named colors was misaligned and poorly rendered in `help color_list`
* [#14710](http://bugzilla.scilab.org/show_bug.cgi?id=14710): fullpath(TMPDIR+...) was bugged on MacOS
* [#14714](http://bugzilla.scilab.org/show_bug.cgi?id=14714): Crash/Leak when deleting datatip
* [#14784](http://bugzilla.scilab.org/show_bug.cgi?id=14784): Setting field of graphics handle using children($) failed.
* [#14796](http://bugzilla.scilab.org/show_bug.cgi?id=14796): `ind2sub(dims, [])` returns [] in version 6. Warnings due to a `[]+1` operation have been removed.

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
* [#14099](http://bugzilla.scilab.org/show_bug.cgi?id=14099): `sci2exp` macro was fixed to avoid "a+[] Warning". string(polynomials|rationals) had badly formated outputs and was not vectorized
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
* [#14429](http://bugzilla.scilab.org/show_bug.cgi?id=14429): Rationals `r+(-r)` and `r-r` did not simplify the denominator to 1 in simp_mode(%t)
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
