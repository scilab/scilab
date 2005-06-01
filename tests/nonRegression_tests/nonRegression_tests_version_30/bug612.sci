// exec("bug612.sci");

mode (-1);
clear;

xset("wresize",0);
xset('wdim',2^16+3,5000);

resultLinux=xget("wdim");
resultWindows=[ 65535.    5000.];

if resultLinux == resultWindows then
	affich_result(%T,612)
else 
	resultLinux
	affich_result(%F,612);
end;
