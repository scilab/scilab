function [xu,yu,zu,xv,yv,zv]=parametricDiffData(X,Y,Z);

[nv,nu]=size(X);

xu=X(:,2:$)-X(:,1:$-1)+%eps;xu(:,$+1)=xu(:,$);
yu=Y(:,2:$)-Y(:,1:$-1)+%eps;yu(:,$+1)=yu(:,$);
zu=Z(:,2:$)-Z(:,1:$-1)+%eps;zu(:,$+1)=zu(:,$);

xv=X(2:$,:)-X(1:$-1,:)+%eps;xv($+1,:)=xv($,:);
yv=Y(2:$,:)-Y(1:$-1,:)+%eps;yv($+1,:)=yv($,:);
zv=Z(2:$,:)-Z(1:$-1,:)+%eps;zv($+1,:)=zv($,:);
