function [nx,ny,nz]=surfnorm(varargin)

[lhs,rhs]=argn(0);

if rhs==1
   Z=varargin(1);
   s=size(argList(1));
   X=1:s(1); 
   Y=1:s(2);
elseif rhs==3
   [X,Y,Z,surfaceIsParam]=checkXYZQuadruple('surfnorm',varargin(1),varargin(2),varargin(3));
   [ly,lx]=size(Z);
   
   if ~surfaceIsParam
      X=X(:);Y=Y(:);
      [zx,zy]=nonParametricDiffData(X,Y,Z);
      n=nonParametricNormals(zx,zy);
   else
      [xu,yu,zu,xv,yv,zv]=parametricDiffData(X,Y,Z);
      n=parametricNormals(xu,yu,zu,xv,yv,zv);
   end

   nx=matrix(n(1,:),ly,lx);
   ny=matrix(n(2,:),ly,lx);
   nz=matrix(n(3,:),ly,lx);

elseif rhs==4
   [nodes,x,y,z]=varargin(1:4);
end

