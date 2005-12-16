function num = tk_choose(varargin)

[lhs,rhs]=argn(0);

ListArg = varargin;

global tk_choose_num;

tk_choose_num = -1;

if rhs == 1
  TCL_SetVar("tkc_thelist",sci2exp(ListArg(1),0));
  TCL_SetVar("tkc_comment","");
  TCL_SetVar("text_button","Cancel");
  TCL_EvalFile(SCI+"/tcl/ged/tk_choose.tcl")
end


if rhs == 2
  TCL_SetVar("tkc_thelist",sci2exp(ListArg(1),0));
  TCL_SetVar("tkc_comment",sci2exp(ListArg(2)));
  TCL_SetVar("text_button","Cancel");
  TCL_EvalFile(SCI+"/tcl/ged/tk_choose.tcl")
end

if rhs == 3
  TCL_SetVar("tkc_thelist",sci2exp(ListArg(1),0));
  TCL_SetVar("tkc_comment",sci2exp(ListArg(2)));
  TCL_SetVar("text_button",,sci2exp(ListArg(3)));
  TCL_EvalFile(SCI+"/tcl/ged/tk_choose.tcl")
end

while tk_choose_num == -1
  // wait for response
end

num = tk_choose_num;

TCL_EvalStr("destroy .tk_choose_menu")

endfunction
