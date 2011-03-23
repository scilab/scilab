// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Serge Steer
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [t] = ghdl2tree(h)
//  converts a graphic handle into a scilab tree (resursive tlist

  if argn(2)<>1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "ghdl2tree", 1));
  end
  
  if size(h,'*')> 1 then 
    t=list();for k=1:size(h,'*'),t(k)=ghdl2tree(h(k)),end
    return
  end
  
  typ=h.type

  //get the vector of fields names and make it a row
  fields=matrix(ghdl_fields(h),1,-1)
 
  if fields<>[] then
    t=tlist([typ,fields])
    for name=fields
      if execstr('p=get(h,name)','errcatch')<>0 then
	disp('error ghdl2tree '+typ+' '+name)
      else
      if type(p)==9 then // current field is a handle or a vector of handle
	if size(p,'*')== 1 then 
	  p=ghdl2tree(p),
	else
	  chl=list();for k=1:size(p,'*'),chl(k)=ghdl2tree(p(k)),end
	  p=chl
	end
      end
      t($+1)=p
      end
    end
  else //no fields defined
    t=tlist(typ)
  end
endfunction

