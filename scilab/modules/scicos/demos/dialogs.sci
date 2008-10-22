function rep=x_message(comment,btns)
// Copyright INRIA
  rep=[]
  [lhs,rhs]=argn(0)
  comment=matrix(comment,prod(size(comment)),1)
  if rhs==1 then
    write(%IO(2),[comment;' ';'Ok ?';' '])
  else
    if size(btns,'*')==1
      write(%IO(2),[comment;' ';'Ok ?';' '])
    else
      write(%IO(2),[comment;' ';btns(:);' '])
      str=readline()	
      rep=find(str==btns)
    end
  end
endfunction

function str=x_dialog(comment,default)
// Copyright INRIA
  [lhs,rhs]=argn(0)
  if rhs==1 then default=' ',end
  comment=matrix(comment,prod(size(comment)),1)
  default=default(1) // for lmitool
  write(%IO(2),[comment;'reponse par defaut :'+default;' '])
  str=[]
  while %t do
    s=readline()	
    if part(s,1)=='o'|part(s,1)=='c' then break,end
    str=[str;s]
  end
  if str==' ' then str=default,end
  write(%IO(2),'o[k]/c[ancel]')
  rep=s
  if part(rep,1)=='c' then str=[],end
endfunction


function str=x_mdialog(description,labels,valuesini)
// Copyright INRIA
  [lhs,rhs]=argn(0)
  n=prod(size(labels))
  if rhs==2 then valuesini(n)=' ',end

  write(%IO(2),description)
  n=prod(size(labels))
  //  disp('ici');pause
  for k=1:n
    write(%IO(2), labels(k) +' valeur par defaut :'+valuesini(k))
    str(k,1)=readline()
    if str(k)==' ' then str(k)=valuesini(k),end
  end
  write(%IO(2),'o[k]/c[ancel]')
  rep=readline()
  if part(rep,1)=='c' then str=[],end
endfunction


function num=tk_choose(tochoose,comment,button)
// Copyright INRIA
  [lhs,rhs]=argn(0)
  n=prod(size(tochoose))
  tochoose=matrix(tochoose,n,1)
  comment=matrix(comment,prod(size(comment)),1)
  if rhs==2 then button='Annuler',end

  sep=[],
  for l=0:n,sep=[sep;':'],end
  lmx=maxi([length(tochoose);length(comment)])
  
  tochoose=[string(0:n)', [button;tochoose]]

  write(%IO(2),[comment;' ';mat2tab(tochoose)])
  num=-1
  while num<0|num>n  then
    write(%IO(2),'Donnez le numero de votre choix')
    num=evstr(readline())
  end
endfunction



function tab=mat2tab(str,del)
// Copyright INRIA
//Etant   donne une  matrice  de chaine  de  caracteres str, mat2tab(str)
//retourne un vecteur colonne de chaines de caracteres  representant les
//elements de str repartis sous forme d'un tableau.
//mat2tab(str,del) (ou del est un vecteur de chaines a un ou deux elements) 
//  retourne le meme tableau ou les colonnes sont separees par la chaine de
//   caracteres contenue dans del(1) et lignes par del(2) s'il exite
//%Exemple
//  tt=['0','a';'1','b';'2','c']
//  write(%io(2),mat2tab(tt))
//  write(%io(2),mat2tab(tt,'|'))
//  write(%io(2),mat2tab(tt,['|','-']))
//!
//origine S Steer 1991
  [lhs,rhs]=argn(0)
  job=0
  if rhs==1 then 
    delc=' ',
  else
    delc=del(1)
    if prod(size(del))==2 then
      dell=del(2)
      dell=part(dell,1)
      job=1
    end
  end
  blk='                              ';blk=blk+blk+blk

  [m,n]=size(str);len=length(str)
  lmx=[];for col=len,lmx=[lmx,maxi(col)+1],end

  ln=sum(lmx)+(n+1)*length(delc)
  if job==1 then
    rd=delc
    for l=2:ln-1,rd=rd+dell,end
    rd=rd+delc
    tab=rd
  else
    tab=[]
  end
  for l=1:m
    ll=delc+str(l,1)
    for k=2:n

      ll=ll+part(blk,1:lmx(k-1)-len(l,k-1))+delc+str(l,k)
    end
    ll=ll+part(blk,1:lmx(n)-len(l,n))+delc
    tab=[tab;ll]
    if job==1 then tab=[tab;rd],end
  end 
endfunction


function [btn,xc,yc,win,Cmenu]=xclick();
// Copyright INRIA  
  str=readline()
  rep=evstr('list('+str+')')
  btn=rep(1)
  xc=rep(2)
  yc=rep(3)
  mrk=xget('mark')
  xset('mark',0,5)
  plot2d(xc,yc,-1,'000')
  plot2d(xc,yc,-1,'000')
  xset('mark',mrk(1),mrk(2))
  win=0
  if size(rep)>=4 then 
    win=rep(4),
  end
  if size(rep)>=5 then 
    Cmenu=rep(5)
  else
    Cmenu=[]
  end
endfunction

function rep=xgetmouse(flag);
// Copyright INRIA 
  str=readline()
  rep=evstr('['+str+']')
endfunction


function str=readline()
// Copyright INRIA 
  comm='/'+'/'
  blank=' '
  while %t do
    rep=read(%IO(1),1,1,'(a)')
    k=strindex(rep,comm)
    if k<>[] then
      k=k(1)
      if part(rep,1:k)<>part(blank,1:k) then 
	str=stripblanks(part(rep,1:k-1))
	com=part(rep,k+1:length(rep))
	if part(str,1:3)=='-->' then
	  execstr(part(str,4:length(str)))
	else
	  break
	end
      end
    else
      n=length(rep)
      str=stripblanks(rep)
      com=emptystr()
      if part(str,1:3)=='-->' then
	execstr(part(str,4:length(str)))
      else
	break
      end
    end
  end
  bl='=';txt=com+':'+str;txt=part(bl,ones(1,60-length(txt)))+txt
endfunction


function c=getcolor(title,cini)
// Copyright INRIA  
  str=readline()
  c=evstr('['+str+']')
endfunction

function color=tk_getcolor(title,color)
// Copyright INRIA  
  str=readline()
  c=evstr('['+str+']')
endfunction

function num=tk_choose(varargin)
  num=tk_choose(varargin(:))
endfunction

function varargout=tk_getvalue(varargin)
   varargout=getvalue(varargin(:))
endfunction


