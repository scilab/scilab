// ============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2023 - Dassault Systèmes S.E. - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
classdef Foo
    enumeration
        A
        B
        C
    end
end

classdef Colors
    properties
        R
        G
        B
    end

    methods
        function Colors(r, g, b)
            this.R = r;
            this.G = g;
            this.B = b;
        end
    end

    enumeration
        Blueish   (18/255,104/255,179/255)
        Reddish   (237/255,36/255,38/255)
        Greenish  (155/255,190/255,61/255)
        Purplish  (123/255,45/255,116/255)
        Yellowish (1,199/255,0)
        LightBlue (77/255,190/255,238/255)
    end
end

classdef CarPainter
    methods
        function paint(carobj, colorobj)
        end
    end
end

classdef Cars < CarPainter
    properties (private)
        Cylinders
        Transmission
        MPG
        Color
    end

    methods
        function Cars(cyl, trans, mpg, color)
            this.Cylinders = cyl;
            this.Transmission = trans;
            this.MPG = mpg;
            this.Color = color;
        end

        function paint(color)
            if isa(color, 'Colors')
                this.Color = color;
            else
                disp('Not an available color')
            end
        end
    end

    enumeration
        Hybrid      (2, 'Manual',    55, Colors.Reddish)
        Compact     (4, 'Manual',    32, Colors.Greenish)
        MiniVan     (6, 'Automatic', 24, Colors.Blueish)
        SUV         (8, 'Automatic', 12, Colors.Yellowish)
    end
end

classdef BasicClass
    properties
        Value
    end

    methods
        function BasicClass(val)
            if nargin == 1
                this.Value = val;
            end
        end

        function r = roundOff()
            r = round(this.Value, 2);
        end

        function r = multiplyBy(n)
            r = this.Value * n;
        end

        function r = plus(o1,o2)
            r = o1.Value + o2.Value;
        end
    end
end

// =============================================================
// Inheritance cycle detection
// =============================================================
function expect_err(exprstr)
    err = execstr(exprstr, 'errcatch');
    assert_checkfalse(err == 0);
endfunction

classdef CycleA < CycleB
end

classdef CycleB < CycleA
end

expect_err("CycleA()");
expect_err("CycleB()");
// the error is the same at each access
msg = msprintf(_("Inheritance cycle detected for class ''%s''.\n"), "CycleA");
assert_checkerror("CycleA()", msg);
assert_checkerror("CycleA()", msg);

// =============================================================
// Failure while loading a class: same error at each access
// =============================================================
classdef LoadFail
    properties (Static)
        k (1,1) {mustBePositive} = -1
    end
end

classdef LoadFailChild < LoadFail
end

msg = msprintf(_("%s: Wrong value for property ''%s'': Positive numbers expected.\n"), "LoadFail", "k");
assert_checkerror("LoadFail.k", msg);
assert_checkerror("LoadFail.k", msg);
assert_checkerror("LoadFailChild()", msg);
assert_checkerror("LoadFailChild()", msg);

// parent defined after a first failure
classdef LateChild < LateParent
end

assert_checkerror("LateChild()", msprintf(_("''%s'' does not exist\n"), "LateParent"));

classdef LateParent
    properties
        v = 3
    end
end

assert_checkequal(LateChild().v, 3);

// =============================================================
// Concatenation of objects without horzcat/vertcat: error, no crash
// =============================================================
classdef NoCat
    properties
        v = 0
    end
    methods
        function NoCat(x)
            this.v = x;
        end
    end
end

// temporaries (used to crash with a double free)
expect_err("[NoCat(1), NoCat(2)]");
expect_err("[NoCat(1); NoCat(2)]");
expect_err("[NoCat(1), 1]");
expect_err("[1, NoCat(1)]");
// variables
a = NoCat(1);
b = NoCat(2);
expect_err("[a, b]");
expect_err("[a; b]");
assert_checkequal(a.v, 1);
assert_checkequal(b.v, 2);

// =============================================================
// Comment lines at classdef body level
// =============================================================
classdef CommentedClass // trailing comment
    // before the first block
    properties
        // inside a block
        x = 1
    end
    // between two blocks

    methods
        function r = getx()
            r = this.x;
        end
    end
    // after the last block
end

assert_checkequal(CommentedClass().getx(), 1);

classdef OnlyComments
    // no block
end

assert_checktrue(isa(OnlyComments(), "OnlyComments"));

// enumeration with a private constructor
classdef PrivEnum
    properties
        v
    end

    enumeration
        One(1)
        Two(2)
    end

    methods (Private)
        function PrivEnum(x)
            this.v = x;
        end
    end
end

assert_checkequal(PrivEnum.Two.v, 2);
expect_err("PrivEnum(3)");

// enumeration of a parent class reached through a derived class first
classdef BaseEnum
    enumeration
        Red
        Green
    end
end

classdef DerivedEnum < BaseEnum
end

assert_checktrue(isa(DerivedEnum.Red, "BaseEnum"));
assert_checktrue(DerivedEnum.Red == BaseEnum.Red);

