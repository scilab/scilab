function [K,Y,err]=leqe(P21,Qx)
// Copyright INRIA
[A,B1,C2,D21,xo,dom]=P21(2:7)
[KT,Y,err]=leqr(syslin(dom,A',C2',B1',D21'),Qx);
K=KT';
endfunction
