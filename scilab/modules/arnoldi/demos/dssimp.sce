// Example converted from dssimp.f from the EXAMPLE/SIMPLE ARPACK directory

// This example program is intended to illustrate the simplest case of using the ARNOLDI module in considerable detail.
//
// This code shows how to use ARNOLDI to find a few eigenvalues (lambda) and corresponding eigenvectors (x) for the standard
// eigenvalue problem:
//
//   A*x = lambda*x
//
// where A is a n by n real symmetric matrix.
//
// The main points illustrated here are
//
// 1) How to declare sufficient memory to find NEV eigenvalues of largest magnitude. Other options are available.
//
// 2) Illustration of the reverse communication interface needed to utilize the top level ARPACK routine DSAUPD
//    that computes the quantities needed to construct the desired eigenvalues and eigenvectors(if requested).
//
// 3) How to extract the desired eigenvalues and eigenvectors using the ARPACK routine DSEUPD.
//
// The only thing that must be supplied in order to use this routine on your problem is to change the array dimensions
// appropriately, to specify WHICH eigenvalues you want to compute and to supply a matrix-vector product
//
//   w <-  Av
//
// in place of the call to AV( )  below.
//
// Once usage of this routine is understood, you may wish to explore the other available options to improve convergence, to solve generalized
// problems, etc.  Look at the file ex-nonsym.doc in DOCUMENTS directory.
// Storage Declarations:
//
// The maximum dimensions for all arrays are set here to accommodate a problem size of N <= MAXN
//
// NEV is the number of eigenvalues requested. See specifications for ARPACK usage below.
//
// NCV is the largest number of basis vectors that will be used in the Implicitly Restarted Arnoldi
// Process.  Work per major iteration is proportional to N*NCV*NCV.
// You must set:
//
// MAXN:   Maximum dimension of the A allowed.
// MAXNEV: Maximum NEV allowed.
// MAXNCV: Maximum NCV allowed.

maxn    = 256;
maxnev  = 12;
maxncv  = 30;
maxiter = 10;

// The following sets dimensions for this problem.
nx    = 10;

// Specifications for ARPACK usage are set below:
// 1) NEV = 4  asks for 4 eigenvalues to be computed.
// 2) NCV = 20 sets the length of the Arnoldi factorization.
// 3) This is a standard problem. (indicated by bmat  = 'I')
// 4) Ask for the NEV eigenvalues of largest magnitude. (indicated by which = 'LM')
//    See documentation in DSAUPD for the other options SM, LR, SR, LI, SI.
//    Note: NEV and NCV must satisfy the following conditions:
//          NEV <= MAXNEV
//          NEV + 2 <= NCV <= MAXNCV

nev   = 3;
ncv   = 6;
bmat  = "I";
which = "LM";

// Local Arrays

iparam  = zeros(11,1);
ipntr   = zeros(14,1);
_select = zeros(ncv,1);
d       = zeros(nev+1,1);
resid   = zeros(nx,1);
v       = zeros(nx,ncv);
workd   = zeros(3*nx+1,1);
workl   = zeros(ncv*ncv+8*ncv,1);

if (nx > maxn) then
    printf("Error with DSSIMP: N is greater than MAXN.\n");
    break;
elseif (nev > maxnev) then
    printf("Error with DSSIMP: NEV is greater than MAXNEV.\n");
    break;
elseif (ncv > maxncv) then
    printf("Error with DSSIMP: NCV is greater than MAXNCV.\n");
    break;
end

// Build the symmetric test matrix
A            = diag(10*ones(nx,1));
A(1:$-1,2:$) = A(1:$-1,2:$) + diag(6*ones(nx-1,1));
A(2:$,1:$-1) = A(2:$,1:$-1) + diag(6*ones(nx-1,1));

// Specification of stopping rules and initial conditions before calling DSAUPD
//
// TOL  determines the stopping criterion.
//
//      Expect
//                 abs(lambdaC - lambdaT) < TOL*abs(lambdaC)
//                     computed   true                       |
//
//      If TOL <= 0,  then TOL <- macheps (machine precision) is used.
//
// IDO  is the REVERSE COMMUNICATION parameter used to specify actions to be taken on return
//      from DSAUPD. (see usage below)
//
//      It MUST initially be set to 0 before the first call to DSAUPD.
//
// INFO on entry specifies starting vector information and on return indicates error codes
//
//      Initially, setting INFO=0 indicates that a random starting vector is requested to
//      start the ARNOLDI iteration.  Setting INFO to a nonzero value on the initial call is used
//      if you want to specify your own starting vector (This vector must be placed in RESID).
//
// The work array WORKL is used in DSAUPD as workspace.

