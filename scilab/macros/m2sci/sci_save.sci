function [stk,txt,top]=sci_save()
// Copyright INRIA
txt=[]
if rhs<=0 then
  stk=list('save(''scilab.save'')','0','0','0','0')
  return
end
args=[]
for k=1:rhs
  args=[args,stk(top-rhs+k)(1)]
end
kopt= find(part(args,1:2)=='''-') 
opt=args(kopt)
args(kopt)=[]
n=size(args)-1

if opt<>[] then
  w=gettempvar(1)
  if or(opt=='-double') then 
    prec=22
  else
    prec=14
  end
  txt=['frmt=format();format(''e'','+string(prec)+');'
      '%u=file(''open'','+args(1)+',''unknown'')']
  for k=2:size(args,2)
    var=part(args(k),2:length(args(k))-1)
    txt=[txt;'write(%u,string('+var+'),''(a)'')']
  end
  txt=[txt;'file(''close'',%u);format(''e'',frmt(2))']
  stk=list(' ','-1','0','0','0')
else
  stk=list('save'+rhsargs(args),'0','0','0','0')
end



