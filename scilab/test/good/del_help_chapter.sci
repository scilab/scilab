function del_help_chapter(helptitle)
	
	// =========================================================================================
	// Author : Pierre MARECHAL
	// Scilab team
	// Copyright INRIA
	// Date : August 1st 2006
	// 
	// del_help_chapter(title,path)  
	// 
	// Parameters 
	// 
	// 	title : a character string, the help chapter title
	// 
	// Description 
	// 
	// 	This function deletes a entry in the helps list.
	// =========================================================================================
	
	global %helps;
	
	// Vérification des paramètres
	// -----------------------------------------------------------------------------------------
	[lhs,rhs]=argn(0);
	if rhs <> 1 then error(39); end
	if type(helptitle) <> 10 then error(55,1); end
	
	// Vérification que le titre est bien présent dans %helps
	// -----------------------------------------------------------------------------------------
	k1 = find( %helps(:,2) == helptitle);
	
	if k1 <> [] then
		%helps(k1,:) = [];
	end
	
	if strindex(stripblanks(helptitle),"(*)") == (length(stripblanks(helptitle)) - 2) then
		
		helptitle = strsubst(helptitle,"(*)","");
		helptitle = stripblanks(helptitle);
		
		for i=1:100
			k2 = find( %helps(:,2) == helptitle+' ('+string(i)+')' );
			
			if k2 <> [] then
				%helps(k2,:) = [];
			end
		end
	end
	
endfunction
