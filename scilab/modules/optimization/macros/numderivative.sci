// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ? - 2008 - Rainer von Seggern
// Copyright (C) ? - 2008 - Bruno Pincon
// Copyright (C) 2009 - INRIA - Michael Baudin
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [J, H] = numderivative(varargin)
    //
    // Check input arguments
    [lhs, rhs] = argn();
    if (rhs < 2 | rhs > 6) then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "numderivative", 2, 6));
    end
    if (lhs < 1 | lhs > 2) then
        error(msprintf(gettext("%s: Wrong number of output arguments: %d to %d expected.\n"), "numderivative", 1, 2));
    end
    //
    // Get input arguments
    __numderivative_f__ = varargin(1)
    if and(type(__numderivative_f__) <> [11 13 15 130]) then
        // Must be a function (uncompiled or compiled) or a list
        error(msprintf(gettext("%s: Wrong type for argument #%d: Function or list expected.\n"), "numderivative", 1));
    end
    if type(__numderivative_f__) == 15 then
        // List case
        // Check that the first element in the list is a function
        if and(type(__numderivative_f__(1)) <> [11 13]) then
            error(msprintf(gettext("%s: Wrong type for argument #%d: Function expected in first element of list.\n"), "numderivative", 1));
        end
        if length(__numderivative_f__) < 2 then
            error(msprintf(gettext("%s: Wrong number of elements in input argument #%d: At least %d elements expected, but current number is %d.\n"), "numderivative", 1, 2, length(__numderivative_f__)));
        end
    end

    //
    // Manage x, to get the size n.
    x = varargin(2);
    if type(x) ~= 1 then
        error(msprintf(gettext("%s: Wrong type for argument #%d: Matrix expected.\n"), "numderivative", 2));
    end
    [n, p] = size(x);
    if (n <> 1 & p <> 1) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Vector expected.\n"), "numderivative", 2));
    end
    // Make x a column vector, if required
    if p <> 1 then
        x = x(:);
        [n, p] = size(x);
    end
    //
    // Manage h: make it a column vector, if required.
    h = [];
    if rhs >= 3 then
        h = varargin(3);
        if type(h) ~= 1 then
            error(msprintf(gettext("%s: Wrong type for argument #%d: Matrix expected.\n"), "numderivative", 3));
        end
        if h <> [] then
            if size(h, "*") <> 1 then
                [nrows, ncols] = size(h);
                if (nrows <> 1 & ncols <> 1) then
                    error(msprintf(gettext("%s: Wrong size for input argument #%d: Vector expected.\n"), "numderivative", 3));
                end
                if ncols <> 1 then
                    h = h(:);
                end
                if or(size(h) <> [n 1]) then
                    error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"), "numderivative", 3, 1));
                end
            end
            if or(h < 0) then
                error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be > %d.\n"), "numderivative", 3, 0));
            end
        end
    end

    order = 2;
    if (rhs >= 4 & varargin(4) <> []) then
        order = varargin(4);
        if type(order) ~= 1 then
            error(msprintf(gettext("%s: Wrong type for argument #%d: Matrix expected.\n"), "numderivative", 4));
        end
        if or(size(order) <> [1 1]) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), "numderivative", 4, 1, 1));
        end
        if and(order <> [1 2 4]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set  {%s}.\n"), "numderivative", 4, "1, 2, 4"));
        end
    end

    H_form = "default";
    if (rhs >= 5 & varargin(5) <> []) then
        H_form = varargin(5);
        if type(H_form) ~= 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"), "numderivative", 5));
        end
        if or(size(H_form) <> [1 1]) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), "numderivative", 5, 1, 1));
        end
        if and(H_form <> ["default" "blockmat" "hypermat"]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set  {%s}.\n"), "numderivative", 5, "default, blockmat, hypermat"));
        end
    end

    Q = eye(n, n);
    Q_not_given = %t;
    if (rhs >= 6 & varargin(6) <> []) then
        Q = varargin(6);
        Q_not_given = %f;
        if type(Q) ~= 1 then
            error(msprintf(gettext("%s: Wrong type for argument #%d: Matrix expected.\n"), "numderivative", 6));
        end
        if or(size(Q) <> [n n]) then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), "numderivative", 6, n ,n));
        end
        if norm(clean(Q*Q'-eye(n, n))) > 0 then
            error(msprintf(gettext("%s: Q must be orthogonal.\n"), "numderivative"));
        end
    end

    //
    // Proceed...
    if h == [] then
        h_not_given = %t;
    else
        h_not_given = %f;
        // If h is scalar, expand to the same size as x.
        if size(h) == [1 1] then
            h = h * ones(x);
        end
    end
    //
    // Compute Jacobian
    if ( h_not_given ) then
        h = numderivative_step(x, order, 1);
    end
    J = numderivative_deriv1(__numderivative_f__, x, h, order, Q);
    //
    // Quick return if possible
    if lhs == 1 then
        return
    end

    m = size(J, 1);
    //
    // Compute Hessian matrix
    if ( h_not_given ) then
        h = numderivative_step(x, order, 2);
    end
    funForHList = list(numderivative_funForH, __numderivative_f__, h, order, Q);
    if ~Q_not_given then
        H = numderivative_deriv1(funForHList, x, h, order, Q);
    else
        H = numderivative_deriv2(funForHList, x, h, order, Q);
    end
    //
    // At this point, H is a m*n-by-n block matrix.
    // Update the format of the Hessian
    if H_form == "default" then
        // H has the old scilab form
        H = matrix(H', n*n, m)'
    end
    if H_form == "hypermat" then
        if m > 1 then
            // H is a hypermatrix if m > 1
            H = H';
            H = matrix(H(:), [n n m]);
        end
    end
endfunction

//
// numderivative_step --
//   Returns the step for given x, given order and given derivative:
//   d = 1 is for Jacobian
//   d = 2 is for Hessian
// Uses the optimal step.
// Then scale the step depending on abs(x).
function h = numderivative_step(x, order, d)
    n = size(x, "*");
    select d
    case 1
        // For Jacobian
        select order
        case 1
            hdefault = sqrt(%eps);
        case 2
            hdefault = %eps^(1/3);
        case 4
            hdefault = %eps^(1/5);
        else
            lclmsg = gettext("%s: Unknown value %s for option %s.\n");
            error(msprintf(lclmsg,"numderivative_step", string(d), "d"));
        end
    case 2
        // For Hessian
        select order
        case 1
            hdefault = %eps^(1/3);
        case 2
            hdefault = %eps^(1/4);
        case 4
            hdefault = %eps^(1/6);
        else
            lclmsg = gettext("%s: Unknown value %s for option %s.\n");
            error(msprintf(lclmsg, "numderivative_step", string(d), "d"));
        end
    else
        lclmsg = gettext("%s: Unknown value %s for option %s.\n");
        error(msprintf(lclmsg, "numderivative_step", string(order), "order"));
    end
    // Convert this scalar into a vector, with same size as x
    // For zero entries in x, use the default.
    // For nonzero entries in x, scales by abs(x).
    h = hdefault * abs(x);
    h(x==0) = hdefault;
endfunction

//
// numderivative_funForH --
//   Returns the numerical derivative of __numderivative_f__.
//   This function is called to compute the numerical Hessian.
function J = numderivative_funForH(x, __numderivative_f__, h, order, Q)
    // Transpose !
    J = numderivative_deriv1(__numderivative_f__, x, h, order, Q)';
    J = J(:);
endfunction

// numderivative_deriv1 --
//   Computes the numerical gradient of __numderivative_f__, using the given step h.
//   This function is used for the computation of the jacobian matrix.
function g = numderivative_deriv1(__numderivative_f__, x, h, order, Q)
    n = size(x, "*");
    %Dy = []; // At this point, we do not know 'm' yet, so we cannot allocate Dy.
    select order
    case 1
        D = Q * diag(h);
        y = numderivative_evalf(__numderivative_f__, x);
        for i=1:n
            d = D(:, i);
            yplus = numderivative_evalf(__numderivative_f__, x+d);
            Dyi = (yplus-y)/h(i);
            %Dy = [%Dy Dyi];
        end
        g = %Dy*Q';
    case 2
        D = Q * diag(h);
        for i=1:n
            d = D(:, i);
            yplus = numderivative_evalf(__numderivative_f__, x+d);
            yminus = numderivative_evalf(__numderivative_f__, x-d);
            Dyi = (yplus-yminus)/(2*h(i));
            %Dy = [%Dy Dyi];
        end
        g = %Dy*Q';
    case 4
        D = Q * diag(h);
        for i=1:n
            d = D(:, i);
            yplus = numderivative_evalf(__numderivative_f__, x+d);
            yminus = numderivative_evalf(__numderivative_f__, x-d);
            yplus2 = numderivative_evalf(__numderivative_f__, x+2*d);
            yminus2 = numderivative_evalf(__numderivative_f__, x-2*d);
            dFh =  (yplus-yminus)/(2*h(i));
            dF2h = (yplus2-yminus2)/(4*h(i));
            Dyi = (4*dFh - dF2h)/3;
            %Dy = [%Dy Dyi];
        end
        g = %Dy*Q';
    end
endfunction

// numderivative_deriv2 --
//   Computes the numerical gradient of the argument __numderivative_f__, using the given step h.
//   This function is used for the computation of the hessian matrix, to take advantage of its symmetry
function g = numderivative_deriv2(__numderivative_f__, x, h, order, Q)
    n = size(x, "*");
    %Dy = zeros(m*n, n); // 'm' is known at this point, so we can allocate Dy to reduce memory operations
    select order
    case 1
        D = Q * diag(h);
        y = numderivative_evalf(__numderivative_f__, x);
        for i=1:n
            d = D(:, i);
            yplus = numderivative_evalf(__numderivative_f__, x+d);
            for j=0:m-1
                Dyi(1+j*n:i-1+j*n) = %Dy(i+j*n, 1:i-1)'; // Retrieving symmetric elements (will not be done for the first vector)
                Dyi(i+j*n:(j+1)*n) = (yplus(i+j*n:(j+1)*n)-y(i+j*n:(j+1)*n))/h(i); // Computing the new ones
            end
            %Dy(:, i) = Dyi;
        end
        g = %Dy*Q';
    case 2
        D = Q * diag(h);
        for i=1:n
            d = D(:, i);
            yplus = numderivative_evalf(__numderivative_f__, x+d);
            yminus = numderivative_evalf(__numderivative_f__, x-d);
            for j=0:m-1
                Dyi(1+j*n:i-1+j*n) = %Dy(i+j*n, 1:i-1)'; // Retrieving symmetric elements (will not be done for the first vector)
                Dyi(i+j*n:(j+1)*n) = (yplus(i+j*n:(j+1)*n)-yminus(i+j*n:(j+1)*n))/(2*h(i)); // Computing the new ones
            end
            %Dy(:, i) = Dyi;
        end
        g = %Dy*Q';
    case 4
        D = Q * diag(h);
        for i=1:n
            d = D(:, i);
            yplus = numderivative_evalf(__numderivative_f__, x+d);
            yminus = numderivative_evalf(__numderivative_f__, x-d);
            yplus2 = numderivative_evalf(__numderivative_f__, x+2*d);
            yminus2 = numderivative_evalf(__numderivative_f__, x-2*d);
            for j=0:m-1
                dFh(1+j*n:i-1+j*n) = %Dy(i+j*n, 1:i-1)'; // Retrieving symmetric elements (will not be done for the first vector)
                dFh(i+j*n:(j+1)*n) = (yplus(i+j*n:(j+1)*n)-yminus(i+j*n:(j+1)*n))/(2*h(i)); // Computing the new ones
                dF2h(1+j*n:i-1+j*n) = %Dy(i+j*n, 1:i-1)'; // Retrieving symmetric elements (will not be done for the first vector)
                dF2h(i+j*n:(j+1)*n) = (yplus2(i+j*n:(j+1)*n)-yminus2(i+j*n:(j+1)*n))/(4*h(i)); // Computing the new ones
            end
            Dyi = (4*dFh - dF2h)/3;
            %Dy(:, i) = Dyi;
        end
        g = %Dy*Q';
    end
endfunction

// numderivative_evalf --
// Computes the value of __numderivative_f__ at the point x.
// The argument __numderivative_f__ can be a function (macro or linked code) or a list.
function y = numderivative_evalf(__numderivative_f__, x)
    if type(__numderivative_f__) == 15 then
        // List case
        __numderivative_fun__ = __numderivative_f__(1);
        instr = "y = __numderivative_fun__(x, __numderivative_f__(2:$))";
    elseif or(type(__numderivative_f__) == [11 13 130]) then
        // Function case
        instr = "y = __numderivative_f__(x)";
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A function expected.\n"), "numderivative", 1));
    end
    ierr = execstr(instr, "errcatch")
    if ierr <> 0 then
        lamsg = lasterror();
        lclmsg = "%s: Error while evaluating the function: ""%s""\n";
        error(msprintf(gettext(lclmsg), "numderivative", lamsg));
    end
endfunction
