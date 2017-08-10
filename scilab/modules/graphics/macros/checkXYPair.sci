// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [X,Y]=checkXYPair(typeOfPlot,x,y,current_figure,cur_draw_mode)

    ok=%F;

    if type(y)==13 // If y is a function
        f=y;
        if and(size(x)~=1)  // then x *must* be a vector
            ResetFigureDDM(current_figure, cur_draw_mode)
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A vector expected.\n"),typeOfPlot, 2));

            return;
        end

        t=x(:); // to ensure that t is a column vector

        [nArgOut,vectInput]=check2dFun(typeOfPlot,f,t,current_figure,cur_draw_mode);

        if nArgOut==1
            X=t;
            if vectInput
                Y=f(t);
            else
                Y=zeros(length(t),1);
                for i=1:length(t)
                    Y(i)=f(t(i));
                end
            end
        elseif nArgOut==2
            if vectInput
                [X,Y]=f(t);
            else
                X=zeros(length(t),1);
                Y=zeros(length(t),1);
                for i=1:length(t)

                    // CANNOT DO THE SAME WITH X(i) and Y(i)
                    // instead of xt and yt (scilab parser sees the stuff
                    // as a comparison)

                    [xt,yt]=f(t(i));
                    X(i)=xt;Y(i)=yt;
                end
            end
        end

    else // "classical" case

        X=x;
        Y=y;

        XScal = isscalar(X);
        YScal = isscalar(Y);

        transposeX = %f;
        transposeY = %f;

        if size(X,1)==1 & ~XScal, X=X', transposeX = %t; end; // Transpose the row vectors,
        if size(Y,1)==1 & ~YScal, Y=Y', transposeY = %t; end; // but no need to transpose scalars.

        if transposeX == ~transposeY then
            if transposeX == %t then
                warning(_("Transposing row vector X to get compatible dimensions"));
            else
                warning(_("Transposing row vector Y to get compatible dimensions"));
            end
        end

        if (size(X)==[0 0])
            ok=%F
            ResetFigureDDM(current_figure, cur_draw_mode)
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A non empty matrix expected.\n"),typeOfPlot, 2));
            return;
        end

        if (size(Y)==[0 0])
            ok=%F
            ResetFigureDDM(current_figure, cur_draw_mode)
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A non empty matrix expected.\n"),typeOfPlot, 3));
            return;
        end

        if and(size(X)==size(Y)) then
            // same size for X and Y
            ok=%T;

            return;
        end

        if (size(X,2)==1) & (size(Y,1)==size(X,1))
            // X is a vector
            ok=%T;

            return;
        end
        if (size(X,2)==1) & (size(Y,2)==size(X,1))
            // X is a column vector and Y has as many columns as X has rows.
            // Y cannot be a square matrix here, because it would have fallen in the previous case (above) and returned.
            if ~YScal then
                warning(_("Transposing data matrix Y to get compatible dimensions"));
                Y=Y';
            end
            ok=%T;

            return;
        end

        if (size(X,2) == 1) & (size(Y,2) == 1) & (size(Y,1) <> size(X,1)) ...
            & (size(Y,1) ~= 1)  then
            // X and Y are vectors but not of same size
            ResetFigureDDM(current_figure, cur_draw_mode)
            error(msprintf(gettext("%s: Wrong size for input arguments #%d and #%d: Incompatible dimensions.\n"),typeOfPlot, 2, 3));
            return;
        end

        // new case : plot(MAT4x4,[1 2 3 4]) HERE Y is a vector and X a matrix
        // Here Y is always a column vector
        // extend y to be a 4x4 matrix defined as [1 1 1 1;2 2 2 2;3 3 3 3;4 4 4 4]
        if or(size(Y) == 1) then
            if size(X,1) == size(Y,1) then
                y=Y;
            elseif size(X,1) == size(Y,2) & ~YScal then
                // Y has as many columns as X has rows. Transpose Y to fit X.
                warning(_("Transposing data matrix Y to get compatible dimensions"));
                y=Y(:);
            elseif size(X,2) == size(Y,1) & ~YScal & ~XScal then
                // Y has as many rows as X has columns. Transpose X to fit Y.
                warning(_("Transposing column vector X to get row vector"));
                X=X';
                y=Y(:);
            elseif size(X,2) == size(Y,2) then
                y=Y;
            else
                ResetFigureDDM(current_figure, cur_draw_mode)
                error(msprintf(gettext("%s: Wrong size for input arguments #%d and #%d: Incompatible dimensions.\n"),typeOfPlot,2, 3));
                return;
            end

            // concatenante y in columns
            Y=y(:,ones(1,size(X,2)));

            ok=%T;
            return;
        end

        if ~ok
            ResetFigureDDM(current_figure, cur_draw_mode)
            error(msprintf(gettext("%s: Wrong size for input arguments #%d and #%d: Incompatible dimensions.\n"),typeOfPlot, 2, 3));
            return;
        end


    end

    // end of checkXYPair
endfunction

