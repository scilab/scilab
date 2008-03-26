function [res,err] = get_param(list_name,param_name,param_default)
[nargout,nargin] = argn();
if ~isdef('param_default','local') then
  param_default = [];
end

res = param_default;

if typeof(list_name)=='plist' then
  if is_param(list_name,param_name) then
    res = list_name(param_name);
    if nargout==2 then 
      err = %F; 
    end
  else
    if nargout==2 then 
      err = %T; 
    else
      warning(sprintf('get_param: parameter %s not defined',param_name));
    end
  end
else
  if nargout==2 then 
    err = %T; 
  else
    warning('get_param: not a plist');
  end
end
endfunction

