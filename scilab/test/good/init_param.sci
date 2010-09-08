// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO 2008-2010 - Yann COLLETTE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function plist = init_param(varargin)
param_name = [];
param_val  = [];
if (modulo(length(varargin),2)~=0) then
  error(sprintf(gettext("%s: Wrong number of input argument(s): An even number expected.\n"),"init_param"));
end
for i=1:2:length(varargin)
  if typeof(varargin(i))~='string' then
    error(sprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"init_param",i));
  end
end

plist = mlist(['plist'],[]);

for i=1:2:length(varargin)
  [plist, _err] = add_param(plist,varargin(i),varargin(i+1));
end
endfunction
