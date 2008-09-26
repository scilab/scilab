// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ok = add_help_chapter(helptitle,path,modulemode)
	
	// =========================================================================================
	// Modified by Pierre MARECHAL
	// Scilab team
	// Copyright INRIA
	// Date : August 1st 2006
	//
	// add_help_chapter(title,path)
	//
	// Parameters
	//
	// 	title : a character string, the help chapter title 
	// 	path : a character string, the path of the directory containing the help files.
	// 	modulemode : a boolean %F by default.
	//  if %F add to %modules_helps else to %helps.
	//
	// Description
	//
	// 	This function adds a new entry in the helps list. The help chapter files are to
	// 	be located in a single directory. If the given  title  already exists in the helps
	// 	list associated with the same path nothing is done. The function checks if the
	// 	directory exist.
	// =========================================================================================
	
	global %helps;
	global %helps_modules;
	ok = %F;
	
	// V�rification des param�tres
	// --------------------------------------------------------------------------------
	[lhs,rhs]=argn(0);
	if rhs <> 3 then modulemode=%F; end
	
	if ( rhs > 3 | rhs < 2 ) then error(39); end
	if type(helptitle) <> 10 then error(55,1); end
	if type(path) <> 10 then error(55,2); end
	
	// Sauvegarde du chemin dans lequel l'on se trouve
	// --------------------------------------------------------------------------------
	current_directory = pwd();
	
	// Conversion du chemin dans le format du syst�me d'exploitation utilis�
	// --------------------------------------------------------------------------------
	path=pathconvert(path,%f,%t);
	
	// V�rification que la cha�ne de caract�re pass� en deuxi�me param�tre est bien un chemin
	// --------------------------------------------------------------------------------
	if ~isdir(path) then 
		error('second argument should give the path to a directory');
	
	// Transformation du chemin pass� en deuxi�me param�tre
	// --------------------------------------------------------------------------------
	else
		chdir(path);
		if MSDOS then
			path = getlongpathname(pwd());
		else
			path = pwd();
		end
		chdir(current_directory);
	end
	
	// V�rification que le titre n'est pas d�ja pr�sent dans %helps
	// --------------------------------------------------------------------------------
	if (modulemode) then
	  k1 = find( %helps_modules(:,2) == helptitle);
	else
	  k1 = find( %helps(:,2) == helptitle);
	end
	
	
	
	if k1 == [] then
		
		// Cas o� le titre du chapitre n'est pas pr�sent
		if (modulemode) then
		 %helps_modules=[%helps_modules;path,helptitle];
		else
		  %helps=[%helps;path,helptitle];
		end
		
		ok = %T;
		return;
	
	else
	  if (modulemode) then
	    k2 = find( %helps_modules(k1,1) == path );
	  else
	    k2 = find( %helps(k1,1) == path );
	  end
		
		if k2 <> [] then 
			// Cas o� le path est �galement le m�me
			return;
		else
			
			for i=1:100
			  if modulemode then
			    k3 = find( %helps_modules(:,2) == helptitle+' ('+string(i)+')' );
			  else
			    k3 = find( %helps(:,2) == helptitle+' ('+string(i)+')' );
			  end
				
				if k3 == [] then
					// On a pas trouv� de "title (i)"
					if modulemode then
					  k4 = find( %helps_modules(k3,1) == path );
					else
					  k4 = find( %helps(k3,1) == path );
					end
					
					if k4 == [] then
					  if modulemode then
					    %helps_modules=[%helps_modules;path,helptitle+' ('+string(i)+')'];
					  else
					    %helps=[%helps;path,helptitle+' ('+string(i)+')'];
					  end
						
						ok = %T;
						return;
					else
						return;
					end
					
				else
					// On a trouv� "title (i)"
					if modulemode then
					  if find( %helps_modules(k3,1) == path ) <> [] then
						  return;
					  end
					else
					  if find( %helps(k3,1) == path ) <> [] then
						  return;
					  end
					end
					
				end
			end
		end
	end
	
endfunction
