//exec("/home/huynh/poubelle/testNonReg/bug542.sce")
 

ierror=execstr("int8(7) & int8(8)","errcatch");

 if ierror == 0 then
 
 affich_result(%T,542);
 
 else
 affich_result(%F,542);
 
 end;
    
