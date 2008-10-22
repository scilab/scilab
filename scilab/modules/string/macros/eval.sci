// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [h]=eval(z)
	// Syntax :  H = eval(Z)
	// returns the evaluation of the matrix of character strings Z.
	// Example: a=1; b=2; Z=['a','b'] ; eval(Z) returns the matrix [1,2];
	//!
	
	[mz,nz]=size(z),
	if mz*nz>1 then
		texte=[];
		for l=1:mz,for k=1:nz,
			texte=[texte;'%h('+string(l)+','+string(k)+')='+z(l,k)],
		end
	end
	else
	texte='%h='+z
	end
	deff('[%h]=%eval()',texte),
	h=%eval(),
endfunction
