Welcome to Scilab 6.2.0
=======================

This file details the changes between Scilab 6.2.0 (this version), and the previous release 6.1.0.
For changelogs of earlier releases, please see [Scilab 6.1.0][1].

This file is intended for the specific needs of advanced users, and describes:
- High-level new features,
- Changes in installation, compilation, and compatibility,
- Changes in the language,
- New and modified features, in each module,
- Changes in functions (removed/added/modified),
- Bug fixes.

Please report anything we could have missed, on the [mailing lists][2] or the [bug tracker][3].

[1]: https://help.scilab.org/docs/6.1.0/en_US/CHANGES.html
[2]: http://mailinglists.scilab.org
[3]: https://bugzilla.scilab.org


Main new features
-----------------

For a high-level description of the main new features of this release, please consult the [embedded help][4]. It is also available as the "What's new" page of the help, by simply typing `help` in Scilab console.

[4]: modules/helptools/data/pages/homepage-en_US.html

In summary, the main new features are:
* The default Java Heap has been increased to 512MB
* Webtools utilities added for HTTP protocol, JSON data usage
   - http_get can be used as an alternative download method for ATOMS
* Profiled values are available as Scilab values
* Values have a cleaner display:
   - they are displayed in order on assignment
   - strings are quoted
   - complex numbers are aligned for matrices
   - numbers (IEEE 754 double) are not rounded
   - polynomials and rationals display is more compact
   - In structures, nested lists or structures are now displayed recursively compactly and more explicitly.
* `uiSpreadsheet` function has been added to easily interface csvRead
* function calls with zero output arguments can be detected with `argn`
   - atoms functions can either display or output values


Installation
------------


Compilation
-----------

* GNU autotools have been updated to :
   - automake (GNU automake) 1.16.1
   - autoconf (GNU Autoconf) 2.69
   - libtool (GNU libtool) 2.4.6 (patched for macOS)

* Linux and macOS builds require a C++17 compiler. Toolboxes might also want to use this standard by specifying `-std=c++17` as a `CFLAGS`

* Migration to Microsoft Visual Studio 2017 and Intel Composer 2018


Dependencies
------------

* Lucene has been updated to 8.4.0; API compatibility is preserved down to 5.2.1.

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

    For more information, please consult: [What are the system requirements for Java?][5]

* [SSE2][6], Streaming SIMD Extensions 2 support is now mandatory to run Scilab on Linux i686.

[5]: http://java.com/en/download/help/sysreq.xml
[6]: https://en.wikipedia.org/wiki/SSE2

Feature changes and additions on 6.1.0
--------------------------------------

* `airy` has been added: Evaluation of Airy functions of the first and second kind, and their first derivative, possibly scaled.
* HTTP get, post, put, upload, patch, delete functions added
* JSON encoding / decoding for Scilab datatypes added
* Memory invalid accesses have been greatly reduced thanks to :
  - PVS-Studio inspections blog report
  - Coverity scan weekly source analysis
* `bitget` is upgraded:
  - It now accepts positive Signed encoded integers.
  - It now supports the new uint64 and int64 types of encoded integers.
  - For decimal numbers: bits with indices > 52 can now be retrieved (up to `log2(number_properties("huge"))` = 1024).
  - For decimal numbers `x > 2^52`, queried bits below `%eps` (indices < log2(x)-52) now return `Nan` instead of 0.
  - Several bits can now be retrieved from each component of an input array.
