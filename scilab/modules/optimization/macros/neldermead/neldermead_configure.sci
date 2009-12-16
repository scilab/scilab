// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// neldermead_configure --
//   Configure neldermead and returns the modified object.
//
function this = neldermead_configure (this,key,value)
  select key
  case "-method" then
    assert_typestring ( value , "value" , 3 );
    select value
    case "fixed" then
      this.method = "fixed";
    case "variable" then
      this.method = "variable";
    case "box" then
      this.method = "box";
    case "mine" then
      this.method = "mine";
    else
      unknownValueForOption ( value , "-method" )
    end
  case "-coords0" then
    assert_typereal ( value , "value" , 3 );
    this.coords0 = value;
  case "-simplex0method" then
    assert_typestring ( value , "value" , 3 );
    select value
    case "given" then
      this.simplex0method = "given";
    case "axes" then
      this.simplex0method = "axes";
    case "spendley" then
      this.simplex0method = "spendley";
    case "pfeffer" then
      this.simplex0method = "pfeffer";
    case "randbounds" then
      this.simplex0method = "randbounds";
    else
      unknownValueForOption ( value , "-simplex0method" )
    end
  case "-simplex0length" then
    assert_typereal ( value , "value" , 3 );
    this.simplex0length = value;
  case "-simplex0deltausual" then
    assert_typereal ( value , "value" , 3 );
    this.simplex0deltausual = value;
  case "-simplex0deltazero" then
    assert_typereal ( value , "value" , 3 );
    this.simplex0deltazero = value;
  case "-rho" then
    assert_typereal ( value , "value" , 3 );
    this.rho = value;
  case "-chi" then
    assert_typereal ( value , "value" , 3 );
    this.chi = value;
  case "-gamma" then
    assert_typereal ( value , "value" , 3 );
    this.gamma = value;
  case "-sigma" then
    assert_typereal ( value , "value" , 3 );
    this.sigma = value;
  case "-tolsimplexizeabsolute" then
    assert_typereal ( value , "value" , 3 );
    this.tolsimplexizeabsolute = value;
  case "-tolsimplexizerelative" then
    assert_typereal ( value , "value" , 3 );
    this.tolsimplexizerelative = value;
  case "-tolsimplexizemethod" then
    assert_typeboolean ( value , "value" , 3 )
    select value
    case %t then
      this.tolsimplexizemethod = value;
    case %f then
      this.tolsimplexizemethod = value;
    else
      unknownValueForOption ( value , "-tolsimplexizemethod" )
    end
  case "-toldeltafv" then
    assert_typereal ( value , "value" , 3 );
    this.toldeltafv = value;
  case "-tolssizedeltafvmethod" then
    assert_typeboolean ( value , "value" , 3 )
    select value
    case %t then
      this.tolssizedeltafvmethod = value;
    case %f then
      this.tolssizedeltafvmethod = value;
    else
      unknownValueForOption ( value , "-tolssizedeltafvmethod" )
    end
  case "-restartmax" then
    assert_typereal ( value , "value" , 3 );
    this.restartmax = value;
  case "-restarteps" then
    assert_typereal ( value , "value" , 3 );
    this.restarteps = value;
  case "-restartstep" then
    assert_typereal ( value , "value" , 3 );
    this.restartstep = value;
  case "-kelleystagnationflag" then
    assert_typeboolean ( value , "value" , 3 )
    this.kelleystagnationflag = value;
  case "-kelleynormalizationflag" then
    assert_typeboolean ( value , "value" , 3 )
    this.kelleynormalizationflag = value;
  case "-kelleystagnationalpha0" then
    assert_typereal ( value , "value" , 3 );
    this.kelleystagnationalpha0 = value;
  case "-restartflag" then
    assert_typeboolean ( value , "value" , 3 )
    this.restartflag = value;
  case "-restartdetection" then
    assert_typestring ( value , "value" , 3 )
    this.restartdetection = value;
  case "-restartsimplexmethod" then
    assert_typestring ( value , "value" , 3 );
    this.restartsimplexmethod = value;
  case "-checkcostfunction" then
    assert_typeboolean ( value , "value" , 3 )
    select value
    case %f then
      this.checkcostfunction = value;
    case %t then
      this.checkcostfunction = value;
    else
      unknownValueForOption ( value , "-checkcostfunction" )
    end
  case "-boxnbpoints" then
    assert_typereal ( value , "value" , 3 );
    this.boxnbpoints = value;
  case "-boxineqscaling" then
    assert_typereal ( value , "value" , 3 );
    this.boxineqscaling = value;
  case "-scalingsimplex0" then
    assert_typestring ( value , "value" , 3 );
    this.scalingsimplex0 = value;
  case "-guinalphamin" then
    assert_typereal ( value , "value" , 3 );
    if ( value <=0.0 ) then 
      errmsg = msprintf(gettext("%s: Unexpected negative value %s for %s option"),"neldermead_configure", value , "-guinalphamin" );
      error(errmsg);
    end
    this.guinalphamin = value;
  case "-boxboundsalpha" then
    assert_typereal ( value , "value" , 3 );
    this.boxboundsalpha = value
  case "-boxtermination" then
    assert_typeboolean ( value , "value" , 3 );
    this.boxtermination = value
  case "-boxtolf" then
    assert_typereal ( value , "value" , 3 );
    this.boxtolf = value
  case "-boxnbmatch" then
    assert_typereal ( value , "value" , 3 );
    this.boxnbmatch = value
  case "-boxreflect" then
    assert_typereal ( value , "value" , 3 );
    this.boxreflect = value
  case "-mymethod" then
    assert_typefunction ( value , "value" , 3 );
    this.mymethod = value
  case "-myterminate" then
    assert_typefunction ( value , "value" , 3 );
    this.myterminate = value
  case "-myterminateflag" then
    assert_typeboolean ( value , "value" , 3 );
    select value
    case %f then
      this.myterminateflag = value;
    case %t then
      this.myterminateflag = value;
    else
      unknownValueForOption ( value , "-myterminateflag" )
    end
  case "-tolvarianceflag" then
    assert_typeboolean ( value , "value" , 3 )
    this.tolvarianceflag = value
  case "-tolabsolutevariance" then
    assert_typereal ( value , "value" , 3 );
    this.tolabsolutevariance = value
  case "-tolrelativevariance" then
    assert_typereal ( value , "value" , 3 );
    this.tolrelativevariance = value
  case "-greedy" then
    assert_typeboolean ( value , "value" , 3 )
    this.greedy = value
  else
    // Delegate to the optimization object
    this.optbase = optimbase_configure ( this.optbase , key , value );
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



