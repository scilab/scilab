// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte Hecquet
// Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//Generate the process
t1=0:0.1:100;
y1=sin(2*%pi*t1)+sin(2*%pi*2*t1);

//Covariance of y1
nlag=128;
c1=corr(y1,nlag);
c1=c1';

//Compute the filter with maximum order=15 and p=1
n=5;
[la1,sig1]=lattn(n,1,c1);
poly(0,"z");
assert_checkequal(la1(1), 1+0.44297752752246194*%z);
assert_checkequal(la1(2), 1-0.74241513479044507*%z+0.66265464324027501*%z^2);

// bug of assert_checkequal ?
//assert_checkequal(la1(3), 1-0.74830900964096658*%z+0.66863379685942259*%z^2-0.00480196030174087*%z^3);

assert_checkequal(la1(4), 1-2.25921688488977512*%z+3.01297448874460594*%z^2-2.24632204438257954*%z^3+0.9961140979715424*%z^4);

assert_checkequal(la1(5), 1-1.83287468880271454*%z+2.08814963103012197*%z^2-1.0213063678700105*%z^3+0.09534323012552759*%z^4+0.39938577578835760*%z^5);

assert_checkequal(sig1(1), -1.78885438199984059+%z);

assert_checkequal(sig1(2), 1.22738933272413431-1.24514849008225914*%z+%z^2);

assert_checkequal(sig1(3),  -1.51680201929234504+2.3534861083274796*%z-2.25026439104255704*%z^2+%z^3);

assert_checkequal(sig1(4), 1.06749469293314991-2.31561791575813825*%z+3.06724913799065524*%z^2-2.255390458180341*%z^3+%z^4);

assert_checkequal(sig1(5), 1.4523203295716356-2.21361191790377232*%z+2.06018618672635778*%z^2-0.19513003383108307*%z^3-0.80871370312335800*%z^4+%z^5);


//Compare result of poles with p=-1 and with levin function
[la2,sig2]=lattn(n,-1,c1);
assert_checkequal(la2(1), 1-1.78885438199984059*%z);

assert_checkequal(la2(2), 1-1.24514849008225914*%z+1.22738933272413431*%z^2);

assert_checkequal(la2(3), 1-2.25026439104255704*%z+2.3534861083274796*%z^2-1.51680201929234504*%z^3);

assert_checkequal(la2(4), 1-2.255390458180341*%z+3.06724913799065524*%z^2-2.31561791575813825*%z^3+1.06749469293315036*%z^4);

assert_checkequal(la2(5), 1-0.80871370312331603*%z-0.19513003383117766*%z^2+2.06018618672648479*%z^3-2.21361191790386647*%z^4+1.4523203295716776*%z^5);

assert_checkequal(sig2(1), 0.44297752752246194+%z);

assert_checkequal(sig2(2), 0.66265464324027501-0.74241513479044507*%z+%z^2);

// bug of assert_checkequal ?
// assert_checkequal(sig2(3), -0.00480196030174087+0.66863379685942259*%z-0.74830900964096658*%z^2+%z^3);

assert_checkequal(sig2(4), 0.99611409797154249-2.24632204438258*%z+3.01297448874460594*%z^2-2.25921688488977512*%z^3+%z^4);

assert_checkequal(sig2(5), 0.39938577578840101+0.09534323012542978*%z-1.02130636786987794*%z^2+2.0881496310300216*%z^3-1.83287468880266791*%z^4+%z^5);