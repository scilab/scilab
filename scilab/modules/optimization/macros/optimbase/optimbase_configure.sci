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
    assert_typereal ( value , "value" , 3 );
    this.verbose = value;
  case "-verbosetermination" then
    assert_typereal ( value , "value" , 3 );
    this.verbosetermination = value;
  case "-logfile" then
    if ( this.logstartup ) then
      this = optimbase_logshutdown ( this )
    end
    this.logfile = value;
    this = optimbase_logstartup ( this );
  case "-x0" then
    assert_typereal ( value , "value" , 3 );
    [n,m] = size(value);
    if m<>1 then
      errmsg = msprintf(gettext("%s: The x0 vector is expected to be a column matrix, but current shape is %d x %d"),"optimbase_configure",n,m);
      error(errmsg);
    end
    this.x0 = value;
  case "-maxfunevals" then
    assert_typereal ( value , "value" , 3 );
    this.maxfunevals = value;
  case "-maxiter" then
    assert_typereal ( value , "value" , 3 );
    this.maxiter = value;
  case "-tolfunabsolute" then
    assert_typereal ( value , "value" , 3 );
    this.tolfunabsolute = value;
  case "-tolfunrelative" then
    assert_typereal ( value , "value" , 3 );
    this.tolfunrelative = value;
  case "-tolxabsolute" then
    assert_typereal ( value , "value" , 3 );
    this.tolxabsolute = value;
  case "-tolxrelative" then
    assert_typereal ( value , "value" , 3 );
    this.tolxrelative = value;
  case "-tolxmethod" then
    assert_typeboolean ( value , "value" , 3 );
    select value
    case %t then
      this.tolxmethod = %t;
    case %f then
      this.tolxmethod = %f;
    else
      unknownValueForOption ( value , "-tolxmethod" )
    end
  case "-tolfunmethod" then
    assert_typeboolean ( value , "value" , 3 );
    select value
    case %t then
      this.tolfunmethod = %t;
    case %f then
      this.tolfunmethod = %f;
    else
      unknownValueForOption ( value , "-tolfunmethod" )
    end
  case "-function" then
    assert_typefunction ( value , "value" , 3 );
    this.fun = value;
  case "-outputcommand" then
    assert_typefunction ( value , "value" , 3 );
    this.outputcommand = value;
  case "-outputcommandarg" then
    this.outputcommandarg = value;
  case "-numberofvariables" then
    assert_typereal ( value , "value" , 3 );
    this.numberofvariables = value;
  case "-storehistory" then
    assert_typeboolean ( value , "value" , 3 );
    if ( value ) then
      this.storehistory = %t;
    else
      this.storehistory = %f;
    end
  case "-costfargument" then
    this.costfargument = value;
  case "-boundsmin" then
    assert_typereal ( value , "value" , 3 );
    this.boundsmin = value;
  case "-boundsmax" then
    assert_typereal ( value , "value" , 3 );
    this.boundsmax = value;
  case "-nbineqconst" then
    assert_typereal ( value , "value" , 3 );
    this.nbineqconst = value;
  case "-withderivatives" then
    assert_typeboolean ( value , "value" , 3 );
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
// Generates an error if the given variable is not of type real
function assert_typereal ( var , varname , ivar )
  if ( type ( var ) <> 1 ) then
    errmsg = msprintf(gettext("%s: Expected real variable for variable %s at input #%d, but got %s instead."),"assert_typereal", varname , ivar , typeof(var) );
    error(errmsg);
  end
endfunction
// Generates an error if the given variable is not of type string
function assert_typestring ( var , varname , ivar )
  if ( type ( var ) <> 10 ) then
    errmsg = msprintf(gettext("%s: Expected string variable for variable %s at input #%d, but got %s instead."),"assert_typestring", varname , ivar , typeof(var) );
    error(errmsg);
  end
endfunction
// Generates an error if the given variable is not of type function (macro)
function assert_typefunction ( var , varname , ivar )
  if ( type ( var ) <> 13 ) then
    errmsg = msprintf(gettext("%s: Expected function but for variable %s at input #%d, got %s instead."),"assert_typefunction", varname , ivar , typeof(var) );
    error(errmsg);
  end
endfunction
// Generates an error if the given variable is not of type boolean
function assert_typeboolean ( var , varname , ivar )
  if ( type ( var ) <> 4 ) then
    errmsg = msprintf(gettext("%s: Expected boolean but for variable %s at input #%d, got %s instead."),"assert_typeboolean", varname , ivar , typeof(var) );
    error(errmsg);
  end
endfunction

// Generates an error if the value corresponding to an option is unknown.
function unknownValueForOption ( value , optionname )
      errmsg = msprintf(gettext("%s: Unknown value %s for %s option"),"unknownValueForOption",value , optionname );
      error(errmsg);
endfunction



