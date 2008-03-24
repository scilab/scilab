function Pop_init = init_func_default(popsize,param)
if ~isdef('param','local') then
  param = [];
end

// We deal with some parameters to take into account the boundary of the domain and the neighborhood size
Dim       = get_param(param,'dimension',2);
MinBounds = get_param(param,'minbound',-2*ones(1,Dim));
MaxBounds = get_param(param,'maxbound',2*ones(1,Dim));

// Pop_init must be a list()
Pop_init = list();
for i=1:popsize
  Pop_init(i) = (MaxBounds - MinBounds).*rand(size(MaxBounds,1),size(MaxBounds,2)) + MinBounds;
end
endfunction
