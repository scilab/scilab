//exec("bug545.sci")
 

 
 
 diary("bug545.txt");
 
 a=int8(hypermat({3 2 2}));b=hypermat({1 2 2});
a(1,:,:)=b;
 
 diary(0);

[rep,stat]=unix_g("fgrep redefining bug545.txt") ;

 if stat == 0 then
 
 affich_result(%F,545);
 
 else
 affich_result(%T,545);
 
 end;
 
 mdelete('bug545.txt') ;
    
