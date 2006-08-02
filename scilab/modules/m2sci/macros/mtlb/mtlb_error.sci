function mtlb_error(varargin)
  
  if argn(2)==1 then
    m=varargin(1)
  else
    F=varargin(1)
    //check if it is an identifier or a message
    //if strindex(F,'%')==[] then //not a format
      if size(tokens(F,':'),'*')>1 then
	// an identifier, it is ignored
	m=msprintf(varargin(2:$))
      else
	m=msprintf(varargin(:))
      end
    //end
  end
  error(m)
endfunction
