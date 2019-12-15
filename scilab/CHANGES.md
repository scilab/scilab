Welcome to Scilab 6.1.X
=======================

This file details the changes between Scilab 6.1.X (this development branch), and the previous release 6.0.X.
For changelogs of earlier releases, please see [Scilab 6.0.0](https://help.scilab.org/docs/6.0.0/en_US/CHANGES.html).

This file is intended for the specific needs of advanced users, and describes:
- High-level new features,
- Changes in installation, compilation, and compatibility,
- Changes in the language,
- New and modified features, in each module,
- Changes in functions (removed/added/modified),
- Bug fixes.

This is an in-development version which might be unstable.
Please report any thing we could have missed, on the [mailing lists][1] or on the [bug tracker][2].

[1]: http://mailinglists.scilab.org
[2]: http://bugzilla.scilab.org


Main new features
-----------------

For a high-level description of the main new features of this release, please consult the [embedded help](modules/helptools/data/pages/homepage-en_US.html). It is also available as the "What's new" page of the help, by simply typing `help` in Scilab console.

In summary, the main new features are:
* Webtools utilities added for HTTP protocol, JSON data usage
* Profiled values are available as Scilab values
* Values have a cleaner display:
   - they are displayed in order on assignement
   - strings are quoted
   - complex numbers are aligned for matrices
   - numbers (IEEE 754 double) are not rounded

Installation
------------


Compilation
-----------

* GNU autotools have been updated to :
   - automake 1.15
   - autoconf 2.69
   - libtool 2.4.6 (patched for macOS)

* Linux/macOS configures check if `-std=c++11` is available and use it by default. Toolboxes might increment the standard C++ version by specifying the C++ version as a `CFLAGS`.

* Migration to Microsoft Visual Studio 2017 and Intel Composer 2018


Dependencies
------------


Packaging & Supported Operating Systems
---------------------------------------

* Scilab embedded JVM is Java 8. To run or compile Scilab you need at least:
  - Windows:
     - Windows 10 (Desktop)
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


Feature changes and additions
-----------------------------

* Empty strings are used as the default values on String allocation
* HTTP get, post, put, upload, patch, delete functions added
* JSON encoding / decoding for Scilab datatypes added
* Memory invalid accesses have been greatly reduced thanks to :
  - PVS-Studio inspections blog report
  - Coverity scan weekly source analysis
* bitget() is upgraded:
  - It now accepts positive Signed encoded integers.
  - It now supports the new uint64 and int64 types of encoded integers.
  - For decimal numbers: bits with indices > 52 can now be retrieved (up to `log2(number_properties("huge"))` = 1024).
  - For decimal numbers `x > 2^52`, querried bits below `%eps` (indices < log2(x)-52) now return `Nan` instead of 0.
  - Several bits can now be retrieved from each component of an input array.
* `edit` now accepts a line number as text (like "23").
* `profileEnable`, `profileDisable`, `profileGetInfo` could be used to instrument functions and gather execution information within Scilab.
* `prettyprint()` is upgraded:
  - Integer and Text input are now actually supported.
  - Default input arguments can be skipped instead of still having to be provided.
  - The result string is better formatted to be easily wrappable and indentable.
* `mesh2d` has been introduced to compute a 2d mesh from vectors (x,y) of points.
* `loadmatfile()` is upgraded:
  - Octave native text data files can now be read.
  - Automatic detection of the input data format implemented.
  - `-toStruct` option added, to return results in a structure instead of in the calling environment.
* `jremove` is optional and `clear` could be used to release Java objects mapped to Scilab.
* `blockdiag()` is introduced to improve, replace, and extend `sysdiag()` to strings.
* `assert_checkequal()` is upgraded:
  - support added for Nan and void in lists, Nan in sparses, implicit lists, graphic handles, macros, libraries, and built-in.
  - For arrays of matching typeof and sizes, and for lists of matching lengths, the failure message now indicates the index and values of the first mismatching elements.
* `diophant()` is improved: Now,
  - Encoded integers are accepted, in addition to decimal numbers. Only integer solutions are then searched.
  - When there is no solution, [] is returned.
  - When there is an infinite number of solutions, the err flag has a specific value.
  - The output size gets the input's one.


Help pages:
-----------

* overhauled / rewritten: `bitget`, `edit`
* fixed / improved:  `bench_run` `M_SWITCH`

User Interface improvements:
----------------------------

* The `ans` variable is editable as any other variable
* Commands history is saved *before* executing a command to have the correct history on crash.
* Used memory per variable is displayed by BrowserVar to give the user numbers on memory usage repartition and let the user `clear` the big ones first.

Xcos
----

* Default ending time reduced from 100000 to 30, to fit default scope block


API modification
----------------


Obsolete functions or features
------------------------------
* `frexp` becomes an internal. Please use `[m,e]=log2(x)` instead.


Removed Functions
-----------------

* `hypermat` was obsolete and has been removed. Please use `matrix` instead.
* `square` was obsolete and has been removed.
* `xgetech` was obsolete and has been removed. Please use `gca` instead.


Known issues
------------


Bug Fixes
---------

### Bugs fixed in 6.1.0:
* [#2694](http://bugzilla.scilab.org/show_bug.cgi?id=2694): `bitget` did not accept positive integers of types int8, int16 or int32.
* [#7724](http://bugzilla.scilab.org/show_bug.cgi?id=7724): When a figure is created in .auto_resize="on" mode, its .axes_size sets its .figure_size accordingly, not the reverse. But this was not documented.
* [#8784](http://bugzilla.scilab.org/show_bug.cgi?id=8784): Automatic self-adjusting blocks `SCALE_CSCOPE` & `SCALE_CMSCOPE` in Xcos.
* [#5512](http://bugzilla.scilab.org/show_bug.cgi?id=5512): `disp()` puzzlingly displayed arguments in reverse order.
* [#9529](http://bugzilla.scilab.org/show_bug.cgi?id=9529): `assert_checkequal(list(1,,3), list(1,,3))` yielded an error.
* [#9673](http://bugzilla.scilab.org/show_bug.cgi?id=9673): Priority of colon `:` operator was too low
* [#10353](http://bugzilla.scilab.org/show_bug.cgi?id=10353): The referential for the uicontrol.position property was not provided.
* [#12520](http://bugzilla.scilab.org/show_bug.cgi?id=12520): Vriable browser did not display the size of the variables.
* [#12534](http://bugzilla.scilab.org/show_bug.cgi?id=12534): Variable browser did not display the size of the variables.
* [#13766](http://bugzilla.scilab.org/show_bug.cgi?id=13766): Minimal values for `.figure_size` were not documented.
* [#14498](http://bugzilla.scilab.org/show_bug.cgi?id=14498): `size([],3)` returned 1 instead of 0.
* [#14557](http://bugzilla.scilab.org/show_bug.cgi?id=14557): `csim` failed when the system has no state.
* [#14498](http://bugzilla.scilab.org/show_bug.cgi?id=14498): `size([],3)` returned 1 instead of 0.
* [#14604](http://bugzilla.scilab.org/show_bug.cgi?id=14604): `emptystr()` is 40x slower with 6.0.0 wrt 5.5.2
* [#14605](http://bugzilla.scilab.org/show_bug.cgi?id=14605): fixed - `bench_run` was too strict about the specification of tests names.
* [#14606](http://bugzilla.scilab.org/show_bug.cgi?id=14606): Memory used by variables returned by `[names,mem]=who()` was always zero.
* [#14741](http://bugzilla.scilab.org/show_bug.cgi?id=14741): The syntax `[m,e]=log2(x)` was not documented. As public function `frexp()` was in duplicate with `[m,e]=log2(x)`.
* [#14812](http://bugzilla.scilab.org/show_bug.cgi?id=14812): Minor typos in messages.
* [#14863](http://bugzilla.scilab.org/show_bug.cgi?id=14863): In Xcos, the default ending time was unhandily high (100000), reduced it to 30.
* [#14982](http://bugzilla.scilab.org/show_bug.cgi?id=14982): `msprintf` segmentation fault was caught due to wrong size
* [#15087](http://bugzilla.scilab.org/show_bug.cgi?id=15087): Deleting rows or columns from a matrix is slow (regression)
* [#15269](http://bugzilla.scilab.org/show_bug.cgi?id=15269): `xgetech` was poor and stiff compared to any combination of `gca()` properties `.axes_bounds`, `.data_bounds`, `.log_flags`, and `.margins`. It is removed.
* [#15271](http://bugzilla.scilab.org/show_bug.cgi?id=15271): `bitget` needed to be upgraded.
* [#15321](http://bugzilla.scilab.org/show_bug.cgi?id=15321): `lu()` was leaking memory.
* [#15425](http://bugzilla.scilab.org/show_bug.cgi?id=15425): The Kronecker product `a.*.b` failed when `a` or `b` or both are hypermatrices, with one or both being polynomials or rationals.
* [#15523](http://bugzilla.scilab.org/show_bug.cgi?id=15523): `%ODEOPTIONS(1)=2` didn't work with solvers 'rk' and 'rkf'
* [#15248](http://bugzilla.scilab.org/show_bug.cgi?id=15248): `lsq()`was leaking memory.
* [#15577](http://bugzilla.scilab.org/show_bug.cgi?id=15577): `edit` did not accept a line number as text, as with `edit linspace 21`.
* [#15668](http://bugzilla.scilab.org/show_bug.cgi?id=15668): `save(filename)` saved all predefined Scilab constants %e %pi etc.. (regression)
* [#15715](http://bugzilla.scilab.org/show_bug.cgi?id=15715): `%nan` indices crashed Scilab.
* [#15581](http://bugzilla.scilab.org/show_bug.cgi?id=15581): display of complex matrix was ugly.
* [#15680](http://bugzilla.scilab.org/show_bug.cgi?id=15680): `loadmatfile()` could not return variables in a structure instead of into the calling environment.
* [#15744](http://bugzilla.scilab.org/show_bug.cgi?id=15744): `sylm(a,b)` yielded an error when degree(a)==0 or degree(b)==0.
* [#15745](http://bugzilla.scilab.org/show_bug.cgi?id=15745): `diophant(0,0,m)`, `diophant([p 0],q)`, `diophant([0 p],q)` with m<>0 and p>q were wrong. There was no flag for cases with an infinite number of solutions. When there is no solution, some values were returned anyway, instead of []. In this case, the documented definition of the err value was dubious. Decimal numbers and integers were accepted, but not encoded integers. Inf and NaN input coefficients were not rejected.
* [#15812](http://bugzilla.scilab.org/show_bug.cgi?id=15812): On assigning variables the source variable may become become corrupted
* [#15840](http://bugzilla.scilab.org/show_bug.cgi?id=15840): `grand(1,"prm",m)` yielded an unsqueezed size([size(m) 1]) hypermatrix
* [#15964](http://bugzilla.scilab.org/show_bug.cgi?id=15954): A complex empty sparse matrix could be obtained after insertion.
* [#15983](http://bugzilla.scilab.org/show_bug.cgi?id=15983): `group()` regressed in 5.5.2 due to a too intrusive fix.
* [#15984](http://bugzilla.scilab.org/show_bug.cgi?id=15984): display scale was wrong with Retina dispplays on OSX..
* [#15995](http://bugzilla.scilab.org/show_bug.cgi?id=15995): patch was missing in surface plot (regression)
* [#16003](http://bugzilla.scilab.org/show_bug.cgi?id=16003): Zoom with mouse scroll wheel was broken on simple surfaces.
* [#16005](http://bugzilla.scilab.org/show_bug.cgi?id=16005): The `intdec` example was biased and not robust when changing sampling frequencies.
* [#16007](http://bugzilla.scilab.org/show_bug.cgi?id=16007): Non-integer index in sparse makes Scilab crash.
* [#16012](http://bugzilla.scilab.org/show_bug.cgi?id=16012): `[struct() struct()]` crashed Scilab.
* [#16013](http://bugzilla.scilab.org/show_bug.cgi?id=16013): Load previously saved environment with "File/Load environment" menu failed.
* [#16014](http://bugzilla.scilab.org/show_bug.cgi?id=16014): after `x.a=1; x(:)=[]` x.a was an empty list.
* [#16015](http://bugzilla.scilab.org/show_bug.cgi?id=116015): `intg(a,b,f)` called f(x) with x outside [a,b].
* [#16021](http://bugzilla.scilab.org/show_bug.cgi?id=16021): `tand([-90 90])` answered [Nan Nan] instead of [-Inf, Inf]. `cotd([-90 90])` answered [Nan Nan] instead of [0 0]. `1 ./cosd([-90 90])` answered [Inf -Inf] instead of [Inf Inf].
* [#16067](http://bugzilla.scilab.org/show_bug.cgi?id=16067): The display of matrices of signed integers was misaligned (regression of 6.0.0 vs 5.5.2)
* [#16071](http://bugzilla.scilab.org/show_bug.cgi?id=16071): `prettyprint(complex(1,%nan))` omitted the "+" in `1 + Nani`. It printed positive exponents with a useless "+". For any input nul polynomial, the string result never included the name of the variable. Default input arguments could not be skipped. ExportFormat was uselessly case-sensitive. For tex|latex: for text input, $ \ % & { } ~ and ^ special characters were not protected ; spaces were not protected, all words were concatenated ; for polynomials and rationals, the result string could be extremely long and not easily wrappable. For MathML: "<" was not protected ; <mi></mi> were missing for text input ; <mtable>, </mtable>, <mtr>, </mtr>, <mtd>, <mfenced> and </mfenced> tags were not wrapped and could not be indented. Delimiters: "" was not documented as possible value ; ")" was wrongly documented. Dynamical linear systems were not documented as possible input.
* [#16072](http://bugzilla.scilab.org/show_bug.cgi?id=16072): `prettyprint()` did not actually support input encoded integers.
* [#16075](http://bugzilla.scilab.org/show_bug.cgi?id=16075): `prettyprint()` was broken for cells.
* [#16085](http://bugzilla.scilab.org/show_bug.cgi?id=16085): insertion in an empty struct was broken.
* [#16087](http://bugzilla.scilab.org/show_bug.cgi?id=16087): Insertion of struct() in a non-empty struct crashed Scilab.
* [#16104](http://bugzilla.scilab.org/show_bug.cgi?id=16104): `assert_checkequal()` did not support properly or at all Nan and void in lists, Nan in sparse, implicit lists, macros, libraries, built-in functions, graphical handles. For input arrays or lists of matching typeof and sizes, the failure message did not display neither the index nor the values of the first mismatching elements.
* [#16111](http://bugzilla.scilab.org/show_bug.cgi?id=16111): `isglobal` was not supporting non-scalar array of strings as input.
* [#16118](http://bugzilla.scilab.org/show_bug.cgi?id=16118): `%s <> (1+%s)` returned %F.
* [#16139](http://bugzilla.scilab.org/show_bug.cgi?id=16139): `auread()` and `auwrite()` kept the sound file open and locked when returning on errors. They poorly handled the default .au sound file extension.
* [#16144](http://bugzilla.scilab.org/show_bug.cgi?id=16144): Addition of sparse matrices gave incorrect results.
* [#16152](http://bugzilla.scilab.org/show_bug.cgi?id=16152): For sparse or boolean sparse matrix `s`, `s([])` returned `[]` instead of `sparse([])`.
* [#16158](http://bugzilla.scilab.org/show_bug.cgi?id=16158): When a multicolumn array of rationals was displayed wide column per column, columns #2:$ were replaced with its column #2.
* [#16174](http://bugzilla.scilab.org/show_bug.cgi?id=16174): `libraryinfo` yielded 0x0 matrix of strings for libs without macro
* [#16208](http://bugzilla.scilab.org/show_bug.cgi?id=16208): Using 3D string matrix with old C-api gateways may crash Scilab.
* [#16209](http://bugzilla.scilab.org/show_bug.cgi?id=16209): grand() causes a freeze after several consecutive calls when using default base generator.
* [#16210](http://bugzilla.scilab.org/show_bug.cgi?id=16210): The uicontrol.units = "normalized" property was not described.
* [#16242](http://bugzilla.scilab.org/show_bug.cgi?id=16242): `loadmatfile()` could not read Octave native text data files.
* [#16257](http://bugzilla.scilab.org/show_bug.cgi?id=16257): `blockdiag()` implemented to replace `sysdiag()`, improved and extended to strings.
* [#16263](http://bugzilla.scilab.org/show_bug.cgi?id=16263): Polynomial insertion was broken for complex case.
* [#16264](http://bugzilla.scilab.org/show_bug.cgi?id=16264): After empty for loop iterator was left uninitialized.
* [#16265](http://bugzilla.scilab.org/show_bug.cgi?id=16265): The translated pages of the `warning` documentation were not up-to-date.
* [#16271](http://bugzilla.scilab.org/show_bug.cgi?id=16271): `loadmatfile()` was never able to automatically detect the input data format.
* [#16272](http://bugzilla.scilab.org/show_bug.cgi?id=16272): `spzeros(0,n)` and `spzeros(n,0)` were different from `sparse(0,0)`.
* [#16275](http://bugzilla.scilab.org/show_bug.cgi?id=16275): `fsolve(x0, fun, tol)` no longer took `tol` into account.


