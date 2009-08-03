// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function files= listfiles(paths,flag,flagexpand)

  // list the files in a set of directories
  // files= listfiles(paths [,flag,flagexpand])
  // paths : a string matrix giving a set of pathnames (eventually ended by a pattern built with  * )
  // flag,flagexpand : boolean optional parameters. (default value  %t ).
  // files : a string matrix
  
  [lhs,rhs]=argn(0) 
  if rhs < 1  then paths='./' ; end 
  if rhs <= 1 then flag = %t ; end 
  if rhs <= 2 then flagexpand = %t ; end 
  files=[];
  
  if flag == %t then 
    paths = pathconvert(paths,%f,flagexpand); 
  end 

  DIR_SEPARATOR=filesep();
  
  for i=1:size(paths,'*')  // Loop on the file/path list
    
    [path,fname,extension]=fileparts(paths(i)); // Retrieve the information
    // about the file
    
    if isdir(paths(i)) then // It is a directory then returns all the file in the dir
      path = paths(i);
      extension='';
      fname='*';
    else
      // It is a file 
      if path == '' then path='./',end;
      if MSDOS // Don't want this case under Linux/Unix 
        // Windows will return the file toto even if you provided toto.*
        if extension == '' then extension='.*',end;
      end
      if fname == '' then fname='*',end;
    end

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
