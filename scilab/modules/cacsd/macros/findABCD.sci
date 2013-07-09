function [sys,K,Q,Ry,S,rcnd]=findABCD(s,n,l,R,meth,nsmpl,tol,printw)
    sys=[];K=[];Q=[];Ry=[];S=[];rcnd=[];
    [nargout,nargin] = argn(0)
    //FINDABCD  Finds the system matrices and the Kalman gain of a discrete-time
    //          system, given the system order and the relevant part of the
    //          R factor of the concatenated block-Hankel matrices, using subspace
    //          identification techniques (MOESP and/or N4SID).
    //
    //        [SYS,K] = FINDABCD(S,N,L,R,METH,NSMPL,TOL,PRINTW)  computes a state-
    //        space realization SYS = (A,B,C,D) (an ss object), and the Kalman
    //        predictor gain K (if NSMPL > 0). The model structure is:
    //
    //             x(k+1) = Ax(k) + Bu(k) + Ke(k),   k >= 1,
    //             y(k)   = Cx(k) + Du(k) + e(k),
    //
    //        where x(k) and y(k) are vectors of length N and L, respectively.
    //
    //        [SYS,K,Q,Ry,S,RCND] = FINDABCD(S,N,L,R,METH,NSMPL,TOL,PRINTW)  also
    //        returns the state, output, and state-output (cross-)covariance
    //        matrices Q, Ry, and S (used for computing the Kalman gain), as well as
    //        the vector RCND of length lr containing the reciprocal condition numbers
    //        of the matrices involved in rank decisions, least squares or Riccati
    //        equation solutions, where
    //           lr = 4,  if Kalman gain matrix K is not required, and
    //           lr = 12, if Kalman gain matrix K is required.
    //
    //        S is the number of block rows in the block-Hankel matrices.
    //
    //        METH is an option for the method to use:
    //        METH = 1 :  MOESP method with past inputs and outputs;
    //             = 2 :  N4SID method;
    //             = 3 :  combined method: A and C via MOESP, B and D via N4SID.
    //        Default:    METH = 3.
    //        Matrix R, computed by FINDR, should be determined with suitable arguments
    //        METH and JOBD.  METH = 1 and JOBD = 1 must be used in findR, for METH = 1
    //        in FINDABCD;  METH = 1 must be used in FINDR, for METH = 3 in FINDABCD.
    //
    //        NSMPL is the total number of samples used for calculating the covariance
    //        matrices and the Kalman predictor gain. This parameter is not needed if
    //        the covariance matrices and/or the Kalman predictor gain matrix are not
    //        desired. If NSMPL = 0, then K, Q, Ry, and S are not computed.
    //        Default:    NSMPL = 0.
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
    //        The number of output arguments may vary, but should correspond to the
    //        input arguments, e.g.,
    //        SYS = FINDABCD(S,N,L,R,METH)  or
    //        [SYS,RCND] = FINDABCD(S,N,L,R,METH)  just return SYS, or SYS and RCND.
    //
    //        See also FINDAC, FINDBD, FINDBDK, FINDR, ORDER, SIDENT
    //

    //        V. Sima 18-01-2000.
    //
    //        Revisions:
    //

    nin = nargin;
    nout = nargout;
    //
    if nin<8 then
        printw = 0;
    end
    if nin<7 then tol = 0;end
    if tol==[] then tol = 0;end

    if nin<6 then nsmpl = 0;end
    if nsmpl==[] then nsmpl = 0;end

    if nin<5 then meth = 3;end
    if meth==[] then meth = 3;end

    if nin<4 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"findABCD",4,8));
    end
    //
    // Compute all system matrices.
    job = 1;
    if nout==1 then
        [A,C,B,D] = sident(meth,job,s,n,l,R,tol,nsmpl,[],[],printw);
    elseif nout>=2 then
        if nsmpl==0 then
            // Here K means rcnd.
            [A,C,B,D,K] = sident(meth,job,s,n,l,R,tol,nsmpl,[],[],printw);
        elseif nout==2 then
            [A,C,B,D,K] = sident(meth,job,s,n,l,R,tol,nsmpl,[],[],printw);
        elseif nout==3 then
            [A,C,B,D,K,Q] = sident(meth,job,s,n,l,R,tol,nsmpl,[],[],printw);
        elseif nout==4 then
            [A,C,B,D,K,Q,Ry] = sident(meth,job,s,n,l,R,tol,nsmpl,[],[],printw);
        elseif nout==5 then
            [A,C,B,D,K,Q,Ry,S] = sident(meth,job,s,n,l,R,tol,nsmpl,[],[],printw);
        elseif nout==6 then
            [A,C,B,D,K,Q,Ry,S,rcnd] = sident(meth,job,s,n,l,R,tol,nsmpl,[],[],printw);
        else
            error(msprintf(gettext("%s: Wrong number of output arguments: %d to %d expected.\n"),"findABCD",1,6));
        end
    else
        error(msprintf(gettext("%s: Wrong number of output arguments: %d to %d expected.\n"),"findABCD",1,6));
    end
    //
    sys = syslin(1,A,B,C,D);
    //
    // end findABCD
endfunction
