function xnumb(x,y,nums,box,angle) 
  n=size(x,'*');
  if argn(2)<5 then angle=zeros(1,n),end
  if argn(2)<4 then box=0,end

  if size(y,'*') >n|size(nums,'*' )<>n|size(angle,'*' )<>n then 
    error('xnumb : the first three arguments should have the same number'+.. 
	  " of entries")
  end
  
  for k=1:n
    xstring(x(k),y(k),msprintf("%-5.2g",nums(k)),angle(k),box)
  end
endfunction
