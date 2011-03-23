// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO 2008-2010 - Yann COLLETTE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [string_list,err] = list_param(list_name)
[nargout,nargin] = argn();
string_list = [];
if typeof(list_name)=='plist' then
  string_list = getfield(1,list_name);
  string_list = string_list(2:$);
  if nargout==2 then err = %F; end
else
  if nargout==2 then
    err = %T; 
  else
    error(sprintf(gettext("%s: Wrong type for input argument #%d: %s expected.\n"), "list_param", 1, "plist"));
  end
end
endfunction
