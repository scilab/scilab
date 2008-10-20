function r=%r_n_r(l1,l2)
//%r_n_r(l1,l2) <=> l1<>l2 for rational matrices
//!
// Copyright INRIA
r=l1('dt')<>l2('dt')
if ~r then
  r=l1('num')<>l2('num')|(l1('den')<>l2('den')&l1('num')<>0)
end
endfunction
