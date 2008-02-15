
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function browsehelp(path,key)
	// when %browsehelp is [] a  menu proposes to select a browser
	global INDEX 
	global %browsehelp
	
	browsehelp_configure()
	// set of possible browsers
	
	if argn(2)==0 then
		path=INDEX
		key="index"
	end
	if or(sciargs()=="-nw") then // the no window case
		if  (or(%browsehelp==['Scilab Browser'])) | ..
			or(%browsehelp==['firefox','nautilus','mozilla/netscape (gnome-moz-remote)','opera','quanta (kde)']) then
			run_help(path,key);
		else
			write(%io(2),mgetl(path))
		end
	else
		run_help(path,key)
	end
endfunction


function  browsehelp_configure(job)
	global %browsehelp
	if argn(2)<1 then job='check',end
	oldbrowsehelp=[];
	
	if ~MSDOS then
	
		if job=='set' then oldbrowsehelp=%browsehelp;%browsehelp=[],end
		browse_modes=['firefox';
				'nautilus';
				'mozilla/netscape (gnome-moz-remote)';
				'opera'
				'quanta (kde)'];
		
		if with_tk() then 
			browse_modes=[browse_modes;
				'Scilab Browser'];
		end
		if %browsehelp<>[] then //help mode already selected
			if and(browse_modes<>%browsehelp) then
				warning('Unhandled  help browser '+%browsehelp)
				%browsehelp=oldbrowsehelp; // If user select cancel
				%browsehelp= help_ask(browse_modes);
			end
		else // ask for an help mode
			%browsehelp=oldbrowsehelp; // If user select cancel
			%browsehelp= help_ask(browse_modes);
		end
	
	else //for windows 
		if job=='set' then oldbrowsehelp=%browsehelp;%browsehelp=[],end
		browse_modes=['Default Windows Browser';];
		if with_tk() then 
			browse_modes=[browse_modes;
				'Scilab Browser'];
		end
	
		if %browsehelp<>[] then //help mode already selected
			if and(browse_modes<>%browsehelp) then
				warning('Unhandled  help browser '+%browsehelp)
				%browsehelp=oldbrowsehelp; // If user select cancel
				%browsehelp= help_ask(browse_modes);
			end
		else // ask for an help mode
			%browsehelp=oldbrowsehelp; // If user select cancel
			%browsehelp= help_ask(browse_modes);
		end
	end
endfunction


function run_help(path,key)
	// the  help browser
	// browse_modes=['nautilus'];
	[lhs,rhs]=argn(0);
	global INDEX
	global %browsehelp
	key1=strsubst(key,' ','_') //for temp file and widget name
	
	select %browsehelp
	case 'firefox' then
		// we must check if a firefox is opened or not
		exec_str = 'if firefox -remote ""ping()"" 2> /dev/null ; then ..
			firefox -remote ""openURL('+path+')"" & ..
			else ..
			firefox '+path+' & ..
			fi ' ;
		unix_s(exec_str);
	case 'nautilus' then 
		unix_s("nautilus --no-desktop "+path+ '&');
	case 'mozilla/netscape (gnome-moz-remote)' then
		unix_s("gnome-moz-remote --raise  file://"+path+ '&');
	case 'opera' then
		unix_s(%browsehelp + " file://" +path+ '&');
	case 'quanta' then
		unix_s(%browsehelp + " --unique file://" +path+ '&');
	case 'Default Windows Browser' then
		winopen(path);
	case 'Scilab Browser' then 
		sciGUIhelp(key);
	else
		write(%io(2),mgetl(path))
	end
endfunction


function md=help_ask(modes)
	global %browsehelp
	n=0
	n=x_choose(modes,['Choose the help browser';'you want to use']);
	if n==0 then // if user cancels then old browser is kept
		md=%browsehelp;
	else
		md=modes(n)
	end
	savedefaultbrowser(md);    
endfunction
