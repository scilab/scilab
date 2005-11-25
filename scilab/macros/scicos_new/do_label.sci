function [mod,scs_m]=do_label(%pt,scs_m)
// do_block - edit a block label
// Copyright INRIA
  mod=%f
  win=%win;
  if Select==[] then
    xc=%pt(1);yc=%pt(2);%pt=[]
    K=getblock(scs_m,[xc;yc])
    if K==[] then return,end
  else
    K=Select(:,1)';%pt=[]
  end
  if size(K,'*')>1 then
    message("Only one block can be selected for this operation.")
  end
    
  
  o=scs_m.objs(K)
  model=o.model
  lab=model.label
  [ok,lab]=getvalue('Give block label','label',list('str',1),lab)
  if ok then
    drawblock(o);
    lab=stripblanks(lab)
    if length(lab)==0 then lab=' ',end
    model.label=lab
    o.model=model
    scs_m.objs(K)=o
    mod=%t
    drawblock(o);
  end
endfunction
