//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - S/E - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

assert_checkalmostequal(erfc(1 + 2*%i), 1.536643565778565033991795559314192749442 + 5.049143703447034669543036958614140565553*%i, 1.e-10);
assert_checkalmostequal(erfc(-1 + 2*%i), 0.4633564342214349660082044406858072505579 + 5.049143703447034669543036958614140565553*%i, 1.e-10);
assert_checkalmostequal(erfc(1 + -2*%i), 1.536643565778565033991795559314192749442 + -5.049143703447034669543036958614140565553*%i, 1.e-10);
assert_checkalmostequal(erfc(-1 + -2*%i), 0.4633564342214349660082044406858072505579 + -5.049143703447034669543036958614140565553*%i, 1.e-10);
assert_checkalmostequal(erfc(9 + -28*%i), -0.3359473673830576996788000505817956637777e304 + 0.1999896139679880888755589794455069208455e304*%i, 1.e-10);
assert_checkalmostequal(erfc(21 + -33*%i), -0.3584459971462946066523939204836760283645e278 + -0.3818954885257184373734213077678011282505e280*%i, 1.e-10);
assert_checkalmostequal(erfc(1e3 + 1e3*%i), 0.0003979577342851360897849852457775473112748 + -0.00002801044116908227889681753993542916894856*%i, 1.e-10);
assert_checkalmostequal(erfc(-3001 + -1000*%i), 2 + 0*%i, 1.e-10);
assert_checkalmostequal(erfc(1e160 + -1e159*%i), 0 + 0*%i, 1.e-10);
assert_checkalmostequal(erfc(5.1e-3 + 1e-8*%i), 0.9942453161409651998655870094589234450651 + -0.1128349818335058741511924929801267822634e-7*%i, 1.e-10);
assert_checkalmostequal(erfc(0 + 2e-6*%i), 1 + -0.2256758334194034158904576117253481476197e-5*%i, 1.e-10);
assert_checkalmostequal(erfc(0 + 2*%i), 1 + -18.56480241457555259870429191324101719886*%i, 1.e-10);
assert_checkalmostequal(erfc(0 + 20*%i), 1 + -0.1474797539628786202447733153131835124599e173*%i, 1.e-10);
assert_checkalmostequal(erfc(0 + 200*%i), complex(1, -%inf), 1.e-10);
assert_checkalmostequal(erfc(2e-6 + 0*%i), 0.9999977432416658119838633199332831406314 + 0*%i, 1.e-10);
assert_checkalmostequal(erfc(2 + 0*%i), 0.004677734981047265837930743632747071389108 + 0*%i, 1.e-10);
assert_checkalmostequal(erfc(20 + 0*%i), 0.5395865611607900928934999167905345604088e-175 + 0*%i, 1.e-10);
assert_checkalmostequal(erfc(200 + 0*%i), 0 + 0*%i, 1.e-10);
assert_checkalmostequal(erfc(%inf + 0*%i), 0 + 0*%i, 1.e-10);
assert_checkalmostequal(erfc(-%inf + 0*%i), 2 + 0*%i, 1.e-10);
assert_checkalmostequal(erfc(0 + %inf*%i), complex(%nan, %nan), 1.e-10);
assert_checkalmostequal(erfc(0 + -%inf*%i), complex(%nan, %nan), 1.e-10);
assert_checkalmostequal(erfc(%inf + %inf*%i), %nan + %nan*%i, 1.e-10);
assert_checkalmostequal(erfc(%inf + -%inf*%i), %nan + %nan*%i, 1.e-10);
assert_checkalmostequal(erfc(%nan + %nan*%i), %nan + %nan*%i, 1.e-10);
assert_checkalmostequal(erfc(%nan + 0*%i), %nan + 0*%i, 1.e-10);
assert_checkalmostequal(erfc(0 + %nan*%i), 1 + %nan*%i, 1.e-10);
assert_checkalmostequal(erfc(%nan + %inf*%i), %nan + %nan*%i, 1.e-10);
assert_checkalmostequal(erfc(%inf + %nan*%i), %nan + %nan*%i, 1.e-10);
assert_checkalmostequal(erfc(88 + 0*%i ), 0 + 0*%i, 1.e-10);

assert_checkerror("erfc(''foo'')",[],999);
assert_checkerror("erfc(sparse([1,2;4,5;3,10],[1,2,3]))",[],999);
