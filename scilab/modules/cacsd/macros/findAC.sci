function [A,C,rcnd]=findAC(s,n,l,R,meth,tol,printw)
    A=[];C=[];rcnd=[];
    [nargout,nargin] = argn(0)
    //FINDAC  Finds the system matrices A and C of a discrete-time system, given the
    //        system order and the relevant part of the R factor of the concatenated
    //        block-Hankel matrices, using subspace identification techniques (MOESP
    //        or N4SID).
    //
    //        [A,C] = FINDAC(S,N,L,R,METH,TOL,PRINTW)  computes the system matrices
    //        A and C. The model structure is:
    //
    //             x(k+1) = Ax(k) + Bu(k) + Ke(k),   k >= 1,
    //             y(k)   = Cx(k) + Du(k) + e(k),
    //
    //        where x(k) and y(k) are vectors of length N and L, respectively.
    //
    //        [A,C,RCND] = FINDAC(S,N,L,R,METH,TOL,PRINTW)  also returns the vector
    //        RCND of length 4 containing the condition numbers of the matrices
    //        involved in rank decisions.
    //
    //        S is the number of block rows in the block-Hankel matrices.
    //
    //        METH is an option for the method to use:
    //        METH = 1 :  MOESP method with past inputs and outputs;
    //             = 2 :  N4SID method.
    //        Default:    METH = 1.
    //        Matrix R, computed by FINDR, should be determined with suitable arguments
    //        METH and JOBD.
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
    //        See also FINDABCD, FINDBD, FINDBDK, FINDR, ORDER, SIDENT
    //

    //        V. Sima 18-01-2000.
    //
    //        Revisions:
    //

    nin = nargin;
    nout = nargout;
    //
    if nin<7 then
        printw = 0;
    end
    if nin<6 then tol = 0;end
    if tol==[] then tol = 0;end
    if nin<5 then meth = 1;end
    if meth==[] then meth = 1;end
    if nin<4 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"findAC",4,7));
    end
    //
    // Compute system matrices A and C.
    job = 2;
    nsmpl = 0;
    if nout==1 then
        A = sident(meth,job,s,n,l,R,tol,nsmpl,[],[],printw);
    elseif nout==2 then
        [A,C] = sident(meth,job,s,n,l,R,tol,nsmpl,[],[],printw);
    elseif nout==3 then
        [A,C,rcnd] = sident(meth,job,s,n,l,R,tol,nsmpl,[],[],printw);
    else
        error(msprintf(gettext("%s: Wrong number of output arguments: %d to %d expected.\n"),"findAC",1,3));
    end
    //
    // end findAC
endfunction
