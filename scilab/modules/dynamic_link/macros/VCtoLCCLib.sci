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
function bOK = VCtoLCCLib()
  // LCC-Win32 only on Windows
  if (findlcccompiler() == %T) then
    bOK = [];
		DirCur = pwd();
		
	  bOK = [bOK , ExportAtlasLibrary('blasplus', WSCI+'\bin')];
		bOK = [bOK , Exportalibrary('LibScilab', WSCI+'\bin')];
		bOK = [bOK , Exportalibrary('libf2c', WSCI+'\bin')];
		bOK = [bOK , Exportalibrary('lapack', WSCI+'\bin')];
		if with_module('scicos') then
		  bOK = [bOK , Exportalibrary('scicos', WSCI+'\bin')];
		  bOK = [bOK , Exportalibrary('scicos_f', WSCI+'\bin')];
		  bOK = [bOK , Exportalibrary('scicos_blocks', WSCI+'\bin')];
		  bOK = [bOK , Exportalibrary('scicos_blocks_f', WSCI+'\bin')];
		end
		bOK = [bOK , Exportalibrary('intersci', WSCI+'\bin')];
		bOK = [bOK , Exportalibrary('dynamic_link', WSCI+'\bin')];
		bOK = [bOK , Exportalibrary('scioutput_stream', WSCI+'\bin')];
	  bOK = [bOK , Exportalibrary('MALLOC', WSCI+'\bin')];
	  bOK = [bOK , Exportalibrary('libintl', WSCI+'\bin')];
		
		chdir(DirCur);
		
		if and(bOK == %T) then
		  bOK = %t;
		end
		
	else
	  bOK = %f;
	end
  
endfunction
//==========================================
function  bOK = ExportAtlasLibrary(libraryname, pathlib)

  destPath = SCIHOME + filesep() + 'lcclib';
  mkdir(destPath);
  if chdir(pathlib) == %f then 
    bOK = %f;
    return
  end
  
  mprintf('\nExports from '+libraryname+'.dll\n');
  ierr = unix('pedump /exp '+libraryname+'.dll >'+TMPDIR+filesep()+libraryname+'.lcc');
  if ierr <> 0 then
	  bOK=%F;	
	  return;
	end
	
	mprintf('Converting Library');
	
	fw = mopen(TMPDIR+filesep()+libraryname+'.exp',"wt");
	fr = mopen(TMPDIR+filesep()+libraryname+'.lcc',"rt");
	
	if (meof(fr) == 0) then 
		line = mfscanf(1,fr,"%s");
		mfprintf(fw,"blasplus.dll\n");
	end

	i=1;
	while ( meof(fr) == 0)
		line = mfscanf(1,fr,"%s");
		if (line ~= []) then
			mfprintf(fw,"_%s_  %s @%d\n",line,line,i);
			i=i+1;
			end
	end
		
	mclose(fw);
	mclose(fr);
	
	mprintf('\n');
	mprintf(gettext('Build %s.lib (Please waiting).\n'),libraryname);
	command = 'buildLib ""'+TMPDIR+filesep()+libraryname+'.exp""'+' ""'+destPath+filesep()+libraryname+'lcc.lib""';
	ierr = unix(command);
	if ierr <> 0 then
	  bOK=%F;	
	  return;
	end
	
	if fileinfo(destPath+filesep()+libraryname+'.lib') <> [] then
	  bOK=%T;
	else
	  bOK=%F;	
	end
endfunction
//==========================================
function bOK = Exportalibrary(libraryname, pathlib)
  destPath = SCIHOME + filesep() + 'lcclib';
  mkdir(destPath);

  if chdir(pathlib) == %f then 
    bOK = %f;
    return
  end
  
  mprintf('\nExports from '+libraryname+'.dll\n');
  ierr = unix('pedump /exp '+libraryname+'.dll >'+TMPDIR+filesep()+libraryname+'.lcc');
  if ierr <> 0 then
	  bOK=%F;	
	  return;
	end
	
	mprintf('Converting Library');
	
	fw = mopen(TMPDIR+filesep()+libraryname+'.exp',"wt");
	fr = mopen(TMPDIR+filesep()+libraryname+'.lcc',"rt");
	
	if (meof(fr) == 0) then 
		line = mfscanf(1,fr,"%s");
		mfprintf(fw,"%s\n",line);
	end

	while ( meof(fr) == 0)
		line = mfscanf(1,fr,"%s");
		if (line ~= []) then
			mfprintf(fw,"_%s\n",line);
		end
	end
	
	mclose(fw);
	mclose(fr);
	
	mprintf('\n');
	mprintf(gettext('Build %s.lib (Please waiting).\n'),libraryname);
	command = 'buildLib ""'+TMPDIR+filesep()+libraryname+'.exp""'+' ""'+destPath+filesep()+libraryname+'lcc.lib""';
	ierr = unix(command);
	if ierr <> 0 then
	  bOK=%F;	
	  return;
	end
	
	if fileinfo(destPath+filesep()+libraryname+'.lib') <> [] then
	  bOK=%T;
	else
	  bOK=%F;	
	end

endfunction
//==========================================