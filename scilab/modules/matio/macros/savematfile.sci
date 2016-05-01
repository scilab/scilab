// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier: bug #13102 fixed
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// Copyright (C) ???? - INRIA - Serge STEER
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
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

    vars=who("get");
    // Verify that all inputs are character strings
    for k=1:size(varargin)
        if type(varargin(k))<>10 then
            error(gettext("All inputs must be character strings."));
        end
    end

    [lhs,rhs]=argn(0);

    mtlb_opts=[]; // Options for ASCII format
    mtlb_thefile=[]; // Name of file to write
    mtlb_names=[]; // Variable names to save
    version=[]; // MAT-file version: 4 or 6 or 7 (7.3 not yet implemented)
    bin=[]; // %T is binary file %F if ASCII file

    // Default format is binary
    if rhs==1 then
        bin=%T;
    end

    // Sort all inputs (Options/Names/Filename)
    k=1
    while k<=lstsize(varargin)
        // All options are converted to lower case
        if part(varargin(k),1)=="-" then
            varargin(k)=convstr(varargin(k));
        end

        select varargin(k)
        case "-append"
            warning(msprintf(gettext("Option %s not implemented: IGNORED."),"-append"));
            k=k+1
        case "-mat"
            bin=%T
            k=k+1
        case "-ascii"
            mtlb_opts=[mtlb_opts varargin(k)];
            bin=%F
            k=k+1
        case "-struct"
            k=k+1;
            stname=varargin(k);
            k=k+1;
            // Verify if one or more field name is/are given
            if k<=lstsize(varargin) & part(varargin(k),1)<>"-" & mtlb_thefile<>"" then // struct field
                while k<=lstsize(varargin) & part(varargin(k),1)<>"-"
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
            bin=%T;
            k=k+1
        case "-v6"
            version=6;
            bin=%T;
            k=k+1
        case "-v7"
            version=7;
            bin=%T;
            k=k+1
        case "-v7.3"
            version=7.3;
            bin=%T;
            k=k+1
        case "-tabs"
            bin=%F;
            mtlb_opts=[mtlb_opts varargin(k)];
            k=k+1
        case "-double"
            bin=%F;
            mtlb_opts=[mtlb_opts varargin(k)];
            k=k+1
        case "-regexp"
            warning(msprintf(gettext("Option %s not implemented: IGNORED."),"-regexp"));
            while k<=lstsize(varargin) & and(varargin(k)<>["-mat","-ascii"])
                k=k+1
            end
        else
            if isempty(mtlb_thefile) then // Filename
                mtlb_thefile=pathconvert(varargin(k),%f,%t);
                if fileparts(mtlb_thefile,"extension")==".mat" & isempty(bin) then // extension .mat and bin not already fixed by options
                    bin=%T
                end
            else // Variable names
                mtlb_names=[mtlb_names;varargin(k)]
            end
            k=k+1
        end
    end

    // Default version 7 for binary files
    if isempty(version) & bin then
        version=7;
        warning(gettext("Option -v7 added."));
    end

    // If no name given then all workspace saved
    if isempty(mtlb_names) then
        mtlb_names=vars;

        // Part to delete Scilab variables from mtlb_names (should be improved)
        predef_names = [predef("names");"savematfile";"varargin"];
        for i=1:size(predef_names, "*")
            mtlb_names(mtlb_names==predef_names(i))=[];
        end
        //mtlb_names(($-predef()+1):$)=[]; // clear predefined variables
    end

    // If binary format and no extension for filename, .mat is added
    if bin & isempty(strindex(mtlb_thefile,".")) then
        mtlb_thefile=mtlb_thefile+".mat"
    end

    // Do not handle function redefinition
    funcprot(0);

    // Binary save
    if bin then
        // LEVEL 4 MAT-file (This part comes from mtlb_save.sci)
        if version==4 then
            // Matlab 5 types are not saved (structs...)
            for k=size(mtlb_names,"*"):-1:1
                execstr("x="+mtlb_names(k))
                if and(type(x)<>[1 4 5 6 10]) then
                    warning(msprintf(gettext("Variable %s can not be saved in level 4 MAT-file: IGNORED."),mtlb_names(k)));
                    mtlb_names(k)=[]
                end
            end

            // Open file for writing
            [mtlb_fd,err]=mopen(mtlb_thefile,"wb",0)

            // Clear variable wich are no more used to avoid name conflicts
            for k=["varargin","mtlb_names","mtlb_fmt","mtlb_fd"]
                if or(mtlb_names==k) then
                    error(msprintf(gettext("Name conflict: it is not possible to save variable with name %s."),k));
                end
            end
            clear("x","k","rhs","lhs","kk","err","bin","version","mtlb_thefile","mtlb_opts");

            // Following 'for loop' from SS
            for mtlb_k=1:size(mtlb_names,"*")
                // perform changes on variables
                execstr("x="+mtlb_names(mtlb_k))
                it=0
                select type(x)
                case 1 then
                    P=0
                    T=0
                    if norm(imag(x),1)<>0 then it=1,end
                case 4 then
                    x=bool2s(x)
                    P=5
                    T=0
                case 5 then
                    if norm(imag(x),1)<>0 then it1=1,else it1=0,end
                    P=0
                    T=2
                    // We transpose the sparse matrix so as to ease the conversion to the matlab sparse format
                    [x,v,mn]=spget(x);
                    if it1==0 then
                        x=[x real(v);[mn 0]]
                    else
                        x=[x real(v) imag(v);[mn 0 0]]
                    end
                case 6 then
                    x=bool2s(x)
                    P=0
                    T=2
                    [x,v,mn]=spget(x);
                    x=[x v;[mn 0]]
                case 8 then
                    T=0
                    select inttype(x)
                    case 4 then P=2,
                    case 14 then P=2,
                    case 2 then P=3
                    case 12 then P=4
                    case 1 then P=5,
                    case 11 then P=5,
                    end
                    x=double(x)
                case 10 then
                    x1=part(x(:),1:max(length(x)))
                    x=[]
                    for l=1:size(x1,1)
                        x=[x;ascii(x1(l))]
                    end
                    P=5
                    T=1
                else
                    error(gettext("Attempt to write an unsupported data type to an ASCII file."));
                end
                [m,n]=size(x)


                M = 0 //little endian
                O = 0
                MOPT=[M O P T]

                [m,n]=size(x)

                head=[MOPT*[1000;100;10;1] m,n,it,length(mtlb_names(mtlb_k))+1]

                mput(head,"uil",mtlb_fd);
                mput([ascii(mtlb_names(mtlb_k)) 0],"c",mtlb_fd);
                select P
                case 0 then
                    flag="dl"
                case 1 then
                    flag="fl"
                case 2 then
                    flag="il"
                case 3 then
                    flag="sl"
                case 4 then
                    flag="usl"
                case 5 then
                    flag="uc"
                end
                if T==0 then
                    if x<>[] then
                        mput(real(x(:).'),flag,mtlb_fd);
                        if it==1
                            mput(imag(x(:).'),flag,mtlb_fd);
                        end
                    end
                elseif T==1
                    v=mput(x(:).',flag,mtlb_fd);
                elseif T==2 then  //sparse
                    mput(x(:).',flag,mtlb_fd);
                end
            end
            mclose(mtlb_fd);
            // End of loop written by SS
            // LEVEL 6 MAT-file
        elseif version==6 then
            // Open file for writing
            mtlb_fd=matfile_open(mtlb_thefile, "w");
            if mtlb_fd == -1 then
                error(msprintf(gettext("%s: Could not open file ''%s''.\n"),"savematfile",mtlb_thefile))
            end
            // Clear variable wich are no more used to avoid name conflicts
            for k=["varargin","mtlb_names","mtlb_fmt","mtlb_fd"]
                if or(mtlb_names==k) then
                    error(msprintf(gettext("Name conflict: it is not possible to save variable with name %s."),k))
                end
            end
            clear("x","k","rhs","lhs","kk","err","sep","bin","version","mtlb_thefile","mtlb_opts");

            // Write variables as miMATRIX data type
            for k=1:size(mtlb_names,"*")
                %var=evstr(mtlb_names(k));
                // We transpose the sparse matrix so as to ease the conversion to the matlab sparse format
                if type(%var)==5 then %var = %var'; end
                if and(type(%var)<>[9 11 13]) then
                    if ~matfile_varwrite(mtlb_fd, mtlb_names(k), %var, %F) then
                        error(msprintf(gettext("savematfile: could not save variable named %s.\n"), mtlb_names(k)));
                    end
                else
                    error(msprintf(gettext("savematfile: could not save variable named %s.\n"), mtlb_names(k)));
                end
            end

            matfile_close(mtlb_fd);
        elseif version==7
            // Open file for writing
            mtlb_fd=matfile_open(mtlb_thefile, "w");
            if mtlb_fd == -1 then
                error(msprintf(gettext("%s: Could not open file ''%s''.\n"),"savematfile",mtlb_thefile))
            end
            // Clear variable wich are no more used to avoid name conflicts
            for k=["varargin","mtlb_names","mtlb_fmt","mtlb_fd"]
                if or(mtlb_names==k) then
                    error(msprintf(gettext("Name conflict: it is not possible to save variable with name %s."),k))
                end
            end
            clear("x","k","rhs","lhs","kk","err","sep","bin","version","mtlb_thefile","mtlb_opts");

            // Write variables as miCOMPRESSED data type
            for k=1:size(mtlb_names,"*")
                %var=evstr(mtlb_names(k));
                // We transpose the sparse matrix so as to ease the conversion to the matlab sparse format
                if type(%var)==5 then %var = %var'; end
                if and(type(%var)<>[9 11 13]) then
                    if ~matfile_varwrite(mtlb_fd, mtlb_names(k), %var, %T) then
                        error(msprintf(gettext("savematfile: could not save variable named %s.\n"), mtlb_names(k)));
                    end
                else
                    error(msprintf(gettext("savematfile: could not save variable named %s.\n"), mtlb_names(k)));
                end
            end

            matfile_close(mtlb_fd);
        else // Version 7.3
            // Open file for writing
            mtlb_fd=matfile_open(mtlb_thefile, "w", "7.3"); // "7.3" specifies a Matlab 7.3 file
            if mtlb_fd == -1 then
                error(msprintf(gettext("%s: Could not open file ''%s''.\n"),"savematfile",mtlb_thefile))
            end
            // Clear variable wich are no more used to avoid name conflicts
            for k=["varargin","mtlb_names","mtlb_fmt","mtlb_fd"]
                if or(mtlb_names==k) then
                    error(msprintf(gettext("Name conflict: it is not possible to save variable with name %s."),k))
                end
            end
            clear("x","k","rhs","lhs","kk","err","sep","bin","version","mtlb_thefile","mtlb_opts");

            // Write variables as miCOMPRESSED data type
            for k=1:size(mtlb_names,"*")
                %var=evstr(mtlb_names(k));
                // We transpose the sparse matrix so as to ease the conversion to the matlab sparse format
                if type(%var)==5 then %var = %var'; end
                if and(type(%var)<>[9 11 13]) then
                    if ~matfile_varwrite(mtlb_fd, mtlb_names(k), %var, %T) then
                        error(msprintf(gettext("savematfile: could not save variable named %s.\n"), mtlb_names(k)));
                    end
                else
                    error(msprintf(gettext("savematfile: could not save variable named %s.\n"), mtlb_names(k)));
                end
            end

            matfile_close(mtlb_fd);
        end

        // ASCII save
    else
        // The end of this function has been adapted from mtlb_save.sci

        // Matlab 5 types are not saved (structs...)
        for k=size(mtlb_names,"*"):-1:1
            execstr("x="+mtlb_names(k))
            if and(type(x)<>[1 4 5 6 10]) then
                warning(msprintf(gettext("Variable %s can not be saved in ASCII file: IGNORED."),mtlb_names(k)));
                mtlb_names(k)=[]
            end
        end
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

        mtlb_fd=file("open",mtlb_thefile,"unknown")

        // Clear variable wich are no more used to avoid name conflicts
        for k=["varargin","mtlb_names","mtlb_fmt","mtlb_fd"]
            if or(mtlb_names==k) then
                error(msprintf(gettext("Name conflict: it is not possible to save variable with name %s."),k));
            end
        end
        clear("x","k","rhs","lhs","kk","err","sep","bin","version","mtlb_thefile","mtlb_opts");

        for mtlb_k=1:size(mtlb_names,"*")
            // perform changes on variables
            execstr("x="+mtlb_names(mtlb_k))
            select type(x)
            case 1 then
                write(mtlb_fd,real(x),"("+string(size(x,2))+mtlb_fmt+")")
            case 4 then
                write(mtlb_fd,bool2s(x),"("+string(size(x,2))+mtlb_fmt+")")
            case 5 then
                // We need to transpose to conform to the matlab sparse format
                [ij,x]=spget(real(x'));x=[ij x];
                write(mtlb_fd,real(x),"(2f8.0,1x"+string(size(x,2))+mtlb_fmt+")")
            case 6 then
                [ij,x]=spget(bool2s(x));x=[ij x];
                write(mtlb_fd,real(x),"(2f8.0,1x"+string(size(x,2))+mtlb_fmt+")")
            case 10 then
                x=part(x(:),1:max(length(x)))
                x1=[]
                for l=1:size(x,1)
                    x1=[x1;ascii(x(l))]
                end
                write(mtlb_fd,x1,"("+string(size(x1,2))+mtlb_fmt+")")
            end
        end
        file("close",mtlb_fd)
    end
endfunction
