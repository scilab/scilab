// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for issue 7814 -->
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Gitlab URL -->
// https://gitlab.com/scilab/scilab/-/issues/7814
//
// <-- Short Description -->
// remove reference to mat_2_graph in classmarkov function

M1 = [0 1 0; 0 0 1; 1 0 0];
[perm,rec,tr,indsRec,indsT] = classmarkov(M1);
assert_checkequal(perm, [1 2 3]);
assert_checkequal(rec, 3);
assert_checkequal(tr, 0);
assert_checkequal(indsT, []);

M2 = [0.5 0.5 0; 0 1 0; 0.3 0.3 0.4];
[perm,rec,tr,indsRec,indsT] = classmarkov(M2);
assert_checkequal(perm, [2 1 3]);
assert_checkequal(rec, 1);
assert_checkequal(tr, 2);
assert_checkequal(indsRec, 2);

M3 = eye(4,4);
[perm,rec,tr,indsRec,indsT] = classmarkov(M3);
assert_checkequal(perm, [1 2 3 4]);
assert_checkequal(rec, ones(1,4));
assert_checkequal(tr, 0);

M4 = [0.5 0.5 0 0 0;
      0.5 0.5 0 0 0;
      0   0   0.5 0.5 0;
      0   0   0.5 0.5 0;
      0.2 0.2 0.2 0.2 0.2];
[perm,rec,tr,indsRec,indsT] = classmarkov(M4);
assert_checkequal(rec, [2 2]);
assert_checkequal(tr, 1);