
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function help(key)
	
	//for compatibility with toolboxes making use of old cat files
	
	if (fileinfo('SCI/modules/helptools/help') <> []) then
		
		global %browsehelp
		nwniarg=find(sciargs()=="-nwni");
		texmacsarg=find(sciargs()=="--texmacs");
		noguiarg=find(sciargs()=="-nogui");
		
		if (%browsehelp<>[])&(nwniarg == [])&(texmacsarg == [])&(noguiarg == []) then
			
			change_old_man()
			INDEX=make_help_index()
			
			if argn(2)==0 then
				global %helps
				helpbrowser(%helps(:,1), getlanguage());
				//browsehelp(INDEX,"index");
				return
			end
			
			key=stripblanks(key)
			
			if or(part(key,1)==['(',')','[',']','{','}','%','''','""',':','*','/', ...
				'\','.','<','>','&','^','|','~','+','-']) then
				key="symbols";
			end
			
			//path=gethelpfile(key)
			
			//if path<>[] then
				global %helps
				helpbrowser(%helps(:,1), key, getlanguage());
				//browsehelp(path,key)
			//else
			//	apropos(key)
			//end
			
		else
			if (%browsehelp == []) then
				warning('%browsehelp not correctly defined. help browser disabled. ');
				return;
			end
			
			errmsg='help browser disabled in this mode : ';
			
			if ( nwniarg <> []) then
				errmsg=errmsg+'-nwni';
				warning(errmsg);
				return;
			end
			
			if ( texmacsarg <> []) then
				errmsg=errmsg+ '--texmacs';
				warning(errmsg);
				return;
			end
			
			if ( noguiarg <> [] ) then
				errmsg=errmsg+ '-nogui';
				warning(errmsg);
				return;
			end
		end
	else
		warning('Help not installed');
	end
	
endfunction









