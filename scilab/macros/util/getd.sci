function getd(path)
// get all .sci files in the specified directory
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<=0 then path='./',end
path=stripblanks(path)
if part(path,length(path))<>'/' then path=path+'/',end
if part(path,1:4)=='SCI/' then path=SCI+part(path,4:length(path)),end
if part(path,1:2)=='~/' then path=getenv('HOME')+part(path,2:length(path)),end
if MSDOS then
  lst=unix_g('dir /b '+strsubst(path,'/','\')+'*.sci')
else
  lst=unix_g('ls '+path+'*.sci')
end

nold=size(who('get'),'*')
prot=funcprot();funcprot(0)

for k=1:size(lst,'*')
  if MSDOS then
    getf(path+lst(k))
  else
    getf(lst(k))
  end
end
funcprot(prot)
new=who('get')
new=new(1:(size(new,'*')-nold-2))
execstr('['+strcat(new,',')+']=resume('+strcat(new,',')+')')
