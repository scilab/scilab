function [ok,name,nx,nin,nout,ng]=compile_modelica(fil)
// Serge Steer 2003, Copyright INRIA
  ng=0
  if exists('modelica_libs')==0 then 
    message('Variable modelica_libs which gives path of modelica library is not set')
    return
  end
  if exists('modelicac_path')==0 then 
    message('Variable modelicac_path which gives path of modelica compiler is not set')
    return
  end
  
  mlibs=pathconvert(modelica_libs,%f,%t)
  if MSDOS then
    modelicac=pathconvert(modelicac_path+'/modelicac.exe',%f,%t)
  else
     modelicac=pathconvert(modelicac_path+'/modelicac',%f,%t)
  end
  modelicac=modelicac+strcat(' -L '+mlibs)
  
  name=basename(fil)
  path=strsubst(stripblanks(fil),name+'.mo','')
  
  
  //do not update C code if needcompile==0 this allows C code
  //modifications for debugging purposes  
  updateC=needcompile <>0|fileinfo(path+name+'.c')==[]
  
  if updateC then
    if unix(modelicac+' '+fil+' -o '+path+name+'.c')<>0 then
      x_message('Modelica compiler error, sorry ');
      ok=%f,nx=0,nin=0,nout=0,ng=0;return
    end
  end
   //adding trace info
  txt=mgetl(path+name+'.c')
  [nx,nin,nout,ng]=analyze_c_code(txt) //to get the dimension of the state
  if updateC then
    txt=modify1(txt,nx)
    mputl(txt,path+name+'.c')
  end
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

function [nx,nin,nout,ng]=analyze_c_code(txt)
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

endfunction

function txt=modify1(txt,nx)
// Serge Steer 2003, Copyright INRIA
  //handling includes for modelica external functions
  I=grep(txt,'#include ""')
  includes=strsubst(strsubst(txt(I),'#include ""',''),'""','')
  if includes<>[] then
    txt(I)=[]
    mlibs=modelica_libs();
    hfiles=listfiles(mlibs+'*.h')
    del=[]
    for k=1:size(hfiles,'*')
      if strindex(hfiles(k),includes)==[] then del=[del k],end
    end
    hfiles(del)=[]
  else
    hfiles=[]
  end
  includes='#include <scicos/scicos_block.h>'
  if hfiles<>[] then includes=[includes;'#include ""'+hfiles+'""'],end
  k=grep(txt,'int* p')
  txt(k:k+1)=[ascii(9)+'int p['+string(nx)+'];';' ']
  T=txt(grep(txt,ascii(9)+'res['+string(0:9)));
  T=T(grep(T,'xd['+string(0:9)));
  I=[]
  for k=1:size(T,1)
    p=strindex(T(k),'xd[');
    p1=strindex(T(k),']');
    for i=p
      ind=evstr(part(T(k),i+3:p1(min(find(p1>i+3)))-1));
      I=[I,ind];
    end
  end
  val=-ones(nx,1)
  if I<>[] then val(I+1)=1,end

  k=grep(txt,'(flag == 6 || flag == 7) ')
  txt=[includes
       ' '
       txt(1:k-1)
       ascii(9)+'} else if (flag == 7) {'
       ascii(9)+ascii(9)+'p['+string(0:nx-1)'+']='+string(val)+';'
       ascii(9)+ascii(9)+'set_pointer_xproperty(p);'
       txt(k+1:$)]

endfunction
