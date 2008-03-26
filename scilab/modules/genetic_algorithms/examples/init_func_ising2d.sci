function Pop_init = init_func_ising2d(popsize,param)
if ~isdef('param','local') then
  param = [];
end

// We deal with some parameters to take into account the boundary of the domain and the neighborhood size
Dim = get_param(param,'dimension',10);

// Pop_init must be a list()
Pop_init = list();
for i=1:popsize
  Pop_init(i) = init_ising2d(Dim,rand());
end
endfunction
