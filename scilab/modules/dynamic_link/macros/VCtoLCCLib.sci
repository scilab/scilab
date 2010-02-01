//==========================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// Copyright (C) DIGITEO - 2009 - Allan CORNET
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
  // Bug 5433
  // VCtoLCCLib called in dynamic_link.start
  // We need to be sure that '%c_a_c' is already loaded
  bCleanVar = %f;
  if ~isdef('%c_a_c') then
    ierr = exec(SCI + '/modules/overloading/macros/%c_a_c.sci','errcatch');
	if ierr == 0 then 
	  bCleanVar = %t;
	end
  end
  
  // LCC-Win32 only on Windows
  if (findlcccompiler() == %T) then
    bOK = [];
    DirCur = pwd();
                 
    bOK = [bOK , ExportAtlasLibrary('blasplus', WSCI+'\bin')];
    
    if with_module('scicos') then
      bOK = [bOK , Exportalibrary('scicos', WSCI+'\bin',[])];
      bOK = [bOK , Exportalibrary('scicos_f', WSCI+'\bin',[])];
      bOK = [bOK , Exportalibrary('scicos_blocks', WSCI+'\bin',[])];
      bOK = [bOK , Exportalibrary('scicos_blocks_f', WSCI+'\bin',[])];
    end
                 
    DATAS_LIBSCILAB = ['LaunchScilabLock', ..
         'LaunchScilab', ..
         'adre_', ..
         'basbrk_', ..
         'basetype_', ..
         'cha1_', ..
         'com_', ..
         'dcoeff_', ..
         'errgst_', ..
         'ierode_', ..
         'intersci_', ..
         'iop_', ..
         'ls0001_', ..
         'lsa001_', ..
         'lsr001_', ..
         'recu_', ..
         'scigsch_', ..
         'scisch_', ..
         'scizgsch_', ..
         'stack_', ..
         'vstk_'];
    bOK = [bOK , Exportalibrary('LibScilab', WSCI+'\bin',DATAS_LIBSCILAB)];

    bOK = [bOK , Exportalibrary('MALLOC', WSCI+'\bin',[])];
    bOK = [bOK , Exportalibrary('libf2c', WSCI+'\bin',[])];
    bOK = [bOK , Exportalibrary('lapack', WSCI+'\bin',[])];
    bOK = [bOK , Exportalibrary('intersci', WSCI+'\bin',[])];
    bOK = [bOK , Exportalibrary('output_stream', WSCI+'\bin',[])];
    bOK = [bOK , Exportalibrary('dynamic_link', WSCI+'\bin',[])];
    bOK = [bOK , Exportalibrary('integer', WSCI+'\bin',[])];
    bOK = [bOK , Exportalibrary('optimization_f', WSCI+'\bin',[])];
    bOK = [bOK , Exportalibrary('libjvm', WSCI+'\bin',[])];
    bOK = [bOK , Exportalibrary('scilocalization', WSCI+'\bin',[])];
    bOK = [bOK , Exportalibrary('libintl', WSCI+'\bin',[])];
    bOK = [bOK , Exportalibrary('linpack_f', WSCI+'\bin',[])];
    bOK = [bOK , Exportalibrary('call_scilab', WSCI+'\bin',[])];
    bOK = [bOK , Exportalibrary('time', WSCI+'\bin',[])];
    bOK = [bOK , Exportalibrary('api_scilab', WSCI+'\bin',[])];
    bOK = [bOK , Exportalibrary('hashtable', WSCI+'\bin',[])];
    bOK = [bOK , Exportalibrary('scilab_windows', WSCI+'\bin',[])];   
  
    if bCleanVar == %t then
      clear %c_a_c;
	end

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
  
  if ( ilib_verbose() <> 0 ) then
    mprintf('\nExports from '+libraryname+'.dll\n');
  end
  ierr = unix('pedump /exp '+libraryname+'.dll >'+TMPDIR+filesep()+libraryname+'.lcc');
  if ierr <> 0 then
    bOK=%F;           
    return;
  end
  
  if ( ilib_verbose() <> 0 ) then
    mprintf('Converting Library');
  end
  
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
  
  if ( ilib_verbose() <> 0 ) then
    mprintf('\n');
    mprintf(gettext('Build %s.lib (Please waiting).\n'),libraryname);
  end
  command = 'buildLib ""'+TMPDIR+filesep()+libraryname+'.exp""'+' ""'+destPath+filesep()+libraryname+'.lib""';
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
function bOK = Exportalibrary(libraryname, pathlib,DATAS)
  destPath = SCIHOME + filesep() + 'lcclib';
  mkdir(destPath);

  if chdir(pathlib) == %f then 
    bOK = %f;
    return
  end

  if ( ilib_verbose() <> 0 ) then  
    mprintf('\nExports from '+libraryname+'.dll\n');
  end
  ierr = unix('pedump /exp '+libraryname+'.dll >'+TMPDIR+filesep()+libraryname+'.lcc');
  if ierr <> 0 then
    bOK=%F;           
    return;
  end
  
  if ( ilib_verbose() <> 0 ) then
    mprintf('Converting Library');
  end
  
  fr = mopen(TMPDIR+filesep()+libraryname+'.lcc',"rt");
  LCC = mgetl(fr);
  mclose(fr); 
  
  LCC = strsubst(LCC,'/\.$/','','r');

  LCC_LINE1 = LCC(1);
  
  LCC = '_' + LCC + ' ' + LCC;
  LCC(1) = LCC_LINE1;

  if DATAS <> [] then
    index_datas = grep(LCC,DATAS);
    for i = index_datas
      LCC(i) = LCC(i) + ' DATA';
    end
  end
  
  fw = mopen(TMPDIR+filesep()+libraryname+'.exp',"wt");
  mputl(LCC,fw);
  mclose(fw);
  
  if ( ilib_verbose() <> 0 ) then
    mprintf('\n');
    mprintf(gettext('Build %s.lib (Please waiting).\n'),libraryname);
  end
  command = 'buildLib ""'+TMPDIR+filesep()+libraryname+'.exp""'+' ""'+destPath+filesep()+libraryname+'.lib""';
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

