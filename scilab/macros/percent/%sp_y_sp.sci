function r=%sp_y_sp(a,b)
//  a./.b with a and b sparse
// Copyright INRIA
[ija,va,mna]=spget(a)
[ijb,vb,mnb]=spget(b)
if size(ijb,1)<>prod(mnb)|or(vb==0) then error(27),end
ia=ija(:,1);ja=ija(:,2)
ib=ijb(:,1);jb=ijb(:,2)

ij=[((ia-ones(ia))*mnb(1)).*.ones(ib)+ones(ia).*.ib,..
    ((ja-ones(ja))*mnb(2)).*.ones(jb)+ones(ia).*.jb]
r=sparse(ij,va./.vb,mna.*mnb)
endfunction
