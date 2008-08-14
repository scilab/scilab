// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function p=derivat(p)
//pd=derivat(p)  computes the derivative of the polynomial or rational
//function marix relative to the dummy variable
//!
  select typeof(p)
  case "constant" then 
    p=0*p
  case "polynomial" then
    [m,n]=size(p);var=varn(p);
    for i=1:m
      for j=1:n
	pij=p(i,j);nij=degree(pij);
	if nij==0 then
	  p(i,j)=0;
	else
	  pij=coeff(pij).*(0:nij);
	  p(i,j)=poly(pij(2:nij+1),var,'c');
	end
      end
    end
  case "rational" then
    num=p.num;den=p.den
    [m,n]=size(num)
    for i=1:m
      for j=1:n
	num(i,j)=derivat(num(i,j))*den(i,j)-num(i,j)*derivat(den(i,j));
	den(i,j)=den(i,j)**2;
      end
    end
    p.num=num;p.den=den;
  else
    error(msprintf(gettext("%s: Wrong type for input argument #%d: A floating point number or polynomial or rational fraction array expected.\n"),"derivat",1))
  end
endfunction
