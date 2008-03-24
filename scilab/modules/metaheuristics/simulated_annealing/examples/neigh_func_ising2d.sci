function x_neigh = neigh_func_ising2d(x_current, T, param)
if ~isdef('param','local')
  param = [];
end

Index_x = ceil(rand()*(size(x_current,1)-%eps)+%eps);
Index_y = ceil(rand()*(size(x_current,1)-%eps)+%eps);

x_neigh = x_current;
x_neigh(Index_x,Index_y) = x_neigh(Index_x,Index_y)*(-1);
endfunction

