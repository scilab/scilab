function out=%s_i_st(i,in,out)
  out=generic_i_st(i,in,out)
  if double(out.dims)==[0 0] then out.dims=int32([1 1]),end
endfunction
