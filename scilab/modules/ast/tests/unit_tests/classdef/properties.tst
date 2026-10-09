// ============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// -------------------------------------------------------------
// Size, type and validators of properties
// -------------------------------------------------------------
classdef PropCheck
    properties
        x (1,1) double {mustBePositive} = 1
        v (1,3) double = 0
        n int8
        r {mustBeInRange(r, 0, 10)} = 5
        s string = "a"
        free = 1
    end

    properties (Static)
        k (1,1) {mustBeInteger} = 2
    end

    methods
        function setX(val)
            this.x = val;
        end
    end
end

// default values are checked and converted
p = PropCheck();
assert_checkequal(p.x, 1);
assert_checkequal(p.v, [0 0 0]);
assert_checkequal(p.r, 5);

// size
p.x = 3;
assert_checkequal(p.x, 3);
msg = msprintf(strsubst(_("%s: Wrong size of property ''%s'': %ls expected.\n"), "%ls", "%s"), "PropCheck", "x", "1 x 1");
assert_checkerror("p = PropCheck(); p.x = [1 2]", msg);
p.v = 4;
assert_checkequal(p.v, [4 4 4]);
p.v = [1;2;3];
assert_checkequal(p.v, [1 2 3]);
p.v(2) = 7;
assert_checkequal(p.v, [1 7 3]);
assert_checkerror("p = PropCheck(); p.x(2) = 7", msg);

// type conversion
p.x = %t;
assert_checkequal(p.x, 1);
p.n = 3;
assert_checkequal(p.n, int8(3));
p.s = 12;
assert_checkequal(p.s, "12");
assert_checkerror("p = PropCheck(); p.x = ""str""", msprintf(strsubst(_("%ls: Unable to convert ""%ls"" to %s.\n"), "%ls", "%s"), "PropCheck.x", "str", "double"));

// validators
positive = _("%s: Wrong value for property ''%s'': Positive numbers expected.\n");
msg = msprintf(positive, "PropCheck", "x");
assert_checkerror("p = PropCheck(); p.x = -5", msg);
assert_checkerror("p.setX(-1)", msg);
assert_checkequal(p.x, 1);
p.r = 3;
assert_checkequal(p.r, 3);
assert_checkerror("p = PropCheck(); p.r = 11", msprintf(_("%s: Wrong value for property ''%s'': Must be in range [%s, %s].\n"), "PropCheck", "r", "0", "10"));

// no declaration, no check
p.free = list(1);
assert_checkequal(typeof(p.free), "list");

// static property
PropCheck.k = 4;
assert_checkequal(PropCheck.k, 4);
msg = msprintf(_("%s: Wrong value for property ''%s'': Integer numbers expected.\n"), "PropCheck", "k");
assert_checkerror("PropCheck.k = 2.5", msg);
assert_checkerror("p = PropCheck(); p.k = 1.5", msg);
assert_checkequal(PropCheck.k, 4);

// inherited property
classdef PropCheckChild < PropCheck
end

assert_checkerror("c = PropCheckChild(); c.x = -2", msprintf(positive, "PropCheckChild", "x"));

// wrong default value
classdef PropBadDefault
    properties
        x (1,1) {mustBePositive} = -1
    end
end

assert_checkerror("PropBadDefault()", msprintf(positive, "PropBadDefault", "x"));

// errors in declarations
assert_checkerror("execstr(""classdef PropUnknownValidator; properties; x {mustBeFoo} = 1; end; end"")", msprintf(_("%s: Unknown validation function ''%s''\n"), "PropUnknownValidator.x", "mustBeFoo"));
assert_checkerror("execstr(""classdef PropUnknownType; properties; x foo = 1; end; end"")", msprintf(_("%s: Unknown conversion function ''%s''\n"), "PropUnknownType.x", "foo"));
