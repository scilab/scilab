// =============================================================================
// Author : Ghislain HELIOT
// Scilab Project
// Copyright INRIA 2007
// =============================================================================

function [resp]=demo_mdialog(fil)
	load(fil,'titl','namevar','value');
	resp=x_mdialog(titl,namevar,value);
endfunction

