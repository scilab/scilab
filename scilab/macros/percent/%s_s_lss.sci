function s=%s_s_lss(d1,s2)
//s=d1-s2
// Copyright INRIA
[a2,b2,c2,d2,x2,dom2]=s2(2:7),
s=tlist(['lss','A','B','C','D','X0','dt'],a2,-b2,c2,d1-d2,x2,dom2),
endfunction
