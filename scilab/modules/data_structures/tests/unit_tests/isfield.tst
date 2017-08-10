// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2011 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for isfield function
// =============================================================================

my_struct = struct("field_1",123,"field_2",456);

// syntax

ierr = execstr("isfield()","errcatch");
if ierr == 0 then pause,end
if lasterror() <> msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"isfield",2) then pause, end

ierr = execstr("isfield(my_struct)","errcatch");
if ierr == 0 then pause,end
if lasterror() <> msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"isfield",2) then pause, end

ierr = execstr("isfield(my_struct,''field_1'',''field_2'')","errcatch");
if ierr == 0 then pause,end

ierr = execstr("isfield(my_struct,2)","errcatch");
if ierr == 0 then pause,end
if lasterror() <> msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"isfield",2) then pause, end

my_struct = ["field_1","field_2"];

ierr = execstr("isfield(my_struct,''field_1'')","errcatch");
if ierr == 0 then pause,end
if lasterror() <> msprintf(gettext("%s: Wrong type for input argument #%d: struct array or tlist or mlist expected.\n"),"isfield",1) then pause, end

// Fonctionnality

my_struct = struct("field_1",123,"field_2",456,"field_4",789);

if ~ isfield(my_struct,"field_1") then pause, end
if ~ isfield(my_struct,"field_2") then pause, end
if   isfield(my_struct,"field_3") then pause, end
if ~ isfield(my_struct,"field_4") then pause, end

fields = [ "field_1" "field_2" ; "field_3" "field_4" ; "field_3" "field_4" ];
ref    = [    %T        %T     ;    %F        %T     ;    %F        %T     ];
if or( isfield( my_struct , fields ) <> ref ) then pause, end


fields = [ "field_1" "field_2" "field_3" "field_4" "field_3" "field_4" ];
ref    = [    %T        %T        %F        %T        %F        %T     ];
if or( isfield( my_struct , fields ) <> ref ) then pause, end


fields = [ "field_1" ; "field_2" ; "field_3" ; "field_4" ; "field_3" ; "field_4" ];
ref    = [    %T     ;    %T     ;    %F     ;    %T     ;    %F     ;    %T     ];
if or( isfield( my_struct , fields ) <> ref ) then pause, end


fields = [ "field_1" "field_2" "field_3" ; "field_4" "field_3" "field_4" ];
ref    = [    %T        %T        %F     ;    %T        %F        %T     ];
if or( isfield( my_struct , fields ) <> ref ) then pause, end


s = tlist(["test" "t" "n","","p"], "Hello", %pi, "anonymous", (1-%z)^3);
a=isfield(s, ["b" "t" ""]);
assert_checkequal(a, [%f %t %t]);

