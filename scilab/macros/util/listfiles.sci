function files= listfiles(paths,flag,flagexpand)
// list the files in a set of directories 
// each path can be ended with a wildcard name 
// path is a string matrix 
// if flagexpand is %t then SCI, HOME ~ are expanded 
// if flag is %t then paths is converted according to 
//    MSDOS value 
// when flag is %t according to the 
// value of flagexpand SCI, HOME ~ are expanded 
// or not 
// 
// Copyright Enpc 
  
  [lhs,rhs]=argn(0) 
  if rhs <= 1 then flag = %t ; end 
  if rhs <= 2 then flagexpand = %t ; end 
  
  if flag == %t then 
    paths = pathconvert(paths,%f,flagexpand); 
  end 
  
  paths= strcat(paths,' ');
  
  if MSDOS then
    files=unix_g('dir /B /OD '""+paths);
    files=files($:-1:1)
  else
    files=unix_g('ls  -t1 '+paths);
  end
endfunction

