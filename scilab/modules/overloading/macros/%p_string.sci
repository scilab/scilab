// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

function txt=%p_string(p)
    [m,n]=size(p)
    s=stripblanks(varn(p))
    txt=emptystr(2*m,n)

    for l=1:m
        for k=1:n
            c=coeff(p(l,k))
            knz=find(c<>0)
            if knz==[] then
                txt(2*l-1:2*l,k)=[" ";"0"];
            else
                c=c(knz)
                //special case for i=1
                if isreal(c(1),0) then
                    w=real(c(1))
                    if w<0 then
                        C(1)=" - "+string(abs(c(1))),
                    else
                        C(1)=string(c(1)),
                    end
                else
                    w=string(c(1));
                    if knz(1)<> 1 then w="("+w+")",end
                    C(1)=w;
                end

                for i=2:size(c,"*"),
                    if isreal(c(i),0) then
                        w=real(c(i))
                        if w<0 then
                            C(i)=" - "+string(abs(c(i))),
                        else
                            C(i)=" + "+string(c(i)),
                        end
                    else
                        C(i)=" + ("+string(c(i))+")";
                    end
                end

                if knz(1)>1 then
                    C=C+s
                else
                    if ~isempty(s) && (size(C, "*") >= 2)
                        C(2:$)=C(2:$)+s
                    end
                end

                i=min(find(knz>2));
                blank=" ";
                if ~isempty(i)
                    e=blank(ones(1,i-1));
                else
                    e = [];
                end
                if size(knz,"*")>=i then
                    e=[e string(knz(i:$)-1)];
                end

                lc=cumsum(length(C))
                C=strcat(C)
                E="";
                if isempty(e)
                    for i=1:size(c,"*")
                        E = E + part(" ", 1:(lc(i) - length(E)));
                    end
                else
                    for i=1:size(c,"*")
                        E = E + part(" ", 1:(lc(i) - length(E))) + e(i);
                    end
                end
                txt(2*l-1:2*l,k)=[E;C];
            end
        end
    end
endfunction
