function p=adj2sp(xadj,adjncy,anz,mn)
//adjacency to sparse conversion.
[LHS ,RHS ]=argn(0);

nb=size(xadj,'*')-1;
nza=size(adjncy,'*');
i = fadj2sp(xadj(:),nb,nza);
nr=max(adjncy);
if RHS == 4  then
p=sparse([i,adjncy(:)],anz,[mn(2),mn(1)])';
else
p=sparse([i,adjncy(:)],anz,[nb,nr])';
end;
endfunction
