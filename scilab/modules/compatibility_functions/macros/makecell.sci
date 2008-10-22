// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function s=makecell(varargin)
// Equivalent for Matlab {}

if lstsize(varargin)==0 then
  dims=[0 0]
  s=mlist(["ce","dims","entries"],int32(dims),list())
  return
end

dims=varargin(1)
if prod(dims)<>lstsize(varargin)-1 then
  error(msprintf(gettext("%s: Wrong input arguments: Dimensions given as first argument do not match specified cell contents.\n"),"makecell"));
end

if prod(dims)==1 then
  s=mlist(["ce","dims","entries"],int32(dims),varargin(2))
else
  listentries=varargin
  listentries(1)=null()
  s=mlist(["ce","dims","entries"],int32(dims($:-1:1)),listentries)'
end
endfunction
