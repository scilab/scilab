// Non-regression test file for bug 731
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Date : 3 mai 2005

clear;
mode(-1);

SCIFILE=TMPDIR+"/bug731.dia";

function y=foo(x)
	a=sin(x)
	function y=sq(x), y=x^2,endfunction
	y=sq(a)+1
endfunction
diary(SCIFILE);
disp(fun2string(foo));
diary(0);

SCIFILECONTENTS=mgetl(SCIFILE);

REFCONTENTS=[" ";
		"!function y=ans(x)   !";
		"!                    !";
		"!  a = sin(x)        !";
		"!                    !";
		"!  function y=sq(x)  !";
		"!                    !";
		"!    , y=x^2,        !";
		"!                    !";
		"!  endfunction       !";
		"!                    !";
		"!  y = sq(a) + 1     !";
		"!                    !";
		"!endfunction         !"]
		
correct=%T

if or(REFCONTENTS<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,731);

clear
