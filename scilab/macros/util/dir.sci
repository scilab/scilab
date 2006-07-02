function L=dir(str)

  mask = int32(61440),
  dirtype = 16384,
  basepath=''
  
  if argn(2)==1 then
  
    files=listfiles(str)
    basepath=str
    
  else
  
   files=listfiles()
   
  end
  
  n=size(files,'*')
  
  if n==0 then
  
    files=[];
    dt=[];
    bytes=[];
    isd=[];
    
  else
  
    files=gsort(files,'g','i')
  
    dt=zeros(n,1)
    bytes=zeros(n,1)
    isd(n,1)=%f
    
    for k=1:n
      [x,ierr]=fileinfo(basepath+'/'+files(k)) 
      if ( (x == []) & (ierr== -1) ) then [x,ierr]=fileinfo(files(k)),end
      
      if x<>[] then
				dt(k)=x(6)
				bytes(k)=x(1);
				isd(k)=double(int32(x(2)) & mask) == dirtype
      end
      
    end
    
  end
  
  L=tlist(['dir','name','date','bytes','isdir'],files,dt,bytes,isd)
  
endfunction

