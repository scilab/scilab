// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Vincent COUVERT
// ...
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f=fullfile(varargin)
// Build a full filename from parts

if lstsize(varargin)<2 then
  error("Wrong number of inputs!");
end

fs = ["/" "\"];
f = varargin(1);
for k=2:lstsize(varargin)
  arg = varargin(k);
  if isempty(f) | isempty(arg)
    f = f+arg;
  else
    if or(part(f,length(f))==fs) & or(part(arg,1)==fs)
      f = f+part(arg,2:length(arg));
    elseif or(part(f,length(f))==fs) | or(part(arg,1)==fs)
      f = f+arg;
    else
      f = f+pathconvert("/")+arg;
    end
  end
end
f=pathconvert(f,%f,%f);
endfunction
