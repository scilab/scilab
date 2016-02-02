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
// optimbase_configure --
//   Configure the current object with the given value for the given key.
//
function this = optimbase_configure (this,key,value)
    UN=number_properties("tiny")
    [lhs,rhs]=argn();
    if ( rhs <> 3 ) then
        errmsg = msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "optimbase_configure", 3);
        error(errmsg)
    end
    select key
    case "-verbose" then
        optimbase_typereal ( value , "value" , 3 );
        optimbase_checkscalar ( "optimbase_configure" , value , "value" , 3 )
        optimbase_checkgreq ( "optimbase_configure" , value , "value" , 3 , 0 )
        this.verbose = value;
    case "-verbosetermination" then
        optimbase_typereal ( value , "value" , 3 );
        optimbase_checkscalar ( "optimbase_configure" , value , "value" , 3 )
        optimbase_checkgreq ( "optimbase_configure" , value , "value" , 3 , 0 )
        this.verbosetermination = value;
    case "-logfile" then
        optimbase_typestring ( value , "value" , 3 );
        optimbase_checkscalar ( "optimbase_configure" , value , "value" , 3 )
        if ( this.logstartup ) then
            this = optimbase_logshutdown ( this )
        end
        this.logfile = value;
        this = optimbase_logstartup ( this );
    case "-x0" then
        optimbase_typereal ( value , "value" , 3 );
        [n,m] = size(value);
        if m<>1 then
            if n==1 then // Allowing row vectors by transposing them into column vectors
                value = value';
                temp = m; // Switch the sizes in case we want to reuse them later in this function
                m = n;
                n = temp;
            else
                error(msprintf(gettext("%s: Wrong size for x0 argument: A vector expected."),"optimbase_configure"));
            end
        end
        this.x0 = value;
        this.numberofvariables = n; // Setting x0 also sets the size of the system
    case "-maxfunevals" then
        optimbase_typereal ( value , "value" , 3 );
        optimbase_checkscalar ( "optimbase_configure" , value , "value" , 3 )
        optimbase_checkgreq ( "optimbase_configure" , value , "value" , 3 , 1 )
        this.maxfunevals = value;
    case "-maxiter" then
        optimbase_typereal ( value , "value" , 3 );
        optimbase_checkscalar ( "optimbase_configure" , value , "value" , 3 )
        optimbase_checkgreq ( "optimbase_configure" , value , "value" , 3 , 1 )
        this.maxiter = value;
    case "-tolfunabsolute" then
        optimbase_typereal ( value , "value" , 3 );
        optimbase_checkscalar ( "optimbase_configure" , value , "value" , 3 )
        optimbase_checkgreq ( "optimbase_configure" , value , "value" , 3 , 0 )
        this.tolfunabsolute = value;
    case "-tolfunrelative" then
        optimbase_typereal ( value , "value" , 3 );
        optimbase_checkscalar ( "optimbase_configure" , value , "value" , 3 )
        optimbase_checkgreq ( "optimbase_configure" , value , "value" , 3 , 0 )
        this.tolfunrelative = value;
    case "-tolxabsolute" then
        optimbase_typereal ( value , "value" , 3 );
        optimbase_checkscalar ( "optimbase_configure" , value , "value" , 3 )
        optimbase_checkgreq ( "optimbase_configure" , value , "value" , 3 , 0 )
        this.tolxabsolute = value;
    case "-tolxrelative" then
        optimbase_typereal ( value , "value" , 3 );
        optimbase_checkscalar ( "optimbase_configure" , value , "value" , 3 )
        optimbase_checkgreq ( "optimbase_configure" , value , "value" , 3 , 0 )
        this.tolxrelative = value;
    case "-tolxmethod" then
        optimbase_typeboolean ( value , "value" , 3 );
        optimbase_checkscalar ( "optimbase_configure" , value , "value" , 3 )
        this.tolxmethod = value;
    case "-tolfunmethod" then
        optimbase_typeboolean ( value , "value" , 3 );
        optimbase_checkscalar ( "optimbase_configure" , value , "value" , 3 )
        this.tolfunmethod = value;
    case "-function" then
        optimbase_typecallable ( value , "value" , 3)
        this.fun = value;
    case "-outputcommand" then
        optimbase_typecallable ( value , "value" , 3)
        this.outputcommand = value;
    case "-numberofvariables" then
        optimbase_typereal ( value , "value" , 3 );
        optimbase_checkscalar ( "optimbase_configure" , value , "value" , 3 )
        optimbase_checkgreq ( "optimbase_configure" , value , "value" , 3 , 1 )
        this.numberofvariables = value;
    case "-storehistory" then
        optimbase_typeboolean ( value , "value" , 3 );
        optimbase_checkscalar ( "optimbase_configure" , value , "value" , 3 )
        this.storehistory = value;
    case "-boundsmin" then
        optimbase_typereal ( value , "value" , 3 );
        this.boundsmin = value;
    case "-boundsmax" then
        optimbase_typereal ( value , "value" , 3 );
        this.boundsmax = value;
    case "-nbineqconst" then
        optimbase_typereal ( value , "value" , 3 );
        optimbase_checkscalar ( "optimbase_configure" , value , "value" , 3 )
        optimbase_checkgreq ( "optimbase_configure" , value , "value" , 3 , 0 )
        this.nbineqconst = value;
    case "-withderivatives" then
        optimbase_typeboolean ( value , "value" , 3 );
        optimbase_checkscalar ( "optimbase_configure" , value , "value" , 3 )
        this.withderivatives = value;
        //
        // Obsolete options.
        //
    else
        errmsg = msprintf(gettext("%s: Unknown key %s"),"optimbase_configure",key)
        error(errmsg)
    end
