// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
assert_checkequal(atanm([]),[]);

A=diag([1 2]);
assert_checkalmostequal(atanm(A),diag(atan([1 2])),0,10*%eps);

A=[1 2;3 4];
assert_checkalmostequal(spec(atanm(A)),atan(spec(A)));
ref=[0.0600240193953584,0.6068858584890636;0.9103287877335954,0.9703528071289539];
assert_checkalmostequal(atanm(A),ref);

A=[1+%i -7 3;%i -2+3*%i 4; 1-%i -7+%i 33];
assert_checkalmostequal(gsort(spec(atanm(A))),gsort(atan(spec(A))));
ref=[ 0.4415511995553387+%i*0.0465571695952897, -2.215123566494861-%i*1.7166811969708002,  0.3121382834610655+%i*0.2222087245252373;
      -0.3347972138643110+%i*0.3449103918109163,-0.8085672628998303+%i*0.1405360719884442, 0.2967971776426956-%i*0.0116504734025363;
      -0.0308507640310242+%i*0.0364398724152414,-0.35975761750424+%i*0.0447753288197247,   1.5841244571549333-%i*0.0033552305228509];

assert_checkalmostequal(atanm(A),ref);

assert_checktrue(execstr("atanm(rand(3,4))","errcatch")<>0);

assert_checktrue(execstr("atanm(poly(1:3,''x'',''c''))","errcatch")<>0);

