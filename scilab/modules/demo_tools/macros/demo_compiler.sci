// =============================================================================
// Author : Ghislain HELIOT
// Scilab Project
// Copyright INRIA 2007
// =============================================================================

function [ans]=demo_compiler()
	if (~haveacompiler()) then
		x_message(['Scilab doesn''t find a C compiler','This demo is disabled'])
		ans=%f;
	else
		ans=%t;
	end
endfunction
