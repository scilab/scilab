function tklib=gettklib()
  tcltkver=TCL_GetVersion('numbers');
	if MSDOS then 
		tklib='tk'+string(tcltkver(1))+string(tcltkver(2))+getdynlibext();
	else
		// In the binary version libtk8.4.so has been copied in
		// the SCI/bin directory and scilab script add SCI/bin
		// to the LD_LIBRARY_PATH (or SHLIB_PATH).
		// So, If libtk8.4.so (or .sl) exists in SCI/bin ... it's ok
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
				mprintf(gettext('messages','Warning: Error loading ')+libname+getdynlibext()+' :""'+lasterror()+'""')
			end
		end
		clear libname;
		clear cmd;
	end
	clear tcltkver;
endfunction
