// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [x_out,ka_out,kb_out]=intersect(a_in,b_in)
// returns the vector of common values of two vectors

	[lhs,rhs]=argn()
	pause
	if a_in==[]|b_in==[] then x=[],ka=[],kb=[],return,end
	[a_in,ka]=unique(a_in(:));
	[b_in,kb]=unique(b_in(:));
	//na=size(a_in,'*');
	//nb=size(b_in,'*');
	kab = [ka;kb];
	[x,k] = sort([a_in;b_in]);
	x = x($:-1:1)';
	k = k($:-1:1);
	kab = kab(k)';
	keq = find( x(2:$) == x(1:$-1) );
	if keq == [] then
		x_out = [];
		ka_out = [];
		kb_out = [];
	else
		x_out = x(keq);
		ka_out = kab(keq+1);
		kb_out = kab(keq);
	end
endfunction
