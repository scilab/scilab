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

// default values (written in the class) are checked and converted
p = PropCheck();
assert_checkequal(p.x, 1);
assert_checkequal(p.v, [0 0 0]);
assert_checkequal(p.r, 5);

// assigned values are checked, not converted: what is read is what was written
// size
p.x = 3;
assert_checkequal(p.x, 3);
size_msg = strsubst(_("%s: Wrong size of property ''%s'': %ls expected.\n"), "%ls", "%s");
assert_checkerror("p = PropCheck(); p.x = [1 2]", msprintf(size_msg, "PropCheck", "x", "1 x 1"));
assert_checkerror("p = PropCheck(); p.x(2) = 7", msprintf(size_msg, "PropCheck", "x", "1 x 1"));
assert_checkerror("p = PropCheck(); p.v = 4", msprintf(size_msg, "PropCheck", "v", "1 x 3"));
assert_checkerror("p = PropCheck(); p.v = [1;2;3]", msprintf(size_msg, "PropCheck", "v", "1 x 3"));
p.v = [1 2 3];
p.v(2) = 7;
assert_checkequal(p.v, [1 7 3]);

// type
type_msg = _("%s: Wrong type for property ''%s'': %s expected.\n");
assert_checkerror("p = PropCheck(); p.x = %t", msprintf(type_msg, "PropCheck", "x", "double"));
assert_checkerror("p = PropCheck(); p.x = ""12""", msprintf(type_msg, "PropCheck", "x", "double"));
assert_checkerror("p = PropCheck(); p.n = 3", msprintf(type_msg, "PropCheck", "n", "int8"));
assert_checkerror("p = PropCheck(); p.s = 12", msprintf(type_msg, "PropCheck", "s", "string"));
p.n = int8(3);
assert_checkequal(p.n, int8(3));
p.s = "12";
assert_checkequal(p.s, "12");

// validators
positive = _("%s: Wrong value for property ''%s'': Positive numbers expected.\n");
msg = msprintf(positive, "PropCheck", "x");
assert_checkerror("p = PropCheck(); p.x = -5", msg);
assert_checkerror("p.setX(-1)", msg);
assert_checkequal(p.x, 3);
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

// default value evaluated out of the definition: function call, variable
propDefault = 7;
classdef PropCallDefault
    properties
        z = zeros(2, 3)
        v = propDefault
    end
end

pd = PropCallDefault();
assert_checkequal(pd.z, zeros(2, 3));
assert_checkequal(pd.v, 7);

// static property of a class as validator parameter: evaluated at each check
classdef PropLimit
    properties (Static)
        MAX = 10
    end

    properties
        x {mustBeLessThan(x, PropLimit.MAX)} = 1
    end
end

pl = PropLimit();
pl.x = 5;
assert_checkequal(pl.x, 5);
assert_checkerror("pl = PropLimit(); pl.x = 20", msprintf(_("%s: Wrong value for property ''%s'': Must be < %s.\n"), "PropLimit", "x", "10"));
PropLimit.MAX = 100;
pl.x = 20;
assert_checkequal(pl.x, 20);

// the object can not be used
assert_checkerror("execstr([""classdef PropThis""; ""properties""; ""m = 1""; ""x {mustBeLessThan(x, this.m)} = 0""; ""end""; ""end""])", msprintf(_("%s: The validation of a property can not use the object.\n"), "PropThis.x"));

// errors in declarations
assert_checkerror("execstr(""classdef PropUnknownValidator; properties; x {mustBeFoo} = 1; end; end"")", msprintf(_("%s: Unknown validation function ''%s''\n"), "PropUnknownValidator.x", "mustBeFoo"));
// unknown type: can be a classdef defined later, checked with the value
classdef PropUnknownType
    properties
        x foo = 1
    end
end

assert_checkerror("PropUnknownType()", msprintf(_("%s: Unknown conversion function ''%s''\n"), "PropUnknownType.x", "foo"));
