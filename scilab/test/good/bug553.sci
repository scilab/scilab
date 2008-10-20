//exec("bug553.sci")
 

n=12;
 

 stat=execstr("for n=1:3; disp(n); end","errcatch");


 if stat == 0 then
 
 affich_result(%T,553);
 
 else
 affich_result(%F,553);
 
 end;
    
