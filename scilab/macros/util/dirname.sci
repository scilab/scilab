function [files]= dirname(files,flag,flagexpand) 
// return the dirnames of files 
// if flag == %t files are first converted 
// according to MSDOS. if flagexpand is also true 
// HOME SCI or ~ are also expanded. 
// Copyright Enpc 
// the returned directory names are not terminated by sep 
  [lhs,rhs]=argn(0) 
  if rhs <= 1 then flag = %t ; end 
  if rhs <= 2 then flagexpand = %t ; end 
  
  if flag == %t then 
    files = pathconvert(files,%f,flagexpand); 
  end 
  
  if MSDOS then 
    sep = '\' 
  else
     sep = '/' 
  end
  
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

