function  ok=modelicac(Flat,Flat_functions,xmlfileTMP,Jacobian,Cfile,with_gui)
//Scilab interface  with external tool modelicac.exe
  incidence=''
  tmpdir=pathconvert(TMPDIR,%t,%t);  //for error log and  shell scripts
  xmlfileTMP=pathconvert(xmlfileTMP,%f,%t);  
  Cfile=pathconvert(Cfile,%f,%t);  

  if fileinfo(Flat_functions)==[] then,
    Flat_functions=" "; 
  else
    Flat_functions='""'+Flat_functions+'""';
  end
  
  if ((with_gui)& (fileinfo(xmlfileTMP)<>[])) then // if GUI is running
    XMLfiles=' -with-init-in ""'+xmlfileTMP+'"" -with-init-out ""'+xmlfileTMP+'""';
  else
    XMLfiles='';
  end 
  exe='""'+pathconvert(SCI+'/bin/modelicac.exe',%f,%t)+'""'
  Flat='""'+Flat+'""'
  out='-o ""'+Cfile+'""'
  if Jacobian then, JAC=' -jac '; else, JAC=''; end
  Errfile= '>""'+tmpdir+'S_modelicac.err""'
  instr=strcat([exe,Flat,Flat_functions,XMLfiles,out,JAC,Errfile],' ')
  
  if MSDOS then,   
    mputl(instr,tmpdir+'genm2.bat'), 
    instr=tmpdir+'genm2.bat';
  end

  if execstr('unix_s(instr)','errcatch')<>0 then  
    messagebox([_('-------Modelica compiler error (with the translator):-------');
		mgetl(Errfile);
		_('Please read the error message in the Scilab window')],'error','modal');	    
    ok=%f,
    return
  end     
  //S. Steer: modelicac produce incorrect include path for Scilab5 fix it
  //the modelicac code has to be changed when available
  mputl(strsubst(mgetl(Cfile),'scicos/scicos_block.h','scicos_block.h'), Cfile)
endfunction
