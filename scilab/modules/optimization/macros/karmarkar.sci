// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//

// xopt=karmarkar(Aeq,beq,c)
// xopt=karmarkar(Aeq,beq,c,x0)
// xopt=karmarkar(Aeq,beq,c,x0,rtolf)
// xopt=karmarkar(Aeq,beq,c,x0,rtolf,gam)
// xopt=karmarkar(Aeq,beq,c,x0,rtolf,gam,maxiter)
// xopt=karmarkar(Aeq,beq,c,x0,rtolf,gam,maxiter,outfun)
// xopt=karmarkar(Aeq,beq,c,x0,rtolf,gam,maxiter,outfun,A,b)
// xopt=karmarkar(Aeq,beq,c,x0,rtolf,gam,maxiter,outfun,A,b,lb)
// xopt=karmarkar(Aeq,beq,c,x0,rtolf,gam,maxiter,outfun,A,b,lb,ub)
// [xopt,fopt] = karmarkar(...)
// [xopt,fopt,exitflag] = karmarkar(...)
// [xopt,fopt,exitflag,iter] = karmarkar(...)
// [xopt,fopt,exitflag,iter,yopt] = karmarkar(...)
//
// exitflag = 1 if algorithm converged.
// exitflag = 0 if maximum number of iterations was reached.
// exitflag = -1 if no feasible point was found
// exitflag = -2 if problem is unbounded.
// exitflag = -3 if search direction became zero.
// exitflag = -4 if algorithm stopped on user's request.
// exitflag = -5 if duality gap became too large
// exitflag = -%inf on internal error.

