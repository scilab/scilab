function [txt,vnms,vtps,nwrk]=ins2for(lst,ilst,vnms,vtps,nwrk)
// traduit un ensemble d'instructions debutant a l'adresse ilst de la
// liste courante lst
//!
// Copyright INRIA
nlst=size(lst)
txt=[]
while ilst<=nlst then
  if type(lst(ilst))==15 then
    [t1,vnms,vtps,nwrk]=cla2for(lst(ilst),vnms,vtps,nwrk)
    ilst=ilst+1
  else
    [t1,ilst,vnms,vtps,nwrk]=cod2for(lst,ilst,vnms,vtps,nwrk) 
  end
  txt=[txt;t1]
end
endfunction
