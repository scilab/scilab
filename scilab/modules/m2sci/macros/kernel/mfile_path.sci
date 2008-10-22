// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function path=mfile_path(nam)
fil=nam+'.m';nf=length(fil)
path=[];
for k=1:size(mfiles,'*')
  pk=mfiles(k);
  kk=strindex(pk,'/');
  if fil==part(pk,kk($)+1:length(pk)) then
    path=pk;
    break
  end
end
endfunction
