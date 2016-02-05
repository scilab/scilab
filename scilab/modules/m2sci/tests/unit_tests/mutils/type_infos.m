% Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
% Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
% Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
% 
 % Copyright (C) 2012 - 2016 - Scilab Enterprises
 %
 % This file is hereby licensed under the terms of the GNU GPL v2.0,
 % pursuant to article 5.3.4 of the CeCILL v.2.1.
 % This file was originally licensed under the terms of the CeCILL v2.1,
 % and continues to be available under such terms.
 % For more information, see the COPYING file which you should have received
 % along with this program.
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
