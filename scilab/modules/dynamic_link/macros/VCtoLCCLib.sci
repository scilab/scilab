//-------------------------------------------------------------------
// Allan CORNET INRIA 2004 - 2006
// Export LibScilab for LCC compiler
//-------------------------------------------------------------------
function VCtoLCCLib()
if with_lcc()==%t then
	if MSDOS==%t then
		DirCur=pwd();
		chdir(WSCI+'\bin');
		mkdir('lcclib');
	
		Exportalibrary('LibScilab');
		ExportAtlasLibrary();
		Exportalibrary('lapack');
		Exportalibrary('scicos');
		Exportalibrary('libf2c');
		Exportalibrary('intersci');
	
		chdir(DirCur);
	end
end
endfunction
//-------------------------------------------------------------------
function bOK=Exportalibrary(libraryname)
  printf('\nExports from '+libraryname+'.dll\n');
	unix('pedump /exp '+libraryname+'.dll >'+TMPDIR+filesep()+libraryname+'.lcc');
	printf('Converting Library');
	
	fw=mopen(TMPDIR+filesep()+libraryname+'.exp',"w");
	fr=mopen(TMPDIR+filesep()+libraryname+'.lcc',"r");
	
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
	command='buildLib ""'+TMPDIR+filesep()+libraryname+'.exp""'+' ""'+WSCI+'\bin\lcclib\'+libraryname+'.lib""';
	unix(command);
	bOK=%T;
endfunction
//-------------------------------------------------------------------
function ExportAtlasLibrary()
	
	printf('\nExports from blasplus.dll\n')
	unix('pedump /exp blasplus.dll >'+TMPDIR+'\blasplus.lcc');
	printf('Converting Library');
	
	fw=mopen(TMPDIR+'\blaspluslcc.exp',"w");
	fr=mopen(TMPDIR+'\blasplus.lcc',"r");

	if (meof(fr) == 0) then 
		line=mfscanf(1,fr,"%s");
		mfprintf(fw,"blasplus.dll\n");
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
	
	printf('\nBuild Atlas.lib\n');
	unix('buildLib ""'+TMPDIR+'\Atlaslcc.exp""'+' ""'+WSCI+'\bin\lcclib\'+'blasplus.lib""');
endfunction
//-------------------------------------------------------------------
