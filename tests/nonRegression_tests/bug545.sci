// Non-regression test file for bug 545
// Copyright INRIA
// Scilab Project
// Date : 9 Dec 2005

 diary("bug545.txt");
 
a=int8(hypermat({3 2 2}));b=hypermat({1 2 2});
a(1,:,:)=b;

diary(0);

if MSDOS then
	[rep,stat]=unix_g("findstr /c:""redefining"" bug545.txt");
else
	[rep,stat]=unix_g("fgrep redefining bug545.txt") ;
end

if stat == 0 then
	affich_result(%F,545);
else
	affich_result(%T,545);
end;
