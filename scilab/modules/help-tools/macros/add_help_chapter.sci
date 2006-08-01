function ok = add_help_chapter(helptitle,path)
	
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
	// 
	// Description 
	// 
	// 	This function adds a new entry in the helps list. The help chapter files are to 
	// 	be located in a single directory. If the given  title  already exists in the helps
	// 	list associated with the same path nothing is done. The function checks if the 
	// 	directory exist. 
	// =========================================================================================
	
	global %helps;
	ok = %F;
	
	// Vérification des paramètres
	// -----------------------------------------------------------------------------------------
	[lhs,rhs]=argn(0);
	if rhs <> 2 then error(39); end
	if type(helptitle) <> 10 then error(55,1); end
	if type(path) <> 10 then error(55,2); end
	
	// Sauvegarde du chemin dans lequel l'on se trouve
	// -----------------------------------------------------------------------------------------
	current_directory = pwd();
	
	// Conversion du chemin dans le format du système d'exploitation utilisé
	// -----------------------------------------------------------------------------------------
	path=pathconvert(path,%f,%t);
	
	// Vérification que la chaîne de caractère passé en deuxième paramètre est bien un chemin
	// -----------------------------------------------------------------------------------------
	if ~isdir(path) then 
		error('second argument should give the path to a directory');
	
	// Transformation du chemin passé en deuxième paramètre
	// -----------------------------------------------------------------------------------------
	else
		chdir(path);
		if MSDOS then
			path = getlongpathname(pwd());
		else
			path = pwd();
		end
		chdir(current_directory);
	end
	
	// Vérification que le titre n'est pas déja présent dans %helps
	// -----------------------------------------------------------------------------------------
	
	k1 = find( %helps(:,2) == helptitle);
	
	if k1 == [] then
		
		// Cas où le titre du chapitre n'est pas présent
		%helps=[%helps;path,helptitle];
		ok = %T;
		return;
	
	else
		k2 = find( %helps(k1,1) == path );
		
		if k2 <> [] then 
			// Cas où le path est également le même
			return;
		else
			
			for i=1:100
				k3 = find( %helps(:,2) == helptitle+' ('+string(i)+')' );
				if k3 == [] then
					// On a pas trouvé de "title (i)"
					k4 = find( %helps(k3,1) == path );
					if k4 == [] then
						%helps=[%helps;path,helptitle+' ('+string(i)+')'];
						ok = %T;
						return;
					else
						return;
					end
					
				else
					// On a trouvé "title (i)"
					if find( %helps(k3,1) == path ) <> [] then
						return;
					end
				end
			end
		end
	end
	
endfunction
