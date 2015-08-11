// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NOT FIXED -->

//mtlb_mode

mtlb_mode(%t);
if ~mtlb_mode() then pause,end
if []+1<>[] then pause,end
mtlb_mode(%f);
if mtlb_mode() then pause,end

// mtlb_mode with structs
mtlb_mode(%t)
clear S;S(1,2).a=3;S(3).a=44 ;
assert_checkequal(S.a, list([],3,44));
assert_checkequal(size(S), [1, 3]);

clear S;S.a=3;S(3).a=44;
assert_checkequal(S.a, list(3,[],44));
assert_checkequal(size(S), [1, 3]);

clear S;S(4).a=3;
assert_checkequal(S.a, list([],[],[],3));
assert_checkequal(size(S), [1, 4]);
mtlb_mode(%f)
