//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

function demo_viewCode(this_script)
	
	if (~isdef('editor') & (funptr('editor')==0)) then
	  return
	end
	
	path = get_absolute_file_path(this_script)+this_script;
	
	// Récupération du handle sur la fenêtre courante
	my_fig = gcf();
	
	//Création du nouveau menu
	my_menu = uimenu( "parent"          , my_fig                       , ..
	    "label"           , gettext(" -- View Code -- ") , ..
	    "Foregroundcolor" , [ 0/255 81/255 6/255 ]       , ..
	    "callback"        , "editor("""+path+""")" );
	
endfunction
