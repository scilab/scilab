// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - INRIA - Serge STEER
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//============================================================================================
//=================================fft(A [,isn [,flag]]) =====================================
//============================================================================================

//1-D transform ******************************************************************************
//R2C case ------------------------------------------------------------------------------------
assert_checkequal(fft(1:4),[10,-2+%i*2,-2,-2-%i*2]);
assert_checkequal(ifft(1:4),conj(fft(1:4)/4));
assert_checkequal(fft(1:4,-1),[10,-2+%i*2,-2,-2-%i*2]);
assert_checkequal(fft(1:4,1),conj(fft(1:4,-1)/4));
//C2R case ------------------------------------------------------------------------------------
//         ODD case
A=[1 0 %i -%i 0];
y=fft(A);
y_ref=[1, 2.17557050458494672, -0.902113032590307062, 2.90211303259030728, -0.17557050458494649];
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);
assert_checkalmostequal(A,ifft(y));

y=fft(A,"symmetric");
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);

y=fft(A,"nonsymmetric");
assert_checktrue(~isreal(y));
assert_checkalmostequal(y,y_ref);


y=fft(A,-1);
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);
assert_checkalmostequal(A,fft(y,1));

//         EVEN case
A=[1 0 %i 2 -%i 0];
y=fft(A);
y_ref=[3, 0.73205080756887719, 1.2679491924311228,-1, 4.7320508075688767,-2.73205080756887719];
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);
assert_checkalmostequal(A,ifft(y),0,10*%eps);

y=fft(A,"symmetric");
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);

y=fft(A,"nonsymmetric");
assert_checktrue(~isreal(y));
assert_checkalmostequal(y,y_ref);

y=fft(A,-1);
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);
assert_checkalmostequal(A,fft(y,1),10*%eps,10*%eps);

//R2R case ------------------------------------------------------------------------------------
//         ODD case
y=fft(1);
assert_checktrue(isreal(y));
assert_checkequal(y,1);

A=[0 1 2 2 1];
y=fft(A);
y_ref=[6,-2.61803398874989446, -0.381966011250104875, -0.381966011250104875, -2.61803398874989446];
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);
assert_checkalmostequal(A,ifft(y),10*%eps,10*%eps);

y=fft(A,"symmetric");
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);

y=fft(A,"nonsymmetric");
assert_checktrue(~isreal(y));
assert_checkalmostequal(y,y_ref);

//        EVEN case
A=1:2;
y=fft(A);
assert_checktrue(isreal(y));
assert_checkalmostequal(y,[3 -1]);
assert_checkalmostequal(A,ifft(y),10*%eps,10*%eps);

A=[0 1 2 5 2 1];
y=fft(A);
y_ref=[11,-6, 2,-3, 2,-6];
assert_checktrue(isreal(y));
assert_checkequal(y,y_ref);
assert_checkalmostequal(A,ifft(y),10*%eps,10*%eps);
y=fft(A,"symmetric");
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);

y=fft(A,"nonsymmetric");
assert_checktrue(~isreal(y));
assert_checkalmostequal(y,y_ref);


//C2C case ------------------------------------------------------------------------------------
A=[1+%i 2 5*%i -8+5*%i];
y=fft(A);
y_ref=[-5+%i*11,-4-%i*14, 7+%i, 6+%i*6];
assert_checkalmostequal(y,y_ref);
assert_checkalmostequal(A,ifft(y));

//2-D transform  *******************************************************************************

//R2R case  ------------------------------------------------------------------------------------
//         ODD X EVEN case
A=[0   1   1
   2  -1  -1
   3   4   4
   2  -1  -1];
y=fft(A);
y_ref=[ 13, 4, 4;
        -9, 0, 0;
        13,-8,-8;
        -9, 0, 0];

assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);
assert_checkalmostequal(A,ifft(y),0,10*%eps);

y=fft(A,"symmetric");
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);

y=fft(A,"nonsymmetric");
assert_checktrue(~isreal(y));
assert_checkalmostequal(y,y_ref);



//         EVEN X EVEN case
A=[0   1  5  1
   2  -1  6 -1
   3   4  7  4
   2  -1  6 -1];
