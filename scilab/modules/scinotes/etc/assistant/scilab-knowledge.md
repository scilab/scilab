# Scilab Language Reference (for AI assistants)

This document corrects common LLM mistakes when generating Scilab code.
Scilab is NOT MATLAB. They share some syntax but have critical differences.

## Comments
- Scilab: `// comment` or `/* block comment */`
- MATLAB uses `%` — NEVER use `%` for comments in Scilab

## String handling
- Scilab: `"hello"` or `'hello'` (both work, double-quote preferred)
- Escaping: double the quote character inside the string — NOT backslash
  - `"He said ""hello"""` produces: He said "hello"
  - `'It''s done'` produces: It's done
  - This applies to BOTH single and double quoted strings
  - There is NO `\n`, `\t` escape — use `ascii(10)` for newline, `ascii(9)` for tab
- Concatenation: `"hello" + " world"` or `strcat(["hello", " world"])`
- Convert number to string: `string(42)` — NOT `num2str(42)`
- Convert string to number: `strtod("42")` or `evstr("42")` — NOT `str2num`

## Variable display
- `disp(x)` displays a variable
- Scilab has NO `fprintf` to stdout — use `mprintf("format", args)`
- `mprintf` uses C-style format: `%d`, `%f`, `%s`, `%g`

## Boolean
- `%t` and `%f` (NOT `true`/`false`)
- Logical operators: `&` (and), `|` (or), `~` (not)

## Special constants
- `%pi`, `%e`, `%i` (imaginary unit), `%inf`, `%nan`, `%eps`
- `%t` (true), `%f` (false)
- Empty matrix: `[]`

## Matrix operations
- Same as MATLAB: `A * B` (matrix multiply), `A .* B` (element-wise)
- Transpose: `A'` (conjugate transpose) or `A.'` (simple transpose)
- Range: `1:10`, `1:0.5:10`
- Indexing: 1-based, `A(row, col)`, `A(:, 1)` for first column
- End index: `A(2:$, :)` uses `$` — MATLAB uses `end`, NOT the same

## Broadcasting / implicit expansion (since Scilab 2026.1)
Element-wise operators automatically expand singleton dimensions (size 1),
like NumPy broadcasting or MATLAB implicit expansion:
```scilab
[1 2] .* [1 2]'          // -> [1 2; 2 4]  (row .* column = outer product)
A - mean(A, "r")         // center each column of A (3x4 minus 1x4)
x ./ sum(x, "c")         // normalize each row (3x4 ./ 3x1)
M + v                    // add column vector v to every column of M
```
- Covered operators: `+ - .* ./ .^ & | == ~= < <= > >=`
- Works on doubles, complex, integers, strings, polynomials, hypermatrices
- Dimensions must match or be 1 — `rand(2,3) + rand(4,1)` is still an error
- PREFER broadcasting over the old idioms: no need for `repmat(v, n, 1)` or
  `v .* ones(n, m)` or `ones(n,1) * v` tricks — write the operation directly.
  Do NOT use these workarounds in generated code targeting Scilab >= 2026.1.

## Function definition
```scilab
function [out1, out2] = myfunction(in1, in2)
    // function body
    out1 = in1 + in2;
    out2 = in1 - in2;
endfunction
```
- Use `endfunction` (NOT `end`)
- Multiple return values in square brackets
- Ignore a return value: use `_` — NOT `~` like MATLAB. Example: `[_, imax] = max(x)`
- Functions are typically saved in `.sci` files
- Load with `exec("myfunction.sci", -1)` — the `-1` suppresses output
- Scilab also supports `classdef` OOP (since 2024.1) — only use if the user explicitly asks for classes

## Arguments validation (since Scilab 2024.0)
Scilab supports argument validation blocks inside functions, similar to MATLAB's `arguments` block:
```scilab
function result = compute(x, y, method)
    arguments
        x (1, :) double {mustBeReal, mustBePositive}
        y (:, 1) double {mustBeReal}
        method string = "linear"
    end
    // function body
endfunction
```
- `arguments` block must be the first expression in the function
- Syntax: `argName (dimensions) datatype {validators} = defaultValue`
- Dimensions: `(rows, cols)` — use `:` for any size, integers for fixed size
- Supported types: `double`, `int8/16/32/64`, `uint8/16/32/64`, `boolean`, `string`
- Default values: parameters with defaults must be at the end
- Scalar inputs are automatically expanded to match expected dimensions
- Vectors are automatically transposed if needed for dimension matching

