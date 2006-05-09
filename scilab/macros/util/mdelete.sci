function mdelete(filename)
// Copyright INRIA
// Scilab Project - V. Couvert
// Delete a file given by filename (or a set of files if meta-characters used in filename) 
rhs=argn(2)
if rhs<>1 then
  error(77);
end
if type(filename)<>10 | size(filename,"*")<>1 then
  error("Wrong type argument, expecting a character string");
end

// Handle file path
if MSDOS then
  filename=strsubst(filename,"\","/")
end
// File path
k=strindex(filename,"/")
if k==[] then
  file_path="./"
else
  file_path=part(filename,1:k($))
  filename=part(filename,k($)+1:length(filename))
end

if strindex(filename," ")<>[] | strindex(file_path," ")<>[] then
  file_path=""""+file_path
  filename=filename+""""
end

if MSDOS then
  fullfilename=strsubst(file_path+filename,"/","\");
  [x,ierr]=fileinfo(fullfilename);
  if ~ierr then
    cmd="del "+fullfilename;
    unix_w(cmd);
  end
else
  cmd="rm -f "+file_path+filename;
  [x,ierr]=fileinfo(file_path+filename);
  if ~ierr then
    cmd="rm -f "+file_path+filename;
    unix_w(cmd);
  end
end

endfunction

