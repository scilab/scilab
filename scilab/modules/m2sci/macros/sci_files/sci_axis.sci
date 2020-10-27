// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function tree = sci_axis(tree)
    // M2SCI function
    // Conversion function for Matlab axis()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    ax = Funcall("gca",1,list(),list())

    i = 1
    if rhs > 0 then
        if tree.rhs(1).vtype==Handle  // The first argin is the target
            ax = tree.rhs(1)
            if typeof(ax) <> "variable"
                ax = gettempvar()
                m2sci_insert(Equal(list(ax), tree.rhs(1)))
            end
            i = 2
        end
    end
    if rhs==(i-1) then
        // v = axis
        tree = Funcall("get",1,Rhs_tlist(ax,"data_bounds"),tree.lhs)
        tree.lhs(1).dims = list(2,Unknown)
        tree.lhs(1).type = Type(Double,Real)
        return
    end

    keepInputSyntax = %f
    for krhs = i:rhs
        if tree.rhs(krhs).vtype==Double then

            // axis([xmin xmax ymin ymax])
            // axis([xmin xmax ymin ymax zmin zmax])
            if or(tree.rhs(krhs).dims(2)==[4,6]) then
                mat = Funcall("matrix",1,list(tree.rhs(krhs),Cste(2),Cste(-1)),list())
                LHS = Operation("ins", list(ax,Cste("data_bounds")), list())
                m2sci_insert(Equal(list(LHS), tree.rhs(krhs)))

                // axis([xmin xmax ymin ymax zmin zmax cmin cmax])
            elseif tree.rhs(krhs).dims(2)==8 then
                no_equiv(expression2code(tree))
                keepInputSyntax = %t

                // Unknown column number for tree.rhs(krhs)
            else
                tree.name = "mtlb_axis"
                keepInputSyntax = %t
            end
        elseif tree.rhs(krhs).vtype==String then

            // Option is a character string
            if typeof(tree.rhs(krhs))=="cste" then

                // axis auto
                if tree.rhs(krhs).value=="auto" then
                    LHS = Operation("ins", list(ax,Cste("auto_scale")), list())
                    m2sci_insert(Equal(list(LHS), Cste("on")))

                    // axis manual
                elseif tree.rhs(krhs).value=="manual" then
                    LHS = Operation("ins", list(ax,Cste("auto_scale")), list())
                    m2sci_insert(Equal(list(LHS), Cste("off")))

                    // axis tight
                elseif tree.rhs(krhs).value=="tight" then
                    LHS = Operation("ins", list(ax,Cste("tight_limits")), list())
                    m2sci_insert(Equal(list(LHS), Cste("on")))

                    // axis fill
                elseif tree.rhs(krhs).value=="fill" then
                    no_equiv(expression2code(tree))
                    keepInputSyntax = %t

                    // axis ij
                elseif tree.rhs(krhs).value=="ij" then
                    axis_ij_xy(ax, Cste("on")) // sets .axes_reverse(2)="on"

                    // axis xy
                elseif tree.rhs(krhs).value=="xy" then
                    axis_ij_xy(ax, Cste("off")) // sets .axes_reverse(2)="off"

                    // axis equal
                elseif tree.rhs(krhs).value=="equal" then
                    LHS = Operation("ins", list(ax,Cste("isoview")), list())
                    m2sci_insert(Equal(list(LHS), Cste("on")))

                    // axis image
                elseif tree.rhs(krhs).value=="image" then
                    no_equiv(expression2code(tree))
                    keepInputSyntax = %t

                    // axis square
                elseif tree.rhs(krhs).value=="square" then
                    set_infos(gettext("cube_scaling only used in 3d mode."),2);
                    LHS = Operation("ins", list(ax,Cste("cube_scaling")), list())
                    m2sci_insert(Equal(list(LHS), Cste("on")))

                    // axis vis3d
                elseif tree.rhs(krhs).value=="vis3d" then
                    LHS = Operation("ins", list(ax,Cste("view")), list())
                    m2sci_insert(Equal(list(LHS), Cste("3d")))

                    // axis normal
                elseif tree.rhs(krhs).value=="normal" then
                    no_equiv(expression2code(tree))
                    keepInputSyntax = %t

                    // axis on
                elseif tree.rhs(krhs).value=="on" then
                    LHS = Operation("ins", list(ax,Cste("axes_visible")), list())
                    m2sci_insert(Equal(list(LHS), Cste("on")))

                    // axis off
                elseif tree.rhs(krhs).value=="off" then
                    LHS = Operation("ins", list(ax,Cste("axes_visible")), list())
                    m2sci_insert(Equal(list(LHS), Cste("off")))

                    // [mode,visibility,direction] = axis('state')
                elseif tree.rhs(krhs).value=="state" then
                    tree.name = "mtlb_axis"
                    keepInputSyntax = %t
                    // Unknown character string
                else
                    tree.name = "mtlb_axis"
                    keepInputSyntax = %t
                end

                // Option is a variable
            else
                tree.name = "mtlb_axis"
                keepInputSyntax = %t
            end

            // axis(axes_handles,...)
        elseif tree.rhs(krhs).vtype==Handle then
            no_equiv(expression2code(tree))
            keepInputSyntax = %t

            // Unknown type for tree.rhs(krhs)
        else
            tree.name = "mtlb_axis"
            keepInputSyntax = %t
        end
    end
    if ~keepInputSyntax then
        tree = list()
    end
endfunction

function axis_ij_xy(ax, value)
    if typeof(ax) <> "variable" | is_a_scalar(ax) // gca()
        LHS = Operation("ins", list(ax,Cste("axes_reverse")), list())
        LHS = Operation("ins", list(LHS,Cste(2)), list())
        m2sci_insert(Equal(list(LHS), value))
    else
        // handles(:) : (because .axes_reverse is a row)
        LHS = Operation("ins", list(ax,Colon), list())
        // handles(:).axes_reverse :
        LHS = Operation("ins", list(LHS,Cste("axes_reverse")), list())
        // handles(:).axes_reverse(:,2) :
        LHS = Operation("ins", list(LHS,Colon,Cste(2)), list())
        m2sci_insert(Equal(list(LHS), value))
    end
endfunction
