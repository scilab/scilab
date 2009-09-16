// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// neldermead_cget --
//   Get the value for the given key.
//   If the key is unknown, generates an error.
//
function value = neldermead_cget (this,key)
  select key
  case "-method" then
    value = this.method;
  case "-coords0" then
    value = this.coords0;
  case "-simplex0" then
    value = this.simplex0;
  case "-simplex0method" then
    value = this.simplex0method;
  case "-simplex0length" then
    value = this.simplex0length;
  case "-simplex0deltausual" then
    value = this.simplex0deltausual;
  case "-simplex0deltazero" then
    value = this.simplex0deltazero;
  case "-rho" then
    value= this.rho;
  case "-chi" then
    value = this.chi;
  case "-gamma" then
    value = this.gamma;
  case "-sigma" then
    value = this.sigma;
  case "-tolfstdeviationmethod" then
  value = this.tolfstdeviationmethod;
  case "-tolfstdeviation" then
    value = this.tolfstdeviation;
  case "-tolsimplexizemethod" then
  value = this.tolsimplexizemethod;
  case "-tolsimplexizeabsolute" then
    value = this.tolsimplexizeabsolute;
  case "-tolsimplexizerelative" then
    value = this.tolsimplexizerelative;
  case "-toldeltafv" then
    value = this.toldeltafv;
  case "-tolssizedeltafvmethod" then
    value = this.tolssizedeltafvmethod;
  case "-restartmax" then
    value = this.restartmax;
  case "-restarteps" then
    value = this.restarteps;
  case "-restartstep" then
    value = this.restartstep;
  case "-kelleystagnationflag" then
    value = this.kelleystagnationflag;
  case "-kelleynormalizationflag" then
    value = this.kelleynormalizationflag;
  case "-kelleystagnationalpha0" then
    value = this.kelleystagnationalpha0;
  case "-restartflag" then
    value = this.restartflag;
  case "-restartdetection" then
    value = this.restartdetection;
  case "-restartsimplexmethod" then
    value = this.restartsimplexmethod;
  case "-boxnbpoints" then
    value = this.boxnbpoints;
  case "-nbineqloops" then
    value = this.nbineqloops;
  case "-ineqscaling" then
    value = this.ineqscaling;
  else
    // Delegate to the optimization object
    value = optimbase_cget ( this.optbase , key );
  end
endfunction
