// exec("bug528.sce")

 
execstr("fun2string(ones)","errcatch");

 

if lasterror()=="first argument is incorrect" then
affich_result(%T,528);
else
affich_result(%F,528);
 
end

