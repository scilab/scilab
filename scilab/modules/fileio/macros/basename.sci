function [files]= basename(files,flag,flagexpand) 
// strip directory and suffix from filenames
// if flag == %t files are first converted 
// according to MSDOS 
// Copyright Enpc 
  
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
      fname = part(fname,pref_pos($) +1:length(fname)); 
    end
    suf_pos = strindex(fname,'.'); 
    if suf_pos <> [] then 
      fname =  part(fname,1:suf_pos($)-1);
    end
    files(i)= fname; 
  end
endfunction

