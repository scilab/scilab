// ===================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) ????-2008 - ENPC
// Copyright (C) 2008 - DIGITEO - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
// Copyright (C) 2018-2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// ===================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

//================================ sparse complex ====================
//  Tests in gsort_sparse.tst
//
//================================ complex matrix ====================
/*
i = %i;
c  = [
i         -1. - i    -i         -i
1. - i    -1.         1.         1. + i
-1. + i     1.         0.         0.
i         -1. + i     1.         1. - i
0.         1. - i     1. + i     0.
1.        -1. - i    -1.        -1. + i
i         -1. + i    -1. + i    -i
1. - i    -1.         1.         1. + i
-1. + i    -i         -1.        -i
];
*/

x = [3  1  5 ; 2 9 8];
y = [2  4  1 ; 4 1 3];
c = x + y * %i;

ref_ind = [  4.    5.    3.  ;    6.    2.    1.  ];
ref_values = [ 9 +   %i, 5 +   %i,  1 + 4*%i
8 + 3*%i, 2 + 4*%i,  3 + 2*%i ];
[v,ind] = gsort(c);
assert_checkequal(ref_ind, ind);
assert_checkequal(ref_values, v);

[v1,ind1] = gsort(abs(c));
[v2,ind2] = gsort(c);
assert_checkequal(ind1, ind2);

A = [18 21 10 7 5];
B = [1  3  22 8 4];
y = complex(A,B);
[a,b] = gsort(y);
assert_checkequal(b, [3 2 1 4 5]);
assert_checkequal(y(b), a);

refMsg = msprintf(_("%s: Argument #%d: Must be in the set {%s}.\n"), "gsort", 2, "''g'',''r'',''c'',''lc'',''lr''");
assert_checkerror("[a,b] = gsort(y,''l'')", refMsg);

ierr = execstr("[a,b] = gsort(y,''g'');","errcatch");
assert_checkequal(ierr, 0);

ierr = execstr("[a,b] = gsort(y,''r'');","errcatch");
assert_checkequal(ierr, 0);

ierr = execstr("[a,b] = gsort(y,''c'');","errcatch");
assert_checkequal(ierr, 0);

ierr = execstr("[a,b] = gsort(y,''lc'');","errcatch");
assert_checkequal(ierr, 0);

ierr = execstr("[a,b] = gsort(y,''lr'');","errcatch");
assert_checkequal(ierr, 0);

// -------------------------------------------------------------------
// Error messages
// -------------------------------------------------------------------
msg = msprintf(_("%s: Argument #%d: Must be in the set {%s}.\n"), "gsort", 2, "''g'',''r'',''c'',''lc'',''lr''");
assert_checkerror("gsort(%i,''q'')", msg);
msg = msprintf(_("%s: Argument #%d: Text(s) expected.\n"), "gsort", 3);
assert_checkerror("gsort(%i,''g'',1)", msg);

msg = msprintf(_("%s: Argument #%d: Must be in the set {%s}.\n"), "gsort", 3, "''i'',''d''");
assert_checkerror("gsort(%i,''g'',''a'')", msg);
msg = "gsort: Argument #4: List expected.";
assert_checkerror("gsort(%i,''g'',''i'', 1)", msg);
msg = "gsort: Arguments #3 and #4: Same numbers of elements expected.";
assert_checkerror("gsort(%i,''g'',''i'', list())", msg);
assert_checkerror("gsort(%i,''g'',''i'', list(real, imag))", msg);
msg = "gsort: Arguments #3 and #4: Same numbers of elements expected.";
assert_checkerror("gsort(%i,''g'',''i'', list())", msg);
msg = "gsort: Argument #4: List of functions identifiers expected.";
assert_checkerror("gsort(%i,''g'',''i'', list(1))", msg);

// ===================================================================
//              Other tests with DENSE matrices of COMPLEX numbers
// ===================================================================
function r = getBuiltinName(b)
    select b
    case real
        r = "real"
    case imag
        r = "imag"
    case abs
        r = "abs"
    case atan
        r = "atan"
    else
        r = "unknown"
    end
endfunction

function r = getDecimalData(C, crit)
    if crit==atan
        r = atan(imag(C), real(C))
    else
        r = crit(C)
    end
endfunction
function checkComplexOrderVec(Vs1, Vs2, Vks, ord2)
    for j = 2:length(Vs1)
        mprintf("%d ",j);
        if Vs1(j)==Vs1(j-1)
            if ord2=="i"
                assert_checktrue(Vs2(j)>=Vs2(j-1));
            else
                assert_checktrue(Vs2(j)<=Vs2(j-1));
            end
            if Vs2(j)==Vs2(j-1) // => check initial order preserved
                assert_checktrue(Vks(j)>Vks(j-1));
            end
        end
    end
