function block=tkscaleblk(block,flag)
// Copyright INRIA
  blknb=string(curblock())
  if flag==4 then
    cur=%cpr.corinv(curblock())
    if size(cur,'*')==1 then // open widget only if the block 
                             // is in main Scicos editor window
      o=scs_m.objs(cur).graphics.orig;
      sz=scs_m.objs(cur).graphics.sz
      pos=point2pixel(1000,o)
      pos(1)=pos(1)+width2pixel(1000,sz(1)) // widget position 
      geom='wm geometry $w +'+string(pos(1))+'+'+ string(pos(2));
      title=block.label
      if title==[] then title="TK source",end
      tit='wm title $w Scale'+blknb // write block label
      bounds=block.rpar(1:2)
      bnds='-from '+string(bounds(1))+' -to '+string(bounds(2))
      cmd='-command ""f'+blknb+' $w.frame.scale""'
      lab='-label ""'+title+'""';
      L='-length 100'
      I='-tickinterval '+string((bounds(2)-bounds(1))/4)
      scale=strcat(['scale $w.frame.scale -orient vertical',..
                    lab,bnds,cmd,L,I],' ')
      initial=mean(bounds) // initial value is the mean
      txt=['set w .vscale'+blknb;
           'set y'+blknb+' 0';
           'catch {destroy $w}';
           'toplevel $w';
           tit
           geom
           'frame $w.frame -borderwidth 10';
           'pack $w.frame';
           scale
           'frame $w.frame.right -borderwidth 15';
           'pack $w.frame.scale -side left -anchor ne';
           '$w.frame.scale set '+string(initial);
           'proc f'+blknb+' {w height} {global y'+blknb+';set y'+blknb+' $height}'
          ];
      TCL_EvalStr(txt) // call TCL interpretor to create widget
      block.outptr(1)=mean(block.rpar(1:2))/block.rpar(3);
    end
  elseif flag==1 then // evaluate output during simulation
    block.outptr(1)=evstr(TCL_GetVar('y'+blknb))/block.rpar(3);
  end
endfunction ///\withPrompt{}



