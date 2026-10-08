---
name: debug
description: |
  Debugging, profiling and optimizing Scilab code: common error patterns and
  fixes, timing/profiling, vectorization, preallocation, memory management.
  Trigger words: "error", "bug", "doesn't work", "crash", "slow", "optimize",
  "performance", "profile", "vectorize", "out of memory".
triggers: error, erreur, erreurs, bug, plante, crash, doesn't work, ne marche pas, ne fonctionne pas, slow, lent, lente, optimize, optimiser, optimise, performance, performances, profile, profiler, profiling, vectorize, vectoriser, vectorise, memory, mémoire, out of memory, debug, debugger, déboguer, débogue, débugger, fuite, lenteur, accélérer, accélère, plus rapide, faster, speed up, temps d'exécution, execution time, goulot, bottleneck, undefined variable, invalid index, inconsistent, dimensions incompatibles, corrige, corriger, fix, profileenable, profilage, diverge, divergence, instable, instabilité, nan, inf, résultat faux, wrong result, bizarre, incohérent
---

# Scilab debugging & optimization

## Debugging workflow
1. Locate the error: `[msg, n, line, func] = lasterror()` gives the message,
   the LINE and the FUNCTION where it happened (the `last_error` tool gives
   the message). To reproduce safely: `execstr("myfun(2)", "errcatch")`
   then `lasterror()`.
2. Identify the source: check variable types with `typeof()`, sizes with `size()`
3. Inspect the workspace with `whos()` or `who("get")` (or the
   list_variables / get_variable tools)
4. Test isolated expressions with `execute_scilab_code`
5. Suggest fixes in the editor

NEVER put interactive/blocking calls in code you execute: `pause`, `debug()`,
`input()`, `halt`, `x_dialog`/`messagebox` without a timeout, `xclick`,
`odeoptions()`... — the interpreter waits for the console and the tool
call hangs. Debug by printing (`mprintf`, `disp`) and by `execstr` +
`lasterror` instead of breakpoints.

## Common error patterns and fixes

### "Undefined variable"
- Check spelling (case-sensitive)
- Ensure the `.sci` file was loaded: `exec("file.sci", -1)`
- Check scope: variables inside functions are local

### "Inconsistent row/column dimensions"
- Check matrix sizes: `size(A)`, `size(B)`
- Element-wise operations need `.*`, `./`, `.^` instead of `*`, `/`, `^`
- Concatenation `[A, B]` requires same number of rows
- Concatenation `[A; B]` requires same number of columns

### "Wrong number of input/output arguments"
- Check function signature with `scilab_help`
- Scilab functions don't support optional args the same way as MATLAB
- Use `argn()` inside functions: `[lhs, rhs] = argn(0)`

### "Warning: redefining function: xxx"
- Harmless when re-running a script that defines functions; silence it
  with `funcprot(0)` at the top of the script.

### "Invalid index" inside a solver / callback
- Often dynamic scoping: a variable of the calling code (or of the solver
  internals, e.g. `f` in NDcost) has the same name as one of your functions.
  Rename the function (`fobj`, `cost_xxx`).

### "Type mismatch"
- `typeof(x)` to check: "constant" (double), "string", "boolean", "sparse", etc.
- Convert: `double(x)`, `string(x)`, `int32(x)`, `bool2s(x)`

## Profiling
```scilab
// Simple timing
tic();
// ... code to profile ...
t = toc();
mprintf("Elapsed: %f seconds\n", t);

// Per-line profiling (profile/showprofile/add_profiling were REMOVED)
profileEnable(myfunction);        // or profileEnable() for all functions
myfunction(args);
p = profileGetInfo();
profileDisable();
p.FunctionTable                   // FunctionName, FileName, FirstLine, LastLine...
lc = p.LineCoverage(1);           // one matrix per profiled function:
                                  // row k = line k of the function, [hits, seconds]
[_, k] = max(lc(:, 2));           // hottest line (k-1 lines after FirstLine)
```
MEASURE FIRST, then optimize: run the profiler (or tic/toc per section)
on the original code and report where the time goes BEFORE rewriting.
Fixing anti-patterns that weigh 0.2 % of the runtime is noise; state the
measured share of each change in the answer.

## Performance optimization rules

### 1. Vectorize (most important)
```scilab
// BAD: loop
for i = 1:n
    y(i) = x(i)^2 + 1;
end

// GOOD: vectorized
y = x.^2 + 1;

// Broadcasting (implicit expansion), since Scilab 2026.1.0:
// column (n x 1) op row (1 x m) -> n x m matrix
D = sqrt((P(:,1) - P(:,1)').^2 + (P(:,2) - P(:,2)').^2);   // all pairwise distances
```
For code that must also run on Scilab < 2026.1, expand explicitly with
`ones`: `P(:,1) * ones(1, n) - ones(n, 1) * P(:,1)'`.

### 2. Preallocate arrays
```scilab
// BAD: growing array
result = [];
for i = 1:1000
    result = [result, compute(i)]; // copies entire array each time!
end

// GOOD: preallocated
result = zeros(1, 1000);
for i = 1:1000
    result(i) = compute(i);
end
```

### 3. Use built-in functions
```scilab
// BAD: manual sum
s = 0;
for i = 1:length(x)
    s = s + x(i);
end

// GOOD: built-in
s = sum(x);
```

### 4. Avoid string concatenation in loops
```scilab
// BAD
s = "";
for i = 1:1000
    s = s + string(i) + " ";
end

// GOOD
parts = string(1:1000);
s = strcat(parts, " ");
```

### 5. Sparse matrices for large sparse problems
```scilab
// Use sparse() for matrices with many zeros
A = sparse([1,1; 2,2; 3,3], [1; 2; 3], [1000, 1000]);
```

## Memory management
- Since Scilab 6 there is NO stack: memory is allocated dynamically from the
  machine's RAM. `stacksize()` and `gstacksize()` were REMOVED — never suggest
  them (they only existed in Scilab 5 and earlier).
- If an old script calls `stacksize(...)`, the fix is simply to delete the call.
- `clear x y z` — free specific variables
- `clearglobal` — free global variables
- `whos()` — inspect variables and their memory footprint (Bytes column);
  `[free, total] = getmemory()` — machine memory in kB
- "Out of memory" means the machine's RAM is exhausted: reduce data size, use
  `sparse()` matrices, or process data in chunks — there is no limit to raise.

## Guidelines
- Always start by reading the error message and inspecting variable types/sizes
- Use `execute_scilab_code` to test hypotheses before modifying the editor
- Suggest the smallest fix possible — don't rewrite the whole function
- When optimizing, explain WHY the optimization works (vectorization, preallocation, etc.)