Available validators:
- Value checks: `mustBeReal`, `mustBeFinite`, `mustBeNonNan`, `mustBeInteger`, `mustBeNumeric`, `mustBeNumericOrBoolean`
- Sign checks: `mustBePositive`, `mustBeNegative`, `mustBeNonnegative`, `mustBeNonpositive`, `mustBeNonzero`
- Range: `mustBeGreaterThan(x, c)`, `mustBeGreaterThanOrEqual(x, c)`, `mustBeLessThan(x, c)`, `mustBeLessThanOrEqual(x, c)`, `mustBeInRange(x, lo, hi [, bounds])` (bounds: "inclusive"/"exclusive"/"exclude-lower"/"exclude-upper")
- Shape: `mustBeScalar`, `mustBeScalarOrEmpty`, `mustBeVector`, `mustBeRow`, `mustBeColumn`, `mustBeSquare`, `mustBeNonempty`, `mustBeNonsparse`
- Membership: `mustBeMember(x, [1 3 5 7])`
- Type: `mustBeA(x, ["double", "string"])` — also supports graphic types like `"figure"`, `"polyline"`, UI types like `"frame"`, `"pushbutton"`, and `"empty"`
- Comparison: `mustBeSameType(x, y)`, `mustBeEqualDims(x, y)`, `mustBeEqualDimsOrEmpty(x, y)`, `mustBeEqualDimsOrScalar(x, y)` — all accept optional `dims` parameter
- String: `mustBeNonzeroLengthText`, `mustBeValidVariableName`
- Path: `mustBeFile`, `mustBeFolder`

## Control flow
```scilab
// if/elseif/else
if condition then
    // ...
elseif other then
    // ...
else
    // ...
end

// for loop
for i = 1:10
    // ...
end

// while loop
while condition
    // ...
end

// select (switch/case)
select variable
case value1 then
    // ...
case value2 then
    // ...
else
    // ...
end
```
- `if` supports optional `then` keyword (syntactic sugar, recommended for readability)
- `select/case` instead of `switch/case`
- `case` supports optional `then` keyword

## Error handling
```scilab
try
    // risky code
catch
    // handle error
    disp(lasterror())
end
```
- `lasterror()` returns the last error message
- NO `try/catch` with exception variable like MATLAB's `catch ME`

## File I/O
- Write lines to file: `mputl(["line1"; "line2"], "file.txt")`
- Read lines from file: `lines = mgetl("file.txt")`
- Check file exists: `isfile("path")`
- Delete file: `mdelete("path")`
- Current directory: `pwd()`
- Change directory: `cd("path")`
- Path separator: use `filesep()` or `pathconvert()`
- Full path: `fullfile("dir", "subdir", "file.ext")`

## Common functions (Scilab names)
| Operation | Scilab | MATLAB (DO NOT USE) |
|-----------|--------|---------------------|
| Display | `disp(x)` | same |
| Print formatted | `mprintf()` | `fprintf` |
| Number to string | `string(x)` | `num2str(x)` |
| String to number | `strtod(s)` | `str2num(s)` |
| String length | `length(s)` | same |
| Find in string | `strindex(s, pattern)` | `strfind` |
| Replace in string | `strsubst(s, old, new)` | `strrep` |
| Type of variable | `typeof(x)` | `class(x)` |
| Size of matrix | `size(x)` or `size(x, "r")`, `size(x, "c")` | `size(x,1)`, `size(x,2)` |
| Zeros matrix | `zeros(n, m)` | same but BEWARE: `zeros(A)` returns a zero matrix the same size as A, NOT `zeros(A(1), A(2))` like MATLAB |
| Ones matrix | `ones(n, m)` | same but BEWARE: `ones(A)` returns a ones matrix the same size as A, NOT `ones(A(1), A(2))` like MATLAB |
| Identity | `eye(n, n)` | same |
| Random | `rand(n, m)` | same |
| Linear space | `linspace(a, b, n)` | same |
| Sort | `gsort(x)` (descending default) | `sort(x)` |
| Find nonzero | `find(x)` | same |
| Field names | `fieldnames(s)` | same |
| Struct creation | `s = struct("field1", val1, "field2", val2)` | `s = struct(...)` (similar) |

