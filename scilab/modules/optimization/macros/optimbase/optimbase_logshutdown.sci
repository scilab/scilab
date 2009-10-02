// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimbase_logshutdown --
//   Shut down the logging.
//   If the logging is already started up, generates an error.
//   If the loggin is started up, if there is a log file, close it.
//
function this = optimbase_logshutdown ( this )
  if ~this.logstartup then
    error ( gettext ( "%s: Logging not started." , "optimbase_logstartup" ) )
  else
    this.logstartup = %f;
    if ( this.logfile <> "" ) then
      mclose( this.logfilehandle );
    end
  end
endfunction

