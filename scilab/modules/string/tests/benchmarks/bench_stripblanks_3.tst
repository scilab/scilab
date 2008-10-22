// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for stripblanks function
//==============================================================================

str     = ascii(9)+" "+ascii(9)+"test    "+ascii(9);
str_mat = emptystr(50,50);

for i=1:50
	for j=1:50
		str_mat(i,j) = str;
	end
end

// <-- BENCH START -->
a = stripblanks(str_mat,%T);
// <-- BENCH END -->
