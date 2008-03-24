function [Crossed_Indiv1, Crossed_Indiv2] = crossover_func_ising2d(Indiv1,Indiv2,param)
if ~isdef('param','local') then
  param = [];
end

Index_x_min = ceil(rand()*(size(Indiv1,1)-%eps)+%eps);
Index_x_max = ceil(rand()*(size(Indiv1,1)-%eps)+%eps);
Index_y_min = ceil(rand()*(size(Indiv1,1)-%eps)+%eps);
Index_y_max = ceil(rand()*(size(Indiv1,1)-%eps)+%eps);

if Index_x_min>Index_x_max then
  tmp         = Index_x_min;
  Index_x_min = Index_x_max;
  Index_x_max = tmp;
end

if Index_y_min>Index_y_max then
  tmp         = Index_y_min;
  Index_y_min = Index_y_max;
  Index_y_max = tmp;
end


Crossed_Indiv1 = Indiv1;
Crossed_Indiv1(Index_x_min:Index_x_max,Index_y_min:Index_y_max) = Indiv2(Index_x_min:Index_x_max,Index_y_min:Index_y_max);
Crossed_Indiv2 = Indiv2;
Crossed_Indiv2(Index_x_min:Index_x_max,Index_y_min:Index_y_max) = Indiv1(Index_x_min:Index_x_max,Index_y_min:Index_y_max);
endfunction
