//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte Hecquet
// Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//Generate signal
x=%pi/10:%pi/10:102.4*%pi;
y=[1; 1] * sin(x) + [sin(2*x); sin(1.9*x)];

//Compute correlations
c=[];
for j=1:2
   for k=1:2
     c=[c;corr(y(k,:),y(j,:),64)];
   end
end
c=matrix(c,2,128);

//Find H,F,G with 6 states
hk=hank(20,20,c);
[H,F,G]=phc(hk,2,6);

//Solve Riccati equation
R0=c(1:2,1:2);
[P,R,T]=lindquist(100,H,F,G,R0);

assert_checkalmostequal(P,[52.953064387089377,-170.091664767326478;390.114441205931939,-1126.45220755477089;-2285.86604463640651,4579.41083686864113;1084.73413409944055,-1564.67420653525346;-5116.06921833499018,8743.68286689257002;-1976.40587294372335,3503.44251127613916]);

assert_checkalmostequal([-704.208398385009673,1053.5647942803439;1053.5647942827934,-1981.29618548816984], R);

assert_checkalmostequal(T, [-0.72821126450254769,-0.45461826616982365;-1.28940233637036883,-1.25076781365101763;0.25073933783035585,2.01963206598920664;3.13684440927242791,0.29283797163618436;-1.64165514197064755,1.68395444469620714;-1.61905018245414611,3.26102925315640091]);
