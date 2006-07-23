function [nutr,A]=mesh2d(x,y,front)
// Copyright INRIA
[lhs,rhs]=argn(0)
if (rhs>3|rhs<2) then error(39), end
nbs=size(x,2);n1=size(y,2);
if(nbs <> n1) then 
  error('Incompatible arrays dimensions')
end
if(nbs < 3) then 
  error('Minimum 3 points needed')
end
n6=6*nbs-9;
n4=4*nbs-4;
if (rhs == 2) then
[iadj,iend,nbt,nutr,ierr]=m6deumesh(nbs,n4,n6,x,y);
if ierr<>0 then
    error('all nodes are colinear');
  return;
end
else  
  n6=6*(nbs+nbs-2)
  lfront=size(front,2);
  cr=[x;y];
  tri=zeros(1,n4);
  [nbt,nutr,c,err]=m6mesh2b(nbs,n6,n4,lfront,cr,tri,front);
if err<>0 then
  select err
    case 2
    error('some points are identical');
  case 5
    error('wrong boundary array');
  case 6
    error('crossed boundary');
  case 7 
    error('wrong orientation of the boundary');
  case 8
    error('size limitation');
  case 9
    error('crossed boundary');
  case 10
    error('an interior point is on the boundary');
  case 12
    error('some points are identical or size limitation');
end
end
  ntot=3*nbt;nunu=nutr(1:ntot);
  nutr=matrix(nunu,3,nbt);
end;
ii=find(nutr(1,:)==0); nutr(:,ii)=[];
if lhs==2 then
  jj=[nutr(1,:)' nutr(2,:)';nutr(2,:)' nutr(3,:)';nutr(3,:)' nutr(1,:)'];
  A=sparse(jj,ones(size(jj,1),1));
  A=bool2s((A<>0)|(A'<>0));
  A=A+speye(A);
end
endfunction
