// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - 2020 - Samuel Gougeon
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// unit test of unique() for real, complex and boolean SPARSE input
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// ------------------------------------------------------------------
// We assume that the result of unique(o,..) with a full o is correct.
// They are tested elsewhere.
// Here, we compare the result of full(unique(sparse(o),..)) with
// the one got with unique(o,..).
// ------------------------------------------------------------------
esb = sparse(%t); esb(1) = [];
m = int(sprand(3,20,0.5)*4);
n = size(m,"*");
k = grand(1,n/5,"uin",1,n);
//m(k) = %nan;    // Requires fixing http://bugzilla.scilab.org/16472
mc = m + imult(m(:,$:-1:1));
b = m > 1;
objects = list(..
   sparse([]), sparse(0), sparse(-7), m(:)', m(:), m, .. // reals
   sparse(1-%i), mc(:).', mc(:), mc, ..                  // complex
   esb, sparse(%f), sparse(%t), b(:)', b(:), b);         // booleans

options = list(list(), list("keepOrder"), list("uniqueNan"), list("keepOrder","uniqueNan"));
for orien = list(list(), "r", "c")
    for opt = options              // 3 x 4 x 16 x 9 = 1728 tests
        for o = objects
            u = unique(o, orien(:), opt(:));
            fu = unique(full(o), orien(:), opt(:));
            assert_checktrue(issparse(u));
            assert_checkequal(full(u), fu);
        
            [u, ki, ku, nb] = unique(o, orien(:), opt(:));
            [fu, fki, fku, fnb] = unique(full(o), orien(:), opt(:));

            assert_checktrue(issparse(u));
            assert_checkfalse(issparse(ki));
            assert_checkfalse(issparse(ku));
            assert_checkfalse(issparse(nb));
            assert_checkequal(full(u), fu);
            select orien
            case list()
                assert_checkequal(u, o(ki));
                assert_checkequal(o, matrix(u(ku),size(o)));
            case "r"
                assert_checkequal(u, o(ki,:));
                assert_checkequal(o, matrix(u(ku,:),size(o)));
            case "c"
                assert_checkequal(u, o(:,ki));
                assert_checkequal(o, matrix(u(:,ku),size(o)));
            end
        end
    end
end
