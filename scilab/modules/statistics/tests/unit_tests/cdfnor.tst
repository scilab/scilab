// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// Run with test_run('statistics','cdfnor',['no_check_error_output']);

//
// References
// Assessing the quality of the Normal distribution function
// References
//   Yalta, A. T. 2008. The accuracy of statistical distributions in Microsoft®Excel 2007. Comput. Stat. Data Anal. 52, 10 (Jun. 2008), 4579-4586. DOI= http://dx.doi.org/10.1016/j.csda.2008.03.005
//   Computation of Statistical Distributions (ELV), Leo Knüsel
// Table 6
// Check inverse of normal law
//

// Table from Yalta, 2008.
table = [
0.5 , 0.0
1.e-1 , -1.28155
1.e-2 , -2.32635
1.e-3 , -3.09023
1.e-4 , -3.71902
1.e-5 , -4.26489
1.e-6 , -4.75342
1.e-7 , -5.19934
1.e-15 , -7.94135
1.e-16 , -8.22208
1.e-100 , -21.2735
1.e-197 , -29.9763
1.e-198 , -30.0529
1.e-300 , -37.0471
];

precision = 1.e-5;
nt = size(table,"r");
for k = 1 : nt
    p = table(k,1);
    expected = table(k,2);
    q = 1 - p;
    Mean = 0;
    Std = 1;
    computed = cdfnor ( "X" , Mean , Std , p , q );
    assert_checkalmostequal ( computed , expected , precision );
end

//
// Values from R-2.8.1
// table = [x mu sigma PDF-P CDF-P CDF-Q]
// Some tests do not pass with Scilab.
//
// See : http://bugzilla.scilab.org/show_bug.cgi?id=8032
//
// Prints the number of accurate digits.
table = [
1.630146181031128094D+01 1.000000000000000000D+00 2.000000000000000000D+00 3.885547484725481156D-14 9.999999999999900080D-01 9.992007221626602924D-15
1.372268177939483991D+01 1.000000000000000000D+00 2.000000000000000000D+00 3.255794261707647101D-10 9.999999998999999917D-01 1.000000082740384053D-10
1.139867516458132002D+01 1.000000000000000000D+00 2.000000000000000000D+00 2.689766239123555948D-07 9.999999000000000526D-01 9.999999994736501079D-08
9.529781587847679702D+00 1.000000000000000000D+00 2.000000000000000000D+00 2.239366490571142939D-05 9.999900000000000455D-01 9.999999999954530395D-06
8.438032970911416797D+00 1.000000000000000000D+00 2.000000000000000000D+00 1.979239833799469940D-04 9.999000000000000110D-01 9.999999999998900014D-05
7.180464612335624608D+00 1.000000000000000000D+00 2.000000000000000000D+00 1.683545038532002075D-03 9.989999999999999991D-01 1.000000000000003924D-03
5.652695748081682403D+00 1.000000000000000000D+00 2.000000000000000000D+00 1.332607110172901940D-02 9.899999999999999911D-01 1.000000000000000021D-02
3.563103131089199849D+00 1.000000000000000000D+00 2.000000000000000000D+00 8.774916596624346421D-02 8.999999999999999112D-01 1.000000000000001027D-01
2.683242467145829036D+00 1.000000000000000000D+00 2.000000000000000000D+00 1.399809602039041034D-01 8.000000000000001554D-01 1.999999999999999001D-01
1.506694206271600001D+00 1.000000000000000000D+00 2.000000000000000000D+00 1.931712667484301871D-01 6.000000000000000888D-01 3.999999999999999112D-01
1.000000000000000000D+00 1.000000000000000000D+00 2.000000000000000000D+00 1.994711402007164069D-01 5.000000000000000000D-01 5.000000000000000000D-01
-6.832424671458280363D-01 1.000000000000000000D+00 2.000000000000000000D+00 1.399809602039041867D-01 2.000000000000000111D-01 8.000000000000000444D-01
-1.563103131089200959D+00 1.000000000000000000D+00 2.000000000000000000D+00 8.774916596624339482D-02 1.000000000000000056D-01 9.000000000000000222D-01
-3.652695748081681959D+00 1.000000000000000000D+00 2.000000000000000000D+00 1.332607110172904022D-02 1.000000000000001062D-02 9.899999999999999911D-01
-7.529781587845650215D+00 1.000000000000000000D+00 2.000000000000000000D+00 2.239366490580828930D-05 9.999999999999960160D-06 9.999900000000000455D-01
-1.172268180480810962D+01 1.000000000000000000D+00 2.000000000000000000D+00 3.255793998537787058D-10 1.000000000000008954D-10 9.999999998999999917D-01
-1.752468017959679969D+01 1.000000000000000000D+00 2.000000000000000000D+00 4.683961267403037244D-20 1.000000000000072017D-20 1.000000000000000000D+00
-2.886667506957698137D+01 1.000000000000000000D+00 2.000000000000000000D+00 7.499857137115675432D-50 9.999999999999744883D-51 1.000000000000000000D+00
];

precision = 1.e-12;
precinv = 1.e-8;

nt = size(table,"r");
for k = 1 : nt
    x = table(k,1);
    mu = table(k,2);
    std = table(k,3);
    p = table(k,5);
    q = table(k,6);
    [ p1 , q1 ] = cdfnor("PQ",x,mu,std);
    x1 = cdfnor("X",mu,std,p,q);
    mu1 = cdfnor("Mean",std,p,q,x);
    std1 = cdfnor("Std",p,q,x,mu);
    if ( %t ) then
        assert_checkalmostequal ( p1 , p , precision );
        assert_checkalmostequal ( q1 , q , precision );
        assert_checkalmostequal ( x1 , x , precinv );
        assert_checkalmostequal ( mu1 , mu , precinv );
        if ( ~isnan(std1) ) then
            assert_checkalmostequal ( std1 , std , precinv );
        end
    end
    if ( %f ) then
        dP = assert_computedigits ( p1 , p );
        dQ = assert_computedigits ( q1 , q );
        dx = assert_computedigits ( x1 , x );
        dmu = assert_computedigits ( mu1 , mu );
        dstd = assert_computedigits ( std1 , std );
        mprintf("Test #%3d/%3d: Digits p1= %.1f, q1=%.1f, X=%.1f, M=%.1f, S=%.1f\n",k,nt,dP,dQ,dx,dmu,dstd);
    end
end

// IEEE support
// See http://bugzilla.scilab.org/show_bug.cgi?id=7296
Mean = 0;
Std = 1;

x = %inf; // Inf
[P, Q] = cdfnor("PQ", x, Mean, Std);
assert_checkequal(P, 1);
assert_checkequal(Q, 0);

x = -%inf; // -Inf
[P, Q] = cdfnor("PQ", x, Mean, Std);
assert_checkequal(P, 0);
assert_checkequal(Q, 1);

x = %nan; // NaN
[P, Q] = cdfnor("PQ", x, Mean, Std);
assert_checkequal(P, %nan);
assert_checkequal(Q, %nan);
