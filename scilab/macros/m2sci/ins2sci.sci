function [txt,vnms,vtps,lcount]=ins2sci(lst,ilst,vnms,vtps)
// traduit un ensemble d'instructions debutant a l'adresse ilst de la
// liste courante lst
//!
// Copyright INRIA
nlst=size(lst)
level;level(2)=0;
txt=[]
while ilst<=nlst then
  if type(lst(ilst))==15 then
    [t1,vnms,vtps]=cla2sci(lst(ilst),vnms,vtps)
    ilst=ilst+1
  else
    if lst(ilst)(1)=='15' then ilst=ilst+1,lcount=lcount+1;end
    [t1,ilst,vnms,vtps,lcount]=cod2sci(lst,ilst,vnms,vtps)
  end
  txt=[txt;t1]
end
endfunction
