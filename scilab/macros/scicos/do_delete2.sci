function [scs_m,DEL]=do_delete2(scs_m,K,gr)
//perform deletion of scs_m object whose index are given in the vector 
//K and all connected links. splits which are not given in K are not deleted
//!
// Copyright INRIA
DEL=[] //table of deleted objects
K0=K
while K<>[] do
  k=K(1);K(1)=[]
  o=scs_m(k);
  if find(DEL==k)==[] then typ=o(1);else typ='Deleted',end
  DEL=[DEL k]

  if typ=='Link' then
    [ct,from,to]=o(7:9)
    tokill=[%t,%t]

    scs_m(to(1))=mark_prt(scs_m(to(1)),to(2),'in',ct(2),0)

    scs_m(from(1))=mark_prt(scs_m(from(1)),from(2),'out',ct(2),0)
    
    // erase and delete link
    if gr==%t then drawobj(o),end
  elseif typ=='Block' then
    // get connected links
    connected=get_connected(scs_m,k)
    //ask for connected links deletion
    K=[K connected]
    // erase and delete block
    if gr==%t then drawobj(scs_m(k)),end
  elseif typ=='Text' then
    if gr==%t then drawobj(o),end
  elseif typ=='Deleted' then
  else
    message('This object can''t be deleted')
  end
end
if gr==%t then 
  if pixmap then xset('wshow'),end,
end
for k=DEL,scs_m(k)=list('Deleted'),end


