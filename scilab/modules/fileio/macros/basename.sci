// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) - 2009 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [files]= basename(files,flag,flagexpand) 
// strip directory and suffix from filenames
// if flag == %t files are first converted
// according to MSDOS

  [lhs,rhs]=argn(0);
  
  if (files == []) | (files == '') then
    return
  end
  
  if type(files) <> 10 then
    error(999, msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"basename",1) );
  end
  
  if rhs <= 1 then flag = %t ; end 
  if rhs <= 2 then flagexpand = %t ; end 
  
  if (type(flag) <> 4) then
    error(999, msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"basename",2));
  end

  if (type(flagexpand) <> 4) then
    error(999, msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"basename",3));
  end

  if flag == %t then 
    files = pathconvert(files,%f,flagexpand); 
  end 

  sep = filesep();

  for i=1:size(files,'*')
    if files(i) <> '' then
      files(i) = fileparts(files(i),'fname');
    end
  end
endfunction

