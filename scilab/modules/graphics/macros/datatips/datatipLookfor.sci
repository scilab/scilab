// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [curve_index,tip_index]=datatipLookfor(curve_handles,pt)
//curve_handles:  a vector of curves which are supposed to have datatips
//pt           :  a 2D point in pixels
//curve_index  :  the index of corresponding curve in curve_handles
//tip_index    :  the datatip index for the curve;
  for curve_index=1:size(curve_handles,'*')
    ud=datatipGetStruct(curve_handles(curve_index))
    if typeof(ud)=='datatips' then
      tips=ud.tips
      for tip_index=1:size(tips,'*')
        d=pixDist(tips(tip_index).children(1).data,pt)
        if d<10 then return, end
      end
    end
  end
  curve_index=[];tip_index=[];
endfunction
