// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// @OBSOLETE

function [p] = tk_getfile(file_mask,path,Title,multip)

warnobsolete("uigetfile", "5.2");

if exists("file_mask","local")==1 then
  filemask = file_mask;
else
  filemask = "*";
end;

initialdir = pwd();
if exists("path","local")==1 then
  initialdir = pathconvert(path,%f,%t)
  initialdir = strsubst(initialdir,"\","/")
else
  global("%tk_getfile_defaultpath")
  if typeof(%tk_getfile_defaultpath)=="string" then
    initialdir = %tk_getfile_defaultpath;
  end
end;

thetitle = gettext("Open");
if exists("title","local")==1 then
  thetitle = title
elseif exists("Title","local")==1 then
  thetitle = Title
end;

if ~exists("multip","local")==1 then
  multip = %F
end;
if multip~="1" then
  multip = %F
else
  multip = %T
end;

p = uigetfile(filemask, initialdir, thetitle, multip)';

if MSDOS then
  if ~p=="" then
    global("%tk_getfile_defaultpath");
    %tk_getfile_defaultpath = dirname(p(1));
    %tk_getfile_defaultpath=strsubst(%tk_getfile_defaultpath,"\","/")
  end;
end;

endfunction

