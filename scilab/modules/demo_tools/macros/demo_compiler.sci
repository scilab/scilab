
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Ghislain HELIOT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [ans]=demo_compiler()
	if (~haveacompiler()) then
		messagebox(['Scilab doesn''t find a C compiler','This demo is disabled'],"modal");
		ans=%f;
	else
		ans=%t;
	end
endfunction
