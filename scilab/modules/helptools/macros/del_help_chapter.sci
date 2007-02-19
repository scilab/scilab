function del_help_chapter(helptitle,modulemode)
	
	// =========================================================================================
	// Author : Pierre MARECHAL
	// Scilab team
	// Copyright INRIA
	// Date : August 1st 2006
	//
	// del_help_chapter(title,modulemode)
	//
	// Parameters
	//
	// title : a character string, the help chapter title
	//
	// Description
	//
	// This function deletes a entry in the helps list.
	// =========================================================================================
	
	global %helps;
	global %modules_helps;
	
	// Vérification des paramètres
	// -----------------------------------------------------------------------------------------
	[lhs,rhs]=argn(0);
	if (rhs<1) | (rhs>2) then error(39); end
	if (rhs == 1) then modulemode=%F; end
	if type(helptitle) <> 10 then error(55,1); end
	
	// Vérification que le titre est bien présent dans %helps
	// -----------------------------------------------------------------------------------------
	
  if modulemode then
    k1 = find( %modules_helps(:,2) == helptitle);
	  if k1 <> [] then
		  %modules_helps(k1,:) = [];
	  end
  else
    k1 = find( %helps(:,2) == helptitle);
	  if k1 <> [] then
		  %helps(k1,:) = [];
	  end
  end
	
	if strindex(stripblanks(helptitle),"(*)") == (length(stripblanks(helptitle)) - 2) then
		
		helptitle = strsubst(helptitle,"(*)","");
		helptitle = stripblanks(helptitle);
		
		for i=1:100
		  if modulemode then
		    k2 = find( %modules_helps(:,2) == helptitle+' ('+string(i)+')' );
			  if k2 <> [] then
				  %modules_helps(k2,:) = [];
			  end
		  else
		    k2 = find( %helps(:,2) == helptitle+' ('+string(i)+')' );
			  if k2 <> [] then
				  %helps(k2,:) = [];
			  end
		  end
		end
	end
	
endfunction
