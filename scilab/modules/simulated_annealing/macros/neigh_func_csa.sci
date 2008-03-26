function x_neigh = neigh_func_csa(x_current, T, param)
if ~isdef('param','local') then
  param = [];
end
if is_param(param,'sigma') then
  Sigma = get_param(param,'sigma');
else
  Sigma = ones(size(x_current,1), size(x_current,2));
end
if is_param(param,'min_bound') then
  Min = get_param(param,'min_bound');
else
  Min = -%inf*ones(size(x_current,1),size(x_current,2));
end

x_neigh = x_current + Sigma.*sqrt(2)*T*rand(1,1,'norm');
x_neigh = max(min(x_neigh, Max),Min);
endfunction
