// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006 - INRIA - Allan CORNET
// Copyright (C) 2008 - DIGITEO - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function bOK = configure_pvm()

  bOK = %F;
  
  if %pvm then 
    if MSDOS then
      if getenv('PVM_ROOT','') == '' then 
        setenv("PVM_ROOT",pathconvert(SCI,%f,%f,'w')+"\modules\pvm\pvm3");
      end
      
      if getenv('PVM_ARCH','') == '' then
        if win64() then
          setenv("PVM_ARCH","WIN64");
        else
          setenv("PVM_ARCH","WIN32");
        end
      end
      
      if getenv('PVM_TMP','') == '' then
        setenv("PVM_TMP",getenv("TEMP"));
      end
      
      if getenv('PVM_RSH','') == '' then
        SystemRoot=getenv('SystemRoot','ndef');
        if (SystemRoot<>'ndef') then
          if fileinfo(SystemRoot+'\system32\rsh.exe') <> [] then
            setenv('PVM_RSH',SystemRoot+'\system32\rsh.exe') ;
          else
            // PVM_RSH must be defined by user
            // setenv('PVM_RSH','') ;
          end
        end
      end
      
      if ~isdef('home') then
        home = getenv('HOME',SCI);
      end
      
      if fileinfo(home+'/.pvmd.conf') == [] then
        pcname = getenv('COMPUTERNAME','');
        pvm_root = getenv('PVM_ROOT');
        pvm_arch = getenv('PVM_ARCH');
        conf = pcname + ' dx='+getshortpathname(pvm_root)+'\lib\' + pvm_arch + '\pvmd3.exe ep=' + SCI + '/bin;' + SCI + '/module/pvm/pvm3/bin/' + pvm_arch;
        try
          mputl(conf, home+'/.pvmd.conf');
        catch
          mputl(conf, SCI+'/.pvmd.conf');
        end
      end
      
      bOK = %T;
      
    else
    
    end
  end
endfunction
//------------------------------------
