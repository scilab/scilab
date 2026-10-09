// ============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

function expect_err(exprstr)
    err = execstr(exprstr, 'errcatch');
    assert_checkfalse(err == 0);
endfunction

// =============================================================
// Basic static property
// =============================================================
classdef Counter
    properties (Static)
        count = 0
    end

    properties
        name = ""
    end

    methods
        function this = Counter(n)
            this.name = n;
            Counter.count = Counter.count + 1;
        end
    end
end

// --- read via class name without instantiation ---
assert_checkequal(Counter.count, 0);

// --- read/write via class name ---
Counter.count = 10;
assert_checkequal(Counter.count, 10);

// --- reset for instance tests ---
Counter.count = 0;

// --- instantiation increments counter ---
c1 = Counter("a");
assert_checkequal(Counter.count, 1);
assert_checkequal(c1.count, 1);

c2 = Counter("b");
assert_checkequal(Counter.count, 2);
assert_checkequal(c1.count, 2);
assert_checkequal(c2.count, 2);

// --- write via instance updates shared value ---
c1.count = 99;
assert_checkequal(c2.count, 99);
assert_checkequal(Counter.count, 99);

// --- instance property is independent ---
assert_checkequal(c1.name, "a");
assert_checkequal(c2.name, "b");

// =============================================================
// Static with visibility
// =============================================================
classdef StaticVis
    properties (Static)
        pub_s = 1
    end

    properties (Static, Private)
        priv_s = 2
    end

    properties (Static, Protected)
        prot_s = 3
    end

    methods
        function r = getPrivS()
            r = this.priv_s;
        end

        function r = getProtS()
            r = this.prot_s;
        end

        function setPrivS(v)
            StaticVis.priv_s = v;
        end
    end
end

// --- public static: accessible from outside ---
assert_checkequal(StaticVis.pub_s, 1);
s = StaticVis();
assert_checkequal(s.pub_s, 1);

// --- private/protected static: not accessible from outside ---
expect_err("StaticVis.priv_s");
expect_err("StaticVis.prot_s");
expect_err("s.priv_s");
expect_err("s.prot_s");

// --- accessible from methods ---
assert_checkequal(s.getPrivS(), 2);
assert_checkequal(s.getProtS(), 3);

// --- writable from methods ---
s.setPrivS(42);
assert_checkequal(s.getPrivS(), 42);

// =============================================================
// Inheritance of static properties
// =============================================================
classdef Base
    properties (Static)
        shared = 100
    end
end

classdef Derived < Base
end

// --- derived class sees parent static ---
assert_checkequal(Base.shared, 100);
assert_checkequal(Derived.shared, 100);

// --- modification via parent is visible in derived ---
Base.shared = 200;
assert_checkequal(Derived.shared, 200);

// --- modification via derived updates parent ---
Derived.shared = 300;
assert_checkequal(Base.shared, 300);

// --- instances share the same static ---
b = Base();
d = Derived();
assert_checkequal(b.shared, 300);
assert_checkequal(d.shared, 300);

b.shared = 400;
assert_checkequal(d.shared, 400);
assert_checkequal(Base.shared, 400);

// =============================================================
// Static methods
// =============================================================
classdef MathHelper
    properties (Static)
        pi_approx = 3.14
    end

    methods (Static)
        function r = add(a, b)
            r = a + b;
        end

        function r = getPi()
            r = MathHelper.pi_approx;
        end
    end
end

// --- call via class name ---
assert_checkequal(MathHelper.add(2, 3), 5);
assert_checkequal(MathHelper.getPi(), 3.14);

// --- call via instance ---
m = MathHelper();
assert_checkequal(m.add(10, 20), 30);
assert_checkequal(m.getPi(), 3.14);

// =============================================================
// Static methods with visibility
// =============================================================
classdef StaticMethVis
    methods (Static)
        function r = pubMethod()
            r = "public";
        end
    end

    methods (Static, Private)
        function r = privMethod()
            r = "private";
        end
    end

    methods
        function r = callPriv()
            r = StaticMethVis.privMethod();
        end
    end
