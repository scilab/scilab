function [xf,yf,zf]=eval3dPolygons(x,y,fun,nbVertices,isParametric)

//
// Generates polygons (N=3 trianges or N=4 quadrilaters) from
// function/numerical data x,y,fun (non parametric surface z=fun(x,y)
// or parametric surface [x,y,z]=fun(u,v)
//
// This function is a quick hack from SCI/macros/xdess/eval3dp.sci
// Original code due to S. Steer, (C) INRIA
//
// S. Mottelet, UTC, 99
//

n1=prod(size(x))
n2=prod(size(y))

// On calcule la valeur de la fonction en tous les couples (x(i),y(j))


if isParametric
   x=ones(1,n2).*.matrix(x,1,n1);
   y=matrix(y,1,n2).*.ones(1,n1);
   [vx,vy,vz]=fun(x,y)
else
   vx=ones(1,n2).*.matrix(x,1,n1);
   vy=matrix(y,1,n2).*.ones(1,n1);
   vz=fun(vx,vy);
end

x=[];y=[];


if nbVertices==4
   ind=ones(1,n1-1).*.[0 1 n1+1 n1]+ (1:n1-1).*.[1 1 1 1];
elseif nbVertices==3
   ind=ones(1,n1-1).*.[0 1 n1 1 n1+1 n1]+ (1:n1-1).*.[1 1 1 1 1 1];
else
   error('eval3dPolygons : nb of vertices must be 3 or 4');
end

ind2=ones(1,n2-1).*.ind+((0:n2-2)*n1).*.ones(ind);

n=prod(size(ind2));

xf=matrix(vx(ind2),nbVertices,n/nbVertices);
yf=matrix(vy(ind2),nbVertices,n/nbVertices);
zf=matrix(vz(ind2),nbVertices,n/nbVertices);

endfunction
