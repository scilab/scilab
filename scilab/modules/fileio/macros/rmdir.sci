// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Allan CORNET
// ...
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//------------------------------------------------------------------------
// rmdir remove a directory
function [status,msg]=rmdir(varargin)
  lhs=argn(1);   
  rhs=argn(2);
  
  DirName = '';
  status = 0;
  msg = '';
  SubDirMode = %F;
  
  select rhs
    case 0
     error('Number of parameters incorrect.');	
    break
    case 1
      DirName = varargin(1);
    break
    case 2
      DirName = varargin(1) ;
      SubDir = convstr(varargin(2),'u');
      if (SubDir == 'S') then
        SubDirMode = %T;
      else
        error('Second parameters incorrect.');
      end
    break
  else
     error('Number of parameters incorrect.');
  end
  
  if ~SubDirMode then
    if findfiles(DirName)<>[] then 
      status = 0
      msg = 'Error : The directory is not empty.'
    else
      [status,msg] = hidden_rmdir(DirName);
    end
  else
  	[status,msg] = hidden_rmdir(DirName);
  end   
endfunction
//------------------------------------------------------------------------
function [status,msg]=hidden_rmdir(DirName)
  status = 0;
  msg = '';
  
  if isdir(DirName) then
    bOK = removedir(DirName);
    if bOK then
      msg = '';
      status = 1;
    else
      msg = 'Error : Undefined error.';
      status = 0;
    end
  else
    msg = 'Error : The system cannot find the file specified.'; 
    status = 0;
  end
  
endfunction
//------------------------------------------------------------------------
