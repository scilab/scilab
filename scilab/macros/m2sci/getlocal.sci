function [out,nwrk,txt]=getlocal(nwrk,nam,typ,m,n)
//Cette macro alloue une variable locale de taille m*n de type fortran typ
//et de nom scilab nam dans le
//    tableau de travail correspondant (modifie en consequence nwrk)
//    et retourne
//      -dans out une chaine de la forme 'work(iwnam)' ou 'iwork(iiwnam)'
//        le n etant defini automatiquement.
//      -dans txt les lignes fortran permettant de definir iwnam ou iiwnam
//
//!
// Copyright INRIA
txt=[]
mn=mulf(m,n)
if typ=='1' then
  i=nwrk(1)
  nw=nwrk(11);[m,n]=size(nw)
  free=find(nw(2,:)=='0')
  k=find(nw(1,free)==mn)
  if k==[] then
    nwrk(10)=[nwrk(10);nam]
    nwrk(11)=[nwrk(11) [mn;'1']]
  else
    i=free(k(1))
    nw(2,i)='1';nwrk(11)=nw
  end
  out='work(iw'+nam+')'
else
  i=nwrk(12)
  nw=nwrk(13);[m,n]=size(nw)
  free=find(nw(2,:)=='0')
  k=find(nw(1,free)==mn)
  if k==[] then
    nwrk(12)=[nwrk(12);nam]
    nwrk(13)=[nwrk(13),[mn;'1']];
  else
    i=free(k(1))
    nw(2,i)='1';nwrk(13)=nw
  end
  out='iwork(iiw'+nam+')'
end
endfunction
