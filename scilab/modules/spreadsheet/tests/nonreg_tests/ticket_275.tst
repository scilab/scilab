// =============================================================================
// Copyright (C) 2010 - 2012 - INRIA - Michael BAUDIN
// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
// <-- Non-regression test for bug 275 -->
//
// <-- URL -->
//  http://forge.scilab.org/index.php/p/csv-readwrite/issues/275/
//
// <-- Short Description -->
// The default precision is insufficient.
// =============================================================================
function y = writereadcycle(x,filename)
    csvWrite(x,filename);
    separator = [];
    decimal=[];
    conversion ="double";
    y = csvRead(filename, separator, decimal, conversion);
endfunction

function tf = checkWriteReadCycle(x,direction,n,filename)
    tf = %t
    for k = 1 : n
        y = writereadcycle(x,filename);
        tf = (x==y);
        if ( ~tf ) then
            break
        end
        x = nearfloat(direction,x);
    end
endfunction

n=1000;
filename = fullfile(TMPDIR,"csvWrite_M_1.csv");
tf = checkWriteReadCycle(1,"succ",100,filename);
assert_checkequal ( tf , %t );
tf = checkWriteReadCycle(1,"pred",100,filename);
assert_checkequal ( tf , %t );
tf = checkWriteReadCycle(0.1,"succ",100,filename);
assert_checkequal ( tf , %t );
tf = checkWriteReadCycle(0.1,"pred",100,filename);
assert_checkequal ( tf , %t );
x = number_properties("huge");
tf = checkWriteReadCycle(x,"succ",100,filename);
assert_checkequal ( tf , %t );
tf = checkWriteReadCycle(x,"pred",100,filename);
assert_checkequal ( tf , %t );
x = number_properties("tiny");
tf = checkWriteReadCycle(x,"succ",100,filename);
assert_checkequal ( tf , %t );
tf = checkWriteReadCycle(x,"pred",100,filename);
assert_checkequal ( tf , %t );
x = number_properties("tiniest");
tf = checkWriteReadCycle(x,"succ",100,filename);
assert_checkequal ( tf , %t );
tf = checkWriteReadCycle(x,"pred",100,filename);
assert_checkequal ( tf , %t );
// =============================================================================
