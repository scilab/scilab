// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function path = get_file_path(name)
//-------------------------------
// returns loader full path
path ='./';
[units,typs,nams]=file();
clear units typs
for k=size(nams,'*'):-1:1
  l=strindex(nams(k),name);
  if l<>[] then
    path=part(nams(k),1:l($)-1);
    break
  end
end
if path == "" then path = pwd() ; end
endfunction
