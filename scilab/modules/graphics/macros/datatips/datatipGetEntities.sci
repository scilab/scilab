// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Gustavo Barbosa Libotte <gustavolibotte@gmail.com>
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
