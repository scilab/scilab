function mputl(str,fd)
[lhs,rhs]=argn()
np=5000 //pack size
LF=ascii(10);CR=ascii(13)
if MSDOS then
  newline=CR+LF
else
  newline=LF
end
str=strcat(str,newline)+newline
n=length(str)
mp=floor(n/np)

if rhs==1 then //write to last opened file
  k1=1
  for i=1:mp
    mput(ascii(part(str,k1:k1+np)),'uc')
    k1=k1+np+1
  end  
  mput(ascii(part(str,k1:n)),'uc')
else
  if type(fd)==10 then // file given by its path
    opened=%t
    fil=fd
    [fd,ierr]=mopen(fil,'wb')
    if ierr~=0 then error('File '+fil+' cannot be opened for writing'),end
  else // file given by its logical unit
    opened=%f
  end
  k1=1
  for i=1:mp
    mput(ascii(part(str,k1:k1+np)),'uc',fd)
    k1=k1+np+1
  end  
  mput(ascii(part(str,k1:n)),'uc',fd);
  if opened then mclose(fd),end
end
endfunction
