// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) BRUNO PINCON
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function B=%sp_p_s(A,p)
   // handle A^p for scalar integer p only

   if length(p) ~= 1 then
      error("A^p with A sparse is not handled for non scalar p")
   end
   if floor(p) ~= p then
      error("A^p with A sparse is not handled for non integer p")
   end
   if p == 0 then
      B = speye(A)
   elseif p < 0 then
      A = inv(A)
      p = -p
   end
   
   select p
     case 1
       B = A
     case 2
       B = A*A
     case 3
       B = A*(A*A)
     case 4
       B = A*A
       B = B*B
   else
      B = A
      for i=2:p   // slow method (we could use the power algorithm)
	 B = A*B
      end
   end
   
endfunction

  
      