* `bitset` is upgraded:
  - It now accepts positive Signed encoded integers.
  - It now supports the new uint64 and int64 types of encoded integers.
  - For decimal numbers: bits with indices > 32 can now be set (up to #1024).
  - Several bits can now be set for each input component.
  - Distributive scalar inputs are now accepted.
* `bitstring` function added.
* `edit` now accepts a line number as text (like "23").
* `profileEnable`, `profileDisable`, `profileGetInfo` could be used to instrument functions and gather execution information within Scilab.
* `prettyprint` is upgraded:
  - Integer and Text input are now actually supported.
  - HTML is now supported as output format.
  - Default input arguments can be skipped instead of still having to be provided.
  - The result string is better formatted to be easily wrappable and indented.
* `mesh2d` has been introduced to compute a 2d mesh from vectors (x,y) of points.
* `loadmatfile` is upgraded:
  - Octave native text data files can now be read.
  - Automatic detection of the input data format implemented.
  - `-toStruct` option added, to return results in a structure instead of in the calling environment.
* `jremove` is optional and `clear` could be used to release Java objects mapped to Scilab.
* `blockdiag` is introduced to improve, replace, and extend `sysdiag` to strings.
* `assert_checkequal` is upgraded:
  - support added for Nan and void in lists, Nan in sparses, implicit lists, graphic handles, macros, libraries, and built-in.
  - For arrays of matching typeof and sizes, and for lists of matching lengths, the failure message now indicates the index and values of the first mismatching elements.
* `diophant` is improved:
  - Encoded integers are accepted, in addition to decimal numbers. Only integer solutions are then searched.
  - When there is no solution, `[]` is returned.
  - When there is an infinite number of solutions, the err flag has a specific value.
  - The output size gets the input's size.
* `csvRead` and `csvTextScan` are now implemented without extra copies.
* `editvar` GUI support copy-paste of strings removing quotes.
* `calendar` can now display formatted calendars.
* `xmlSetValues` clones `setPreferencesValue` that is now obsolete.
* `unique` is upgraded to:
  - return the number of occurrences of distinct entities found in the input array.
  - return distinct entities in their initial order (rather than sorted), with the `"keepOrder"` option.
  - consider all `Nan` values as the same one, with the `"uniqueNan"` option.
* `ellipj` function introduced, to compute the `sn`, `cn`, `dn`, `ns`, `nc` and `nd` Jacobi elliptic functions.
* `riccati` is upgraded:
  - `riccati(H)` and `riccati(H,E)` syntaxes added, to describe the Riccati equation through its Hamiltonian H or (E,H) pencil.
  - The residual is returned as a new 3rd output argument.
  - When no solution is found, `X=[]` | `X1=[]` is returned instead of yielding an error.
* `hallchart` is upgraded:
  - The default set of modules is set automatically, according to data bounds.
  - Handles of the grids and labels are returned. Post-processing them becomes easy.
  - Named colors accepted; better default colors.
  - Improved placement of labels.
* `playsnd` is upgraded:
  - the name of a wav file can now be specified (instead of the sound data).
  - the input sound can now be repeated a given number of times.
  - the modal/non-modal processing is improved.
* `nicholschart` is improved: more neutral default frame color; improved labels positionning; colors can now be specified by their predefined name or "#RRGGBB" hexa code; a structure of handles is now returned to easily postprocess both subframes and the set of labels.
* `sciargs` returns a column instead of formerly a row.
* Booleans and encoded integers can now be concatenated together, as in `[%f int8(-5)]`.
* `gsort` can now perform multilevel sorting. This noticeably allows sorting completely complex numbers.
* `factorial` can be used now from n=171 up to n=10^14.
* `intersect` now supports complex numbers.
* `setdiff` now supports complex numbers.
* `twinkle` can now blink together several hierarchically independent objects, like a curve and its labels, etc.
* `repmat` has been rewritten. It is 7 times faster now.
* `plot` can now work in semi or bi-logarithmic mode with some input logflag.
* `plotimplicit` function added.
* The default smarter grid_style value is now 7 instead of 3.
* `weekday` accepts new date formats and new syntaxes. A language option applying to the day names is now available.
* `vectorfind` is upgraded:
  - vectorfind(m, []) now returns [] instead of an error.
  - A too long needle now returns [] instead of an error.
  - %nan values never matched. They are now regularly processed, as other values.
  - A needle shorter than the haystack size can now be used.
  - A wildcard value matching any value of the haystack can now be specified and used in the needle. Actual matching ranges can then be returned: Options `joker` and `matching` added.
  - Hypermatrices are now accepted as haystack.
  - The probing direction can now be numeric: 1, 2, ..
  - Returned indices can now be formatted with the new option `indType`.
  - There were no unit tests. More than 100 tests are added.
* `datafit` is now able to fit weighted data. It now supports any gap function vectorized for Data points, and so is much faster. It now accepts any matrix of parameters (not necessarily a colum). It now returns the average Mode-to-data distance, and the termination status for the quasi-Newton algo.
* `tree_show` is upgraded:
  - New `rootTitle` and `styles` optional inputs.
  - New `arrayByFields` option, to allow displaying each array component as an object in its whole.
  - Improved layout: detailled indices for 2D arrays, simplified symbols, etc.
  - The content of implicitlist objects, and information for Scilab functions and libraries of functions are now displayed.
* `nchoosek` is introduced, to compute the binomial coefficients.
* The left .\. and right ./. Kronecker divisions are now implemented, for arrays of decimal or complex numbers.
* `perms` can now build and return only unique permutations, without duplicates.
* Most of graphic objects can be moved interactively in opened figures.
* `circshift` is introduced.
* `atomsGetInstalledPath` is no longer sensitive to the case or completeness of the modules names. Providing the modules versions is now optional.
* `function` replaces `mc` as the new overloading code for functions in Scilab language.
* `mgeti` can now read correctly 64-bit int64 / uint64 integers bigger than 2^52.


Feature changes and additions on 6.1.1
--------------------------------------

* Axes labels and title default font size had been increased for better default rendering.
* `gsort` is upgraded:
  - It can now sort any sparse 2D matrix, in all `g, r, c, lr, lc` methods, including sparse booleans and in multi-level mode. It was formerly limited to sparse real or complex vectors and only to the `g` mode.
  - Any hypermatrix can be sorted along a dimension > 2.
* `unique` is enabled for any 2D sparse arrays, in simple, 'c' and 'r' modes.
* `%chars` constant added, to easily access to some selected sets of unicode symbols.
* Lists are displayed in a more compact and comprehensive way.
* `interp1` is upgraded:
  - complex numbers `y` now supported: the real and imaginary parts are interpolated separately.
  - extrapolation option extended: `edgevalue` mode added for all interpolations; `periodic` mode added for linear and spline interpolations. `linear` mode added for the spline interpolations.
* `xlfont` did not support the documented 4th argument.
* `bitcmp` is upgraded:
  - Entended to 64-bit integers.
  - Extended to all signed integers.
  - Decimal positive integers > 2^52 up to 2^1024 = number_properties("huge") can now be processed, with bitnum up to 1024 instead of 52.
  - bitnum can actually be an array. It is now optional as well for input decimal integers.
* The `Arnoldi` module is now internal.
* `sgolay` and the companion `sgolayfilter` and `sgolaydiff` functions have been added to implement Savitsky-Golay filters.
* `clock` now returns the milliseconds, the time zone, and the daylight saving time.
* `mapsound` upgraded to have a colormap argument
* `mprintf`, `msprintf` and `mfprintf` can now print input booleans, as `0`|`1` or as `T`|`F`.
* `setdiff` now supports input booleans and sparse matrices (boolean or numeric).
* `intersect` is extended to any sparse boolean or numeric arrays, in all simple, 'c' or 'r' modes.
* `union` now support boolean, sparse boolean, and sparse numerical matrices.
* `kron` and `.*.` are extended to boolean arrays.
* `gamma` is extended to incomplete gamma integrals.
* `close` is extended to close the help browser, xcos, or the variables browser or editor GUIs.
* `polyint` is introduced to compute polynomial antiderivatives.
* `det` is now actually extended to sparse matrices.
* `deff` is upgraded:
   - The created function may be returned  as output argument.
   - Providing the function headline and body concatenated in a single input text vector is possible.
   - For a simple function, a single string input definition can be provided.
   - For assignable calls to `deff`, the `@` symbol can be used as function pseudo-name.
* `mopen` and `mclose` now handle multiple files and file descriptors.
* `hash` function with SHA-1,SHA-2 and SHA-3 Secure Hash Algorithms has been added.
* `loglog`, `semilogx`, `semilogy` Matlab-compatible functions added.
* `scatter` and `scatter3d` are reforged. New `scatter` options: `"smallOnTop"` and `"datatips"`.
* `asciimat` now supports input strings of different lengths or including international UTF8 extended characters.


Help pages:
-----------

* overhauled / rewritten: `bitget`, `edit`, `factorial`, `fft`, `vectorfind`, `datafit`, `mgeti`
* fixed / improved:  `bench_run` `M_SWITCH`, `comet`, `comet3d`, `plot2d`
* Rewritten: `getdate`, `weekday`
* Translations added:
  - (ru): `weekday`


User Interface improvements
---------------------------

* The `ans` variable is editable as any other variable in Editvar.
* Commands history is saved before executing a command to have the correct history on a crash.
* Used memory per variable is displayed by BrowserVar to give the user numbers on memory usage repartition and let the user `clear` the big ones first.
* In browsevar, clicking on any function, library, list, cell, structure, custom tlist or mlist, or graphic handle now edits or displays their content.
* Autoscroll of console in GUI mode is disabled when the user scrolls up until he scrolls to the bottom.
* Help browser improved:
  - The browser's language can be set independently of the session's language
  - The menu "Online" allows to display in your web browser the online version of the current page
  - The menu "Issues" allows to display bugzilla entries related to the feature of the current page
  - ATOMS chapters are automatically loaded in the browser.
  - The page left at exit is restored at next startup.
  - On Preferences option, querring some Matlab term can now redirect and display the documentation of the Scilab equivalent feature.
* `x_matrix` can now edit matrices of booleans, integers, or text. Matrices of real or complex numbers are better displayed.


Xcos
----

* Default ending time reduced from 100000 to 30, to fit default scope block.
* `CURVE_c` settings did not always display a window.
* Exporting all layers from a diagram will generate one unique file per layer.

API/ABI modification
----------------

* Functions could return zeros arguments, the following gateway functions have been updated:
 - on APIv5 C `CheckOutputArgument(ctx, min, max)` the `min` argument should be set to `0` if needed.
 - on APIv5 C `getNbOutputArgument(ctx)` the return value could be `0`
 - on APIv6 C++ the `_iRetCount` argument value could be `0`.
* On APIv6 C++, a virtual method has been added on internal types which break the ABI. Toolboxes need to be recompiled.

Obsolete functions or features
------------------------------
* `frexp` becomes an internal. Please use `[m,e]=log2(x)` instead.
* `champ1` is obsolete. Please use `champ.colored` instead.
* `setPreferencesValue` is obsolete. Please use `xmlSetValues` instead.
* `%sn` is obsolete. Please use `ellipj` instead.
* `sysdiag` is obsolete. Please use `blockdiag` instead.
* `ric_desc` is obsolete and will be removed from Scilab 6.1.x. Please use `riccati` instead.
* `noisegen` will be removed from Scilab 6.1.x. Please use `grand` instead.
* `get_figure_handle(n)` is obsolete. Please use `findobj('figure_id',n)` instead.
* `hist3d` is renamed `bar3d`. The former `hist3d` function is declared obsolete and will be replaced by an actual 3D histogram function in Scilab 6.2.
* `scatter3` is obsolete. Please use `scatter3d` instead.
* `saveafterncommands` is canceled. It will be removed from Scilab 6.1.x
* `xname` is obsolete. It will be removed from Scilab 6.2.x. Please use `gcf().figure_name` instead.
* `soundsec` is obsolete. It will be removed from Scilab 6.2.x. Please use `0 : 1/freq : t*(1-%eps)` instead.

Removed Functions
-----------------

* `getPreferencesValue` was obsolete and has been removed. Please use `xmlGetValues` instead.
* `eval` was obsolete and has been removed. Please use `evstr` instead.
* `hypermat` was obsolete and has been removed. Please use `matrix` instead.
* `lstsize` was obsolete and has been removed. Please use `size` instead.
* `denom` was obsolete and has been removed. Please use the `.den` rational field instead.
* `numer` was obsolete and has been removed. Please use the `.num` rational field instead.
* `square` was obsolete and has been removed.
* `strcmpi` was obsolete and has been removed. Please use `strcmp(,'i')` instead.
* `xgetech` was obsolete and has been removed. Please use `gca` instead.
* `fac3d` was obsolete since Scilab 4 and has been removed. Please use `plot3d` instead.
* `fac3d1` was obsolete since Scilab 4 and has been removed. Please use `plot3d1` instead.
* `xinfo` was obsolete and has been removed. Please use `gcf().info_message` instead.
* `with_tk` was obsolete and has been removed. Please use `with_module('tclsci')` instead.


Known issues
------------


Bug Fixes
---------

### Bugs fixed in 6.2.0:


### Bugs fixed in 6.1.1:

* [#3188](https://bugzilla.scilab.org/3188): `part()` was slower than in Scilab 4.1.2.
* [#4648](https://bugzilla.scilab.org/4648): Scilab missed more secure hashing functions (md5 is now broken)
* [#5511](https://bugzilla.scilab.org/5511): printf_conversion page was poorly presented and had many issues: The described types of accepted value and printed result were often switched; nothing was told about the processing of complex numbers; special escaped sequences \n \r \t \\ were not described; possible numbering of placeholders was not described; in the pt_BR version, the itemized lists were wrongly unnested and characters typing placeholders (d,u,o,x,f,e,g..) were missing.
* [#7202](https://bugzilla.scilab.org/7202): It was not possible to send string parameters via the opar structure of an xcos block to a block of type 4.
* [#8059](https://bugzilla.scilab.org/8059): A local `.wgetrc` config file could make troubles in `atomsDownload`.
* [#8100](https://bugzilla.scilab.org/8100): `cumsum()` on sparse documented.
* [#8378](https://bugzilla.scilab.org/8378): Datatip `ContextMenu => Delete last datatip` was useless.
* [#8761](https://bugzilla.scilab.org/8761): Xcos masked superblocks had invalid names.
* [#9221](https://bugzilla.scilab.org/9221): There was no way in Scilab to easily access to sets of unicode symbols like greek letters, etc.
* [#9909](https://bugzilla.scilab.org/9909): In the help browser, add a way to open the online version of the current page.
* [#10046](https://bugzilla.scilab.org/10046): By default, the `MAXMIN` block did not propose 2 input ports to work in an element-wise way (unlike MIN_f and MAX_f), which is its most original working mode.
* [#10465](https://bugzilla.scilab.org/10465): At Scilab exit, the help browser is not saved nor restored.
* [#10476](https://bugzilla.scilab.org/10476): From `browsevar`, displaying the content of lists, structures, cells, or other custom tlists or mlists was not possible.
* [#10490](https://bugzilla.scilab.org/10490): The `mapsound` page was poor with a single interesting example.
* [#10964](https://bugzilla.scilab.org/10964): Scinotes coud not execute the current file in NW mode.
* [#11600](https://bugzilla.scilab.org/11600): `rand()` was parsing its inputs incorrectly.
* [#11677](https://bugzilla.scilab.org/11677): The original Arnoldi functions were obsolete.
* [#11888](https://bugzilla.scilab.org/11888): The `struct` page did not document how to build an array of structures.
* [#12418](https://bugzilla.scilab.org/12418): Using bvode() with "continuation", i.e. `ipar(9) > 1` led to an error.
* [#12516](https://bugzilla.scilab.org/12516): From `browsevar`, clicking on any graphical handle did not edit its figure with `ged`.
* [#12532](https://bugzilla.scilab.org/12532): From `browsevar`, clicking on any function did not edit it with `edit`. The content of libraries could not be displayed either.
* [#12719](https://bugzilla.scilab.org/12719): `A(%s)` gave the same result as `A($)`.
* [#12889](https://bugzilla.scilab.org/12889): In the help browser, add a menu allowing to select the language of help pages, regardless of the language of the session.
* [#13303](https://bugzilla.scilab.org/13303): `mprintf`, `msprintf` and `mfprintf` could not print input booleans.
* [#13417](https://bugzilla.scilab.org/13417): `csvRead` page did not document the way to use the `range` up to the last row/column.
* [#13593](https://bugzilla.scilab.org/13593): `csvRead()` did not take the `range` into account when `header` is provided. `[]` could not be used as default `range`.
* [#13762](https://bugzilla.scilab.org/13762): In the `fft` page, the formula for the inverse FFT missed the 1/n normalization factor.
* [#13855](https://bugzilla.scilab.org/13855): The documentation of `pause` was not up-to-date and could be improved.
* [#13985](https://bugzilla.scilab.org/13985): The display of lists was very loose and poor.
* [#14033](https://bugzilla.scilab.org/14033): `x_matrix` could not edit matrices of booleans, encoded integers or text. Matrices of real or complex numbers were poorly displayed.
* [#14098](https://bugzilla.scilab.org/14098): The `genlib` and `library` help pages were outdated with respect to Scilab 6.
* [#14297](https://bugzilla.scilab.org/14297): Documentation of `cumsum()` improved.
* [#14435](https://bugzilla.scilab.org/14435): Errors were not well handled in overloaded functions.
* [#14477](https://bugzilla.scilab.org/14477): `mtlb_sparse()` removed from the documentation.
* [#14488](https://bugzilla.scilab.org/14488): The `frameflag=9` and `strf=".9."` values of these `plot2d` options were no longer accepted. Their documentation was ambiguous.
* [#14567](https://bugzilla.scilab.org/14567): In Scinotes, the `TODO` keyword was no longer highlighted.
* [#14718](https://bugzilla.scilab.org/14718): `user` is removed for a while but was still documented.
* [#14873](https://bugzilla.scilab.org/14873): `setfield` page: The output and the 6.0 history were documented only on the en_US version. The input was wrongly restricted to matrices, while any Scilab object is acceptable. The specific role of `setfield` for mlists was not really described nor illustrated. The example did not include any call to setfield.
* [#15012](https://bugzilla.scilab.org/15012): `covStart(["isempty" ; "isscalar"])` crashed Scilab.
* [#15163](https://bugzilla.scilab.org/15163): `getdate` page: The time referential was obscure: a) UTC for Unix Time Convention vs Coordinated Universal Time. b) unclear influence of the time zone.
* [#15226](https://bugzilla.scilab.org/15226): The `get` page needed to be overhauled: `get(0)`, `get(0,prop)`, `get(tag)`, `get(tag,prop)` and other features were not documented.
* [#15280](https://bugzilla.scilab.org/15280): `gsort` was unable to sort any hypermatrix along dimensions > "r"|"c".
* [#15330](https://bugzilla.scilab.org/15330): spec.tst was crashing on Linux.
* [#15839](https://bugzilla.scilab.org/15839): `gsort`: the only sparse possible input were real or complex vectors, and only with the `g` method.
* [#15841](https://bugzilla.scilab.org/15841): `intersect` did not support neither sparse boolean nor sparse numeric inputs.
* [#15842](https://bugzilla.scilab.org/15842): `unique` could not process 2D sparse matrices.
* [#15868](https://bugzilla.scilab.org/15868): `setdiff(s,s2)` yielded an error when `s` or/and `s2` is sparse encoded.
* [#15954](https://bugzilla.scilab.org/15954): `mfile2sci` abusively added a 6 lines `mode(0); ieee(1)` header to every converted file.
* [#16069](https://bugzilla.scilab.org/16069): [].figure_name crashes Scilab.
* [#16106](https://bugzilla.scilab.org/16106): Xcos sciblk4 user-defined blocks did not handle opar and odstate/oz correctly.
* [#16122](https://bugzilla.scilab.org/16122): concat polynomials with <> var did not raise an error.
* [#16141](https://bugzilla.scilab.org/16141): recursive extraction `(m:n)($)` crashed Scilab
* [#16151](https://bugzilla.scilab.org/16151): `isequal(1:$, 2:$)` returned `%T`.
* [#16193](https://bugzilla.scilab.org/16193): `covStart()` clear previous coverage information. `profileEnable()` could be use to append a macro later on.
* [#16196](https://bugzilla.scilab.org/16196): `covStart()` help page was incomplete about the API usage.
* [#16274](https://bugzilla.scilab.org/16274): assert_checkequal() did not considered equal matching Nan or void elements in (nested) containers.
* [#16284](https://bugzilla.scilab.org/16284): `typename()` still considered removed uncompiled macros (11, "m"), and "mc" overloading code instead of "function".
* [#16288](https://bugzilla.scilab.org/16288): An error message from `wavread` was wrong.
* [#16297](https://bugzilla.scilab.org/16297): After function test(), e={}, endfunction; macr2tree(test)  crashes Scilab.
* [#16305](https://bugzilla.scilab.org/16305): `unwrap()` failed unfolding curves with straight segments.
* [#16337](https://bugzilla.scilab.org/16337): The 3rd output of `[U,km,ku] = unique(..)` was not implemented.
* [#16340](https://bugzilla.scilab.org/16340): `setdiff("","")` produced `""` instead of `[]`. The `"c"` and `"r"` options yielded an error for string inputs. Input arguments were poorly checked.
* [#16342](https://bugzilla.scilab.org/16342): `strcat()` was much slower in Scilab 6.0.2.
* [#16350](https://bugzilla.scilab.org/16350): in if/while conditions, the empty sparse boolean was considered as TRUE.
* [#16358](https://bugzilla.scilab.org/16358): `isdef([],..)` yielded an error instead of returning [].
* [#16357](https://bugzilla.scilab.org/16357): `script2var` did get variables generated by `exec()`.
* [#16362](https://bugzilla.scilab.org/16362): sparse empty matrix could no be concatenated.
* [#16365](https://bugzilla.scilab.org/16365): `median(m,"r")` and `median(m,"c")` yielded wrong results (6.1.0 regression)
* [#16366](https://bugzilla.scilab.org/16366): `plot([0 1], ":")` plotted a dash-dotted curve instead of a dotted one.
* [#16369](https://bugzilla.scilab.org/16369): Right divisions / involving one or two sparse numerical matrices were no longer supported.
* [#16370](https://bugzilla.scilab.org/16370): `msprintf()` did not handle LaTeX dollars anymore.
* [#16374](https://bugzilla.scilab.org/16374): Any plot with datatips saved in Scilab 5.5 could not be loaded in Scilab 6.
* [#16385](https://bugzilla.scilab.org/16385): In Xcos without any diagram, the menu `Simulation => Modelica initialize` displayed "Undefined variable: flag" in the console.
* [#16391](https://bugzilla.scilab.org/16391): `csvRead()` was crashing with CSV files containing empty lines.
* [#16396](https://bugzilla.scilab.org/16396): recursive extraction `(m:n)(:)` crashed Scilab
* [#16397](https://bugzilla.scilab.org/16397): display of long (real) column vectors was slow (regression).
* [#16399](https://bugzilla.scilab.org/16399): `mtlb_zeros([])` was crashing Scilab.
* [#16401](https://bugzilla.scilab.org/16401): global `external_object_java` class was crashing Scilab.
* [#16403](https://bugzilla.scilab.org/16403): 1D extraction of matrix with implicit index had wrong dimensions.
* [#16406](https://bugzilla.scilab.org/16406): `edit_curv` yielded an error when reading data.
* [#16407](https://bugzilla.scilab.org/16407): Fec rendering was incorrect
* [#16408](https://bugzilla.scilab.org/16408): toJSON(var, indent, filename) is the right call sequence. Documentation has been udpated.
* [#16438](https://bugzilla.scilab.org/16438): `asciimat(text)` did not accept strings of different lengths or including some extended UTF-8 characters.
* [#16445](https://bugzilla.scilab.org/16445): `colorbar(..)` ignored how to guess `umin` and `umax` for a Champ object (with .colored="on").
* [#16449](https://bugzilla.scilab.org/16449): Insertion of implicit vector in Cell was crahsing Scilab
* [#16450](https://bugzilla.scilab.org/16450): Concatenating encoded integers with decimal or complex numbers was not possible.
* [#16451](https://bugzilla.scilab.org/16451): `setdiff(a,b,"r"|"c")` with a and b boolean yielded an error.
* [#16452](https://bugzilla.scilab.org/16452): `setdiff(sparse([1 3 0 2]), sparse([3 7]))` missed returning 0, and wrongly returned 3.
* [#16454](https://bugzilla.scilab.org/16454): `gsort` yielded an error when sorting any sparse vector including some NaN.
* [#16458](https://bugzilla.scilab.org/16458): `mean()` did not handle sparse numerical matrices.
* [#16465](https://bugzilla.scilab.org/16465): Scinotes OpenRecent menu was not updated when it should.
* [#16473](https://bugzilla.scilab.org/16473): Deleting rows in a sparse squared the matrix with padding zeros (Scilab 6 regression).
* [#16474](https://bugzilla.scilab.org/16474): `imult(%z)` crashed Scilab.
* [#16476](https://bugzilla.scilab.org/16476): `issquare` was not overloaded.
* [#16488](https://bugzilla.scilab.org/16488): Concatenations mixing boolean and double with at least one operand being sparse were not supported.
* [#16496](https://bugzilla.scilab.org/16496): The `getdate` page should be rewritten: a) `getdate("s")` does NOT take leap seconds into account. b) `D=getdate(X)` is vectorized, accepts fractional seconds and returns them in `[0,1)` in D(10) instead of milliseconds. Moreover, the time referential of the result was unclear (time zone, daylight saving offset).
* [#16504](https://bugzilla.scilab.org/16504): `clock` returned none of the milliseconds, time zone, and daylight saving time.
* [#16508](https://bugzilla.scilab.org/16508): csvTextScan and csvRead did not handle well complex data.
* [#16512](https://bugzilla.scilab.org/16512): 1 ./ uint8(0) crashes Scilab (idem with int8, uint16, int16, uint32, int32, uint64, int64).
* [#16517](https://bugzilla.scilab.org/16517): `getdate("s")` truncated the actual time to integer seconds. `getdate(u)(10)` returned fractional seconds instead of milliseconds as `getdate()`.
* [#16522](https://bugzilla.scilab.org/16522): `bitget(x,pos)` and `bitset(x,pos)` results could be wrong when `pos` is an encoded integer.
* [#16525](https://bugzilla.scilab.org/16525): `soundsec(t,freq)` has the trivial equivalence `0 : 1/freq : t*(1-%eps)` and should be removed.
* [#16529](https://bugzilla.scilab.org/16529): `deff` could not return the created function as output argument, preventing to cretae and use anonymous functions. The function's headline and body had to be provided separately. For Simple functions, a one-string input (possibly console-oriented) definition was not supported.
* [#16530](https://bugzilla.scilab.org/16530): `mapsound` needed to be reforged.
* [#16540](https://bugzilla.scilab.org/16540): uicontrol spinner did not "snaptoticks"
* [#16549](https://bugzilla.scilab.org/16549): simple script crashed Scilab in GUI mode.
* [#16551](https://bugzilla.scilab.org/16551): `num2cell` returned {} for any input array of empty strings.
* [#16552](https://bugzilla.scilab.org/16552): `mfile2sci`: conversion of `ispc` and `isunix` still used `MSDOS` removed far ago. Conversion of `ismac` was missing. Conversion of `exist` missed using `mtlb_exist`. Conversion of `dos` yielded some "operation +" warnings. Unit tests for the conversion of `cd`, `dir` and `ferror` overwrote some Scilab reserved keywords. Conversion of `return` yielded `mtlb(resume)`.
* [#16553](https://bugzilla.scilab.org/16553): `unique(["" ""])` returned `["" ""]`.
* [#16557](https://bugzilla.scilab.org/16557): `macr2tree` + `tree2code` translated `e={2}` into `"e=1"` and `e={2,"ab"}` into `"e=[2,"ab"]"`.
* [#16559](https://bugzilla.scilab.org/16553): `isempty(A)` was true for sparse matrix of dimension 2^16 or larger.
* [#16561](https://bugzilla.scilab.org/16561): `histplot(-10:0.2:10, 2)` yielded a warning from `histc` and an error.
* [#16565](https://bugzilla.scilab.org/16565): `edit(user_defined_function)` corrupted the original code.
* [#16567](https://bugzilla.scilab.org/16567): `mfile2sci` did not support Matlab block-comments %{ ..%}.
* [#16568](https://bugzilla.scilab.org/16568): The operator `**` was undocumented.
* [#16571](https://bugzilla.scilab.org/16571): `mfile2sci` had several issues when converting the NOT ~ operator: 1) `~(1-1)` was converted into `~1-1` instead of `~(1-1)`  2) ~ applied to an integer expression yielded an error from `convert2double`  3) `~i` was converted into `~%i` instead of `~abs(%i)`.
* [#16573](https://bugzilla.scilab.org/16573): `mfile2sci`: Some `axis` conversions were wrong or not reliable.
* [#16586](https://bugzilla.scilab.org/16586): `mfile2sci`: The `prettyprintoutput` flag badly managed appended comments.
* [#16592](https://bugzilla.scilab.org/16592): %i|[], []|%i, %i&[], and []&%i were wrong. %t|%i, %t&%i, and %i&%t were not defined.
* [#16596](https://bugzilla.scilab.org/16596): Concatenating encoded integers with sparse numeric data was not possible.
* [#16601](https://bugzilla.scilab.org/16601): libmatio >= 1.5.18 was not supported.
* [#16608](https://bugzilla.scilab.org/16608): `union` did not support input boolean, sparse boolean, nor sparse numerical matrices. The result of `union(complexA, complexB)` was wrongly conjugate.
* [#16609](https://bugzilla.scilab.org/16609): `bitcmp` needed to be upgraded for Scilab 6.
* [#16612](https://bugzilla.scilab.org/16612): Unlike the `.*.` operator, `kron()` was not defined for sparse numeric matrices.
* [#16613](https://bugzilla.scilab.org/16613): Neither `.*.` nor `kron()` worked with boolean or sparse boolean matrices.
* [#16614](https://bugzilla.scilab.org/16614): Out of gcf(), a figure with some xstring content could be not properly reframed to contents.
* [#16617](https://bugzilla.scilab.org/16617): `gamma` did not support incomplete gamma integrals.
* [#16618](https://bugzilla.scilab.org/16618): `close` could not close the help browser, xcos, nor the variable editor or browser.
* [#16620](https://bugzilla.scilab.org/16620): `derivat` had no reciprocal `polyint` polynomial integration function.
* [#16622](https://bugzilla.scilab.org/16622): `inv` could no longer be overloaded for hypermatrices of decimal or complex numbers.
* [#16623](https://bugzilla.scilab.org/16623): `rand(2,2,2)^2` yielded a wrong result instead of trying to call the `%s_p_s` overload for input hypermatrices.
* [#16624](https://bugzilla.scilab.org/16624): `fullfile` badly handled any forced final file separator when building a directory's path.
* [#16626](https://bugzilla.scilab.org/16626): == and <> between libraries were no longer defined.
* [#16627](https://bugzilla.scilab.org/16627): `importgui` function's name was not explicit enough. `importgui` is renamed `uiSpreadsheet`. `importgui` internals were public.
* [#16629](https://bugzilla.scilab.org/16629): `interp1`'s documentation did not tell the spline edges conditions ; extrapolation modes were poorly explained. ; the description of the result's size was completely wrong ; x as an option was not documented. A wrong extrapolation value could silently return a wrong result. There was some dead code like `if varargin(5)==%nan`. A bugged error message yielded its own error. When x is implicit, the argument index in error messages could be wrong. `periodic` and `edgevalue` extrapolation modes were not available. `linear` extrapolation was not available for splines. When `xp` is an hypermatrix with `size(xp,1)==1`, the size of the result was irregular/wrong.
* [#16631](https://bugzilla.scilab.org/16631): read-only handle properties were reported as unknown when trying to set them.
* [#16632](https://bugzilla.scilab.org/16632): Scilab did not start with unsupported locale on macOS.
* [#16636](https://bugzilla.scilab.org/16636): `det(sparse)` most often yielded `Nan`. `[e,m]=det(sparse)` was not actually implemented.
* [#16638](https://bugzilla.scilab.org/16638): `getcolor` shew a bunch of issues.
* [#16639](https://bugzilla.scilab.org/16639): `atomsInstall` and `atomsRemove` did not update the Toolboxes menu.
* [#16644](https://bugzilla.scilab.org/16644): `input("message:")` yielded a wrong error message about `mprintf` in case of non-interpretable input.
* [#16654](https://bugzilla.scilab.org/16654): `interp` was leaking memory.
* [#16661](https://bugzilla.scilab.org/16661): `x=spzeros(1e10,1e10)` yielded an incorrect error message.
* [#16664](https://bugzilla.scilab.org/16664): `diag(spzeros(2,2))` yielded an `Operation -[]` warning.
* [#16665](https://bugzilla.scilab.org/16665): `help echo` could not redirect to `help mode` when preferred, for new users coming from Octave.
* [#16677](https://bugzilla.scilab.org/16677): In offline mode, `atomsInstall` was flashing many times the console.
* [#16679](https://bugzilla.scilab.org/16679): `get_function_path("acosh")` yielded an error (regression from Scilab 6.0.0).
* [#16683](https://bugzilla.scilab.org/16683): The `m2sci` code converter needed a set of fix and improvements.
* [#16685](https://bugzilla.scilab.org/16685): `asciimat("A";"";"B")` yielded a transposed matrix of codes. When processing an hypermatrix of codes, the dim#2 of the result was squeezed (unlike Matlab/Octave's behavior).


### Bugs fixed in 6.1.0:
* [#2694](https://bugzilla.scilab.org/2694): `bitget` did not accept positive integers of types int8, int16 or int32.
* [#5824](https://bugzilla.scilab.org/5824): The `datafit` algorithm was not documented.
* [#6070](https://bugzilla.scilab.org/6070): How to make multiscaled plots was not documented.
* [#7293](https://bugzilla.scilab.org/7293): There was no function to circularly shift components, rows, columns or subarrays of an array. `circshift` introduced.
* [#7562](https://bugzilla.scilab.org/7562): `factorial` could use a huge memory amount even for a scalar argument.
* [#7589](https://bugzilla.scilab.org/7589): There was no function computing the binomial coefficients.
* [#7657](https://bugzilla.scilab.org/7657): `lstsize` was a duplicate of `size` and should be removed.
* [#7724](https://bugzilla.scilab.org/7724): When a figure is created in .auto_resize="on" mode, its .axes_size sets its .figure_size accordingly, not the reverse. But this was not documented.
* [#7732](https://bugzilla.scilab.org/7732): The `datafit` help page needed to be fixed and overhauled.
* [#7765](https://bugzilla.scilab.org/7765): `champ1` is useless. `champ().colored` is available for a long time.
* [#7777](https://bugzilla.scilab.org/7777): `ged` did not allow moving several objects.
* [#7948](https://bugzilla.scilab.org/7948): `gsort` could not perform multilevel sorting, and could not sort complex numbers completely.
* [#7967](https://bugzilla.scilab.org/7967): The tricky size `[ny,nx]` of `meshgrid(x,y)` results and usages with graphics was not enough documented.
* [#8301](https://bugzilla.scilab.org/8301): `definedfields` wrongly considered as defined void elements in lists, tlists and mlists.
* [#8307](https://bugzilla.scilab.org/8307): `list2vec` and `vec2list` were located in the optimization module instead of in data_structures, and were missing in the `See also` section of `list`.
* [#8328](https://bugzilla.scilab.org/8328): Tabs of the Xcos block Format => Edit dialog were unclear.
* [#8393](https://bugzilla.scilab.org/8393): `isdef` and `definedfields` had mismatching answers for void elements in lists, mlists or tlists.
* [#8418](https://bugzilla.scilab.org/8418): `unique` was not able to return the number of occurences of returned dictinct entities.
* [#8784](https://bugzilla.scilab.org/8784): Automatic self-adjusting blocks `SCALE_CSCOPE` & `SCALE_CMSCOPE` in Xcos.
* [#5512](https://bugzilla.scilab.org/5512): `disp` puzzlingly displayed arguments in reverse order.
* [#9007](https://bugzilla.scilab.org/9007): On Linux, `sciargs()` included twice `-nw` when `-nw` is used.
* [#9130](https://bugzilla.scilab.org/9130): In the SigBuilder Xcos page, the parameters of the block were not described. The page refered to the CURVE_c block removed fron Scilab 5.0.0.
* [#9529](https://bugzilla.scilab.org/9529): `assert_checkequal(list(1,,3), list(1,,3))` yielded an error.
* [#9673](https://bugzilla.scilab.org/9673): Priority of colon `:` operator was too low
* [#10014](https://bugzilla.scilab.org/10014): `DominationRank` could be made private in `optim_nsga2`.
* [#10078](https://bugzilla.scilab.org/10078): `isinf` was not reliable for polynomials.
* [#10092](https://bugzilla.scilab.org/10092): The left and right Kronecker divisions were not actually implemented nor documented.
* [#10353](https://bugzilla.scilab.org/10353): Documentation: The referential for the uicontrol.position property was not provided. Moreover, `gca().axes_bounds` refered to `fig.figure_size` instead of `fig.axes_size`.
* [#10553](https://bugzilla.scilab.org/10553): After calling `colorbar`, retrieving the handle of the new color bar was not trivial.
* [#10702](https://bugzilla.scilab.org/10702): The icon of the FROMWSB xcos block did not show the name of the source variable.
* [#10723](https://bugzilla.scilab.org/10723): `subplot`'s action was unclearly described in its help page. Page improved.
* [#11852](https://bugzilla.scilab.org/11852): File browser didn't update after file creation or removal.
* [#11363](https://bugzilla.scilab.org/11363): `show_window()` did not raise the current graphics window.
* [#12013](https://bugzilla.scilab.org/12013): `bitset` did not work for numbers greater than 2^32-1.
* [#12302](https://bugzilla.scilab.org/12302): Blocks of legends could not be moved interactively.
* [#12428](https://bugzilla.scilab.org/12428): A part of an error message from `toprint` was not translated.
* [#12520](https://bugzilla.scilab.org/12520): Variable browser did not display the size of the variables.
* [#12534](https://bugzilla.scilab.org/12534): Variable browser did not display the size of the variables.
* [#12837](https://bugzilla.scilab.org/12837): `strcmpi` was an obsolete duplicate of `strcmp(,'i')`. It is removed.
* [#13603](https://bugzilla.scilab.org/13603): `plot(1:3, 10)` failed.
* [#13683](https://bugzilla.scilab.org/13683): unsetmenu for a dockable="off" figure crashed Scilab.
* [#13738](https://bugzilla.scilab.org/13738): `get_figure_handle` was a specific duplicate of the more powerful `findobj('figure_id',n)` feature. It is now obsolete.
* [#13766](https://bugzilla.scilab.org/13766): Minimal values for `.figure_size` were not documented.
* [#14015](https://bugzilla.scilab.org/14015): Nan terms added to a polynomial were ignored.
* [#14191](https://bugzilla.scilab.org/14191): Unlike `plot2d()`, `plot()` did not accept logflags among input arguments.
* [#14397](https://bugzilla.scilab.org/14397): `mgeti` wrongly handled `int64` or `uint64` integers bigger then 2^52. The `mget`/`mgeti` help page was unclear.
* [#14422](https://bugzilla.scilab.org/14422): `clc(0)` did nothing, now clears last console entry. Remove leading blank after a `clc()`.
* [#14498](https://bugzilla.scilab.org/14498): `size([],3)` returned 1 instead of 0.
* [#14501](https://bugzilla.scilab.org/14501): `strsubst` crashed on consecutive occurrences.
* [#14557](https://bugzilla.scilab.org/14557): `csim` failed when the system has no state.
* [#14498](https://bugzilla.scilab.org/14498): `size([],3)` returned 1 instead of 0.
* [#14487](https://bugzilla.scilab.org/14487): matrix indexing was not coherent with MATLAB.
* [#14502](https://bugzilla.scilab.org/14502): `Demo > GUI > Uicontrols 2` could not be exported to a file.
* [#14585](https://bugzilla.scilab.org/14585): `closeEditvar()` had to be replaced with `editvar("close")`.
* [#14604](https://bugzilla.scilab.org/14604): `emptystr` is 40x slower with 6.0.0 wrt 5.5.2
* [#14605](https://bugzilla.scilab.org/14605): fixed - `bench_run` was too strict about the specification of tests names.
* [#14606](https://bugzilla.scilab.org/14606): Memory used by variables returned by `[names,mem]=who()` was always zero.
* [#14610](https://bugzilla.scilab.org/14610): `x = ric_desc(H,E)` always yielded an error. [x1,x2,residual] = ric_desc(..) returned a wrong `residual` value.
* [#14642](https://bugzilla.scilab.org/14642): No more "\r" carriage return with printf.
* [#14655](https://bugzilla.scilab.org/14655): `bitset` worked only element-wise, without accepting mixed scalar and array inputs.
* [#14660](https://bugzilla.scilab.org/14660): `dec2base` was 53-bit limited by double and not uint64 ready
* [#14734](https://bugzilla.scilab.org/14734): The `simulated_annealing` module was out of the `optimization` one.
* [#14735](https://bugzilla.scilab.org/14735): The `genetic_algorithms` module was out of the `optimization` one.
* [#14738](https://bugzilla.scilab.org14738): `hist3d` was not actually an histogram function. It is renamed `bar3d`.
* [#14741](https://bugzilla.scilab.org/14741): The syntax `[m,e]=log2(x)` was not documented. As public function `frexp` was in duplicate with `[m,e]=log2(x)`.
* [#14742](https://bugzilla.scilab.org/14742): `mprintf`, `msprintf` a non finite number displayed nothing.
* [#14746](https://bugzilla.scilab.org/14746): Tiny numbers were sometimes displayed as 0.
* [#14791](https://bugzilla.scilab.org/14791): `sleep 1` crashed Scilab.
* [#14812](https://bugzilla.scilab.org/14812): Minor typos in messages.
* [#14836](https://bugzilla.scilab.org/14836): Calling `exportUI` on a non dockable figure crashed scilab.
* [#14863](https://bugzilla.scilab.org/14863): In Xcos, the default ending time was unhandily high (100000), reduced it to 30.
* [#14900](https://bugzilla.scilab.org/14900): In Scilab 6.0, the new overloading code `function` replacing `mc` for functions in Scilab language was not documented.
* [#14982](https://bugzilla.scilab.org/14982): `msprintf` segmentation fault was caught due to wrong size
* [#14985](https://bugzilla.scilab.org/14985): Scilab crashed if a .bin file was not found.
* [#15067](https://bugzilla.scilab.org/15067): `.xmi` files saved from Xcos could not be opened by drag & dropping them into the console.
* [#15068](https://bugzilla.scilab.org/15068): `size(sum(ones(2,3,4,5),4))` was [4 2 3] instead of [2 3 4].
* [#15087](https://bugzilla.scilab.org/15087): Deleting rows or columns from a matrix is slow (regression)
* [#15200](https://bugzilla.scilab.org/15200): `weekday` yielded an error for some row of input dates.
* [#15248](https://bugzilla.scilab.org/15248): `lsq`was leaking memory.
* [#15260](https://bugzilla.scilab.org/15260): `sci2exp` was no longer able to convert non-scalar structures.
* [#15269](https://bugzilla.scilab.org/15269): `xgetech` was poor and stiff compared to any combination of `gca()` properties `.axes_bounds`, `.data_bounds`, `.log_flags`, and `.margins`. It is removed.
* [#15271](https://bugzilla.scilab.org/15271): `bitget` needed to be upgraded.
* [#15309](https://bugzilla.scilab.org/15309): `eval` was a weak duplicate of `evstr`. It should be removed.
* [#15321](https://bugzilla.scilab.org/15321): `lu` was leaking memory.
* [#15350](https://bugzilla.scilab.org/15350): `ric_desc` should be merged into `riccati`.
* [#15357](https://bugzilla.scilab.org/15357): `atomsGetInstalledPath` returned "" if the provided technical name differs only by the case, or is a fragment, or failed when the version is not provided.
* [#15359](https://bugzilla.scilab.org/15359): `twinkle` was not able to blink several independent objects.
* [#15360](https://bugzilla.scilab.org/15360): `numer` and `denom` were poor and duplicates of the `.num` and `.den` fields of rationals. They are removed.
* [#15368](https://bugzilla.scilab.org/15368): `freson` silently returned frequencies not corresponding to a maximum, or returned [] instead of some still computable maxima frequencies.
* [#15374](https://bugzilla.scilab.org/15374):  Trivial infinite loop could not be interrupted.
* [#15392](https://bugzilla.scilab.org/15392): `comet` and `comet3d` did not allow specifying colors with colors names.
* [#15393](https://bugzilla.scilab.org/15393): In a new figure, `nicholschart` plotted nothing. The default frame color was a flashy cyan. The position of gain labels could be puzzling. It was not possible to specify colors by their names. Postprocessing the frames and the set of labels was not easy.
* [#15421](https://bugzilla.scilab.org/15421): A new smarter default grid_style was required since smart lines styles #7-10 are available.
* [#15425](https://bugzilla.scilab.org/15425): The Kronecker product `a .*. b` failed when `a` or `b` or both are hypermatrices, with one or both being polynomials or rationals.
* [#15428](https://bugzilla.scilab.org/15428): `repmat` was slow. Its code did not use `kron` and was complex.
* [#15431](https://bugzilla.scilab.org/15431): The empty matrix `[]` and its non-convertibility were poorly documented.
* [#15451](https://bugzilla.scilab.org/15451): The code was not adapted to use `lucene 4.10` in Debian.
* [#15481](https://bugzilla.scilab.org/15481): `scatter` and `scatter3d` failed when specifying a single data point.
* [#15514](https://bugzilla.scilab.org/15514): The `set` documentation page needed to be overhauled.
* [#15517](https://bugzilla.scilab.org/15517): `factorial` could be actually used up to only n=170.
* [#15522](https://bugzilla.scilab.org/15522): `unique` was not able to consider all Nan values as the same value. A `uniqueNan` option now allows it.
* [#15523](https://bugzilla.scilab.org/15523): `%ODEOPTIONS(1)=2` didn't work with solvers 'rk' and 'rkf'
* [#15534](https://bugzilla.scilab.org/15534): Booleans and encoded integers could not be concatenated together.
* [#15577](https://bugzilla.scilab.org/15577): `edit` did not accept a line number as text, as with `edit linspace 21`.
* [#15580](https://bugzilla.scilab.org/15580): `det(sparse([],[]))` yielded an error.
* [#15581](https://bugzilla.scilab.org/15581): display of complex matrix was ugly.
* [#15595](https://bugzilla.scilab.org/15595): `unique` was not able to return distinct values in their original order, without sorting them. A `keepOrder` option now allows it.
* [#15628](https://bugzilla.scilab.org/15628): `with_tk` was a duplicate of `with_module('tclsci')`. It is removed.
* [#15643](https://bugzilla.scilab.org/15643): Features with the signed zero -0 were not documented.
* [#15668](https://bugzilla.scilab.org/15668): `save(filename)` saved all predefined Scilab constants %e %pi etc.. (regression)
* [#15680](https://bugzilla.scilab.org/15680): `loadmatfile` could not return variables in a structure instead of into the calling environment.
* [#15701](https://bugzilla.scilab.org/15701): `A\B` was not faster when `A` is square and triangular.
* [#15715](https://bugzilla.scilab.org/15715): `%nan` indices crashed Scilab.
* [#15734](https://bugzilla.scilab.org/15734): `intersect` did not support complex numbers.
* [#15737](https://bugzilla.scilab.org/15737): `setdiff` was wrong with complex numbers.
* [#15742](https://bugzilla.scilab.org/15742): The `compatibility_functions` module should be merged in the `m2sci` one.
* [#15744](https://bugzilla.scilab.org/15744): `sylm(a,b)` yielded an error when degree(a)==0 or degree(b)==0.
* [#15745](https://bugzilla.scilab.org/15745): `diophant(0,0,m)`, `diophant([p 0],q)`, `diophant([0 p],q)` with m<>0 and p>q were wrong. There was no flag for cases with an infinite number of solutions. When there is no solution, some values were returned anyway, instead of []. In this case, the documented definition of the err value was dubious. Decimal numbers and integers were accepted, but not encoded integers. Inf and NaN input coefficients were not rejected.
* [#15812](https://bugzilla.scilab.org/15812): On assigning variables the source variable may become become corrupted
* [#15821](https://bugzilla.scilab.org/15821): `fac3d` and `fac3d1` were still in Scilab 6.0 despite they were tagged obsolete 14 years ago in Scilab 4.1
* [#15825](https://bugzilla.scilab.org/15825): `gsort` could not sort completely dense matrices of complex numbers.
* [#15840](https://bugzilla.scilab.org/15840): `grand(1,"prm",m)` yielded an unsqueezed size([size(m) 1]) hypermatrix
* [#15861](https://bugzilla.scilab.org/15861): The `overloadname` internal macro defined in CACSD was a duplicate of `typeof(,"overload")`. It is removed.
* [#15874](https://bugzilla.scilab.org/15874): The `scatter3` function name was an unfounded naming exception. It had to be renamed `scatter3d`.
* [#15921](https://bugzilla.scilab.org/15921): Xcos user's function `scifunc_block_m` block: any `<` in the expression was not displayed on the block's icon.
* [#15934](https://bugzilla.scilab.org/15934): The `^ hat` page wrongly indicated that `^` applied to a rectangular matrix not being a vector is done element-wise.
* [#15948](https://bugzilla.scilab.org/15948): `xlabel`, `ylabel`, `zlabel` and `title` needed to be upgraded.
* [#15963](https://bugzilla.scilab.org/15963): The `EXPRESSION` block was more expected in the Mathematical palette.
* [#15964](https://bugzilla.scilab.org/15954): A complex empty sparse matrix could be obtained after insertion.
* [#15965](https://bugzilla.scilab.org/15965): `plot(x,y)` did not accept y as encoded integers.
* [#15967](https://bugzilla.scilab.org/15967): `setdiff(1,[])` was [] instead of `1`. `setdiff(a,[],..)` was not enough tested.
* [#15969](https://bugzilla.scilab.org/15969): Fix spelling
* [#15974](https://bugzilla.scilab.org/15974): `msprintf("%d", %nan)` did not return Nan
* [#15977](https://bugzilla.scilab.org/15977): The documentation for `wavread(..,'info')` had a mistake. The `wavread` page deserved some improvements.
* [#15978](https://bugzilla.scilab.org/15978): The `writewav` page in english said that input data are one column per channel, instead of one row per channel. In addition, in case of writing error, `savewave` kept the output file open and locked.
* [#15979](https://bugzilla.scilab.org/15979): `EXPRESSION`, `CLR` and `DLR` Xcos blocks did not (always) correctly display multicharacter exponents in expressions displayed in their icons.
* [#15981](https://bugzilla.scilab.org/15981): `wavread` kept the wav file open and locked when returning on errors. It weakly managed the input file name. It claimed for invalid data formats instead of unsupported ones, with poor information about the current format vs the supported ones. Several error messages refered to a wrong function.
* [#15983](https://bugzilla.scilab.org/15983): `group` regressed in 5.5.2 due to a too intrusive fix.
* [#15984](https://bugzilla.scilab.org/15984): display scale was wrong with Retina dispplays on OSX..
* [#15995](https://bugzilla.scilab.org/15995): patch was missing in surface plot (regression)
* [#15998](https://bugzilla.scilab.org/15998): `mfile2sci` yielded an unsuitable warning when converting `atan`.
* [#16000](https://bugzilla.scilab.org/16000): `mfile2sci` converted `acoth` using `atanh` and yielding a wrong warning message.
* [#16003](https://bugzilla.scilab.org/16003): Zoom with mouse scroll wheel was broken on simple surfaces.
* [#16005](https://bugzilla.scilab.org/16005): The `intdec` example was biased and not robust when changing sampling frequencies.
* [#16007](https://bugzilla.scilab.org/16007): Non-integer index in sparse makes Scilab crash.
* [#16008](https://bugzilla.scilab.org/16008): Uimenu .callback_type, .children, .handle_visible, .user_data properties, and using HTML styling in .label were not documented.
* [#16011](https://bugzilla.scilab.org/16011): `colorbar(umin, umax)` did not work after `plot3d2`.
* [#16012](https://bugzilla.scilab.org/16012): `[struct() struct()]` crashed Scilab.
* [#16013](https://bugzilla.scilab.org/16013): Load previously saved environment with "File/Load environment" menu failed.
* [#16014](https://bugzilla.scilab.org/16014): after `x.a=1; x(:)=[]` x.a was an empty list.
* [#16015](https://bugzilla.scilab.org/16015): `intg(a,b,f)` called f(x) with x outside [a,b].
* [#16019](https://bugzilla.scilab.org/16019): `polarplot(x,Z)` yielded an error when x is a vector and Z a matrix.
* [#16021](https://bugzilla.scilab.org/16021): `tand([-90 90])` answered [Nan Nan] instead of [-Inf, Inf]. `cotd([-90 90])` answered [Nan Nan] instead of [0 0]. `1 ./cosd([-90 90])` answered [Inf -Inf] instead of [Inf Inf].
* [#16026](https://bugzilla.scilab.org/16026): For `atanh`, neither the documentation pages nor the `m2sci` converter were up to date.
* [#16028](https://bugzilla.scilab.org/16028): The length of `intdec(intdec(x, r), 1/r)` was most often different from length(x).
* [#16046](https://bugzilla.scilab.org/16046): After `w=ssrand(2,3,4)`, `[]+w`, `[]-w`, `w+[]` and `w-[]` yielded an "operation +/- []" warning.
* [#16051](https://bugzilla.scilab.org/16051): undefined list elements could be of 2 distinct typeof "void" or "listundefined" according to the way they are created.
* [#16053](https://bugzilla.scilab.org/16053): `plot(,"color",c)` no longer supported standard abbreviated color names c like "k" for black.
* [#16062](https://bugzilla.scilab.org/16062): `tbx_make` did not allow easily to force building all toolbox sections without the toolbox builder (if any). `tbx_make(Dir,"*")` now does it.
* [#16064](https://bugzilla.scilab.org/16064): `tbx_make(Dir,'localization')` did not update `.mo` files from `.po`.
* [#16065](https://bugzilla.scilab.org/16065): Building help pages including some <scilab:image> tags used and finally deleted the current on-screen figures (if any)!
* [#16067](https://bugzilla.scilab.org/16067): The display of matrices of signed integers was misaligned (regression of 6.0.0 vs 5.5.2)
* [#16071](https://bugzilla.scilab.org/16071): `prettyprint(complex(1,%nan))` omitted the "+" in `1 + Nani`. It printed positive exponents with a useless "+". For any input nul polynomial, the string result never included the name of the variable. Default input arguments could not be skipped. ExportFormat was uselessly case-sensitive. For tex|latex: for text input, $ \ % & { } ~ and ^ special characters were not protected ; spaces were not protected, all words were concatenated ; for polynomials and rationals, the result string could be extremely long and not easily wrappable. For MathML: "<" was not protected ; <mi></mi> were missing for text input ; <mtable>, </mtable>, <mtr>, </mtr>, <mtd>, <mfenced> and </mfenced> tags were not wrapped and could not be indented. Delimiters: "" was not documented as possible value ; ")" was wrongly documented. Dynamical linear systems were not documented as possible input.
* [#16072](https://bugzilla.scilab.org/16072): `prettyprint` did not actually support input encoded integers.
* [#16075](https://bugzilla.scilab.org/16075): `prettyprint` was broken for cells.
* [#16079](https://bugzilla.scilab.org/16079): `part("",$)` and `part("",$-1:$+2)` yielded an error instead of returning "" and "  ".
* [#16081](https://bugzilla.scilab.org/16081): `prettyprint` did not allow to export objects into HTML.
* [#16085](https://bugzilla.scilab.org/16085): insertion in an empty struct was broken.
* [#16087](https://bugzilla.scilab.org/16087): Insertion of struct in a non-empty struct crashed Scilab.
* [#16089](https://bugzilla.scilab.org/16089): x=1:1e10 freezed Scilab.
* [#16090](https://bugzilla.scilab.org/16090): The documentation of `prbs_a` was poor and unclear.
* [#16095](https://bugzilla.scilab.org/16095): For a matrix of graphic handles of same types, it was no longer possible to clear a property by assigning [].
* [#16097](https://bugzilla.scilab.org/16097): `coth` was more than twice slower than `1./tanh()`. `tanh` and `coth` did not accept sparse-encoded input.
* [#16102](https://bugzilla.scilab.org/16102): `lstcat` could not concatenate input lists with mixed objects of types>10 like 1:$, sin, sind, struct, cells, tlists or mlists. In addition, it was slow for very long inputs lists.
* [#16104](https://bugzilla.scilab.org/16104): `assert_checkequal` did not support properly or at all Nan and void in lists, Nan in sparse, implicit lists, macros, libraries, built-in functions, graphical handles. For input arrays or lists of matching typeof and sizes, the failure message did not display neither the index nor the values of the first mismatching elements.
* [#16108](https://bugzilla.scilab.org/16108): For the CSCOPE and CMSCOPE blocks, the Label&id parameter entered in their interface were not displayed below the block's icon.
* [#16111](https://bugzilla.scilab.org/16111): `isglobal` was not supporting non-scalar array of strings as input.
* [#16118](https://bugzilla.scilab.org/16118): `%s <> (1+%s)` returned %F.
* [#16120](https://bugzilla.scilab.org/16120): `tree_show` was broken cells within containers.
* [#16121](https://bugzilla.scilab.org/16121): `tree_show` void components were displayed as non-empty.
* [#16124](https://bugzilla.scilab.org/16124): `sci2exp({})` result could not be executed.
* [#16128](https://bugzilla.scilab.org/16128): `tree_show` need improvements.
* [#16129](https://bugzilla.scilab.org/16129): `uiDumpTree()` interpreted "\n" "\r" "\t" sequences in contents as special ones.
* [#16135](https://bugzilla.scilab.org/16135): base2dec did not detect invalid numbers.
* [#16137](https://bugzilla.scilab.org/16137): After running `playsnd` on Windows for a long sound, it was not possible to stop it.
* [#16138](https://bugzilla.scilab.org/16138): `playsnd` could not be run in a non-modal (asynchronous) way on Linux and MacOS.
* [#16139](https://bugzilla.scilab.org/16139): `auread` and `auwrite` kept the sound file open and locked when returning on errors. They poorly handled the default .au sound file extension.
* [#16140](https://bugzilla.scilab.org/16140): `playsnd` could not directly accept an audio file and could not repeat the input sound.
* [#16143](https://bugzilla.scilab.org/16143): `clc(n)` cleared n+1 lines instead of n>0. Thus, it was not possible to clear only one line.
* [#16144](https://bugzilla.scilab.org/16144): Addition of sparse matrices gave incorrect results.
* [#16145](https://bugzilla.scilab.org/16145): `integrate` and `intg` default absolute tolerances atol were no longer the same. `integrate`'s default relative tolerance rtol was not the same in case of 3 or 4 input arguments.
* [#16148](https://bugzilla.scilab.org/16148): `setPreferencesValue` is a general usage function and deserved to be renamed `xmlSetValues`.
* [#16149](https://bugzilla.scilab.org/16149): `fullpath` did not support symbolic links in paths
* [#16152](https://bugzilla.scilab.org/16152): For sparse or boolean sparse matrix `s`, `s([])` returned `[]` instead of `sparse([])`.
* [#16155](https://bugzilla.scilab.org/16155): `flipdim(hypermat,dim,sb)` yielded wrong results for dim<3 and sb>1, and yielded an error for dim>2.
* [#16158](https://bugzilla.scilab.org/16158): When a multicolumn array of rationals was displayed wide column per column, columns #2:$ were replaced with its column #2.
* [#16160](https://bugzilla.scilab.org/16160): `ppol` changed values of third input variable.
* [#16162](https://bugzilla.scilab.org/16162): `noisegen()` and `prbs_a()` deserved to be moved to the elementary_functions module, gathered with `rand()` and `grand()` in a new `elementarymatrices/random` help subsection.
* [#16163](https://bugzilla.scilab.org/16163): `prbs_a()` did not comply with the requested number of switches. The biased number of actual switches was unpredictable, preventing to manage its statistical distribution. The probability of the initial state was biased. For instance, prbs_a(n,0) always returned ones(1,n).
* [#16164](https://bugzilla.scilab.org/16164): Help pages in elementary_functions/signal_processing were mislocated.
* [#16166](https://bugzilla.scilab.org/16166): `noisegen` is crooky, even threatening, and very poor. It is tagged as obsolete and will be removed from Scilab 6.1.x.
* [#16167](https://bugzilla.scilab.org/16167): When get(0).showHiddenHandles is "on", clf('reset') unexpectedly removed native menus.
* [#16168](https://bugzilla.scilab.org/16168): On some Linux installations, `test_run` always failed.
* [#16174](https://bugzilla.scilab.org/16174): `libraryinfo` yielded 0x0 matrix of strings for libs without macro
* [#16177](https://bugzilla.scilab.org/16177): In the uicontrol help page, the first example was bugged.
* [#16178](https://bugzilla.scilab.org/16178): `sci2exp` yielded an error for any input macro without output argument.
* [#16181](https://bugzilla.scilab.org/16181): `mfile2sci` yielded an error on `a(end)` standing for `a($)` (Scilab 6 regression).
* [#16190](https://bugzilla.scilab.org/16190): console width was fixed at 75 in nw and nwni mode regardless of terminal width.
* [#16197](https://bugzilla.scilab.org/16197): result of extraction was not decomplexified when applicable.
* [#16199](https://bugzilla.scilab.org/16199): In the documentation, `zpk2tf` and `zpk2ss` examples yielded an error.
* [#16200](https://bugzilla.scilab.org/16200): Concatenation of transposed cells crashed Scilab.
* [#16204](https://bugzilla.scilab.org/16204): `for i=1:$, end` crashed Scilab.
* [#16208](https://bugzilla.scilab.org/16208): Using 3D string matrix with old C-api gateways may crash Scilab.
* [#16209](https://bugzilla.scilab.org/16209): `grand` causes a freeze after several consecutive calls when using default base generator.
* [#16210](https://bugzilla.scilab.org/16210): The uicontrol.units = "normalized" property was not described.
* [#16215](https://bugzilla.scilab.org/16215): `mfile2sci` badly converted the colon `:` into `mtlb(:)`
* [#16223](https://bugzilla.scilab.org/16223): `EXPRESSION`, `CLR` and `DLR` Xcos blocks sometimes displayed multiplied terms in an ambiguous way in their icons.
* [#16227](https://bugzilla.scilab.org/16227): `WSCI` was not defined as environment variable and could not be used as `%WSCI%` in commands sent with `host` or `unix_*`.
* [#16228](https://bugzilla.scilab.org/16228): `bode` and `nyquist` inline demos did not clear and reset the current axes where to draw, what could yield an error.
* [#16229](https://bugzilla.scilab.org/16229): Some outdated parts of the `help scilab` page were misleading.
* [#16230](https://bugzilla.scilab.org/16230): `MSWin>scilab --help` missed displaying some options like `-args..`, `-noatomsautoload`, etc.
* [#16232](https://bugzilla.scilab.org/16232): `colorbar` did not support `$` in `colminmax`, standing for the number of colors in the current color map.
* [#16234](https://bugzilla.scilab.org/16234): Airy functions were not available.
* [#16242](https://bugzilla.scilab.org/16242): `loadmatfile` could not read Octave native text data files.
* [#16244](https://bugzilla.scilab.org/16244): `perms` could freeze the whole computer. It could not ignore duplicate permutations.
* [#16245](https://bugzilla.scilab.org/16245): `gsort` could not sort booleans.
* [#16246](https://bugzilla.scilab.org/16246): `isvector` was broken for sparse matrices.
* [#16257](https://bugzilla.scilab.org/16257): `blockdiag` implemented to replace `sysdiag`, improved and extended to strings.
* [#16259](https://bugzilla.scilab.org/16259): `*` and `.*` multiplications involving a sparse boolean and a double, or involving 2 booleans, were not implemented.
* [#16260](https://bugzilla.scilab.org/16260): overloading `nnz` was not possible (regression).
* [#16263](https://bugzilla.scilab.org/16263): Polynomial insertion was broken for complex case.
* [#16264](https://bugzilla.scilab.org/16264): After empty for loop iterator was left uninitialized.
* [#16265](https://bugzilla.scilab.org/16265): The translated pages of the `warning` documentation were not up-to-date.
* [#16269](https://bugzilla.scilab.org/16269): Scalar structures were poorly displayed.
* [#16271](https://bugzilla.scilab.org/16271): `loadmatfile` was never able to automatically detect the input data format.
* [#16272](https://bugzilla.scilab.org/16272): `spzeros(0,n)` and `spzeros(n,0)` were different from `sparse(0,0)`.
* [#16273](https://bugzilla.scilab.org/16273): `calendar` had no formated display mode.
* [#16275](https://bugzilla.scilab.org/16275): `fsolve(x0, fun, tol)` no longer took `tol` into account.
* [#16290](https://bugzilla.scilab.org/16290): The `cn`, `dn`, `ns`, `nc` and `nd` Jacobi elliptic functions were not available.
* [#16292](https://bugzilla.scilab.org/16292): `hallchart` had bugs and a poor rendering.
* [#16293](https://bugzilla.scilab.org/16293): Some demos run in step-by-step console mode(4) did not focus user's attention to the console to proceed.
* [#16299](https://bugzilla.scilab.org/16299): After `graypolarplot`, `colorbar` displayed an empty ungraduated color bar.
* [#16303](https://bugzilla.scilab.org/16303): log10(x) had wrong dimensions when x is an hypermatrix.
* [#16307](https://bugzilla.scilab.org/16307): `dockable="off"` and `closerequestfcn` on figures crached Scilab.
* [#16320](https://bugzilla.scilab.org/16320): There were unsupported UTF-8 characters in some gettext id messages.
* [#16321](https://bugzilla.scilab.org/16321): There were typo errors in the documentation.
* [#16323](https://bugzilla.scilab.org/16323): `conj(sparse(x))` was complex when x is real.
* [#16325](https://bugzilla.scilab.org/16325): `mgetl` could not read single line data which is greater than ~260,000 characters.
* [#16333](https://bugzilla.scilab.org/16333): `tree_show` crashed for an input Xcos block.
* [#16373](https://bugzilla.scilab.org/16373): Scilab Crashed or showing wrong output when running on Arm processor
* [#16459](https://bugzilla.scilab.org/16459): The display of one column hypermatrix was wrong.
* [#16463](https://bugzilla.scilab.org/16463): `matrix(sparse([]),[0 0])` yielded `[]` instead of `sparse([])`.
* [#16483](https://bugzilla.scilab.org/16483): substraction of complex polynomial matrices was broken (Scilab 6 regression).
* [#16498](https://bugzilla.scilab.org/16498): After `esb=sparse([%t %f]); esb(1,:)=[];` , `size(esb)` answered `[0 2]` instead of `[0 0]` .
