// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
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
// nmplot_search --
//   Search the minimum with Nelder-Mead algorithm.
//
function this = nmplot_search (this)
    nmplot_log ( this , "nmplot_search")
    this = nmplot_startupfiles ( this )
    this.nmbase = neldermead_configure ( this.nmbase , "-outputcommand" , list(nmplot_outputcmd,this) );
    this.nmbase = neldermead_search ( this.nmbase )
    this = nmplot_shutdownfiles ( this )
endfunction


//
// nmplot_header --
//   Returns a header for the output files
// Arguments
//    filename : the target file name
//    name : the type of data in the file
//
function report = nmplot_header ( this , filename , name )
    nmplot_log ( this , "nmplot_header")
    report = list()
    report($+1) = sprintf("//\n")
    report($+1) = sprintf("// %s--\n" , filename)
    report($+1) = sprintf("//   History of the %s during Nelder-Mead algorithm.\n",name)
    report($+1) = sprintf("//\n")
    report($+1) = sprintf("// Copyright (C) 2008-2009 - INRIA - Michael Baudin\n")
    report($+1) = sprintf("//\n")
endfunction

//
// nmplot_startupfiles --
//   Opens the output files to prepare the algorithm
//   execution.
// Arguments
//   <no arg>
//
function this = nmplot_startupfiles ( this )
    nmplot_log ( this , "nmplot_startupfiles")
    if this.simplexfn <> "" then
        this.simplexhandle  = mopen ( this.simplexfn , "w" )
        header = nmplot_header ( this , this.simplexfn , "simplex" )
        fd = this.simplexhandle
        for i = 1:length(header)
            mfprintf(fd,"%s\n",header(i))
        end
        mfprintf(fd,"history = list()\n")
    end
    if this.fbarfn <> "" then
        this.fbarhandle  = mopen ( this.fbarfn , "w" )
        header = nmplot_header ( this , this.fbarfn , "function value average" )
        fd = this.fbarhandle
        for i = 1:length(header)
            mfprintf(fd,"%s\n",header(i))
        end
        mfprintf(fd,"history = [\n")
    end
    if this.foptfn <> "" then
        this.fopthandle  = mopen ( this.foptfn , "w" )
        header = nmplot_header ( this , this.foptfn , "function value optimum" )
        fd = this.fopthandle
        for i = 1:length(header)
            mfprintf(fd,"%s\n",header(i))
        end
        mfprintf(fd,"history = [\n")
    end
    if this.sigmafn <> "" then
        this.sigmahandle  = mopen ( this.sigmafn , "w" )
        header = nmplot_header ( this , this.sigmafn , "maximum of oriented length" )
        fd = this.sigmahandle
        for i = 1:length(header)
            mfprintf(fd,"%s\n",header(i))
        end
        mfprintf(fd,"history = [\n")
    end
endfunction

//
// nmplot_shutdownfiles --
//   Close the output files to shutdown the algorithm
//   execution.
// Arguments
//   <no arg>
//
function this = nmplot_shutdownfiles ( this )
    nmplot_log ( this , "nmplot_shutdownfiles")
    if this.simplexfn <> "" then
        fd  = this.simplexhandle
        mclose ( fd )
        nmplot_log ( this , sprintf( "Exported Simplex history in %s",this.simplexfn))
    end
    if this.fbarfn <> "" then
        fd = this.fbarhandle
        mfprintf ( fd , "]\n" )
        mclose ( fd )
        nmplot_log ( this , sprintf( "Exported fbar history in %s",this.fbarfn))
    end
    if this.foptfn <> "" then
        fd = this.fopthandle
        mfprintf ( fd , "]\n" )
        mclose ( fd )
        nmplot_log ( this , sprintf( "Exported fopt history in %s",this.foptfn))
    end
    if this.sigmafn <> "" then
        fd = this.sigmahandle
        mfprintf ( fd , "]\n" )
        mclose ( fd )
        nmplot_log ( this , sprintf( "Exported sigma history in %s",this.sigmafn))
    end
endfunction

