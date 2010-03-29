% Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
% Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
% Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
% 
% This file must be used under the terms of the CeCILL.
% This source file is licensed as described in the file COPYING, which
% you should have received as part of this distribution.  The terms
% are also available at    
% http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
%

function txt=type_infos(resStruct)

if strcmp(resStruct.class, 'char')==1
  txt='String|Real';
elseif resStruct.sparse==1
  if resStruct.complex==0
    txt='Sparse|Real';
  else
    txt='Sparse|Complex';
  end
elseif strcmp(resStruct.class, 'logical')==1
  txt='Boolean|Real';
else
  if resStruct.complex==0
    txt='Double|Real';
  else
    txt='Double|Complex';
  end
end  
