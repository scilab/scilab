function T = temp_law_vfsa(T, step_mean, step_var, temp_stage, n, param)
if (~isdef('param','local')) then
  param = []; // First create the empty param var
end
if is_param(param,'c') then
  c = get_param(param,'c');
else
  c = 0.01;
end
if is_param(param,'dimension') then
  dimension = get_param(param,'dimension');
else
  dimension = 3;
end
T = T * (n/(n+1))^(1/dimension);
endfunction