endfunction
// Generates an error if the given variable is not of type real
function optimbase_typereal ( var , varname , ivar )
    if ( type ( var ) <> 1 ) then
        errmsg = msprintf(gettext("%s: Expected real variable for variable %s at input #%d, but got %s instead."),"optimbase_typereal", varname , ivar , typeof(var) );
        error(errmsg);
    end
endfunction
// Generates an error if the given variable is not of type string
function optimbase_typestring ( var , varname , ivar )
    if ( type ( var ) <> 10 ) then
        errmsg = msprintf(gettext("%s: Expected string variable for variable %s at input #%d, but got %s instead."),"optimbase_typestring", varname , ivar , typeof(var) );
        error(errmsg);
    end
endfunction
// Generates an error if the given variable is not of type function (macro)
function optimbase_typefunction ( var , varname , ivar )
    if ( type ( var ) <> 13 ) then
        errmsg = msprintf(gettext("%s: Expected function but for variable %s at input #%d, got %s instead."),"optimbase_typefunction", varname , ivar , typeof(var) );
        error(errmsg);
    end
endfunction
// Generates an error if the given variable is not of type boolean
function optimbase_typeboolean ( var , varname , ivar )
    if ( type ( var ) <> 4 ) then
        errmsg = msprintf(gettext("%s: Expected boolean but for variable %s at input #%d, got %s instead."),"optimbase_typeboolean", varname , ivar , typeof(var) );
        error(errmsg);
    end
endfunction

// Generates an error if the value corresponding to an option is unknown.
function unknownValueForOption ( value , optionname )
    errmsg = msprintf(gettext("%s: Unknown value %s for %s option"),"unknownValueForOption",value , optionname );
    error(errmsg);
endfunction

function optimbase_typecallable ( var , varname , ivar )
    // Check that var is a function or a list
    if ( and ( type ( var ) <> [11 13 15] ) ) then
        errmsg = msprintf(gettext("%s: Expected function or list for variable %s at input #%d, but got %s instead."),"optimbase_typecallable", varname , ivar , typeof(var) );
        error(errmsg);
    end
    if ( type ( var ) == 15 ) then
        // Check that var(1) is a function
        if ( and ( type ( var(1) ) <> [11 13] ) ) then
            errmsg = msprintf(gettext("%s: Expected function for variable %s(1) at input #%d, but got %s instead."),"optimbase_typecallable", varname , ivar , typeof(var) );
            error(errmsg);
        end
    end
endfunction

function optimbase_checkrange ( funname , var , varname , ivar , vmin , vmax )
    if ( ~and ( vmin <= var & var <= vmax ) ) then
        k = find ( vmin > var | var > vmax )
        k = k(1)
        errmsg = msprintf(gettext("%s: Expected that all entries of input argument %s at input #%d are in the range [%s,%s], but entry #%d is equal to %s."),funname,varname,ivar,string(vmin),string(vmax),k,string(var(k)));
        error(errmsg);
    end
endfunction

function optimbase_checkgreq ( funname , var , varname , ivar , thr )
    if ( or ( var < thr ) ) then
        k = find ( var < thr )
        k = k(1)
        errmsg = msprintf(gettext("%s: Expected that all entries of input argument %s at input #%d are greater or equal than %s, but entry #%d is equal to %s."),funname,varname,ivar,string(thr),k,string(var(k)));
        error(errmsg);
    end
endfunction

function optimbase_checkflint ( funname , var , varname , ivar )
    if ( or ( round(var)<>var ) ) then
        k = find ( round(var)<>var )
        k = k(1)
        errmsg = msprintf(gettext("%s: Expected floating point integer for input argument %s at input #%d, but entry #%d is equal to %s."),funname,varname,ivar,k,string(var(k)));
        error(errmsg);
    end
endfunction

function optimbase_checkscalar ( funname , var , varname , ivar )
    if ( or(size(var) <> [1 1]) ) then
        strcomp = strcat(string(size(var))," ")
        errmsg = msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), funname, ivar , 1, 1 );
        error(errmsg)
    end
endfunction

function optimbase_checkoption ( funname , var , varname , ivar , expectedopt )
    if ( and ( var <> expectedopt ) ) then
        stradd = """ or """
        strexp = """" + strcat(string(expectedopt),stradd) + """"
        errmsg = msprintf(gettext("%s: Expected value [%s] for input argument %s at input #%d, but got ""%s"" instead."),funname,strexp,varname,ivar,string(var));
        error(errmsg);
    end
endfunction
