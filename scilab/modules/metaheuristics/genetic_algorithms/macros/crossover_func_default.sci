function [Crossed_Indiv1, Crossed_Indiv2] = crossover_func_default(Indiv1,Indiv2,param)
if ~isdef('param','local') then
  param = [];
end

// We deal with some parameters to take into account the boundary of the domain and the neighborhood size
Beta      = get_param(param,'beta',0);
MinBounds = get_param(param,'minbound',-2*ones(size(Indiv1,1),size(Indiv1,2)));
MaxBounds = get_param(param,'maxbound',2*ones(size(Indiv1,1),size(Indiv1,2)));

mix = (1 + 2*Beta)*rand(1,1) - Beta;
Crossed_Indiv1 =     mix*Indiv1 + (1-mix)*Indiv2;
Crossed_Indiv2 = (1-mix)*Indiv1 +     mix*Indiv2;

Crossed_Indiv1 = max(min(Crossed_Indiv1, MaxBounds),MinBounds);
Crossed_Indiv2 = max(min(Crossed_Indiv2, MaxBounds),MinBounds);
endfunction
