// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function t=justify(t,job)
  //Justify character array.
  [m,n]=size(t)
  for k=1:n
    L=length(t(:,k));mx=max(L);
    if part(job,1)=='l' then //right
      t(:,k)=part(t(:,k),1:mx)
    elseif part(job,1)=='r' then //left
      for l=1:m
	t(l,k)=part(' ',1:(mx-length(t(l,k))))+t(l,k)
      end
    elseif part(job,1)=='c' then //center
      for l=1:m
	nl=int((mx-length(t(l,k)))/2)
	nr=(mx-length(t(l,k)))-nl
	t(l,k)=part(' ',1:nl)+t(l,k)+part(' ',1:nr)
      end
    else
      error(msprintf(gettext("%s: Wrong values for input argument #%d: ''r'',''l'' or ''c'' expected.\n"),"justify",2));
    end
  end
endfunction
