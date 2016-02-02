// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function tree=sci_axis(tree)
    // M2SCI function
    // Conversion function for Matlab axis()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // Insert %v0=gca()
    a=Funcall("gca",1,list(),list())

    if rhs>0 then
        for krhs=1:rhs
            if tree.rhs(krhs).vtype==Double then

                // axis([xmin xmax ymin ymax])
                // axis([xmin xmax ymin ymax zmin zmax])
                if or(tree.rhs(krhs).dims(2)==[4,6]) then
                    mat=Funcall("matrix",1,list(tree.rhs(krhs),Cste(2),Cste(-1)),list())
                    tree=Funcall("set",1,Rhs_tlist(a,"data_bounds",mat),tree.lhs)

                    // axis([xmin xmax ymin ymax zmin zmax cmin cmax])
                elseif tree.rhs(krhs).dims(2)==8 then
                    no_equiv(expression2code(tree))

                    // Unknown column number for tree.rhs(krhs)
                else
                    tree.name="mtlb_axis"
                end
            elseif tree.rhs(krhs).vtype==String then

                // Option is a character string
                if typeof(tree.rhs(krhs))=="cste" then

                    // axis auto
                    if tree.rhs(krhs).value=="auto" then
                        tree=Funcall("set",1,Rhs_tlist(a,"auto_scale","on"),tree.lhs)

                        // axis manual
                    elseif tree.rhs(krhs).value=="manual" then
                        tree=Funcall("set",1,Rhs_tlist(a,"auto_scale","off"),tree.lhs)

                        // axis tight
                    elseif tree.rhs(krhs).value=="tight" then
                        tree=Funcall("set",1,Rhs_tlist(a,"tight_limits","on"),tree.lhs)

                        // axis fill
                    elseif tree.rhs(krhs).value=="fill" then
                        no_equiv(expression2code(tree))

                        // axis ij
                    elseif tree.rhs(krhs).value=="ij" then
                        mat=Operation("cc",list(Cste(180),Cste(270)),list())
                        tree=Funcall("set",1,Rhs_tlist(a,"rotation_angles",mat),tree.lhs)

                        // axis xy
                    elseif tree.rhs(krhs).value=="xy" then
                        mat=Operation("cc",list(Cste(0),Cste(270)),list())
                        tree=Funcall("set",1,Rhs_tlist(a,"rotation_angles",mat),tree.lhs)

                        // axis equal
                    elseif tree.rhs(krhs).value=="equal" then
                        tree=Funcall("set",1,Rhs_tlist(a,"isoview","on"),tree.lhs)

                        // axis image
                    elseif tree.rhs(krhs).value=="image" then
                        no_equiv(expression2code(tree))

                        // axis square
                    elseif tree.rhs(krhs).value=="square" then
                        set_infos(gettext("cube_scaling only used in 3d mode."),2);
                        tree=Funcall("set",1,Rhs_tlist(a,"cube_scaling","on"),tree.lhs)

                        // axis vis3d
                    elseif tree.rhs(krhs).value=="vis3d" then
                        tree=Funcall("set",1,Rhs_tlist(a,"view","3d"),tree.lhs)

                        // axis normal
                    elseif tree.rhs(krhs).value=="normal" then
                        no_equiv(expression2code(tree))

                        // axis on
                    elseif tree.rhs(krhs).value=="on" then
                        tree=Funcall("set",1,Rhs_tlist(a,"axes_visible","on"),tree.lhs)

                        // axis off
                    elseif tree.rhs(krhs).value=="off" then
                        tree=Funcall("set",1,Rhs_tlist(a,"axes_visible","off"),tree.lhs)

                        // [mode,visibility,direction] = axis('state')
                    elseif tree.rhs(krhs).value=="state" then
                        tree.name="mtlb_axis"

                        // Unknown character string
                    else
                        tree.name="mtlb_axis"
                    end

                    // Option is a variable
                else
                    tree.name="mtlb_axis"
                end

                // axis(axes_handles,...)
            elseif tree.rhs(krhs).vtype==Handle then
                no_equiv(expression2code(tree))

                // Unknown type for tree.rhs(krhs)
            else
                tree.name="mtlb_axis"
            end
        end
        // v = axis
    else
        tree=Funcall("set",1,Rhs_tlist(a,"data_bounds"),tree.lhs)
        tree.lhs(1).dims=list(2,Unknown)
        tree.lhs(1).type=Type(Double,Real)
    end
endfunction