endfunction
// -------------------------------------------------------------------
function checkComplexOrder(C, Cs, ks, method, order, crits)
    ord1 = order(1);
    Cs1 = getDecimalData(Cs, crits(1));
    // Checking the order according to the first criteria:
    //   If the Cs order is correct, its first criteria part should have the
    //   same order than the sorted C (necessary but not sufficient condition).
    //   This should be true for all methods, but only for values: Indices of
    //   equal first part values can be changed by the second criteria
    // FIRST CRITERION
    // ---------------
    C1 = getDecimalData(C, crits(1));
    [C1ref, k1] = gsort(C1, method, ord1);
    if and(method~=["lr" "lc"]) | (size(order,"*")==1 | size(crits)<2) then
        assert_checkalmostequal(Cs1, C1ref);
    end
    // SINGLE CRITERIA
    // ---------------
    if size(order,"*")==1 | size(crits)<2
        // then we can compare directly k1 and ks: they must be equal
        assert_checkequal(ks, k1);
        // We assume that gsort() works correctly with reals.
        // So we do not check here the actual order of Cs1 values and the
        // correctness of ks indices.
        // These checking should be done by tests with reals.
        mprintf("\n");
        return
    end
    // SECOND CRITERIA
    // ---------------
    ord2 = order(2);
    if or(method==["g" "r" "c"]) then
        Cs2 = getDecimalData(Cs, crits(2));
        if method=="g"
            checkComplexOrderVec(Cs1, Cs2, ks, ord2);
        elseif method=="r"
            for col = 1:size(C,2)
                checkComplexOrderVec(Cs1(:,col), Cs2(:,col), ks(:,col), ord2);
            end
        elseif method=="c"
            for row = 1:size(C,1)
                checkComplexOrderVec(Cs1(row,:), Cs2(row,:), ks(row,:), ord2);
            end
        end
        mprintf("\n");
    else // lr, lc
        mprintf("  %s: values unchecked\n", method);
    end
endfunction

clc
[nr, nc] = (6,6);
r = grand(nr, nc, "uin", -1, 1);
i = grand(nr, nc, "uin", -1, 1);
C = r + i*%i;
sizeC = size(C);
colNan = ones(C(:,1))*%nan;
crits = list(real, imag, abs, atan);

// ================
// SINGLE CRITERION
// ================
methods = ["g" "r" "c" "lr" "lc"];
for m = methods
    for  sdir = ["i" "d"]
        for c = crits
            mprintf("%s  %s  %s", m, sdir, getBuiltinName(c));
            [sC, k]  = gsort(C, m, sdir, list(c));
            checkComplexOrder(C, sC, k, m, sdir, list(c));
        end
    end
end
// -------------------------------------------------------------------
// ============
// TWO CRITERIA
// ============
[nr, nc] = (10,3);
r = grand(nr, nc, "uin",-1,1);
i = grand(nr, nc, "uin",-1,1);
C = r+i*%i;
sizeC = size(C);
colNan = ones(C(:,1))*%nan;

methods = ["g" "c" "r" "lr" "lc"];
crits = list(real, imag, abs, atan);
for m = methods
    for sdir = ["i" "i" ; "i" "d" ; "d" "i" ; "d" "d"]'
        for c1 = crits
            for c2 = crits
                if c1 == c2
                    continue
                end
                crit = list(c1, c2);
                mprintf("%s  %s  %s  %s : ", m, strcat(sdir), ..
                getBuiltinName(c1), getBuiltinName(c2));
                [sC, k]  = gsort(C, m, sdir, crit);
                // Check values
                checkComplexOrder(C, sC, k, m, sdir, crit);
                // Check indices:
                if m=="g"
                    assert_checkequal(sC, matrix(C(k),sizeC));
                elseif m=="c"
                    // get linearized indices:
                    ak = (1:nr)'*ones(1,nc)+(k-1)*nr;
                    assert_checkequal(sC, matrix(C(ak),sizeC));
                elseif m=="r"
                    // get linearized indices:
                    ak = ones(nr,1)*(0:nc-1)*10 + k;
                    assert_checkequal(sC, matrix(C(ak),sizeC));
                elseif m=="lc"
                    assert_checktrue(isrow(k));
                    assert_checkequal(sC, C(:,k));
                else // m=="lr"
                    assert_checktrue(iscolumn(k));
                    assert_checkequal(sC, C(k,:));
                end
            end
        end
    end
end

// -------------------------------------------------------------------
// Incomplete sorting
// -------------------------------------------------------------------
m = complex([7  6  9  2  8  1  0  4  3  2], 0);
assert_checkequal(gsort(m, "g", "i", list(atan)), m);
assert_checkequal(gsort(m, "g", "i", list(imag)), m);
//assert_checkequal(gsort(m, "g", ["i" "i"], list(imag, atan)), m);
// zeros(n,2) is actually sorted, while it should be preserved. TO BE INVESTIGATED
assert_checkequal(gsort(m+%i, "g", "d", list(imag)), m+%i);

// -------------------------------------------------------------------
// Double sorting criteria with Nan values in the secondary criterion
// -------------------------------------------------------------------
c = [-1 1 ; -1 0; 0 2; 0 %nan; 0 -1; 0 %inf ; 0 1; 1 %nan ; 1 1; 1 -1];
c = complex(c(:,1), c(:,2))
// 1   -1. +    i
// 2   -1.
// 3          2.i
// 4         Nani
// 5           -i
// 6         Infi
// 7            i
// 8    1. + Nani
// 9    1. +    i
// 10   1. -    i
[v, k] = gsort(c, "g", "i");
assert_checkequal(k, [2  5  7  1  9  10  3  6  4  8]');
[v, k] = gsort(c, "g", ["i" "i"], list(real, imag));
assert_checkequal(k, [2  1  5  7  3  6  4  10  9  8]');
[v, k] = gsort(c,"g", ["i" "d"], list(real, imag));
assert_checkequal(k, [1  2  4  6  3  7  5  8  9  10]');
[v, k] = gsort(c,"g", ["d" "i"], list(real, imag));
assert_checkequal(k, [10  9  8  5  7  3  6  4  2  1]');
