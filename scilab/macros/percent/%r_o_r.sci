function r=%r_o_r(l1,l2)
// l1==l2 with l1 and l2 rationals
//!
//r=l1('dt')==l2('dt')&l1('num')==l2('num')&(l1('den')==l2('den')|l1('num')==0)
// Copyright INRIA
if varn([l1('num'),l1('den')])~=varn([l2('num'),l2('den')]) then 
  r=%f;
else
r=(l1('dt')==l2('dt'))&((l1-l2)==0)
end
endfunction
