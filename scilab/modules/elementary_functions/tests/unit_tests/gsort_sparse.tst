// ===================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) ????-2008 - ENPC
// Copyright (C) 2008 - DIGITEO - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
// Copyright (C) 2018 - 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// ===================================================================
//
// Included gsort() tests:
//  * sparse reals (all sizes)
//  * sparse complex (all sizes), including multi-level sorting
//  * sparse boolean
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// ========================= sparse reals ============================

b = [5 1 3 2 4 ; 6 1 2 4 1];
c = full(sprand(10,8,0.1));
objects = list(b(:)', b(:), b, c(:)', c(:), c);
for method = ["g" "lr" "lc" "r" "c" ] //  
    mprintf("\n%s : ", method);
    for o = objects
        O = sparse(o);
        for order = ["i" "d"]
            mprintf("%s ",order)
            [s,k] = gsort(o, method, order);
            [S,K] = gsort(O, method, order);
            assert_checktrue(issparse(S));
            assert_checkequal(full(S), s);
            assert_checkequal(K, k);
        end
    end
end

// Uniform row, column or matrix of zeros or of ones
objects = list(spzeros(1,10), spzeros(10,1), spzeros(6,8))
for a = [0 1]
    for o = objects
        sz = size(o);
        z = o + a;
        [s, k]= gsort(z, "g");
        assert_checkequal(s, z);
        assert_checkequal(k, matrix(1:prod(sz),sz(1),-1));
        [s, k]= gsort(z, "r");
        assert_checkequal(k, (1:sz(1))'*ones(1,sz(2)));
        [s, k]= gsort(z, "c");
        assert_checkequal(k, ones(sz(1),1)*(1:sz(2)));
        [s, k]= gsort(z, "lr");
        assert_checkequal(k, (1:sz(1))');
        [s, k]= gsort(z, "lc");
        assert_checkequal(k, 1:sz(2));
    end
end

// ========================== sparse complex =========================

// Sparse column of complex numbers (< 2018)
// -----------------------------------------
A = [1 -%i;%i 0];
A1 = sparse(A);
B = [1 1+%i 4; -2*%i 3 3-%i];
B1 = sparse(B);
C = [-%i 3*%i;4 9;-2*%i 7];
C1 = sparse(C);
mprintf("\nSparse real:\n");
for order = ["i" "d"]
    for method = ["g" "r" "c" "lr" "lc"]
        c = gsort(A(:), method, order);
        d = full(gsort(A1(:), method, order));
        assert_checkequal(c, d);

        c = gsort(B(:), method, order);
        d = full(gsort(B1(:), method, order));
        assert_checkequal(c, d);

        [s,k] = gsort(C(:), method, order);
        [s1,k1] = gsort(C1(:), method, order);
        assert_checkequal(full(s1), s);
        assert_checkequal(k1, k);
    end
end

// Sparse matrix of complex numbers
// --------------------------------
c0 = sprand(50,10,0.2) + imult(sprand(50,10,0.2));
objects = list(c0(:).', c0(:), c0);
Crit = list(list(abs, atan), list(real, imag));
Orders = ["i" "i" ; "i" "d"];
mprintf("\nSparse complex:\n");
for method = ["g" "r" "c" "lr" "lc"]
    mprintf("\n%s : ", method);
    for O = objects
        for j = 1:2
            order = Orders(j,:);
            crit = Crit(j);
            mprintf("%s ", strcat(order));
            S = gsort(O, method, order, crit);
            s = gsort(full(O), method, order, crit);
            assert_checktrue(issparse(S));
            assert_checkequal(s, full(S));

            [S, K] = gsort(O, method, order, crit);
            [s, k] = gsort(full(O), method, order, crit);
            assert_checktrue(issparse(S));
            assert_checkequal(K, k);
            assert_checkequal(s, full(S));
        end
    end
end

// ========================= sparse boolean =========================

b = sprand(10,500,0.1) > 0;
objects = list(b(:)', b(:), b);
mprintf("\nSparse booleans:\n");
for method = ["g" "r" "c" "lr" "lc"]
    mprintf("\n%s : ", method);
    for O = objects
        for order = ["i" "d"]
            mprintf("%s ", order);
            [S, K] = gsort(O, method, order);
            [s, k] = gsort(full(O), method, order);
            assert_checktrue(issparse(S));
            assert_checkequal(K, k);
            assert_checkequal(full(S), s);

            S = gsort(O, method, order);
            assert_checktrue(issparse(S));
            assert_checkequal(full(S), s);
        end
    end
end

