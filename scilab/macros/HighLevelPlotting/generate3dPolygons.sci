function [X,Y,Z]=generate3dPolygons(X,Y,Z,nbVertices,isParametric)

//
// Generates polygons (N=3 trianges or N=4 quadrilaters) from
// numerical parametric data X,Y,Z (matrices of the same size)
//
// This function is a quick hack from SCI/macros/xdess/eval3dp.sci
// Original code due to S. Steer, (C) INRIA
//
// S. Mottelet, UTC, 99
//

[nv,nu]=size(Z);

rev=$:-1:1;
	 
if isParametric 
//   X=X(rev,:);Y=Y(rev,:);Z=Z(rev,:); // to solve the pb with plot3d1
   X=X(:);Y=Y(:);Z=Z(:);
else
//   X=X(rev);
//   Z=Z(:,rev);
   X=ones(1,nu).*.matrix(X,1,nv);
   Y=matrix(Y,1,nu).*.ones(1,nv);
end

if nbVertices==4
   ind=ones(1,nv-1).*.[0 1 nv+1 nv]+ (1:nv-1).*.[1 1 1 1];
elseif nbVertices==3
   ind=ones(1,nv-1).*.[0 1 nv 1 nv+1 nv]+ (1:nv-1).*.[1 1 1 1 1 1];
else
   error('generate3dPolygons : nb of vertices must be 3 or 4');
end

ind2=ones(1,nu-1).*.ind+((0:nu-2)*nv).*.ones(ind);
n=prod(size(ind2));
X=matrix(X(ind2),nbVertices,n/nbVertices);
Y=matrix(Y(ind2),nbVertices,n/nbVertices);
Z=matrix(Z(ind2),nbVertices,n/nbVertices);	 
