// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier: bug #13102 fixed
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// Copyright (C) ???? - INRIA - Serge STEER
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function savematfile(varargin)
    // Save variables in a Matlab binary or ASCII file into Scilab
    // This function has been developed following the 'MAT-File Format' description:
    // www.mathworks.com/access/helpdesk/help/pdf_doc/matlab/matfile_format.pdf

    // INITIALIZATIONS
    // ===============
    ?vars? = who_user(%f); // On first line => ignores variables inner to savematfile()

    [lhs,rhs] = argn(0);

    mtlb_opts = [];     // Options for ASCII format
    mtlb_thefile = [];  // Name of file to write
    mtlb_names = [];    // Variables names to save
    version = [];       // MAT-file version: 4 or 6 or 7 or 7.3
    bin = %T;           // %T is binary file %F if ASCII file

    // ==================================
    // CHECKING INPUTS & SETTING DEFAULTS
    // ==================================
    // Verify that all inputs are character strings
    for k = 1:size(varargin)
        if type(varargin(k))<>10 then
            msg = _("%s: Argument #%d: String expected.\n")
            error(msprintf(msg, "savematfile", k));
        end
    end

    // Sort all inputs (Options/Names/Filename)
    k = 1
    while k <= size(varargin)
        // All options are converted to lower case
        if part(varargin(k),1)=="-" then
            varargin(k)=convstr(varargin(k));
        end

        select varargin(k)
        case "-append"
            warning(msprintf(gettext("Option %s not implemented: IGNORED."),"-append"));
            k=k+1
        case "-mat"
            k=k+1
        case "-ascii"
            mtlb_opts=[mtlb_opts varargin(k)];
            bin = %F
            k=k+1
        case "-struct"
            k=k+1;
            stname=varargin(k);
            k=k+1;
            // Verify if one or more field name is/are given
            if k <= size(varargin) & part(varargin(k),1)<>"-" & mtlb_thefile<>"" then // struct field
                while k <= size(varargin) & part(varargin(k),1)<>"-"
                    // Add field to variable names
                    mtlb_names=[mtlb_names;varargin(k)];
                    execstr(varargin(k)+"="+stname+"(mtlb_names($))");
                    k=k+1;
                end
            else // All vars(1)=[];fields have to be saved
                fields=getfield(1,evstr(stname));
                fields(1:2)=[]
                for kk=fields
                    mtlb_names=[mtlb_names;kk];
                    execstr(kk+"="+stname+"(mtlb_names($))");
                end
            end
        case "-v4"
            version=4;
            k=k+1
        case "-v6"
            version=6;
            k=k+1
        case "-v7"
            version=7;
            k=k+1
        case "-v7.3"
            version=7.3;
            k=k+1
        case "-tabs"
            bin = %F;
            mtlb_opts=[mtlb_opts varargin(k)];
            k=k+1
        case "-double"
            bin = %F;
            mtlb_opts=[mtlb_opts varargin(k)];
            k=k+1
        case "-regexp"
            warning(msprintf(gettext("Option %s not implemented: IGNORED."),"-regexp"));
            while k <= size(varargin) & and(varargin(k)<>["-mat","-ascii"])
                k=k+1
            end
        else
            if isempty(mtlb_thefile) then // Filename
                mtlb_thefile = pathconvert(varargin(k),%f,%t);
            else // Variable names
                mtlb_names=[mtlb_names;varargin(k)]
            end
            k=k+1
        end
    end

    // Default version 7 for binary files
    if isempty(version) & bin then
        version = 7;
        warning(gettext("Option -v7 added."));
    end

    // If no name given then all user workspace saved
    if isempty(mtlb_names) then
        mtlb_names = ?vars?;

        // Part to delete Scilab variables from mtlb_names (should be improved)
        predef_names = [predef("names");"savematfile";"varargin"];
        for k=1:size(predef_names, "*")
            mtlb_names(mtlb_names==predef_names(k))=[];
        end
        // polynomials, graphic handles, macros, builtin, libraries, lists.. are ignored:
        for k = size(mtlb_names,"*"):-1:1
             execstr("x="+mtlb_names(k));
             if and(type(x)<>[1 4 5 6 7 8 10 17])
                 mtlb_names(k) = []
             end
         end
    end
    clear ?vars?

    // Clear variable which are no more used to avoid name conflicts
    for k = ["varargin","mtlb_names","mtlb_fmt","mtlb_fd"]
        if or(mtlb_names==k) then
            msg = gettext("%s: Name conflict: it is not possible to save variable with name ""%s"" (ignored).")
            warning(msprintf(msg, "savematfile", k));
        end
    end

    // If binary format and no extension for filename, .mat is added
    if bin & isempty(strindex(mtlb_thefile,".")) then
        mtlb_thefile=mtlb_thefile+".mat"
    end

    // Do not handle function redefinition
    funcprot(0);

    // ------------------------------------------------------------------------

    // ===========
    // BINARY SAVE
    // ===========
    if bin then
        // LEVEL 4 MAT-file (This part comes from mtlb_save.sci)
        // -----------------------------------------------------
        if version==4 then
            // Matlab 5 types are not saved (structs...)
            mtlb_names = checkTypeBeforeMatSave(mtlb_names, [1 4 5 6 8 10], "4")

            // Open file for writing
            [mtlb_fd,err] = mopen(mtlb_thefile,"wb",0)

            // Clear variable wich are no more used to avoid name conflicts
            clear k x rhs lhs kk err bin version mtlb_thefile mtlb_opts

            // Following 'for loop' from SS
            for mtlb_k = 1:size(mtlb_names,"*")
                // perform changes on variables
                execstr("__x__="+mtlb_names(mtlb_k))

                // hypermatrix => we concatenate its pages horizontally:
                __s__ = size(__x__);
                if length(__s__)>2    
                    __x__ = matrix(__x__,__s__(1),-1);
                end
                //
                __it__ = 0  // == has imaginary part
                __P__ = 0   // v4 encoding type
                __T__ = 0
                select type(__x__)
                case 1 then
                    if norm(imag(__x__),1)<>0
                        __it__ = 1
                    end
                case 4 then     // boolean
                    __x__ = bool2s(__x__)
                    __P__ = 5
                case 5 then     // sparse
                    if norm(imag(__x__),1)<>0 then
                        __it1__ = 1
                    else
                        __it1__ = 0
                    end
                    __T__ = 2
                    // We transpose the sparse matrix so as to ease the conversion to the matlab sparse format
                    [__x__, __v__, __mn__] = spget(__x__);
                    if __it1__==0 then
                        __x__ = [__x__ real(__v__);[__mn__ 0]]
                    else
                        __x__ = [__x__ real(__v__) imag(__v__);[__mn__ 0 0]]
                    end
                case 6 then     // sparse boolean
                    __x__ = bool2s(__x__)
                    __P__ = 0
                    __T__ = 2
                    [__x__, __v__, __mn__] = spget(__x__);
                    __x__ = [__x__ __v__ ; [__mn__ 0]]
                case 8 then
                    // Supported Matlab v4 encodings:
                    // 0 double
                    // 2 32-bit signed integers
                    // 3 16-bit signed integers
                    // 4 16-bit unsigned integers
                    // 5 8-bit unsigned integers
                    select inttype(__x__)   
                    case 8  then    // int64  = l
                        __P__ = 0
                        __x__ = double(__x__);
                    case 18  then   // uint64  = ul
                        __P__ = 0
                        __x__ = double(__x__);
                    case 4  then __P__ = 2, // int32  = i
                    case 14 then            // uint32 = ui
                        if find(__x__>int32(%inf))==[]
                            __P__ = 2
                        else
                            __P__ = 0
                            __x__ = double(__x__);
                        end
                    case 2  then __P__ = 3  // int16  = s
                    case 12 then __P__ = 4  // uint16 = us
                    case 1  then    // int8
                        if find(__x__<0,1)==[]
                            __P__ = 5,      // uc
                        else
                            __x__ = int16(__x__);
                            __P__ = 3,      // s
                        end
                    case 11 then __P__ = 5, // uint8 = uc
                    end

                case 10 then    // Text
                    __x1__ = part(__x__(:), 1:max(length(__x__)))
                    __x__ = []
                    for l = 1:size(__x1__,1)
                        __x__ = [__x__; ascii(__x1__(l))]
                    end
                    __P__ = 5
                    __T__ = 1
                else
                    // Should never happen:
                    mclose(mtlb_fd);
                    error(gettext("Attempt to write an unsupported data type to a binary file."));
                end

                //          v.....little endian forced
                __MOPT__ = [0  0 __P__ __T__]

                __head__ = [__MOPT__*[1000;100;10;1] size(__x__)(1:2), __it__, length(mtlb_names(mtlb_k))+1]

                mput(__head__,"uil",mtlb_fd);
                mput([ascii(mtlb_names(mtlb_k)) 0],"uc", mtlb_fd);
                __flag__ = ["dl" "fl" "il" "sl" "usl" "uc"]
                __flag__ = __flag__(__P__+1);
                if __T__==0 then
                    if __x__<>[] then
                        if type(__x__)<>8
                            mput(real(__x__(:).'), __flag__, mtlb_fd);
                            if __it__==1
                                mput(imag(__x__(:).'), __flag__, mtlb_fd);
                            end
                        else
                            mput(__x__(:).', __flag__, mtlb_fd);
                        end
                    end
                elseif __T__==1         // Text
                    mput(__x__(:).', __flag__, mtlb_fd);
                elseif __T__==2         // sparse and sparse boolean
                    mput(__x__(:).', __flag__, mtlb_fd);
                end
            end
            mclose(mtlb_fd);

        // Versions 6, 7 and 7.3
        // ---------------------
        else
            // Filtering supported types
            // Unsupported : handles 9, macros 13, primitives 130, Others 128, 129
            // Unsupported : booleans 4 : http://bugzilla.scilab.org/15568
            mtlb_names = checkTypeBeforeMatSave(mtlb_names, [1 5 7 8 10 17], version)
            if mtlb_names==[]
                msg = gettext("savematfile: No variable to save => No file written.")
                warning(msg);
                return
            end

            // Open file for writing
            if version < 7.3
                mtlb_fd = matfile_open(mtlb_thefile, "w");
            else
                mtlb_fd = matfile_open(mtlb_thefile, "w", "7.3");
            end
            if mtlb_fd == -1 then
                msg = gettext("%s: Could not open file ''%s''.\n")
                error(msprintf(msg, "savematfile", mtlb_thefile))
            end
            ?compression? = version>6
            // Clear variable which are no more used to avoid name conflicts
            clear k x rhs lhs kk err bin version mtlb_thefile mtlb_opts

            // Write variables as miMATRIX data type
            for ?k? = 1:size(mtlb_names,"*")
                ?var? = evstr(mtlb_names(?k?));
                // We transpose the sparse matrix so as to ease the conversion
                //  to the matlab sparse format
                if type(?var?)==5 then
                    ?var? = ?var?'
                end
                if ~matfile_varwrite(mtlb_fd, mtlb_names(?k?), ?var?, ?compression?) then
                    msg = gettext("savematfile: could not write variable ""%s"".\n")
                    warning(msprintf(msg, mtlb_names(?k?)));
                end
            end
            matfile_close(mtlb_fd);
        end

        // --------------------------------------------------------------------
        // ==========
        // ASCII save
        // ==========
    else
        // The end of this function has been adapted from mtlb_save.sci

        // Matlab 5 types are not saved (structs...)
        mtlb_names = checkTypeBeforeMatSave(mtlb_names, [1 4 5 6 10], "ASCII")
        if ( (mtlb_opts <> []) & (strindex("-tabs",mtlb_opts)<>[]) ) then
            sep = ascii(9);
        else
            sep=" "
        end
        if size(mtlb_opts,"*")==1 then //8 digits save
            mtlb_fmt="(2x,1pe14.7"+sep+")"
        else
            mtlb_fmt="(2x,1pe23.15"+sep+")"
        end

        mtlb_fd = file("open", getshortpathname(mtlb_thefile), "unknown")

        // Clear variable wich are no more used to avoid name conflicts
        clear i k x rhs lhs kk err sep bin version mtlb_thefile mtlb_opts

        for mtlb_k=1:size(mtlb_names,"*")
            // perform changes on variables
            execstr("__x__="+mtlb_names(mtlb_k))
            select type(__x__)
            case 1 then
                write(mtlb_fd,real(__x__),"("+string(size(__x__,2))+mtlb_fmt+")")
            case 4 then
                write(mtlb_fd,bool2s(__x__),"("+string(size(__x__,2))+mtlb_fmt+")")
            case 5 then
                // We need to transpose to conform to the matlab sparse format
                [__ij__, __x__] = spget(real(__x__'));
                __x__ = [__ij__ __x__];
                write(mtlb_fd, real(__x__), "(2f8.0,1x"+string(size(__x__,2))+mtlb_fmt+")")
            case 6 then
                [__ij__, __x__] = spget(bool2s(__x__));
                __x__ = [__ij__ __x__];
                write(mtlb_fd, real(__x__), "(2f8.0,1x"+string(size(__x__,2))+mtlb_fmt+")")
            case 10 then
                __x__ = part(__x__(:),1:max(length(__x__)))
                __x1__ = []
                for l = 1:size(__x__,1)
                    __x1__ = [__x1__; ascii(__x__(l))] // will fail with UTF-8 (variable length)
                end
                write(mtlb_fd, __x1__, "("+string(size(__x1__,2))+mtlb_fmt+")")
            end
        end
        file("close",mtlb_fd)
    end
endfunction

// ----------------------------------------------------------------------------

function ?names? = checkTypeBeforeMatSave(?names?, ?typesOK?, ?version?)
    for ?k? = size(?names?,"*"):-1:1
        execstr("?x?="+?names?(?k?))
        if and(type(?x?)<>?typesOK?) then
            ?msg? = gettext("%s: Variable ""%s"" can not be saved in MAT-file version %s (type %d unsupported): IGNORED.\n")
            warning(msprintf(?msg?, "savematfile", ?names?(?k?), string(?version?), type(?x?)));
            ?names?(?k?) = []
        end
    end
endfunction
