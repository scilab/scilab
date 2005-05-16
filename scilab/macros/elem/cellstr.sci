function   c=cellstr(s)
// Copyright INRIA
// This function converts a string array (matrix or a vector) into a cell of strings.
// Input : s, vector,matrix of strings  
// Output : c, cell of strings components 
// size(c,1)=size(s,1) and size(c,2)=1
// F.B

// The input size must be inferor or equal to 2
if size(size(s),"*")>2 then
  error("Bad dimension of input argument")
end
// The input argument must be a string 
if typeof(s)<>"string" then
  error("input argument must be a string")
else
  C=cell(size(s,1),1)
  for i=1:size(s,1)
    c(i,1).entries=strcat(s(i,:))
  end
end
endfunction


