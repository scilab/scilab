function translatepaths(Paths,res_path)
// Copyright INRIA
if exists('m2scilib')==0 then load('SCI/macros/m2sci/lib'),end
//logfile=%io(2)
 if MSDOS then 
  sep='\'
  Paths=strsubst(Paths,'/',sep)
  res_path=strsubst(res_path,'/',sep)
else
  sep='/'
  Paths=strsubst(Paths,'\',sep)
  res_path=strsubst(res_path,'\',sep)
end

res_path=stripblanks(res_path)
if part(res_path,length(res_path))<>sep then
  res_path=res_path+sep
end
Paths=stripblanks(Paths)
logfile=file('open',res_path+'log','unknown')
whsfil_unit=file('open',res_path+'whatis','unknown')

for k=1:size(Paths,'*')
  if part(Paths(k),length(Paths(k)))<>sep then 
    Paths(k)=Paths(k)+sep,
  end
end
mfiles=[]
for k=1:size(Paths,'*')
  path=Paths(k)
  if MSDOS then 
    mfiles=[mfiles;unix_g('dir /b '+path+'*.m')]
  else
    mfiles=[mfiles;unix_g('ls '+path+'*.m')]
  end
end
for k1=1:size(mfiles,1)
  if MSDOS then
    fnam=part(mfiles(k1),1:length(mfiles(k1))-2)
    mpath=path+mfiles(k1)
  else   
    kk=strindex(mfiles(k1),sep)
    fnam=part(mfiles(k1),kk($)+1:length(mfiles(k1))-2)
    mpath=mfiles(k1)    
  end
  scipath=res_path+fnam+'.sci'
  scepath=res_path+fnam+'.sce'
  if newest(mpath,scipath,scepath)==1 then
    mfile2sci(mpath,res_path,%f,%t)
  end
end
endfunction
