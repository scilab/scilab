// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [files]= dirname(files,flag,flagexpand) 
// return the dirnames of files 
// if flag == %t files are first converted 
// according to getos() == 'Windows'. if flagexpand is also true 
// HOME SCI or ~ are also expanded. 
// the returned directory names are not terminated by sep 
  [lhs,rhs]=argn(0) 
  if rhs <= 1 then flag = %t ; end 
  if rhs <= 2 then flagexpand = %t ; end 
  
  if flag == %t then 
    files = pathconvert(files,%f,flagexpand); 
  end 

  sep = filesep();
  
  for i=1:size(files,'*')
    fname = files(i) 
    pref_pos = strindex(fname,sep); 
    if pref_pos <> [] then 
      fname = part(fname,1:pref_pos($)-1);
    else
       fname = "."; //emptystr();
    end
    files(i)= fname;
  end
endfunction

