function [out,nwrk,txt]=getwrk(nwrk,typ,m,n)
//Cette macro alloue une matrice de taille m*n de type fortran typ dans le
//    tableau de travail correspondant (modifie en consequence nwrk)
//    et retourne
//      -dans out une chaine de la forme 'work(iwn)' ou 'iwork(iiwn)'
//        le n etant defini automatiquement.
//      -dans txt les lignes fortran permettant de definir iwn ou iiwn
//
//!
// Copyright INRIA
mn=mulf(m,n)
if typ=='1' then
  i=nwrk(1)
  nw2=nwrk(2);[m2,n2]=size(nw2)
  if n2==0 then i=0,end
  if n2>0 then
    free=find(nw2(2,:)=='0')
    k=find(nw2(1,free)==mn)
    if k==[] then
       nwrk(1)=nwrk(1)+1
       nwrk(2)=[nwrk(2) [mn;'1']]
       txt=[' iw'+string(i+1)+' = '+addf('iw'+string(i),mn)];
    else
       i=free(k(1))
       nw2(2,i)='1';nwrk(2)=nw2
    end
  else
    nwrk(1)=nwrk(1)+1
    nwrk(2)=[nwrk(2) [mn;'1']]
    txt=[' iw'+string(i+1)+' = '+addf('iw'+string(i),mn)];
  end
  out='work(iw'+string(i)+')'
else
  i=nwrk(4)
  nw5=nwrk(5);[m5,n5]=size(nw5)
  if n5==0 then i=0,end
  if n5>0 then
    free=find(nw5(2,:)=='0')
    k=find(nw5(1,free)==mn)
    if k==[] then
      nwrk(4)=nwrk(4)+1
      nwrk(5)=[nwrk(5),[mn;'1']];
      txt=[' iiw'+string(i+1)+' = '+addf('iiw'+string(i),mn)];
    else
      i=free(k(1))
      nw5(2,i)='1';nwrk(5)=nw5
    end
  else
    nwrk(4)=nwrk(4)+1
    nwrk(5)=[nwrk(5),[mn;'1']]
    txt=[' iiw'+string(i+1)+' = '+addf('iiw'+string(i),mn)];
  end
  out='iwork(iiw'+string(i)+')'
end
endfunction
