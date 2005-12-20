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
  TCL_SetVar("tkc_comment",sci2exp(ListArg(2),0));
  TCL_SetVar("text_button","Cancel");
  TCL_EvalFile(SCI+"/tcl/ged/tk_choose.tcl")
end

if rhs == 3
  TCL_SetVar("tkc_thelist",sci2exp(ListArg(1),0));
  TCL_SetVar("tkc_comment",sci2exp(ListArg(2),0));
  TCL_SetVar("text_button",sci2exp(ListArg(3),0));
  TCL_EvalFile(SCI+"/tcl/ged/tk_choose.tcl")
end

while tk_choose_num == -1
  // wait for response
  sleep(1);
end

num = tk_choose_num;

clear tk_choose_num;
TCL_UnsetVar("tkc_thelist");
TCL_UnsetVar("tkc_comment");
TCL_UnsetVar("text_button");

TCL_EvalStr("destroy .tk_choose_menu")

endfunction
