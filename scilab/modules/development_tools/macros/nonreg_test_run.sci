//-----------------------------------------------------------------------------
// Pierre MARECHAL
// Scilab team
// Copyright INRIA
// Date : 22/11/2007
//
// Launch Non-Regression tests
//-----------------------------------------------------------------------------

function nonreg_test_run(varargin)
	
	lhs = argn(1);
	rhs = argn(2);
	
	if (rhs == 0) then
		test_run([],[],["nonreg_tests"]);
		
	elseif rhs == 1 then
		argument_1 = varargin(1);
		test_run(argument_1,[],["nonreg_tests"]);
		
	elseif rhs == 2 then
		argument_1 = varargin(1);
		argument_2 = varargin(2);
		test_run(argument_1,argument_2,["nonreg_tests"]);
		
	elseif rhs == 3 then
		argument_1 = varargin(1);
		argument_2 = varargin(2);
		argument_3 = varargin(3);
		test_run(argument_1,argument_2,[argument_3,"nonreg_tests"]);
		
	end
	
endfunction
