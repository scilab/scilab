function [sl,name]=bloc2ss(syst)
//
// Copyright INRIA
[lhs,rhs]=argn(0)
 
if type(syst)<>15 then
    error('input must be a list for block-diagram')
end;
syst1=syst(1);
if syst1(1)<>'blocd' then
    error('input must be a list for block-diagram')
end;

nsyst=size(syst)
dom=[]
for l=2:nsyst
 sys=syst(l)
 typ=part(sys(1),1)
 if typ<>'l' then
   if typ=='b' then 
     //recursion pour traiter les description bloc imbriquees
     sys=list('transfert',bloc2ss(sys))
     syst(l)=sys
   end
   transfert=sys(2)
   if type(transfert)==10 then transfert=evstr(transfert);end
   tr1=transfert(1);
   if tr1(1)=='r' then transfert=tf2ss(transfert);end
   syst(l)=list('transfert',transfert)
   
   if type(transfert)==16 then
      d=transfert(7)
      if d<>[] then
        if dom==[] then
          dom=d
        elseif dom<>d then
          if dom=='c'  then
             error('Hybrid system not implemented')
          elseif dom=='d'&type(d)==1 then
             dom=d
          elseif type(dom)==1&type(d)==1 then
             error('Multi rate discrete system  not implemented')
          end
        end
      end 
    end
 end;
end; 
[lboites,lliens,lentrees,lsorties]=blocdext(syst)
nio=[prod(size(lentrees)),prod(size(lsorties))]
//
lliens=[lliens lentrees lsorties]
if lhs==2 then
  in_names=[]
  for kvar=lentrees
    obj=syst(kvar)
    in_names=[in_names;obj(2)]
  end
  out_names=[]
  for kvar=lsorties
    obj=syst(kvar)
    out_names=[out_names;obj(2)]
  end
  name=list(in_names,out_names)
end
//on ecrit le systeme bloc diagonal constitue de l'ensemble des sous systemes

a=[];ms=0;b=[];mi=0;c=[];mo=0;d=[];lgain=[]
insize=1;outsize=1;inn=1;outn=1;
in=[];out=[];
corresp=[];l=0
for numero=lboites
    l=l+1
    corresp(numero)=l
    bloc=syst(numero);transfert=bloc(2)
    if type(transfert)==1 then  //c'est un gain
      [mok,mik]=size(transfert)
      [msk,msk]=size([])
      if ms>0 then
        b(ms,mi+1:mi+mik)=0*ones(1,mik)
        c(mo+1:mo+mok,ms)=0*ones(mok,1)
      end
      d(mo+1:mo+mok,mi+1:mi+mik)=transfert
      mo=mo+mok;mi=mi+mik;ms=ms+msk
      insize=[insize insize(inn)+mik];inn=inn+1
      outsize=[outsize outsize(outn)+mok];outn=outn+1
    else  // un bloc dynamique
      [mok,mik]=size(transfert(5))
      [msk,msk]=size(transfert(2))
      a(ms+1:ms+msk,ms+1:ms+msk)=transfert(2)
      b(ms+1:ms+msk,mi+1:mi+mik)=transfert(3)
      c(mo+1:mo+mok,ms+1:ms+msk)=transfert(4)
      d(mo+1:mo+mok,mi+1:mi+mik)=transfert(5)
      mo=mo+mok;mi=mi+mik;ms=ms+msk
      insize=[insize insize(inn)+mik];inn=inn+1
      outsize=[outsize outsize(outn)+mok];outn=outn+1
    end
