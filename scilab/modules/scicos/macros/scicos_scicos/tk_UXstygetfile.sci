//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function [p] = tk_UXstygetfile(file_mask,path,Title,multip)
if ~with_tk() then
  error("Tcl/Tk interface not defined"),
end;
arg = ""
if exists("file_mask","local")==1 then
  TCL_SetVar("ftypes","{""Requested Extensions"" {"+file_mask+"} }")
  arg = arg+" -filetypes $ftypes"
end;
if exists("path","local")==1 then
  path = pathconvert(path,%f,%t)
  path=strsubst(path,"\","/")
  arg = arg+" -initialdir {"+path+"}"
else
  if MSDOS then
    global("%tk_getfile_defaultpath")
    if exists("%tk_getfile_defaultpath","global")==1 then
      arg = arg+" -initialdir {"+%tk_getfile_defaultpath+"}"
    end;
  end;
end;
if exists("title","local")==1 then
  Title=title
  arg = arg+" -title {"+Title+"}",
elseif exists("Title","local")==1 then
  arg = arg+" -title {"+Title+"}",
end;
if ~exists("multip","local")==1 then
  multip = "0"
end;
if multip~="1" then
  multip = "0"
end;
arg = arg+" -multiple "+multip,
//TCL_EvalStr("set scifilepath [tk_getOpenFile"+arg+"]")
TCL_EvalStr("set scifilepath [::tk::dialog::file:: open "+arg+"]")
// the output of tk_getOpenFile with -multiple 1 and -multiple 0 is not
// the same wrt to escaping special chars such as spaces - therefore two cases
if multip=="1" then
  // since TCL_GetVar does not handle lists,
  // let TCL parse the list output from tk_getOpenFile
  TCL_EvalStr("array set sfpa {};set sfpai 1;foreach sfpae $scifilepath {set sfpa($sfpai,1) $sfpae;incr sfpai};if {$scifilepath==""""} {array unset sfpa;set sfpa """"}")
  // and get back a column matrix of string
  p = TCL_GetVar("sfpa")
  TCL_EvalStr("unset -nocomplain sfpa")
else
  // -multiple 0 case, just recover the string output from tk_getOpenFile
  p = TCL_GetVar("scifilepath")
end;
if MSDOS then
  if ~p=="" then
    global("%tk_getfile_defaultpath");
    %tk_getfile_defaultpath = dirname(p(1));
    %tk_getfile_defaultpath=strsubst(%tk_getfile_defaultpath,"\","/")
  end;
end;

endfunction

