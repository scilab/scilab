function [Path,path,ok]=whereintree(Tree,xc,yc)
// Copyright INRIA
[m,k]=mini((Tree('x')-xc).^2+(Tree('y')-yc).^2)
ok=%t;path=[];Path=list()
if k==1 then return,end

O=Tree('orig')
path=Tree('paths')(k-1);
if size(O,'*')>size(super_path,'*') then
  message(['Navigator tree doesnt correspond to this'
      'diagram'])
  ok=%f
  return
end
n=size(O,'*')
if or(O<>super_path(1:n)) then
  message(['Navigator tree doesnt correspond to this'
      'diagram'])
  ok=%f
  return
end

sp=super_path;sp(1:n)=[]
if size(path,'*')<size(sp,'*') then
  message(['Upper level or New branch';
      'close current edition window first'])
  ok=%f
  return
end
n=size(sp,'*')
if or(sp<>path(1:n)) then
  message(['New branch, close current edition window first'])
  ok=%f
  return
end
path(1:n)=[]



Path=list();
for pk=path
  Path=lstcat(Path,'objs',pk,'model','rpar');
end
endfunction
