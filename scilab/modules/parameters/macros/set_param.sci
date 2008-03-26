function [ga_list,err] = set_param(list_name,param_name,param_value)
[nargout,nargin] = argn();
ga_list = list_name;
if typeof(list_name)=='plist' then
  if nargout==2 then err = %F; end
  if is_param(ga_list,param_name) then
    ga_list(param_name) = param_value;
  else
    err = %T;
    warning('set_param: parameter doesn''t exist');
  end
else
  if nargout==2 then 
    err = %T;
  else
    warning('set_param: not a plist');
  end
end
endfunction

