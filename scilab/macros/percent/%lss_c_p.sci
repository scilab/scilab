function [s]=%lss_c_p(s1,d2)
//s=%lss_c_p(s1,d2) 
// u=[u1;u2]    y=y1+d2*u1
// s=[s1,d2]
// Copyright INRIA
[a1,b1,c1,d1,x1,dom1]=s1(2:7)
[n1,m1]=size(b1);[p2,m2]=size(d2);
s=tlist(['lss','A','B','C','D','X0','dt'],a1,[b1 0*ones(n1,m2)],c1,[d1 d2],x1,dom1)
endfunction
