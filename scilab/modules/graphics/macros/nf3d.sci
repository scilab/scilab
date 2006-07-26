function [xx,yy,zz]=nf3d(x,y,z)
// 3d coding transformation 
// from facets coded in three matrices x,y,z to scilab code for facets 
// accepted by plot3d 
//---------------------------------------------------------
// Copyright INRIA
[n1,n2]=size(x)
ind=ones(1,n1-1).*.[0 1 n1+1 n1]+ (1:n1-1).*.[1 1 1 1];
// ind=[1,2,n1+2,n1+1 , 2,3,n1+3,n1+2, ....  ,n1-1,n1,2n1,2n1-1
ind2=ones(1,n2-1).*.ind+((0:n2-2)*n1).*.ones(ind);
nx=prod(size(ind2))
xx=matrix(x(ind2),4,nx/4);
yy=matrix(y(ind2),4,nx/4);
zz=matrix(z(ind2),4,nx/4);
endfunction
