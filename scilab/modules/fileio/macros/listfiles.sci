function files= listfiles(paths,flag,flagexpand)
// INRIA 2007
// Allan CORNET
// list the files in a set of directories 
// files= listfiles(paths [,flag,flagexpand])
// paths : a string matrix giving a set of pathnames (eventually ended by a pattern built with  * ) 
// flag,flagexpand : boolean optional parameters. (default value  %t ). 
// files : a string matrix
  
  [lhs,rhs]=argn(0) 
  if rhs < 1 then paths='./' ; end 
  if rhs <= 1 then flag = %t ; end 
  if rhs <= 2 then flagexpand = %t ; end 
  files=[];
  
  if flag == %t then 
    paths = pathconvert(paths,%f,flagexpand); 
  end 
  
  if MSDOS then
    DIR_SEPARATOR='\';
  else
    DIR_SEPARATOR='/';
  end
  
  paths=strsubst(stripblanks(paths),' ','\')
  
  for i=1:size(paths,'*') 
    [path,fname,extension]=fileparts(paths(i));
    
    if extension == '' then extension='.*',end;
    if fname == '' then fname='*',end;
    
    filesi=findfiles(path,fname+extension);
    
    if filesi<>[] then 
	    filesi=filesi($:-1:1);
	    // prepend with the path, if required 
	    // when listing a full directory, path is not prepended
	    dirs=paths(i);
	    
	    if part(dirs,length(dirs))<>DIR_SEPARATOR then
	      if isdir(dirs) then //yes
	        with_dir=%f;
	      else                //no
	        k=strindex(dirs,DIR_SEPARATOR);
	        if k==[] then 
	          with_dir=%f;
	        else
	          dirs=part(dirs,1:k($));
	          with_dir=%t;
	        end
	      end
	    else
	      with_dir=%f;
      end // if part(dirs,length(dirs) ...
      
	    if with_dir then 
	      filesi = dirs+filesi;
	    end
	    
      files=[files;filesi];
      
    end // if filesi<>[] then
  end // for i=1:size(paths,'*')
  
endfunction
