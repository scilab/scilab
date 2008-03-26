function [Pop_out,FObj_Pop_out,Efficiency,MO_Total_FObj_out] = selection_func_random(Pop_in,Indiv1,Indiv2,FObj_Pop_in,FObj_Indiv1,FObj_Indiv2, ...
                                                                                     MO_Total_FObj_in,MO_FObj_Indiv1,MO_FObj_Indiv2,param)

[nargout,nargin] = argn();

mo_is_defined = ~isempty('MO_Total_FObj_in');

if ~isdef('param','local') then
  param = [];
end

pressure = get_param(param,'pressure',0.05);

pop_size = length(Pop_in);

Total_Pop  = lstcat(Pop_in, Indiv1, Indiv2);
Total_FObj = [FObj_Pop_in' FObj_Indiv1' FObj_Indiv2']';

// Normalization of the efficiency
FObj_Pop_Max = max(Total_FObj);
FObj_Pop_Min = min(Total_FObj);

Efficiency = (1 - pressure) * (FObj_Pop_Max - Total_FObj)/max([FObj_Pop_Max - FObj_Pop_Min, %eps]) + pressure;

// We select at random pop_size individuals
Index_selected = ceil((length(Total_FObj)-1)*rand(pop_size,1) + 1);

// Extraction and selection of the phenotype
FObj_Pop_out = Total_FObj(Index_selected);
// Extraction and selection of the genotype
Pop_out = list(Total_Pop(Index_selected));
// Extraction of the efficiencies
Efficiency = Efficiency(Index_selected);
// Extraction of the multiobjective values (if defined)
if mo_is_defined then
  // MO_Total_FObj for multiobjective function values
  MO_Total_FObj_out = [MO_Total_FObj_in' MO_FObj_Indiv1' MO_FObj_Indiv2']';
  MO_Total_FObj_out = MO_Total_FObj_out(Index_selected,:);
end

Total_Pop = list(); // Reinitialisation of Total_Pop
endfunction

