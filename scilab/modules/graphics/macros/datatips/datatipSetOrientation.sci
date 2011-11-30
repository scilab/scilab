// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function datatipSetOrientation(tip_handle,o)
  if argn(2)<>2 then
    error(msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),...
                   "datatipSetOrientation",2))
  end
  orientations=["automatic" "upper left" "upper right", "lower left","lower right"]
  if type(tip_handle)<>9|size(tip_handle,'*')<>1|or(tip_handle.type<>"Compound") then
    error(msprintf(_("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),...
                   "datatipSetOrientation",1,"datatip"))
  end

  k=find(orientations==convstr(o))
  if k<>[] then
    string_handle=tip_handle.children(2)
    point_handle=tip_handle.children(1)
    string_handle.user_data(1)=k-1
    setStringPosition(tip_handle, point_handle.data)
    drawnow()
  end
endfunction
