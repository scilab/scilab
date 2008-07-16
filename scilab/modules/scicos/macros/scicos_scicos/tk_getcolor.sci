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

//** How does it work?
// Create a TCL/TK dialog to display a grid containing all colors from current colormap
//  - Colormap is obtained with either get(gcf(),"color_map") or xget('colormap')
//  - Colors are expressed using values ranging from 0 to 1 for each RGB component
//  - Components are converted from 0..1 to 0..255 for what seems to be display
// purposes only
//  - Output argument (Color) is the index of the clicked cell, i.e the index of
// the chosen color in the colormap

function Color=tk_getcolor(title1,Color)

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
  
col=get(gcf(),"color_map")  //** SC5: xget('colormap')
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

