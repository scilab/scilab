function [x,Z,z,ul,iters]=bigM(F,blck_szs,c,x0,M,nu,abstol,reltol,tv,maxiters);
// [x,Z,z,ul,iters]=bigM(F,blck_szs,c,x0,M,nu,abstol,reltol,tv,maxiters);
//
// minimize    c^T x 
// subject to  F(x) = F0 + x1*F1 + ... + xm*Fm >= 0 
//             Tr F(x) <= M
//
// maximize    -Tr F0*(Z-zI) - Mz
// subject to  Tr Fi*(Z-zI) = c_i
//             Z >= 0, z>= 0
//
// Convergence criteria:
// (1) maxiters is exceeded
// (2) duality gap is less than abstol
// (3) primal and dual objective are both positive and
//     duality gap is less than (reltol * dual objective)
//     or primal and dual objective are both negative and
//     duality gap is less than (reltol * minus the primal objective)
// (4) reltol is negative and
//     primal objective is less than tv or dual objective is greater
//     than tv
//
// Input arguments:
// F:         (sum_i n_i^2) times (m+1) matrix
//            [ F_0^1(:) F_1^1(:) ... F_m^1(:) ]
//            [ F_0^2(:) F_1^2(:) ... F_m^2(:) ]
//                ...      ...          ...
//            [ F_0^L(:) F_1^L(:) ... F_m^L(:) ]
//            F_i^j: jth block of F_i, size n_i times n_i.
// blck_szs:  L-vector [n_1 ... n_L], dimensions of diagonal blocks.
// c:         m-vector.  Specifies primal objective.
// x0:        m-vector.  The primal starting point.  F(x0) > 0.  
// M:         scalar. M > Tr F(x0).    
// nu:        >= 1.0.  Controls the rate of convergence.
// abstol:    absolute tolerance.
// reltol:    relative tolerance.  Has a special meaning when negative.
// tv:        target value.
// maxiters:  maximum number of iterations.
//
// Output arguments:
// x:         m-vector; last primal iterate.
// Z:         last dual iterate; block-diagonal matrix stored as 
//            [ Z^1(:);  Z^2(:); ... ; Z^L(:) ].
// z:         scalar part of last dual iterate.  
// ul:        ul(1): primal objective, ul(1): dual objective.
// iters:     number of iterations taken.

// Copyright INRIA

[rowf,colf]=size(F);
m = colf-1;
if (rowf ~= sum(blck_szs.*blck_szs))
    error('Dimensions of F do not match blck_szs.');
end;

[rowx0,colx0]=size(x0);
if (rowx0 ~= m) | (colx0 ~= 1)
   error('x0 must be an m-vector.'); 
end;

if (prod(size(x0)) ~= m), 
   error('c must be an m-vector.'); 
end;

// I is the identity
I = zeros(rowf,1);
blck_szs=matrix(blck_szs,1,prod(size(blck_szs)));
k=0;  for n=blck_szs,
   I(k+[1:n*n]) = matrix(eye(n,n),n*n,1);   // identity
   k = k+n*n;   // k = sum n_i*n_i
end;

// Z0 = projection of I on dual feasible space 

Z0 = I-F(:,2:m+1) * ...
     ( (F(:,2:m+1)'*F(:,2:m+1)) \ ( F(:,2:m+1)'*I - c ) );

// mineigZ is the smallest eigenvalue of Z0
mineigZ = 0.0;
k=0; for n=blck_szs,
  mineigZ = min(mineigZ, min(real(spec(matrix(Z0(k+[1:n*n]),n,n)))));
  k=k+n*n;
end;

// z = max( 1e-5, -1.1*mineigZ )
Z0(k+1) = max( 1e-5, -1.1*mineigZ);  
Z0(1:k) = Z0(1:k) + Z0(k+1)*I; 

if (M < I'*F*[1;x0] + 1e-5), 
   error('M must be strictly greater than trace of F(x0).'); 
end;

// add scalar block Tr F(x) <= M
F = [F; M-I'*F(:,1),-I'*F(:,2:m+1)]; 
blck_szs = [blck_szs,1];
[x,Z,ul,info]=...
  semidef(x0,pack(Z0),pack(F),blck_szs,c,[nu,abstol,reltol,tv,maxiters]);
iters = info(2);
nz=prod(size(Z))
z=Z(nz)
Z=unpack(Z(1:nz-1),blck_szs(1:prod(size(blck_szs))-1))
Z = Z(1:k);



