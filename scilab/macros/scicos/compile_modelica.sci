function [ok,name,nx,nin,nout,ng,nm,nz]=compile_modelica(fil)
// Serge Steer 2003, Copyright INRIA
  
  if ~with_modelica_compiler() then
    message('Modelica compiler unavailable')
    ok=%f,name='',nx=0,nin=0,nout=0,ng=0,nm=0,nz=0
    return
  end
  
  ng=0
  fil=pathconvert(fil,%f,%t)
  mlibs=pathconvert(modelica_libs,%f,%t)
  
  name=basename(fil)
  path=strsubst(stripblanks(fil),name+'.mo','')

  //do not update C code if needcompile==0 this allows C code
  //modifications for debugging purposes  
  updateC=needcompile <>0|fileinfo(path+name+'.c')==[]

  if updateC then
    if MSDOS then      
      if fileinfo(SCI+'/bin/translator.exe')<>[] then 	 
	translator=pathconvert(SCI+'/bin/translator.exe',%f,%t)
	ext='\*.mo';
	molibs=[];
	for k=1:size(mlibs,'*')
	  molibs=[molibs;listfiles(mlibs(k)+ext)];
	end
	txt=[];
	for k=1:size(molibs,'*')
	  [pathx,fnamex,extensionx]=fileparts(molibs(k));
	  if (fnamex<>'MYMOPACKAGE') then 
	    txt=[txt;mgetl(molibs(k))];
	  end
	end
	mputl(txt,TMPDIR+'/MYMOPACKAGE.mo');
	// all Modelica files found in "modelica_libs" directories are
        // merged into the  temporary file  "MYMOPACKAGE.mo". This is done
        // because in Windows the length of the command line is limited.
	
	translator=translator+strcat(' -lib '+ TMPDIR+'/MYMOPACKAGE.mo');
        //translator=translator+strcat(' -lib '+molibs);
	instr=translator+' -lib '+fil+' -o '+path+name+'_flattened.mo"+" -"+...
	      "command ""'+name+' x;"" > '+TMPDIR+'/Wtranslator.err';        
	mputl(instr,path+'gent.bat')
	instr=path+'gent.bat'      
	if execstr('unix_s(instr)','errcatch')<>0 then
	  x_message(['Modelica translator error:'
		     mgetl(TMPDIR+'/Wtranslator.err');
		     'Please look up errors in the indicated file"+...
		     " and correct them in your Modelica file ']);
	  ok=%f,nx=0,nin=0,nout=0,ng=0;nz=0;return
	end
	FlatName=path+name+'_flattened.mo';
      else
	FlatName=fil;
      end           
      modelicac=pathconvert(SCI+'/bin/modelicac.exe',%f,%t)     
      if strindex(modelicac,' ')<>[] then modelicac='""'+modelicac+'""',end
      modelicac=modelicac+strcat(' -L ""'+mlibs+'""')
      instr=modelicac+' '+FlatName+' -o '+path+name+'.c"+" -jac';           
      mputl(instr,path+'genc.bat')
      instr=path+'genc.bat'
    else //==================Unix/Linux/Mac....
      if fileinfo(SCI+'/bin/translator')<>[] then 
	translator=pathconvert(SCI+'/bin/translator',%f,%t)
	ext='/*.mo';
	molibs=[];
	for k=1:size(mlibs,'*')
	  molibs=[molibs;listfiles(mlibs(k)+ext)];
	end
	
	txt=[];
	for k=1:size(molibs,'*')
	  [pathx,fnamex,extensionx]=fileparts(molibs(k));
	  if (fnamex<>'MYMOPACKAGE') then 
	    txt=[txt;mgetl(molibs(k))];
	  end
	end
	mputl(txt,TMPDIR+'/MYMOPACKAGE.mo');
        // all Modelica files found in "modelica_libs" directories are
        // merged into the  temporary file  "MYMOPACKAGE.mo". This is done
        // because in Windows the length of the command line is limited.
	translator=translator+strcat(' -lib '+ TMPDIR+'/MYMOPACKAGE.mo');	
	//translator=translator+strcat(' -lib '+molibs);
	
	instr=translator+' -lib '+fil+' -o '+path+name+'_flattened.mo"+" -"+...
	      "command ""'+name+' x;"" > '+TMPDIR+'/Ltranslator.err'; 

	if execstr('unix_s(instr)','errcatch')<>0 then
	  x_message(['Modelica translator error:'
		     mgetl(TMPDIR+'/Ltranslator.err');
		     'sorry ']);
	  ok=%f,nx=0,nin=0,nout=0,ng=0;nz=0;return
	end
	FlatName=path+name+'_flattened.mo';
      else
	FlatName=fil;
      end
      
      modelicac=pathconvert(SCI+'/bin/modelicac',%f,%t)
      modelicac=modelicac+strcat(' -L '+mlibs)
      instr=modelicac+' '+FlatName+' -o '+path+name+'.c"+...
	    " -jac '+' > '+TMPDIR+'/unix.err';    
    end //===========================end of IFs
    
    if execstr('unix_s(instr)','errcatch')<>0 then
      x_message(['Modelica compiler error:'
		 mgetl(TMPDIR+'/unix.err');
		 'sorry ']);
      ok=%f,nx=0,nin=0,nout=0,ng=0;nz=0;return
    end
    mprintf('   C code generated at '+path+name+'.c\n')
  end
  
  Cfile=path+name+'.c'
  if MSDOS then Ofile=path+name+'.obj', else Ofile=path+name+'.o', end
  
  //get the Genetrated block properties
  [nx,nin,nout,ng,nm,nz]=analyze_c_code(mgetl(Cfile)) 
  
  mprintf('\n\r Modelica blocks are reduced to a block with:');
  mprintf('\n\r Number of continuous time states: %d',nx);
  mprintf('\n\r Number of discrete time states  : %d',nz);
  mprintf('\n\r Number of zero-crossing surfaces: %d',ng);
  mprintf('\n\r Number of modes  : %d',nm);
  mprintf('\n\r Number of inputs : %d',nin);
  mprintf('\n\r Number of outputs: %d',nout);
  mprintf('\n\r ');

  //below newest(Cfile,Ofile) is used instead of  updateC in case where
  //Cfile has been manually modified (debug,...)
  if newest(Cfile,Ofile)==1 then
    //unlink if necessary
    [a,b]=c_link(name); while a ; ulink(b);[a,b]=c_link(name);end
    //  build the list of external functions libraries

    // remove repreated directories from mlibs
    rep=[];
    for k=1:size(mlibs,'*')
      for j=k+1:size(mlibs,'*')
        if stripblanks(mlibs(k))==stripblanks(mlibs(j)) then rep=[rep,j]; end
      end
    end
    mlibs(rep)=[];
    //--------------------------------
    libs=[];
    if MSDOS then ext='\*.ilib',else ext='/*.a',end 
    // removing .a or .ilib sufixs
    for k=1:size(mlibs,'*')
      aa=listfiles(mlibs(k)+ext);
      for j=1:size(aa,'*')
        [pathx,fnamex,extensionx]=fileparts(aa(j));
        libsname= fullfile(pathx,fnamex);
        libs=[libs;libsname];
      end
    end

    // add modelica_libs to the list of directories to be searched for *.h
    //if MSDOS then ext='\*.h',else ext='/*.h',end
    EIncludes=''
    for k=1:size(mlibs,'*')
      EIncludes=EIncludes+'  -I""'+ mlibs(k)+'""';
    end

    E2='';
    for i=1:length(EIncludes)
      if (part(EIncludes,i)=='\') then
        E2=E2+'\';
      end
      E2=E2+part(EIncludes,i);
    end

    //** build shared library with the C code

    // files=name+'.o';Make=path+'Make'+name;loader=path+name+'.sce'
    // ierr=execstr('libn=ilib_for_link(name,files,libs,''c'',Make,loader,'''','''',E2)','errcatch')
    // if ierr<>0 then 
    //   ok=%f;x_message(['sorry compilation problem';lasterror()]);
    //   return;
    // end

    // executing loader file
    // if execstr('exec(loader); ','errcatch')<>0 then
    //   ok=%f;
    //   x_message(['Problem while linking generated code';lasterror()]);
    //   return;
    // end

    files = name;
    ok = buildnewblock(name,files,'',libs,TMPDIR,'',E2);
    if ~ok then return, end

  end
endfunction

function [nx,nin,nout,ng,nm,nz]=analyze_c_code(txt)
// Serge Steer 2003, Copyright INRIA
  match=  'number of variables = '
  T=txt(grep(txt(1:10),match))//look for match in the first 10 lines
  nx=evstr(strsubst(T,match,''))

  match=  'number of inputs = '
  T=txt(grep(txt(1:10),match))//look for match in the first 10 lines
  nin=evstr(strsubst(T,match,''))

  match=  'number of outputs = '
  T=txt(grep(txt(1:10),match))//look for match in the first 10 lines
  nout=evstr(strsubst(T,match,''))

  match=  'number of zero-crossings = '
  T=txt(grep(txt(1:10),match))//look for match in the first 10 lines
  ng=evstr(strsubst(T,match,''))

  match=  'number of modes = '
  T=txt(grep(txt(1:10),match))//look for match in the first 10 lines
  nm=evstr(strsubst(T,match,''))

  match=  'number of discrete variables = '
  T=txt(grep(txt(1:10),match))//look for match in the first 10 lines
  nz=evstr(strsubst(T,match,''))

endfunction
