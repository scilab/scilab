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

function plotframe(rect,axisdata,options,legs,subwindow)
    // plotframe - fixes scales, tics and grid on a graphic,
    //%Syntax
    //  plotframe(rect,axisdata [,flags or leg or subwindow, ...)
    //%Parameters
    //  rect    : [xmin,ymin,xmax,ymax] data boudaries
    //  axisdata: [nx,mx,ny,my]  mx and my x and y tics, nx,ny : x and y subtics
    //  flags   : [quad,bounds] ou quad is a boolean if %t a grid is added
    //	      bounds a booleen also : if bounds is %t then rect can be modified
    //	      in order to have better scales on both axes which contains the
    //	      rect initial data.
    //  subwindow : see xsetech (wrect)
    //!
    [lhs,rhs]=argn(0)

    if rhs < 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "plotframe", 1));
        return ;
    end

    f_subwin   = %f ;
    f_flags    = %f ;
    f_captions = %f ;
    f_tics     = %f ;

    // check if we found optional args
    if exists("tics","local") == 1 then
        f_tics = %t ;
    end

    if exists("flags","local") == 1 then
        f_flags = %t ;
    else
        flags = [%f,%f] ;
    end

    if exists("Captions","local") == 1 then
        f_captions = %t ;
    end
    if exists("captions","local") == 1 then
        f_captions = %t ;
        Captions = captions;
    end

    if exists("subwin","local") == 1 then
        f_subwin = %t ;
    end

    if rhs >= 2 & ~f_subwin & ~f_captions & ~f_flags & ~f_tics then
        // no optional argument specified we use the old syntax
        // with 2,3,4 or five parameters

        f_tics = %t ;
        tics = axisdata ;
        if rhs == 5 then
            select type(subwindow),
            case 1 , subwin   = subwindow, f_subwin   = %t ;
            case 4 , flags    = subwindow, f_flags    = %t ;
            case 10, Captions = subwindow, f_captions = %t ;
            end
        end
        if rhs >= 4 then
            select type(legs),
            case 1 , subwin   = legs, f_subwin   = %t ;
            case 4 , flags    = legs, f_flags    = %t ;
            case 10, Captions = legs, f_captions = %t ;
            end
        end
        if rhs >= 3 then
            select type(options),
            case 1 , subwin   = options, f_subwin   = %t ;
            case 4 , flags    = options, f_flags    = %t ;
            case 10, Captions = options, f_captions = %t ;
            end
        end

    end


    if f_subwin then
        xsetech(subwin,rect);
    end
    // -- trace du cadre et des echelles
    if flags(2) then
        if f_tics then
            plot2d( [],[],0,"051"," ",rect,tics ) ;
        else
            plot2d( [],[],0,"051"," ",rect ) ;
        end
    else
        if f_tics then
            plot2d( [],[],0,"011"," ",rect, tics ) ;
        else
            plot2d( [],[],0,"011"," ",rect ) ;
        end
    end

    // -- trace des legendes d'axes et du titre
    if f_captions then
        select size(Captions,"*"),
        case 1, xtitle(Captions(1)) ;
        case 2, xtitle(Captions(1),Captions(2)) ;
        case 3, xtitle(Captions(1),Captions(2),Captions(3));
        end
    end

    if flags(1) then  xgrid(); end

endfunction
