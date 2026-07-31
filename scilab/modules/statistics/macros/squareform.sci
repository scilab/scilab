// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function out = squareform(in, str)
    arguments
        in {mustBeA(in, "double")}
        str (1,1) {mustBeA(str, "string"), mustBeMember(str, ["default", "tomatrix", "tovector"])} = "default"
    end

    if isempty(in) then
        out = [];
        return
    end

    if str == "default" then
        if isvector(in) then
            str = "tomatrix";
        else
            str = "tovector";
        end
    end

    [m, n] = size(in);

    select str
    case "tomatrix"
        // case: vector -> matrix
        N = (1 + sqrt(1 + 8 * length(in)))/2;
        if abs(N - round(N)) > %eps then
            error(msprintf(_("%s: Wrong length for input argument #%d: Cannot form a square matrix from %d elements.\n"), "squareform", 1, length(in)));
        end
        out = zeros(N,N);
        out(tril(ones(N, N), -1) == 1) = in;
        out = out + out.';

    case "tovector"
        // case matrix -> vector
        // in must be square and symmetric matrix. This diagonal must only contain 0.
        if m <> n then
            error(msprintf(_("%s: Wrong type for input argument #%d: Must be a square matrix.\n"), "squareform", 1));
        end
        if norm(in-in.') <> 0 then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be a symmetric.\n"), "squareform", 1));
        end
        if or(diag(in) <> 0) then
            error(msprintf(_("%s: Wrong value for input argument #%d: the diagonal must only contain 0.\n"), "squareform", 1));
        end

        out = zeros(1, (n * (n - 1))/2);
        out = in(tril(ones(n,n), -1) == 1).';
    end
    
endfunction