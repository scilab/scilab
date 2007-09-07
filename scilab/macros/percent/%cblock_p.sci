function %cblock_p(m)
  n=size(definedfields(m),'*')
  txt=[];
  for k=2:n
    txt=[txt string(getfield(k,m))];
  end
  l=max(length(txt),'r');
  n=size(txt,2);
  for k=1:n
    txt(:,k)=part(txt(:,k),1:l(k));
  end

  nlc=lines()
  k1=1
  while %t
    L=cumsum(l(k1:$)+2)
    k2=max(find(L<nlc(1)-3))
    if k2==[] then k2=k1  ;else k2=k1-1+k2;end
    t=emptystr(size(txt,1),1);
    for i=k1:k2
      t=t+txt(:,i)+'  ';
    end
    if k1==1&k2==n then  mprintf('%s\n','  '+t),break,end

    if k1==k2 then
      mprintf('%s\n',['Column '+string(k1);' ';'  '+t])
    else
      mprintf('%s\n',['Columns '+string(k1)+' to '+string(k2);' ';+'  '+t;' '])
    end
    if k2==n then break,end
    k1=k2+1
  end
endfunction

