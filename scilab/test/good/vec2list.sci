function li=vec2list(bigVector,varsizes,ind)
//bigVector: big vector
//varsizes: k x 2 matrix, varsizes(i,:)=size of ith matrix
//li: list of k matrices, li(i)=matrix of size varsizes(i,:);
// Copyright INRIA
[LHS,RHS]=argn(0)
if bigVector==[] then
     n=0;for dimi=varsizes',n=n+prod(dimi);end
bigVector=zeros(n,1);
end
li=list();point=1;i=0;
for dimi=varsizes'
 newpoint=point+prod(dimi)-1;i=i+1;
 li(i)=matrix(bigVector(point:newpoint),dimi(1),dimi(2));
 point=newpoint+1;
end
if RHS==3 then li=recons(li,ind); end
endfunction
