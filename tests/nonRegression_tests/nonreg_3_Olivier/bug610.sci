
// exec("bug610.sci");

 

function varargout = mysum(a, b)
  varargout = list(a + b, a - b, a * b);
endfunction

iCodeError= execstr('function [y, varargout] = MyNew(a,b) ..
   [y, varargout] = mysum(a,b) ..
   endfunction' ..
,'errcatch' );

//lasterror()
 
if iCodeError   then
disp("-----------------------");
lasterror()
disp("-----------------------");
affich_result(%T,610)
else
affich_result(%F,610)

end;
 
