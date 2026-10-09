// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for issue 17601 -->
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17601
//
// <-- Short Description -->
// classdef: static methods could not access protected/private static members
// of their own class, while any method of any class could.

classdef A17601
    properties (Static, Protected)
        a = 2
    end

    properties (Static, Private)
        p = 3
    end

    methods (Static)
        function r = aff()
            r = sprintf("a=%d", A17601.a);
        end

        function r = getP()
            r = A17601.priv();
        end

        function setP(v)
            A17601.p = v;
        end
    end

    methods (Static, Private)
        function r = priv()
            r = A17601.p;
        end
    end
end

classdef B17601 < A17601
    methods (Static)
        function r = affB()
            r = sprintf("a=%d", A17601.a);
        end
    end
end

classdef C17601
    methods (Static)
        function r = readA()
            r = A17601.a;
        end
    end

    methods
        function r = readAFromMethod()
            r = A17601.a;
        end
    end
end

// static method accesses protected/private static members of its class
assert_checkequal(A17601.aff(), "a=2");
assert_checkequal(A17601.getP(), 3);
A17601.setP(5);
assert_checkequal(A17601.getP(), 5);

// static method called through an instance
obj = A17601();
assert_checkequal(obj.aff(), "a=2");
assert_checkequal(obj.getP(), 5);

// inherited static method and static method of a derived class
assert_checkequal(B17601.aff(), "a=2");
assert_checkequal(B17601.affB(), "a=2");
objB = B17601();
assert_checkequal(objB.aff(), "a=2");
assert_checkequal(objB.affB(), "a=2");

// no access from outside or from another class
msg = msprintf(_("Wrong extraction: ''%s'' is not accessible.\n"), "a");
assert_checkerror("A17601.a", msg);
assert_checkerror("C17601.readA()", msg);
assert_checkerror("C17601().readAFromMethod()", msg);
assert_checkerror("A17601.priv()", msprintf(_("Wrong extraction: ''%s'' is not accessible.\n"), "priv"));
assert_checkerror("A17601.p = 1", msprintf(_("Wrong insertion: property ''%s'' is not accessible.\n"), "p"));
assert_checkequal(A17601.getP(), 5);

// static methods do not receive this
classdef D17601
    properties
        v = 1
    end

    methods (Static)
        function r = useThis()
            r = this.v;
        end
    end
end

assert_checkerror("D17601.useThis()", msprintf(_("Undefined variable: %s\n"), "this"));

// a function used as static method gets the class context only during the call
function r = readAs17601()
    r = E17601.s;
end

classdef E17601
    properties (Static, Protected)
        s = 7
    end

    methods (Static)
        get = readAs17601
    end
end

assert_checkequal(E17601.get(), 7);
assert_checkerror("readAs17601()", msprintf(_("Wrong extraction: ''%s'' is not accessible.\n"), "s"));
