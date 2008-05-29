// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x=signm(a)
//   signm - computes the matrix sign function.
//%CALLING SEQUENCE
//   x=signm(a)
//%PARAMETERS
//   a   : square hermitian matrix
//   x   : square hermitian matrix
//%DESCRIPTION
//   This macro is called by the function sign to compute square matrix
//   sign function.

	[m,n]=size(a);
	
	if m<>n then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: Square matrix expected.\n"),"signm",1));
	end
	
	flag=or(a<>a');
	
	if flag then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: Non-hermitian matrix.\n"),"signm",1));
	end
	
	[u,s]=schur(a);
	x=u'*diag(sign(real(diag(s))))*u;
endfunction
