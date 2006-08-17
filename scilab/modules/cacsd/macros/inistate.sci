function [x0,V,rcnd]=inistate(A,B,C,D,y,u,tol,printw)
x0=[];V=[];rcnd=[];
[nargout,nargin] = argn(0)
//INISTATE  Estimates the initial state of a discrete-time system, given the 
//          (estimated) system matrices, and a set of input/output data.
// 
//        X0 = INISTATE(SYS,Y,U,TOL,PRINTW)  estimates the initial state X0 of 
//        the discrete-time system SYS = (A,B,C,D), using the output data Y
//        and the input data U. The model structure is :
// 
//             x(k+1) = Ax(k) + Bu(k),   k >= 1,
//             y(k)   = Cx(k) + Du(k),
// 
//        The vectors y(k) and u(k) are transposes of the k-th rows of Y and U,
//        respectively.
//        Instead of the first input parameter SYS (an ss object), equivalent
//        information may be specified using matrix parameters, for instance,
//        X0 = INISTATE(A,B,C,Y,U);   or   X0 = INISTATE(A,C,Y); 
// 
//        TOL is the tolerance used for estimating the rank of matrices. 
//        If  TOL > 0,  then the given value of  TOL  is used as a lower bound
//        for the reciprocal condition number.
//        Default:    prod(size(matrix))*epsilon_machine where epsilon_machine
//                    is the relative machine precision.
// 
//        PRINTW is a select for printing the warning messages.
//        PRINTW = 1: print warning messages;
//               = 0: do not print warning messages.
//        Default:    PRINTW = 0.
// 
//        [x0,V,rcnd] = INISTATE(SYS,Y,U,TOL,PRINTW)  returns, besides x0, 
//        the orthogonal matrix V which reduces the system state matrix A to 
//        a real Schur form, as well as an estimate of the reciprocal condition
//        number of the coefficient matrix of the least squares problem solved.
// 
//        See also FINDBD, FINDX0BD
// 
 
//        V. Sima 13-05-2000.
// 
//        For efficiency, most errors are checked in the mexfile findBD. Also,
//        except for scalars, the input parameters are not copied, but renamed. 
// 
//        Revisions:
//        V. Sima, July 2000.
// 
 
ni = nargin;
if mtlb_isa(A,'lti') then
  // Get the system matrices of the ss object, and the remaining parameters.
  // General call     x0 = inistate(A,B,C,D,y,u,tol,printw);
  // Special call     x0 = inistate(sys,y,u,tol,printw); 
  // 
  if A.dt=='c' then
    error('The system SYS must be a discrete-time system');
  end
  if ni<2 then
    error('INISTATE needs at least 2 input parameters');
  end
  [As,Bs,Cs,Ds] = abcd(A)
  [ny,nu] = size(A);
  [ty,p] = size(B);
  if ni>2 then
    [tu,m] = size(C);
    if ~(((tu==ty|tu==0)&(m==nu))&(ty>1)) then
      tol = C;
      // Special call     x0 = inistate(sys,y,tol,printw); 
      if ni>3 then
        printw = D;
      else
        printw = 0;
      end
      // Below, B means y !
      [x0,Vl,rcndl] = findBD(1,3,As,Cs,B,tol,printw);
    else
      if ni>3 then
        // Special call     x0 = inistate(sys,y,u,tol,printw); 
        tol = D;
        if ni>4 then
          printw = y;
        else
          printw = 0;
        end
      else
        tol = 0;
        printw = 0;
      end
      // Below, B means y, and C means u !
      if norm(Ds,1)==0 then
        [x0,Vl,rcndl] = findBD(1,2,1,As,Bs,Cs,B,C,tol,printw);
      else
        [x0,Vl,rcndl] = findBD(1,2,2,As,Bs,Cs,Ds,B,C,tol,printw);
      end
    end
  else
    // Special call     x0 = inistate(sys,y); 
    // Below, B means y !
    [x0,Vl,rcndl] = findBD(1,3,As,Cs,B);
  end
  // 
else
  // The system matrices are directly specified.
  // General call     x0 = inistate(A,B,C,D,y,u,tol,printw);
  // Special calls    x0 = inistate(A,B,C,y,u,tol,printw); 
  //                  x0 = inistate(A,C,y,tol,printw); 
  // 
  if ni<3 then
    error('INISTATE needs at least 3 input parameters');
  end
  [m2,n2] = size(B);
  [m3,n3] = size(C);
  if ni>=4 then
    [m4,n4] = size(D);
    if ni>=5 then
      [m5,n5] = size(y);
      if ni>=6 then
        [m6,n6] = size(u);
        if ni>=7 then
          if (ni==7)&(m6*n6>1) then
            // Special call     x0 = inistate(A,B,C,D,y,u,tol); 
            [x0,Vl,rcndl] = findBD(1,2,1,A,B,C,D,y,u,tol);
          elseif ni==7 then
            // Special call     x0 = inistate(A,B,C,y,u,tol,printw);
            // Below, D means y and y means u !
            printw = tol;
            tol = u;
            [x0,Vl,rcndl] = findBD(1,2,1,A,B,C,D,y,tol,printw);
          else
            [x0,Vl,rcndl] = findBD(1,2,2,A,B,C,D,y,u,tol,printw);
          end
        else
          if m6*n6>1 then
            [x0,Vl,rcndl] = findBD(1,2,2,A,B,C,D,y,u);
          else
            // Special call     x0 = inistate(A,B,C,y,u,tol);
            // Below, y means U and D means y !
            tol = u;
            [x0,Vl,rcndl] = findBD(1,2,1,A,B,C,D,y,tol);
          end
        end
      else
        // Special calls    x0 = inistate(A,B,C,y,u); 
        //                  x0 = inistate(A,C,y,tol,printw); 
        if m5*n5>1 then
          // Below, y means u and D means y !
          [x0,Vl,rcndl] = findBD(1,2,1,A,B,C,D,y);
        else
          // Below, C means y and B means C !
          tol = D;
          printw = y;
          [x0,Vl,rcndl] = findBD(1,3,A,B,C,tol,printw);
        end
      end
    else
      // Below, D means tol, C means y, and B means C !
      [x0,Vl,rcndl] = findBD(1,3,A,B,C,D);
    end
  else
    // Below, C means y, and B means C !
    [x0,Vl,rcndl] = findBD(1,3,A,B,C);
  end
end
// 
if nargout>2 then
  V = Vl;
  rcnd = rcndl;
elseif nargout>1 then
  V = Vl;
end
// 
// end inistate
endfunction
