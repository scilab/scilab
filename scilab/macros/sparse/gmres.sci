// [x, err, iter, flag, res] = gmres( A, b, x, M, restrt, max_it, tol )
//
// GMRES solves the linear system Ax=b
// using the Generalized Minimal residual ( GMRESm ) method with restarts .
//
// input   A        REAL nonsymmetric positive definite matrix or function
//         x        REAL initial guess vector
//         b        REAL right hand side vector
//         M        REAL preconditioner matrix or function
//         restrt   INTEGER number of iterations between restarts
//         max_it   INTEGER maximum number of iterations
//         tol      REAL error tolerance
//
// output  x        REAL solution vector
//         err      REAL final residual norm
//         iter     INTEGER number of iterations performed
//         flag     INTEGER: 0 = solution found to tolerance
//                           1 = no convergence given max_it
//         res      REAL residual vector

//     Details of this algorithm are described in 
//
//     "Templates for the Solution of Linear Systems: Building Blocks 
//     for Iterative Methods", 
//     Barrett, Berry, Chan, Demmel, Donato, Dongarra, Eijkhout,
//     Pozo, Romine, and Van der Vorst, SIAM Publications, 1993
//     (ftp netlib2.cs.utk.edu; cd linalg; get templates.ps).
//
//     "Iterative Methods for Sparse Linear Systems, Second Edition"
//     Saad, SIAM Publications, 2003
//     (ftp ftp.cs.umn.edu; cd dept/users/saad/PS; get all_ps.zip).

function [x, err, iter, flag, res] = gmres(A, varargin)

// -----------------------
// Parsing input arguments
// -----------------------

[lhs,rhs]=argn(0);
if ( rhs < 2 ),
  error("gmres: not enough argument");
end

// Parsing the matrix A et the right hand side vector b
select type(A)
case 1 then
  matrixType = 1;
case 5 then
  matrixType = 1;
case 13 then
  matrixType = 0;
end
// If A is a matrix (full or sparse)
if (matrixType == 1),
  if (size(A,1) ~= size(A,2)),
    error("gmres: matrix A must be square");
  end
end
b=varargin(1);
if (size(b,2) ~= 1),
  error("gmres: right hand side member must be a column vector");
end
if (matrixType==1),
  if (size(b,1) ~= size(A,1)),
    error("gmres: right hand side vector must have the size of the matrix A");
  end 
end

// Parsing of the initial vector x
if (rhs >= 3),
  x=varargin(2);
  if (size(x,2) ~= 1),
    error("Initial guess x0 must be a column vector");
  end
  if ( size(x,1) ~= size(b,1) ),
    error("gmres: initial guess x0 must have the size of the matrix A");
  end 
else
  x=zeros(b);
end

// Parsing of the preconditioner matrix M
if (rhs >= 4),
  M = varargin(3);
  select type(precondType)
  case 1 then
    precondType=1;
  case 5 then
    precondType=1;
  case 13 then
    precondType=0;
  end 
  if (precondType == 1),
    if (size(M,1) ~= size(M,2)),
      error("gmres: preconditionner matrix M must be square");
    end 
    if ( size(M,1) ~= size(b,1) ), 
      error("Preconditionner matrix M must have same size as the problem");
    end
  end
  if (precondType == 0),
    M = varargin(3);
  end
else
  precondType = 2; // no preconditionning
end

// Number of iterations between restarts
if (rhs >= 5),
  restrt=varargin(4);
  if (size(restrt) ~= [1 1]),
    error("gmres: restart must be a scalar");
  end 
else
  restrt=20;
end

// Maximum number of iterations max_it
if (rhs >= 6),
  max_it=varargin(5);
  if (size(max_it) ~= [1 1]),
    error("gmres: max_it must be a scalar");
  end 
else
  max_it=size(b,1);
end

// Error tolerance tol
if (rhs >= 7),
  tol=varargin(6);
  if (size(tol) ~= [1 1]);
    error("gmres: tol must be a scalar");
  end
