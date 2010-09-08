// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre Koumar
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function b = uiEqualsTree(tree1, tree2)

	[lhs,rhs]=argn(0);

	//Input arguments checking
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "uiEqualsTree",2));
		return;
	end

	// Check 1st and 2nd inputs : tree1 & tree2
	if rhs == 2 then
		if (typeof(tree1) == 'Tree') then
			myTree1 = tree1;
		else
			error(msprintf(gettext("%s: Wrong type for input argument #%d: Tree expected.\n"), "uiEqualsTree",1));
			return;
		end
		
		if (typeof(tree2) == 'Tree') then
			myTree2 = tree2;
		else
			error(msprintf(gettext("%s: Wrong type for input argument #%d: Tree expected.\n"), "uiEqualsTree",2));
			return;				
		end
	end

	// Check if features of each nodes are matching
	if myTree1(1) == myTree2(1) then
		if myTree1(2).label == myTree2(2).label then
			if myTree1(2).icon == myTree2(2).icon then
				if myTree1(2).callback == myTree2(2).callback then
					b = %T
				else 
					b = %F
					return;
				end
			else 
				b = %F	
				return;
			end
		else 
			b = %F
			return;
		end
	else 
		b = %F
		return;
	end

endfunction
