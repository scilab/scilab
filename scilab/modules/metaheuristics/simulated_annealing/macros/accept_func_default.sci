function Level = accept_func_default(F_current, F_neigh, T)
Level = exp(-(F_neigh - F_current)/T);
endfunction
