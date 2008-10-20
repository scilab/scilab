function h=generic_i_h(i,v,h)
  hsave=h
//  if v==[] then error('Field property cannot be []'),end
  if type(i)==10 then set(h,i,v),return,end
  if type(i)<>15 then  error('Invalid path'),end

  if and(type(i($))<>[1 2 4 8 129 15]) then
    i($+1)=: 
  end
  n=lstsize(i)
  hdl=h;hind=[]
  for k=1:lstsize(i)// walk down in the handle tree
    p=i(k)
    lasthandle=hdl,
    if type(p)==10 then
      hdl=get(hdl,p),
    elseif or(type(p)==[1 2 4 8 129]) then
      hdl=hdl(p)
    elseif type(p)==15 then
      hdl=hdl(p(:))
    else
      error('Invalid path')
    end
    
    if type(hdl)<>9 then //a leaf found
      property=hdl
      hdl=lasthandle
      hind=p
      if (k+1)==size(i) then
	index=i($)
      else
	index=list(i(k+1:$))
      end
      break
    end
  end
  if hind<>[] then // a property found
    if type(index)==15 & and(type(property)<>[15 16 17]) then
      property(index(:))=v
    else
      if or(size(index)<>[-1 -1]) then
	property(index)=v
      else
	property=v
      end
    end
    if size(hdl,'*')==1 then //a single handle
      hdl(hind)=property
    else //mutiple handle 
      np=size(hdl,'*')
      for k=1:np
	h=hdl(k);h(hind)=property
      end
      
    end
  else
    error('Invalid path')
  end
 h= hsave
endfunction
