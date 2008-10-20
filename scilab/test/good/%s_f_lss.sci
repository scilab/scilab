function s=%s_f_lss(d1,s2)
//s=%s_f_lss(d1,s2) <=>  s=[d1,s2]
//!
// origine s. steer inria 1987
//
// Copyright INRIA
[a2,b2,c2,d2,x2,dom2]=s2(2:7)
[n2,m2]=size(c2);[p1,m1]=size(d1)
s=tlist(['lss','A','B','C','D','X0','dt'],a2,b2,[0*ones(p1,m2);c2],..
    [d1;d2],x2,dom2)
endfunction
