function M1=%hm_3_hm(M1,M2)
  if and(M1.dims==M2.dims)==%f then
    error('>= : arguments have incompatible dimensions')
  end
  M1.entries=(M1.entries>=M2.entries)
endfunction
