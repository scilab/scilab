function h=%c_i_h(i,v,h)
 hsave=h

//pause
  if v==[] then
    error('Field property cannot be []')
  end
  if type(i)==10 then set(h,i,v),return,end

  if type(i)<>15 then  error('Invalid path'),end

  if and(type(i($))<>[1 4 2 129 15]) then
    i($+1)=: 
  end
  n=lstsize(i)

//    pause

  hdl=h;hind=[]
  for k=1:lstsize(i)// walk down in the handle tree
    p=i(k)
//    disp("inside FOR!")
//    pause
    lasthandle=hdl,
    if type(p)==10 then
      hdl=get(hdl,p),
    elseif or(type(p)==[1 4 2 129]) then
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

//  disp("out for")
// pause
  if hind<>[] then // a property found
    if type(index)==15 & and(type(property)<>[15 16 17]) then
//      disp('DANS if hind<>[] then // a property found')
//    pause
      property(index(:))=v
    else
      if index<>: | hind=="auto_ticks" | hind=="axes_visible" | hind=="axes_reverse" then
//      disp('inside IF index<>: 1')
//      pause
	property(index)=v
      else
//      disp('inside IF index==: 2')
//      pause
	property=v
      end
    end

//    disp('apres  if hind<>[] then // a property found')
//    pause
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




