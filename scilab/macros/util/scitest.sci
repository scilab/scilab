function scitest(tstfile,force,error_check,keep_prompt,postscript_graphics)
// if error_check is %t then execution errors are reported 
// if error_check is %f then only test checked error are detected 
// (see examples in tests directory) 
// if keep_prompt is %t the the prompt is kept in the dia file 
// this can be useful for producing demos 
  [lhs,rhs]=argn(0);
  if exists('force','local')==0 then force = %f ; end 
  if exists('error_check','local')==0 then error_check = %f ; end 
  if exists('keep_prompt','local')==0 then keep_prompt = %f ; end 
  if exists('postscript_graphics','local')==0 then postscript_graphics = %f ; end 

  ind=strindex(tstfile,"."); 
  if ind==[] then 
    diafile = tstfile+'.tst'
  else
     suf = part(tstfile,ind($):length(tstfile));
     if suf == '.dia' then 
       error('scitest: first argument should not be a file with .dia suffix" );
       return 
     end
     diafile=strsubst(tstfile,suf,'.dia')
  end
  if newest(tstfile,diafile)==2 & force == %f  then return,end
  
  mydisp('------------------- File '+tstfile+'--------------------')
  //Reset standard globals
  rand('seed',0);rand('uniform');
  if MSDOS then
    SCI=strsubst(SCI,'/','\')
    tmpfiles=strsubst(TMPDIR,'/','\')+'\tmp.'
  else
     tmpfiles=TMPDIR+'/tmp.'
  end

  // Do some modification in  tst file 
  // ---------------------------------
  txt=mgetl(tstfile)
  txt=strsubst(txt,'pause,end','bugmes();quit;end')
  txt=strsubst(txt,'-->','@#>') 
  //to avoid suppression of input --> with prompts
  txt=strsubst(txt,'halt()','');
 
  if postscript_graphics then 
    pg1='driver(''Pos'');xinit('''+tmpfiles+'gr'+''');',
    pg2='xend();'
  else
    pg1=''
    pg2=''
  end
  head='mode(3);clear;lines(28,72);lines(0);'..
       +'deff(''[]=bugmes()'',''write(%io(2),''''error on test'''')'');'..
       +'predef(''all'');'..
       +'tmpdirToPrint = msprintf(''TMPDIR1=''''%s''''\n'',TMPDIR);'..
       +'diary('''+tmpfiles+'dia'+''');'..
       +'write(%io(2),tmpdirToPrint);'..
       +pg1;
  
  tail="diary(0);"+pg2+"exit;"
  
  txt=[head;
       txt;
       tail];
  // and save it in a temporary file 
  mputl(txt,tmpfiles+'tst')
  
  myexec()

  //  Do some modification in  dia file 
  // ----------------------------------
  dia=mgetl(tmpfiles+'dia')
  dia(grep(dia,'exec('))=[];
  TMP=dia(1);dia(1)=[]
  dia(grep(dia,'diary(0)'))=[];
  
  execstr(TMP)
  
  dia=strsubst(dia,TMPDIR,'TMPDIR');
  dia=strsubst(dia,TMPDIR1,'TMPDIR');
  dia=strsubst(dia,TMPDIR1,'TMPDIR');
  dia=strsubst(dia,SCI,'SCI');

  //suppress the prompts
  if keep_prompt == %f then 
    dia=strsubst(dia,'-->','') 
  end
  dia=strsubst(dia,'@#>','-->')
  dia=strsubst(dia,'-1->','')
  
  //standardise  number display   
  dia=strsubst(strsubst(strsubst(strsubst(dia,' .','0.'),..
				 'E+','D+'),'E-','D-'),'-.','-0.')
  //not to change the ref files
  dia=strsubst(dia,'bugmes();return','bugmes();quit');
  // write down the resulting dia file
  mputl(dia,diafile)
  //Check for execution errors
  // -------------------------
  
  if  error_check == %t then 
    if grep(dia,'!--error')<>[] then 
      mydisp("Test failed ERROR DETECTED  while executing "+tstfile)
      return 
    end
  end   
  
  if grep(dia,'error on test')<>[] then
    mydisp("Test failed ERROR DETECTED  while executing "+tstfile)
    return
  end
  //Check for diff with the .ref file
  // --------------------------------
  [u,ierr]=mopen(diafile+'.ref','r')
  if ierr== 0 then //ref file exists
    ref=mgetl(u);mclose(u)
    // suppress blank (diff -nw)
    dia=strsubst(dia,' ','')
    ref=strsubst(ref,' ','')
    
    if or(ref<>dia) then 
      if MSDOS then
	mydisp('Test Failed SEE : fc /L /N  '+diafile+' '+diafile+'.ref ')
      else
	 mydisp('Test Failed SEE : diff -w  '+diafile+' '+diafile+'.ref ')
      end
    else
       mydisp('Test passed')
    end
  end
  mydisp('----------------------------------------------------------')
endfunction

function mydisp(str)
//write(result,str,'(a)')
  write(%io(2),str,'(a)')
endfunction

function myexec()
  if MSDOS then
      if fileinfo(tmpfiles+'dia')<>[] then  
	unix_s('del '+tmpfiles+'dia')
      end
      unix_s('""'+SCI+'\bin\scilex.exe'+'""'+' -nwni -args -nouserstartup < '+tmpfiles+'tst')
  else
     if fileinfo(tmpfiles+'dia')<>[] then  
       unix_s('rm -f '+tmpfiles+'dia')
     end
     unix_s('( '+SCI+'/bin/scilab -nw -args -nouserstartup <'+tmpfiles+'tst > '+tmpfiles+'res ) 2> '+tmpfiles+'err')
  end
endfunction
