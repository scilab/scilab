function Mut_Indiv = mutation_func_default(Indiv,param)
if ~isdef('param','local') then
  param = [];
end
// We deal with some parameters to take into account the boundary of the domain and the neighborhood size
Delta     = get_param(param,'delta',0.1);
MinBounds = get_param(param,'minbound',-2*ones(size(Indiv,1),size(Indiv,2)));
MaxBounds = get_param(param,'maxbound', 2*ones(size(Indiv,1),size(Indiv,2)));

Mut_Indiv = Indiv + 2*Delta*rand(size(Indiv,1),size(Indiv,2)) - Delta*ones(size(Indiv,1),size(Indiv,2));

Mut_Indiv = max(min(Mut_Indiv, MaxBounds),MinBounds);
endfunction
