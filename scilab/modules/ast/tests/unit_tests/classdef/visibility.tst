// ============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

function check_exec(exprstr, var)
    err = execstr(exprstr, 'errcatch');
    assert_checktrue(err == 0);

    if isdef("var", "l") then
        execstr(sprintf("%s = resume(%s)", var, var));
    end
endfunction

function expect_err(exprstr)
    err = execstr(exprstr, 'errcatch');
    assert_checkfalse(err == 0);
endfunction

// -------------------------------------------------------------
// Property visibility: public / protected / private
// -------------------------------------------------------------
classdef VisA
    properties
        pub = 1
    end

    properties (Protected)
        prot = 2
    end

    properties (Private)
        priv = 3
    end

    methods
        function v = readAll()
            v = [this.pub, this.prot, this.priv];
        end

        function v = readProt()
            v = this.prot;
        end

        function v = readPriv()
            v = this.priv;
        end

        function setProt(val)
            this.prot = val;
        end
    end
end

classdef VisB < VisA
    methods
        function v = readFromChild()
            v = [this.pub, this.prot];
        end

        function v = tryReadPriv() // should error
            v = this.priv;
        end

        function setProtFromChild(val)
            this.prot = val;
        end
    end
end

// Inside class: all readable
check_exec("[v]=VisA().readAll();", "v");
assert_checkequal(v, [1 2 3]);

// From child: pub + prot ok
check_exec("[v]=VisB().readFromChild();", "v");
assert_checkequal(v, [1 2]);

// From child: private should fail
expect_err("VisB().tryReadPriv();");

// Outside: read access
check_exec("VisA().pub;");
expect_err("VisA().prot;");
expect_err("VisA().priv;");

// Outside: write access
check_exec("x=VisA(); x.pub=5;");
expect_err("x=VisA(); x.prot=5;");
expect_err("x=VisA(); x.priv=5;");

// Child writing protected should be allowed
check_exec("b=VisB(); b.setProtFromChild(10); pv=b.readProt();", "pv");
assert_checkequal(pv, 10);

// -------------------------------------------------------------
// Method visibility: public / protected / private
// -------------------------------------------------------------
classdef MethA
    methods
        function s = pub(), s = "pub"; end
    end

    methods (Protected)
        function s = pro(), s = "pro"; end
    end

    methods (Private)
        function s = pri(), s = "pri"; end
    end

    methods
        function s = callAll() // calling private from inside base => ok
        s = this.pri();
        end
    end
end

classdef MethB < MethA
    methods
        function s = callProt()
            s = this.pro();
        end

        function s = callPriv()
            s = this.pri();
        end
    end
end

// Outside calls
check_exec("[s]=MethA().pub();", "s");
assert_checkequal(s, "pub");
expect_err("MethA().pro();");
expect_err("MethA().pri();");

// Inside base: private allowed
check_exec("[s]=MethA().callAll();", "s");
assert_checkequal(s, "pri");

// From child: protected ok, private forbidden
check_exec("[s]=MethB().callProt();", "s");
assert_checkequal(s, "pro");
expect_err("MethB().callPriv();");

// -------------------------------------------------------------
// Base method accessing base private on derived instance
// -------------------------------------------------------------
classdef PrivBase
    properties (Private)
        s = 99
    end

    methods
        function y = getS()
            y = this.s;
        end
    end
end

classdef PrivChild < PrivBase
end

check_exec("[y]=PrivChild().getS();", "y");
assert_checkequal(y, 99);

// -------------------------------------------------------------
// Base methods and base constructor writing base private on derived instance
// -------------------------------------------------------------
classdef PrivWBase
    properties (Private)
        s = 1
    end

    methods
        function PrivWBase(v)
            this.s = this.s + v;
        end

        function setS(v)
            this.s = v;
        end

        function y = getS()
            y = this.s;
        end
    end
end

classdef PrivWChild < PrivWBase
    methods
        function PrivWChild(v)
            PrivWBase(v);
        end

        function trySetS(v) // should error
            this.s = v;
        end
    end
end

// parent constructor reading and writing its private on a derived instance
check_exec("c=PrivWChild(5); [y]=c.getS();", "y");
assert_checkequal(y, 6);

// inherited setter on a derived instance
check_exec("c=PrivWChild(5); c.setS(7); [y]=c.getS();", "y");
assert_checkequal(y, 7);

// child method or outside: base private still forbidden
expect_err("c=PrivWChild(5); c.trySetS(1);");
expect_err("c=PrivWChild(5); c.s=1;");

// -------------------------------------------------------------
// Constructor visibility (public/protected/private)
// -------------------------------------------------------------

// Public
classdef CPublic
    methods
        function CPublic()
        end
    end
end

check_exec("CPublic();");

// Protected
classdef CProt
    methods (Protected)
        function CProt()
        end
    end
end

classdef CProtChild < CProt
    methods
        function CProtChild()
            CProt();
        end
    end
end

check_exec("CProtChild();");
expect_err("CProt();");

// Private
classdef CPriv
    methods (Private)
        function CPriv()
        end
    end
end

classdef CPrivChild < CPriv
    methods
        function CPrivChild()
            CPriv();
        end
    end
end

expect_err("CPrivChild();");
expect_err("CPriv();");


classdef CVar
    properties
        var1 = 1;
    end

    methods
        function CVar()
            this.var1 = 10
        end
    end
end

classdef CVar2 < CVar
    methods
        function CVar2()
            CVar();
        end
    end
end

check_exec("[y]=CVar2().var1;", "y");
assert_checkequal(y, 10);

// Diamant : A <- B, A <- C, D < B, C
classdef A
    methods
        function v = test()
            v="A";
        end
    end
end

classdef B < A
end

classdef C < A
end

classdef D < B & C
end

assert_checkequal(D().test(), "A");