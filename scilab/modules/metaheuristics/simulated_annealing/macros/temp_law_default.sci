function T = temp_law_default(T, step_mean, step_var, temp_stage, n,param)
if (~isdef('param','local')) then
  param = []; // First create the empty var param
end
if is_param(param,'beta') then
  Beta = get_param(param,'beta');
else
  Beta = 0;
end

if (is_param(param,'alpha')) then
  _alpha = get_param(param,'alpha');
else
  _alpha = 0.9;
end
T = _alpha*T;
endfunction
