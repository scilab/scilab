// =============================================================================
// Author : Ghislain HELIOT
// Scilab Project
// Copyright INRIA 2007
// =============================================================================

function demo_begin
	mode(-1);
	save(SCI+"/var.data",who('local'));
	clear
	oldln=lines();
	lines(0);
endfunction
