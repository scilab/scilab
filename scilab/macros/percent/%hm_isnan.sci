function b=%hm_isnan(M)
  b=hypermat(M.dims,isnan(M.entries))
endfunction
