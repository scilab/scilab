//-----------------------------------------------------------------------------
// Pierre MARECHAL
// Scilab team
// Copyright INRIA
// Date : 25/10/2007
//
// Launch unitary tests
//-----------------------------------------------------------------------------

function unit_test_run(varargin)
	
	lhs = argn(1);
	rhs = argn(2);
	
	if (rhs == 0) then
		test_run([],[],["unit_tests"]);
		
	elseif rhs == 1 then
		argument_1 = varargin(1);
		test_run(argument_1,[],["unit_tests"]);
		
	elseif (rhs == 2) | (rhs == 3) then
		argument_1 = varargin(1);
		argument_2 = varargin(2);
		test_run(argument_1,argument_2,["unit_tests"]);
		
	elseif rhs == 3 then
		argument_1 = varargin(1);
		argument_2 = varargin(2);
		argument_3 = [argument_3,"unit_tests"];
		test_run(argument_1,argument_2,argument_3);
	end
	
endfunction
