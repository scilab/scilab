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
    assert_typestring ( value );
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
    assert_typereal ( value );
    this.coords0 = value;
  case "-simplex0method" then
    assert_typestring ( value );
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
    assert_typereal ( value );
    this.simplex0length = value;
  case "-simplex0deltausual" then
    assert_typereal ( value );
    this.simplex0deltausual = value;
  case "-simplex0deltazero" then
    assert_typereal ( value );
    this.simplex0deltazero = value;
  case "-rho" then
    assert_typereal ( value );
    this.rho = value;
  case "-chi" then
    assert_typereal ( value );
    this.chi = value;
  case "-gamma" then
    assert_typereal ( value );
    this.gamma = value;
  case "-sigma" then
    assert_typereal ( value );
    this.sigma = value;
  case "-tolsimplexizeabsolute" then
    assert_typereal ( value );
    this.tolsimplexizeabsolute = value;
  case "-tolsimplexizerelative" then
    assert_typereal ( value );
    this.tolsimplexizerelative = value;
  case "-tolsimplexizemethod" then
    assert_typeboolean ( value )
    select value
    case %t then
      this.tolsimplexizemethod = value;
    case %f then
      this.tolsimplexizemethod = value;
    else
      unknownValueForOption ( value , "-tolsimplexizemethod" )
    end
  case "-toldeltafv" then
    assert_typereal ( value );
    this.toldeltafv = value;
  case "-tolssizedeltafvmethod" then
    assert_typeboolean ( value )
    select value
    case %t then
      this.tolssizedeltafvmethod = value;
    case %f then
      this.tolssizedeltafvmethod = value;
    else
      unknownValueForOption ( value , "-tolssizedeltafvmethod" )
    end
  case "-restartmax" then
    assert_typereal ( value );
    this.restartmax = value;
  case "-restarteps" then
    assert_typereal ( value );
    this.restarteps = value;
  case "-restartstep" then
    assert_typereal ( value );
    this.restartstep = value;
  case "-kelleystagnationflag" then
    assert_typeboolean ( value )
    this.kelleystagnationflag = value;
  case "-kelleynormalizationflag" then
    assert_typeboolean ( value )
    this.kelleynormalizationflag = value;
  case "-kelleystagnationalpha0" then
    assert_typereal ( value );
    this.kelleystagnationalpha0 = value;
  case "-restartflag" then
    assert_typeboolean ( value )
    this.restartflag = value;
  case "-restartdetection" then
    assert_typestring ( value )
    this.restartdetection = value;
  case "-restartsimplexmethod" then
    assert_typestring ( value );
    this.restartsimplexmethod = value;
  case "-checkcostfunction" then
    assert_typeboolean ( value )
    select value
    case %f then
      this.checkcostfunction = value;
    case %t then
      this.checkcostfunction = value;
    else
      unknownValueForOption ( value , "-checkcostfunction" )
    end
  case "-boxnbpoints" then
    assert_typereal ( value );
    this.boxnbpoints = value;
  case "-boxineqscaling" then
    assert_typereal ( value );
    this.boxineqscaling = value;
  case "-scalingsimplex0" then
    assert_typestring ( value );
    this.scalingsimplex0 = value;
  case "-guinalphamin" then
    assert_typereal ( value );
    if ( value <=0.0 ) then 
      errmsg = msprintf(gettext("%s: Unexpected negative value %s for %s option"),"neldermead_configure", value , "-guinalphamin" );
      error(errmsg);
    end
    this.guinalphamin = value;
  case "-boxboundsalpha" then
    assert_typereal ( value );
    this.boxboundsalpha = value
  case "-boxtermination" then
    assert_typeboolean ( value );
    this.boxtermination = value
  case "-boxtolf" then
    assert_typereal ( value );
    this.boxtolf = value
  case "-boxnbmatch" then
    assert_typereal ( value );
    this.boxnbmatch = value
  case "-boxreflect" then
    assert_typereal ( value );
    this.boxreflect = value
  case "-mymethod" then
    assert_typefunction ( value );
    this.mymethod = value
  case "-myterminate" then
    assert_typefunction ( value );
    this.myterminate = value
  case "-myterminateflag" then
    assert_typeboolean ( value );
    select value
    case %f then
      this.myterminateflag = value;
    case %t then
      this.myterminateflag = value;
    else
      unknownValueForOption ( value , "-myterminateflag" )
    end
  case "-tolvarianceflag" then
    assert_typeboolean ( value )
    this.tolvarianceflag = value
  case "-tolabsolutevariance" then
    assert_typereal ( value );
    this.tolabsolutevariance = value
  case "-tolrelativevariance" then
    assert_typereal ( value );
    this.tolrelativevariance = value
  case "-greedy" then
    assert_typeboolean ( value )
    this.greedy = value
  else
    // Delegate to the optimization object
    this.optbase = optimbase_configure ( this.optbase , key , value );
  end
endfunction
// Generates an error if the given variable is not of type real
function assert_typereal ( var )
  if ( type ( var ) <> 1 ) then
    errmsg = msprintf(gettext("%s: Expected real variable but got %s instead"),"assert_typereal", typeof(var) );
    error(errmsg);
  end
endfunction
// Generates an error if the given variable is not of type string
function assert_typestring ( var )
  if ( type ( var ) <> 10 ) then
    errmsg = msprintf(gettext("%s: Expected string variable but got %s instead"),"assert_typestring", typeof(var) );
    error(errmsg);
  end
endfunction
// Generates an error if the given variable is not of type function (macro)
function assert_typefunction ( var )
  if ( type ( var ) <> 13 ) then
    errmsg = msprintf(gettext("%s: Expected function but got %s instead"),"assert_typefunction", typeof(var) );
    error(errmsg);
  end
endfunction
// Generates an error if the given variable is not of type boolean
function assert_typeboolean ( var )
  if ( type ( var ) <> 4 ) then
    errmsg = msprintf(gettext("%s: Expected boolean but got %s instead"),"assert_typeboolean", typeof(var) );
    error(errmsg);
  end
endfunction


// Generates an error if the value corresponding to an option is unknown.
function unknownValueForOption ( value , optionname )
      errmsg = msprintf(gettext("%s: Unknown value %s for %s option"),"unknownValueForOption",value , optionname );
      error(errmsg);
endfunction



