// ============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2023 - Dassault Systèmes S.E. - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// -------------------------------------------------------------
// Basic class, ctor args, simple method
// -------------------------------------------------------------
classdef T1_Point
    properties
        x = 0
        y = 0
    end

    methods
        function T1_Point(xx, yy)
            if argn(2) >= 2 then
                this.x = xx;
                this.y = yy;
            end
        end

        function r = norm()
            r = sqrt(this.x ^ 2 + this.y ^ 2);
        end
    end
end

assert_checkequal(T1_Point(3,4).norm(), 5);

// -------------------------------------------------------------
// Inheritance & override, multiple returns
// -------------------------------------------------------------
classdef T2_Shape
    methods
        function a = area(this)
            a = 0;
        end
    end
end

classdef T2_Rectangle < T2_Shape
    properties
        w = 3
        h = 4
    end

    methods
        function T2_Rectangle(ww, hh)
            if argn(2) >= 2 then
                this.w = ww;
                this.h = hh;
            end
        end

        function a = area()
            a = this.w * this.h;
        end

        function [mn, mx] = sides()
            if this.w < this.h then
                mn = this.w;
                mx = this.h;
            else
                mn = this.h;
                mx = this.w;
            end
        end
    end
end

r2 = T2_Rectangle(5, 6);
assert_checkequal(r2.area(), 30);
[mn, mx] = r2.sides();
assert_checkequal(mn, 5);
assert_checkequal(mx, 6);

// -------------------------------------------------------------
// Multiple inheritance, deterministic MRO for conflicts
// -------------------------------------------------------------
classdef T3_A
    methods
        function s = test()
            s = "A";
        end
    end
end

classdef T3_B
    methods
        function s = test()
            s = "B";
        end
    end
end

classdef T3_C < T3_A & T3_B
end

classdef T3_D < T3_B & T3_A
end

assert_checkequal(T3_C().test(), "A");
assert_checkequal(T3_D().test(), "B");

// -------------------------------------------------------------
// Shadowing: derived property hides base property in base method
// -------------------------------------------------------------
classdef T4_Base
    properties
        p = 10
    end

    methods
        function v = getP()
            v = this.p
        end
    end
end

classdef T4_Derived < T4_Base
    properties
        p = 1
    end
end

assert_checkequal(T4_Derived().getP(), 1);
assert_checkequal(T4_Derived().p, 1);

// -------------------------------------------------------------
// Diamond shape inheritance (A <- B, A <- C, D < B & C)
// Method defined only in A should be found consistently.
// -------------------------------------------------------------
classdef T5_A
    methods
        function s = test(this)
            s = "A";
        end
    end
end

classdef T5_B < T5_A
end

classdef T5_C < T5_A
end

classdef T5_D < T5_B & T5_C
end

assert_checkequal(T5_D().test(), "A");

// -------------------------------------------------------------
// Methods with multiple outputs and regular parameters
// -------------------------------------------------------------
classdef T6_Calc
    methods
        function [q, r] = divmod(a, b)
            q = floor(a / b);
            r = a - q * b;
        end
    end
end

[q, r] = T6_Calc().divmod(17, 5);
assert_checkequal(q, 3);
assert_checkequal(r, 2);

// -------------------------------------------------------------
// Ctor with args + method consuming external args
// -------------------------------------------------------------
classdef T7_Line
    properties
        a = 1
        b = 0
    end

    methods
        function T7_Line(aa, bb)
            if argn(2) >= 2 then
                this.a = aa;
                this.b = bb;
            end
        end

        function y = eval(x)
            y = this.a * x + this.b;
        end
    end
end

ln = T7_Line(2, 10);
assert_checkequal(ln.eval(7), 24);

// -------------------------------------------------------------
// Default methods injected (disp/outline) exist and return strings
// -------------------------------------------------------------
classdef T8_Plain
    properties
        x = 123
    end
end

// outline should exist by default (unless you already override it)
s = T8_Plain().outline();
assert_checkequal(type(s), 10);

//overload operators
classdef Matrix
    properties
        value = []
    end
    methods
        function Matrix(v)
            this.value = v;
        end

        function r = plus(a ,b) //generic overload for operation +
            if isa(a, "Matrix") & isa(b, "Matrix") then
                r = Matrix(a.value + b.value);
            else
                error(sprintf("Operation + not defined for %s and %s.\n", typeof(a), typeof(b)));
            end
        end

        function r = plus_s(a ,b) //overload for Matrix + dobule or double + Matrix
            if isa(a, "Matrix") then
                r = Matrix(a.value + b);
            else
                r = Matrix(a + b.value);
            end
        end

        function r = plus_i(a ,b) //overload for Matrix + int or int + Matrix
            if isa(a, "Matrix") then
                r = Matrix(a.value + double(b));
            else
                r = Matrix(double(a) + b.value);
            end
        end

        function disp()
            disp(this.value);
        end
    end
end

a = Matrix([1 2 3 4]);
b = Matrix([4 3 2 1]);
res = a + b;
assert_checkequal(res.value, [5 5 5 5]);
res = a + 10;
assert_checkequal(res.value, [11 12 13 14]);
res = 10 + b;
assert_checkequal(res.value, [14 13 12 11]);
res = a + int8(10);
assert_checkequal(res.value, [11 12 13 14]);
res = int8(10) + b;
assert_checkequal(res.value, [14 13 12 11]);

// -------------------------------------------------------------
// clone
// -------------------------------------------------------------
classdef T_Clone
    properties
        v = 1
    end

    properties (Static)
        shown = 0
    end

    methods
        function disp()
            T_Clone.shown = this.v;
        end
    end
end

o = T_Clone();
o.v = 2;
c = o.clone();
assert_checkequal(c.v, 2);
c.v = 3;
assert_checkequal(o.v, 2);
// display of the copy uses the disp method
disp(c);
assert_checkequal(T_Clone.shown, 3);

// -------------------------------------------------------------
// function used as method: `this` only during the call
// -------------------------------------------------------------
function r = T_hasThis(varargin)
    r = isdef("this", "l");
end

classdef T_FuncMethod
    methods
        has = T_hasThis
        not = T_hasThis
    end
end

o = T_FuncMethod();
assert_checktrue(o.has());
assert_checkfalse(T_hasThis());
assert_checktrue(~o);
assert_checkfalse(T_hasThis());
