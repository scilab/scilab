function [Y,I]=mtlb_sort(x,dim,txt)
// Copyright INRIA
// Emulation function for sort() Matlab function
// V.C.

[lhs,rhs]=argn(0);

if rhs==1 then
  if lhs==1 then
    if type(x)==10 then
      x=mstr2sci(x)
      if find(size(x)==1)==[] then
	tmp=gsort(x,"r","i");
      else
	tmp=gsort(x,"g","i");
      end
      Y=[]
      for k=1:size(tmp,1)
	Y=[Y;strcat(tmp(k,:))];
      end
    elseif or(type(x)==[4,6]) then
      if find(size(x)==1)==[] then
	Y=gsort(bool2s(x),"r","i");
      else
	Y=gsort(bool2s(x),"g","i");
      end      
    else
      if find(size(x)==1)==[] then
	Y=gsort(x,"r","i");
      else
	Y=gsort(x,"g","i");
      end      
    end
  // lhs==2
  else
    if type(x)==10 then
      x=mstr2sci(x)
      if find(size(x)==1)==[] then
	[tmp,I]=gsort(x,"r","i");
       else
	[tmp,I]=gsort(x,"g","i");
      end

       // Compute Y
      Y=[]
      for k=1:size(tmp,1)
	Y=[Y;strcat(tmp(k,:))];
      end
    
       // Reorder I
      for k=1:size(Y,1)
	m=1
	tmp2=[]
	tmpI=I(k,:)
	while m<=length(Y(k))
	  if part(Y(k),m)==part(Y(k),m+1) then
	    if tmp2==[] then
	      tmp2=tmpI(m)
	    end
	    tmp2=[tmp2,tmpI(m+1)]
	  else
	    if tmp2<>[] then
	      tmpI=[tmpI(1:m-size(tmp2,"*")),gsort(tmp2,"c","i"),tmpI(m+1:size(tmpI,"*"))]
	    end
	    tmp2=[]
	  end
	  m=m+1
	end
	I(k,:)=tmpI
      end
    
    elseif or(type(x)==[4,6]) then
      if find(size(x)==1)==[] then
	[Y,I]=gsort(bool2s(x),"r","i");
      else
	[Y,I]=gsort(bool2s(x),"g","i");
      end      
    else
      if find(size(x)==1)==[] then
	[Y,I]=gsort(x,"r","i");
      else
	[Y,I]=gsort(x,"g","i");
      end      
    end
  end
// rhs==2
elseif rhs==2
  if dim==1 then
    dim="r"
  else
    dim="c"
  end
  
  if type(x)==10 then
    x=mstr2sci(x)
    [tmp,I]=gsort(x,dim,"i");

    // Compute Y
    Y=[]
    for k=1:size(tmp,1)
      Y=[Y;strcat(tmp(k,:))];
    end
  
      // Reorder I
      for k=1:size(Y,1)
	m=1
	tmp2=[]
	tmpI=I(k,:)
	while m<=length(Y(k))
	  if part(Y(k),m)==part(Y(k),m+1) then
	    if tmp2==[] then
	      tmp2=tmpI(m)
	    end
	    tmp2=[tmp2,tmpI(m+1)]
	  else
	    if tmp2<>[] then
	      tmpI=[tmpI(1:m-size(tmp2,"*")),gsort(tmp2,"c","i"),tmpI(m+1:size(tmpI,"*"))]
	    end
	    tmp2=[]
	  end
	  m=m+1
	end
	I(k,:)=tmpI
      end
    
    elseif or(type(x)==[4,6]) then
    [Y,I]=gsort(bool2s(x),dim,"i");
  else
    [Y,I]=gsort(x,dim,"i");
  end

elseif rhs==3
  
  if dim==1 then
    dim="r"
  elseif dim==2
    dim="c"
  end
  
  if txt=="ascend" 
    txt="i"
  elseif txt=="descend"
    txt="d"
  end
    
  if type(x)==10 then
    x=mstr2sci(x)
    [tmp,I]=gsort(x,dim,txt);

    // Compute Y
    Y=[]
    for k=1:size(tmp,1)
      Y=[Y;strcat(tmp(k,:))];
    end
  
      // Reorder I
      for k=1:size(Y,1)
	m=1
	tmp2=[]
	tmpI=I(k,:)
	while m<=length(Y(k))
	  if part(Y(k),m)==part(Y(k),m+1) then
	    if tmp2==[] then
	      tmp2=tmpI(m)
	    end
	    tmp2=[tmp2,tmpI(m+1)]
	  else
	    if tmp2<>[] then
	      tmpI=[tmpI(1:m-size(tmp2,"*")),gsort(tmp2,"c",txt),tmpI(m+1:size(tmpI,"*"))]
	    end
	    tmp2=[]
	  end
	  m=m+1
	end
	I(k,:)=tmpI
      end
    
    elseif or(type(x)==[4,6]) then
    [Y,I]=gsort(bool2s(x),dim,txt);
  else
    [Y,I]=gsort(x,dim,txt);
  end
end
endfunction

