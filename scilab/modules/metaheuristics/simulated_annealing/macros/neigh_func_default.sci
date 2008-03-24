function x_neigh = neigh_func_default(x_current, T, param)
if ~isdef('param','local') then
  param = [];
end

if is_param(param,'min_delta') then
  sa_min_delta = get_param(param,'min_delta');
else
  sa_min_delta = -0.1*ones(size(x_current,1),size(x_current,2));
end
if is_param(param,'max_delta') then
  sa_max_delta = get_param(param,'max_delta');
else
  sa_max_delta = 0.1*ones(size(x_current,1),size(x_current,2));
end
if is_param(param,'min_bound') then
  Min = get_param(param,'min_bound');
else
  Min = -%inf*ones(size(x_current,1),size(x_current,2));
end

if is_param(param,'max_bound') then
  Max = get_param(param,'max_bound');
else
  Max = %inf*ones(size(x_current,1),size(x_current,2));
end

x_neigh = x_current + (sa_max_delta - sa_min_delta).*rand(size(x_current,1),size(x_current,2)) + sa_min_delta;
x_neigh = max(min(x_neigh, Max),Min);
endfunction
