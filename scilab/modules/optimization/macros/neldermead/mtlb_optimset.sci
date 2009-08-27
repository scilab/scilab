// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// mtlb_optimset --
//   Emulate the optimset command of Matlab.
// Usage:
//   options = mtlb_optimset ()
//   options = mtlb_optimset ('TolX',1e-4)
//   options = mtlb_optimset (options,'TolX',1e-4)
//
function options = mtlb_optimset (varargin)
  [lhs,rhs]=argn();
  //mprintf("lhs\n");
  //disp(lhs);
  //mprintf("rhs\n");
  //disp(rhs);
  if rhs==1 then
    //
    // If there is only one argument, it is expected to be the
    // name of a method.
    //
    method = varargin(1);
    options = mtlb_optimset_method ( method );
  elseif modulo(rhs,2)<>0 then
    //
    // If the number of arguments is odd,
    // the first argument is expected to be a optimset struct.
    //
    options = varargin(1);
    t1 = typeof(options);
    if t1<>"st" then
      errmsg = sprintf("Odd number of arguments : the first argument is expected to be a struct, but is a %s",t1);
      error(errmsg)
    end
    // ivar is a counter of the input arguments
    ivar = 1;
  else
    options = mtlb_optimset_new ();
    // ivar is a counter of the input arguments
    ivar = 0;
  end
  //
  // Process key,values as pairs.
  //
  nbkeys = rhs/2;
  for i=1:nbkeys
    ivar = ivar + 1;
    key = varargin(ivar);
    ivar = ivar + 1;
    value = varargin(ivar);
    //mprintf("Argument #%d\n",ivar);
    //mprintf("key = %s\n",key);
    //mprintf("value = %s\n",string(value)); // One cannot display a function.
    options = mtlb_optimset_configure (options,key,value);
  end
endfunction
//
// mtlb_optimset_configure --
//   Configure the given key with the given value.
// Arguments
//   options : a optimset struct
//   key : a string, the key to configure
//   value : the value
//
function options = mtlb_optimset_configure ( options , key , value )
    select key
    case "Display" then
      options.Display = value;
    case "FunValCheck" then
      options.FunValCheck = value;
    case "MaxFunEvals" then
      options.MaxFunEvals = value;
    case "MaxIter" then
      options.MaxIter = value;
    case "OutputFcn" then
      options.OutputFcn = value;
    case "PlotFcns" then
      options.PlotFcns = value;
    case "TolFun" then
      options.TolFun = value;
    case "TolX" then
      options.TolX = value;
    else
      errmsg = sprintf("Unknown key %s",key)
      error(errmsg)
    end
endfunction
//
// mtlb_optimset_new --
//   Returns an empty optimset struct
//
function options = mtlb_optimset_new ()
    options = struct(...
      "Display" ,[],...
      "FunValCheck" ,[],...
      "MaxFunEvals" ,[],...
      "MaxIter" ,[],...
      "OutputFcn" ,[],...
      "PlotFcns" ,[],...
      "TolFun" ,[],...
      "TolX" ,[]...
      );
endfunction
//
// mtlb_optimset_method --
//   Returns an optimset struct which content is the default 
//   settings for the given method.
// Arguments
//   method : a string, the name of the method
//
function options = mtlb_optimset_method ( method )
    options = mtlb_optimset_new ();
    select method
    case "fminsearch" then
      options = mtlb_optimset_configure ( options , "Display" , "notify" );
      options = mtlb_optimset_configure ( options , "MaxFunEvals" , "200*numberofvariables" );
      options = mtlb_optimset_configure ( options , "MaxIter" , "200*numberofvariables" );
      options = mtlb_optimset_configure ( options , "TolFun" , 1.e-4 );
      options = mtlb_optimset_configure ( options , "TolX" , 1.e-4 );
    else
      errmsg = sprintf("Unknown method %s",method)
      error(errmsg)
    end
endfunction

