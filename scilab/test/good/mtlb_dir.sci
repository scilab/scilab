function [L]=mtlb_dir(path)
// Copyright INRIA
// Emulation function for dir() Matlab function
// V.C.
// See SCI/macros/util/dir.sci for details

// opt=='disp' -> result not affected to a variable
// opt=='var' -> result affected to a variable

rhs=argn(2)

mask = int32(61440)
dirtype = 16384
basepath=''

if rhs==1 then
  files=listfiles(path)
  basepath=path
else
  files=listfiles()
end

n=size(files,"*")
if n<>0 then 
  files=gsort(files,"g","i")
end

lfiles=list()
ldate=list()
lbytes=list()
lisdir=list()
for k=1:n
  lfiles(k)=files(k)
  [x,ierr]=fileinfo(basepath+'/'+files(k)) 
  if ( (x == []) & (ierr== -1) ) then [x,ierr]=fileinfo(files(k)),end
  if x<>[] then
    w=getdate(x(6))
    month=["Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"]
    ldate(k)=string(w(6))+"-"+month(w(2))+"-"+string(w(1))+" "+string(w(7))+":"+string(w(8))+":"+string(w(9))
    lbytes(k)=x(1);
    lisdir(k)=double(int32(x(2)) & mask) == dirtype
  end
end
if n==1 then
  lfiles=lfiles(1);
  ldate=ldate(1);
  lbytes=lbytes(1);
  lisdir=lisdir(1);
end
L=mlist(["st","dims","name","date","bytes","isdir"],int32([n 1]),lfiles,ldate,lbytes,lisdir)

endfunction
