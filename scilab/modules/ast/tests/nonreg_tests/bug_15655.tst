// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15655 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15655
//
// <-- Short Description -->
// `clear a; a(1:4,:,1) = (1:4)` => Submatrix incorrectly defined  (regression)

clear a; a(:) = 1:4;
assert_checkequal(a,1:4);
clear a; a(:,:) = 1:4;
assert_checkequal(a,1:4);
clear a; a(:,:,:) = 1:4;
assert_checkequal(a,1:4);
clear a; a(:) = (1:4)';
assert_checkequal(a,(1:4)');
clear a; a(1:4,:,1) = (1:4)';
assert_checkequal(a,(1:4)');
clear a; a(1:4,:,1) = 1:4;
assert_checkequal(a,(1:4)');
clear a; a(1:4,:,2) = (1:4)';
assert_checkequal(a,matrix([0 0 0 0 1 2 3 4],[4 1 2]));
clear a;a(:,1:2,:,:,1:2)=[1 2;3 4];
assert_checkequal(a,matrix([1 3 2 4],[1 2 1 1 2]));
clear a;a(1,:,:)=1:4;
assert_checkequal(a,matrix(1:4,[1 1 4]));
clear a;a(1,:,:)=(1:4)';
assert_checkequal(a,matrix(1:4,[1 4]));
clear a;a(:,:,:,1:2)=matrix(1:16,[2 4 2]) ;
assert_checkequal(a,matrix(1:16,[2 4 1 2]));
clear a;a(:,:,1:4,1:2)=matrix(1:16,[2 4 2]); 
assert_checkequal(a,matrix(1:16,[2 1 4 2]));
clear a;a(:,:,1:4,:,1:2)=matrix(1:16,[2 4 2]);
assert_checkequal(a,matrix(1:16,[2 1 4 1 2]));
