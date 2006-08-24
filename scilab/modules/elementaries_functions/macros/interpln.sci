function [y]=interpln(xyd,x)
//Given a set of points in the plane with coordinates xyd and a set of 
//x points 
// [y]=interpln(xyd,x) allows to compute a set of y obtained by linear interpolation.
//% parameters
//  xyd :  coordinates of points (2 rows matrix)
//        first row : x
//        second row : y
//  x   : vector of x's for which y is wanted
//  y   : vector of y computed.
//!
// S Steer INRIA 1991

// Copyright INRIA
[md,nd]=size(xyd);
if md<>2 then
  error('interpln:xyd must have two rows!')
end
//

n=size(x,'*');
d=xyd(:,1:nd-1)-xyd(:,2:nd);
k=find(abs(d(1,1:nd-1))==0)
if k<>[] then
  d(1,k)=%eps*ones(k)
  xyd(1,k+ones(k))=xyd(1,k)+%eps*ones(k);
end
d=d(2,:)./d(1,:);
for pt=1:n,
  xpt=x(pt);
  k=find(xpt>xyd(1,1:nd-1)&xpt<=xyd(1,2:nd));
  if k==[] then
    if xpt<=xyd(1,1) then k=1,else k=nd-1,end
  else
    k=k(size(k,'*'));
  end
  y(1,pt)=xyd(2,k)+(xpt-xyd(1,k))*d(k)
end
endfunction
