// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimbase_outputcmd --
//   Calls back user's output command
// Arguments
//   this : the current object
//   state : the state of the algorithm,
//     "init", "done", "iter"
//   data : the data to pass to the client output command
//
function  optimbase_outputcmd ( this , ...
   state , data )
  if this.outputcommand <> "" then
    if typeof(this.outputcommandarg)=="string" then
      this.outputcommand ( state , data )
    else
      this.outputcommand ( state , data , this.outputcommandarg )
    end
  end
endfunction

