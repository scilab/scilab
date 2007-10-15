function scs_m = do_scsm_tree(scs_m)
// Copyright INRIA
  tt=scs_m_TreeView(scs_m);
  cur_wd = getcwd();
  chdir(TMPDIR);
  mputl(tt,scs_m.props.title(1)+'.tcl');
  chdir(cur_wd)
  TCL_EvalFile(TMPDIR+'/'+scs_m.props.title(1)+'.tcl')
endfunction

function tt=scs_m_TreeView(scs_m)
  x=[];y=0;
tt=["set BWpath [file dirname '"$env(SCIPATH)/tcl/BWidget-1.7.0'"] "
    "if {[lsearch $auto_path $BWpath]==-1} {"
    "    set auto_path [linsert $auto_path 0 $BWpath]"
    "}" 
    "package require BWidget 1.7.0"
    'set wyy .palettes'
    'proc ppxs {label} {global blko; set blko $label;ScilabEval '"Cmenu=''PlaceinDiagram'''"}'
    'proc qqxs {label} {global blko; set blko $label;ScilabEval '"Cmenu=''TkPopup'''"}'
    'catch {destroy $wyy}'
    'toplevel $wyy'
    'Tree $wyy.t -xscrollcommand {$wyy.xsb set} -yscrollcommand {$wyy.ysb set} "+...
    "-deltax 10 -deltay 10 -width 30 -bg white'
    'scrollbar $wyy.ysb -command {$wyy.t yview}'
    'scrollbar $wyy.xsb -command {$wyy.t xview} -orient horizontal'
    'grid $wyy.t $wyy.ysb -sticky nsew'
    ' grid $wyy.xsb -sticky ew'
    ' grid rowconfig    $wyy 0 -weight 1'
    ' grid columnconfig $wyy 0 -weight 1'
   ];

tt=[tt;'wm title $wyy '"Navigator:'" '+scs_m.props.title(1)];

Path='root'
tt=crlist2(scs_m,Path,tt);

tt=[tt;' $wyy.t bindText <Double-1> {ppxs}'];
tt=[tt;' $wyy.t bindText <3> {qqxs}'];
endfunction
 
//TCL_EvalStr('$wyy.t opentree node1')
//   pa=TCL_GetVar('x');pa=part(pa,6:length(pa));
//   execstr('pa=list('+pa+')');       o=scs_m(scs_full_path(pa))


function tt=crlist2(scs_m,Path,tt)
  for i=1:size(scs_m.objs)
    o=scs_m.objs(i);
    path=Path+','+string(i)
    if typeof(o)=='Link' then
      titre2='link'
      tt=[tt;'$wyy.t insert end '+Path+' '+path+' -text '"'+titre2+''"']
    elseif typeof(o)=='Deleted' then
      titre2='Deleted'
      tt=[tt;'$wyy.t insert end '+Path+' '+path+' -text '"'+titre2+''"']
    else
      if o.model.sim=='super' then
	titre2=o.model.rpar.props.title(1);
	tt=[tt;'$wyy.t insert end '+Path+' '+path+' -image [Bitmap::get folder] -text '"'+titre2+''"']
	tt=crlist2(o.model.rpar,path,tt)
      else
	titre2=o.gui;
	  tt=[tt;'$wyy.t insert end '+Path+' '+path+' -text '"'+titre2+''"']
      end
    end
  end
  
endfunction

