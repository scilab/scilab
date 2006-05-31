//exec("bug564.sce")
 

function foo()
  if ok then
    
    disp(1);
    edited=%t;
    disp(2);
    needcompile=4
    disp(3);
    Cmenu='Replot';
    disp(4);,disp(Cmenu);
    break
  else
    Cmenu='Open/Set' 
    break  
  end
endfunction 

ok=%t;

diary('bug564.txt");

foo();
exec(foo);
diary(0); 
 
 
 [rep,stat]= unix_g("diff bug564.ref bug564.txt");



 if stat == 0 then
 
 affich_result(%T,564);
 
 else
 affich_result(%F,564);
 
 end;
    


mdelete("bug564.txt");
