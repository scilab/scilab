//exec("bug557.sci")
 

fileName= "bug557.txt";
 
diary(fileName);


 
mprintf('hello world\n');
diary(0);

 
 [rep,stat]= unix_g("diff bug557.ref bug557.txt");
 
 
 
 
 
 if stat == 0 then
 
 affich_result(%T,557);
 
 else
 affich_result(%F,557);
 
 end;
    
    mdelete(fileName);
