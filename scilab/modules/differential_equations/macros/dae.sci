// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
// Copyright (C) 2008 - INRIA - Sabine GAUZERE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [varargout] = dae(varargin)

    [lhs, rhs] = argn();

    if size(varargin) == 0 then
        error(sprintf(gettext("%s: Wrong number of input argument(s): %d or %d expected.\n"), "dae", 3, 8));
    end
    if isdef("%DAEOPTIONS") == %F then
        if varargin(1) <> "root2" then
            %DAEOPTIONS = list ([], 0, [], [], [], 0, 0);
        else
            %DAEOPTIONS = list ([], 0, [], [], [], 0, [], 0, [], 0, [], [], [], 1);
        end
    end
    if type(varargin(1)) == 1 then //standard case (dassl)

        if rhs == 4 then //call without optional arguments
            [x0, t0, t, res] = varargin(:)
            if lhs == 2 then
                [y, hd] = dassl(x0, t0, t, res, %DAEOPTIONS);
            elseif lhs == 1 then
                [y] = dassl(x0, t0, t, res, %DAEOPTIONS);
            else
                error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 1, 2));
            end

        elseif rhs == 8 then // call with all the optional arguments
            [x0, t0, t, rtol, atol, res, jac, hd] = varargin(:)
            if lhs == 2 then
                [y, hd] = dassl(x0,t0, t, atol, rtol, res, jac, %DAEOPTIONS, hd);
            elseif lhs == 1 then
                [y] = dassl(x0, t0, t, atol, rtol, res, jac, %DAEOPTIONS, hd);
            else
                error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 1, 2));
            end

        elseif rhs == 5 then

            if type(varargin(4)) == 1 then
                [x0, t0, t, atol, res] = varargin(:)
                if lhs == 2 then
                    [y, hd] = dassl(x0, t0, t, atol, res, %DAEOPTIONS);
                elseif lhs == 1 then
                    [y] = dassl(x0, t0, t, atol, res, %DAEOPTIONS);
                else
                    error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 1, 2));
                end

            else

                if type(varargin(5)) == 1 then
                    [x0, t0, t, res, hd] = varargin(:)
                    if lhs == 2 then
                        [y, hd] = dassl(x0, t0, t, res, %DAEOPTIONS, hd);
                    elseif lhs == 1 then
                        [y] = dassl(x0, t0, t, res, %DAEOPTIONS, hd);
                    else
                        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 1, 2));
                    end

                else
                    [x0, t0, t, res, jac] = varargin(:)
                    if lhs == 2 then
                        [y, hd] = dassl(x0, t0, t, res, jac, %DAEOPTIONS);
                    elseif lhs == 1 then
                        [y] = dassl(x0, t0, t, res, jac, %DAEOPTIONS);
                    else
                        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 1, 2));
                    end

                end

            end

        elseif rhs == 6 then

            if type(varargin(4)) == 1 then

                if type(varargin(5)) == 1 then
                    [x0, t0, t, rtol, atol, res] = varargin(:)
                    if lhs == 2 then
                        [y, hd] = dassl(x0, t0, t, atol, rtol, res, %DAEOPTIONS);
                    elseif lhs == 1 then
                        [y] = dassl(x0, t0, t, atol, rtol, res, %DAEOPTIONS);
                    else
                        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 1, 2));
                    end

                else

                    if type(varargin(6)) == 1 then
                        [x0, t0, t, atol, res, hd] = varargin(:)
                        if lhs == 2 then
                            [y, hd] = dassl(x0, t0, t, atol, res, %DAEOPTIONS, hd);
                        elseif lhs == 1 then
                            [y] = dassl(x0, t0, t, atol, res, %DAEOPTIONS, hd);
                        else
                            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 1, 2));
                        end

                    else
                        [x0, t0, t, atol, res, jac] = varargin(:)
                        if lhs == 2 then
                            [y, hd] = dassl(x0, t0, t, atol, res, jac, %DAEOPTIONS);
                        elseif lhs == 1 then
                            [y] = dassl(x0, t0, t, atol, res, jac, %DAEOPTIONS);
                        else
                            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 1, 2));
                        end

                    end
                end

            else
                [x0, t0, t, res, jac, hd] = varargin(:)
                if lhs == 2 then
                    [y, hd] = dassl(x0, t0, t, res, jac, %DAEOPTIONS, hd);
                elseif lhs == 1 then
                    [y] = dassl(x0, t0, t, res, jac, %DAEOPTIONS, hd);
                else
                    error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 1, 2));
                end

            end

        elseif rhs == 7 then

            if type(varargin(5)) == 1 then

                if type(varargin(7)) == 1 then
                    [x0, t0, t, rtol, atol, res, hd] = varargin(:)
                    if lhs == 2 then
                        [y, hd] = dassl(x0, t0, t, atol, rtol, res, %DAEOPTIONS, hd);
                    elseif lhs == 1 then
                        [y] = dassl(x0, t0, t, atol, rtol, res, %DAEOPTIONS, hd);
                    else
                        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 1, 2));
                    end

                else
                    [x0, t0, t, rtol, atol, res, jac] = varargin(:)
                    if lhs == 2 then
                        [y, hd] = dassl(x0, t0, t, atol, rtol, res, jac, %DAEOPTIONS);
                    elseif lhs == 1 then
                        [y] = dassl(x0, t0, t, atol, rtol, res, jac, %DAEOPTIONS);
                    else
                        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 1, 2));
                    end

                end

            else
                [x0, t0, t, atol, res, jac, hd] = varargin(:)
                if lhs == 2 then
                    [y, hd] = dassl(x0, t0, t, atol, res, jac, %DAEOPTIONS, hd);
                elseif lhs == 1 then
                    [y] = dassl(x0, t0, t, atol, res, jac, %DAEOPTIONS, hd);
                else
                    error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 1, 2));
                end
            end

        else
            error(sprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "dae", 4, 8));
        end

        if %DAEOPTIONS(2) == 0 then
            [r, c] = size(y);
            y = y([2:1:r], :);
        end
        if lhs == 2 then
            varargout = list(y, hd);
        elseif lhs == 1 then
            varargout = list(y);
        else
            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 1, 2));
        end

    elseif varargin(1) == "root" then // Case root (dasrt)
        [lhs, rhs] = argn();

        if rhs == 7 then // Call without optional arguments
            [typ, x0, t0, t, res, ng, surface] = varargin(:)
            if lhs == 2 then
                [y, nn] = dasrt(x0, t0, t, res, ng, surface, %DAEOPTIONS);
            elseif lhs == 3 then
                [y, nn, hd] = dasrt(x0, t0, t, res, ng, surface, %DAEOPTIONS);
            else
                error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
            end

        elseif rhs == 11 then // Call with all the optional arguments
            [typ, x0, t0, t, rtol, atol, res, jac, ng, surface, hd] = varargin(:)
            if lhs == 2 then
                [y, nn] = dasrt(x0,t0,t,atol,rtol,res,jac,ng,surface,%DAEOPTIONS,hd);
            elseif lhs == 3 then
                [y, nn, hd] = dasrt(x0, t0, t, atol, rtol, res, jac, ng, surface, %DAEOPTIONS, hd);
            else
                error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
            end

        elseif rhs == 8 then

            if type(varargin(5)) == 1 then
                [typ, x0, t0, t, atol, res, ng, surface] = varargin(:)
                if lhs == 2 then
                    [y, nn] = dasrt(x0, t0, t, atol, res, ng, surface,%DAEOPTIONS);
                elseif lhs == 3 then
                    [y, nn, hd] = dasrt(x0, t0, t, atol, res, ng, surface, %DAEOPTIONS);
                else
                    error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                end

            else

                if type(varargin(8)) == 1 then
                    [typ, x0, t0, t, res, ng, surface, hd] = varargin(:)
                    if lhs == 2 then
                        [y, nn] = dasrt(x0, t0, t, res, ng, surface, %DAEOPTIONS, hd);
                    elseif lhs == 3 then
                        [y, nn, hd] = dasrt(x0, t0, t, res, ng, surface, %DAEOPTIONS, hd);
                    else
                        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                    end

                else
                    [typ, x0, t0, t, res, jac, ng, surface] = varargin(:)
                    if lhs == 2 then
                        [y, nn] = dasrt(x0, t0, t, res, jac, ng, surface, %DAEOPTIONS);
                    elseif lhs == 3 then
                        [y, nn, hd] = dasrt(x0, t0, t, res, jac, ng, surface, %DAEOPTIONS);
                    else
                        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                    end

                end

            end

        elseif rhs == 9 then

            if type(varargin(5)) == 1 then

                if type(varargin(6)) == 1 then
                    [typ, x0, t0, t, rtol, atol, res, ng, surface] = varargin(:)
                    if lhs == 2 then
                        [y, nn] = dasrt(x0, t0, t, atol, rtol, res, ng, surface, %DAEOPTIONS);
                    elseif lhs == 3 then
                        [y, nn, hd] = dasrt(x0, t0, t, atol, rtol, res, ng, surface, %DAEOPTIONS);
                    else
                        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                    end

                else

                    if type(varargin(9)) == 1 then
                        [typ, x0, t0, t, atol, res, ng, surface, hd] = varargin(:)
                        if lhs == 2 then
                            [y, nn] = dasrt(x0, t0, t, atol, res, ng, surface, %DAEOPTIONS, hd);
                        elseif lhs == 3 then
                            [y, nn, hd] = dasrt(x0, t0, t, atol, res, ng, surface, %DAEOPTIONS, hd);
                        else
                            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                        end

                    else
                        [typ, x0, t0, t, atol, res, jac, ng, surface] = varargin(:)
                        if lhs == 2 then
                            [y, nn] = dasrt(x0, t0, t, atol, res, jac, ng, surface, %DAEOPTIONS);
                        elseif lhs == 3 then
                            [y, nn, hd] = dasrt(x0, t0, t, atol, res, jac, ng, surface, %DAEOPTIONS);
                        else
                            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                        end

                    end
                end

            else
                [typ, x0, t0, t, res, jac, ng, surface, hd] = varargin(:)
                if lhs == 2 then
                    [y, nn] = dasrt(x0, t0, t, res, jac, ng, surface, %DAEOPTIONS, hd);
                elseif lhs == 3 then
                    [y, nn, hd] = dasrt(x0, t0, t, res, jac, ng, surface, %DAEOPTIONS, hd);
                else
                    error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                end
            end

        elseif rhs == 10 then

            if type(varargin(5)) == 1 then

                if type(varargin(6)) == 1 then

                    if type(varargin(10)) == 1 then
                        [typ, x0, t0, t, rtol, atol, res, ng, surface, hd] = varargin(:)
                        if lhs == 2 then
                            [y, nn] = dasrt(x0, t0, t, atol, rtol, res, ng, surface, %DAEOPTIONS, hd);
                        elseif lhs == 3 then
                            [y, nn, hd] = dasrt(x0, t0, t, atol, rtol, res, ng, surface, %DAEOPTIONS, hd);
                        else
                            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                        end

                    else
                        [typ, x0, t0, t, rtol, atol, res, jac, ng, surface] = varargin(:)
                        if lhs == 2 then
                            [y, nn] = dasrt(x0, t0, t, atol, rtol, res, jac, ng, surface, %DAEOPTIONS);
                        elseif lhs == 3 then
                            [y, nn, hd] = dasrt(x0, t0, t, atol, rtol, res, jac, ng, surface, %DAEOPTIONS);
                        else
                            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                        end
                    end

                else
                    [typ, x0, t0, t, atol, res, jac, ng, surface, hd] = varargin(:)
                    if lhs == 2 then
                        [y, nn] = dasrt(x0, t0, t, atol, res, jac, ng, surface, %DAEOPTIONS, hd);
                    elseif lhs == 3 then
                        [y, nn, hd] = dasrt(x0, t0, t, atol, res, jac, ng, surface, %DAEOPTIONS, hd);
                    else
                        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                    end

                end

            else
                [typ, x0, t0, t, res, jac, ng, surface, hd] = varargin(:)
                if lhs == 2 then
                    [y, nn] = dasrt(x0, t0, t, res, jac, ng, surface, %DAEOPTIONS, hd);
                elseif lhs == 3 then
                    [y, nn, hd] = dasrt(x0, t0, t, res, jac, ng, surface, %DAEOPTIONS, hd);
                else
                    error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                end
            end

        else
            error(sprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "dae", 7, 11));
        end
        if %DAEOPTIONS(2) == 0 then
            [r, c] = size(y);
            y = y([2:1:r], :);
        end
        if lhs == 2 then
            varargout = list(y, nn);
        elseif lhs == 3 then
            varargout = list(y, nn, hd);
        else
            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
        end

    elseif varargin(1) == "root2" then // Case root2 (daskr)
        [lhs, rhs] = argn();

        if rhs == 7 then // Call without optional arguments
            [typ, x0, t0, t, res, ng, surface] = varargin(:)
            if lhs == 2 then
                [y, nn] = daskr(x0, t0, t, res, ng, surface, %DAEOPTIONS);
            elseif lhs == 3 then
                [y, nn, hd] = daskr(x0, t0, t, res, ng, surface, %DAEOPTIONS);
            else
                error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
            end

        elseif rhs == 13 then // Call with all the optional arguments
            [typ, x0, t0, t, rtol, atol, res, jac, ng, surface, psol, pjac, hd] = varargin(:)
            if lhs == 2 then
                [y, nn] = daskr(x0,t0,t,atol,rtol,res,jac,ng,surface,%DAEOPTIONS, psol, pjac, hd);
            elseif lhs == 3 then
                [y, nn, hd] = daskr(x0, t0, t, atol, rtol, res, jac, ng, surface, %DAEOPTIONS, psol, pjac, hd);
            else
                error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
            end

        elseif rhs == 8 then

            if type(varargin(5)) == 1 then
                [typ, x0, t0, t, atol, res, ng, surface] = varargin(:)
                if lhs == 2 then
                    [y, nn] = daskr(x0, t0, t, atol, res, ng, surface,%DAEOPTIONS);
                elseif lhs == 3 then
                    [y, nn, hd] = daskr(x0, t0, t, atol, res, ng, surface, %DAEOPTIONS);
                else
                    error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                end

            else

                if type(varargin(8)) == 1 then
                    [typ, x0, t0, t, res, ng, surface, hd] = varargin(:)
                    if lhs == 2 then
                        [y, nn] = daskr(x0, t0, t, res, ng, surface, %DAEOPTIONS, hd);
                    elseif lhs == 3 then
                        [y, nn, hd] = daskr(x0, t0, t, res, ng, surface, %DAEOPTIONS, hd);
                    else
                        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                    end

                else
                    [typ, x0, t0, t, res, jac, ng, surface] = varargin(:)
                    if lhs == 2 then
                        [y, nn] = daskr(x0, t0, t, res, jac, ng, surface, %DAEOPTIONS);
                    elseif lhs == 3 then
                        [y, nn, hd] = daskr(x0, t0, t, res, jac, ng, surface, %DAEOPTIONS);
                    else
                        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                    end

                end

            end

        elseif rhs == 9 then

            if type(varargin(5)) == 1 then

                if type(varargin(6)) == 1 then
                    [typ, x0, t0, t, rtol, atol, res, ng, surface] = varargin(:)
                    if lhs == 2 then
                        [y, nn] = daskr(x0, t0, t, atol, rtol, res, ng, surface, %DAEOPTIONS);
                    elseif lhs == 3 then
                        [y, nn, hd] = daskr(x0, t0, t, atol, rtol, res, ng, surface, %DAEOPTIONS);
                    else
                        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                    end

                else

                    if type(varargin(9)) == 1 then
                        [typ, x0, t0, t, atol, res, ng, surface, hd] = varargin(:)
                        if lhs == 2 then
                            [y, nn] = daskr(x0, t0, t, atol, res, ng, surface, %DAEOPTIONS, hd);
                        elseif lhs == 3 then
                            [y, nn, hd] = daskr(x0, t0, t, atol, res, ng, surface, %DAEOPTIONS, hd);
                        else
                            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                        end

                    else
                        [typ, x0, t0, t, atol, res, jac, ng, surface] = varargin(:)
                        if lhs == 2 then
                            [y, nn] = daskr(x0, t0, t, atol, res, jac, ng, surface, %DAEOPTIONS);
                        elseif lhs == 3 then
                            [y, nn, hd] = daskr(x0, t0, t, atol, res, jac, ng, surface, %DAEOPTIONS);
                        else
                            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                        end

                    end
                end

            else
                if type(varargin(9)) == 1 then
                    [typ, x0, t0, t, res, jac, ng, surface, hd] = varargin(:)
                    if lhs == 2 then
                        [y, nn] = daskr(x0, t0, t, res, jac, ng, surface, %DAEOPTIONS, hd);
                    elseif lhs == 3 then
                        [y, nn, hd] = daskr(x0, t0, t, res, jac, ng, surface, %DAEOPTIONS, hd);
                    else
                        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                    end
                else
                    [typ, x0, t0, t, res, ng, surface, psol, pjac] = varargin(:)
                    if lhs == 2 then
                        [y, nn] = daskr(x0, t0, t, res, ng, surface, %DAEOPTIONS, psol, pjac);
                    elseif lhs == 3 then
                        [y, nn, hd] = daskr(x0, t0, t, res, ng, surface, %DAEOPTIONS, psol, pjac);
                    else
                        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                    end
                end

            end

        elseif rhs == 10 then

            if type(varargin(5)) == 1 then

                if type(varargin(6)) == 1 then

                    if type(varargin(10)) == 1 then
                        [typ, x0, t0, t, rtol, atol, res, ng, surface, hd] = varargin(:)
                        if lhs == 2 then
                            [y, nn] = daskr(x0, t0, t, atol, rtol, res, ng, surface, %DAEOPTIONS, hd);
                        elseif lhs == 3 then
                            [y, nn, hd] = daskr(x0, t0, t, atol, rtol, res, ng, surface, %DAEOPTIONS, hd);
                        else
                            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                        end

                    else
                        [typ, x0, t0, t, rtol, atol, res, jac, ng, surface] = varargin(:)
                        if lhs == 2 then
                            [y, nn] = daskr(x0, t0, t, atol, rtol, res, jac, ng, surface, %DAEOPTIONS);
                        elseif lhs == 3 then
                            [y, nn, hd] = daskr(x0, t0, t, atol, rtol, res, jac, ng, surface, %DAEOPTIONS);
                        else
                            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                        end
                    end

                else
                    if type(varargin(10)) == 1 then
                        [typ, x0, t0, t, atol, res, jac, ng, surface, hd] = varargin(:)
                        if lhs == 2 then
                            [y, nn] = daskr(x0, t0, t, atol, res, jac, ng, surface, %DAEOPTIONS, hd);
                        elseif lhs == 3 then
                            [y, nn, hd] = daskr(x0, t0, t, atol, res, jac, ng, surface, %DAEOPTIONS, hd);
                        else
                            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                        end
                    else
                        [typ, x0, t0, t, atol, res, ng, surface, psol, pjac] = varargin(:)
                        if lhs == 2 then
                            [y, nn] = daskr(x0, t0, t, atol, res, ng, surface, %DAEOPTIONS, psol, pjac);
                        elseif lhs == 3 then
                            [y, nn, hd] = daskr(x0, t0, t, atol, res, ng, surface, %DAEOPTIONS, psol, pjac);
                        else
                            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                        end
                    end

                end

            else
                if type(varargin(10)) == 1 then
                    [typ, x0, t0, t, res, ng, surface, psol, pjac, hd] = varargin(:)
                    if lhs == 2 then
                        [y, nn] = daskr(x0, t0, t, res, ng, surface, %DAEOPTIONS, psol, pjac, hd);
                    elseif lhs == 3 then
                        [y, nn, hd] = daskr(x0, t0, t, res, ng, surface, %DAEOPTIONS, psol, pjac, hd);
                    else
                        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                    end
                else
                    [typ, x0, t0, t, res, jac, ng, surface, psol, pjac] = varargin(:)
                    if lhs == 2 then
                        [y, nn] = daskr(x0, t0, t, res, jac, ng, surface, %DAEOPTIONS, psol, pjac);
                    elseif lhs == 3 then
                        [y, nn, hd] = daskr(x0, t0, t, res, jac, ng, surface, %DAEOPTIONS, psol, pjac);
                    else
                        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                    end
                end
            end

        elseif rhs == 11 then

            if type(varargin(5)) == 1 then

                if type(varargin(6)) == 1 then

                    if type(varargin(11)) == 1 then
                        [typ, x0, t0, t, rtol, atol, res, jac, ng, surface, hd] = varargin(:)
                        if lhs == 2 then
                            [y, nn] = daskr(x0, t0, t, atol, rtol, res, jac, ng, surface, %DAEOPTIONS, hd);
                        elseif lhs == 3 then
                            [y, nn, hd] = daskr(x0, t0, t, atol, rtol, res, jac, ng, surface, %DAEOPTIONS, hd);
                        else
                            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                        end

                    else
                        [typ, x0, t0, t, rtol, atol, res, ng, surface, psol, pjac] = varargin(:)
                        if lhs == 2 then
                            [y, nn] = daskr(x0, t0, t, atol, rtol, res, ng, surface, %DAEOPTIONS, psol, pjac);
                        elseif lhs == 3 then
                            [y, nn, hd] = daskr(x0, t0, t, atol, rtol, res, ng, surface, %DAEOPTIONS, psol, pjac);
                        else
                            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                        end
                    end

                else
                    if type(varargin(11)) == 1 then
                        [typ, x0, t0, t, atol, res, ng, surface, psol, pjac, hd] = varargin(:)
                        if lhs == 2 then
                            [y, nn] = daskr(x0, t0, t, atol, res, ng, surface, %DAEOPTIONS, psol, pjac, hd);
                        elseif lhs == 3 then
                            [y, nn, hd] = daskr(x0, t0, t, atol, res, ng, surface, %DAEOPTIONS, psol, pjac, hd);
                        else
                            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                        end
                    else
                        [typ, x0, t0, t, atol, res, jac, ng, surface, psol, pjac] = varargin(:)
                        if lhs == 2 then
                            [y, nn] = daskr(x0, t0, t, atol, res, jac, ng, surface, %DAEOPTIONS, psol, pjac);
                        elseif lhs == 3 then
                            [y, nn, hd] = daskr(x0, t0, t, atol, res, jac, ng, surface, %DAEOPTIONS, psol, pjac);
                        else
                            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                        end
                    end
                end

            else
                [typ, x0, t0, t, res, jac, ng, surface, psol, pjac, hd] = varargin(:)
                if lhs == 2 then
                    [y, nn] = daskr(x0, t0, t, res, jac, ng, surface, %DAEOPTIONS, psol, pjac, hd);
                elseif lhs == 3 then
                    [y, nn, hd] = daskr(x0, t0, t, res, jac, ng, surface, %DAEOPTIONS, psol, pjac, hd);
                else
                    error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                end
            end

        elseif rhs == 12 then

            if type(varargin(5)) == 1 then

                if type(varargin(6)) == 1 then

                    if type(varargin(12)) == 1 then
                        [typ, x0, t0, t, rtol, atol, res, ng, surface, psol, pjac, hd] = varargin(:)
                        if lhs == 2 then
                            [y, nn] = daskr(x0, t0, t, atol, rtol, res, ng, surface, %DAEOPTIONS, psol, pjac, hd);
                        elseif lhs == 3 then
                            [y, nn, hd] = daskr(x0, t0, t, atol, rtol, res, ng, surface, %DAEOPTIONS, psol, pjac, hd);
                        else
                            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                        end

                    else
                        [typ, x0, t0, t, rtol, atol, res, jac, ng, surface, psol, pjac] = varargin(:)
                        if lhs == 2 then
                            [y, nn] = daskr(x0, t0, t, atol, rtol, res, jac, ng, surface, %DAEOPTIONS, psol, pjac);
                        elseif lhs == 3 then
                            [y, nn, hd] = daskr(x0, t0, t, atol, rtol, res, jac, ng, surface, %DAEOPTIONS, psol, pjac);
                        else
                            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                        end
                    end

                else
                    [typ, x0, t0, t, atol, res, jac, ng, surface, psol, pjac, hd] = varargin(:)
                    if lhs == 2 then
                        [y, nn] = daskr(x0, t0, t, atol, res, jac, ng, surface, %DAEOPTIONS, psol, pjac, hd);
                    elseif lhs == 3 then
                        [y, nn, hd] = daskr(x0, t0, t, atol, res, jac, ng, surface, %DAEOPTIONS, psol, pjac, hd);
                    else
                        error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
                    end
                end
            end

        elseif rhs == 13 then

            [typ, x0, t0, t, rtol, atol, res, jac, ng, surface, psol, pjac, hd] = varargin(:)
            if lhs == 2 then
                [y, nn] = daskr(x0, t0, t, atol, rtol, res, jac, ng, surface, %DAEOPTIONS, psol, pjac, hd);
            elseif lhs == 3 then
                [y, nn, hd] = daskr(x0, t0, t, atol, rtol, res, jac, ng, surface, %DAEOPTIONS, psol, pjac, hd);
            else
                error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
            end

        else
            error(sprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "dae", 7, 13));
        end
        if %DAEOPTIONS(2) == 0 then
            [r, c] = size(y);
            y = y([2:1:r], :);
        end
        if lhs == 2 then
            varargout = list(y, nn);
        elseif lhs == 3 then
            varargout = list(y, nn, hd);
        else
            error(sprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "dae", 2, 3));
        end
    else
        error(sprintf(gettext("%s: Invalid option %s: real matrix expected.\n"), "dae", "root"));
    end

endfunction
