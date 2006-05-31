// exec( "bug570.sce");


 clear;
 exec("affich_result.sci");
iErrorCode=execstr('a(2,3,2)=''x''','errcatch');
 
//disp(' code retour = ' + string(iErrorCode));
  
if iErrorCode  then


affich_result(%F,570)
else
affich_result(%T,570)

end;
 




