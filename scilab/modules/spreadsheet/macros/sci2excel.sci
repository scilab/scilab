
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function sci2excel(var,fname,sep,dec)
// var : a matrix of numbers or a matrix of strings
// fname : a character string, the path of the file to create
// sep   : the column separator mark, a tab by default
// dec   : the decimal mark . or ,
// Example
// a=rand(3,5);
// sci2spreadsheet(a,'/tmp/foo.txt',sep=',')
if exists('sep','local')==0 then sep=code2str(-40),end
if exists('dec','local')==0 then dec=',',end

if dec<>['.',','] then 
  error(msprintf(gettext("%s: Wrong input argument #%d: ''%s'' or ''%s'' expected.\n"), 'sci2excel', 3, '.', ','));
end

if type(var)==1 then
  var=string(var)
  if dec<>'.' then var =strsubst(var,'.',','),end
end
t=var(:,1)
for k=2:size(var,2)
  t=t+sep+var(:,k)
end
mputl(t,fname);
endfunction
