// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- Non-regression test for bug 13306 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13306
//
// <-- Short Description -->
// Evaluating a function set as a structure field could fail.


function res = SumAList(myList)
   res = myList(1) + myList(2);
endfunction
Namespace.SumAList = SumAList;
assert_checkequal(Namespace.SumAList(list(1,1)), 2);

function res = SumAList2(myList, optional)
    //now it enters the function because nParam = length(List)
    res = myList(1) + myList(2);
endfunction
Namespace.SumAList2 = SumAList2;
assert_checkequal(Namespace.SumAList2(list(1,1)), 2);
