// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006 - INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
