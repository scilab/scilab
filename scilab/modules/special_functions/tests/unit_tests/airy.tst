// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON - Le Mans Université
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
// ------------------------------
// unit tests of airy() functions
// ------------------------------

// Check the result's size:
for o = list([], -3, rand(1,10), rand(10,1), rand(10,12), rand(3,4,2))
    assert_checkequal(size(airy(o)), size(o));
end

// With decimal numbers
// --------------------
// Checking values for z=0
// Ref data out of  W o l f r a m alpha
// Ai, Ai', Bi, Bi'
ref = [0.35502805388781724 -0.25881940379280680 0.61492662744600073 0.44828835735382636];
for k = 0:3
    assert_checkalmostequal(airy(k,0), ref(k+1), %eps);
end

// Checking values around 0
m = 10^(-1:-3:-150)';
m = [m -m];

// Other values
// Ref data out of  O c t a v e, checked up to rtol=1e-16 against W o l f r a m
// alpha for ~20 values sampled among all.
u = logspace(-10,7,18);
NaN = %nan; Inf = %inf;
AiRef =  [0.3550280538619353  0.3550280536289978  0.3550280512996231  0.3550280280058769  0.3550277950684134  0.3550254656937794  0.3550021719474971  0.3547692345431742  0.3524399188055506  0.329203129943538  0.1352924163128814  0.0000000001104753  0.   0.   0.   0.   0.   0.];
dAiRef = [-0.2588194037928068  -0.2588194037928068  -0.2588194037928068  -0.258819403792805  -0.2588194037926293  -0.2588194037750556  -0.2588194020177528  -0.258819226365053  -0.2588017386620673  -0.2571304219075862  -0.1591474412967932  -0.0000000003520634   0.   0.   0.   0.   0.   0.];
BiRef = [0.6149266274908295  0.6149266278942891  0.6149266319288842  0.6149266722748364  0.614927075734358  0.6149311103295744  0.6149714562818387  0.6153749159058796  0.6194096138808873  0.6598616901941892  1.2074235949528711  455641153.54823458  6.04122399667D+288  Inf  Inf  Inf  Inf  Inf];
dBiRef = [0.4482883573538264  0.4482883573538264  0.4482883573538264  0.4482883573538294  0.4482883573541339  0.448288357384573  0.4482883604286089  0.4482886649665696  0.4483192531167072  0.4515126311496465  0.9324359333927754  1429236134.4828949  6.03971274531D+289  Inf  Inf  Inf  Inf  Inf];
AiScaledRef = [0.3550280538619355  0.3550280536290053  0.3550280512998598  0.3550280280133615  0.3550277953050986  0.3550254731783735   0.3550024086156907  0.3547767138141784  0.3526749570888103  0.3362170143618417  0.2635136447491401  0.1581236668543461  0.0891969209363304 ..
 0.0501641707499709  0.0282094762389028  0.0158633558512873  0.0089206205798346  0.];
dAiScaledRef = [-0.258819403792807  -0.2588194037928123  -0.2588194037929794  -0.2588194037982614  -0.2588194039651756  -0.2588194092314478  -0.2588195745640783  -0.2588246828109525  -0.2589743306787886  -0.2626087509259002  -0.3099768889605148  -0.5039093607113109  -0.8921920625040315  ..
 -1.586342905829885  -2.820948329126741  -5.0164336220411077  -8.9206205820647817  0.];
BiScaledRef = [ 0.614926627490829  0.6149266278942761  0.6149266319284743  0.6149266722618727  0.6149270753244066   0.6149310973656884  0.6149710463010045  0.6153619428003007  0.618996811754298   0.6460961952031843  0.6199119435726785  0.3183401053367412  0.1784310111708354  ..
0.1003290024731052  0.0564189642317552  0.0317267119115927  0.0178412411633862  0.];
dBiScaledRef = [ 0.4482883573538261  0.4482883573538169  0.4482883573535276  0.4482883573443787  0.4482883570552749  0.4482883479338247  0.4482880615698016  0.4482792143113184  0.4480204732189945  0.4420935438547972  0.4787285706049846  0.9985559426738586  1.783863754962809  ..
 3.1726565491304131  5.6418950127005294  10.03286715154597  17.841241158925861  0.];
Ref = list(AiRef, dAiRef, BiRef, dBiRef, AiScaledRef, dAiScaledRef, BiScaledRef, dBiScaledRef);
i = 1;
for scaled = 0:1
    for k = ["Ai" "dAi" "Bi" "dBi"]
        tmp = airy(k, u, scaled);
        ref = Ref(i);
        j = find(~isinf(ref) & ~isinf(tmp) & ~isnan(tmp));
        assert_checkalmostequal(tmp(j), ref(j), 500*%eps, 1e-16);
        i = i + 1;
    end
end

// With Complex numbers
// --------------------
// Check the property airy(k, z') = airy(k, z)'
m = rand(10,10);
z = (m-0.7)*10 + (m'-0.5)*10*%i;
for k = 0:3
    assert_checkalmostequal(airy(k,z'), airy(k,z)', 1e-13);
end

// Check some values, on concentric circles around 0
r = 10.^(-17:0);            // modulus
a = (0:30:359)/180*%pi;     // argument
[A, R] = meshgrid(a,r);
Z = R.*exp(%i*A);
load(SCI+"/modules/special_functions/tests/unit_tests/airy_complex.sod.ref");
// Ref data out of  O c t a v e, checked up to rtol=1e-16 against W o l f r a m
// alpha for ~10 values sampled among all.
ref = cat(3,AiRef,dAiRef,BiRef,dBiRef,AiRefScaled,dAiRefScaled,BiRefScaled,dBiRefScaled);
i = 1;
for scaled = 0:1
    for k = ["Ai" "dAi" "Bi" "dBi"]
        tmp = airy(k, Z, scaled);
        assert_checkalmostequal(tmp, ref(:,:,i), 50*%eps, 5e-16);
        i = i + 1;
    end
end

// Check error messages
// --------------------
assert_checkerror("airy()", "airy: Wrong number of input arguments: 1 to 3 expected.");
assert_checkerror("airy(1,2,1,4)", "Wrong number of input arguments.");
assert_checkerror("airy(%z)", "airy: Argument #1: Decimal or complex number expected.");
assert_checkerror("airy(sparse(1:4))", "airy: Argument #1: Decimal or complex number expected.");
assert_checkerror("airy(%z,3)", "airy: Argument #1: Integer number or string expected.");
msg = "airy: Argument #1: Must be in the set {""Ai"",""Bi"",""dAi"",""dBi"",0,1,2,3}.";
assert_checkerror("airy(-1,3)", msg);
assert_checkerror("airy(4,3)", msg);
assert_checkerror("airy(1.5,3)", msg);
assert_checkerror("airy(""Ci"",3)", msg);
