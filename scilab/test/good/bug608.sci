// exec( "bug608.sci");


 

a=uint8(zeros(4,4));
b=(a<=uint8(0))
 
c =  [ %T %T %T %T  ; %T %T %T %T ; %T %T %T %T ; %T %T %T %T  ];
 
//disp(' code retour = ' + string(iErrorCode));
  
if c == b  then


affich_result(%T,608)
else
affich_result(%F,608)

end;
 




