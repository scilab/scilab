function y=char(varargin)
// Copyright INRIA
// Case : One input argument
// Input : a cell of characters arrays 
// Output : vector (column) of strings, 
// This function returns a vector in  which the rows are the components of the cell characters arrays
//
// Input : an array of ascii codes
// Output :a characters array corresponding into ascii codes
// If dims input =[n1,n2,n3,n4,....],then returned a value has the dims=[n1,n3,n4,...],
// returned value have same size as input value instead of second dims !
//
// Case : more than one input argument : y=char(st1,st2,st3,....)
// Inputs must be string : st1,st2,st3,...
// Outputs : vector (column) of strings
// This function returns a vector in  which the rows are st1, st2,st3,...,
// the length of all strings sti is completed by blanks,in order to have the same length for each sti.
// F.B

  rhs=argn(2)
  y=[]
  //One input argument
  if rhs==1 then
    if typeof(varargin(1))=="ce" then //input argument is a cell of characters arrays
      c=varargin(1)
      lst=c.entries
      if size(c)==1 then //cell contains one element
	if typeof(lst)=="hypermat" then // cell elements are characters arrays
	  if type(lst.entries)==10 then
	    t=emptystr(size(lst,1),1)
	    lst=matrix(lst,[size(lst,1),size(lst,"*")/size(lst,1)])
	    for j=1:size(lst,2)
	      t=t+lst(:,j)
	    end
	    y=[y;t]
	  else
	    error("cell elements must be characters arrays")
	  end
	elseif  type(lst)==10 then //cell elements are a string matrix
	  t=emptystr(size(lst,1),1)
	  for k=1:size(lst,2)
	    t=t+lst(:,k)
	  end
	  y=[y,t]
	else // cell contains more than one element
	  error("cell elements must be arrays chars")
	end
      else
	for i=1:size(lst)
	  if typeof(lst(i))=="hypermat" then
	    if type(lst(i).entries)==10 then		  
	      ctemp=lst(i)
	      t=emptystr(size(ctemp,1),1)
	      ctemp=matrix(ctemp,[size(ctemp,1),size(ctemp,"*")/size(ctemp,1)])
	      for j=1:size(ctemp,2)
		t=t+ctemp(:,j)
	      end
	      y=[y;t]
	    else
	      error("cell elements must be arrays chars")
	    end
	  elseif type(lst(i))==10 then
	    t=emptystr(size(lst(i),1),1)
	    for k=1:size(lst(i),2)
	      t=t+lst(i)(:,k)
	    end	
	    y=[y;t]
	  else
	    error("cell elements must be arrays chars")	
	  end
	end
      end    
      // Add blank at the length of strings
      bl=' '
      maxstr=max(length(y))
      for i=1:size(y,"*")
	nb_bl=maxstr-length(y(i))
	if nb_bl>0 then
	  y(i)=y(i)+part(bl,ones(1,nb_bl))
	end
      end
    elseif type(varargin(1))==1|type(varargin(1))==8 then //Input is a matrix of integers (or reals) 
      y=asciimat(varargin(1))
    elseif type(varargin(1))==10 then //Input is a matrix of strings
      for i=1:size(varargin(1),1)
	y(i)=strcat(varargin(1)(i,:))
      end
    elseif typeof(varargin(1))=="hypermat" then //Input is a hypermatrix of strings
      if type(varargin(1).entries)==10 then
	y=varargin(1)
      elseif type(varargin(1).entries)==1|type(varargin(1).entries)==8 then ////Input is a hypermatrix of integers (or reals) 
	y=asciimat(varargin(1))
      else
	error("not implemented")
      end
    else 
      error("input argument must be a cell")
    end 
    //More than one input argument
  elseif rhs>1 then
    for i=1:size(varargin)
      if typeof(varargin(i))=="hypermat" then //Input sti is a hypermatrix of strings
	lst=varargin(i)
	if type(lst.entries)==10 then
	  t=emptystr(size(lst,1),1)
	  lst=matrix(lst,[size(lst,1),size(lst,"*")/size(lst,1)])
	  for j=1:size(lst,2)
	    t=t+lst(:,j)
	  end
	  y=[y;t]
	else
	  error("Inputs arguments must be characters arrays")
	end
      elseif  type(varargin(i))==10 then //Input sti is a matrix of strings
	lst=varargin(i)
	t=emptystr(size(lst,1),1)
	for k=1:size(lst,2)
	  t=t+lst(:,k)
	end
	y=[y;t]
      else 
	error("Inputs arguments must be characters arrays")
      end
    end
    //Add blanks at the length of strings
    bl=' '
    maxstr=max(length(y))
    for i=1:size(y,"*")
      nb_bl=maxstr-length(y(i))
      if nb_bl>0 then
	y(i)=y(i)+part(bl,ones(1,nb_bl))
      end
    end
  end

endfunction
