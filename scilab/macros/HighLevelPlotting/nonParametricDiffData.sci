function [zx,zy]=nonParametricDiffData(x,y,Z);

nx=length(x);
ny=length(y);

zx=(Z(:,2:$)-Z(:,1:$-1))*sparse([1:nx-1;1:nx-1]',1../(x(2:$)-x(1:$-1)));
zy=sparse([1:ny-1;1:ny-1]',1../(y(2:$)-y(1:$-1)))*(Z(2:$,:)-Z(1:$-1,:));

zx(:,$+1)=zx(:,$);
zy($+1,:)=zy($,:);
