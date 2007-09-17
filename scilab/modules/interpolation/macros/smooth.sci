function [pt]=smooth(ptd,pas)
//
// Copyright INRIA
[lhs,rhs]=argn(0)
[m,n]=size(ptd)
d=splin(ptd(1,:),ptd(2,:))
if rhs==1 then l=abs(ptd(1,n)-ptd(1,1));pas=l/100;end
pt=[ptd(1,1)+pas:pas:ptd(1,n)]
pt=[ptd(:,1) [pt;interp(pt,ptd(1,:),ptd(2,:),d)] ptd(:,n)]
endfunction
