// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) -2011 - INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function varargout=%hm_find(Mb,nmax)
  lhs=argn(1)
  if argn(2)==2 then
    I=find(Mb.entries,nmax)
  else
    I=find(Mb.entries)
  end
  if lhs==1 then
    varargout=list(I)
  else
    dims=size(Mb);
    nd=size(dims,'*')
    if nd>lhs then
      dims=[dims(1:lhs-1) prod(dims(lhs:$))]
    else
      dims(nd+1:lhs)=1;
    end
    varargout=list()
    I=ind2sub(dims,I)
    for k=1:size(I,2)
      varargout(k)=I(:,k)'
    end
  end
endfunction
