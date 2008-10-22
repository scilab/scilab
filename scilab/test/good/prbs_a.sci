function [u]=prbs_a(n,nc,ids)
//generation of pseudo random binary sequences
//u=[u0,u1,...,u_(n-1)];
//u takes values in {-1,1} and changes at most nc times its sign.
//ids can be used to fix the date at which u must change its sign 
//ids is then an integer vector with values in [1:n].
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs <=2,
  rand('uniform');
  yy= int(mini(maxi(n*rand(1,nc),1*ones(1,nc)),n*ones(1,nc)));
  ids=sort(yy);ids=[n,ids,1];
else
  [n1,n2]=size(ids);
  ids=[n,mini(n*ones(ids),maxi(sort(ids),1*ones(ids))),1];
end
u=0*ones(1,n);
[n1,n2]=size(ids);
val=1;
for i=1:n2-1,
        if ids(i)<>ids(i+1);
        u(ids(i+1):ids(i))=val*ones(ids(i+1):ids(i));val=-1*val;
       end
end
endfunction
