// exec("bug625.sci");  
 
iReturnCode=execstr('xget(''mark size'')','errcatch');

if iReturnCode == 0 then
	affich_result(%T,625);
else
	disp (lasterror());
	affich_result(%F,625);
end
