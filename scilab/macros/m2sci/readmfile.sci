function txt=readmfile(fil)
fd=mopen(fil,'r')
block=100
t=[]
txt=[]
cr=10
nl=13
//end of line
EOL=ascii(10)
while %t
  t=[t mget(block,'uc',fd)]
  k1=1
  keol=find(t==10|t==13)
  if size(keol,2)>1 then
    if and(t(keol(1:2))==[13,10]) then
      if t($)==13 then
	t($)=[]
	keol($)=[]
	mseek(-1,fd,'cur')
      end
      t(keol(1:2:$))=[]
      keol=keol(1:2:$)-[0 cumsum(ones(1,size(keol,2)/2-1))]
    end
  end
  k2=0
  for k=1:size(keol,2)
    k2=keol(k)
    txt=[txt;ascii(t(k1:k2-1))]
    k1=k2+1
  end
  t([1:k2])=[];
  if meof(fd)<>0 then
    mclearerr(fd)
    mclose(fd)
    break
  end
end
if t<>[] then
  txt=[txt;ascii(t)]
end
