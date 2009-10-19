// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimbase_configure --
//   Configure the current object with the given value for the given key.
//
function this = optimbase_configure (this,key,value)
  select key
  case "-verbose" then
    this.verbose = value;
  case "-verbosetermination" then
    this.verbosetermination = value;
  case "-logfile" then
    if ( this.logstartup ) then
      this = optimbase_logshutdown ( this )
    end
    this.logfile = value;
    this = optimbase_logstartup ( this );
  case "-x0" then
    [n,m] = size(value);
    if m<>1 then
      errmsg = msprintf(gettext("%s: The x0 vector is expected to be a column matrix, but current shape is %d x %d"),"optimbase_configure",n,m);
      error(errmsg);
    end
    this.x0 = value;
  case "-maxfunevals" then
    this.maxfunevals = value;
  case "-maxiter" then
    this.maxiter = value;
  case "-tolfunabsolute" then
    this.tolfunabsolute = value;
  case "-tolfunrelative" then
    this.tolfunrelative = value;
  case "-tolxabsolute" then
    this.tolxabsolute = value;
  case "-tolxrelative" then
    this.tolxrelative = value;
  case "-tolxmethod" then
    select value
    case %t then
      this.tolxmethod = %t;
    case %f then
      this.tolxmethod = %f;
    else
      errmsg = msprintf(gettext("%s: Unknown value %s for -tolxmethod option"),"optimbase_configure",value);
      error(errmsg);
    end
  case "-tolfunmethod" then
    select value
    case %t then
      this.tolfunmethod = %t;
    case %f then
      this.tolfunmethod = %f;
    else
      errmsg = msprintf(gettext("%s: Unknown value %s for -tolfunmethod"),"optimbase_configure",value);
      error(errmsg);
    end
  case "-function" then
    this.fun = value;
  case "-outputcommand" then
    this.outputcommand = value;
  case "-outputcommandarg" then
    this.outputcommandarg = value;
  case "-numberofvariables" then
    this.numberofvariables = value;
  case "-storehistory" then
    if ( value ) then
      this.storehistory = %t;
    else
      this.storehistory = %f;
    end
  case "-costfargument" then
    this.costfargument = value;
  case "-boundsmin" then
    this.boundsmin = value;
  case "-boundsmax" then
    this.boundsmax = value;
  case "-nbineqconst" then
    this.nbineqconst = value;
  case "-withderivatives" then
    if ( value ) then
      this.withderivatives = %t;
    else
      this.withderivatives = %f;
    end
  else
    errmsg = msprintf(gettext("%s: Unknown key %s"),"optimbase_configure",key)
    error(errmsg)
  end
endfunction
