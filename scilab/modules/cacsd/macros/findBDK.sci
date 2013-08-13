function [B,D,K,Q,Ry,S,rcnd]=findBDK(s,n,l,R,A,C,meth,job,nsmpl,tol,printw)
    B=[];D=[];K=[];Q=[];Ry=[];S=[];rcnd=[];
    [nargout,nargin] = argn(0)
    //FINDBDK  Finds the system matrices B and D and the Kalman gain of a discrete-time
    //         system, given the system order, the matrices A and C, and the relevant
    //         part of the R factor of the concatenated block-Hankel matrices, using
    //         subspace identification techniques (MOESP or N4SID).
    //
    //        [B,D,K] = FINDBDK(S,N,L,R,A,C,METH,JOB,NSMPL,TOL,PRINTW)  computes the
    //        system matrices B (if JOB = 1), B and D (if JOB = 2), and the Kalman
    //        predictor gain K (if NSMPL > 0). The model structure is:
    //
    //             x(k+1) = Ax(k) + Bu(k) + Ke(k),   k >= 1,
    //             y(k)   = Cx(k) + Du(k) + e(k),
    //
    //        where x(k) and y(k) are vectors of length N and L, respectively.
    //
    //        [B,D,RCND] = FINDBDK(S,N,L,R,A,C,METH,JOB)  also returns the vector
    //        RCND of length 4 containing the reciprocal condition numbers of the
    //        matrices involved in rank decisions.
    //
    //        [B,D,K,Q,Ry,S,RCND] = FINDBDK(S,N,L,R,A,C,METH,JOB,NSMPL,TOL,PRINTW)  also
    //        returns the state, output, and state-output (cross-)covariance
    //        matrices Q, Ry, and S (used for computing the Kalman gain), as well as
    //        the vector RCND of length 12 containing the reciprocal condition numbers
    //        of the matrices involved in rank decisions, least squares or Riccati
    //        equation solutions.
    //
    //        S is the number of block rows in the block-Hankel matrices.
    //
    //        METH is an option for the method to use:
    //        METH = 1 :  MOESP method with past inputs and outputs;
    //             = 2 :  N4SID method.
    //        Default:    METH = 2.
    //        Matrix R, computed by FINDR, should be determined with suitable arguments
    //        METH and JOBD.  METH = 1 and JOBD = 1 must be used in FINDR, for METH = 1
    //        in FINDBDK. Using METH = 1 in FINDR and METH = 2 in FINDBDK is allowed.
    //
    //        JOB is an option specifying which system matrices should be computed:
    //        JOB = 1 :   compute the matrix B;
    //            = 2 :   compute the matrices B and D.
    //        Default:    JOB = 2.
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
    //                    B = FINDBDK(S,N,L,R,A,C,METH,1)  or
    //                [B,D] = FINDBDK(S,N,L,R,A,C,METH,2)  or
    //           [B,D,RCND] = FINDBDK(S,N,L,R,A,C,METH,2)
    //        return B, or B and D, or B, D, and RCND, respectively.
    //
    //        See also FINDABCD, FINDAC, FINDBD, FINDR, ORDER, SIDENT
    //

    //        V. Sima 18-01-2000.
    //
    //        Revisions:
    //        V. Sima, July 2000.
    //

    nin = nargin;
    nout = nargout;
    //
    if nin<11 then printw = 0;end
    if nin<10 then tol = 0;end
    if tol==[] then tol = 0,end
    if nin<9 then nsmpl = 0;end
    if nsmpl==[] then nsmpl = 0;end
    if nin<8 then job = 2;end
    if job==[] then job = 2;end
    if nin<7 then meth = 2;end
    if meth == [] then meth = 2;end
    if nin<6 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"findBDK",6,11));
    end
    //
    jobl = job+2;
    //
    // Compute system matrices B and D.
    if nout==1 then
        B = sident(meth,jobl,s,n,l,R,tol,nsmpl,A,C,printw);
    elseif nout==2 then
        // If job = 1 and nsmpl > 0, D means K.
        // If job = 1 and nsmpl = 0, D means rcnd.
        [B,D] = sident(meth,jobl,s,n,l,R,tol,nsmpl,A,C,printw);
    elseif nout==3 then
        // If below, job = 1 and nsmpl = 0, D means rcnd, K is not assigned.
        // If job = 1 and nsmpl > 0, D means K, K means Q.
        [B,D,K] = sident(meth,jobl,s,n,l,R,tol,nsmpl,A,C,printw);
    elseif nout==4 then
        // If job = 1 and nsmpl > 0, D means K, K means Q, Q means Ry.
        [B,D,K,Q] = sident(meth,jobl,s,n,l,R,tol,nsmpl,A,C,printw);
    elseif nout==5 then
        // If job = 1 and nsmpl > 0, D means K, K means Q, Q means Ry, Ry means S.
        [B,D,K,Q,Ry] = sident(meth,jobl,s,n,l,R,tol,nsmpl,A,C,printw);
    elseif nout==6 then
        // If job = 1 and nsmpl > 0, D means K, K means Q, Q means Ry,
        // Ry means S, S means rcnd.
        [B,D,K,Q,Ry,S] = sident(meth,jobl,s,n,l,R,tol,nsmpl,A,C,printw);
    elseif nout==7 then
        [B,D,K,Q,Ry,S,rcnd] = sident(meth,jobl,s,n,l,R,tol,nsmpl,A,C,printw);
    else
        error(msprintf(gettext("%s: Wrong number of output arguments: %d to %d expected.\n"),"findBDK",1,7));
    end
    //
    // end findBDK
endfunction
