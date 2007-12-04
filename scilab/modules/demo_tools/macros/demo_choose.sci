// =============================================================================
// Author : Ghislain HELIOT
// Scilab Project
// Copyright INRIA 2007
// =============================================================================

function [num]=demo_choose(fil)
	load(fil,'choice','titl');
	num=tk_choose(choice,titl);
endfunction
