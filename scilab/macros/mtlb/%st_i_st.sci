function M=%st_i_st(varargin)
//insert the struct varargin($-1) into the struct varargin($)
//at position varargin(1), varargin(2),varargin(3),...
//insert the struct varargin($-1) as the field varargin(1)
//of the struct varargin($)
  [lhs,rhs]=argn(0)
  M=varargin($)
  N=varargin($-1)//inserted matrix
  dims=double(matrix(M.dims,-1,1));
  //v=matrix(M.entries,-1,1);
  
  if type(varargin(1))==10 //addind a new field
    flds=getfield(1,M);flds=[flds,varargin(1)];
    setfield(1,flds,M);setfield(lstsize(M)+1,N,M);
    return
  end  
  
  //Subscripted assignment between structures
  
  //check if the two struct have the same fields
  FM=getfield(1,M);FM=FM(3:$);nFM=size(FM,'*')
  FN=getfield(1,N);FN=FN(3:$);nFN=size(FN,'*')
  [FR,kR]=unique([FM FN])

  Nfields=size(FR,'*')
  //computing the dimension of the result
  nd=size(dims,'*')
  if rhs-2>nd then dims(nd+1:rhs-2)=1;end  

  //convert N-dimensionnal indexes to 1-D and extend dims if necessary
  [Ndims,I]=convertindex(dims,varargin(1:$-2))

   if or(Ndims>dims) then
    //extend the destination matrix
    I1=0
    for k=size(Ndims,'*'):-1:1
      ik1=(1:dims(k))'
      if ik1<>[] then
	if Ndims(k)>1 then
	  if size(I1,'*')>1 then
	    I1=(Ndims(k)*I1).*.ones(ik1)+ones(I1).*.(ik1-1)
	  else
	    I1=Ndims(k)*I1+ik1-1
	  end
	else
	  I1=Ndims(k)*I1+ik1-1
	end
      end
    end

    v1=list();for k=1:Ndims,v1(k)=[];end
    // create the resulting matrix
    R=mlist(['st','dims',matrix(FR,1,-1)],int32(Ndims));
    for k=1:size(FR,'*'),setfield(2+k,v1,R),end
    // populate it with M entries

    for k=1:nFM
      v2=v1;
      kf=find(FR==FM(k));
      w=getfield(k+2,M);if type(w)<>15 then w=list(w),end
      for i=1:size(I1,'*'), 
	v2(I1(i)+1)=w(i)
      end
      setfield(kf+2,v2,R);
    end

   end
   
   //insert N entries into result
   for k=1:nFN
     kf=find(FR==FN(k))
     v2=getfield(kf+2,R)
     w=getfield(k+2,N);if type(w)<>15 then w=list(w),end
     for i=1:size(I,'*'), 
       v2(I(i))=w(i)
     end 
     setfield(kf+2,v2,R);
   end
   
   //remove trailing unitary dimensions
   while  Ndims($)==1 then Ndims($)=[],end
   R.dims=int32(Ndims)
   M=R

endfunction
