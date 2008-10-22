// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function M1=%hm_c_hm(M1,M2)

   d1=M1.dims;
   d2=M2.dims;
   if size(d1,'*')<>size(d2,'*') then
      error('In concatenation the number of dimensions for each component must match');
   end

   kd=find(d1<>d2)
   kd(find(kd==2))=[]
   if kd<>[] then
      error('inconsistent dimensions')
   end
   
   M1.entries=matrix([matrix(M1.entries,double(d1(1)*d1(2)),-1);...
		      matrix(M2.entries,double(d2(1)*d2(2)),-1)] ,-1,1)
   M1.dims(2)=d1(2)+d2(2)
endfunction
