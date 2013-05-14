// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


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

        if size(X,1)==1, X=X', end;  // si l'un des vecteurs est une ligne
        if size(Y,1)==1, Y=Y', end;  // on le transpose.

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
            // X is a vector
            Y=Y';
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
            elseif size(X,1) == size(Y,2) then
                y=Y(:);
            elseif size(X,2) == size(Y,1) then
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

