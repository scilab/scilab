// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [retval, K] = unwrap(a, varargin)
    // [unwr, cuspPoints] = unwrap(a)        // row or column vector or matrix
    // [unwr, cuspPoints] = unwrap(a, jump)  // default jump = 2*%pi
    // jump=0 => = average local slope
    // [unwr, cuspPoints] = unwrap(a, jump, direc)
    //          direc="c": unwrap/unfold along columns
    //          direc="r": along rows
    //          else: along both directions (default)
    // [unwr, cuspPoints] = unwrap(a, "unfold")

    // Initializations
    // --------------
    [lhs,rhs] = argn(0);
    transposed = %f
    unfold = %f     // unfold (1D) instead of unwrap (1D|2D)
    jump = 2*%pi
    direc  = ""
    retval = []
    K = []

    // EXAMPLES
    // --------
    if rhs==0 then
        %_unwrap()      // display 1D examples
        halt(_("Press return to display 2D examples"))
        %_unwrap("2D")  // display 2D examples
        return
    end

    // =================== CHECKING PARAMETERS ====================
    if type(a)~=1 then
        msg = _("%s: Wrong type for input argument #%d: Real expected.\n")
        error(msprintf(msg, "unwrap",1))
    end
    if rhs>1
        if typeof(varargin(1))=="string" then
            if varargin(1)=="unfold" then
                unfold = %t
            else
                msg = _("%s: Wrong value for input argument #%d: Must be in the set  {%s}.\n")
                error(msprintf(msg, "unwrap",2,"""unfold"""))
            end
        else
            jump = varargin(1)
            if type(jump)~=1 then
                msg = _("%s: Wrong type for input argument #%d: Real expected.\n")
                error(msprintf(msg, "unwrap",2))
            else
                jump = abs(jump(1))
            end
        end
        if ~unfold & rhs>=3 then
            direc = varargin(2)
            if typeof(direc)~="string" then
                msg = _("%s: Wrong type for input argument #%d: String expected.\n")
                error(msprintf(msg, "unwrap", 3))
            else
                direc = direc(1)
                if and(direc~=["r" "c"]) then
                    msg = _("%s: Wrong value for input argument #%d: Must be in the set  {%s}.\n")
                    error(msprintf(msg, "unwrap", 3, """r"",""c"""))
                end
            end
        end
    end

    if direc=="c" | size(a,2)==1
        a = a.'
        transposed = %t
    end

    [m,n] = size(a)
    if (n < 4)
        if transposed then
            msg = _("%s: Wrong size for input argument #%d: Must have at least 4 rows.\n")
        else
            msg = _("%s: Wrong size for input argument #%d: Must have at least 4 columns.\n")
        end
        error(msprintf(msg, "unwrap",1))
    end
    if unfold & m>1 then
        msg = _("%s: Wrong size for input argument #%d: Vector expected.\n")
        error(msprintf(msg, "unwrap",1))
    end

    // ============================ PROCESSING ==============================

    // Columns #1 and #$ are duplicated:
    rae = [a(:,1)  a  a(:,$)];       // => n+2 columns
    // Derivative along rows (assuming equally spaced x values)
    d = rae(:,2:$) - rae(:, 1:$-1);     // n+1 columns
    clear rae

    // ---------------------------  U N W R A P  ---------------------------
    if ~unfold then
        // jump (may be)  (n-1 columns)
        ju = d(:, 2:$-1)
        // average Local derivative (before/after jump) (n-1 columns) :
        avL = (d(:, 1:$-2)+d(:, 3:$))/2
        // where
        wh = abs(ju)>4*abs(avL) & d(:, 1:$-2).*d(:, 3:$)>0
        wh = abs(ju)>5*abs(avL)
        K = find(wh)
        d = d(:, 2:$-1)                  // n-1 columns
        if jump~=0 then
            d(K) = ju(K) - sign(ju(K)).*jump
            // Cleaning wrongly inserted jumps :
            k = find(abs(d(K)-mean(d))>5*stdev(d))
            if ~isempty(k)
                d(K(k)) = d(K(k)) - sign(d(K(k))).*jump
            end
        else
            d(K) = avL(K)
        end

        if and(size(a)~=1) & direc==""
            // 2D case: levels all rows according to the unwrapped 1st column
            p = unwrap(a(:,1),jump)
        else
            p  = a(:,1)
        end
        r = [p d]

        // ---------------------------  U N F O L D  ---------------------------
    else
        // Local radius of curvature: calculation
        Rc = ((1+d(:,2:$).^2).^1.5) ./ abs(d(:,2:$)-d(:,1:$-1))
        K = find(Rc < mean(Rc)/30) // criterium to detect cusp points
        [nR,nC] = size(Rc)

        // trimming edges (due to duplication of 1st & last columns):
        tmp = find(K==1 | K==nC)
        K(tmp) = []

        // trimming duplicates (if any)
        tmp = find(K(2:$)-K(1:$-1)==1)
        K(tmp+1) = []

        // fine targetting of cusp points:
        k = find(d(K).*d(K+1)>0)
        K(k) = K(k)+1

        // We unfold:
        // The level and the slope at the left edge are taken as references.
        // The slope may be not of convenient sign. In this case, the
        //  unfolded profile will be upside down
        // One section over two must be kept as is, others must be flipped

        nCusp = length(K)   // number of detected cusp points
        if nCusp>0 then
            flip = %t
            // Main loop over cusp points:
            for p = 1:nCusp
                if flip then
                    iLeft = K(p)+1
                    if p<nCusp then
                        iRight = K(p+1)
                    else
                        iRight = nC
                    end
                    tmp = iLeft:iRight
                    d(tmp) = -d(tmp)        // the section is flipped
                    d(iLeft) = (d(iLeft-1)+d(iLeft+1))/2 // the reconnexion is smoothed
                end
                flip = ~flip    // one section / 2 is left as is
            end
        end

        // trimming both edges =>  n-1 columns:
        d = d(:, 2:$-1)

        // The first point is taken as anchor:
        r = [a(1)  d]
    end

    // Building the new profile from its restored derivative
    retval = cumsum(r,"c")

    // Post-processing
    if transposed then
        retval = retval.'
    end
endfunction
