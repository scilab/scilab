function %cblock_p(m)
n=size(definedfields(m),'*')
txt=[]
for k=2:n
  txt=[txt string(getfield(k,m))]
end
l=max(length(txt),'r')
n=size(txt,2)
for k=1:n
  txt(:,k)=part(txt(:,k),1:l(k))
end

nlc=lines()
k1=1
while %t
  L=cumsum(l(k1:$)+2)
  k2=max(find(L<nlc(1)))
  if k2==[] then k2=k1  ;end
  t=emptystr(size(txt,1),1)
  for i=k1:k2
    t=t+txt(:,i)+'  '
  end
  if k1==1&k2==n then disp(t),break,end

  if k1==k2 then
    disp(['Column '+string(k1);t])
  else
    disp(['Columns '+string(k1)+' to '+string(k2);t])
  end
  if k2==n then break,end
end
endfunction
