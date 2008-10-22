// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [x,ka,kb]=intersect(a,b)
// returns the vector of common values of two vectors

	[lhs,rhs]=argn()
	if a==[]|b==[] then x=[],ka=[],kb=[],return,end
	[a,ka]=unique(a(:));
	[b,kb]=unique(b(:));
	na=size(a,'*');
	kab=[ka;kb];
	[x,k]=sort([a;b]);x=x($:-1:1)';k=k($:-1:1);
	kab=kab(k)';
	keq=find(x(2:$)==x(1:$-1));
	if keq==[] then
		x=[],ka=[];kb=[]
	else
		x=x(keq)
		ka=kab(keq+1)
		kb=kab(keq)
	end
endfunction
