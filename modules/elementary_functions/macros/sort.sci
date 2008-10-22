// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function varargout = sort(varargin)
// sort function
// help sort for more informations

  OUT1 ='';
  OUT2 ='';
  
  // get Rhs & Lhs
  [Lhs,Rhs]=argn(0);
  
  // check number input & output
  if ( (Lhs < 1) | (Lhs > 2) ) then
    error(42);
  end
  
  if ( (Rhs < 1) | (Rhs > 2) ) then
    error(41);
  end
  
  // default flag
  F = 'g';
  
  V = varargin(1);
  
  if (Rhs == 2) then 
  
    F = varargin(2);
    // check type argument 2 must be a string
    if (type(F) <> 10) then 
      error(44,2);
    end
    
    //  check if it is a correct flag
    if ( (F <> 'r') & (F <> 'c') ) then 
      error(44,2);
    end
  end
  
  select type(V)
  	case 1 then // matrix
			if isreal(V) then // real
			  [OUT1,OUT2] = gsort(V,F);
			else // complex
  		  realpart = real(V);
			  imagpart = imag(V);
			  modulus = sqrt(realpart.*realpart + imagpart.*imagpart);
			  [OUT1,OUT2] = gsort(modulus,F);
			  OUT1 = V(OUT2);
			end
			
  	case 10 then  // strings
  	  // Warning Compatiblity with previous version
  	  // string are sorting by increasing order
  	  [OUT1,OUT2] = gsort(V,F,'i'); 
  	case 5 then // vector sparse
  	 S = size(V);
  	 if (S <> 1) then
  	   error(msprintf(gettext("%s: Wrong size for first input argument: Vector expected.\n"),"sort"),10000);
  	 else
  	   // Lhs == 1 only with sparse vectors
  	   if (Lhs == 2) then
  	     error(41);
  	   end
  	   
  	   // sort sparse vector
  	   [pout1,pout2] = spget(V);
  	   OUT1 = sparse(pout1,gsort(pout2,F));
  	 end
  	else // others cases
  	  error(246)
  	end
  	
  	// set output
  	varargout(1) = OUT1;
  	if (Lhs == 2) then
  	  varargout(2) = OUT2;
  	end
endfunction

