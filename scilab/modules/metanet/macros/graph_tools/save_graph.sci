
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function save_graph(g,path)
// path
  if argn(2)<2 then path='./',end
  if type(path)<>10|prod(size(path))<>1 then
    error('Second argument must be a string')
  end
  path=pathconvert(path,%t,%t)
  if isdir(path) then
    path=path+g.name+'.graph'
  else
    l=length(path)
    if part(path,l-5:l)<>'.graph' then
      path=path+'.graph'
    end
  end

  check_graph(g)
  save(path,g)
endfunction
