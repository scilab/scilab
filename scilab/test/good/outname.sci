function [out,nwrk,txt]=outname(nwrk,typ,m,n,excluded)
//cette macro regarde si l'operation suivante est une affectation,
// si oui et si cette affectation ne se fait pas sur une des variables listees
//    dans le vecteur de chaine excluded, la macro retourne dans out
//    le nom sur lequel se fait l'affectation
// sinon elle alloue une matrice de taille m*n de type fortran typ dans le
//    tableau de travail correspondant (modifie en consequence nwrk)
//    et retourne
//      -dans out une chaine de la forme 'work(iwn)' ou 'iwork(iiwn)'
//        le n etant defini automatiquement.
//      -dans txt les lignes fortran permettant de definir iwn ou iiwn
//
//!
// Copyright INRIA
txt=[]
[lhs,rhs]=argn(0)
nvar=prod(size(typ))
if rhs<5 then excluded(nvar)=' ',end
for k=1:nvar
  op=lst(ilst+k)
  out(k)=' ';
  if op(1)=='1'&and(op(2)<>excluded(k)) then
    k3=find(op(2)==vnms(:,2))
    if k3==[] then
      // la variable n'existe pas il faut lui allouer de la place
      if isnum(n(k))&isnum(m(k)) then
	//les dimensions sont des nombres l'emplacement sera alloue localement
	out(k)=op(2)
      else
	//dimensions formelles on alloue dans les tableau de travail
	[o,nwrk,t1]=getlocal(nwrk,op(2),typ(k),m(k),n(k))
	txt=[txt;t1]
	out(k)=o;
      end
    else
      //  la variable existe deja 
      out(k)=op(2)
    end
  end
  if out(k)==' ' then
    if isnum(n(k))&isnum(m(k)) then
      //les dimensions sont des nombres l'emplacement sera alloue localement
      //(to be done)
      [o,nwrk,t]=getwrk(nwrk,typ(k),m(k),n(k))
      out(k)=o;txt=[txt;t]
    else
      [o,nwrk,t]=getwrk(nwrk,typ(k),m(k),n(k))
      out(k)=o;txt=[txt;t]
    end
    // mark temporary to be freed at the end of instruction (to be done)
  end
end



  
endfunction
