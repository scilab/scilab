
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function change_help_language(language)
	
	// =========================================================================
	// change_help_language(language)
	// =========================================================================
	
	global %helps;
	global %helps_modules;
	%HELPS=[%helps_modules;%helps];
	// V�rification des param�tres
	// -------------------------------------------------------------------------
	[lhs,rhs]=argn(0);
	if rhs <> 1 then error(39); end
	if type(language) <> 10 then error(55,1); end
	
	// Sauvegarde du chemin dans lequel l'on se trouve
	// -------------------------------------------------------------------------
	current_directory = pwd();
	
	dirs = %HELPS(:,1);
	titles = %HELPS(:,2);
	
	// Parcours des r�pertoires list�s dans %helps
	for k=1:size(dirs,'*')
		
		// V�rification que le whatis.htm existe
		if fileinfo(pathconvert(dirs(k)+"/../"+language+"/whatis.htm",%f,%f)) <> [] then
			
			// Construction du nouveau r�pertoire
			chdir(pathconvert(dirs(k)+"/../"+language,%f,%f));
			if MSDOS then
				new_dir = getlongpathname(pwd());
			else
				new_dir = pwd();
			end
			
			// Construction du nouveau titre
			whatis = mgetl("whatis.htm");
			title_row = grep(convstr(whatis,"l"),"<title>");
			if strindex(convstr(whatis(title_row),"l"),"</title>") <> [] then
				
				start_title = strindex(convstr(whatis(title_row),"l"),"<title>") + 7;
				end_title = strindex(convstr(whatis(title_row),"l"),"</title>") - 1;
				new_title = stripblanks(part(whatis(title_row),start_title:end_title));
				
				// Modification de %helps
				%HELPS(k,1) = new_dir;
				%HELPS(k,2) = new_title;
			end
		end
	end
	
	chdir(current_directory);

endfunction
