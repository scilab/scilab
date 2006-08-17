function [K,X]=lqe(P21)
// Copyright INRIA
[A,B1,C2,D21,xo,dom]=P21(2:7)
[kk,X]=lqr(syslin(dom,A',C2',B1',D21'));
K=kk';
endfunction
