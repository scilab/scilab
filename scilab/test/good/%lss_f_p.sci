function [s]=%lss_f_p(s1,d2)
//[s]=%lss_f_p(s1,d2) <=> s=[s1;d2]
//!
// origine s. steer inria 1987
//
// Copyright INRIA
[a1,b1,c1,d1,x1,dom1]=s1(2:7)
[n1,m1]=size(c1);[p2,m2]=size(d2);
s=tlist(['lss','A','B','C','D','X0','dt'],a1,b1,[c1;0*ones(p2,m1)],[d1;d2],x1,dom1)
endfunction
