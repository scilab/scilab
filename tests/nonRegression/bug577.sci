
// exec("bug577.sci");

 
x=rand(5,5,100)+%i;



iCodeError= execstr('mtlb_save(''gain.mat'',''x'')','errcatch' );

lasterror()
 
if iCodeError ==0   then

affich_result(%T,577)
else
affich_result(%F,577)
end;
 
