// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3567 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3567
//
// <-- Short Description -->
// gsort called with a complex array
warning("off");
c = [1 2 3]+%i;
v = gsort(c);
ref_v = [ 3. + %i      2. + %i      1. + %i  ];
if or(v <> ref_v) then pause,end


x = [5 8 1; 1 9 4];
y = [4 1 8; 1 4 9];
c = x+y*%i;
[v,ind] = gsort(c);
C = abs(c);
[v2,ind2] = gsort(C);

ref_ind = [     4.    3.    1.  ;    6.    5.    2. ];
ref_v = [  9. + 4.*%i    8. + %i      5. + 4.*%i  ;    4. + 9.*%i    1. + 8.*%i    1. + %i  ];
if or(ref_ind <> ind) then pause,end
if or(ref_v <> v) then pause,end
if or(ref_ind <> ind2) then pause,end

x = [3  1  5 ; 2 9 8];
y = [2  4  1 ; 4 1 3];
c = x+y*%i;
C = abs(c);

ref_values = [ 9. + %i      5. + %i      1. + 4.*%i  ;   8. + 3.*%i    2. + 4.*%i    3. + 2.*%i ];
ref_indices = [     4.    5.    3.  ;    6.    2.    1.];

[values_1,indices_1] = gsort(c);
if or(ref_values <> values_1) then pause,end
if or(ref_indices <> indices_1) then pause,end

ref_indices_C = [     4.    5.    3.  ;    6.    2.    1.];
[values_C, indices_C] = gsort(C);
if or(indices_C <> ref_indices_C) then pause,end

[values_2,indices_2] = gsort(c,'g');
if or(values_1 <> values_2) then pause,end
if or(indices_1 <> indices_2) then pause,end

ref_values_3 = [  3. + 2.*%i    2. + 4.*%i    8. + 3.*%i  ;    1. + 4.*%i    5. + %i      9. + %i  ];
ref_indices_3 = [    1.    2.    6.  ;    3.    5.    4.];

[values_3,indices_3] = gsort(c,'g','i');
if or(ref_values_3 <> values_3) then pause,end
if or(ref_indices_3 <> indices_3) then pause,end

[values_4,indices_4] = gsort(c,'g','d');
if or(ref_values <> values_4) then pause,end
if or(ref_indices <> indices_4) then pause,end

[values_gsort,indices_gsort] = gsort(c);
[values_sort,indices_sort] = gsort(c);
if or(values_gsort <> values_sort) then pause,end
if or(indices_gsort <> indices_sort) then pause,end


x = [3,1,5];
y = [2,4,1];
c = x+y*%i;
ierr = execstr("v = gsort(c,''lr'');","errcatch");
if ierr <> 0 then pause,end

ierr = execstr("v = gsort(c,''l'');","errcatch");
if ierr <> 999 then pause,end

ierr = execstr("v = gsort(c,''lc'');","errcatch");
if ierr <> 0 then pause,end

ierr = execstr("v = gsort(c,''rc'');","errcatch");
if ierr <> 999 then pause,end

ierr = execstr("v = gsort(c,''r'');","errcatch");
if ierr <> 0 then pause,end

ierr = execstr("v = gsort(c,''c'');","errcatch");
if ierr <> 0 then pause,end

