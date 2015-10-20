// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

scicos_log("TRACE");

b = scicos_block();

// Standard filling
b.graphics = mlist(..
["graphics","orig","sz","flip","theta","exprs","pin","pout","pein","peout","gr_i","id","in_implicit","out_implicit"],..
[40,0],..
[10,40],..
%t,..
0,..
"2",..
[0;0],..
0,..
[],..
[],..
list(),..
"",..
["E";"E"],..
"E");

// Omit "sz" and "id", so GEOMETRY#2 and LABEL are missing from the log
b.graphics = mlist(..
["graphics","orig","flip","theta","exprs","pin","pout","pein","peout","gr_i","in_implicit","out_implicit"],..
[40,0],..
%t,..
0,..
"2",..
[0;0],..
0,..
[],..
[],..
list(),..
["E";"E"],..
"E");

// Add a fake wrong property "fake" with value "fake". The log must be the same as above
b.graphics = mlist(..
["graphics","orig","flip","fake","theta","exprs","pin","pout","pein","peout","gr_i","in_implicit","out_implicit"],..
[40,0],..
%t,..
"fake",..
0,..
"2",..
[0;0],..
0,..
[],..
[],..
list(),..
["E";"E"],..
"E");

// Swap "flip" and "in_implicit"
b.graphics = mlist(..
["graphics","orig","in_implicit","fake","theta","exprs","pin","pout","pein","peout","gr_i","flip","out_implicit"],..
[40,0],..
["E";"E"],..
"fake",..
0,..
"2",..
[0;0],..
0,..
[],..
[],..
list(),..
%t,..
"E");


// Check that all the model items are freed
clear
