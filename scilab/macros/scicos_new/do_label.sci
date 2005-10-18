function [mod,scs_m]=do_label(%pt,scs_m)
// do_block - edit a block label
// Copyright INRIA
  mod=%f
  win=%win;
  xc=%pt(1);yc=%pt(2);
  K=getblock(scs_m,[xc;yc])
  if K==[] then return,end
  
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
