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
  
  if MSDOS then
    dirs= dirname(paths,flag,flagexpand);
    files=[];
    for i=1:size(paths,'*') 
      // dir returns names without the dirname 
      filesi=unix_g('dir /B /OD ""'+paths(i)+'""');
      if filesi<>"" & filesi<>[] then 
	filesi=filesi($:-1:1)
	if dirs(i) <> '.' then 
	  filesi= dirs(i)+'\'+filesi
	end
      end
      files=[files;filesi];
    end
  else
    paths=  strcat(paths,' ');
    files=unix_g('ls  -t1 '+paths);
  end
endfunction

