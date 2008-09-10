// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x=tanm(a)
//Matrix wise Tangent
	
	rhs = argn(2);
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"tanm",1));
	end
	
	if a==[] then x=[],return,end
	
	[m,n]=size(a);
	if m<>n then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: Square matrix expected.\n"),"tanm",1));
	end
	
	//diagonalization
	[x,t,bs]=bdiag(a+0*%i*ones(a),1/%eps)
	
	if find(bs>1)<>[] then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: Matrix is not diagonalisable.\n"),"tanm",1));
	end
	
	x=t*diag(tan(diag(x)))/t;
	
	if and(imag(a)==0) then x=real(x),end
	
endfunction
