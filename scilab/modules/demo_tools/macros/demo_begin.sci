
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function demo_begin()
	
	// Sauvegarde du mode
	demo_save_mode  = mode();
	
	// Sauvegarde de la pagination
	demo_save_lines = lines();
	
	// Sauvegarde des variables locales
	local_variables  = who("local");
	
	// Construction de la commande pour sauvegarder les variables locales
	local_cmd_str = "save(TMPDIR+''/who_local.dat''";
	for i=1:size(local_variables,"r")
		local_cmd_str = local_cmd_str + "," + local_variables(i);
	end
	local_cmd_str = local_cmd_str + ");";
	
	// Sauvegarde des variables locales dans le fichier TMPDIR/who_local.dat
	execstr(local_cmd_str);
	
	// Suppression des variables afin d'avoir un environnement propre pour les
	// Démos
	clear
	
	// Suppression de la pagination
	lines(0);
	
endfunction
