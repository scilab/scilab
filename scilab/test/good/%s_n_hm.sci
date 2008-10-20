function M=%s_n_hm(s,M)
// Copyright INRIA
//s<>M
if size(s,'*')<> 1 then
  M=%t;return
end
M('entries')=s<>M('entries')
endfunction
