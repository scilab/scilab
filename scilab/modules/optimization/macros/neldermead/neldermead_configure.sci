// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//
// neldermead_configure --
//   Configure neldermead and returns the modified object.
//
function this = neldermead_configure (this,key,value)
    UN=number_properties("tiny")
    [lhs,rhs]=argn();
    if ( rhs <> 3 ) then
        errmsg = msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "neldermead_configure", 3);
        error(errmsg)
    end
    select key
    case "-method" then
        nelmead_typestring ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkoption ( "neldermead_configure" , value , "value" , 3 , ["fixed" "variable" "box" "mine"])
        this.method = value
    case "-coords0" then
        nelmead_typereal ( value , "value" , 3 );
        this.coords0 = value;
    case "-simplex0method" then
        nelmead_typestring ( value , "value" , 3 );
        nelmead_checkoption ( "neldermead_configure" , value , "value" , 3 , ["given" "axes" "spendley" "pfeffer" "randbounds"])
        this.simplex0method = value
    case "-simplex0length" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , UN )
        this.simplex0length = value;
    case "-simplex0deltausual" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , UN )
        this.simplex0deltausual = value;
    case "-simplex0deltazero" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , UN )
        this.simplex0deltazero = value;
    case "-rho" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , UN )
        this.rho = value;
    case "-chi" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , UN )
        this.chi = value;
    case "-gamma" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkrange ( "neldermead_configure" , value , "value" , 3 , UN , 1-%eps )
        this.gamma = value;
    case "-sigma" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkrange ( "neldermead_configure" , value , "value" , 3 , UN , 1-%eps )
        this.sigma = value;
    case "-tolsimplexizeabsolute" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , 0 )
        this.tolsimplexizeabsolute = value;
    case "-tolsimplexizerelative" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , 0 )
        this.tolsimplexizerelative = value;
    case "-tolsimplexizemethod" then
        nelmead_typeboolean ( value , "value" , 3 )
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        this.tolsimplexizemethod = value;
    case "-toldeltafv" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , 0 )
        this.toldeltafv = value;
    case "-tolssizedeltafvmethod" then
        nelmead_typeboolean ( value , "value" , 3 )
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        this.tolssizedeltafvmethod = value;
    case "-restartmax" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , UN )
        nelmead_checkflint ( "neldermead_configure" , value , "value" , 3 )
        this.restartmax = value;
    case "-restarteps" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , UN )
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        this.restarteps = value;
    case "-restartstep" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , UN )
        n = optimbase_cget ( this.optbase , "-numberofvariables" );
        steprows = size ( value , "r" );
        stepcols = size ( value , "c" );
        if ( steprows * stepcols <> 1 ) then
            if ( ( steprows <> n ) | ( stepcols <> 1 ) ) then
                errmsg = msprintf(gettext("%s: The restartstep vector is expected to have %d x %d shape, but current shape is %d x %d"),"neldermead_configure",n,1,steprows,stepcols);
                error(errmsg);
            end
        end
        this.restartstep = value;
    case "-kelleystagnationflag" then
        nelmead_typeboolean ( value , "value" , 3 )
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        this.kelleystagnationflag = value;
    case "-kelleynormalizationflag" then
        nelmead_typeboolean ( value , "value" , 3 )
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        this.kelleynormalizationflag = value;
    case "-kelleystagnationalpha0" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , UN )
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        this.kelleystagnationalpha0 = value;
    case "-restartflag" then
        nelmead_typeboolean ( value , "value" , 3 )
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        this.restartflag = value;
    case "-restartdetection" then
        nelmead_typestring ( value , "value" , 3 )
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkoption ( "neldermead_configure" , value , "value" , 3 , ["oneill" "kelley"])
        this.restartdetection = value;
    case "-restartsimplexmethod" then
        nelmead_typestring ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkoption ( "neldermead_configure" , value , "value" , 3 , ["oriented" "axes" "spendley" "pfeffer" "randbounds"])
        this.restartsimplexmethod = value;
    case "-checkcostfunction" then
        nelmead_typeboolean ( value , "value" , 3 )
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        this.checkcostfunction = value;
    case "-boxnbpoints" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , UN )
        nelmead_checkflint ( "neldermead_configure" , value , "value" , 3 )
        this.boxnbpoints = value;
    case "-boxineqscaling" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkrange ( "neldermead_configure" , value , "value" , 3 , 0 , 1 )
        this.boxineqscaling = value;
    case "-scalingsimplex0" then
        nelmead_typestring ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkoption ( "neldermead_configure" , value , "value" , 3 , ["tox0" "tocenter"])
        this.scalingsimplex0 = value;
    case "-guinalphamin" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , UN )
        this.guinalphamin = value;
    case "-boxboundsalpha" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , 0 )
        this.boxboundsalpha = value
    case "-boxtermination" then
        nelmead_typeboolean ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        this.boxtermination = value
    case "-boxtolf" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , 0 )
        this.boxtolf = value
    case "-boxnbmatch" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , UN )
        nelmead_checkflint ( "neldermead_configure" , value , "value" , 3 )
        this.boxnbmatch = value
    case "-boxreflect" then
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , 1+%eps )
        this.boxreflect = value
    case "-mymethod" then
        nelmead_typefunction ( value , "value" , 3 );
        this.mymethod = value
    case "-greedy" then
        nelmead_typeboolean ( value , "value" , 3 )
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        this.greedy = value
        //
        // Obsolete options.
        //
    case "-myterminate" then
        nelmead_warnoptobs ( "-myterminate" , "-outputcommand" , "5.4.1" )
        nelmead_typefunction ( value , "value" , 3 );
        this.myterminate = value
    case "-myterminateflag" then
        nelmead_warnoptobs ( "-myterminateflag" , "-outputcommand" , "5.4.1" )
        nelmead_typeboolean ( value , "value" , 3 );
        this.myterminateflag = value;
    case "-tolvarianceflag" then
        nelmead_warnoptobs ( "-tolvarianceflag" , "-outputcommand" , "5.4.1" )
        nelmead_typeboolean ( value , "value" , 3 )
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        this.tolvarianceflag = value
    case "-tolabsolutevariance" then
        nelmead_warnoptobs ( "-tolabsolutevariance" , "-outputcommand" , "5.4.1" )
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , 0 )
        this.tolabsolutevariance = value
    case "-tolrelativevariance" then
        nelmead_warnoptobs ( "-tolrelativevariance" , "-outputcommand" , "5.4.1" )
        nelmead_typereal ( value , "value" , 3 );
        nelmead_checkscalar ( "neldermead_configure" , value , "value" , 3 )
        nelmead_checkgreq ( "neldermead_configure" , value , "value" , 3 , 0 )
        this.tolrelativevariance = value
    else
        // Delegate to the optimization object
        this.optbase = optimbase_configure ( this.optbase , key , value );
    end
