function  M=createstruct(index,N)
// Authors: S. Steer, F. Delebecque, V. Couvert
// Copyright INRIA 
// Create a struct with field pointed by index equal to N

// If index is not a list, creation is done by routines
if type(index)==15 then
  
  // When only two indexes
  if size(index)==2 then
    
    // Second index is a fieldname
    if type(index(2))==10 then
      
      // First index is a numerical value
      if type(index(1))==1 then
	// X(p).f = y  -> index=list(p,f)
	if mtlb_mode() then
	  Dims=[1 index(1)]
	else
	  Dims=[index(1) 1]
	end
	M=mlist(["st","dims",index(2)],int32(Dims));
	nmax=prod(Dims);
	Li=list();
	for kl=1:nmax
	  Li(kl)=[];
	end
	Li($)=N;
	setfield(3,Li,M);
	return;
      // First index is a list of numerical values	
      elseif type(index(1))==15
	// X(p,q[,...]).f=z -> index=list(list(p,q[,...]),f)
	Dims=[];
	for kDims=1:lstsize(index(1))
	  Dims=[Dims,int32(index(1)(kDims))];
	end
	Li=list();
	for kl=1:prod(double(Dims))
	  Li(kl)=[];
	end
	Li($)=N;
	M=mlist(["st","dims",index(2)],Dims,Li);
	return;
      // First index is also a fieldname	
      else
	// X.f.g=y -> index=list(f,g)
	M=mlist(["st","dims",index(1)],int32([1,1]),...
	    mlist(["st","dims",index(2)],int32([1,1]),N));
	return;
      end
    
    // Second index is a fieldname  
    else
      // X.f(p[,q,...])=y : index=list(f,list(p[,q,...]))  
       
      if typeof(N)=="st" then // When recursive call of createstruct
	if type(index(2))==15 then // index=list(f,list(p,q[,...])) 
	  Dims=list2vec(index(2))'
	else // index=list(f,p)
	  if mtlb_mode() then
	    Dims=[1 index(2)]
	  else
	    Dims=[index(2) 1]
	  end
	end
	kmax=prod(Dims)
	z=list()
	for kl=1:kmax
	  z(kl)=[]
	end
	z(kmax)=getfield(3,N)
	z=mlist(getfield(1,N),int32(Dims),z);
      else
	z(index(2)(:))=N;
      end
      M=mlist(["st","dims",index(1)],int32([1,1]),z);
      return;
    end
  
  // Any number of indexes <> 2  
  else
  
    // Last index is a fieldname -> init struct
    if type(index($))==10 then
      M=mlist(["st","dims",index($)],int32([1,1]),N);
      index($)=null();
    // Last index is a numerical value or a list of numerical values  
    elseif type(index($))==1 | type(index($))==15 then
 
      // If only one index value (happens when recursive calls to createstruct)
      if size(index)==1 then
	if typeof(N)=="st" then
	  if type(index($))==15 then
	    Dims=list2vec(index($))'
	  else
	    if mtlb_mode() then
	      Dims=[1 index($)]
	    else
	      Dims=[index($) 1]
	    end
	  end
	  kmax=prod(Dims)
	  z=list()
	  for kl=1:kmax
	    z(kl)=[]
	  end
	  z(kmax)=getfield(3,N)
	  M=mlist(getfield(1,N),int32(Dims),z);
	else
	  M(index($)(:))=N;
	end
	index($)=null()
      // More than one index value	
      else
	if typeof(N)=="st" then
	  if type(index($))==15 then
	    Dims=list2vec(index($))'
	  else
	    if mtlb_mode() then
	      Dims=[1 index($)]
	    else
	      Dims=[index($) 1]
	    end
	  end
	  kmax=prod(Dims)
	  z=list()
	  for kl=1:kmax
	    z(kl)=[]
	  end
	  z(kmax)=getfield(3,N)
	  z=mlist(getfield(1,N),int32(Dims),z);
	else
	  z(index($)(:))=N;
	end
	M=mlist(["st","dims",index($-1)],int32([1,1]),z);
	index($)=null()
	index($)=null()
      end
    else
      // This case should not happen (unknown kind of index)
      error("Not implemented");
    end
    if index==list() then
      return
    end
    M=createstruct(index,M);
    return;
  end
else
  if type(index)==10 then
    M=mlist(["st","dims",index($)],int32([1,1]),N);
  else
    error("Not implemented");
  end
end
endfunction