end

// --- public static method accessible from outside ---
assert_checkequal(StaticMethVis.pubMethod(), "public");
sv = StaticMethVis();
assert_checkequal(sv.pubMethod(), "public");

// --- private static method not accessible from outside ---
expect_err("StaticMethVis.privMethod()");
expect_err("sv.privMethod()");

// --- private static method accessible from instance method ---
assert_checkequal(sv.callPriv(), "private");

// =============================================================
// Inheritance of static methods
// =============================================================
classdef BaseM
    methods (Static)
        function r = hello()
            r = "hello from BaseM";
        end
    end
end

classdef DerivedM < BaseM
end

// --- derived class can call parent static method ---
assert_checkequal(BaseM.hello(), "hello from BaseM");
assert_checkequal(DerivedM.hello(), "hello from BaseM");

dm = DerivedM();
assert_checkequal(dm.hello(), "hello from BaseM");

// =============================================================
// Static method calls static method
// =============================================================
classdef Chain
    methods (Static)
        function r = double(x)
            r = x * 2;
        end

        function r = quadruple(x)
            r = Chain.double(Chain.double(x));
        end
    end
end

assert_checkequal(Chain.quadruple(3), 12);
ch = Chain();
assert_checkequal(ch.quadruple(5), 20);

// =============================================================
// Instance method calls static method
// =============================================================
classdef Mixer
    properties
        factor = 1
    end

    methods (Static)
        function r = base_value()
            r = 100;
        end
    end

    methods
        function r = compute(this)
            r = Mixer.base_value() * this.factor;
        end

        function r = compute_via_this(this)
            r = this.base_value() * this.factor;
        end
    end
end

mx = Mixer();
mx.factor = 3;
assert_checkequal(mx.compute(), 300);
assert_checkequal(mx.compute_via_this(), 300);

// =============================================================
// Override static method in derived class
// =============================================================
classdef BaseGreet
    methods (Static)
        function r = greet()
            r = "hello from Base";
        end
    end
end

classdef DerivedGreet < BaseGreet
    methods (Static)
        function r = greet()
            r = "hello from Derived";
        end
    end
end

assert_checkequal(BaseGreet.greet(), "hello from Base");
assert_checkequal(DerivedGreet.greet(), "hello from Derived");

bg = BaseGreet();
dg = DerivedGreet();
assert_checkequal(bg.greet(), "hello from Base");
assert_checkequal(dg.greet(), "hello from Derived");

// =============================================================
// Static property with complex default values
// =============================================================
classdef ComplexDefaults
    properties (Static)
        vec = [1, 2, 3]
        mat = [1 0; 0 1]
        str = "hello"
        flag = %t
    end
end

assert_checkequal(ComplexDefaults.vec, [1, 2, 3]);
assert_checkequal(ComplexDefaults.mat, [1 0; 0 1]);
assert_checkequal(ComplexDefaults.str, "hello");
assert_checkequal(ComplexDefaults.flag, %t);

// --- modify and check sharing ---
ComplexDefaults.vec = [4, 5, 6];
cd1 = ComplexDefaults();
cd2 = ComplexDefaults();
assert_checkequal(cd1.vec, [4, 5, 6]);
assert_checkequal(cd2.vec, [4, 5, 6]);

// =============================================================
// clear variable does not destroy static values
// =============================================================
classdef Persistent
    properties (Static)
        data = 42
    end
end

p = Persistent();
Persistent.data = 99;
clear p;
assert_checkequal(Persistent.data, 99);
p2 = Persistent();
assert_checkequal(p2.data, 99);

// =============================================================
// a constructor can not be static
// =============================================================
msg = msprintf(_("%s: Constructor of ''%s'' can not be static.\n"), "''classdef''", "StaticCtor");
assert_checkerror("execstr(""classdef StaticCtor; methods (Static); function StaticCtor(); end; end; end"")", msg);

function StaticCtorFunc()
end
assert_checkerror("execstr(""classdef StaticCtor; methods (Static); StaticCtor = StaticCtorFunc; end; end"")", msg);