function [xopt,fopt,exitflag,iter,yopt]=karmarkar(varargin)
    function argin = optim_argindefault ( rhs , vararglist , ivar , default )
        // Returns the value of the input argument #ivar.
        // If this argument was not provided, or was equal to the
        // empty matrix, returns the default value.
        if ( rhs < ivar ) then
            argin = default
        else
            if ( vararglist(ivar) <> [] ) then
                argin = vararglist(ivar)
            else
                argin = default
            end
        end
    endfunction

    //
    [lhs,rhs]=argn(0)
    if ( rhs<3 | rhs>12 | rhs==9 ) then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"karmarkar",3,12));
    end
    //
    Aeq = varargin(1)
    beq = varargin(2)
    c = varargin(3)
    x0 = optim_argindefault ( rhs , varargin , 4 , [] )
    rtolf = optim_argindefault ( rhs , varargin , 5 , 1.d-5 )
    gam = optim_argindefault ( rhs , varargin , 6 , 1/2 )
    maxiter = optim_argindefault ( rhs , varargin , 7 , 200 )
    __karmarkar_outfun__ = optim_argindefault ( rhs , varargin , 8 , [] )
    A = optim_argindefault ( rhs , varargin , 9 , [] )
    b = optim_argindefault ( rhs , varargin , 10 , [] )
    lb = optim_argindefault ( rhs , varargin , 11 , [] )
    ub = optim_argindefault ( rhs , varargin , 12 , [] )
    //
    // Check input arguments
    //
    //
    // Check type
    //
    if ( typeof(Aeq) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",1));
    end
    if ( typeof(beq) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",2));
    end
    if ( typeof(c) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",3));
    end
    if ( typeof(x0) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",4));
    end
    if ( typeof(rtolf) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",5));
    end
    if ( typeof(gam) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",6));
    end
    if ( typeof(maxiter) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",7));
    end
    if ( __karmarkar_outfun__ <> [] ) then
        if ( and(typeof(__karmarkar_outfun__) <> ["function" "list" "fptr"] ) ) then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: %s expected.\n"),"karmarkar",8,"function or list"));
        end
        if ( typeof(__karmarkar_outfun__)=="list" ) then
            if ( and(typeof(__karmarkar_outfun__(1))<>["function" "fptr"] ) ) then
                error(msprintf(gettext("%s: Wrong type for input argument #%d: %s expected.\n"),"karmarkar",8,"function"));
            end
        end
    end
    if ( typeof(A) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",9));
    end
    if ( typeof(b) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",10));
    end
    if ( typeof(lb) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",11));
    end
    if ( typeof(ub) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",12));
    end
    //
    // Check size
    //
    [ne,pe]=size(Aeq)
    [ni,pi]=size(A)
    plb=size(lb,"*")
    pub=size(ub,"*")
    if ( Aeq <> [] ) then
        p = pe
    elseif ( A <> [] ) then
        p = pi
    elseif ( lb <> [] ) then
        p = plb
    elseif ( ub <> [] ) then
        p = pub
    else
        error(msprintf(gettext("%s: Either one of Aeq, A, lb or ub must be non-empty."),"karmarkar"));
    end
    // The case where Aeq==[] and A==[] is treated below.
    if ( ( Aeq <> [] & beq == [] ) | ( Aeq == [] & beq <> [] ) ) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",2));
    end
    if ( Aeq <> [] ) then
        if ( or ( size(beq) <> [ne 1] ) ) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",2));
        end
    end
    if ( or ( size(c) <> [p 1] ) ) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",3));
    end
    if ( x0 <> [] ) then
        if ( or ( size(x0) <> [p 1] ) ) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",4));
        end
    end
    if ( or ( size(rtolf) <> [1 1] ) ) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",5));
    end
    if ( or ( size(gam) <> [1 1] ) ) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",6));
    end
    if ( or ( size(maxiter) <> [1 1] ) ) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",7));
    end
    if ( A <> [] ) then
        if ( Aeq <> [] ) then
            if ( pi <> pe ) then
                error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",9));
            end
        end
    end
    if ( ( A <> [] & b == [] ) | ( A == [] & b <> [] ) ) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",10));
    end
    if ( b <> [] ) then
        if ( size(b) <> [ni 1] ) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",10));
        end
    end
    if ( lb <> [] ) then
        if ( or ( size(lb) <> [p 1] ) ) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",11));
        end
    end
    if ( ub <> [] ) then
        if ( or ( size(ub) <> [p 1] ) ) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",12));
        end
    end
    //
    // Check content
    //
    if ( rtolf < 0 | rtolf > 1 ) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d. rtolf must be in [0,1]."),"karmarkar",5));
    end
    if ( gam < 0 | gam > 1 ) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d. gam must be in [0,1]."),"karmarkar",6));
    end
    if ( maxiter < 1 ) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d. maxiter must be greater than 1."),"karmarkar",7));
    end
    if ( floor(maxiter) <> maxiter ) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d. maxiter must be a floating point integer."),"karmarkar",7));
    end
    if ( lb == [] & ub == [] ) then
        if ( x0 <> [] ) then
            if ( min(x0)<0 ) then
                error(msprintf(gettext("%s: Wrong value for input argument #%d. x0 is not positive."),"karmarkar",4));
            end
        end
    end
    if ( lb <> [] & ub <> [] ) then
        if ( or ( ub < lb ) ) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d. One entry of the upper bound ub is lower than the lower bound lb."),"karmarkar",12));
        end
    end
    if ( lb <> [] & x0 <> [] ) then
        if ( or ( lb > x0 ) ) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d. x0 lower than lower bound lb."),"karmarkar",12));
        end
    end
    if ( ub <> [] & x0 <> [] ) then
        if ( or ( ub < x0 ) ) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d. x0 greater than upper bound ub."),"karmarkar",12));
        end
    end
    //
    // Proceed
    //
    // Transform the general LP into standard form
    [AAeq,bbeq,cc,xx0,pinit,newposvars] = karmarkar_preprocess ( Aeq , beq , c , A , b , lb , ub , x0 )
    //
    // Solve the standard form LP
    outfun = list(__karmarkar_outfun__ , pinit , newposvars )
    [xxopt,fopt,exitflag,iter,yyopt] = karmarkar_findStandardLP ( AAeq , bbeq , cc , xx0 , rtolf , gam , maxiter , outfun )
    //
    // Extract the solution from the initial problem
    [xopt,fopt,yopt] = karmarkar_postprocess ( Aeq , beq , c , A , b , lb , ub , pinit , xxopt , yyopt , exitflag )

    if lhs < 3 & exitflag~= 1 then
        warning(msprintf(gettext("%s: The algorithm did not converge (exitflag= %d).\n"),"karmarkar",exitflag));
    end

endfunction

function [AAeq,bbeq,cc,xx0,pinit,newposvars] = karmarkar_preprocess ( Aeq , beq , c , A , b , lb , ub , x0 )
    // Transform the general LP into a standard LP.
    //
    // Parameters
    // Aeq : a ne-by-p matrix of doubles, the matrix of linear equality constraints
    // beq : a ne-by-1 matrix of doubles, the right hand side of linear equality constraints
    // A : a ni-by-p matrix of doubles, the matrix of linear inequality constraints
    // b : a ni-by-1 matrix of doubles, the right hand side of linear inequality constraints
    // c : a p-by-1 matrix of doubles, the linear objective
    // x0 : a p-by-1 matrix of doubles, the initial guess. If x0=[], this means that no initial guess is provided.
    // AAeq : a m-by-q matrix of doubles, the matrix of linear equality constraints. We have m = ne + ni and q = p + ni.
    // bbeq : a m-by-1 matrix of doubles, the right hand side of linear equality constraints
    // cc : a p-by-1 matrix of doubles, the linear objective
    // xx0 : a q-by-1 matrix of doubles, the initial guess. If xx0=[], this means that no initial guess is provided.
    // pinit : the initial value of p. The solution of the original general LP is xs(1:pinit), where xs is the solution of the modified standard LP.
    // newposvars : a 1-by-1 matrix of booleans, %f if no positive variables have been introduced, %t if positive variables have been introduced.
    //
    // Description
    // Transform the general linear program into a standard linear program.
    //
    // More precisely, if A <> [] transform the general linear program (L.P.):
    //
    // min c'*x
    // A * x <= b
    // Aeq * x = beq
    //
    // into the standard LP:
    //
    // min cc'*z
    // AAeq * z = bbeq
    // z >= 0
    //
    // where z is the new unknown, positive, which may contain slack variables.
    //
    // An unrestricted variable xi is transformed into x = xp - xn, where xp,xn >= 0.
    // This turns the inequality constraint Ax <= b into [A -A][xp;xn] <= b.
    // The inequality constraint is turned into an equality constraint by introducing
    // slack variables si.
    // This transforms the inequality into: [A -A][xp;xn] + s = b which can be written :
    //
    // [A -A I][xp;xn;s] = b
    //
    // Therefore, the number of variables increases from pinit to 2*pinit + ni.
    // The initial variable is x(1:pinit)-x(pinit+1:2*pinit).
    // The same happens for the equality constraints which is turned from
    // Aeq*x=b to [Aeq -Aeq 0][xp;xn;s]=b.
    // The cost function is turned from c'*x into [c;-c;0]'[xp;xn].
    //
    newposvars = %f
    //
    [ne,pe]=size(Aeq)
    [ni,pi]=size(A)
    plb=size(lb,"*")
    pub=size(ub,"*")
    if ( Aeq <> [] ) then
        pinit = pe
    elseif ( A <> [] ) then
        pinit = pi
    elseif ( lb <> [] ) then
        pinit = plb
    elseif ( ub <> [] ) then
        pinit = pub
    end
    //
    if ( A == [] & lb == [] & ub == [] ) then
        AAeq = Aeq
        bbeq = beq
        cc = c
        xx0 = x0
        return
    end
    //
    // Process the lower bound : x >= lb.
    // Move it as an inequality constraint: -I * x <= -lb.
    // TODO : process the case lb <> [] by shifting x (instead of introducing positive variables).
    if ( lb <> [] ) then
        if ( A == [] ) then
            A = -eye(plb,plb)
            b = -lb
        else
            A = [A;-eye(plb,plb)]
            b = [b;-lb]
        end
    end
    //
    // Process the upper bound : x <= ub.
    // Move it as an inequality constraint : I * x <= ub.
    if ( ub <> [] ) then
        if ( A == [] ) then
            A = eye(pub,pub)
            b = ub
        else
            A = [A;eye(pub,pub)]
            b = [b;ub]
        end
    end
    //
    // Remove constraints where b(i) = %inf.
    // Such an A(i,:)*x <= %inf = b(i) will be satisfied anyway, but
    // may cause failures in the algorithm.
    iinf = find(b == %inf)
    b(iinf) = []
    A(iinf,:) = []
    //
    // Create the map from the initial constraints to the final constraints.
    //
    // Update the number of inequalities,
    // given that the bounds have been updated.
    [ni,pi]=size(A)
    //
    // Initialize AAeq, bbeq, cc and xx0.
    //
    // If ni inequality constraints are given, transform the problem by
    // adding pinit positive variables and ni slack variables.
    // The inequality is Ax <= b.
    if ( A <> [] ) then
        //
        // Create the matrix
        // AAeq = [
        //     Aeq -Aeq 0
        //     A   -A   I
        // ]
        AAeq (1:ne+ni,1:2*pinit+ni) = zeros(ne+ni,2*pinit+ni)
        if ( Aeq <> [] ) then
            AAeq (1:ne,1:pinit) = Aeq
            AAeq (1:ne,pinit+1:2*pinit) = -Aeq
        end
        AAeq (ne+1:ne+ni,1:pinit) = A
        AAeq (ne+1:ne+ni,pinit+1:2*pinit) = -A
        AAeq (ne+1:ne+ni,2*pinit+1:2*pinit+ni) = eye(ni,ni)
        bbeq = [beq;b]
        cc = [c;-c;zeros(ni,1)]
        newposvars = %t
        if ( x0 == [] ) then
            xx0 = []
        else
            xx0 = zeros(2*pinit+ni,1)
            xx0(1:pinit) = max(x0,0)
            xx0(pinit+1:2*pinit) = -min(x0,0)
            s = b - A*x0
            if ( min(s)<0 ) then
                error(msprintf(gettext("%s: Wrong value for input argument #%d. x0 does not satisfy the inequality constraints."),"karmarkar",4));
            end
            xx0(2*pinit+1:2*pinit+ni) = s
        end
    end
endfunction

function [xopt,fopt,yopt] = karmarkar_postprocess ( Aeq , beq , c , A , b , lb , ub , pinit , xxopt , yyopt , exitflag )
    // Transform the solution of the standard LP into the solution of the original general LP.
    //
    // Extract the solution from the initial problem
    if ( pinit < size(xxopt,"*") ) then
        xopt = xxopt(1:pinit) - xxopt(pinit+1:2*pinit)
    else
        xopt = xxopt
    end
    //
    // Extract the dual solution.
    [ne,pe]=size(Aeq)
    [ni,pi]=size(A)
    plb=size(lb,"*")
    pub=size(ub,"*")
    //
    // Initialize
    if ( yyopt == [] ) then
        yopt.ineqlin = []
        yopt.eqlin = []
        yopt.lower = []
        yopt.upper = []
    else
        yopt.ineqlin = []
        yopt.eqlin = []
        yopt.lower = zeros(pinit,1)
        yopt.upper = zeros(pinit,1)
    end
    //
    // Update depending on the presence of the options.
    kstart = 1
    if ( ne > 0 ) then
        kstop = kstart + ne - 1
        yopt.eqlin = -yyopt(kstart:kstop)
        kstart = kstop + 1
    end
    if ( ni > 0 ) then
        noninf = find(b<>%inf)
        kinf = find(b==%inf)
        kstop = kstart + ni - 1 - size(kinf,"*")
        yopt.ineqlin(noninf) = -yyopt(kstart:kstop)
        kstart = kstop + 1
    end
    if ( ni == 0 & plb == 0 & pub == 0 ) then
        yopt.lower = c - Aeq'*yyopt
    elseif ( plb > 0 ) then
        noninf = find(lb<>%inf)
        kinf = find(lb==%inf)
        kstop = kstart + plb - 1 - size(kinf,"*")
        yopt.lower(noninf) = -yyopt(kstart:kstop)
        kstart = kstop + 1
    end
    if ( pub > 0 ) then
        noninf = find(ub<>%inf)
        kinf = find(ub==%inf)
        kstop = kstart + pub - 1 - size(kinf,"*")
        yopt.upper(noninf) = -yyopt(kstart:kstop)
        kstart = kstop + 1
    end
endfunction

function [xopt,fopt,exitflag,iter,yopt] = karmarkar_findStandardLP ( Aeq , beq , c , x0 , rtolf , gam , maxiter , outfun )
    // Solves a linear problem in standard form (searches for x0 if necessary).
    //
    // Parameters
    // pinit : a 1-by-1 matrix of floating point integers, the number of parameters before the introduction of slack variables.
    //
    // Given x0, uses a primal affine scaling (P.A.S.) algorithm to iteratively
    // find the solution of a linear program in standard form:
    //
    // min c'*x
    // Aeq*x = beq
    // x >= 0
    //
    // If x0 is the empty matrix, compute a strictly feasible point.
    //
    [ne,p]=size(Aeq)
    if ( x0 == [] ) then
        //
        // Compute a strictly feasible point.
        //
        // Add an extra variable x(p+1) and solve :
        //
        // min z(p+1)
        // AAeq*z = beq
        // z >= 0
        //
        // where z=[x(1) x(2) ... x(p) x(p+1)].
        // AAeq has the same number of rows, but one column more than Aeq.
        // The initial guess for the modified problem is z0=[1 1 ... 1]=e(n+1).
        // The last column of AAeq is beq-Aeq*e(n) so that the initial guess z0 satisfies AAeq*z0=beq.
        //
        // References
        // "A Primal-Dual Exterior Point Algorithm For Linear Programming Problems"
        // Nikolaos Samaras, Angelo Sifaleras, Charalampos Triantafyllidis
        // Yugoslav Journal of Operations Research
        // Vol 19 (2009), Number 1, 123-132
        //
        // "A modification of karmarkar's linear programming algorithm",
        // Robert J. Vanderbei, Marc S. Meketon and Barry A. Freedman,
        // Algorithmica, Volume 1, Numbers 1-4, 395-407, 1986.
        //
        s = beq - Aeq*ones(p,1)
        AAeq = [Aeq,s]
        cc = [zeros(p,1);1]
        xx0 = ones(p+1,1)
        step = 1
        xfeasmax = %eps
        iterstart = 0
        [xopt1,fopt1,exitflag1,iter1,yopt1] = karmarkar_findxopt ( AAeq , beq , cc , xx0 , rtolf , gam , maxiter , outfun , step , xfeasmax , iterstart )
        if ( exitflag1 <> 1 ) then
            // The algorithm did not converge.
            xopt = []
            fopt = []
            exitflag = -1
            iter = iter1
            yopt = []
            return
        end
        x0 = xopt1(1:p)
    else
        iter1 = 0
    end
    //
    // Check that the initial guess is feasible.
    if ( x0 <> [] ) then
        if ( norm(Aeq*x0-beq)>sqrt(%eps)*norm(beq) ) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d. x0 does not satisfy the equality constraints."),"karmarkar",4));
        end
    end
    //
    // Find the optimum x
    step = 2
    xfeasmax = %nan
    iterstart = iter1
    [xopt,fopt,exitflag,iter,yopt] = karmarkar_findxopt ( Aeq , beq , c , x0 , rtolf , gam , maxiter , outfun , step , xfeasmax , iterstart )
endfunction

function [xopt,fopt,exitflag,iter,yopt] = karmarkar_findxopt ( Aeq , beq , c , x0 , rtolf , gam , maxiter , outfun , step , xfeasmax , iterstart )
    // Solves a linear problem in standard form, given x0.
    //
    // Parameters
    // step : a 1-by-1 matrix of floating point integers, the kind of algorithm performed. We have step=1 during the search for a feasible point x0, and step=2 during the search for x*.
    // xfeasmax : a 1-by-1 matrix of double, the maximum value of the feasibility variable, during the search for a feasible point x0.
    // pinit : a 1-by-1 matrix of floating point integers, the number of parameters before the introduction of slack variables.
    // iterstart : the initial number of iterations, including the iterations of the previous steps
    //
    // Description
    // Given x0, uses a primal affine scaling (P.A.S.) algorithm to iteratively
    // find the solution of a linear program in standard form:
    //
    // min c'*x
    // Aeq*x = beq
    // x >= 0
    //
    // We assume that x0 is strictly feasible, i.e. || Aeq*x0-beq || is small and x0 > 0.
    //
    // If step = 1, we stop when xopt($) is below the feasibility threshold xfeasmax.
    // If step = 2, we stop when the objective function does not vary anymore, or the maximum number
    // of iterations exceeds the maximum, or the users asks to.
    //
    // exitflag = 1 if algorithm converged.
    // exitflag = 0 if maximum number of iterations was reached.
    // exitflag = -2 if problem is unbounded.
    // exitflag = -3 if search direction became zero.
    // exitflag = -4 if step=2 and algorithm stopped on user's request.
    // exitflag = -%inf on internal error.
    //
    // References
    //      "A variation on Karmarkar’s algorithm for solving linear programming problems,
    //      Earl R. Barnes, Mathematical Programming, Volume 36, Number 2, 174-182, 1986.
    //
    //      "A modification of karmarkar's linear programming algorithm",
    //      Robert J. Vanderbei, Marc S. Meketon and Barry A. Freedman,
    //      Algorithmica, Volume 1, Numbers 1-4, 395-407, 1986.
    //
    //      "Practical Optimization: Algorithms and Engineering Applications",
    //      Andreas Antoniou, Wu-Sheng Lu, Springer, 2007,
    //      Chapter 12, "Linear Programming Part II: Interior Point Methods".
    //
    [ne,p]=size(Aeq)
    xopt=x0
    yopt = []
    tc=c'
    fopt=tc*xopt
    dualgapmin = %inf
    dualgap = %inf
    funccount = 1
    fprev = fopt+1
    iter=iterstart
    s = zeros(p,1)
    stop = %f
    exitflag = -%inf
    firstloop = %t
    //
    state = "init"
    if ( step == 1 ) then
        procedure = "x0"
    else
        procedure = "x*"
    end
    optimValues = struct(...
    "funccount" , funccount , ...
    "fval" , fopt , ...
    "iteration" , iter , ...
    "procedure" , procedure, ...
    "dualgap" , dualgap ...
    );
    stop = karmarkar_outfunDriver ( xopt , optimValues , state , outfun )
    //
    while ( %t )
        if ( iter >= maxiter ) then
            exitflag = 0
            break
        end
        if ( step == 1 ) then
            if ( xopt($) <= xfeasmax ) then
                exitflag = 1
                break
            end
        end
        if ( step == 2 ) then
            if ( abs(fprev-fopt)<=rtolf*abs(fprev) ) then
                exitflag = 1
                break
            end
        end
        //
        // Compute the duality gap
        if ( dualgap > 1.e5 * dualgapmin ) then
            // Unbounded problem.
            exitflag = -2
            break
        end
        //
        // Calls back the output function
        if ( step == 1 ) then
            state = "init"
        else
            state = "iter"
        end
        if ( step == 1 ) then
            procedure = "x0"
        else
            procedure = "x*"
        end
        optimValues = struct(...
        "funccount" , funccount , ...
        "fval" , fopt , ...
        "iteration" , iter , ...
        "procedure" , procedure, ...
        "dualgap" , dualgap ...
        );
        stop = karmarkar_outfunDriver ( xopt , optimValues , state , outfun )
        if ( stop ) then
            exitflag = -4
            break
        end
        iter=iter+1
        // Compute B as B = Aeq*X where X = diag(xopt).
        // The following method is equivalent, but faster.
        xt = xopt'
        B = Aeq.*xt(ones(ne,1),:)
        v = xopt.*c
        // y = inv(B*B') * (B*v) i.e. y is the solution of (B*B') y = B*v.
        // This implies that y is the solution of (B')*y = v, i.e.
        yopt = B'\v
        p = -v+B'*yopt
        if ( min(p)==0 ) then
            exitflag = -3
            break
        end
        d = xopt.*p
        if ( min(d)>0 ) then
            // Unbounded problem.
            exitflag = -2
            break
        end
        alpha = -gam / min(p)
        s = alpha*d
        xopt=xopt+s
        fprev = fopt
        fopt=tc*xopt
        funccount = funccount + 1
        //
        // Compute the duality gap
        dualgap = abs(yopt'*beq - fopt)
        if ( firstloop ) then
            dualgapmin = dualgap
        else
            if ( dualgapmin > dualgap ) then
                dualgapmin = dualgap
            end
        end
        firstloop = %f
    end
    //
    if ( step == 1 ) then
        state = "init"
    else
        state = "done"
    end
    if ( step == 1 ) then
        procedure = "x0"
    else
        procedure = "x*"
    end
    optimValues = struct(...
    "funccount" , funccount , ...
    "fval" , fopt , ...
    "iteration" , iter , ...
    "procedure" , procedure, ...
    "dualgap" , dualgap ...
    );
    stop = karmarkar_outfunDriver ( xopt , optimValues , state , outfun )
endfunction

function stop = karmarkar_outfunDriver ( xopt , optimValues , state , outfun )
    //
    // The driver for the output function.
    // outfun : a list where the first item is the output function.
    __karmarkar_outfun__ = outfun (1)
    pinit = outfun (2)
    newposvars = outfun (3)
    //
    // Calls back the user's output function, if required.
    // Reduce the size of the vectors, to take into account for potential slack variables.
    if ( __karmarkar_outfun__ <> [] ) then
        if ( newposvars ) then
            xopt = xopt(1:pinit) - xopt(pinit+1:2*pinit)
        else
            xopt = xopt(1:pinit)
        end
        cbktype = typeof( __karmarkar_outfun__ )
        if ( cbktype == "list" ) then
            __karmarkar_outfun__f_ = __karmarkar_outfun__ (1)
            stop = __karmarkar_outfun__f_ ( xopt , optimValues , state , __karmarkar_outfun__ (2:$))
        elseif ( or(cbktype == ["function" "fptr"] ) ) then
            stop = __karmarkar_outfun__ ( xopt , optimValues , state )
        end
    else
        stop = %f
    end
endfunction


