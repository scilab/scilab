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
    select value
    case "fixed" then
      this.method = "fixed";
    case "variable" then
      this.method = "variable";
    case "box" then
      this.method = "box";
    else
      errmsg = sprintf("Unknown method %s",value);
      error(errmsg);
    end
  case "-coords0" then
    this.coords0 = value;
  case "-simplex0method" then
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
      errmsg = sprintf("Unknown -simplex0method %s",value);
      error(errmsg);
    end
  case "-simplex0length" then
    this.simplex0length = value;
  case "-simplex0deltausual" then
    this.simplex0deltausual = value;
  case "-simplex0deltazero" then
    this.simplex0deltazero = value;
  case "-rho" then
    this.rho = value;
  case "-chi" then
    this.chi = value;
  case "-gamma" then
    this.gamma = value;
  case "-sigma" then
    this.sigma = value;
  case "-tolfstdeviation" then
    this.tolfstdeviation = value;
  case "-tolfstdeviationmethod" then
    select value
    case "enabled" then
      this.tolfstdeviationmethod = value;
    case "disabled" then
      this.tolfstdeviationmethod = value;
    else
      errmsg = sprintf("Unknown tolerance on function standard deviation method %s",value);
      error(errmsg);
    end
  case "-tolsimplexizeabsolute" then
    this.tolsimplexizeabsolute = value;
  case "-tolsimplexizerelative" then
    this.tolsimplexizerelative = value;
  case "-tolsimplexizemethod" then
    select value
    case "enabled" then
      this.tolsimplexizemethod = value;
    case "disabled" then
      this.tolsimplexizemethod = value;
    else
      errmsg = sprintf("Unknown tolerance on simplex size method %s",value);
      error(errmsg);
    end
  case "-toldeltafv" then
    this.toldeltafv = value;
  case "-tolssizedeltafvmethod" then
    select value
    case "enabled" then
      this.tolssizedeltafvmethod = value;
    case "disabled" then
      this.tolssizedeltafvmethod = value;
    else
      errmsg = sprintf("Unknown tolerance on simplex size and delta function value method %s",value);
      error(errmsg);
    end
  case "-restartmax" then
    this.restartmax = value;
  case "-restarteps" then
    this.restarteps = value;
  case "-restartstep" then
    this.restartstep = value;
  case "-kelleystagnationflag" then
    this.kelleystagnationflag = value;
  case "-kelleynormalizationflag" then
    this.kelleynormalizationflag = value;
  case "-kelleystagnationalpha0" then
    this.kelleystagnationalpha0 = value;
  case "-restartflag" then
    this.restartflag = value;
  case "-restartdetection" then
    this.restartdetection = value;
  case "-restartsimplexmethod" then
    this.restartsimplexmethod = value;
  case "-boxnbpoints" then
    this.boxnbpoints = value;
  case "-nbineqloops" then
    this.nbineqloops = value;
  case "-ineqscaling" then
    this.ineqscaling = value;
  else
    // Delegate to the optimization object
    this.optbase = optimbase_configure ( this.optbase , key , value );
  end
endfunction
