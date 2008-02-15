
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function manedit(manitem,editor)
	
	// manitem : character string giving a manitem
	
	path=gethelpfile(manitem)
	
	if path<>[] then 
		px=strsubst(path,'.htm','.xml')
		if fileinfo(px)<>[] then 
			path=px
		else
			ierr=execstr('t=type('+manitem+')','errcatch')
			if ierr==0&t==13 then
				path=TMPDIR+'/'+manitem+'.xml'
				help_skeleton(manitem,TMPDIR)
			else
				path=[]
			end
		end
	end
	
	if path<>[] then
		scipad(path);
	end
	
endfunction
