function genlib(nam,path)
// get all .sci files in the specified directory
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<=1 then 
  path='./',
  // try to get the path from the library if it already exists
  if exists(nam)==1 then
    if evstr('type('+nam+')')==14 then
      path=evstr('string('+nam+')');path=path(1)
    end
  end
end
path=stripblanks(path)
if MSDOS then
  if part(path,length(path))<>'\' then
     path=path+'\';
  end
else
  if part(path,length(path))<>'/' then
     path=path+'/';
  end
end

path1=path
if part(path,1:4)=='SCI/' then path=SCI+part(path,4:length(path)),end
if part(path,1:2)=='~/' then path=getenv('HOME')+part(path,2:length(path)),end

if MSDOS then
  if part(path,1:4)=='SCI\' then path=SCI+part(path,4:length(path)),end
  path=strsubst(path,'/','\')
  lst=unix_g('dir /B /OD '""+path+'* ""')
  lst=lst($:-1:1)
else
  lst=unix_g('ls  -t1 '+path+'*.*') 
end
// lookfor .sci files
sci=[];
for k=1:size(lst,'*')
  ke=strindex(lst(k),'.sci')
  if ke<>[] then 
    if ke($)==length(lst(k))-3 then
      sci=[sci;k];
    end
  end
end

names=[];
modified=%f
for k1=1:size(sci,'*')  // loop on .sci files
  k=sci(k1);
  if MSDOS then
    fl=path+lst(k)
    fnam=lst(k)
  else
    fl=lst(k)
    ks=strindex(fl,'/')
    if ks==[] then fnam=fl;else fnam=part(fl,ks($)+1:length(fl));end
  end
  names=[names;strsubst(fnam,'.sci','')] // add file name to file name vector
  bin=find(strsubst(lst(k),'.sci','.bin')==lst)
  if bin==[]| bin>k then
    modified=%t
    getsave(fl) // getf sci file and save functions it defines as a .bin file
  end
end

if modified
  // write 'names' file in directory given by path
  u=file('open',path+'names','unknown');
  write(u,names,'(a)');file('close',u)
  // create library
  execstr(nam+'=lib('''+path1+''')')
  [u,ierr]=mopen(path+'lib','wb')
  // save library in directory given by path
  if ierr<>0 then 
    error(path+'lib file cannot be opened for writing')
  end
  execstr('save(u,'+nam+')');mclose(u)
else
  execstr(nam+'=lib('''+path1+''')')
end
execstr(nam+'=resume('+nam+')')

function getsave(fl)
prot=funcprot();funcprot(0)
nold=size(who('get'),'*')

errcatch(-1,'continue')
getf(fl) // get functions defined in file 'fl'
if iserror(-1) then
  errclear (-1)
  warning('Error in file '+fl+' file ignored')
else
  errcatch(-1)
  // lookfor names of the functions defined in file 'fl'
  new=who('get')
  new=new(1:(size(new,'*')-nold-1))
  // create output file name
  fl=strsubst(fl,'.sci','.bin')
  // save all functions in the output file
  [u,ierr]=mopen(fl,'wb')
  if ierr<>0 then 
    error('Impossible to open file '+fl+' for writing, ')
  end
  if new<>[] then 
    execstr('save(u,'+strcat(new($:-1:1),',')+')'); 
  else 
    write(%io(2),'function ""'+fl+'"" does not contain any functions');
  end
  mclose(u)
end

funcprot(prot)
