// [x, err, iter, flag, res] = pcg(A, b, x, M, max_it, tol)
//
// pcg.m solves the symmetric positive definite linear system Ax=b 
// using the Preconditionned Conjugate Gradient.
//
// input   A        REAL symmetric positive definite matrix or function
//         x        REAL initial guess vector
//         b        REAL right hand side vector
//         M        REAL preconditioner matrix (default: none)
//         max_it   INTEGER maximum number of iterations (default: 50)
//         tol      REAL error tolerance (default: 1e-8)
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
//     Pozo, Romine, and van der Vorst, SIAM Publications, 1993
//     (ftp netlib2.cs.utk.edu; cd linalg; get templates.ps).
//
//     "Iterative Methods for Sparse Linear Systems, Second Edition"
//     Saad, SIAM Publications, 2003
//     (ftp ftp.cs.umn.edu; cd dept/users/saad/PS; get all_ps.zip).

// Sage Group (IRISA, 2004)

function [x, err, iter, flag, res] = pcg(A, varargin)

// -----------------------
// Parsing input arguments
// -----------------------
[lhs,rhs] = argn(0);

// Parsing the matrix A
if (rhs == 0),
  error("pcg: matrix or function is expected");
end
select type(A)
case 1 then
  matrixType = 1;
case 5 then
  matrixType = 1;
case 13 then 
  matrixType = 0;
else
  error("pcg: unknown type for A");
end

// If A is a matrix (dense or sparse)
if (matrixType == 1),
  if (size(A,1) ~= size(A,2)),
    error("pcg: matrix A must be square");
  end
  // We don't test for symmetry (too expensive)
  //if ( bool2s(or( A ~= A')) == 1 ),
  //  error("pcg: matrix A must be symmetric");
  //end
end

// Parsing the right hand side b
if (rhs == 1),
  error("pcg: right hand side vector b is expected");
end  
b=varargin(1);
if (size(b,2) ~= 1),
  error("pcg: right hand side b must be a column vector");
end
if (matrixType ==1),
  if (size(b,1) ~= size(A,1)),
    error("pcg: right hand side b must have the size of the matrix A");
  end 
end

// Parsing the initial vector x
if (rhs >= 3),
   x=varargin(2);
   if (size(x,2) ~= 1),
      error("pcg: initial guess x0 must be a column vector");
   end
   if (size(x,1) ~= size(b,1)),
     error("pcg: initial guess x0 must have the size of b");
   end 
else
  x=zeros(b);
end

// Parsing the preconditioner M
if (rhs >=4),
  M = varargin(3);
  select type(M)
  case 1 then
    precondType = 1;
  case 5 then
    precondType = 1;
  case 13 then
    precondType = 0;
  else
    error("pcg: unknown type for preconditionner");
  end 
  if (precondType == 1),
    if (size(M,1) ~= size(M,2)),
      error("pcg: preconditionner matrix M must be square");
    end 
    if ( size(M,1) ~= size(b,1) ),
      error("pcg: preconditionner matrix M must have the size of b");
    end
  end
else
  precondType = 2;
end

// Parsing of the maximum number of iterations max_it
if (rhs >= 5),
   maxIter = varargin(4);
   if (size(maxIter) ~= [1 1]),
      error("pcg: maxIter must be a scalar");
   end 
else
   maxIter = min(size(b,1),50);
end

// Parsing of the error tolerance tol
if (rhs >= 6),
   tol=varargin(5);
   if (size(tol,1) ~= [1 1]),
      error("pcg: tol must be a scalar");
   end
else
   tol=1e-8;
end

// input arguments are parsed !
if (rhs > 6),
   error("pcg: too many input arguments");
end

// ------------
// Computations
// ------------

// initialization
flag = 0; 
bnrm2 = norm(b);
if  (bnrm2 == 0), 
  x = zeros(b);
  err = 0;
  iter = 0;
  res = err;
end

// r = b - A*x;
if (matrixType ==1),
  r = b - A*x;
else
  r = b - A(x);
end
err = norm(r) / bnrm2;
res = err;
if (err < tol),
  iter = 0;
  return; 
end

// begin iteration
for i = 1:maxIter,
  // z  = M \ r;
  if (precondType == 1),
    z = M \ r;
  elseif (precondType == 0),
    z = M(r);
  else
    z = r;
  end
  rho = (r'*z);
  if (i > 1),
    bet = rho / rho_1;
    p = z + bet*p;
  else
    p = z;
  end
  
  // q = A*p;
  if (matrixType ==1),
    q = A*p;
  else
    q = A(p);
  end
  alp = rho / (p'*q );
  x = x + alp*p;
  r = r - alp*q;
  err = norm(r) / bnrm2;
  res = [res;err];
  if (err <= tol),
    iter=i; 
    break;
  end 
  rho_1 = rho;
  if (i == maxIter ), 
    iter=i;
  end

end
// test for convergence
if (err > tol),
  flag = 1; 
end
