function [s]=%lss_m_lss(s1,s2)
[a1,b1,c1,d1,x1,dom1]=s1(2:7),
[a2,b2,c2,d2,x2]=s2(2:6),
b1c2=b1*c2,
s=list('lss',[a1,b1c2;0*b1c2' ,a2],...
       [b1*d2;b2],[c1,d1*c2],d1*d2,[x1;x2],dom1),