end
//on ecrit la matrice de feedback K
k=0*ones(insize(inn)-1,outsize(outn)-1)
for numero=lliens

  fil=syst(numero)

  nnum=size(fil)-3
  if nnum<=0 then 
     error('incorrect link:'+string(numero))
  end
  debut=fil(3)
  bdebut=debut(1)  //numero de la boite origine du lien ou de l'entree
  pdebut=debut(2:prod(size(debut))) // ports origines
  if pdebut==[] then // dimension implicite
     fin=fil(4);pfin=fin(2:prod(size(fin)))
     if pfin<>[] then 
      //meme dimension que le port d'entree du premier bloc connecte
       pdebut=1:prod(size(pfin))
     else
       if fin(1)>0 then // dimension d'entree du premier bloc aval
         bfin=corresp(fin(1))
         pdebut=1:(insize(bfin+1)-insize(bfin))
       else // dimension de sortie du  bloc amont
         bd=corresp(bdebut)
         pdebut=1:(outsize(bd+1)-outsize(bd))
       end
     end
  end
  if bdebut>0 then, //le lien est issu d'un bloc
   bdebut=corresp(bdebut)
   ll0=1
  else // le lien  correspond a une entree
    if nnum>1 then, //l'entree attaque plusieurs blocs,on rajoute un gain 
      np=prod(size(pdebut))
      d(mo+np,mi+np)=eye(np,np)
      if ms>0 then b(ms,mi+np)=0;c(mo+np,ms)=0;end
      mo=mo+np
      mi=mi+np
      in=[in mi]
      insize=[insize insize(inn)+np];inn=inn+1
      outsize=[outsize outsize(outn)+np];outn=outn+1
      bdebut=outn-1
      k(mi,mo)=0
    else
      fin=fil(4)
      bfin=corresp(fin(1))
      fin=fin(2:prod(size(fin)))
      if fin==[] then //dimension implicite
        fin=1:(insize(bfin+1)-insize(bfin))
      end
      in=[in, (insize(bfin)-1)*ones(fin)+fin]
      ll0=2
    end
  end
  for ll=ll0:nnum
    fin=fil(ll+3)
    bfin=fin(1)
    if bfin >0 then
      bfin=corresp(bfin)
      pfin=fin(2:prod(size(fin)))
      if pfin==[] then //dimension implicite
        pfin=1:(insize(bfin+1)-insize(bfin))
      end
      iin=(insize(bfin)-1)*ones(pfin)+pfin
      iou=(outsize(bdebut)-1)*ones(pdebut)+pdebut
      k(iin,iou)=eye(prod(size(iin)),prod(size(iou)))
    else // le lien fournit une  sortie
      pfin=fin(2:prod(size(fin)))
      if pfin==[] then //dimension implicite
        pfin=1:prod(size(pdebut))
      end
      if prod(size(pfin))<>prod(size(pdebut)) then
        error('le fil issu du bloc '+string(bdebut)+..
               ' est de dimension  incorrecte')
      end
      out=[out, (outsize(bdebut)-1)*ones(pfin)+pdebut(pfin)]
    end
  end
end
if or(size(d')<>size(k)) then
  error('invalid data')
end
sl=syslin([],a,b,c,d)/.(-k)
sl=sl(out,in)
sl(7)=dom

endfunction
function [lboites,lliens,lentrees,lsorties]=blocdext(syst)
//!
//
lboites=[]
lliens=[]
lentrees=[]
lsorties=[]
nsyst=size(syst)
for k=2:nsyst
  obj=syst(k)
  if type(obj)==15, if size(obj)>1 then
  select part(obj(1),1)
     case 't',
         lboites=[lboites,k]
     case 'l'
         obj2=obj(3)

         if obj2(1)>0 then
            nobj=size(obj)
            is_sortie=[]
            for ko=3:nobj
               objk=obj(ko)
               if objk(1)<0 then is_sortie=[is_sortie,-objk(1)],end
            end;
            if is_sortie==[] then lliens=[lliens,k],
                            else lsorties(1,is_sortie)=k
            end;
                      else lentrees(1,-obj2(1))=k,
         end;
     else error('undefined type')
  end;
  end,end
end;
if lsorties==[] then error('no output'),end
if lentrees==[] then error('no input'),end
if mini(lsorties)==0 then error('undefined input'),end
if mini(lentrees)==0 then error('undefined output'),end
endfunction