endfunction
// Generates an error if the given variable is not of type real
function nelmead_typereal ( var , varname , ivar )
    if ( type ( var ) <> 1 ) then
        errmsg = msprintf(gettext("%s: Expected real variable for variable %s at input #%d, but got %s instead."),"nelmead_typereal", varname , ivar , typeof(var) );
        error(errmsg);
    end
endfunction
// Generates an error if the given variable is not of type string
function nelmead_typestring ( var , varname , ivar )
    if ( type ( var ) <> 10 ) then
        errmsg = msprintf(gettext("%s: Expected string variable for variable %s at input #%d, but got %s instead."),"nelmead_typestring", varname , ivar , typeof(var) );
        error(errmsg);
    end
endfunction
// Generates an error if the given variable is not of type function (macro)
function nelmead_typefunction ( var , varname , ivar )
    if ( type ( var ) <> 13 ) then
        errmsg = msprintf(gettext("%s: Expected function but for variable %s at input #%d, got %s instead."),"nelmead_typefunction", varname , ivar , typeof(var) );
        error(errmsg);
    end
endfunction
// Generates an error if the given variable is not of type boolean
function nelmead_typeboolean ( var , varname , ivar )
    if ( type ( var ) <> 4 ) then
        errmsg = msprintf(gettext("%s: Expected boolean but for variable %s at input #%d, got %s instead."),"nelmead_typeboolean", varname , ivar , typeof(var) );
        error(errmsg);
    end
endfunction

function nelmead_warnoptobs ( oldoption , newoption , removedVersion )
    warnMessage = msprintf(_("Option %s is obsolete."),oldoption)
    warnMessage = [warnMessage, msprintf(_("Please use %s instead."),newoption)]
    warnMessage = [warnMessage, msprintf(_("This feature will be permanently removed in Scilab %s"), removedVersion)]
    warning(warnMessage);
endfunction

function nelmead_checkrange ( funname , var , varname , ivar , vmin , vmax )
    if ( ~and ( vmin <= var & var <= vmax ) ) then
        k = find ( vmin > var | var > vmax )
        k = k(1)
        errmsg = msprintf(gettext("%s: Expected that all entries of input argument %s at input #%d are in the range [%s,%s], but entry #%d is equal to %s."),funname,varname,ivar,string(vmin),string(vmax),k,string(var(k)));
        error(errmsg);
    end
endfunction

function nelmead_checkgreq ( funname , var , varname , ivar , thr )
    if ( or ( var < thr ) ) then
        k = find ( var < thr )
        k = k(1)
        errmsg = msprintf(gettext("%s: Expected that all entries of input argument %s at input #%d are greater or equal than %s, but entry #%d is equal to %s."),funname,varname,ivar,string(thr),k,string(var(k)));
        error(errmsg);
    end
endfunction

function nelmead_checkflint ( funname , var , varname , ivar )
    if ( or ( round(var)<>var ) ) then
        k = find ( round(var)<>var )
        k = k(1)
        errmsg = msprintf(gettext("%s: Expected floating point integer for input argument %s at input #%d, but entry #%d is equal to %s."),funname,varname,ivar,k,string(var(k)));
        error(errmsg);
    end
endfunction

function nelmead_checkscalar ( funname , var , varname , ivar )
    if ( or(size(var) <> [1 1]) ) then
        strcomp = strcat(string(size(var))," ")
        errmsg = msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), funname, ivar , 1, 1 );
        error(errmsg)
    end
endfunction

function nelmead_checkoption ( funname , var , varname , ivar , expectedopt )
    if ( and ( var <> expectedopt ) ) then
        stradd = """ or """
        strexp = """" + strcat(string(expectedopt),stradd) + """"
        errmsg = msprintf(gettext("%s: Expected value [%s] for input argument %s at input #%d, but got ""%s"" instead."),funname,strexp,varname,ivar,string(var));
        error(errmsg);
    end
endfunction

