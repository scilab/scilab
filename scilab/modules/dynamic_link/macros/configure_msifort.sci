//==========================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//==========================================
function bOK = configure_msifort()

//==========================================
//  functions defined only in configure_ifort
//==========================================
function ifpath = getIFpath(ifversion)
  select ifversion ,
    case 'ifort9' then
       ifpath = getenv('IFORT_COMPILER9','');
    case 'ifort10' then
       ifpath = getenv('IFORT_COMPILER10','');
	  else
      ifpath = '';
    end
endfunction
//==========================================
function bOK = set_commons_msifort(ifpath,machinepath)
  bOK = %F;
 
  // example set PATH=%IFORT_COMPILER10%\EM64T\Bin;
  ENV_PATH = getenv('PATH','');
  PATH_TO_ADD = ifpath + machinepath + filesep() + 'bin';
  if isdir(PATH_TO_ADD) then
    ENV_PATH = PATH_TO_ADD + pathsep() + ENV_PATH;
    err = setenv('PATH',ENV_PATH);
    if (err == %F) then bOK = %F,return,end
    bOK = %T;
  else
    bOK = %F;
    return;
  end
  
  // example set INCLUDE=%IFORT_COMPILER10%\EM64T\Include;
  ENV_INCLUDE = getenv('INCLUDE','');
  PATH_TO_ADD = ifpath + machinepath + filesep() + 'Include';
  if isdir(PATH_TO_ADD) then
    ENV_INCLUDE = PATH_TO_ADD + pathsep() + ENV_INCLUDE;
    err = setenv('INCLUDE',ENV_INCLUDE);
    if (err == %F) then bOK = %F,return,end
    bOK = %T;
  else
    bOK = %F;
    return;
  end
  
  // example set LIB=%IFORT_COMPILER10%\EM64T\Lib;
  ENV_LIB = getenv('LIB','');
  PATH_TO_ADD = ifpath + machinepath + filesep() + 'Lib';
  if isdir(PATH_TO_ADD) then
    ENV_LIB = PATH_TO_ADD + pathsep() + ENV_LIB;
    err = setenv('LIB',ENV_LIB);
    if (err == %F) then bOK = %F,return,end
    bOK = %T;
  else
    bOK = %F;
    return;
  end
  
endfunction
//==========================================

  bOK = %F;
  if MSDOS then
    ifortcompiler = findmsifortcompiler();
    if ifortcompiler <> 'unknown' then
      if_path = getIFpath(ifortcompiler);
      
      if win64() & detectmsifort64tools() then
        set_commons_msifort(if_path,'EM64T');
      else
        set_commons_msifort(if_path,'IA32');
      end
      
    end
  end

endfunction

//==========================================

