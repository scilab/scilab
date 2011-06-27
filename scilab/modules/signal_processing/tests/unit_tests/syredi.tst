// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

test_path = SCI + '/modules/signal_processing/tests/unit_tests/';

[fact,b2,b1,b0,c1,c0,zzeros,zpoles]=syredi(1,4,[2*%pi/10,4*%pi/10,0,0],0.02,0.001);
ref_fact = 0.005976;
if norm(fact-ref_fact) > 0.0000037 then pause,end;
if or(b2<>[1 1 1]) then pause,end;
if or(size(b1)<>[1 3]) then pause,end;
if b1(3)<> 1 then pause,end;
if or(b0<>[1 1 0]) then pause,end;
if or(size(c1)<>[1 3]) then pause,end;
if or(size(c0)<>[1 3]) then pause,end;
if c0(3)<> 0 then pause,end;
if or(size(zzeros)<>[1 5]) then pause,end;
if or(size(zpoles)<>[1 5]) then pause,end;

import_from_hdf5(test_path+"syredi.h5");
[fact1, b21, b11, b01, c11, c01, zzeros1, zpoles1] = syredi(1, 4, [1, 2, 0, 0], 0.02, 0.001);
if fact1 <> fact1_ref then pause, end
if b21 <> b21_ref then pause, end
if b11 <> b11_ref then pause, end
if b01 <> b01_ref then pause, end
if c11 <> c11_ref then pause, end
if c01 <> c01_ref then pause, end
if zzeros1 <> zzeros1_ref then pause, end
if zpoles1 <> zpoles1_ref then pause, end

[fact2, b22, b12, b02, c12, c02, zzeros2, zpoles2] = syredi(3, 4, [1, 2, 3, 3.01], 0.1, 0.001);
if fact2 <> fact2_ref then pause, end
if b22 <> b22_ref then pause, end
if b12 <> b12_ref then pause, end
if b02 <> b02_ref then pause, end
if c12 <> c12_ref then pause, end
if c02 <> c02_ref then pause, end
if zzeros2 <> zzeros2_ref then pause, end
if zpoles2 <> zpoles2_ref then pause, end

