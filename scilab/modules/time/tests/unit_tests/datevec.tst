// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for datevec function
// =============================================================================


warning("off"); // WARNING_EMPTY_OPS
if datevec(1) <> [0,1,1,0,0,0] then pause,end
if datevec(719529) <> [1970,1,1,0,0,0] then pause,end

in  = [ 158493 247745 637308 ; 567032 499035 514298 ; 165 471293 658662 ];
out = [ 433 12 8 0 0 0; ..
1552 6  24  0 0 0 ; ..
0    6  13  0 0 0 ; ..
678  4  20  0 0 0 ; ..
1366 4  23  0 0 0 ; ..
1290 5  9   0 0 0 ; ..
1744 11 20  0 0 0 ; ..
1408 2  6   0 0 0 ; ..
1803 5  10  0 0 0];

if datevec(in) <> out then pause,end

in  = [ 250000 500000 750000 1000000 ];
out = datevec(in);
if size(out) <> [ 4 6 ] then pause,end

in  = [ 250000 500000 ; 750000 1000000 ];
out = datevec(in);
if size(out) <> [ 4 6 ] then pause,end


in  = [ 250000 ; 500000 ; 750000 ; 1000000 ];
out = datevec(in);
if size(out) <> [ 4 6 ] then pause,end
