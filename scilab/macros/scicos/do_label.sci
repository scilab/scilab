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
o=scs_m(K)
model=o(3)
if size(model)<13 then model(13)=' ',model(14)=list(),end
lab=model(13)
lab=dialog('Give block label',lab)
if lab<>[] then
  lab=stripblanks(lab)
  if length(lab)==0 then lab=' ',end
  model(13)=lab
  o(3)=model
  scs_m(K)=o
  mod=%t
end


