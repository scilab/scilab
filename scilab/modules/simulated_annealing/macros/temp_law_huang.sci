function T = temp_law_huang(T, step_mean, step_var, temp_stage, n,param)
if (~isdef('param','local')) then
  param = []; // First create the empty param var
end
if is_param(param,'lambda') then
  lambda = get_param(param,'lambda');
else
 lambda = 0.01;
end
T = T * exp(-lambda*T/step_var);
endfunction
