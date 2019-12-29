// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2019 - Samuel GOUGEON - Le Mans Université
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->

// --------------------------------
// unit tests of ellipj() functions
// --------------------------------

// Check the result's size
// =======================
for o = list([], -3, rand(1,10), rand(10,1), rand(10,12), rand(3,4,2))
    assert_checkequal(size(ellipj(o,0.1)), size(o));
end

// Special input value
// ===================
[sn,cn,dn] = ellipj(%inf, 0.5)
assert_checkequal([sn cn dn], [%nan %nan %nan]);

// ========
// sn(real)
// ========
ref = [// Reference values from  W o l f r a m   A l p h a
// x         m = 0.7         rtol
   0.   0                   0
   1.   0.7867623461751605  5e-16
   2.   0.9991470517177337  5e-16
   3.   0.8496055646182321  5e-16
   4.   0.1497633260257520  1e-15
   5.  -0.7090590533602067  5e-16
   6.  -0.9922509616227788  1e-15
   7.  -0.8991504331040869  %eps
   8.  -0.2939180408456228  %eps
   9.   0.6153795020583048  %eps
   10.  0.9780732803575124  %eps
   ];
for i = 1:size(ref,1)
    assert_checkalmostequal(ellipj(ref(i), 0.7), ref(i,2), ref(i,3));
    assert_checkalmostequal(ellipj(-ref(i), 0.7), -ref(i,2), ref(i,3));
end
// Powers of 10:
// x          m = 0.05         rtol          m = 0.5      rtol        m = 0.95        rtol
ref = [ // Reference values from  W o l f r a m   A l p h a
   1      0.8377707054731771  2e-16   0.8030018248956439, 5e-16   0.7658522624824511  5e-16
   10    -0.4378729750655788  2e-15   0.8588125059527787, 5e-16  -0.934978691527766   5e-16
   100   -0.9742526048177048  1e-15   0.1196019281577434, 2e-13  -0.811531427200948   1e-14
   1e3    0.7482430909578792  5e-14  -0.8878321984811046, 1e-13  -0.437315386732894   2e-12
   1e4    0.8624182984404760  2e-13   0.7384500010693715, 2e-12  -0.818571113592376   5e-12
   1e5    0.7988835860291854  2e-12  -0.9402218551681980, 5e-12  -0.2618682783394857  5e-10
   1e6   -0.2329089936053463  2e-10   0.8547923424610843, 1e-10  -0.998669351872119   2e-11
   1e7    0.7352980451000191  5e-10  -0.0220307669223278, 1e-7   -0.989185882675048   1e-9
   1e8   -0.9430613258095234  1e-9    0.2176954461361798, 1e-7   -0.3661544064410038  2e-7
   1e9   -0.2890611571169084  1e-7    0.9690463387009056, 5e-8   -0.972228411178568   2e-7
   1e10  -0.2448382176246635  2e-6   -0.2129864775603840, 1e-5   -0.992002095107755   1e-6
   1e11   0.6477036508616879  5e-6   -0.9771767833683986, 5e-6    0.3888373760830963  5e-4
   1e12  -0.6485071868288868  5e-5    0.6266882256650892, 2e-4    0.946251142823198   1e-4
   1e13  -0.6565030164219840  5e-4   -0.3692666652213536, 5e-3   -0.2663201863292489  5e-2
   1e14  -0.7321933463148297  5e-3    0.1188550465207018, 2e-1    0.998513062880542   2e-3
   ];
ref(:,[3 5 7]) = ref(:,[3 5 7])*2; // Linux is a bit less accurate than Windows
for i = 1:size(ref,1)
    assert_checkalmostequal(ellipj(ref(i), 0.05), ref(i,2), ref(i,3))
    assert_checkalmostequal(ellipj(ref(i), 0.50), ref(i,4), ref(i,5))
    assert_checkalmostequal(ellipj(ref(i), 0.95), ref(i,6), ref(i,7))
    // oddness
    assert_checkalmostequal(ellipj(-ref(i), 0.05), -ref(i,2), ref(i,3))
    assert_checkalmostequal(ellipj(-ref(i), 0.50), -ref(i,4), ref(i,5))
    assert_checkalmostequal(ellipj(-ref(i), 0.95), -ref(i,6), ref(i,7))
end

// ========
// cn(real)
// ========
ref = [// Reference values from  W o l f r a m   A l p h a
// x         m = 0.7         rtol
   0.   1                   0
   1.   0.61725603329652182 1e-15
   2.   0.04129369254208586 5e-13
   3.  -0.5274186046867658  2e-15
   4.  -0.9887218750375175  2e-16
   5.  -0.7051491039829290  5e-16
   6.  -0.1242498658295887  5e-14
   7.   0.4376396904403587  5e-16
   8.   0.9558306258252405  %eps
   9.   0.7882309740466133  %eps
   10.  0.2082610339230434  2e-15
   ];
