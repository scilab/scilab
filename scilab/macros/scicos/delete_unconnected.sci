function scs_m=delete_unconnected(scs_m);
//delete unconnected blocks and all relevant parts of a diagram
//may be used before compilation
//scs_m_s=scs_m

// Copyright INRIA
n=size(scs_m)
DEL=[]
DELL=[]
for k=2:n  //loop on scs_m objects
  if scs_m(k)(1)=='Block' then
    if scs_m(k)(5)<>'SUM_f'&scs_m(k)(5)<>'SOM_f' then
    if find(scs_m(k)(2)(5)==0)<>[] then
      // at least one  input port is not connected
      // delete the block
      [scs_m,DEL1,DELL1]=do_delete1(scs_m,k,%f)
      DEL=[DEL DEL1]
      DELL=[DELL DELL1]
    end
    end
  end
end 

//suppress rigth-most deleted elements
while scs_m($)==list('Deleted') then
  scs_m($)=null();
end

// Notify by hiliting and message edition

if DEL<>[] then 
  wins=xget('window')
  if path<>[] then
    mxwin=maxi(winsid())
    for k=1:size(path,'*')
      hilite_obj(scs_m_s(path(k)))
      scs_m_s=scs_m_s(path(k))(3)(8);
      scs_show(scs_m_s,mxwin+k)
    end
  end
  for k=DEL
    if find(k==DELL)==[] then hilite_obj(scs_m_s(k)),end
  end
  message(['Hilited blocks or links are ignored because of'
      'undefined input(s)'])
  for k=DEL
    if find(k==DELL)==[] then unhilite_obj(scs_m_s(k)),end
  end
  for k=size(path,'*'):-1:1,xdel(mxwin+k),end
  scs_m_s=null()
  xset('window',wins)
  if path<>[] then unhilite_obj(scs_m_s(path(1))),end
end