## Plotting
```scilab
plot(x, y)           // basic 2D plot
plot(x, y, "r-")     // red solid line
xlabel("X axis")
ylabel("Y axis")
title("My plot")
legend(["curve1", "curve2"])
clf()                // clear figure
scf()                // create new figure
gcf()                // get current figure
gca()                // get current axes
```
- Property access: `gca().font_size = 3` or `set(gca(), "font_size", 3)`
- Scilab graphic properties use underscores: `font_size`, `line_style`, `mark_size`

## Removed functions (Scilab 2027.0) — NEVER generate them
These no longer exist: calling them is an error. When fixing an old script,
replace them:
| Removed (DO NOT USE) | Use instead | Removed in |
|----------------------|-------------|------------|
| `unix_g`, `unix_s`, `unix_w`, `unix_x`, `unix`, `dos` | `host` (see below) | 2027.0 |
| `readmps` | `quapro` ATOMS toolbox | 2027.0 |
| `demo_begin`, `demo_run`, `demo_end` | `exec` | 2026.1 |
| `demo_choose`, `demo_file_choice`, `demo_function_choice` | `x_choose` | 2026.1 |
| `demo_compiler` | `haveacompiler` | 2026.1 |
| `dassl`, `dasrt`, `daskr`, `impl` | `dae` (or `ida`) | 2026.0 |
| `captions` | `legend` | 2026.0 |
| `princomp` | `pca` | 2026.0 |
| `testmatrix` | `magic`, `invhilb`, `frank` | 2026.0 |
| `jetcolormap(n)`, `hotcolormap(n)`, ... (all `*colormap`) | `jet(n)`, `hot(n)`, ... (drop the suffix) | 2026.0 |
| `getURL`, `splitURL` | `http_get`, `url_split` | 2025.0 |
| `sound` | `playsnd` | 2025.0 |
| `xget`, `xset` | graphic handle properties (`gcf()`, `gca()`) | 2024.0 |
| `plotframe`, `fplot2d` | `plot2d`, `plot` | 2024.0 / 2023.0 |
| `xdel`, `xname` | `close`, `gcf().figure_name` | 2023.0 |

OS commands: `host` is the ONLY way to run a shell command:
```scilab
[status, out, err] = host("ls -l")   // out, err: column vectors of lines
[status, out] = host("make")          // 2 outputs: stdout+stderr merged, in order
host("ping localhost", echo=%t)       // print outputs live in the console
```
- `unix_g(cmd)` → `[status, out] = host(cmd)`; `unix_s(cmd)` → check
  `host(cmd) == 0`; `unix_w`/`unix_x` → `host(cmd, echo=%t)` (or
  `messagebox(out)`).
- `host` runs `sh` on Linux/macOS and `cmd.exe` on Windows (`powershell()`
  also exists on Windows); test `getos() == "Windows"` for OS-specific commands.

## Obsolete functions (still work in 2027.0, removed in 2027.1)
Do not use them in new code; `getdeprecated()` lists them at runtime:
- `xmltojar()` → `tbx_build_help()` (no help jar is built anymore).
- `tbx_make(path, "help", languages)` → `tbx_make(path, "help")`: help
  images are generated for all languages at once.

## Script vs Function files
- `.sce` files: scripts (executed line by line)
- `.sci` files: function definitions (loaded, not executed)
- Load functions: `exec("functions.sci", -1)`
- Run script: `exec("script.sce")`

## Workspace
- List variables: `whos()` or `who("get")`
- Clear all: `clear`
- Clear specific: `clear x y z`
- Save workspace: `save("file.sod", x, y)` (Scilab binary format)
- Load workspace: `load("file.sod")`

## Memory model
- Since Scilab 6 there is NO stack: memory is allocated dynamically from the
  machine's RAM. `stacksize()` and `gstacksize()` were REMOVED — NEVER suggest
  them. If an old script calls them, the fix is to delete the call.

## Recent built-in functions (do NOT claim they are missing)
These may be newer than your training data — they ARE built into Scilab, no
toolbox needed. Check signatures with scilab_help:
- Clustering (statistics module): `kmeans` (2025.0), `dbscan`, `meanshift`,
  `estimate_bandwidth` (2026.0) — e.g. `idx = kmeans(X, k)`
- Data types: `table`, `timeseries` with `readtable`, `join`, `pivot`,
  `groupsummary`, `retime`... (2024.0) — see the data-import skill
- File formats: `parquetRead`/`parquetWrite`, `xlsxRead`/`xlsxWrite` (2026.1)
