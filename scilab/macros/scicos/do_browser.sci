function do_browser(scs_m)
// Copyright INRIA

  tt = scs_TreeView(scs_m);
  cur_wd = getcwd();
  chdir(TMPDIR);
  mputl(tt,scs_m.props.title(1)+'.tcl');
  chdir(cur_wd)
  TCL_EvalFile(TMPDIR+'/'+scs_m.props.title(1)+'.tcl')
endfunction

function tt = scs_TreeView(scs_m)
  x = [];
  y = 0 ;
tt=["set BWpath [file dirname '"$env(SCIPATH)/tcl/BWidget-1.7.0'"] "
    "if {[lsearch $auto_path $BWpath]==-1} {"
    "    set auto_path [linsert $auto_path 0 $BWpath]"
    "}" 
    "package require BWidget 1.7.0"
    'set wzz .scsTree'
    'proc ppsc {label} {global blkox; set blkox $label;ScilabEval '"Cmenu=''BrowseTo'''"}'
    'catch {destroy $wzz}'
    'toplevel $wzz'
    'Tree $wzz.t -xscrollcommand {$wzz.xsb set} -yscrollcommand {$wzz.ysb set} "+...
      " -width 50 -bg white'
    'scrollbar $wzz.ysb -command {$wzz.t yview}'
    'scrollbar $wzz.xsb -command {$wzz.t xview} -orient horizontal'
    'grid $wzz.t $wzz.ysb -sticky nsew'
    ' grid $wzz.xsb -sticky ew'
    ' grid rowconfig    $wzz 0 -weight 1'
    ' grid columnconfig $wzz 0 -weight 1'
   ];

tt = [tt;'wm title $wzz {Browser (double click to open diagram)}'];
Path = 'root'
tt = crlist1(scs_m,Path,tt);

tt = [tt;' $wzz.t bindText <Double-1> {ppsc}'];

endfunction
 
//TCL_EvalStr('$wzz.t opentree node1')
//   pa=TCL_GetVar('x');pa=part(pa,6:length(pa));
//   execstr('pa=list('+pa+')');       o=scs_m(scs_full_path(pa))


function tt = crlist1(scs_m,Path,tt)
  for i=1:size(scs_m.objs)
    o = scs_m.objs(i);
    
    if typeof(o)=="Block" then
      path = Path+','+string(i)
      
      if o.model.sim=='super' then
	titre2 = o.model.rpar.props.title(1);
	tt = [tt;'$wzz.t insert end '+Path+' '+path+' -text '"'+titre2+''"']

	tt = crlist1(o.model.rpar,path,tt); //** BEWARE: Recursive Call at the 
                                            //** very same function 
      end 
    
    end //** Blocks and Super Blocks filter 
  
   end //**..  loop on objects 
endfunction

