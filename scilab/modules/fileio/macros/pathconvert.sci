// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) DIGITEO - 2009 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function paths = pathconvert(paths, flagtrail, flagexpand, str)
// convert a path from dos style to unix style
// using cygwin conventions i.e x: -> /cygdrive/x
// if flagtrail is true then a trailing / or \ is added 
// if flagexpand is true then SCI, HOME, ~ ,TMPDIR are expanded 
// str gives the target style 'u' or 'w' 

  [lhs,rhs] = argn(0);

  unix_sep = '/';
  dos_sep = '\';

  sci = SCI; 

  if rhs <= 1 then flagtrail = %t ;end 
  if rhs <= 2 then flagexpand = %t ; end 
  if rhs <= 3 then 
    if MSDOS then 
      str = 'w'; 
    else 
      str = 'u';
    end
  end

  if str == 'w' then 
    sep = dos_sep;
    if isdef('WSCI') then sci = WSCI;end 
  else
    sep = unix_sep;
  end

  if type(paths) <> 10 then 
    error(999,msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"),'pathconvert',1));
  end

  if type(flagtrail) <> 4 then 
    error(999,msprintf(_("%s: Wrong type for input argument #%d: A boolean expected.\n"),'pathconvert',2));
  end

  if size(flagtrail,'*') <> 1 then
    error(999,msprintf(_("%s: Wrong size for input argument #%d: A boolean expected.\n"),'pathconvert',2));
  end 

  if type(flagexpand) <> 4 then 
    error(999,msprintf(_("%s: Wrong type for input argument #%d: A boolean expected.\n"),'pathconvert',3));
  end 

  if size(flagexpand,'*') <> 1 then
    error(999,msprintf(_("%s: Wrong size for input argument #%d: A boolean expected.\n"),'pathconvert',3));
  end 

  if type(str) <> 10 then 
    error(999,msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"),'pathconvert',4));
  end

  if size(str,'*') <> 1 then
    error(999,msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),'pathconvert',4));
  end

  if ( (str <> 'w') & (str <> 'u') ) then
    error(999,msprintf(_("%s: Wrong value for input argument #%d: ''w'' or ''u'' expected.\n"),'pathconvert',4));
  end

  // strip leading and trailink blanks 
  paths = stripblanks(paths); 

  // \ and / conversion 
  if str == 'w' then 
    paths = strsubst(paths, unix_sep, dos_sep);
  else
    paths = strsubst(paths, dos_sep, unix_sep);
  end

  // add a trailing / or \ at the end 

  if flagtrail then 
    for i = 1:size(paths,'*') 
      path = paths(i);
      len = length(path);
      if len > 0 then
        if part(path, length(path))<> sep then 
          paths(i) = paths(i) + sep;
        end
      end
    end
  else
    for i = 1:size(paths,'*') 
      path = paths(i);
      len = length(path);
      if (len > 0) & (path <> sep) then
        if part(path, length(path)) == sep then 
          paths(i) = part(paths(i), 1:length(path)-1);
        end
      end
    end
  end
  
  // expand HOME, SCI and ~ , TMPDIR
  
  is_expanded = %f;
  
  if flagexpand then 
    for i = 1:size(paths,'*') 
      path = paths(i);
      if part(path, 1:4) == 'SCI' + sep then 
        is_expanded = %t ;
        paths(i) = sci + part(path,4:length(path));
      elseif part(path, 1:2) == '~' + sep then 
        is_expanded = %t ;
        paths(i) = getenv('HOME','/home/') + part(path,2:length(path));
      elseif part(path, 1:5) == 'HOME' + sep then 
        is_expanded = %t;
        paths(i) = getenv('HOME','/home/')+part(path,5:length(path));
      elseif part(path, 1:7) == 'TMPDIR' + sep then
        is_expanded = %t;
        paths(i) = TMPDIR + part(path, 7:length(path));
      end
    end
  end

  // \ and / conversion again 
  if is_expanded then 
    if str == 'w' then 
      paths = strsubst(paths, unix_sep, dos_sep);
    else
      paths = strsubst(paths, dos_sep, unix_sep);
    end
  end

  // check for \cygdrive\xx\ 
  if str == 'w' then 
    // convert \cygdrive\xx to xx: 
    for i=1:size(paths,'*') 
      path=paths(i);
      if part(path,1:10) == '\cygdrive\' then 
        // get the letter name 
        k = strindex(path,'\'); 
        if length(k) <= 2 then 
          paths(i) = part(path,11:length(path))+':' ;
        else
          paths(i) = part(path,11:k(3)-1) + ':' + part(path,k(3):length(path));
        end
      end
    end
  else 
     // convert xx: to /cygdrive/xx 
     for i = 1:size(paths,'*') 
       path = paths(i);
       k = strindex(path,'/');
       if k == [] then 
         if part(path,length(path)) == ':' then 
           paths(i) = '/cygdrive/' + part(path,1:length(path)-1) + sep;
         end
       elseif k(1) > 1 then 
         if part(path,k(1)-1) == ':' then 
           paths(i) = '/cygdrive/' + part(path,1:k(1)-2)+ part(path,k(1):length(path));
         end
       end
     end
  end

endfunction 
