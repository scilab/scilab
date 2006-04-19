function [X]=foo1(A)
// This function returns the positive components fo A diagonal

// Check the type and the size of A
if  type(A)<>1 then
    error("type of input argument must be a double");
end
if  size(A,1)<>size(A,2) then
    error("input argument must be square matrix");
end
//extraction of the positive components
X=[];
for i=1:size(A,1)
  if  A(i,i)>0 then
    X($+1)=A(i,i);
  end
end
endfunction
