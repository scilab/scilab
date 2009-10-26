// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function manedit(manitem)
	
	lhs=argn(1);
	rhs=argn(2);
	
	if rhs<>1 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"manedit",1));
	end
	
	// manitem : character string giving a manitem
	
	path=get_item_xml_path(manitem);
	
	if path==[] then
		ierr=execstr('t=type('+manitem+')','errcatch')
		if ierr==0&t==13 then
			path = pathconvert(TMPDIR+"/"+manitem+".xml",%F,%F);
			help_skeleton(manitem,TMPDIR);
		else
			path=[]
		end
	end
	
	if path <> [] then
	  if (isdef('editor') | (funptr('editor')<>0)) then
		  editor(path);
		end
	end
	
endfunction



function path = get_item_xml_path(manitem)
	
	path = [];
	
	global %helps
	global %helps_modules;
	%HELPS=[%helps_modules;%helps];
	
	for k=1:size(%HELPS(:,1),'*')
		if fileinfo(pathconvert(%HELPS(k,1)+"/"+manitem+".xml",%f,%f)) <> [] then
			path = pathconvert(%HELPS(k,1)+"/"+manitem+".xml",%f,%f);
			return;
		end
	end
	
endfunction
