// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [result,err] = is_param(list_name,param_name)
[nargout,nargin] = argn();
result = [];
if typeof(list_name)=='plist' then
  result = or(getfield(1,list_name)==param_name);
  if nargout==2 then err = %F; end
else
  if nargout==2 then 
    err = %T;
  else
    warning(sprintf(gettext("%s: not a plist"),"get_param"));
  end
end
endfunction

