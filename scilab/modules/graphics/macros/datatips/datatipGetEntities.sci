// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Gustavo Barbosa Libotte <gustavolibotte@gmail.com>
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function curve_handles=datatipGetEntities(ax)
    curve_handles=[];
    if argn(2)<1 then
        ax=gca()
        fig = ax.parent;
        for k=1:size(fig.children,"*")
            fc=fig.children(k);
            select fc.type
            case "Axes" then
                axes=fc;
                for j=1:size(axes.children,"*")
                    ac=axes.children(j);
                    select ac.type
                    case "Compound" then
                        compound=ac;
                        for i=1:size(compound.children,"*")
                            cc=compound.children(i);
                            select cc.type
                            case "Polyline" then
                                curve_handles=[curve_handles cc];
                            end
                        end
                    end
                end
            end
        end
    else
        if type(ax)<>9|size(ax,"*")<>1|and(ax.type<>["Axes" "Compound"]) then
            error(msprintf(_("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),"datatipGetEntities",1,"Axes"))
        else
            for j=1:size(ax.children,"*")
                ac=ax.children(j);
                select ac.type
                case "Compound" then
                    compound=ac;
                    for i=1:size(compound.children,"*")
                        cc=compound.children(i);
                        select cc.type
                        case "Polyline" then
                            curve_handles=[curve_handles cc];
                        end
                    end
                end
            end
        end
    end
endfunction
