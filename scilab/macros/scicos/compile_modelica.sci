function [ok,name,nx,nin,nout,ng,nm]=compile_modelica(fil)
// Serge Steer 2003, Copyright INRIA
  
  if ~with_modelica_compiler() then
    message('Modelica compiler unavailable')
    ok=%f,name='',nx=0,nin=0,nout=0,ng=0,nm=0
    return
  end
  
  ng=0

  mlibs=pathconvert(modelica_libs,%f,%t)
  if MSDOS then
    modelicac=pathconvert(SCI+'/bin/modelicac.exe',%f,%t)
  else
     modelicac=pathconvert(SCI+'/bin/modelicac',%f,%t)
  end
  modelicac=modelicac+strcat(' -L '+mlibs)
  
  name=basename(fil)
  path=strsubst(stripblanks(fil),name+'.mo','')
  
  
  //do not update C code if needcompile==0 this allows C code
  //modifications for debugging purposes  
  updateC=needcompile <>0|fileinfo(path+name+'.c')==[]
  if updateC then
    
    if execstr('unix_s(modelicac+'' ''+fil+'' -o ''+path+name+''.c'')','errcatch')<>0 then
      
      x_message(['Modelica compiler error:'
		 mgetl(TMPDIR+'/unix.err');
		 'sorry ']);
      ok=%f,nx=0,nin=0,nout=0,ng=0;return
    end
  end
   //adding trace info
  txt=mgetl(path+name+'.c')
  [nx,nin,nout,ng,nm]=analyze_c_code(txt) //to get the dimension of the state
					  
  //if updateC then txt=modify1(txt,nx); mputl(txt,path+name+'.c');end
  
  //unlink if necessary
  [a,b]=c_link(name); while a ; ulink(b);[a,b]=c_link(name);end

 
  // build shared library with the C code
  files=name+'.o';Make=path+'Make'+name;loader=path+name+'.sce'
  ierr=execstr('libn=ilib_for_link(name,files,[],''c'',Make,loader)','errcatch')
  if ierr<>0 then 
    ok=%f;x_message(['sorry compilation problem';lasterror()]);
    return;
  end

  // link the generated C code
  if execstr('link(libn,name,''c'')','errcatch')<>0 then 
    ok=%f;
    x_message(['Problem while linking generated code';lasterror()]);
    return;
  end
endfunction

function [nx,nin,nout,ng,nm]=analyze_c_code(txt)
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

endfunction

function txt=modify1(txt,nx)

endfunction
