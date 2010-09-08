// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function y=sinc(x,fl)
//               [  sin(x(i))/x(i) if x(i)~=0
// computes y(i)=[
//               [  1 if x(i)~=0
  if argn(2)==2 then // for compatibility
    warning('obsolete use of sinc, use filt_sinc instead')
    ffilt=ffilt;//load ffilt and its subfunctions
    y=filt_sinc(x,fl)
    return
  end
  y=ones(x)
  kz=find(x<>0)
  y(kz)=sin(x(kz))./(x(kz));
endfunction
