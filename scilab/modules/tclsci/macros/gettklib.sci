// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function tklib=gettklib()
tklib = [];
  tcltkver=TCL_GetVersion('numbers');
	if MSDOS then 
		tklib='tk'+string(tcltkver(1))+string(tcltkver(2))+getdynlibext();
	else
		// In the binary version libtk8.X.so has been copied in
		// the SCI/bin directory and scilab script add SCI/bin
		// to the LD_LIBRARY_PATH (or SHLIB_PATH).
		// So, If libtk8.X.so (or .sl) exists in SCI/bin ... it's ok
		libname='libtk'+string(tcltkver(1))+'.'+string(tcltkver(2));
		if fileinfo('SCI/bin/'+libname+getdynlibext()) <> [] then
			tklib=libname+getdynlibext();
			return;
		end
		currentwarningmode=warning('query');
		warning('off');
		cmd ="link(''"+libname+getdynlibext()+"'')";
		ierr=execstr(cmd, 'errcatch');
		warning(currentwarningmode);
		if (ierr == 0) then
			tklib=libname+getdynlibext();
		else
			currentwarningmode=warning('query');
			warning('off');
			cmd ="link(''"+libname+getdynlibext()+".0'')";
			ierr=execstr(cmd, 'errcatch')
			warning(currentwarningmode);
			if (ierr== 0) then
				tklib=libname+getdynlibext()+'.0';
			else
				warning(msprintf(gettext("Error %s while loading library %s"),lasterror(),libname+getdynlibext()));
			end
		end
		clear libname;
		clear cmd;
	end
	clear tcltkver;
endfunction
