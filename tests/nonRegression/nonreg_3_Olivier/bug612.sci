// exec("bug612.sci");

xset("wresize",0);



xset('wdim',2^16+3,5000);


resultLinux=xget("wdim");


resultWindows= [ 65539.    5000.];

if resultLinux == resultWindows then

affich_result(%T,612)

else 

 resultLinux
 affich_result(%F,612);

end;


