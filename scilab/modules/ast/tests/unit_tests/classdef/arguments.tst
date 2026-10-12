// ============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

classdef test
    properties
        value = 0
    end

    methods
        function test(val)
            arguments
                val = 10;
            end

            this.value = val;
        end
    end
end

assert_checkequal(test().value, 10);

// -------------------------------------------------------------
// validators of methods using the object or the class: evaluated at each call
// -------------------------------------------------------------
classdef ArgLim
    properties
        maxv = 10
    end

    properties (Static)
        limit = 5
    end

    methods
        function r = belowMax(v)
            arguments
                v {mustBeLessThan(v, this.maxv)}
            end
            r = v;
        end

        function r = belowLimit(v)
            arguments
                v {mustBeLessThan(v, ArgLim.limit)}
            end
            r = v;
        end

        function r = sameAs(o)
            arguments
                o {mustBeSameType(o, this)}
            end
            r = typeof(o);
        end

        function r = typed(o)
            arguments
                o ArgLim
            end
            r = typeof(o);
        end
    end
end

classdef ArgLimChild < ArgLim
end

o = ArgLim();
msg = _("%s: Wrong value for input argument #%d: Must be < %s.\n");

// property of the object
assert_checkequal(o.belowMax(3), 3);
assert_checkerror("o = ArgLim(); o.belowMax(30)", msprintf(msg, "ArgLim.belowMax", 1, "10"));
o.maxv = 50;
assert_checkequal(o.belowMax(30), 30);

// static property of the class
assert_checkequal(o.belowLimit(2), 2);
assert_checkerror("o = ArgLim(); o.belowLimit(7)", msprintf(msg, "ArgLim.belowLimit", 1, "5"));
ArgLim.limit = 100;
assert_checkequal(o.belowLimit(7), 7);

// the object itself
assert_checkequal(o.sameAs(ArgLim()), "ArgLim");
assert_checkerror("o = ArgLim(); o.sameAs(1)", msprintf(_("%s: Wrong type for input argument #%d: Must be same type of #%s.\n"), "ArgLim.sameAs", 1, "this"));

// field of another input: struct, tlist, mlist, object
function r = argWithOpts(opts, v)
    arguments
        opts
        v {mustBeLessThan(v, opts.max)}
    end
    r = v;
end

s.max = 4;
assert_checkequal(argWithOpts(s, 3), 3);
assert_checkerror("s.max = 4; argWithOpts(s, 9)", msprintf(msg, "argWithOpts", 2, "4"));
tl = tlist(["T", "max"], 4);
assert_checkequal(argWithOpts(tl, 3), 3);
assert_checkerror("tl = tlist([""T"", ""max""], 4); argWithOpts(tl, 9)", msprintf(msg, "argWithOpts", 2, "4"));
ml = mlist(["M", "max"], 4);
assert_checkequal(argWithOpts(ml, 3), 3);
assert_checkerror("ml = mlist([""M"", ""max""], 4); argWithOpts(ml, 9)", msprintf(msg, "argWithOpts", 2, "4"));
classdef ArgBox
    properties
        max = 6
    end
end

assert_checkequal(argWithOpts(ArgBox(), 3), 3);
assert_checkerror("argWithOpts(ArgBox(), 9)", msprintf(msg, "argWithOpts", 2, "6"));

// nested field, and expression shown in the message
function r = argSameDims(opts, v)
    arguments
        opts
        v {mustBeEqualDims(v, opts.lim.dims)}
    end
    r = v;
end

s.lim.dims = [1 2];
assert_checkequal(argSameDims(s, [3 4]), [3 4]);
assert_checkerror("s.lim.dims = [1 2]; argSameDims(s, 1:3)", msprintf(_("%s: Wrong size for input argument #%d: Must be of the same dimensions of #%s.\n"), "argSameDims", 2, "opts.lim.dims"));

// evaluation failure: the cause is given
unable = _("%s: Unable to evaluate argument.\n");
assert_checkerror("argWithOpts(struct(""other"", 1), 3)", [msprintf(unable, "argWithOpts"); msprintf(strsubst(_("Unknown field : %ls.\n"), "%ls", "%s"), "max")]);

// this outside of a method
function r = argPlainThis(v)
    arguments
        v {mustBeLessThan(v, this)}
    end
    r = v;
end

assert_checkerror("argPlainThis(1)", [msprintf(unable, "argPlainThis"); msprintf(_("Undefined variable: %s\n"), "this")]);

// -------------------------------------------------------------
// class as type: an object of this class or of a derived one, no conversion
// -------------------------------------------------------------
assert_checkequal(o.typed(ArgLim()), "ArgLim");
assert_checkequal(o.typed(ArgLimChild()), "ArgLimChild");
assert_checkerror("o = ArgLim(); o.typed(1)", msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "ArgLim.typed", 1, "ArgLim"));

// class known at call time only
function r = argTypedLater(o)
    arguments
        o ArgLater
    end
    r = typeof(o);
end

assert_checkerror("argTypedLater(1)", msprintf(_("%s: Unknown conversion function ''%s''\n"), "argTypedLater", "ArgLater"));

classdef ArgLater
end

assert_checkequal(argTypedLater(ArgLater()), "ArgLater");

// class as type of a property
classdef ArgHolder
    properties
        item ArgLim
    end
end

h = ArgHolder();
h.item = ArgLimChild();
assert_checkequal(typeof(h.item), "ArgLimChild");
assert_checkerror("h = ArgHolder(); h.item = 3", msprintf(_("%s: Wrong type for property ''%s'': %s expected.\n"), "ArgHolder", "item", "ArgLim"));
