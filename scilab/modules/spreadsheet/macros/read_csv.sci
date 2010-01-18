// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function mat = read_csv(fname,sep)
// Given an ascii  file created by Excel using "Text and comma" format
// read_csv(fname) returns the corresponding Scilab matrix of strings.

  if (type(fname) <> 10) then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: a string expected.\n"), 'write_csv', 1));
  end
  
  if (size(fname,'*')<> 1) then
    error(msprintf(gettext("%s: Wrong size for input argument #%d: a string expected.\n"), 'write_csv', 1));  
  end
  
  if argn(2)<2 then 
    sep = ',';
  end
  
  if ~isfile(fname) then
    error(msprintf(gettext("%s: file %s does not exist.\n"), 'write_csv', 1, fname));  
  end
  
  v = mgetl(fname);
  ns = length(sep);
  mat = [];
  ki = 1;
  for i = 1 : size(v,'*')
    line = v(i);
    K = [strindex(line, sep)];
    ki = 1;
    row = [];
    for k = K
      row = [row, part(line,ki:k-1)];
      ki = k + ns;
    end
    row = [row, part(line, ki:length(line))];
    if (i > 1) then
      if size(row,2) > size(mat,2) then
        mat($,size(row,2)) = '';
      elseif size(row,2) < size(mat,2) then
        row(size(mat,2)) = '';
      end
    end
    mat = [mat; row];
  end
endfunction
