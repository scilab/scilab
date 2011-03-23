// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function ok=datatipInitStruct(curve_handle,varargin)
//This function is used to initialize the data structure dedicated to
//data tips handling.;
//The data structure is a tlist attached to the polyline entity
// curve_handle : a handle on a polyline, the parent of the datatips data structure
// varargin     : a list containing a sequence of pairs;
//               (<property name), <property value>);
  if argn(2)<1 then
    error(msprintf(_("%s: Wrong number of input argument(s): At least %d expected.\n"),"datatipInitStruct",1))
  end

  ok=%t
  ud=datatipGetStruct(curve_handle)
  if typeof(ud)<>'datatips' then;
    if prod(size(ud))==0 then
      ud=tlist(['datatips','interpolate','replace','formatfunction','tips'],%f,%t,datatipDefaultDisplay,[])
      for k=1:2:size(varargin)
        key=varargin(k)
        if and(key<>['interpolate','replace','formatfunction']) then
          ud(1)=[ud(1) key]
        end
        if key<>'formatfunction' then
          ud(key)=varargin(k+1)
        else
          if exists(varargin(k+1))==1 then
            ffun=eval(varargin(k+1))
            if or(type(ffun)==[11 13]) then
              ud(key)=eval(varargin(k+1))
            else
              ud(key)=datatipDefaultDisplay
            end
          else
            ud(key)=datatipDefaultDisplay
          end
;
        end
      end
    else
      ok=%f
      warning(msprintf(_("Data tips cannot be attached to this curve,\nthe user_data field is already used")))
    end
  end
  datatipSetStruct(curve_handle,ud)
endfunction
