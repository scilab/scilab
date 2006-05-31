// Non-regression test file for bug 564
// Copyright INRIA
 
 // Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 23 Mar 2005

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
 
BUG564TXT = mgetl('bug564.txt');
BUG564REF = mgetl('bug564.ref');
 
 if or(BUG564TXT<>BUG564REF) then
	affich_result(%F,564);
else
	affich_result(%T,564);
end

mdelete("bug564.txt");
