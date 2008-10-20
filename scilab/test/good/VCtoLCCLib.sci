//-------------------------------------------------------------------
// Allan CORNET INRIA 2004 - 2006
// Export LibScilab for LCC compiler
//-------------------------------------------------------------------
function VCtoLCCLib()
if with_lcc()==%t then
	if MSDOS==%t then
		DirCur=pwd();
		chdir(WSCI+'\bin');
	
		Exportalibrary('LibScilab');
		if ( with_atlas() == %T ) then
			ExportAtlasLibrary();
			Exportalibrary('arpack');
			Exportalibrary('lapack');
			Exportalibrary('libf2c');
		end
	
		chdir(DirCur);
	end
end
endfunction
//-------------------------------------------------------------------
function bOK=Exportalibrary(libraryname)
  printf('\nExports from '+libraryname+'.dll\n');
	unix('pedump /exp '+libraryname+'.dll >'+TMPDIR+'\'+libraryname+'.lcc');
	printf('Converting Library');
	
	fw=mopen(TMPDIR+'\'+libraryname+'.exp',"w");
	fr=mopen(TMPDIR+'\'+libraryname+'.lcc',"r");
	
	if (meof(fr) == 0) then 
		line=mfscanf(1,fr,"%s");
		mfprintf(fw,"%s\n",line);
		printf('.');
	end

	while ( meof(fr) == 0)
		line=mfscanf(1,fr,"%s");
		if (line ~= []) then
			mfprintf(fw,"_%s\n",line);
		end
	end

	mclose(fw);
	mclose(fr);
	
	printf('\nBuild '+libraryname+'.lib (Please waiting).\n');
	command='buildLib ""'+TMPDIR+'\'+libraryname+'.exp""'+' ""'+WSCI+'\bin\'+libraryname+'lcc.lib""';
	unix(command);
	bOK=%T;
endfunction
//-------------------------------------------------------------------
function ExportAtlasLibrary()
	
	printf('\nExports from atlas.dll\n')
	unix('pedump /exp atlas.dll >'+TMPDIR+'\atlas.lcc');
	printf('Converting Library');
	
	fw=mopen(TMPDIR+'\atlaslcc.exp',"w");
	fr=mopen(TMPDIR+'\atlas.lcc',"r");

	if (meof(fr) == 0) then 
		line=mfscanf(1,fr,"%s");
		mfprintf(fw,"atlas.dll\n");
	end

	i=1;
	while ( meof(fr) == 0)
		line=mfscanf(1,fr,"%s");
		if (line ~= []) then
			mfprintf(fw,"_%s_  %s @%d\n",line,line,i);
			i=i+1;
			end
	end

	mclose(fw);
	mclose(fr);
	
	printf('\nBuild Atlaslcc.lib\n');
	unix('buildLib ""'+TMPDIR+'\Atlaslcc.exp""'+' ""'+WSCI+'\bin\'+'Atlaslcc.lib""');
endfunction
//-------------------------------------------------------------------
