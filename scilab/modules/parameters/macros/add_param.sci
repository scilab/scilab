// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [ga_list,err] = add_param(list_name,param_name,param_value)
[nargout,nargin] = argn();
if typeof(list_name)=='plist' then
  setfield(1,[getfield(1,list_name) param_name],list_name);
  if nargout==2 then err = %F; end
  if nargin==3 then list_name(param_name) = param_value; end
else
  if nargout==2 then
    err = %T; 
  end
  warning(sprintf(gettext("%s: not a plist"),"add_param"));
end
ga_list = list_name;
endfunction

