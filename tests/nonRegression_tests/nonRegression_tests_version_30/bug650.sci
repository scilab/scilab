// bug 650
// Allan CORNET
// INRIA 2004
// verifier qu'il n'y a pas de fichier avec l'extension .testbug650 pour effectuer le test
 
if MSDOS then
	clear
	mode (-1)
	correct = %F
	correctT1 = %F
	correctT2 = %F
	correctT3 = %F
	correctT4 = %F
	// unix_w
	res=execstr("unix_w(''dir *.testbug650'')",'errcatch','n');
	err=lasterror();
	err=stripblanks(err);
	
	if (res==9999) then
		if ( err == "unix_w: Fichier introuvable" ) then
			correctT1 = %T;
		else
			correctT1 = %F;
		end
	end
	
	// unix_x
	res=execstr("unix_x(''dir *.testbug650'')",'errcatch','n');
	err=lasterror();
	err=stripblanks(err);
	
	if (res==9999) then
		if ( err == "unix_x: Fichier introuvable" ) then
			correctT2 = %T;
		else
			correctT2 = %F;
		end
	end
	
	// unix_s
	res=execstr("unix_s(''dir *.testbug650'')",'errcatch','n');
	err=lasterror();
	err=stripblanks(err);
	
	if (res==9999) then
		if ( err == "unix_s: error during ``dir *.testbug650'''' execution" ) then
			correctT3 = %T;
		else
			correctT3 = %F;
		end
	end
	
	// unix_g
	res=execstr("[t,h]=unix_g(''dir *.testbug650'')",'errcatch','n');
	if (h==1) then
		if ( t == "" ) then
			correctT4 = %T;
		else 
			correctT4 = %F;
		end
	end
	
	// verifie que tous les unix_ passent
	if correctT1 & correctT2 & correctT3 & correctT4 then
		correct=%T;
	else
		correct=%F;
	end
	
	affich_result(correct, 650)
	clear
	
else

	affich_result(%T, 650)
	
end
