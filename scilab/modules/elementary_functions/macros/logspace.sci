// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function y=logspace(d1, d2, n)
// Logarithmically spaced vector.
// logspace(d1, d2) generates a row vector of n logarithmically
// equally spaced points between 10^d1 and 10^d2.  If d2=%pi
// then the points are between 10^d1 and pi.
// logspace(d1, d2, n) generates n points.

	if argn(2) == 2 then n = 50;end
	if d2==%pi then d2 = log(%pi)/log(10);end
	y=10^(d1+[(0:n-2)*(d2-d1)/(floor(n)-1), d2-d1]);
endfunction
