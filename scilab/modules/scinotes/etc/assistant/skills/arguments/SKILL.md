---
name: arguments
description: |
  Input validation in Scilab functions with the `arguments` block (2024.0+):
  size, type, validators (mustBe*), default values — and named arguments
  (name=value calls, checkNamedArguments). Syntax close to MATLAB's but
  with important differences (no name=value with an arguments block).
  Trigger words: "arguments", "validation", "mustBe", "default value",
  "named argument", "check inputs".
triggers: arguments, argument, bloc arguments, arguments block, validation, valider les entrées, vérifier les entrées, validate inputs, input validation, mustbe, mustbepositive, mustbemember, mustbea, validateur, validators, valeur par défaut, default value, defaults, argument nommé, arguments nommés, named argument, named arguments, name=value, checknamedarguments, nargin, varargin, paramètres optionnels, optional arguments, robuste, robust
---

# Scilab `arguments` block (2024.0+) and named arguments

## Syntax

```scilab
function r = smooth_mean(x, n, mode)
    arguments
        x (1, :) double {mustBeReal, mustBeFinite}
        n double {mustBeInteger, mustBePositive} = 3
        mode string {mustBeMember(mode, ["mean", "max"])} = "mean"
    end
    ...
endfunction
```
Each line: `name (dims) type {validators} = default`, every part optional.
- ALL inputs must be listed, in the prototype order; arguments with a
  default must come last. A default can use earlier arguments (`b = a * 2`).
- `varargin` may be listed (plain `varargin`) as the last entry.
- dims: `(1, 10)`, `(:, 3)`, `(1, :)`, alternatives `(4, [3 6 10])`.
  With FIXED sizes a scalar is expanded and a vector transposed to fit
  (`(1, 10)` accepts a 10x1 column). With `:` there is NO automatic
  transposition: `(1, :)` rejects a column ("Wrong size ... 1 x n expected")
  — use `{mustBeVector}` then `x = x(:)'` if both orientations are wanted.
- types: `double`, `boolean`, `string`, `int8`...`uint64`; convertible
  inputs are converted (an `int8` given for `double` becomes double).
  A classdef name is also a type (2027.0+): object of this class or of a
  subclass, no conversion. Unknown type names are reported at the first
  call ("f: Unknown conversion function 'float'"), not at definition.
- Errors are raised automatically with clear messages, e.g.
  "f: Wrong value for input argument #2: Integer numbers expected."

## Validators (inside `{ }`, comma-separated)

Short form (value implicit): `mustBePositive`, `mustBeNonnegative`,
`mustBeNegative`, `mustBeNonpositive`, `mustBeNonzero`, `mustBeReal`,
`mustBeFinite`, `mustBeNonNan`, `mustBeInteger`, `mustBeNumeric`,
`mustBeNumericOrBoolean`, `mustBeNonempty`, `mustBeScalar`,
`mustBeScalarOrEmpty`, `mustBeVector`, `mustBeRow`, `mustBeColumn`,
`mustBeSquare`, `mustBeNonsparse`, `mustBeNonzeroLengthText`,
`mustBeValidVariableName`, `mustBeFile`, `mustBeFolder`.

Call form (extra parameters, argument repeated):
`mustBeGreaterThan(x, c)`, `mustBeGreaterThanOrEqual(x, c)`,
`mustBeLessThan(x, c)`, `mustBeLessThanOrEqual(x, c)`,
`mustBeInRange(x, lo, hi [, "exclusive"])`, `mustBeMember(x, set)`,
`mustBeA(x, ["double", "string"])`, `mustBeEqualDims(x, y [, dims])`,
`mustBeEqualDimsOrScalar(x, y)`, `mustBeSameType(x, y)`.

Validator parameters must be a CONSTANT, the plain NAME of another input,
or (2027.0+) a value evaluated at each call: a field of an input
(`opts.max`) and, in classdef methods, `this`, `this.balance` or a static
property `MyClass.limit`. Other expressions are refused:
- `n double {mustBeLessThanOrEqual(n, nmax)}` with `nmax` an input: OK
  ("Must be <= 5");
- `mustBeLessThanOrEqual(n, size(x, "*"))`: rejected when the function is
  defined ("argument must be constant expression").
Derived checks (n <= length(x)...) go in the body:
`if n > size(x, "*") then error(...); end`. An invalid validator
expression is also reported at definition ("f: Unable to evaluate argument.").

## Named arguments (name=value calls)

Scilab binds `f(a=1, b=2)` to the PARAMETER NAMES of the prototype (any
order, mixed with positional ones). A parameter not given is simply
undefined:
```scilab
function r = scaled(x, gain, offset)
    if checkNamedArguments() <> [] then       // unknown names used
        error("scaled: unknown named argument(s): " + strcat(checkNamedArguments(), ", "));
    end
    if ~isdef("gain", "l") then gain = 1; end
    if ~isdef("offset", "l") then offset = 0; end
    r = gain * x + offset;
endfunction
scaled(1:3, offset=10)
```

**Named arguments are NOT compatible with an `arguments` block**: calling
a function that has one with `name=value` fails ("Named argument are not
compatible with arguments block"). Choose per function:
- strict validation of positional inputs -> `arguments` block;
- options by name -> plain parameters + `isdef(..., "l")` defaults +
  `checkNamedArguments()`, and validate by hand with `if ... error(...)`:
  the `mustBe*` validators exist ONLY inside an arguments block (calling
  `mustBePositive(x)` elsewhere gives "Undefined variable").

MATLAB's `arguments ... opts.Name = value` (options struct) does NOT exist.

## In classdef methods

Works the same; `this` is implicit and not declared:
```scilab
function add(v)
    arguments
        v (1, 1) double {mustBePositive, mustBeLessThanOrEqual(v, this.limit)}
    end
    this.total = this.total + v;
end
```
