// [x, flag, err, iter, res] = qmr( A, Ap, b, x, M1, M2, max_it, tol )
//
// QMR solves the linear system Ax=b using the 
// Quasi Minimal Residual method with preconditioning.
//
// input   A        REAL matrix or function
//         x        REAL initial guess vector
//         b        REAL right hand side vector
//         M1       REAL left preconditioner matrix
//         M2       REAL right preconditioner matrix
//         max_it   INTEGER maximum number of iterations
//         tol      REAL error tolerance
//
// output  x        REAL solution vector
//         flag     INTEGER: 0: solution found to tolerance
//                           1: no convergence given max_it
//                     breakdown:
//                          -1: rho
//                          -2: Beta
//                          -3: gam
//                          -4: delta
//                          -5: ep
//                          -6: xi
//         err      REAL final residual norm
//         iter     INTEGER number of iterations performed
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

// Sage Group (IRISA, 2005)

function [x, flag, err, iter, res] = qmr( A, varargin)

// -----------------------
// Parsing input arguments
// -----------------------
  [lhs,rhs]=argn(0);
  if ( rhs < 2 ),
    error("qmr: not enough input arguments");
  end

  // Parsing the matrix A
  select type(A)
  case 1 then
    cpt=1;
  case 5 then
    cpt=1;
  case 13 then
    cpt=0;
  else
    error("qmr: unknown type for A");
  end

  // If A is a matrix (dense or sparse)
  if (cpt==1),
    if (size(A,1) ~= size(A,2)),
      error("qmr: the matrix A must be square",502);
    end
    if (rhs == 1),
      error("qmr: right hand side vector b is expected",502);
    end
    deff('y=matvec(x)','y=A*x');
    deff('y=matvecp(x)','y=A''*x');
    fct=0;
  end

  // If A is a function
  if (cpt==0),
    if (rhs == 1),
      error("qmr: transpose of the function A is expected",502);
    end
    matvec=A;
    fct=1;
  end
  if (rhs >= 2 & fct==1 ),
    matvecp=varargin(1);
    if ( type(matvecp) ~= 13 ),
      error("qmr: the second variable must be the transpose of the function A",502);
    end
  end

  // Parsing right hand side b
  if ( rhs >= fct+2 ),
    b=varargin(fct+1);
    if ( size(b,2) ~= 1),
      error("qmr: right hand side member must be a column vector",502);
    end
  else 
    error("qmr: right hand side vector b is expected",502);
  end

  // Parsing initial vector x
  if ( rhs >= fct+3),
    x=varargin(fct+2);
    if (size(x,2) ~= 1),
      error("qmr: initial guess x0 must be a column vector",502);
    end
    if ( size(x,1) ~= size(b,1)),
      error("qmr: initial guess x0 must have the size of the vector b",502);
    end
  else
    x=zeros(size(b,1),1);
  end

  //--------------------------------------------------------
  // Parsing of the preconditioner matrix M1
  //--------------------------------------------------------

  if (rhs >=fct+4),
    Prec_g=varargin(fct+3);
    select type(Prec_g)
    case 1 then
      cpt=1;
    case 5 then
      cpt=1;
    case 13 then
      cpt=0;
    end 
    if ( cpt==1 ),
      if (size(Prec_g,1) ~= size(Prec_g,2)),
	error("qmr: the preconditionner matrix M1 must be square",502);
      end 
      if (size(Prec_g,1)~=size(b,1)),
	error("qmr: the size of the preconditionner matrix M1 must be the size of b",502);
      end 
      deff('y=precond_g(x)','y=Prec_g \ x');
      deff('y=precondp_g(x)','y=Prec_g'' \ x');
    end
    if ( cpt==0 ),
      if ( rhs >= fct+5 ),
	Precp_g=varargin(fct+4);
	select type(Precp_g)
	case 1 then
	  cpt1=1;
	case 5 then
	  cpt1=1;
	case 13 then
	  cpt1=0;
	end 
	if ( cpt1==0 ),
	  precond_g=Prec_g;
	  precondp_g=Precp_g;
	  fct=fct+1;
	else
	  error("qmr: the transpose function of M1 is expected",502);
	end
      else
	error("qmr: the transpose function of M1 is expected",502);
      end
    end
  else
    deff('y=precond_g(x)','y=x');
    deff('y=precondp_g(x)','y=x');
  end  
  
  //--------------------------------------------------------
  // Parsing of the preconditioner matrix M1
  //--------------------------------------------------------

  if (rhs >=fct+5),
    Prec_d=varargin(fct+4);
    select type(Prec_d)
    case 1 then
      cpt=1;
    case 5 then
      cpt=1;
    case 13 then
      cpt=0;
    end 
    if ( cpt==1 ),
      if (size(Prec_d,1) ~= size(Prec_d,2)),
	error("qmr: the preconditionner matrix M2 must be square",502);
      end 
      if (size(Prec_d,1)~=size(b,1)),
	error("qmr: the size of the preconditionner matrix M2 must be the size of b",502);
      end 
      deff('y=precond_d(x)','y=Prec_d \ x');
      deff('y=precondp_d(x)','y=Prec_d'' \ x');
    end
    if ( cpt==0 ),
      if ( rhs >= fct+6 ),
	Precp_d=varargin(fct+5);
	select type(Precp_d)
	case 1 then
	  cpt1=1;
	case 5 then
	  cpt1=1;
	case 13 then
	  cpt1=0;
	end 
	if ( cpt1==0 ),
	  precond_d=Prec_d;
	  precondp_d=Precp_d;
	  fct=fct+1;
	else
	  error("qmr: the transpose function of M2 is expected",502);
	end
      else
	error("qmr: the transpose function of M2 is expected",502);
      end
    end
    
  else
    deff('y=precond_d(x)','y=x');
    deff('y=precondp_d(x)','y=x');
  end

  //--------------------------------------------------------
  // Parsing of the maximum number of iterations max_it
  //--------------------------------------------------------

  if (rhs >= fct+6),
    max_it=varargin(fct+5);
    if (size(max_it,1) ~= 1 | size(max_it,2) ~=1),
      error("qmr: max_it must be a scalar",502);
    end 
  else
    max_it=size(b,1);
  end

  //--------------------------------------------------------
  // Parsing of the error tolerance tol
  //--------------------------------------------------------

  if (rhs == fct+7),
    tol=varargin(fct+6);
    if (size(tol,1) ~= 1 | size(tol,2) ~=1),
      error("qmr: tol must be a scalar",502);
    end
  else
    tol=1000*%eps;
  end

  //--------------------------------------------------------
  // test about input arguments number
  //--------------------------------------------------------

  if (rhs > fct+8),
    error("qmr: too many input arguments",502);
  end

  // ------------
  // Computations
  // ------------

  // initialization
  i = 0;
  flag = 0;
  bnrm2 = norm( b );
  if  (bnrm2 == 0.0), 
    bnrm2 = 1.0; 
  end

  //   r = b - A*x;
  r = b - matvec(x);
  err = norm( r ) / bnrm2;
  res = err;
  if ( err < tol ), return; end

  // [M1,M2] = lu( M );

  v_tld = r;
  // y = M1 \ v_tld;
  y = precond_g(v_tld)
  rho = norm( y );

  w_tld = r;
  //   z = M2' \ w_tld;
  z = precondp_d(w_tld);
  xi = norm( z );

  gam =  1.0;
  eta = -1.0;
  theta =  0.0;

  for i = 1:max_it,                      // begin iteration

    if ( rho == 0.0 | xi == 0.0 ), iter=i; break; end

    v = v_tld / rho;
    y = y / rho;

    w = w_tld / xi;
    z = z / xi;

    delta = z'*y;
    if ( delta == 0.0 ), iter=i; break; end

    //    y_tld = M2 \ y;
    y_tld = precond_d(y);
    //    z_tld = M1'\ z;
    z_tld = precondp_g(z);

    if ( i > 1 ),                       // direction vector 
      p = y_tld - ( xi*delta / ep )*p;
      q = z_tld - ( rho*delta / ep )*q;
    else
      p = y_tld;
      q = z_tld;
    end

    //    p_tld = A*p;
    p_tld = matvec(p);

    ep = q'*p_tld;
    if ( ep == 0.0 ), iter=i; break; end

    Beta = ep / delta;
    if ( Beta == 0.0 ), iter=i; break; end

    v_tld = p_tld - Beta*v;
    //    y =  M1 \ v_tld;
    y = precond_g(v_tld);
    
    rho_1 = rho;
    rho = norm( y );
    //    w_tld = ( A'*q ) - ( Beta*w );
    w_tld = ( matvecp(q) ) - ( Beta*w );
    //    z =  M2' \ w_tld;
    z =  precondp_d(w_tld);

    xi = norm( z );

    gamma_1 = gam;
    theta_1 = theta;

    theta = rho / ( gamma_1*Beta );
    gam = 1.0 / sqrt( 1.0 + (theta^2) );
    if ( gam == 0.0 ), iter=i; break; end

    eta = -eta*rho_1*(gam^2) / ( Beta*(gamma_1^2) );

    if ( i > 1 ),                         // compute adjustment
      d = eta*p + (( theta_1*gam )^2)*d;
      s = eta*p_tld + (( theta_1*gam )^2)*s;
    else
      d = eta*p;
      s = eta*p_tld;
    end

    x = x + d;                               // update approximation

    r = r - s;                               // update residual
    err = norm( r ) / bnrm2;               // check convergence
    res = [res;err];
    
    if ( err <= tol ), iter=i; break; end

    if ( i == max_it ), iter=i; end
    
  end

  if ( err <= tol ),                        // converged
    flag =  0;
  elseif ( rho == 0.0 ),                      // breakdown
    flag = -1;
  elseif ( Beta == 0.0 ),
    flag = -2;
  elseif ( gam == 0.0 ),
    flag = -3;
  elseif ( delta == 0.0 ),
    flag = -4;
  elseif ( ep == 0.0 ),
    flag = -5;
  elseif ( xi == 0.0 ),
    flag = -6;
  else                                        // no convergence
    flag = 1;
  end

endfunction
