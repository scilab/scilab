function [mod,scs_m]=do_label(scs_m)
// do_block - edit a block label
// Copyright INRIA
mod=%f
while %t
  [btn,xc,yc,win,Cmenu]=cosclick()
  if Cmenu<>[] then
    Cmenu=resume(Cmenu)
  end
  K=getblock(scs_m,[xc;yc])
  if K<>[] then break,end
end
o=scs_m.objs(K)
model=o.model
lab=model.label
lab=dialog('Give block label',lab)
if lab<>[] then
  lab=stripblanks(lab)
  if length(lab)==0 then lab=' ',end
  model.label=lab
  o.model=model
  scs_m.objs(K)=o
  mod=%t
end
endfunction
