function [res,err] = is_param(list_name,param_name)
[nargout,nargin] = argn();
res = [];
if typeof(list_name)=='plist' then
  res = or(getfield(1,list_name)==param_name);
  if nargout==2 then err = %F; end
else
  if nargout==2 then 
    err = %T;
  else
    warning('get_param: not a plist');
  end
end
endfunction

