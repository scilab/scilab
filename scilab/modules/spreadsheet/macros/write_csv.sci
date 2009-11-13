// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function write_csv(var, fname, sep, dec)
// var : a matrix of numbers or a matrix of strings
// fname : a character string, the path of the file to create
// sep   : the column separator mark, a tab by default
// dec   : the decimal mark . or ,
// Example
// a = rand(3,5);
// write_csv(a,'/tmp/foo.txt',sep=',')

  if exists('sep','local') == 0 then
    sep = ascii(9);
  end
  
  if exists('dec','local') == 0 then
    dec = ',';
  end
  
  if ( (type(var) <> 10) & (type(var) <> 1) ) then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: ''%s'' or ''%s'' expected.\n"), 'write_csv', 1, 'real', 'string'));
  end

  if (type(fname) <> 10) then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: a string expected.\n"), 'write_csv', 2));
  end
  
  if (size(fname,'*')<> 1) then
    error(msprintf(gettext("%s: Wrong size for input argument #%d: a string expected.\n"), 'write_csv', 2));  
  end

  if (type(dec) <> 10) then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: a string expected.\n"), 'write_csv', 3));
  end

  if (size(dec,'*')<> 1) then
    error(msprintf(gettext("%s: Wrong size for input argument #%d: a string expected.\n"), 'write_csv', 3));  
  end

  if dec <> ['.',','] then 
    error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"), 'write_csv', 3, '.', ','));
  end

  if type(var) == 1 then
    var = string(var);
    if dec <> '.' then 
      var = strsubst(var,'.',',');
    end
  end

  t = var(:, 1);
  for k=2:size(var, 2)
    t = t + sep + var(:, k);
  end
  
  mputl(t,fname);
  
endfunction
