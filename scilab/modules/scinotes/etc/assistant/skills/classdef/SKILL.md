---
name: classdef
description: |
  Object-oriented programming in Scilab with classdef (Scilab 2026.0+):
  properties, methods with implicit `this`, constructors, inheritance,
  visibility, Static members (2026.1+), enumerations, operator overloading.
  Scilab's classdef looks like MATLAB's but differs deeply (implicit this,
  handle semantics, overload method names). Trigger words: "classdef",
  "class", "object", "method", "inheritance", "constructor", "enumeration".
triggers: classdef, classe, classes, class, objet, objets, object, objects, oop, poo, orienté objet, object-oriented, méthode, méthodes, method, methods, héritage, hériter, inheritance, inherit, constructeur, constructor, enumeration, énumération, enum, this, surcharge d'opérateur, surcharger, operator overloading, overload, static, statique, saveobj, loadobj
---

# Scilab classdef (2026.0+)

classdef is RECENT (2026.0, Static in 2026.1) and NOT MATLAB's: do not
transpose MATLAB habits. Verify unusual constructs with a quick
execute_scilab_code test.

## Syntax

```scilab
classdef Point < Shape & Named          // inheritance: <, multiple with &
    properties                          // (public by default)
        x = 0                           // default value ([] if none)
        y = 0
    end
    properties (private)                // private | protected | public
        cache = []
    end
    properties (Static)                 // shared by all instances (2026.1+)
        count = 0
    end

    methods
        function Point(a, b)            // CONSTRUCTOR: same name, NO output
            if argn(2) >= 2 then        // optional args: argn / nargin
                this.x = a;  this.y = b;
            end
            Point.count = Point.count + 1;
        end

        function r = norm()             // `this` is IMPLICIT: not a parameter
            r = sqrt(this.x^2 + this.y^2);
        end

        function move(dx, dy)           // modifies the object in place
            this.x = this.x + dx;  this.y = this.y + dy;
        end

        function disp()                 // custom display
            mprintf("Point(%g, %g)\n", this.x, this.y);
        end
    end

    methods (Static)
        function r = origin()           // no `this`; call Point.origin()
            r = Point(0, 0);
        end
    end
end

p = Point(3, 4);   p.norm()   // 5
p.move(1, 1);                 // p is now (4, 5)
```

Static methods have no `this`: they reach static members through the class
name (`Point.count`), whatever their visibility, and can call a private
constructor (factory methods). To work on an object, pass it as argument.
In instance methods, `this.count` and `Point.count` are the same shared
value: prefer the class name. The constructor is never static: keep it in a
plain `methods` block (in a `methods (Static)` block the class definition
fails with "Constructor of 'X' can not be static").

Property declarations (2027.0+) accept the `arguments` block syntax: size
`(1,3)`, type `double`/`int8`/`string`/..., validators `{mustBePositive}`,
`{mustBeInRange(x, 0, 10)}`, type can be a class name. Validator parameters:
the property itself, constants, static properties `MyClass.MAX` (evaluated
at each check) — NOT `this` or other properties ("can not use the object"):
check such constraints in a setter method with an `arguments` block. The
DEFAULT value is converted/expanded like a function argument
(`n int8 = 1` gives int8(1)), but an ASSIGNED value is NOT converted: it must
already have the declared type and size (`p.n = 3` on an int8 property
fails: "Wrong type for property 'n': int8 expected"; write `int8(3)`).

| MATLAB habit | Scilab classdef |
|---|---|
| `function obj = Point(a, b)` constructor | `function Point(a, b)` — no output, assign `this.x` |
| `function r = norm(obj)` | `function r = norm()` — use `this`, not a first argument |
| value class: `q = p` copies | **HANDLE semantics**: `q = p` shares the object; a function receiving `p` modifies the caller's object |
| `obj = obj.move(1)` to update | `p.move(1)` — mutations through `this` persist |
| `nargin` in constructor | `argn(2)` (or `nargin`) |
| `[p1, p2]` object array | not native: only works if the class defines `horzcat`/`vertcat` (see below); otherwise use `list(p1, p2)` |
| `isa`, `properties`, `methods` | same: `isa(p, "Point")`, `properties(p)`, `methods(p)` |

Need an independent copy? Build a new object explicitly (e.g. a `copy()`
method returning `Point(this.x, this.y)`).

## Visibility, errors

- `private`: only inside this class's methods; `protected`: + all
  subclasses (any depth); `public` (default). Access from outside a private
  property: "Wrong extraction: property 'x' is not accessible."
- Rights belong to the class of the running method, not to the object: a
  method can read private members of ANOTHER instance of its class, but a
  plain function called from a method cannot read private members of `this`.
- Assigning an UNDECLARED property (`p.z = 3`) fails: "Wrong insertion:
  property 'z' does not exist."
- `typeof(p)` returns the class name ("Point"); `isfield` does not work on
  objects — use `or(properties(p) == "z")`.

## Inheritance

- Subclass properties shadow base ones (base methods then see the derived
  value). Multiple inheritance with `&`: on conflicts the FIRST listed
  parent wins.
- Methods can be overridden by redefining them in the subclass.
- Parent constructor: call it by its class name inside the child
  constructor, it acts on the current `this` (there is no `super`):