y=fft(A);
y_ref=[37, -17, 25,-17;
       -11, -1,  1, -1;
        13, -1,-15, -1;
       -11, -1,  1, -1];

assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);
assert_checkalmostequal(A,ifft(y),0,10*%eps);

y=fft(A,"symmetric");
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);

y=fft(A,"nonsymmetric");
assert_checktrue(~isreal(y));
assert_checkalmostequal(y,y_ref);


//         ODD X ODD case
A=[0  1  1
   2 -1 -1
   2 -1 -1];
y=fft(A);
y_ref=[ 2, 5, 5;
        2,-4,-4;
        2,-4,-4];

assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref); 
assert_checkalmostequal(A,ifft(y),0,10*%eps);

A=[0   1  5  1
   2  -1  6 -1
   3   4  7  4
   2  -1  6 -1];
y=fft(A);
y_ref=[37, -17, 25,-17;
       -11, -1,  1, -1;
        13, -1,-15, -1;
       -11, -1,  1, -1];

assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);
assert_checkalmostequal(A,ifft(y),0,10*%eps);

y=fft(A,-1,"symmetric");
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);

y=fft(A,-1,"nonsymmetric");
assert_checktrue(~isreal(y));
assert_checkalmostequal(y,y_ref);


A=[1 2 3 3 2;4 6 9 11 8;5 7 10 10 7;4 8 11 9 6];
y=fft(A);
y_ref=[ 126,-25.1803398874989490,-2.81966011250105097,-2.81966011250105097,-25.1803398874989490;
       -28, 10.391435051850296,-1.68915303351051138, 1.21701707851093177,-1.91929909685071642;
       -26, 6.2360679774997898, 1.76393202250021019, 1.76393202250021019, 6.2360679774997898;
       -28,-1.91929909685071642, 1.21701707851093177,-1.68915303351051138, 10.391435051850296];
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref); 
assert_checkalmostequal(A,ifft(y));

y=fft(A,-1,"symmetric");
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);

y=fft(A,-1,"nonsymmetric");
assert_checktrue(~isreal(y));
assert_checkalmostequal(y,y_ref);


//R2C case ------------------------------------------------------------------------------------
A=[1 2 3;7 3 9;-1 5 4;0 8 -3];
y=fft(A);
y_ref=[38,     -8.5-%i*4.33012701892219276,                -8.5+%i*4.33012701892219276;
      -2-%i*14, 18.722431864335455-%i*1.76794919243112281  -10.7224318643354568-%i*5.23205080756887675;               
      -10,     -5.5+%i*4.33012701892219276,                -5.5-%i*4.33012701892219276;
      -2+%i*14,-10.7224318643354568+%i*5.23205080756887675, 18.722431864335455+%i*1.76794919243112281];    
assert_checkalmostequal(y,y_ref);
assert_checkalmostequal(A,ifft(y),0,10*%eps);

y=fft(A,1);
y_ref=[3.16666666666666607,                       -0.70833333333333326+%i*0.36084391824351614,-0.70833333333333326-%i*0.36084391824351614;
      -0.16666666666666660+%i*1.16666666666666652, 1.56020265536128777+%i*0.14732909936926014,-0.89353598869462125+%i*0.43600423396407301;
      -0.83333333333333337,                       -0.45833333333333320-%i*0.36084391824351603,-0.45833333333333320+%i*0.36084391824351603;
      -0.16666666666666660-%i*1.16666666666666652,-0.89353598869462125-%i*0.43600423396407301,1.56020265536128777-%i*0.14732909936926014];
assert_checkalmostequal(y,y_ref);
//C2R case
//------------------------------------------------------------------------------------
//         ODD X EVEN case
A=[0     %i  -%i
   2+%i  -1  -1
   3      4   4
   2-%i  -1  -1];
