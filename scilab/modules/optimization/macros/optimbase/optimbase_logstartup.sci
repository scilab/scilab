// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
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
// optimbase_logstartup --
//   Automatically startup logging.
//   If the logging is already started up, generates an error.
//   If the logging is not started up, if there is a log file configured,
//   open that log file to append messages.
//
function this = optimbase_logstartup ( this )
    if ( this.logstartup ) then
        error ( gettext ( "%s: Logging already started." , "optimbase_logstartup" ) )
    else
        this.logstartup = %t;
        if ( this.logfile <> "" ) then
            if ( this.logfilehandle <> 0 ) then
                error ( gettext ( "%s: Log file handle non zero while starting up the logging." , "optimbase_logstartup" ) )
            end
            this.logfilehandle = mopen( this.logfile , "a" );
            c = clock();
            year = c(1);
            month = c(2);
            day = c(3);
            hour = c(4);
            minute = c(5);
            seconds = c(6);
            mfprintf ( this.logfilehandle , "Optimbase ready for logging at %d-%d-%d %d:%d:%d\n" , ...
            year , month , day , hour , minute , seconds );
        end
    end
endfunction

