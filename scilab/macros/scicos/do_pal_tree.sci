function scs_m = do_pal_tree(scicos_pal)
// Copyright INRIA

  scs_m = scicos_diagram(version=get_scicos_version());
  scs_m.props.title(1) = 'Palettes';
  sup = SUPER_f('define');

  for i=1:size(scicos_pal,1)
    o = sup;
    o.model.rpar = charge(scicos_pal(i,:)); //** local functions:
                                            //** see below in this file
    scs_m.objs(i) = o;
  end

  tt = pal_TreeView(scs_m); //** local functions: see below in this file

  cur_wd = getcwd();
  chdir(TMPDIR);
  mputl(tt,scs_m.props.title(1)+'.tcl');
  chdir(cur_wd)
  TCL_EvalFile(TMPDIR+'/'+scs_m.props.title(1)+'.tcl'); 
endfunction
//**-----------------------------------------------------------------------

function scs_m=charge(pal)
[ok,scs_m,cpr,edited]=do_load(pal(2),'palette')
if ok & size(scs_m.objs)>0 then
  scs_m.props.title(1)=pal(1)
else
  scs_m= scicos_diagram(version=get_scicos_version())
  scs_m.props.title(1)='error loading '+pal(1)
end
endfunction
//**------------------------------------------------------------------------

function tt = pal_TreeView(scs_m)

tt=[ "set BWpath [file dirname '"$env(SCIPATH)/tcl/BWidget-1.7.0'"] "
     "if {[lsearch $auto_path $BWpath]==-1} {"
     "    set auto_path [linsert $auto_path 0 $BWpath]"
     "}" 
     "package require BWidget 1.7.0"
     'namespace inscope :: package require BWidget'
     'set wxx .palettes'
     'proc pp {label} {global blko; set blko $label;ScilabEval '"Cmenu=''PlaceinDiagram'''"}'
     'proc qq {label} {global blko; set blko $label;ScilabEval '"Cmenu=''TkPopup'''"}'
     'catch {destroy $wxx}'
     'toplevel $wxx'
     'Tree $wxx.t -xscrollcommand {$wxx.xsb set} -yscrollcommand {$wxx.ysb set} "+...
        "-deltay 45 -deltax 30 -width 30 -bg white'
     'scrollbar $wxx.ysb -command {$wxx.t yview}'
     'scrollbar $wxx.xsb -command {$wxx.t xview} -orient horizontal'
     'grid $wxx.t $wxx.ysb -sticky nsew'
     ' grid $wxx.xsb -sticky ew'
     ' grid rowconfig    $wxx 0 -weight 1'
     ' grid columnconfig $wxx 0 -weight 1'
   ];

tt = [tt; 'wm title $wxx '+scs_m.props.title(1) ];
Pgif = SCI+"/macros/scicos/scicos_doc/man/gif_icons/";
GIFT = listfiles(Pgif+'*.gif');
GIF  = [];

for i=1:size(GIFT,1)
  [jxpath,Gname,jext] = splitfilepath_cos(GIFT(i));
  GIF = [GIF;Gname];
end

Path = 'root'
tt = crlist(scs_m, Path, tt);
tt = [tt;' $wxx.t bindImage <B1-Motion> {pp}']; 
tt = [tt;' $wxx.t bindText <B1-Motion> {pp}']; 
tt = [tt;' $wxx.t bindImage <Double-1> {pp}'];
tt = [tt;' $wxx.t bindText <Double-1> {pp}'];
tt = [tt;' $wxx.t bindImage <3> {qq}'];
endfunction
//**-----------------------------------------------------------------------------
 
//**================================================================
//TCL_EvalStr('$wxx.t opentree node1')
//   pa=TCL_GetVar('x');pa=part(pa,6:length(pa));
//   execstr('pa=list('+pa+')');       o=scs_m(scs_full_path(pa))
//**================================================================

function tt = crlist(scs_m,Path,tt)
  for i=1:size(scs_m.objs)
    o=scs_m.objs(i);
    path=Path+','+string(i)
    
    if typeof(o)=="Link" then
      titre2='link'
      tt=[tt;'$wxx.t insert end '+Path+' '+path+' -text '"'+titre2+''"']
    
    elseif typeof(o)=="Deleted" then
      titre2='Deleted'
      tt=[tt;'$wxx.t insert end '+Path+' '+path+' -text '"'+titre2+''"']
    
    else //** Blocks and Super Blocks :)
      
      if o.model.sim=='super'&(o.model.rpar.props.title(1)<>'Super Block') then
        //** Super Blocks  
	titre2 = o.model.rpar.props.title(1);
	tt = [tt;'$wxx.t insert end '+Path+' '+path+' -image [Bitmap::get folder] -text '"'+titre2+''"']
	tt = crlist(o.model.rpar,path,tt)
      else 
        //** Standard Blocks 
	titre2 = o.gui;
	if find(titre2==GIF)<>[] then
	   //** a valid icon (GIF) is found 
           tt = [tt;'$wxx.t insert end '+Path+' '+path+' -"+...
	        "helptext '"'+titre2+''" -image [image create photo -file '"'+Pgif+titre2+'.gif'"]']
	else
           //** NO icon is found: use the block's name   
	  tt = [tt;'$wxx.t insert end '+Path+' '+path+' -text '"'+titre2+''"']
	end
      
      end //**... Blocks ans Super Blocks  
      
    end //**.. object filter  

  end //**.. object loop 
  
endfunction

