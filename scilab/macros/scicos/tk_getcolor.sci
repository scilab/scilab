function Color=tk_getcolor(title1,Color)
// Copyright INRIA

  title1=sci2tcl(title1)
  Color=Color-1;
  
txt=  'proc createmxn {frame r g b taille ind} {global maxrow n;set n"+...
      " {$taille / $maxrow};set n [expr $n];set nn {$taille % $maxrow};set n"+...
      "n [expr $nn];if ($nn>0) {set n [expr {$n+1}]};set m $maxrow;set c"+...
      " 0;for {set i 0} {$i< $m} {incr i} {for {set j 0} {$j<$n} {incr j"+...
      "} {if ($c>=$taille) {break};set ax [lindex $r $c];set ax [expr"+...
      " $ax];set bx [lindex $g $c];set bx [expr $bx];set dx [lindex $b $"+...
      "c];set dx [expr $dx];set bg [format '"#%02x%02x%02x'" $ax $bx"+...
      " $dx ];if ($c!=$ind) { button '"$frame.$i$j'" -padx 6 -pady 3"+...
      " -bg $bg  -borderwidth 4 -activebackground $bg -relief sunken -command [list titi $i $j]} else {button '"$frame.$i$j'"  -padx 6 -pady 3 -bg $bg -borderwidth 4 -activebackground $bg -relief raised -command [list titi $i $j]};grid '"$frame.$i$j'" -row $i -column $j -sticky news;bind '"$frame.$i$j'" <Enter> [list '"$frame.$i$j'" configure -relief raised];bind '"$frame.$i$j'" <Leave> [list '"$frame.$i$j'" configure -relief sunken];incr {c}};	grid columnconfigure $frame $i -weight 1;grid rowconfigure $frame $j -weight 1}};proc titi {g h} {global maxrow n;set x {$h + $n*$g +1};set x [expr $x];ScilabEval Color=''$x'';destroy .toto};'
  
col=get(gcf(),"color_map")  //xget('colormap')
ta=size(col,1);
  maxrow=ceil(sqrt(ta))
  maxcol=ceil(ta/maxrow)
  
  txt=txt+'catch {destroy .toto};toplevel .toto;wm title .toto '"'+title1+''";global maxrow ;set m"+...
      "axrow '+string(maxrow)+';frame  .toto.titi -width '+string(maxcol*400)+';';

  
  col=int(col*255);  
  col=string(col);
 ttr=[];ttg=[];ttb=[];
  for x=col'
    ttr=ttr+' '+x(1);
    ttg=ttg+' '+x(2);
    ttb=ttb+' '+x(3);
  end
  txt=txt+'createmxn .toto.titi [list '+ttr+'] [list '+ttg+'] [list"+...
      " '+ttb+'] '+string(ta)+' '+string(Color)+';pack .toto.titi;tkwait w"+...
      "indow .toto;'
  Color=[];
  TCL_EvalStr(txt)
  Color=evstr(Color)
endfunction



