// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// ...
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [%ODEOPTIONS] = odeoptions(%ODEOPTIONS)

    //%ODEOPTIONS=[itask,tcrit,h0,hmax,hmin,jactyp,mxstep,..
    //             maxordn,maxords,ixpr, ml,mu]
    // This function displays the command line
    // %ODEOPTIONS=[...] to be executed
    // for defining the variable %ODEOPTIONS
    // This variables sets a number of optional parameters
    // for the lsod* fortran routines.

    //** This function can be (ab)used from the Scilab command line and
    //** inside a Scicos "context". In order to handle the different situations,
    //** the required libraries are loaded if not already present in the
    //** "semiglobal-local-environment".

    if exists("scicos_scicoslib")==0 then
        load("SCI/modules/scicos/macros/scicos_scicos/lib") ;
    end

    if exists("scicos_autolib")==0 then
        load("SCI/modules/scicos/macros/scicos_auto/lib") ;
    end

    if exists("scicos_utilslib")==0 then
        load("SCI/modules/scicos/macros/scicos_utils/lib") ;
    end

    options = [1,0,0,%inf,0,2,500,12,5,0,-1,-1];
    default = [string(options(1:10)),sci2exp(options(11:12))]
    default(find(default=="Inf"))="%inf"

    if argn(2)>0 then
        options=%ODEOPTIONS
    end

    lab_=[string(options(1:10)),sci2exp(options(11:12))]
    lab_(find(lab_=="Inf"))="%inf"


    chapeau=[gettext("Defining %ODEOPTIONS variable");
    "*****************************";
    gettext("Meaning of itask and tcrit:");
    gettext("1 : normal computation at specified times");
    gettext("2 : computation at mesh points (given in first row of output of ode)");
    gettext("3 : one step at one internal mesh point and return");
    gettext("4 : normal computation without overshooting tcrit");
    gettext("5 : one step, without passing tcrit, and return");
    " ";
    gettext("Meaning of jactype:");
    gettext("0 : functional iterations (no jacobian used (''adams'' or ''stiff'' only))");
    gettext("1 : user-supplied full jacobian");
    gettext("2 : internally generated full jacobian");
    gettext("3 : internally generated diagonal jacobian (''adams'' or ''stiff'' only)");
    gettext("4 : user-supplied banded jacobian (see ml,mu)");
    gettext("5 : internally generated banded jacobian (see ml,mu)");
    " ";
    gettext("Meaning of ml,mu:");
    gettext("If jactype = 4 or 5 ml and mu are the lower and upper half-bandwidths");
    gettext("of the banded jacobian: the band is the i,j''s with i-ml <= j <= ny-1");
    gettext("If jactype = 4 the jacobian function must return");
    gettext("a matrix J which is  ml+mu+1 x ny (where ny=dim of y in ydot=f(t,y))");
    gettext("such that column 1 of J is made of mu zeros followed by");
    gettext("df1/dy1, df2/dy1, df3/dy1,... (1+ml possibly non-zero entries)");
    gettext("column 2 is made of mu-1 zeros followed by df1/dx2, df2/dx2,etc");
    " ";
    gettext("Default values are given in square brackets");
    gettext("If the function is called without argument, default values are used");
    ]

    dims = list("vec",1,"vec",1,"vec",1,"vec",1,"vec",1,..
    "vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",2);


    labels = ["itask (1,2,3,4,5) ",gettext("tcrit (assumes itask=4 or 5)"),...
    gettext("h0 (first step tried)"),...
    gettext( "hmax (max step size)"),...
    gettext("hmin (min step size)"),...
    "jactype (0,1,2,3,4,5)",...
    gettext("mxstep (max number of steps allowed)"),...
    gettext("maxordn (maximum non-stiff order allowed, at most 12)"),...
    gettext("maxords (maximum stiff order allowed, at most 5)"),...
    gettext("ixpr (print level 0 or 1)"),"[ml,mu]"] +"    ["+default+"]";

    [ok,itask,tcrit,h0,hmax,hmin,jactyp,mxstep,maxordn,maxords,ixpr,mlmu] = getvalue(chapeau,labels,dims,lab_);
    ml = mlmu(1);
    mu = mlmu(2);
    ODEOPTIONS = [itask,tcrit,h0,hmax,hmin,jactyp,mxstep,maxordn,maxords,ixpr,ml,mu];

    if ODEOPTIONS<>[] then
        %ODEOPTIONS=ODEOPTIONS
    else
        %ODEOPTIONS=options
    end

endfunction
