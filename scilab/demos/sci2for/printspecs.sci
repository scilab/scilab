function []=printspecs(mac,type_desc)
//displays description of a macro to translate into fortran
//!

// Copyright INRIA
blk='                                 ';
typs=['entier','double precision','?','?']
n=size(type_desc)
[lhs,rhs]=argn(0)

[out,in,cod]=string(mac)
txt=['['+makeargs(out)+']='+nam+'('+makeargs(in)+')';cod]

desc='| variable number | fortran type    |# rows  |# columns  |'
dd=[20,17,11,13]
for k=1:n,
  tk=type_desc(k)
  num=string(k);num=num+part(blk,1:dd(1)-length(num))
  ftyp=typs(evstr(tk(1))+1);ftyp=ftyp+part(blk,1:dd(2)-length(ftyp))
  nl=tk(2);nl=nl+part(blk,1:dd(3)-length(nl))
  nc=tk(3);nc=nc+part(blk,1:dd(4)-length(nc))
  desc=[desc;'|'+num+'|'+ftyp+'|'+nl+'|'+nc+'|']
end

x_message(['       source code of Scilab function';..
         '-----------------------------';' ';txt;..
         ' ';' ';..
         'type and dimension of input parameters';..
         '---------------------------------------';..
         ' ';desc])

write(%io(2),[' ';'Now translation is made...';' '])

function [txt]=gendesc(mac,type_desc,fnam)
// For intersci...
//!
[out,in]=string(mac)
n=size(type_desc)
first=fnam;
ret='sequence '
for l=1:prod(size(out)), ret=ret+' '+out(l);end
txt=[]
for k=1:n
  tk=type_desc(k)
  first=first+' '+in(k)
  desck=in(k)
  if tk(2:3)== ['1','1'] then 
    desck=desck+'  scalar'
  elseif tk(2)=='1' then
    desck=desck+'  column '+tk(3)
  elseif tk(3)=='1' then
    desck=desck+'  row '+tk(2)
  else
    desck=desck+'  matrix '+tk(2)+' '+tk(3)
  end
  txt=[txt;desck]
end
txt=[first;txt;' ';'fortran args']
for k=1:n
  tk=type_desc(k)
  first=first+' '+in(k)
  desck=in(k)
  if tk(1)== '0' then 
    desck=desck+'  int'
  elseif tk(2)=='1' then
    desck=desck+'  double'
  end
  txt=[txt;desck]
end
