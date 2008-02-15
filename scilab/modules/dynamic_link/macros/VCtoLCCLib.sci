//==========================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//==========================================
// Export Libraries of Scilab for LCC compiler
//==========================================
function VCtoLCCLib()
  // LCC-Win32 only on Windows
  if (findlcccompiler() == %T) then
		DirCur = pwd();
		chdir(WSCI+'\bin');
		mkdir('lcclib');

		Exportalibrary('LibScilab');
		Exportalibrary('libf2c');
		ExportAtlasLibrary();
		Exportalibrary('lapack');
		Exportalibrary('scicos');
		Exportalibrary('intersci');
		Exportalibrary('dynamic_link');
		Exportalibrary('scioutput_stream');
		Exportalibrary('MALLOC');
		Exportalibrary('libintl');
	
		chdir(DirCur);
	end

endfunction
//==========================================
function bOK = Exportalibrary(libraryname)
  printf('\nExports from '+libraryname+'.dll\n');
	unix('pedump /exp '+libraryname+'.dll >'+TMPDIR+filesep()+libraryname+'.lcc');
	printf('Converting Library');
	
	fw = mopen(TMPDIR+filesep()+libraryname+'.exp',"w");
	fr = mopen(TMPDIR+filesep()+libraryname+'.lcc',"r");
	
	if (meof(fr) == 0) then 
		line = mfscanf(1,fr,"%s");
		mfprintf(fw,"%s\n",line);
		printf('.');
	end

	while ( meof(fr) == 0)
		line = mfscanf(1,fr,"%s");
		if (line ~= []) then
			mfprintf(fw,"_%s\n",line);
		end
	end

	mclose(fw);
	mclose(fr);
	
	printf('\nBuild '+libraryname+'.lib (Please waiting).\n');
	command = 'buildLib ""'+TMPDIR+filesep()+libraryname+'.exp""'+' ""'+WSCI+'\bin\lcclib\'+libraryname+'.lib""';
	unix(command);
	bOK=%T;
endfunction
//==========================================
function ExportAtlasLibrary()
	
	printf('\nExports from blasplus.dll\n')
	unix('pedump /exp blasplus.dll >'+TMPDIR+'\blasplus.lcc');
	printf('Converting Library');
	
	fw = mopen(TMPDIR+'\blaspluslcc.exp',"w");
	fr = mopen(TMPDIR+'\blasplus.lcc',"r");

	if (meof(fr) == 0) then 
		line = mfscanf(1,fr,"%s");
		mfprintf(fw,"blasplus.dll\n");
	end

	i=1;
	// TO DO : update this part
	// see MKL compatibility
	while ( meof(fr) == 0)
		line = mfscanf(1,fr,"%s");
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
//==========================================
