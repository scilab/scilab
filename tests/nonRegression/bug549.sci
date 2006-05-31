//exec("bug549.sci")
 

p=poly(0,'p');
 

G = syslin('c', (1-p)/((p+1)*(p+3)));

 t = [0:1/100:8];
 
 diary("bug549.txt");
 
csim('step',t,G);

diary(0);

if MSDOS then
[rep,stat]=unix_g('findstr /c:""warning"" bug549.txt')
else
[rep,stat]=unix_g("fgrep warning bug549.txt") ;
end
 if stat == 0 then
 
 affich_result(%F,549);
 
 else
 affich_result(%T,549);
 
 end;
 mdelete("bug549.txt");
    
