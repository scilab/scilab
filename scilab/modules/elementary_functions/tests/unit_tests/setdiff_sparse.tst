// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON - Le Mans Université
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

//
// <-- Short Description -->
// Unit tests for setdiff() with boolean sparse or numerical sparse matrices

// See also : http://bugzilla.scilab.org/15867

eb = sparse(%t); eb(1) = [];

// Element-wise processing
// -----------------------
objects = list(sparse([]), sparse(0), sparse(5), sparse([2 0 -3 0 4]), ..
    sparse([2 0 -3 0 4]'), sparse([0 2 -1 ; 3 0 2]), ..
    eb, sparse(%t), sparse(%f), sparse([%t %f %f %t %f]), ..
    sparse([%t %f %f %t %f]'), sparse([%t %f %t ; %t %t %f]));
for a = objects
    typeref = 1
    for b = objects
        [dref, kref] = setdiff(full(a), full(b))
        d = setdiff(a, b);
        assert_checktrue(issparse(d));
        assert_checkequal(full(d), dref);
        [d, k] = setdiff(a, b);
        assert_checkequal(k, kref);
    end
end
// orientation "c"
// ---------------
m = [1  0  0  1  1  0  0  1  0  1
     1  0  0  1  1  1  0  1  1  0
     0  0  1  0  1  0  0  0  0  1
    ];
sm = sparse(m);
m2 = [0  1  1  1  0
      1  1  1  1  0
      1  1  0  0  0
     ];
sm2 = sparse(m2);

objects = list([], sparse([]), sparse(5), sparse([2 0 -3 0 4]), ..
    sparse([2 0 -3 0 4]'), sparse([0 2 -1 ; 3 0 2]), m, sm, m2, sm2, ..
    eb, sparse(%t), sparse(%f), sparse([%t %t %t]), ..
    sparse([%f %f %f]'), sparse([%t %f %t ; %t %t %f]), ..
    m==1, sm==1, m2==1, sm2==1);
for a = objects
    for b = objects
        if (size(a,1)<>0 & size(b,1)<>0 & size(a,1)<>size(b,1)) | ..
            (~issparse(a) & ~issparse(b))
            continue
        end
        [dref, kref] = setdiff(full(a), full(b), "c");

        d = setdiff(a, b, "c");
        try
        assert_checkequal(issparse(d), issparse(a));
        if issparse(a)
            dref = sparse(dref);
            if type(a)==6 & dref==sparse([])
                dref = eb
            end
        end
        assert_checkequal(d, dref);

        [d, k] = setdiff(a, b, "c");
        assert_checkequal(issparse(d), issparse(a));
        assert_checkequal(d, dref);
        assert_checkequal(k, kref);
    catch
        pause
    end

    end
end