// =============================================================
// Redefinition of a parent class: derived classes and their objects follow
// =============================================================
classdef RedefBase
    properties
        a = 1
    end

    properties (Static)
        s = 10
    end

    methods
        function r = getA()
            r = this.a;
        end
    end
end

classdef RedefChild < RedefBase
    properties
        c = 5
    end
end

classdef RedefGrand < RedefChild
end

child = RedefChild();
grand = RedefGrand();
stored = child.getA;
assert_checkequal(child.getA(), 1);
assert_checkequal(RedefChild.s, 10);

classdef RedefBase
    properties
        a = 2
        b = 3
    end

    properties (Static)
        s = 20
    end

    methods
        function r = getA()
            r = this.a * 100;
        end

        function r = getB()
            r = this.b;
        end
    end
end

// existing objects: new methods, new properties, values kept
assert_checkequal(child.getA(), 100);
assert_checkequal(child.getB(), 3);
assert_checkequal(child.c, 5);
assert_checkequal(grand.getA(), 100);
assert_checkequal(RedefChild.s, 20);
// new objects
n = RedefGrand();
assert_checkequal([n.a, n.b, n.c], [2 3 5]);
// a method stored before the redefinition can still be called
assert_checkequal(stored(), 1);

// a parent that fails to load: same error from derived classes and their objects
classdef RedefBase
    properties
        a = 1
    end

    properties (Static)
        k (1,1) {mustBePositive} = -1
    end
end

msg = msprintf(_("%s: Wrong value for property ''%s'': Positive numbers expected.\n"), "RedefBase", "k");
assert_checkerror("RedefChild()", msg);
assert_checkerror("c = child.c", msg);

// fixed parent: everything is reloaded
classdef RedefBase
    properties
        a = 7
    end
end

assert_checkequal(RedefChild().a, 7);
assert_checkequal(child.c, 5);

// =============================================================
// Parent constructors
// =============================================================
classdef CtorA
    properties
        trace = ""
    end

    methods
        function CtorA()
            this.trace = this.trace + "A";
        end
    end
end

// not called by the constructor: called implicitly, before it
classdef CtorImplicit < CtorA
    methods
        function CtorImplicit()
            this.trace = this.trace + "B";
        end
    end
end

assert_checkequal(CtorImplicit().trace, "AB");

// called explicitly: where it is called, once
classdef CtorExplicit < CtorA
    methods
        function CtorExplicit()
            this.trace = this.trace + "b";
            CtorA();
            this.trace = this.trace + "B";
        end
    end
end

assert_checkequal(CtorExplicit().trace, "bAB");

// no constructor: parents are initialized
classdef CtorNone < CtorA
end

assert_checkequal(CtorNone().trace, "A");

// x = CtorA() builds another object
classdef CtorOther < CtorA
    properties
        other = []
    end

    methods
        function CtorOther()
            this.other = CtorA();
            this.trace = this.trace + "B";
        end
    end
end

o = CtorOther();
assert_checkequal(o.trace, "AB");
assert_checkequal(o.other.trace, "A");

// diamond: the common parent is initialized once
classdef CtorLeft < CtorA
end

classdef CtorRight < CtorA
end

classdef CtorDiamond < CtorLeft & CtorRight
end

assert_checkequal(CtorDiamond().trace, "A");

// explicit call only from the constructor of a direct subclass
msg = msprintf(_("Constructor of ''%s'' can only be called from the constructor of a direct subclass.\n"), "CtorA");

classdef CtorInMethod < CtorA
    methods
        function again()
            CtorA();
        end
    end
end

assert_checkerror("o = CtorInMethod(); o.again()", msg);

classdef CtorGrand < CtorImplicit
    methods
        function CtorGrand()
            CtorA();
        end
    end
end

assert_checkerror("CtorGrand()", msg);

// parent constructor with arguments: must be called explicitly
classdef CtorArgs
    properties
        v = 0
    end

    methods
        function CtorArgs(v)
            this.v = v;
        end
    end
end

classdef CtorArgsMissing < CtorArgs
end

classdef CtorArgsGiven < CtorArgs
    methods
        function CtorArgsGiven()
            CtorArgs(5);
        end
    end
end

assert_checkequal(CtorArgsGiven().v, 5);
assert_checkerror("CtorArgsMissing()", [msprintf(_("%s: implicit call of the constructor of ''%s'' without argument failed, call it in the constructor.\n"), "CtorArgsMissing", "CtorArgs"); msprintf(_("Undefined variable: %s\n"), "v")]);

// private parent constructor: the class can not be instantiated
classdef CtorPrivate
    methods (Private)
        function CtorPrivate()
        end
    end
end

classdef CtorPrivateChild < CtorPrivate
end

assert_checkerror("CtorPrivateChild()", msprintf(_("Constructor of ''%s'' is not accessible.\n"), "CtorPrivate"));

// =============================================================
// Default values of a class defined by execstr (definition deleted after execution)
// =============================================================
execstr("classdef ExecDefined; properties; v = [4 5 6] + 0; end; end");
assert_checkequal(ExecDefined().v, [4 5 6]);