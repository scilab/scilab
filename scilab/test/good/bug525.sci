// exec("bug525.sce")
stacksize(2e6);
 
execstr("function foo","errcatch");

 

if lasterror()=="endfunction is missing" then
affich_result(%T,525);
else
affich_result(%F,525);
 
end

