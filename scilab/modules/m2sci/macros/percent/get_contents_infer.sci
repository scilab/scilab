// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [infertlist,pos]=get_contents_infer(from,index)
// Search inference data in the contents of a cell or a struct corresponding to a particular index
// Input: 
//  - from: the cell or struct where we have to search
//  - index: index to search
// Output:
//  - infertlist: inference data found
//  - pos: position where index has been found

pos=0;
infertlist=Infer();

for k=1:lstsize(from.contents.index)
  allequal=[]
  if lstsize(index)==lstsize(from.contents.index(k)) then // Indexes must have the same size
    for ki=1:lstsize(index)
      if typeof(index(ki))==typeof(from.contents.index(k)(ki)) then // Index elements must have the same type
	if typeof(index(ki))<>"list" then
	  allequal=allequal & ( and(index(ki)==from.contents.index(k)(ki)) | and(from.contents.index(k)(ki)==Cste("*")) )
	elseif typeof(index(ki))=="list" then
	  for kii=1:lstsize(index(ki))
	    allequal=allequal & ( and(index(ki)(kii)==from.contents.index(k)(ki)(kii)) | and(from.contents.index(k)(ki)(kii)==Cste("*")) )
	  end
	end
      end
    end
  end
  if ~isempty(allequal) & allequal then
    pos=k
    infertlist=from.contents.data(k)
    return
  end
end
endfunction
