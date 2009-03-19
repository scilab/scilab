// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU <sylvestre.ledru@scilab.org>
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

// List of installed toolboxes

function tboxes = atomsGetInstalled()
  reps = atomsToolboxDirectory()
  tboxes = []
  for i=1:size(reps, 1)
	files = listfiles(reps(i))
	for j=1:size(files, 1)
	  tbox = reps(i) + "/" + files(j)
	  if fileinfo(tbox + "/loader.sce") <> [] then
	    tboxes($+1) = tbox
	  end
	end
  end
endfunction
