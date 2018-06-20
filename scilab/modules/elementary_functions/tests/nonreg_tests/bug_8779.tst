// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- Non-regression test for bug 8779 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8779
//
// <-- Short Description -->
//  gsort(x,'lr') does not preserve order of equal items in index output
// =============================================================================

x=[ 1     0
2     2
0     1
1     0
1     2
1     2
2     1
2     1
0     1
1     0
2     2
0     1];
[y,k]=gsort(x,"lr");
assert_checkequal(k,[2,11,7,8,5,6,1,4,10,3,9,12]');
[y,k]=gsort(int8(x),"lr");
assert_checkequal(k,[2,11,7,8,5,6,1,4,10,3,9,12]');
[y,k]=gsort(int16(x),"lr");
assert_checkequal(k,[2,11,7,8,5,6,1,4,10,3,9,12]');
[y,k]=gsort(int32(x),"lr");
assert_checkequal(k,[2,11,7,8,5,6,1,4,10,3,9,12]');
[y,k]=gsort(int64(x),"lr");
assert_checkequal(k,[2,11,7,8,5,6,1,4,10,3,9,12]');

[y,k]=gsort(uint8(x),"lr");
assert_checkequal(k,[2,11,7,8,5,6,1,4,10,3,9,12]');
[y,k]=gsort(uint16(x),"lr");
assert_checkequal(k,[2,11,7,8,5,6,1,4,10,3,9,12]');
[y,k]=gsort(uint32(x),"lr");
assert_checkequal(k,[2,11,7,8,5,6,1,4,10,3,9,12]');
[y,k]=gsort(uint64(x),"lr");
assert_checkequal(k,[2,11,7,8,5,6,1,4,10,3,9,12]');

[y,k]=gsort(string(x),"lr");
assert_checkequal(k,[2,11,7,8,5,6,1,4,10,3,9,12]');
