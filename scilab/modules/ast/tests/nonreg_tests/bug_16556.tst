// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16556 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16556
//
// <-- Short Description -->
// Modifying nested lists/arrays in function changes variable in calling scope

// Define variables
testArray = list();
for i=1:1:10
    testArray($+1) = list();
    for j=1:1:10
        testArray(i)($+1) = [];
        testArray(i)(j).testMember = 1;
    end
end
testArrayOrig = testArray;

// Define functions
function outArray = testFunc(inArray)
    for i=1:1:10
        for j=1:1:10
            inArray(i)(j).testMember = 5
        end
    end
    outArray = inArray;
endfunction

// Send the variable as argument to the function
testArrayOut = testFunc(testArray);

assert_checkequal(testArrayOut(1)(1).testMember, 5);
assert_checkequal(testArrayOrig(1)(1).testMember, 1);
assert_checkequal(testArray(1)(1).testMember, 1);
