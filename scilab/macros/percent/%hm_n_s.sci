function M=%hm_n_s(M,s)
// Copyright INRIA
//M<>s
if size(s,'*')<> 1 then
  M=%t;return
end
M('entries')=M('entries')<>s
endfunction