tol     = 0;
ido     = 0;

// Specification of Algorithm Mode:
//
// This program uses the exact shift strategy (indicated by setting IPARAM(1) = 1).
// IPARAM(3) specifies the maximum number of Arnoldi iterations allowed.  Mode 1 of DSAUPD is used
// (IPARAM(7) = 1). All these options can be changed by the user. For details see the documentation in DSAUPD.

ishfts = 1;
maxitr = 300;
mode1  = 1;

iparam(1) = ishfts;
iparam(3) = maxitr;
iparam(7) = mode1;

sigma = 0; // the real part of the shift

// M A I N   L O O P (Reverse communication)

iter = 0;
while(iter<maxiter)
    info_dsaupd = 0;
    iter = iter + 1;
    // Repeatedly call the routine DSAUPD and take actions indicated by parameter IDO until
    // either convergence is indicated or maxitr has been exceeded.

    [ido,resid,v,iparam,ipntr,workd,workl,info_dsaupd] = dsaupd(ido,bmat,nx,which,nev,tol,resid,ncv,v,iparam,ipntr,workd,workl,info_dsaupd);

    if (ido==99) then
        // BE CAREFUL: DON'T CALL dseupd IF ido == 99 !!
        break;
    end

    if (ido==-1 | ido==1) then
        // Perform matrix vector multiplication
        //                 y <--- Op*x
        // The user should supply his/her own matrix vector multiplication routine here
        // that takes workd(ipntr(1)) as the input vector, and return the matrix vector
        // product to workd(ipntr(2)).

        workd(ipntr(2)+1:ipntr(2)+nx) = A*workd(ipntr(1)+1:ipntr(1)+nx);
        // L O O P   B A C K to call DSAUPD again.
        continue
    end

    // Either we have convergence or there is an error.
    if (info_dsaupd < 0) then
        // Error message, check the documentation in DSAUPD.
        printf("\nError with dsaupd, info = %d\n",info_dsaupd);
        printf("Check the documentation of dsaupd\n\n");
    else
        // No fatal errors occurred.
        // Post-Process using DSEUPD.
        //
        // Computed eigenvalues may be extracted.
        //
        // Eigenvectors may be also computed now if desired.  (indicated by rvec = %T)
        //
        // The routine DSEUPD now called to do this post processing (Other modes may require
        // more complicated post processing than mode1,)

        rvec    = 1;
        howmany = "A";

        info_dseupd = 0;

        [d,d(1,2),v,resid,v,iparam,ipntr,workd,workl,info_dseupd] = dseupd(rvec,howmany,_select,d,d(1,2),v,sigma, ...
        bmat,nx,which,nev,tol,resid,ncv,v, ...
        iparam,ipntr,workd,workl,info_dseupd);

        // The real parts of the eigenvalues are returned in the first column of the two dimensional
        // array D, and the IMAGINARY part are returned in the second column of D.  The corresponding
        // eigenvectors are returned in the first NCONV (= IPARAM(5)) columns of the two
        // dimensional array V if requested.  Otherwise, an orthogonal basis for the invariant subspace
        // corresponding to the eigenvalues in D is returned in V.

        if (info_dseupd~=0) then
            // Error condition:
            // Check the documentation of DSEUPD.
            printf("\nError with dseupd, info = %d\n", info_dseupd);
            printf("Check the documentation of dseupd.\n\n");
        end

        // Print additional convergence information.
        if (info_dseupd==1) then
            printf("\nMaximum number of iterations reached.\n\n");
        elseif (info_dseupd==3) then
            printf("\nNo shifts could be applied during implicit Arnoldi update, try increasing NCV.\n\n");
        end
    end
end

// Done with program dssimp.
printf("\nDSSIMP\n");
printf("======\n");
printf("\n");
printf("Iterations is %d\n", iter);
printf("Size of the matrix is %d\n", nx);
printf("The number of Ritz values requested is %d\n", nev);
printf("The number of Arnoldi vectors generated (NCV) is %d\n", ncv);
printf("What portion of the spectrum: %s\n", which);
printf("The number of Implicit Arnoldi update iterations taken is %d\n", iparam(3));
printf("The number of OP*x is %d\n", iparam(9));
printf("The convergence criterion is %d\n", tol);