```scilab
classdef Savings < Account
    properties
        rate = 0.02
    end
    methods
        function Savings(owner, balance, rate)
            Account(owner, balance);        // runs Account's constructor on this
            if argn(2) >= 3 then this.rate = rate; end
        end
    end
end
```
  Without output only (`x = Account(...)` builds another object), and only
  in the constructor of a DIRECT subclass (elsewhere: "can only be called
  from the constructor of a direct subclass"). A parent constructor not
  called this way (or when the child has no constructor) is called
  implicitly WITHOUT argument before the child constructor (2027.0+): a
  parent constructor that needs arguments must be called explicitly.
- Parent methods (constructor included) keep access to the parent's private
  properties when `this` is a subclass instance; subclass methods do not:
  declare `protected` what subclasses must use.

## Enumerations

```scilab
classdef Color
    properties
        R; G; B
    end
    methods
        function Color(r, g, b)
            this.R = r; this.G = g; this.B = b;
        end
    end
    enumeration
        RED   (255, 0, 0)            // constructor arguments
        GREEN (0, 255, 0)
    end
end
c = Color.RED;
```

The constructor of an enumeration can be private: values are built by the
class itself.

## Operator overloading — methods with RESERVED names

Define a method named after the operation; both operands are explicit
arguments (no `this`), one of them is the object:

| op | method | op | method | op | method |
|---|---|---|---|---|---|
| `a + b` | `plus(a, b)` | `a - b` | `minus` | `-a` | `uminus(a)` |
| `a * b` | `mtimes` | `a .* b` | `times` | `a / b` | `mrdivide` |
| `a ./ b` | `rdivide` | `a ^ b` | `mpower` | `a .^ b` | `power` |
| `a == b` | `eq` | `a <> b` | `ne` | `a < b` | `lt` (`le`, `gt`, `ge`) |
| `a & b` | `and` | `a \| b` | `or` | `~a` | `not` |
| `a'` | `ctranspose` | `[a b]` | `horzcat` | `[a; b]` | `vertcat` |
| `a(i, j)` | `extract(i, j)` | `a(i) = b` | `insert(i, b)` | | |

Type-specific variants take priority: `plus_s` (object + double), `plus_i`
(integers)... then `plus`, then Scilab's `%type_op_type` overloads. The
`_s` variant handles both orders (`v * 3` and `3 * v`): test `isa(a, ...)`
to know which operand is the object. With two objects, the left operand's
methods are searched first, then the right one's. `&&` / `||` call `and` /
`or` like `&` / `|`.

```scilab
function r = plus(a, b)
    if isa(a, "Vec") & isa(b, "Vec") then
        r = Vec(a.v + b.v);
    else
        error("Vec + " + typeof(b) + " not defined");
    end
end
```

Operator methods are reached ONLY through the operator: `eq(u, v)` or
`u.eq(v)` fail ("Attempt to reference field of non-structure array"). To
reuse one in another, use the operator itself:
`function r = ne(a, b), r = ~(a == b); end`.

Overriding a Scilab function for the class: define a method with the same
name (`disp()`, ...).

### Concatenation `[a, b]` / `[a; b]`

Objects cannot be concatenated natively: WITHOUT `horzcat`/`vertcat`
methods, `[a, b]` raises "Undefined operation for the given operands". The
class decides what concatenation means by defining them:
```scilab
function r = horzcat(a, b)            // [a, b, c] chains: horzcat(horzcat(a, b), c)
    if typeof(a) == "list" then r = a; else r = list(a); end
    r($+1) = b;                       // here: a list of objects
end
function r = vertcat(a, b)            // [a; b]: e.g. matrix of the coordinates
    r = [a.v; b.v];
end
```
The second call of a chain receives what the first returned (a list
here): handle it. Without these methods, store objects in `list(...)`.

## Life cycle and persistence

- `delete()` is called when the object is cleared.
- `save` REFUSES objects unless the class implements
  `function data = saveobj()` (return a struct/list of native values) and
  `function loadobj(data)` (restore `this` from it).

## Comments: NOT at class-body level

A comment line directly inside `classdef ... end` — before the first
block or BETWEEN two blocks — is a syntax error ("unexpected line comment,
expecting enumeration or methods or properties or end"). Put comments
above the `classdef` line, at the end of the `classdef` line, or INSIDE a
`properties`/`methods` block:
```scilab
// Point in the plane                  <- OK (before classdef)
classdef Point                         // OK (same line)
    properties
        // coordinates                 <- OK (inside a block)
        x = 0
    end
    // helpers                         <- SYNTAX ERROR (between blocks)
    methods
    end
end
```

## Workflow notes

- A classdef block is defined by executing the script that contains it;
  re-running it after an edit REDEFINES the class, and existing instances
  pick up the new methods. `clear ClassName` does not remove the class.
- Arguments of methods can be validated with an `arguments` block (see
  the arguments skill) — `this` is not declared in it. A class name can be
  used as type (`o Point`: object of Point or a subclass, no conversion),
  and validators can use `this`, `this.max` or `Point.max`, evaluated at
  each call: `v {mustBeLessThan(v, this.max)}`.
- Keep one class per file for anything reusable and `exec` it before use.
