function M=%ce_e(varargin)
// Copyright INRIA
//extract an sub_cell x(i,j,k...)
  [lhs,rhs]=argn(0)
  M=varargin($); //the source cell

  nind=size(varargin)-1
  dims1=double(M.dims)  
  if nind>size(dims1,'*') then
    //extend the dimension according to the number of indexes
    dims1(size(dims1,'*')+1:nind)=1
  elseif nind<size(dims1,'*') then
    //reduce the dimension according to the number of indexes
    dims1=[dims1(1:nind-1) prod(dims1(nind:$))]
    if size(dims1,'*')==1 then dims1=[dims1 1],end
  end
  
  //get the "linear" vector of indexes
  [indx,I]=convertindex(dims1,varargin(1:$-1)); 
  dims=zeros(1,nind)

  //computing dimensions of the result
  //following loop should be merged in the convertindex code
  for kk=1:nind
    ik=varargin(kk)
    
    if or(type(ik)==[2 129]) then
      ik=horner(ik,dims1(kk))
    end
    if type(ik)==4 then
       dims(kk)=max(find(ik))
    elseif size(ik,1)~=-1 then 
      dims(kk)=size(ik,'*')
    else 
      dims(kk)=dims1(kk)
    end
  end

    
  while  dims($)==1 then dims($)=[],end
  select size(dims,'*')
  case 0 then
    dims=[1,1]
  case 1 then
    dims=[dims,1]
  else 
    dims=matrix(dims,1,-1)

  end
  I=double(I);
  
  M.dims=int32(dims)
  
  M.entries=list(M.entries(I))
  if lstsize(M.entries)==1 then
    M.entries=M.entries(1)
  end
endfunction
