function [x,Z,z,ul,iters] = phase1(F,blck_szs,M,nu,abstol,maxiters);
// [x,Z,z,ul,iters] = phase1(F,blck_szs,M,nu,abstol,maxiters);
//
// Find an x s.t. F(x) > 0 and Tr F(x) < M 
//    or prove that no such x exists.

// Copyright INRIA
// minimize    t 
// subject to  F(x) + t*I = F_0 + x_1*F_1 + ... + x_m*F_m + t*I >= 0 
//             Tr F(x) <= M
//
// maximize    -Tr F_0*(Z-z*I) - M*z
// subject to  0 = Tr F_i*(Z - z*I)
//             1 = Tr Z 
//             Z >= 0, z >= 0
//
// Convergence criterion:
// (1) maxiters is exceeded
// (2) duality gap is less than abstol
// (3) primal objective is less than zero or dual objective is greater
//     than zero
//  
// Input arguments:
// F:         (sum_i n_i^2) x (m+1) matrix
//            [ vec(F_0(1)) vec(F_1(1)) ... vec(F_m(1)) ]
//            [ vec(F_0(2)) vec(F_1(2)) ... vec(F_m(2)) ]
//                ...        ...              ...
//            [ vec(F_0(L)) vec(F_1(L)) ... vec(F_m(L)) ]
//            where F_i(j) denotes the jth block of F_i, 
//            F_i(j) has size n_i x n_i
// blck_szs:  L-vector [n_1 ... n_L], contains dimensions of blocks
// M:         scalar
// nu:        >= 1.0
// abstol:    absolute tolerance
// maxiters:  maximum number of iterations
//
// Output arguments:
// x:         m-vector: 
// Z:         block-diagonal matrix stored as 
//            [ vec(Z(1)); vec(Z(2)); ... ; vec(Z(L)) ]
//            where Z(i) is the ith block
// z:         scalar part of last dual iterate, 
// ul:        2-vector, primal and dual objective
// iters:     scalar
if type(F)==5 then F=full(F);end
[mf,nf]=size(F)
m = nf-1;
if (mf ~= sum(blck_szs^2)) 
   error('Dimensions of F do not match blck_szs.');
end;

// mineigF is the smallest eigenvalue of F_0
mineigF = 0.0;
blck_szs=matrix(blck_szs,1,prod(size(blck_szs)));
k=0; for n=blck_szs,
   mineigF = min(mineigF, min(real(spec(matrix(F(k+[1:n*n],1),n,n)))));
   k=k+n*n;   // k = sum n_i*n_i 
end;

// I is the identity
I = zeros(mf,1);  
k=0; for n=blck_szs,
   I(k+[1:n*n]) = matrix(eye(n,n),n*n,1);   // identity
   k=k+n*n;   // k = sum n_i*n_i 
end;

if (M < I'*F(:,1)+1e-5), 
   error('M must be strictly greater than the trace of F_0.'); 
end;

// initial x0 
x0 = [zeros(m,1); max(-1.1*mineigF, 1e-5)];

// Z0 is the projection of I on the space Tr F_i*Z = 0
Z0 = I - F(:,2:m+1) * ( F(:,2:m+1) \ I );
// mineigZ is the smallest eigenvalue of Z0
mineigZ = 0.0;
k=0; for n=blck_szs,
   mineigZ = min( mineigZ, min(real(spec(matrix(Z0(k+[1:n*n]),n,n)))) );
   k=k+n*n;   // k = sum n_i*n_i 
end;

// z = max ( 1e-5, -1.1*mineigZ)
Z0(k+1) = max( -1.1 * mineigZ, 1e-5 );  // z  
Z0(1:k) = Z0(1:k) + Z0(k+1)*I; 
Z0 = Z0 / (I'*Z0(1:k));    // make Tr Z0 = 1
// add scalar block Tr F(x) <= M

F = [F, I; M-I'*F(:,1), -I'*F(:,2:m+1), 0]; 
blck_szs = [blck_szs,1];
c = [zeros(m,1); 1];
//Pack Z0 and F
Z0=pack(Z0,blck_szs);F=pack(F,blck_szs);
//pause;
[x,Z,ul,info]=semidef(x0,Z0,F,blck_szs,c,[nu,abstol,-1,0,maxiters]);
nz=prod(size(Z))
z=Z(nz)
Z=unpack(Z(1:nz-1),blck_szs(1:prod(size(blck_szs))-1))
Z = Z(1:k);
x = x(1:m);
iters = info(2);
select info(1)
case 1
     error('Max. iters. exeeded');
case 2 then
     disp('Absolute accuracy reached');
case 3 then
     disp('relative accuracy reached');
case 4 then
     disp('target value reached');
case 5 then
     error('target value not achievable');
       else
     error('semidef fails');
end
