// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function varargout=%hm_find(Mb,nmax)
  if argn(2)==1 then nmax=-1,end
  N=double(Mb.dims); n=length(N) 
  
  i=find(Mb.entries,nmax)
  if argn(1)==1 then 
    varargout=list(i)
    return
  end
  if i==[] then
    varargout=list()
    for k=1:n,varargout($+1)=[],end
  else
    varargout=list(modulo(i-1,N(1))+1)
    for k=1:n-1
      varargout($+1)=modulo(int((i-1)/prod(N(1:k))),N(k+1))+1     
    end
  end
endfunction
