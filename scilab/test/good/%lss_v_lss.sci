function [s]=%lss_v_lss(s1,s2)
//s=%lss_v_lss(s1,s2)  <=> s = s1 "fedback" with s2
//             <=> s=s1/.s2    (in state-space)
//!
// origine s. steer inria 1987
// Copyright INRIA
[s1,s2]=sysconv(s1,s2)
[a1,b1,c1,d1,x1,dom1]=s1(2:7)
[a2,b2,c2,d2,x2]=s2(2:6);
e12=1/(eye()+d2*d1)
e21=eye()-d1*e12*d2;b21=b2*e21;e12=b1*e12
a=[a1-e12*d2*c1 , -e12*c2 ; b21*c1 , a2-b21*d1*c2]
s=tlist(['lss','A','B','C','D','X0','dt'],a,[e12;b21*d1],e21*[c1 -d1*c2],...
            e21*d1,[x1;x2],dom1)
endfunction
