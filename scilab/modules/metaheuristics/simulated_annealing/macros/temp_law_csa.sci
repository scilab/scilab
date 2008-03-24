function T = temp_law_csa(T, step_mean, step_var, temp_stage, n,param)
T = T * log(1+temp_stage)/log(2+temp_stage);
endfunction