for i = 1:size(ref,1)
    assert_checkalmostequal(ellipj("cn", ref(i), 0.7).cn,  ref(i,2), ref(i,3));
    assert_checkalmostequal(ellipj("cn",-ref(i), 0.7).cn,  ref(i,2), ref(i,3));
end

// =====================
// Complex input numbers
// =====================
// sn
// --
i = %i;
ref = [// Reference values from  W o l f r a m   A l p h a
// x+i       m = 0.2                             rtol
   0.   1.240256801726044*i                      1e-15
   1.   1.244235518007841+ 0.5361363537284757*i  %eps
   2.   1.357582954109075-0.2637195472417189*i   2e-15
   3.   0.5532930870698886-1.132321462346316*i   1e-15
   4.  -1.012560212495065-0.8336903517823439*i   2e-15
   5.  -1.393091630940202+0.01603358285570428*i  1e-12
   6.  -0.9703342870049057+0.8729818250007476*i  2e-15
   7.   0.6181470471828112+1.104123494298855*i   5e-15
   8.   1.3661439825585795+0.229895115817073*i   2e-15
   9.   1.221464554049673-0.5745426411640073*i   2e-15
   10. -0.07691854949353121-1.238248750082493*i  5e-15
   ];
ref(:,3) = ref(:,3)*2;   // for Linux
for k = 1:size(ref,1)
    assert_checkalmostequal(ellipj(ref(k)+i,  0.2),  ref(k,2), ref(k,3));
    assert_checkalmostequal(ellipj(-ref(k)-i, 0.2),  -ref(k,2), ref(k,3));
    assert_checkalmostequal(ellipj(ref(k)-i,  0.2),  conj(ref(k,2)), ref(k,3));
    assert_checkalmostequal(ellipj(-ref(k)+i,  0.2), -conj(ref(k,2)), ref(k,3));
end

// cn
// --
ref = [// Reference values from  W o l f r a m   A l p h a
// x+i       m = 0.2                             rtol
   0.   1.593184526107292                         5e-16
   1.   0.7411843154734415-0.90001890201640132*i  1e-15
   2.  -0.3745351729218371-0.9559079837754972 *i  2e-15
   3.  -1.4689832461724766-0.42648930073878082*i  2e-15
   4.  -1.1149172184787264+0.75715189053017862*i  1e-15
   5.   0.02302611466014416+0.9700399055568715*i  1e-12
   6.   1.16253366655224664+0.7286534756817488*i  2e-15
   7.   1.43624591822981084-0.4752046074165396*i  5e-15
   8.   0.3273265481638554-0.9595000187271159*i   1e-15
   9.  -0.7908838324816837-0.8873407726262612*i   1e-15
   10. -1.590888982052295+0.05986860104195037*i   5e-15
   ];
ref(:,3) = ref(:,3)*5; // for Linux
for k = 1:size(ref,1)
    assert_checkalmostequal(ellipj("cn",ref(k)+i,  0.2).cn,  ref(k,2), ref(k,3));
    assert_checkalmostequal(ellipj("cn",-ref(k)-i, 0.2).cn,  ref(k,2), ref(k,3));
    assert_checkalmostequal(ellipj("cn",ref(k)-i,  0.2).cn,  conj(ref(k,2)), ref(k,3));
    assert_checkalmostequal(ellipj("cn",-ref(k)+i, 0.2).cn,  conj(ref(k,2)), ref(k,3));
end


// ====================
// Check error messages
// ====================
msg = "ellipj: Wrong number of input arguments: 2 or 3 expected."
assert_checkerror("ellipj()", msg);
assert_checkerror("ellipj(1, 0.1, 2, 4)", msg);
assert_checkerror("ellipj(""ns"", 0.1, 2, 4)", msg);
msg = "ellipj: Wrong number of output arguments: 1 expected.";
assert_checkerror("[a,b]=ellipj(""sn"",3,0.1)", msg);
msg = "ellipj: Argument #1: Decimal or complex number expected.";
assert_checkerror("ellipj(%t, 0.2)", msg);
msg = "ellipj: Argument #2: Must be in the interval [0, 1].";
assert_checkerror("ellipj(1, 1.2)", msg);
msg = "ellipj: Argument #1: Text expected."
assert_checkerror("ellipj(1, 0.1, 2)", msg);
msg = "ellipj: Argument #2: Decimal or complex number expected.";
assert_checkerror("ellipj(""sn"", %t, 0.2)", msg);
msg = "ellipj: Argument #3: Must be in the interval [0, 1].";
assert_checkerror("ellipj(""sn"",1, 1.2)", msg);
