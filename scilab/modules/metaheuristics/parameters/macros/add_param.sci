function [ga_list,err] = add_param(list_name,param_name,param_value)
[nargout,nargin] = argn();
if typeof(list_name)=='plist' then
  setfield(1,[getfield(1,list_name) param_name],list_name);
  if nargout==2 then err = %F; end
  if nargin==3 then list_name(param_name) = param_value; end
else
  if nargout==2 then
    err = %T; 
  else
    warning('add_param: not a plist');
  end
end
ga_list = list_name;
endfunction

