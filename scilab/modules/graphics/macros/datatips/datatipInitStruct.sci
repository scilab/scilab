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
  if type(curve_handle)<>9|size(curve_handle,'*')<>1|curve_handle.type<>"Polyline" then
    error(msprintf(_("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),"datatipInitStruct",1,"Polyline"))
  end
  ok=%t;
  ud=datatipGetStruct(curve_handle);
  if typeof(ud)<>"datatips" then
    //Create structure and set  default values
    ud=tlist(["datatips","style","interpolate","replace", "formatfunction",      "tips","selected"],..
             [1 1 1],     %t,          %t,      datatipDefaultDisplay,  [],      0);
  end
  //loop on argument pairs
  for k=1:2:size(varargin)
    key=varargin(k)
    wh=find(key==["interpolate","replace","formatfunction","style","selected"])
    if wh==[] then  ud(1)=[ud(1) key];end //add a new property

    select key
    case "style" then
      style=varargin(k+1)
      if type(style)<>1|~isreal(style) then
        error(msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"),..
                       "datatipInitStruct",k+1))
      end
      if size(style,'*')<>2 then
        error(msprintf(_("%s: Wrong size for input argument #%d: A %d elements array expected.\n"),..
                       "datatipInitStruct",k+1,2))
      end
      if and(style(1)<>[1 2]) then
        error(msprintf(_("%s: Wrong value for input argument #%d(1): Must be between %d and %d.\n"),..
                       "datatipInitStruct",k+1,1,2))
      end
      if and(style(2)<>[0 1]) then
        error(msprintf(_("%s: Wrong value for input argument #%d(2): Must be between %d and %d.\n"),..
                       "datatipInitStruct",k+1,0,1))

      end
      ud(key)=style
    case "interpolate" then
      interpolate=varargin(k+1)
      if type(interpolate)<>4|size(interpolate,'*')<>1 then
        error(msprintf(_("%s: Wrong type for input argument #%d: A boolean expected.\n"),..
                       "datatipInitStruct",k+1))
      end
      ud(key)=interpolate
    case "replace" then
      replace=varargin(k+1)
      if type(replace)<>4|size(replace,'*')<>1 then
        error(msprintf(_("%s: Wrong type for input argument #%d: A boolean expected.\n"),..
                       "datatipInitStruct",k+1))
      end
      ud(key)=replace
    case "formatfunction" then
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
    case "selected"
      selected=varargin(k+1)
      if type(selected)<>1|~isreal(selected)|size(selected,'*')<>1|selected<0 then
        error(msprintf(_("%s: Wrong type for input argument #%d: Non-negative int expected.\n"),..
                       "datatipInitStruct",k+1))
      end
      ud(key)=selected
    else
      //user defined property (no tests)
      ud(key)=varargin(k+1)
    end
  end
  ok=datatipSetStruct(curve_handle,ud)
endfunction
