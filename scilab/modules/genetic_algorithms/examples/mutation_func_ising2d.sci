function Mut_Indiv = mutation_func_ising2d(Indiv,param)
if ~isdef('param','local') then
  param = [];
end
// We deal with some parameters to take into account the boundary of the domain and the neighborhood size
Proba = get_param(param,'proba',0.1);
ChangeMatrix = rand(size(Indiv,1),size(Indiv,2));
Index = find(ChangeMatrix<0.1);
Mut_Indiv = Indiv;
Mut_Indiv(Index) = Mut_Indiv(Index)*(-1);
endfunction
