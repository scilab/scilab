// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre Koumar
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function uiDumpTree(tree, b)

	[lhs,rhs]=argn(0);

	//Input arguments checking
	if rhs <= 0 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "uiDumpTree",1,2));
		return;
	end

	// Check 1st input : tree
	if rhs >= 1 then
		if (typeof(tree) == 'Tree') then
			myTree = tree;
			myB = %F;
		else
			error(msprintf(gettext("%s: Wrong type for input argument #%d: Tree expected.\n"), "uiDumpTree",1));
			return;
		end
		// Check 2nd input : b
		if rhs == 2 then
			if (type(b) == 4) then
				myB = b;
			else
				error(msprintf(gettext("%s: Wrong type for input argument #%d: Boolean expected.\n"), "uiDumpTree",2));
				return;
			end
		end
	end

	// Printing the tree
	function prettyPrint(myTree, myB, indentation)
		
		mprintf(indentation);
			mprintf('|_./ ');
			mprintf(myTree(2).label + '\n');

			indentation = indentation + "| ";

		if myB then
	  		mprintf(indentation);
				mprintf('`- ' + myTree(2).icon + '\n');
	  		mprintf(indentation);
				mprintf('`- ' + myTree(2).callback + '\n');
		end

		for childIndex = 3:size(myTree)
			prettyPrint(myTree(childIndex), myB, indentation);
		end

	endfunction

	prettyPrint(myTree, myB, "");

endfunction






