// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function s=struct(varargin)
// Equivalent for Matlab struct function

rhs=argn(2)

fields=["st","dims"]

if rhs==0 then
  // No Matlab equivalent
  s=mlist(fields,int32([0,0]))
  return
end

if floor(rhs/2)*2<>rhs then
  error(msprintf(gettext("%s: Wrong number of input argument(s)."),'struct'));
end

nbfields=size(varargin)/2

dims=[]
for kf=1:2:size(varargin)
  fields=[fields varargin(kf)]
end

dims=[1 1]
// Search struct size
for kf=2:2:size(varargin)
  if typeof(varargin(kf))=="ce" then
    if or(double(varargin(kf).dims)<>[1 1]) then
      dims=varargin(kf).dims
      break
    end
  end
end
s=mlist(fields,int32(dims))

// Search if one value is a scalar cell
fnb=3
for kf=2:2:size(varargin)
  if typeof(varargin(kf))<>"ce" then
    value=list()
    for kk=1:prod(double(dims))
      value(kk)=varargin(kf)
    end
  elseif and(double(varargin(kf).dims)==[1 1]) then
    value=list()
    for kk=1:prod(double(dims))
      value(kk)=varargin(kf).entries
    end
  else
    value=varargin(kf).entries
  end
  if prod(double(dims))==1 then
  setfield(fnb,value(1),s)
  else
    setfield(fnb,value,s)
  end
  fnb=fnb+1
end

endfunction
