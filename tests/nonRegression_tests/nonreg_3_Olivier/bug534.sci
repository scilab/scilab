// exec("bug534.sce")

 
x=[3 1 5];

iErrorCode=execstr("geomean(x)","errcatch");

 

if iErrorCode==0 then
affich_result(%T,534);
else
affich_result(%F,534);
 
end

