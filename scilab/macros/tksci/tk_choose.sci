function _result=tk_choose(_items,_title,_button)

// Author : S. Mottelet, Tue Jan 24 12:01:25 CET 2006
// Use of a namespace makes things easier (no global variables to pass
// arguments from the Scilab level).

global LANGUAGE

if argn(2)<=1
    _title="";
end
if argn(2)<=2
  _button="Cancel"
  if LANGUAGE=="fr"
    _button="Annuler"
  end
end

if with_gtk()
    _result=x_choose(_items,_title,_button);
    return
end

// Create the namespace

TCL_EvalStr("namespace eval tkChoose {}")

// Set parameter arrays in the Tcl world

TCL_SetVar("tkChoose::title",_title(:));
TCL_SetVar("tkChoose::items",_items(:));
TCL_SetVar("tkChoose::button",_button(:));

// Finally launch the Tcl script

TCL_EvalFile(SCI+"/tcl/utils/tk_choose.tcl")

// Event loop at the Scilab level

while ~TCL_ExistVar("tkChoose::result")
  // wait for response
  sleep(1);
end

_result=evstr(TCL_GetVar("tkChoose::result"))+1;

// Destroy the toplevel tkChoose widget

TCL_EvalStr("catch {destroy $tkChoose::t}")

// Yank the namespace (all variables therein are also
// destroyed)

TCL_EvalStr("namespace delete tkChoose")

endfunction

