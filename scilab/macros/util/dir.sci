function L=dir(str)
  mask = int32(61440), dirtype = 16384,
  if argn(2)==1 then
    files=listfiles(str)
  else
   files=listfiles()
  end
  
  n=size(files,'*')
  if n<>0 then files=gsort(files,'g','i'),end
  
  dt=zeros(n,1)
  bytes=zeros(n,1)
  isd(n,1)=%f
  for k=1:n
    [x,ierr]=fileinfo(files(k)) 
    if x<>[] then
      dt(k)=x(6)
      bytes(k)=x(1);
      isd(k)=double(int32(x(2)) & mask) == dirtype
    end
  end
  L=tlist(['dir','name','date','bytes','isdir'],files,dt,bytes,isd)
endfunction

