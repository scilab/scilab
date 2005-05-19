function h=%grayplot_i_h(i,v,h)
if type(i)==10 then
  set(h,i,v)
elseif type(i)==15 then
  hdl=h
  p=i($)
  if type(p)==1|type(p)==4|type(p)==2|type(p)==129|type(p)==15 then
    index=i($)
    i($)=null()
  else
    index=:
  end
  n=lstsize(i)
  for k=1:n-1
    p=i(k)
    if type(p)==10 then
      hdl=get(hdl,p),
    elseif type(p)==1|type(p)==4|type(p)==2|type(p)==129 then
      hdl=hdl(p)
    elseif type(p)==15 then
      hdl=hdl(p(:))
    else
      error('Invalid path')
    end
  end
  if type(index)==15 then
    hdl=hdl(index(:))
  else
    hdl=hdl(index)
  end
  set(hdl,i($),v)
else
  error('Invalid path')
end
endfunction