function lst=lstfiles(path,opt)
// Copyright INRIA
// lstfiles - retourne les noms des fichiers correspondants a un filtre
//%Syntaxe
//  lst=lstfiles(path [,opt])
//%Parametres
// path   : chaine de caractere donnant la regle de selection des fichiers
//          par defaut on recherche tous les fichier du repertoire courant
// lst    : vecteur de chaines de caractere donnant les path-name des
//          fichiers
// opt    : chaine de caracteres pouvant valoir 'file' (defaut),'dir','all'
//          permettant de selectionner uniquement les fichiers de type
//          correspondant.

[lhs,rhs]=argn(0)
if rhs<1 then path=' ',end
if rhs<=1 then opt='file',end
[np,mp]=size(path);np=np*mp;path=matrix(path,np,1);
lst=[]
for k=1:np
  if MSDOS then 
    cmd='dir /b'+path(k);
    tmp = strsubst(TMPDIR,'/','\')+'\unix.out';
    host(cmd+'> '+ tmp);
    lst1=read(tmp,-1,1,'(a)');
    host('del '+tmp);
    for k1=1:size(lst1,'*')
      lst1(k1)=path(k)+lst1(k1)
    end
    lst=[lst;lst1]
  else
    cmd='ls -F -c '+path(k)
    tmp=TMPDIR+'/unix.out';
    host(cmd+'>'+tmp+' 2>'+TMPDIR+'/unix.err')
    lst=[lst;read(tmp,-1,1,'(a)')]
    host('rm -f '+tmp);
  end
end
if prod(size(lst))==0 then lst=[],end
n=prod(size(lst))
if n==0 then return,end
isdir=[]
for k=1:n
  last=part(lst(k),length(lst(k)))
  if last=='/' then
    isdir(k)=%t
    lst(k)=part(lst(k),1:length(lst(k))-1)
  else
    if last=='*'|last=='@' then lst(k)=part(lst(k),1:length(lst(k))-1),end
    isdir(k)=%f
  end
end
if opt=='file' then
  lst=lst(find(~isdir))
elseif opt=='dir' then
  lst=lst(find(isdir))
end
endfunction
