function Level = accept_func_vfsa(F_current, F_neigh, T)
Level = 1 / (1 + exp(-(F_current - F_neigh)/T));
endfunction
