// Non-regression test file for bug 584
// Copyright INRIA

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 23 Mar 2005

format('e',32);
result=logspace( 3, 3, 1 );

trueValue=1.0000000000000006821210263E+03;

if result == trueValue then ;
	format('v',10);
	affich_result(%T,584);
else
	format('v',10);
	affich_result(%F,584);
end

format('v',10);
