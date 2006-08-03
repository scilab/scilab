//------------------------------------
// Allan CORNET INRIA 2006
//------------------------------------
function bOK=configure_pvm()
  bOK=%F;
  if %pvm then 
    if MSDOS then
      setenv("PVM_ROOT",pathconvert(SCI,%f,%f,'w')+"\modules\pvm\src\pvm3");
      setenv("PVM_ARCH","WIN32");
      setenv("PVM_TMP",getenv("TEMP"));
      NT=getenv('OS','ndef');
      if (NT<>'ndef') then
        SystemRoot=getenv('SystemRoot','ndef');
        if (SystemRoot<>'ndef') then
          setenv('PVM_RSH',SystemRoot+'\system32\rsh.exe') ;
        end
      end
      bOK=%T;
    else
    end
  end
endfunction
//------------------------------------
