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

if strindex(filename," ") then
  filename=""""+filename+""""
end

if strindex(file_path," ") then
  file_path=""""+file_path+""""
end
  

if MSDOS then
  cmd="del "+file_path+filename;
else
  cmd="rm -f "+file_path+filename;
end
unix_w(cmd);
endfunction

