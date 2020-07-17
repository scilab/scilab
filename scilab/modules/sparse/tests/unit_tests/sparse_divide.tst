
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

A = list(rand(4,5), rand(4,5) + rand(4,5)*%i, ..
         sprand(4,5,0.5), sprand(4,5,0.5) + sprand(4,5,0.5)*%i);
B = list(rand(5,6), rand(5,6)+rand(5,6)*%i, ..
         sprand(5,6,0.5), sprand(5,6,0.5) + sprand(5,6,0.5)*%i);

// ----------------
// RIGHT DIVISION /
// ----------------
// p = a * b   => a = p / b
// b rectangular
// -------------
for a = A
    isspa = issparse(a)
    for b = B
        isspb = issparse(b)
        // complex sparse / complex sparse is not supported
        if ~isspa & ~isspb | (isspa & isspb & (~isreal(a) | ~isreal(b)))
            continue
        end
        p = a * b;
        a2 = p / b;
        if ~issparse(p) | ~isspb
            assert_checkfalse(issparse(a2));
        else
            assert_checktrue(issparse(a2));
        end
        assert_checkequal(size(a2), size(a));
        assert_checkalmostequal(clean(a2*b,%eps,%eps), p);
    end
end
// Square b
// --------
SB = list(rand(5,5), rand(5,5)+rand(5,5)*%i, ..
         sprand(5,5,0.5), sprand(5,5,0.5) + sprand(5,5,0.5)*%i);
for a = A
    isspa = issparse(a)
    for b = SB
        isspb = issparse(b)
        if ~isspa & ~isspb
            continue
        end
        p = a * b;
        a2 = p / b;
        if ~issparse(p) | ~isspb
            assert_checkfalse(issparse(a2));
        else
            assert_checktrue(issparse(a2));
        end
        assert_checkequal(size(a2), size(a));
        assert_checkalmostequal(clean(a2*b,%eps,%eps), p);
    end
end

// ---------------
// LEFT DIVISION \
// ---------------
// p = a * b   => b = a \ p
// a rectangular
// -------------
for a = A
    isspa = issparse(a)
    for b = B
        isspb = issparse(b)
        // complex sparse \ complex sparse is not supported
        if ~isspa & ~isspb | (isspa & isspb & (~isreal(a) | ~isreal(b)))
            continue
        end
        p = a * b;
        b2 = a \ p;
        if ~isspa | ~issparse(p)
            assert_checkfalse(issparse(b2));
        else
            assert_checktrue(issparse(b2));
        end
        assert_checkequal(size(b2), size(b));
        assert_checkalmostequal(clean(a*b2,%eps,%eps), p);
    end
end
// Square a
// --------
// A = SB
for a = SB
    isspa = issparse(a)
    for b = B
        isspb = issparse(b)
        if ~isspa & ~isspb
            continue
        end
        p = a * b;
        b2 = a \ p;
        if ~isspa | ~issparse(p)
            assert_checkfalse(issparse(b2));
        else
            assert_checktrue(issparse(b2));
        end
        assert_checkequal(size(b2), size(b));
        assert_checkalmostequal(clean(a*b2,%eps,%eps), p);
    end
end

