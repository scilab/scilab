// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [x,k]=unique(x)
// extract unique components of a vector
	if size(x,'*')==1 then
		k=1
	else
		[lhs,rhs]=argn()
		if lhs==2 then
		  [x,k]=sort(x);
		  keq=find(x(2:$)==x(1:$-1))
		  if keq<>[] then keq=keq+1;end 
		  x(keq)=[]
		  k(keq)=[]
		  k=k($:-1:1)
		  x=x($:-1:1)
		else
		  x=sort(x);x=x($:-1:1);
		  x(find(x(2:$)==x(1:$-1)))=[]
		end
	end
endfunction