else
  tol = 1e-6;
end

if (rhs > 7),
  error("gmres: too many input arguments");
end

// ------------
// Computations
// ------------

   j = 0; 
   flag = 0;
   it2 = 0;
 
   bnrm2 = norm(b);
   if (bnrm2 == 0.0), 
     x = zeros(b);
     err = 0;
     iter = 0;
     res = err;
     flag = 0;
     return
   end
   
   // r = M \ ( b-A*x );
   if (matrixType == 1),
     r = b - A*x;
   else 
     r = b - A(x);
   end
   if (precondType == 1),
     r = M \ r;
   elseif (precondType == 0),
     r = M(r);
   end
   err = norm(r)/bnrm2;
   res = err;
   if (err < tol), 
     iter=0; 
     return; 
   end

   n = size(b,1);
   m = restrt;
   V(1:n,1:m+1) = zeros(n,m+1);
   H(1:m+1,1:m) = zeros(m+1,m);
   cs(1:m) = zeros(m,1);
   sn(1:m) = zeros(m,1);
   e1    = zeros(n,1);
   e1(1) = 1.0;

   for j = 1:max_it
     // r = M \ ( b-A*x );
     if (matrixType == 1),
       r = b - A*x;
     else 
       r = b - A(x);
     end
     if (precondType == 1),
       r = M \ r;
     elseif (precondType == 0),
       r = M(r);
     end
     
     V(:,1) = r / norm( r );
     s = norm( r )*e1;
     for i = 1:m      // construct orthonormal
       it2 = it2 + 1; // basis using Gram-Schmidt
       // w = M \ (A*V(:,i)); 
       if (matrixType == 1),
	 w = A*V(:,i);
       else 
	 w = A(V(:,i));
       end
       if (precondType == 1),
	 w = M \ w;
       elseif (precondType == 0),
	 w = M(w);
       end
       
       for k = 1:i 
	 H(k,i)= w'*V(:,k);
	 w = w - H(k,i)*V(:,k);
       end
       H(i+1,i) = norm( w );
       V(:,i+1) = w / H(i+1,i);
       for k = 1:i-1 // apply Givens rotation
	 temp     =  cs(k)*H(k,i) + sn(k)*H(k+1,i);
	 H(k+1,i) = -sn(k)*H(k,i) + cs(k)*H(k+1,i);
	 H(k,i)   = temp;
       end
        // form i-th rotation matrix
       [tp1,tp2] = rotmat( H(i,i), H(i+1,i) );
       cs(i)  = tp1;
       sn(i)  = tp2;
       temp   = cs(i)*s(i);      
       s(i+1) = -sn(i)*s(i);
       s(i)   = temp;
       H(i,i) = cs(i)*H(i,i) + sn(i)*H(i+1,i);
       H(i+1,i) = 0.0;
       err  = abs(s(i+1)) / bnrm2;
       res = [res;err];
       if ( err <= tol ),
	 y = H(1:i,1:i) \ s(1:i);
	 x = x + V(:,1:i)*y;
	 break;
       end
     end
     if (err <= tol), 
       iter = j-1+it2;
       break; 
     end
     y = H(1:m,1:m) \ s(1:m);
     // update approximation
     x = x + V(:,1:m)*y;
     // r = M \ ( b-A*x )
     if (matrixType == 1),
       r = b - A*x;
     else 
       r = b - A(x);
     end
     if (precondType == 1),
       r = M \ r;
     elseif (precondType == 0),
       r = M(r);
     end
     s(j+1) = norm(r);
     err = s(j+1) / bnrm2;
     res = [res;err];
     
     if ( err <= tol ),
       iter = j+it2;
       break; 
     end
     if ( j== max_it ), 
       iter=j+it2; 
     end
   end
   if ( err > tol ), 
     flag = 1; 
     if (lhs < 2),
       warning('GMRES did not converge');
     end
   end
