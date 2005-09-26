function files= listfiles(paths,flag,flagexpand)
// list the files in a set of directories 
// each path can be ended with a wildcard name 
// path is a string matrix 
// if flagexpand is %t then SCI, HOME ~ are expanded 
// if flag is %t then paths is converted according to  MSDOS value 
// when flag is %t according to the 
// value of flagexpand SCI, HOME ~ are expanded 
// or not 
// 
// Copyright Enpc 
  
  [lhs,rhs]=argn(0) 
  if rhs < 1 then paths='./' ; end 
  if rhs <= 1 then flag = %t ; end 
  if rhs <= 2 then flagexpand = %t ; end 
  
  if flag == %t then 
    paths = pathconvert(paths,%f,flagexpand); 
  end 
  
  //redefining  disp to avoid message when no file are found
  prot=funcprot();funcprot(0);deff('disp(txt)',' ');funcprot(prot)
  files=[];

  if MSDOS then
    for i=1:size(paths,'*') 
      // dir returns names without the dirname 
      filesi=unix_g('dir /B /OD ""'+paths(i)+'""');
      if filesi == "" then filesi=[],end
      if filesi<>[] then 
	filesi=filesi($:-1:1)
	// prepend with the path, if required 
	// when listing a full directory, path is not prepended
	dirs=paths(i)
	if part(dirs,length(dirs))<>'\' then
	  if isdir(dirs) then //yes
	    with_dir=%f
	  else                //no
	    k=strindex(dirs,'\');
	    if k==[] then 
	      with_dir=%f
	    else
	      dirs=part(dirs,1:k($))
	      with_dir=%t
	    end
	  end
	else
	  with_dir=%f
        end
	if with_dir then 
	  filesi = dirs+filesi
	end
      end
      files=[files;filesi];
    end
  else  
    paths=strsubst(stripblanks(paths),' ','\ ')
    for i=1:size(paths,'*') 
      filesi=unix_g('ls  -t1 '+paths(i));
      if filesi== "" then filesi=[],end
      files=[files;filesi]
    end
  end
endfunction

