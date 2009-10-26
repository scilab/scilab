// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
        error ( gettext ( "%s: Logging not started up, log file not empty but log file handle non zero." , "optimbase_logstartup" ) )
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

