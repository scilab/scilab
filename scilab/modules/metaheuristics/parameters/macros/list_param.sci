function [string_list,err] = list_param(list_name)
[nargout,nargin] = argn();
if typeof(list_name)=='plist' then
  string_list = getfield(1,list_name);
  string_list = string_list(2:$);
  if nargout==2 then err = %F; end
else
  if nargout==2 then
    err = %T; 
  else
    warning('list_param: not a plist');
  end
end
endfunction

