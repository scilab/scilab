// exec( "/home/huynh/poubelle/testNonReg/bug568.sce");
// exec("e:\testNonReg\bug568.sce");

aa=zeros(4,4,11);
 
iErrorCode=execstr('bb(1,1,:)=aa(1,1,:)','errcatch');
 
//disp(' code retour = ' + string(iErrorCode));
  
if iErrorCode  then


affich_result(%T,568)
else
affich_result(%F,568)

end;
 




