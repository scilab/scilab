// [x, flag, resNorm, iter, resVec] = pcg(A, b, tol, maxIter, M, M2, x)
//
// PCG solves the symmetric positive definite linear system Ax=b 
// using the Preconditionned Conjugate Gradient.
//
// input   A        REAL symmetric positive definite matrix or function
//         b        REAL right hand side vector
//         tol      REAL error tolerance (default: 1e-8)
//         maxIter  INTEGER maximum number of iterations (default: 50)
//         M        REAL preconditioner matrix (default: none)
//         M2       REAL preconditioner matrix (default: none)
//         x        REAL initial guess vector
//
// output  x        REAL solution vector
//         flag     INTEGER: 0 = solution found to tolerance
//                           1 = no convergence given maxIter
//         resNorm  REAL final relative norm of the residual
//         iter     INTEGER number of iterations performed
//         resVec   REAL residual vector

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

// Sage Group (IRISA, 2004)

function [x, flag, resNorm, iter, resVec] = pcg(A, varargin)

// -----------------------
// Parsing input arguments
// -----------------------
  [lhs,rhs] = argn(0);
  if (rhs < 2),
    error("pcg: not enough input arguments");
  end

  // Parsing the matrix A
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
  end

  // Parsing right hand side b
  b=varargin(1);
  if (size(b,2) ~= 1),
    error("pcg: right hand side b must be a column vector");
  end
  if (matrixType ==1),
    if (size(b,1) ~= size(A,1)),
      error("pcg: right hand side b must have the size of the matrix A");
    end 
  end

  // Parsing of the error tolerance tol
  if (rhs >= 3),
    tol=varargin(2);
    if (size(tol) ~= [1 1]),
      error("pcg: tol must be a scalar");
    end
  else
    tol=1e-8;
  end

  // Parsing of the maximum number of iterations max_it
  if (rhs >= 4),
    maxIter = varargin(3);
    if (size(maxIter) ~= [1 1]),
      error("pcg: maxIter must be a scalar");
    end 
  else
    maxIter = min(size(b,1),50);
  end

  // Parsing the preconditioner M
  if (rhs >=5),
    M = varargin(4);
    select type(M)
    case 1 then
      precondType = 1;
    case 5 then
      precondType = 1;
    case 13 then
      precondType = 2;
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
    precondType = 0; //no preconditionner
  end

  // Parsing the preconditioner M
  if (rhs >=6),
    M2 = varargin(5);
    select type(M2)
    case 1 then
      precondBis = 1;
    case 5 then
      precondBis = 1;
    case 13 then
      precondBis = 2;
    else
      error("pcg: unknown type for preconditionner");
    end 
    if (precondBis == 1),
      if (size(M2,1) ~= size(M2,2)),
	error("pcg: preconditionner matrix M2 must be square");
      end 
      if ( size(M2,1) ~= size(b,1) ),
	error("pcg: preconditionner matrix M2 must have the size of b");
      end
    end
  else
    precondBis = 0; //no preconditionner
  end

  // Parsing the initial vector x
  if (rhs >= 7),
    x=varargin(6);
    if (size(x,2) ~= 1),
      error("pcg: initial guess x0 must be a column vector");
    end
    if (size(x,1) ~= size(b,1)),
      error("pcg: initial guess x0 must have the size of b");
    end 
  else
    x=zeros(b);
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
  iter = 0;
  bnrm2 = norm(b);
  if  (bnrm2 == 0), 
    x = zeros(b);
    resNorm = 0;
    resVec = resNorm;
  end

  // r = b - A*x;
  if (matrixType ==1),
    r = b - A*x;
  else
    r = b - A(x);
  end
  resNorm = norm(r) / bnrm2;
  resVec = resNorm;
  if (resNorm < tol),
    return; 
  end

  // begin iteration
  for i = 1:maxIter-1,
    // z  = M \ r;
    if (precondType == 1),
      z = M \ r;
    elseif (precondType == 2),
      z = M(r);
    else
      z = r;
    end
    // z  = M2 \ r;
    if (precondBis == 1),
      z = M2 \ r;
    elseif (precondBis == 2),
      z = M2(r);
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
    resNorm = norm(r) / bnrm2;
    resVec = [resVec;resNorm];
    if (resNorm <= tol),
      iter=i; 
      break;
    end 
    rho_1 = rho;
    if (i == maxIter ), 
      iter=i;
    end
  end

  // test for convergence
  if (resNorm > tol),
    flag = 1; 
    if (lhs < 2),
      warning('PCG did not converge');
    end
  end
endfunction
