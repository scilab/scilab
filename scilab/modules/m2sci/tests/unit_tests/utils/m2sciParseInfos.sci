// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

function [sizeInfo, typeInfo, propInfo] = m2sciParseInfos(strInfos)
parts = tokens(strInfos, "|");

if size(parts, "*") == 3 then
  // The name is not given
  sizeInfo = msscanf(parts(1), "%d %d");
  typeInfo = parts(2);
  propInfo = parts(3);
else
  // The name is given
  sizeInfo = msscanf(parts(2), "%d %d");
  typeInfo = parts(3);
  propInfo = parts(4);
  
end
endfunction