y=fft(A);
y_ref=[ 11, 6.7320508075688767,   3.2679491924311228;
        -9, 4.7320508075688767,   1.2679491924311228;
        11,-5.26794919243112325, -8.7320508075688767;
       -13, 0.73205080756887719, -2.73205080756887719];
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);  
assert_checkalmostequal(A,ifft(y),0,10*%eps);
A=[1 2 3 3 2;4 6 9 11 8;5 7 10 10 7;4 8 11 9 6];
y=fft(A);
y_ref=[ 126,-25.1803398874989490,-2.81966011250105097,-2.81966011250105097,-25.1803398874989490;
       -28, 10.391435051850296,-1.68915303351051138, 1.21701707851093177,-1.91929909685071642;
       -26, 6.2360679774997898, 1.76393202250021019, 1.76393202250021019, 6.2360679774997898;
       -28,-1.91929909685071642, 1.21701707851093177,-1.68915303351051138, 10.391435051850296];
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref); 
assert_checkalmostequal(A,ifft(y));

y=fft(A,-1,"symmetric");
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);

y=fft(A,-1,"nonsymmetric");
assert_checktrue(~isreal(y));
assert_checkalmostequal(y,y_ref);


//         ODD X ODD case
A=[0     %i  -%i
   2+%i  -1  -1
   2-%i  -1  -1];
y=fft(A);
y_ref=[0,                    7.7320508075688767,   4.26794919243112325;
       1.73205080756887719,  0.46410161513775439, -3;
       -1.73205080756887719,-2.99999999999999956, -6.46410161513775350];

assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);  
assert_checkalmostequal(A,ifft(y),0,10*%eps);

y=fft(A,-1,"symmetric");
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);

y=fft(A,-1,"nonsymmetric");
assert_checktrue(~isreal(y));
assert_checkalmostequal(y,y_ref);


A=[1440, 32-%i*28,-102-%i*188,-102+%i*188, 32+%i*28;
   140-%i*52,-21+%i*14,-33+%i*8, 31+%i*77, 8+%i*169;
   30,-179+%i*40,-32-%i*175, 97-%i*107,-191-%i*147;
   20,-83-%i*162,-127+%i*71,-127-%i*71,-83+%i*162;
   30,-191+%i*147, 97+%i*107,-32+%i*175,-179-%i*40;
   140+%i*52, 8-%i*169, 31-%i*77,-33-%i*8,-21-%i*14];
y=fft(A);
y_ref=[ 600, 1201.23812370118321, 2697.02967953848838, 2102.2365384314553, 2399.49565832887311;
        1800.35521029058418, 1800.85854102712074, 897.335302928384408, 598.365730980120588,2102.75200480588182;
        2097.89073857917083, 901.733860383104911, 600.566385143489583, 602.768240434540644,1796.70756549178577;
        900, 1503.68526108760511, 2698.74007483319838, 598.106629651731282, 299.468034427465;
        2.10926142082939805, 1797.22849022236483, 902.940272151356453, 1500.39858431578159,2697.65660185757633;
        2399.64478970941582, 898.55653188654594, 2102.57661444176256, 601.137293239859559,2098.41798069032438];
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);  
assert_checkalmostequal(A,ifft(y),0,1000*%eps);


y=fft(A,-1,"symmetric");
assert_checktrue(isreal(y));
assert_checkalmostequal(y,y_ref);

y=fft(A,-1,"nonsymmetric");
assert_checktrue(~isreal(y));
assert_checkalmostequal(y,y_ref);


//C2C case ------------------------------------------------------------------------------------
A=[0      %i  3-%i
   2+%i   -1  -1
   3       4   4
   2-3*%i -1  -1];
y=fft(A);
y_ref=[14-%i*2, 5.23205080756887675+%i*0.59807621135331601,1.76794919243112281-%i*4.598076211353316;
       -4,      5.23205080756887675+%i*2.598076211353316,  1.76794919243112281-%i*2.598076211353316;
       14+%i*2,-6.7679491924311233+%i*4.598076211353316,  -10.2320508075688767-%i*0.59807621135331601;
       -12,    -2.76794919243112281+%i*2.598076211353316,  -6.23205080756887675-%i*2.598076211353316];
assert_checkalmostequal(y,y_ref);  
assert_checkalmostequal(A,ifft(y),0,10*%eps);


