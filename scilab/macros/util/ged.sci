function ged(k,win)
  //xset, xget used because ged should handle both old and new style
  global ged_current_figure
  global ged_cur_fig_handle
  
  ged_current_figure=xget('window')
  xset('window',win) 
  isold=get('figure_style')=='old'
  if isold&k>3 then 
    message('this menu does not apply to old style graphics')
    xset('window',ged_current_figure)
    clearglobal ged_current_figure
    clear ged_current_figure
    return
  end
  
  
  scf(win);
  ged_cur_fig_handle=gcf();
  
  if k>3 then
    TCL_EvalStr("set isgedinterp [interp exists ged]")
    if TCL_GetVar("isgedinterp")=='0' then    
      TCL_EvalStr("interp create ged")
      TCL_EvalStr("load {'+gettklib()+'} Tk ged")
      TCL_EvalStr("ged eval {wm withdraw .}")
    end
  end

  select k
    case 1 then //Select (make it current)
      return
    case 2 then //redraw
      if isold then xbasr(win),end
    case 3 then //erase
      xbasc()
         
    case 4 then //edit current figure properties
      // hierarchical viewer
    TK_send_handles_list(ged_cur_fig_handle)
    TCL_SetVar("curgedindex",string(Get_handle_pos_in_list(ged_cur_fig_handle)))
    
    //color_map array for color sample display
    f=ged_cur_fig_handle;
    for i=1:size(f.color_map,1)
      redname= "RED("+string(i)+")";
      TCL_EvalStr('set '+redname+" "+string(f.color_map(i,1)));
      grename= "GREEN("+string(i)+")";
      TCL_EvalStr('set '+grename+" "+string(f.color_map(i,2)));
      bluname= "BLUE("+string(i)+")";
      TCL_EvalStr('set '+bluname+" "+string(f.color_map(i,3)));
    end
    
    TCL_SetVar("msdos",string(MSDOS)) // to know the OS
    TCL_SetVar("sciGedIsAlive","Alive") ; // to know wether ged is opened or not

    //ged_fontarray = ["Courier" "Symbol" "Times" "Times Italic" "Times Bold" "Times Bold Italic"  "Helvetica"  "Helvetica Italic" "Helvetica Bold" "Helvetica Bold Italic"];
    
    ged_figure(ged_cur_fig_handle)
    case 5 then //edit current axes
      // hierarchical viewer
    TK_send_handles_list(ged_cur_fig_handle)
    TCL_SetVar("curgedindex",string(Get_handle_pos_in_list(gca())))
    
    //color_map array for color sample display
    f=ged_cur_fig_handle;
    for i=1:size(f.color_map,1)
      redname= "RED("+string(i)+")";
      TCL_EvalStr('set '+redname+" "+string(f.color_map(i,1)));
      grename= "GREEN("+string(i)+")";
      TCL_EvalStr('set '+grename+" "+string(f.color_map(i,2)));
      bluname= "BLUE("+string(i)+")";
      TCL_EvalStr('set '+bluname+" "+string(f.color_map(i,3)));
    end
    
    TCL_SetVar("msdos",string(MSDOS)) // to know the OS
    TCL_SetVar("sciGedIsAlive","Alive") ; // to know wether ged is opened or not

    ged_axes(gca())
    case 6 then //start Entity picker
      seteventhandler("ged_eventhandler")
    case 7 then //stop Entity picker
      seteventhandler("")
  end
  xset('window',ged_current_figure)
endfunction


function curgedindex_ = Get_handle_pos_in_list(h)
global ged_cur_fig_handle

curgedindex_ = [];

handles = Get_handles_list(ged_cur_fig_handle)
for i=1:size(handles,1)
  if (h==handles(i))
    curgedindex_ = i;
  end
end

// Other case :
// a label has been selected (and for now they are included inside the Axes)
if (curgedindex_==[])
  if h.type == "Label"
    h = h.parent;
    for i=1:size(handles,1)
      if (h==handles(i))
	curgedindex_ = i;
      end
    end
  end
end
  
endfunction


// Search the depth level for each handle
// Usefull for new hierarchical graphic tree.
function ged_levels = Get_levels(handles);

ged_levels = 1; // for Figure, always 1

f = handles(1);

for i=2:size(handles,1)
  ged_levels(i) = Get_Depth(f,handles(i));
end

//disp("les levels sont:")xb

//disp(ged_levels);

endfunction

function depth = Get_Depth(f,h)

depth = 2;

while  h.parent <> f
  h = h.parent;
  depth = depth + 1;
end

endfunction



function TK_send_handles_list(h)
iFig = 0;
iUim = 0;
iAxe = 0; // <=> subwindow in C code
iAgr = 0;
iPol = 0;
iPl3 = 0;
iFac = 0;
iRec = 0;
iTex = 0;
iLeg = 0;
iArc = 0;
iSeg = 0;
iCha = 0; // Champ
iFec = 0;
iGra = 0;
iMat = 0; // forgotten object F.Leray 22.10.04
iAxi = 0; // axis : entity created when using drawaxis method for example
iLab = 0;

f=getparfig(h);
handles = Get_handles_list(f)

ged_levels = Get_levels(handles);

TCL_SetVar("ged_handle_list_size",string(size(handles,1)));

for i=1:size(handles,1)
  SelObject="LEVELS("+string(i)+")";
  TCL_EvalStr('set '+SelObject+" "+string(ged_levels(i)));
end


for i=1:size(handles,1)
 SelObject="SELOBJECT("+string(i)+")";
 hand = handles(i);
 select  hand.type
   case "Figure"
    iFig = iFig+1;
    figname= "Figure("+string(iFig)+")";
    TCL_EvalStr('set '+SelObject+" "+figname);
   case "Uimenu"
    iUim = iUim+1;
    uimname= "Uimenu("+string(iUim)+")";
    TCL_EvalStr('set '+SelObject+" "+uimname);
   
   case "Axes"
    iAxe = iAxe+1;
    axename= "Axes("+string(iAxe)+")";
    TCL_EvalStr('set '+SelObject+" "+axename);
//   case "Label"  // to see later: have labels at the same level than Axes (to have a better visibility)
//    iLab = iLab+1;
//    labname= "Label("+string(iLab)+")";
//    TCL_EvalStr('set '+SelObject+" "+labname);
   case "Compound"
    iAgr = iAgr+1;
    agrname= "Compound("+string(iAgr)+")";
    TCL_EvalStr('set '+SelObject+" "+agrname);
   case "Polyline"
    iPol = iPol+1;
    polname= "Polyline("+string(iPol)+")";
    TCL_EvalStr('set '+SelObject+" "+polname);
   case "Plot3d"
    iPl3 = iPl3+1;
    pl3name= "Plot3d("+string(iPl3)+")";
    TCL_EvalStr('set '+SelObject+" "+pl3name);
   case "Fac3d"
    iFac = iFac+1;
    Facname= "Fac3d("+string(iFac)+")";
    TCL_EvalStr('set '+SelObject+" "+Facname);
   case "Rectangle"
    iRec = iRec+1;
    Recname= "Rectangle("+string(iRec)+")";
    TCL_EvalStr('set '+SelObject+" "+Recname);
   case "Text"
    iTex = iTex+1;
    Texname= "Text("+string(iTex)+")";
    TCL_EvalStr('set '+SelObject+" "+Texname);
   case "Legend"
    iLeg = iLeg+1;
    Legname= "Legend("+string(iLeg)+")";
    TCL_EvalStr('set '+SelObject+" "+Legname);
   case "Arc"
    iArc = iArc+1;
    Arcname= "Arc("+string(iArc)+")";
    TCL_EvalStr('set '+SelObject+" "+Arcname);
   case "Segs"
    iSeg = iSeg+1;
    Segname= "Segs("+string(iSeg)+")";
    TCL_EvalStr('set '+SelObject+" "+Segname);
   case "Champ"
    iCha = iCha+1;
    Chaname= "Champ("+string(iCha)+")";
    TCL_EvalStr('set '+SelObject+" "+Chaname);
   case "Fec"
    iFec = iFec+1;
    Fecname= "Fec("+string(iFec)+")";
    TCL_EvalStr('set '+SelObject+" "+Fecname);
   case "Grayplot"
    iGra = iGra+1;
    Graname= "Grayplot("+string(iGra)+")";
    TCL_EvalStr('set '+SelObject+" "+Graname);
   case "Matplot"
    iMat = iMat+1;
    Matname= "Matplot("+string(iMat)+")";
    TCL_EvalStr('set '+SelObject+" "+Matname);
   case "Axis"
    iAxi = iAxi+1;
    Axiname= "Axis("+string(iAxi)+")";
    TCL_EvalStr('set '+SelObject+" "+Axiname);
  end
end
endfunction


//function h=Get_handle_from_index(index)
function Get_handle_from_index(index)
   global ged_handle;
   global ged_cur_fig_handle

   hl = Get_handles_list(ged_cur_fig_handle);

   ged_handle = hl(index);
//   h=ged_handle;
   tkged();

endfunction


////////////////////////////////////////////
function  hfig= getparfig( h )

htmp = h;
hfig= []

while htmp.type<>'Figure' do
  htmp=htmp.parent
end

hfig = htmp;
endfunction


function h_out_list=Get_handles_list(h);

global ged_handle_out;

f=getparfig(h);
ged_handle_out=[f];

List_handles(f);

//disp(ged_handle_out);

h_out_list=ged_handle_out;

endfunction


function List_handles(h)
global ged_handle_out;

i = 1;
//if h.type=="Axes" then
//   ged_handle_out = [ged_handle_out;h.x_label;h.y_label;h.z_label;h.title];
//end
psonstmp = h.children;
if psonstmp <> [] then
  psonstmp = h.children(1);
end
//pause
while ((psonstmp <>[]) & ((i) <=size(psonstmp.parent.children,1)))
  i = i+1;
  ged_handle_out = [ged_handle_out;  psonstmp];
  List_handles(psonstmp);
  //  disp("Processus recursif RETOUR")
  //  disp("    ici 1");
  //  pause
  if ((i) <=size(psonstmp.parent.children,1)) then
    psonstmp = psonstmp.parent.children(i);
  else
    psonstmp=[];
  end
  //  disp("    ici 2");
  //  pause
end

endfunction



function ged_Compound(h)
  global ged_handle;ged_handle=h;
  TCL_SetVar("curvis",h.visible)
  TCL_EvalFile(SCI+'/tcl/ged/Compound.tcl')
endfunction


function ged_figure(h)
  global ged_handle;ged_handle=h;
  TCL_SetVar("background",string(h.background))
  TCL_SetVar("rotation_style",h.rotation_style)
  TCL_SetVar("figure_name",h.figure_name)
  TCL_SetVar("figure_id",string(h.figure_id))
  TCL_SetVar("figure_xposition",string(h.figure_position(1)))
  TCL_SetVar("figure_yposition",string(h.figure_position(2)))
  TCL_SetVar("figure_xsiz",string(h.figure_size(1)))
  TCL_SetVar("figure_ysiz",string(h.figure_size(2)))
  TCL_SetVar("figure_xaxesiz",string(h.axes_size(1)))
  TCL_SetVar("figure_yaxesiz",string(h.axes_size(2)))
  TCL_SetVar("bcolor",string(h.background))
  TCL_SetVar("ncolors",string(size(h.color_map,1)))
  TCL_SetVar("curvis",h.visible)
  TCL_SetVar("curpix",h.pixmap)
  TCL_SetVar("curpdm",h.pixel_drawing_mode)
  TCL_SetVar("curautoresize",h.auto_resize)
  TCL_SetVar("currotation_style",h.rotation_style)
  TCL_EvalFile(SCI+'/tcl/ged/Figure.tcl')
endfunction


function ged_axes(h)
  global ged_handle;ged_handle=h;
  
  LoadTicks2TCL(h);
  
  TCL_SetVar("Xaxes_reverseToggle",h.axes_reverse(1))
  TCL_SetVar("Yaxes_reverseToggle",h.axes_reverse(2))
  TCL_SetVar("Zaxes_reverseToggle",h.axes_reverse(3))

// forgotten axes bounds info.
  TCL_SetVar("axes_boundsL",string(h.axes_bounds(1,1)))
  TCL_SetVar("axes_boundsU",string(h.axes_bounds(1,2)))
  TCL_SetVar("axes_boundsW",string(h.axes_bounds(1,3)))
  TCL_SetVar("axes_boundsH",string(h.axes_bounds(1,4)))

 // forgotten visibility info.
  TCL_SetVar("xlabel_visibility",string(h.x_label.visible))
  TCL_SetVar("ylabel_visibility",string(h.y_label.visible))
  TCL_SetVar("zlabel_visibility",string(h.z_label.visible))
  TCL_SetVar("titlelabel_visibility",string(h.title.visible))

  TCL_SetVar("Lmargins",string(h.margins(1)));
  TCL_SetVar("Rmargins",string(h.margins(2)));
  TCL_SetVar("Tmargins",string(h.margins(3)));
  TCL_SetVar("Bmargins",string(h.margins(4)));
  ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash"]; 
  TCL_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))
  if (h.clip_box==[])
    TCL_SetVar("old_Xclipbox","")
    TCL_SetVar("old_Yclipbox","")
    TCL_SetVar("old_Wclipbox","")
    TCL_SetVar("old_Hclipbox","")
    TCL_SetVar("Xclipbox","")
    TCL_SetVar("Yclipbox","")
    TCL_SetVar("Wclipbox","")
    TCL_SetVar("Hclipbox","")
   else
    TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
    TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
    TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
    TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
    TCL_SetVar("Xclipbox",string(h.clip_box(1)))
    TCL_SetVar("Yclipbox",string(h.clip_box(2)))
    TCL_SetVar("Wclipbox",string(h.clip_box(3)))
    TCL_SetVar("Hclipbox",string(h.clip_box(4)))
  end
  TCL_SetVar("curclipstate",h.clip_state);
  TCL_SetVar("curautoclear",h.auto_clear);
  TCL_SetVar("curautoscale",h.auto_scale);
  //TCL_SetVar("curfillmode",h.fill_mode);
  TCL_SetVar("curalpharotation",string(h.rotation_angles(1)))
  TCL_SetVar("curthetarotation",string(h.rotation_angles(2)))
  ged_fontarray = ["Courier" "Symbol" "Times" "Times Italic"...
	 "Times Bold" "Times Bold Italic"  "Helvetica"  "Helvetica  Italic"...
	 "Helvetica Bold" "Helvetica Bold Italic"];
  TCL_SetVar("Xlabelpos",h.x_location)
  TCL_SetVar("Ylabelpos",h.y_location)
  TCL_SetVar("Xlabelfontstyle",ged_fontarray(h.x_label.font_style+1))
  TCL_SetVar("Ylabelfontstyle",ged_fontarray(h.y_label.font_style+1))
  TCL_SetVar("Zlabelfontstyle",ged_fontarray(h.z_label.font_style+1))
  TCL_SetVar("TITLEfontstyle",ged_fontarray(h.title.font_style+1))
  TCL_SetVar("fontstyle",ged_fontarray(h.font_style+1))
  TCL_SetVar("xlabel",h.x_label.text)
  TCL_SetVar("ylabel",h.y_label.text)
  TCL_SetVar("zlabel",h.z_label.text)
  TCL_SetVar("tlabel",h.title.text)
//  TCL_SetVar("xlabel_fontforeground",string(h.x_label.font_foreground))
//  TCL_SetVar("ylabel_fontforeground",string(h.y_label.font_foreground))
//  TCL_SetVar("zlabel_fontforeground",string(h.z_label.font_foreground))
//  TCL_SetVar("titlelabel_fontforeground",string(h.title.font_foreground))
  TCL_SetVar("xlabel_foreground",string(h.x_label.foreground))
  TCL_SetVar("ylabel_foreground",string(h.y_label.foreground))
  TCL_SetVar("zlabel_foreground",string(h.z_label.foreground))
  TCL_SetVar("titlelabel_foreground",string(h.title.foreground))
  TCL_SetVar("xlabel_background",string(h.x_label.background))
  TCL_SetVar("ylabel_background",string(h.y_label.background))
  TCL_SetVar("zlabel_background",string(h.z_label.background))
  TCL_SetVar("titlelabel_background",string(h.title.background))
  TCL_SetVar("x_position",sci2exp(h.x_label.position,0))
  TCL_SetVar("y_position",sci2exp(h.y_label.position,0))
  TCL_SetVar("z_position",sci2exp(h.z_label.position,0))
  TCL_SetVar("title_position",sci2exp(h.title.position,0))
  TCL_SetVar("xauto_position",h.x_label.auto_position)
  TCL_SetVar("yauto_position",h.y_label.auto_position)
  TCL_SetVar("zauto_position",h.z_label.auto_position)
  TCL_SetVar("titleauto_position",h.title.auto_position)
  TCL_SetVar("xauto_rotation",h.x_label.auto_rotation)
  TCL_SetVar("yauto_rotation",h.y_label.auto_rotation)
  TCL_SetVar("zauto_rotation",h.z_label.auto_rotation)
  TCL_SetVar("titleauto_rotation",h.title.auto_rotation)
  TCL_SetVar("Xfillmode",h.x_label.fill_mode)
  TCL_SetVar("Yfillmode",h.y_label.fill_mode)
  TCL_SetVar("Zfillmode",h.z_label.fill_mode)
  TCL_SetVar("Titlefillmode",h.title.fill_mode)
  TCL_SetVar("xlabel_fontsize",string(h.x_label.font_size))
  TCL_SetVar("ylabel_fontsize",string(h.y_label.font_size))
  TCL_SetVar("zlabel_fontsize",string(h.z_label.font_size))
  TCL_SetVar("titlelabel_fontsize",string(h.title.font_size))
  TCL_SetVar("ncolors",string(size(f.color_map,1)))
  TCL_SetVar("fcolor",string(h.foreground))
  TCL_SetVar("bcolor",string(h.background))
  TCL_SetVar("curthick",string(h.thickness))
  TCL_SetVar("curvis",h.visible)
  TCL_SetVar("curfontsize",string(h.font_size))
  TCL_SetVar("curfontcolor",string(h.font_color))
  TCL_SetVar("limToggle",h.tight_limits)
  TCL_SetVar("isoToggle",h.isoview)
  TCL_SetVar("cubToggle",h.cube_scaling)
  TCL_SetVar("viewToggle",h.view)
  TCL_SetVar("boxToggle",h.box)
  TCL_SetVar("xToggle",part(h.log_flags,1))
  TCL_SetVar("yToggle",part(h.log_flags,2))
  TCL_SetVar("zToggle",part(h.log_flags,3))
  TCL_SetVar("xGrid",string(h.grid(1)))
  TCL_SetVar("yGrid",string(h.grid(2)))
  
  TCL_SetVar("hiddencolor",string(h.hiddencolor))
  TCL_SetVar("curfontangle_x",string(h.x_label.font_angle))
  TCL_SetVar("curfontangle_y",string(h.y_label.font_angle))
  TCL_SetVar("curfontangle_z",string(h.z_label.font_angle))
  TCL_SetVar("curfontangle_title",string(h.title.font_angle))

  
  select h.view
    case "2d" 
    drawlater();
    h.view='3d'
    TCL_SetVar("old_curalpharotation",string(h.rotation_angles(1)))
    TCL_SetVar("old_curthetarotation",string(h.rotation_angles(2)))
    TCL_SetVar("zGrid",string(h.grid(3)))
    TCL_SetVar("zGrid_initial",string(h.grid(3))) //to avoid useless redraw (see Axes.tcl)
    TCL_SetVar("dbxmin",string(h.data_bounds(1,1)))
    TCL_SetVar("dbymin",string(h.data_bounds(1,2)))
    TCL_SetVar("dbzmin",string(h.data_bounds(1,3)))
    TCL_SetVar("dbxmax",string(h.data_bounds(2,1)))
    TCL_SetVar("dbymax",string(h.data_bounds(2,2)))
    TCL_SetVar("dbzmax",string(h.data_bounds(2,3)))
    h.view='2d'
    drawnow();
    case "3d"
    TCL_SetVar("zGrid",string(h.grid(3)))
    TCL_SetVar("zGrid_initial",string(h.grid(3))) //to avoid useless redraw (see Axes.tcl)
    TCL_SetVar("dbxmin",string(h.data_bounds(1,1)))
    TCL_SetVar("dbymin",string(h.data_bounds(1,2)))
    TCL_SetVar("dbzmin",string(h.data_bounds(1,3)))
    TCL_SetVar("dbxmax",string(h.data_bounds(2,1)))
    TCL_SetVar("dbymax",string(h.data_bounds(2,2)))
    TCL_SetVar("dbzmax",string(h.data_bounds(2,3)))
  end
 
  TCL_EvalFile(SCI+'/tcl/ged/Axes.tcl')
endfunction

function ged_rectangle(h)
  global ged_handle; ged_handle=h
  if (h.clip_box==[])
    TCL_SetVar("old_Xclipbox","")
    TCL_SetVar("old_Yclipbox","")
    TCL_SetVar("old_Wclipbox","")
    TCL_SetVar("old_Hclipbox","")
    TCL_SetVar("Xclipbox","")
    TCL_SetVar("Yclipbox","")
    TCL_SetVar("Wclipbox","")
    TCL_SetVar("Hclipbox","")
   else
    TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
    TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
    TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
    TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
    TCL_SetVar("Xclipbox",string(h.clip_box(1)))
    TCL_SetVar("Yclipbox",string(h.clip_box(2)))
    TCL_SetVar("Wclipbox",string(h.clip_box(3)))
    TCL_SetVar("Hclipbox",string(h.clip_box(4)))
  end
  TCL_SetVar("curclipstate",h.clip_state);

  f=h;while stripblanks(f.type)<>"Figure" then f=f.parent,end
  ax=h;while stripblanks(ax.type)<>"Axes" then ax=ax.parent,end
  TCL_SetVar("ncolors",string(size(f.color_map,1)))
  TCL_SetVar("curcolor",string(h.foreground))
  TCL_SetVar("curback",string(h.background))
  TCL_SetVar("curthick",string(h.thickness))
  TCL_SetVar("curvis",h.visible)
  ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash"]; 
  TCL_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))
  ged_markstylearray=["dot" "plus" "cross" "star" "filled diamond" ..
  "diamond" "triangle up" "triangle down" "diamond plus" "circle" ..
  "asterisk" "square" "triangle right" "triangle left" "pentagram"];
  TCL_SetVar("curmarkstyle",ged_markstylearray(abs(h.mark_style)+1))
  TCL_SetVar("curmarkmode",h.mark_mode)
  TCL_SetVar("curmarksize",string(h.mark_size))
  TCL_SetVar("curmarksizeunit",h.mark_size_unit);
  TCL_SetVar("curmarkforeground",string(h.mark_foreground))
  TCL_SetVar("curmarkbackground",string(h.mark_background))

 
  TCL_SetVar("curlinemode",h.line_mode)
  TCL_SetVar("curfillmode",h.fill_mode)
  // Rectangle data
   select ax.view
    case "2d" 
    drawlater();
    ax.view='3d'
    TCL_SetVar("Xval",string(h.data(1)))
    TCL_SetVar("Yval",string(h.data(2)))
    TCL_SetVar("Zval",string(h.data(3)))
    TCL_SetVar("Wval",string(h.data(4)))
    TCL_SetVar("Hval",string(h.data(5)))
    ax.view='2d'
    drawnow();
    case "3d"
    TCL_SetVar("Xval",string(h.data(1)))
    TCL_SetVar("Yval",string(h.data(2)))
    TCL_SetVar("Zval",string(h.data(3)))
    TCL_SetVar("Wval",string(h.data(4)))
    TCL_SetVar("Hval",string(h.data(5)))
  end
  TCL_EvalFile(SCI+'/tcl/ged/Rectangle.tcl')
endfunction

function ged_polyline(h)
    global ged_handle; ged_handle=h
 
  if (h.clip_box==[])
    TCL_SetVar("old_Xclipbox","")
    TCL_SetVar("old_Yclipbox","")
    TCL_SetVar("old_Wclipbox","")
    TCL_SetVar("old_Hclipbox","")
    TCL_SetVar("Xclipbox","")
    TCL_SetVar("Yclipbox","")
    TCL_SetVar("Wclipbox","")
    TCL_SetVar("Hclipbox","")
   else
    TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
    TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
    TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
    TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
    TCL_SetVar("Xclipbox",string(h.clip_box(1)))
    TCL_SetVar("Yclipbox",string(h.clip_box(2)))
    TCL_SetVar("Wclipbox",string(h.clip_box(3)))
    TCL_SetVar("Hclipbox",string(h.clip_box(4)))
  end
  TCL_SetVar("curclipstate",h.clip_state);

    f=h;while stripblanks(f.type)<>"Figure" then f=f.parent,end
    TCL_SetVar("ncolors",string(size(f.color_map,1)))
    TCL_SetVar("curcolor",string(h.foreground))
    TCL_SetVar("curback",string(h.background))
    TCL_SetVar("curthick",string(h.thickness))
    TCL_SetVar("curarrowsizefactor",string(h.arrow_size_factor))
    TCL_SetVar("curvis",h.visible)

    ged_polylinestylearray=["interpolated" "staircase" "barplot" "arrowed" "filled" "bar"];
    TCL_SetVar("curpolylinestyle",ged_polylinestylearray(max(h.polyline_style,1)))
    ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash"];
    TCL_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))
    ged_markstylearray=["dot" "plus" "cross" "star" "filled diamond" ..
    "diamond" "triangle up" "triangle down" "diamond plus" "circle" ..
    "asterisk" "square" "triangle right" "triangle left" "pentagram"];

    TCL_SetVar("curmarkstyle",ged_markstylearray(abs(h.mark_style)+1))
    TCL_SetVar("curmarkmode",h.mark_mode)
    TCL_SetVar("curmarksize",string(h.mark_size))
    TCL_SetVar("curmarksizeunit",h.mark_size_unit);
    TCL_SetVar("curmarkforeground",string(h.mark_foreground))
    TCL_SetVar("curmarkbackground",string(h.mark_background))

    TCL_SetVar("curlinemode",h.line_mode)
    TCL_SetVar("curclosedmode",h.closed)
    TCL_SetVar("curfillmode",h.fill_mode)
    TCL_SetVar("curinterpcolormode",h.interp_color_mode)
    TCL_SetVar("curinterpcolorvector",sci2exp(h.interp_color_vector,0))
        
    d="["+strcat(string(size(h.data)),'x')+" double array]"
    TCL_SetVar("curdata",d);

    select get(getparaxe(h),'view')
      case "2d"
      TCL_SetVar("nbcol",string(2));
      case "3d"
      TCL_SetVar("nbcol",string(3));
    end
    TCL_EvalFile(SCI+'/tcl/ged/Polyline.tcl')
endfunction


function ged_plot3d(h)
  global ged_handle; ged_handle=h

  //  if (h.clip_box==[])
//    TCL_SetVar("old_Xclipbox","")
//    TCL_SetVar("old_Yclipbox","")
//    TCL_SetVar("old_Wclipbox","")
//    TCL_SetVar("old_Hclipbox","")
//    TCL_SetVar("Xclipbox","")
//    TCL_SetVar("Yclipbox","")
//    TCL_SetVar("Wclipbox","")
//    TCL_SetVar("Hclipbox","")
//   else
//    TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
//    TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
//    TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
//    TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
//    TCL_SetVar("Xclipbox",string(h.clip_box(1)))
//    TCL_SetVar("Yclipbox",string(h.clip_box(2)))
//    TCL_SetVar("Wclipbox",string(h.clip_box(3)))
//    TCL_SetVar("Hclipbox",string(h.clip_box(4)))
//  end
//  TCL_SetVar("curclipstate",h.clip_state);

  TCL_SetVar("curvis",h.visible)
  TCL_SetVar("curcolormode",string(h.color_mode))
  TCL_SetVar("colorflagToggle",string(h.color_flag))
  TCL_SetVar("curforeground",string(h.foreground))
  TCL_SetVar("curhiddencolor",string(h.hiddencolor))
  TCL_SetVar("curthick",string(h.thickness))

  ged_markstylearray=["dot" "plus" "cross" "star" "filled diamond" ..
  "diamond" "triangle up" "triangle down" "diamond plus" "circle" ..
  "asterisk" "square" "triangle right" "triangle left" "pentagram"];

  TCL_SetVar("curmarkstyle",ged_markstylearray(abs(h.mark_style)+1))
  TCL_SetVar("curmarkmode",h.mark_mode)
  TCL_SetVar("curmarksize",string(h.mark_size))
  TCL_SetVar("curmarksizeunit",h.mark_size_unit);
  TCL_SetVar("curmarkforeground",string(h.mark_foreground))
  TCL_SetVar("curmarkbackground",string(h.mark_background))
  TCL_SetVar("curlinemode",h.surface_mode)


  d="["+strcat(string(size(h.data.x)),'x')+" double array]"
  TCL_SetVar("curdata_x",d);
  d="["+strcat(string(size(h.data.y)),'x')+" double array]"
  TCL_SetVar("curdata_y",d);
  d="["+strcat(string(size(h.data.z)),'x')+" double array]"
  TCL_SetVar("curdata_z",d);

  TCL_EvalStr('set flagCOLOR 0')
  if(h.data(1)==["3d" "x" "y" "z" "color"])
  TCL_EvalStr('set flagCOLOR 1')
    d="["+strcat(string(size(h.data.color)),'x')+" integer array]"
    TCL_SetVar("curdata_color",d);
  end
  TCL_EvalFile(SCI+'/tcl/ged/Plot3d.tcl')
endfunction


function ged_fac3d(h)
  global ged_handle; ged_handle=h

  //  if (h.clip_box==[])
//    TCL_SetVar("old_Xclipbox","")
//    TCL_SetVar("old_Yclipbox","")
//    TCL_SetVar("old_Wclipbox","")
//    TCL_SetVar("old_Hclipbox","")
//    TCL_SetVar("Xclipbox","")
//    TCL_SetVar("Yclipbox","")
//    TCL_SetVar("Wclipbox","")
//    TCL_SetVar("Hclipbox","")
//   else
//    TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
//    TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
//    TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
//    TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
//    TCL_SetVar("Xclipbox",string(h.clip_box(1)))
//    TCL_SetVar("Yclipbox",string(h.clip_box(2)))
//    TCL_SetVar("Wclipbox",string(h.clip_box(3)))
//    TCL_SetVar("Hclipbox",string(h.clip_box(4)))
//  end
//  TCL_SetVar("curclipstate",h.clip_state);

  TCL_SetVar("curvis",h.visible)
  TCL_SetVar("curcolormode",string(h.color_mode))
  TCL_SetVar("colorflagToggle",string(h.color_flag))
  TCL_SetVar("curforeground",string(h.foreground))
  TCL_SetVar("curhiddencolor",string(h.hiddencolor))
  TCL_SetVar("curthick",string(h.thickness))

  ged_markstylearray=["dot" "plus" "cross" "star" "filled diamond" ..
  "diamond" "triangle up" "triangle down" "diamond plus" "circle" ..
  "asterisk" "square" "triangle right" "triangle left" "pentagram"];

  TCL_SetVar("curmarkstyle",ged_markstylearray(abs(h.mark_style)+1))
  TCL_SetVar("curmarkmode",h.mark_mode)
  TCL_SetVar("curmarksize",string(h.mark_size))
  TCL_SetVar("curmarksizeunit",h.mark_size_unit);
  TCL_SetVar("curmarkforeground",string(h.mark_foreground))
  TCL_SetVar("curmarkbackground",string(h.mark_background))
  TCL_SetVar("curlinemode",h.surface_mode)


  d="["+strcat(string(size(h.data.x)),'x')+" double array]"
  TCL_SetVar("curdata_x",d);
  d="["+strcat(string(size(h.data.y)),'x')+" double array]"
  TCL_SetVar("curdata_y",d);
   d="["+strcat(string(size(h.data.z)),'x')+" double array]"
  TCL_SetVar("curdata_z",d);

  TCL_EvalStr('set flagCOLOR 0')
  if(h.data(1)==["3d" "x" "y" "z" "color"])
  TCL_EvalStr('set flagCOLOR 1')
    d="["+strcat(string(size(h.data.color)),'x')+" integer array]"
    TCL_SetVar("curdata_color",d);
  end


  TCL_EvalFile(SCI+'/tcl/ged/Fac3d.tcl')
endfunction


function ged_text(h)
  global ged_handle; ged_handle=h
  f=h;while stripblanks(f.type)<>"Figure" then f=f.parent,end
  TCL_SetVar("curvis",h.visible)
  TCL_SetVar("ncolors",string(size(f.color_map,1)))
  TCL_SetVar("curfontforeground",string(h.font_foreground))
  ged_fontarray = ["Courier" "Symbol" "Times" "Times Italic"...
	 "Times Bold" "Times Bold Italic"  "Helvetica"  "Helvetica  Italic"...
	 "Helvetica Bold" "Helvetica Bold Italic"];
  TCL_SetVar("curfontstyle",ged_fontarray(h.font_style+1))
  TCL_SetVar("curfontsize",string(h.font_size))
  TCL_SetVar("curfontangle",string(h.font_angle))
//TCL_SetVar("curtextbox1",string(h.textbox(1)))
//TCL_SetVar("curtextbox2",string(h.textbox(2)))
  TCL_SetVar("curtextboxmode",h.text_box_mode)
  TCL_SetVar("curtext",h.text)
  TCL_SetVar("curforeground",string(h.foreground))
  TCL_SetVar("curbackground",string(h.background))
  TCL_SetVar("curboxmode",h.box)

  if (h.clip_box==[])
    TCL_SetVar("old_Xclipbox","")
    TCL_SetVar("old_Yclipbox","")
    TCL_SetVar("old_Wclipbox","")
    TCL_SetVar("old_Hclipbox","")
    TCL_SetVar("Xclipbox","")
    TCL_SetVar("Yclipbox","")
    TCL_SetVar("Wclipbox","")
    TCL_SetVar("Hclipbox","")
   else
    TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
    TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
    TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
    TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
    TCL_SetVar("Xclipbox",string(h.clip_box(1)))
    TCL_SetVar("Yclipbox",string(h.clip_box(2)))
    TCL_SetVar("Wclipbox",string(h.clip_box(3)))
    TCL_SetVar("Hclipbox",string(h.clip_box(4)))
  end
  TCL_SetVar("curclipstate",h.clip_state);

  TCL_EvalFile(SCI+'/tcl/ged/Text.tcl')
endfunction


function ged_legend(h)
  global ged_handle; ged_handle=h
  TCL_SetVar("curvis",h.visible)
  TCL_SetVar("ncolors",string(size(f.color_map,1)))
  TCL_SetVar("curforeground",string(h.foreground))
  ged_fontarray = ["Courier" "Symbol" "Times" "Times Italic"...
	 "Times Bold" "Times Bold Italic"  "Helvetica"  "Helvetica  Italic"...
	 "Helvetica Bold" "Helvetica Bold Italic"];
  TCL_SetVar("curfontstyle",ged_fontarray(h.font_style+1))
  TCL_SetVar("curfontsize",string(h.font_size))
  TCL_SetVar("curtext",h.text)

  TCL_EvalFile(SCI+'/tcl/ged/Legend.tcl')
endfunction


function ged_arc(h)
  global ged_handle; ged_handle=h

  if (h.clip_box==[])
    TCL_SetVar("old_Xclipbox","")
    TCL_SetVar("old_Yclipbox","")
    TCL_SetVar("old_Wclipbox","")
    TCL_SetVar("old_Hclipbox","")
    TCL_SetVar("Xclipbox","")
    TCL_SetVar("Yclipbox","")
    TCL_SetVar("Wclipbox","")
    TCL_SetVar("Hclipbox","")
   else
    TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
    TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
    TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
    TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
    TCL_SetVar("Xclipbox",string(h.clip_box(1)))
    TCL_SetVar("Yclipbox",string(h.clip_box(2)))
    TCL_SetVar("Wclipbox",string(h.clip_box(3)))
    TCL_SetVar("Hclipbox",string(h.clip_box(4)))
  end
  TCL_SetVar("curclipstate",h.clip_state);
  ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash"];
  TCL_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))
  TCL_SetVar("nbcol",string(size(h.data,2)))
  for i=1:size(h.data,2)
    val= "arcVAL("+string(i)+")";
      TCL_EvalStr('set '+val+" "+string(h.data(2)));
  end
  f=h;while stripblanks(f.type)<>"Figure" then f=f.parent,end
  TCL_SetVar("ncolors",string(size(f.color_map,1)))
  TCL_SetVar("curcolor",string(h.foreground))
  TCL_SetVar("curback",string(h.background))
  TCL_SetVar("curvis",h.visible)
  TCL_SetVar("curlinemode",h.line_mode)
  TCL_SetVar("curfillmode",h.fill_mode)
  TCL_SetVar("curthick",string(h.thickness))

  ax=getparaxe(h);
  // Arc data
  select ax.view
  case "2d" 
    drawlater();
    ax.view='3d'  //strange behavior in 3D... seems to be bugged!!
    TCL_SetVar("Xval",string(h.data(1)))
    TCL_SetVar("Yval",string(h.data(2)))
    TCL_SetVar("Zval",string(h.data(3)))
    TCL_SetVar("Wval",string(h.data(4)))
    TCL_SetVar("Hval",string(h.data(5)))
    TCL_SetVar("A1val",string(h.data(6)))
    TCL_SetVar("A2val",string(h.data(7)))
     ax.view='2d'
     drawnow();
    case "3d"
    TCL_SetVar("Xval",string(h.data(1)))
    TCL_SetVar("Yval",string(h.data(2)))
    TCL_SetVar("Zval",string(h.data(3)))
    TCL_SetVar("Wval",string(h.data(4)))
    TCL_SetVar("Hval",string(h.data(5)))
    TCL_SetVar("A1val",string(h.data(6)))
    TCL_SetVar("A2val",string(h.data(7)))
  end
  TCL_EvalFile(SCI+'/tcl/ged/Arc.tcl')
endfunction


function ged_segs(h)
  global ged_handle; ged_handle=h
  TCL_SetVar("curvis",h.visible)
  TCL_SetVar("ncolors",string(size(f.color_map,1)))
  TCL_SetVar("curarrowsize",string(h.arrow_size))
  TCL_SetVar("curthick",string(h.thickness))
  ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash"]; 
  TCL_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))

  ged_markstylearray=["dot" "plus" "cross" "star" "filled diamond" ..
  "diamond" "triangle up" "triangle down" "diamond plus" "circle" ..
  "asterisk" "square" "triangle right" "triangle left" "pentagram"];

  TCL_SetVar("curmarkstyle",ged_markstylearray(abs(h.mark_style)+1))
  TCL_SetVar("curmarkmode",h.mark_mode)
  TCL_SetVar("curmarksize",string(h.mark_size))
  TCL_SetVar("curmarksizeunit",h.mark_size_unit);
  TCL_SetVar("curmarkforeground",string(h.mark_foreground))
  TCL_SetVar("curmarkbackground",string(h.mark_background))
  TCL_SetVar("curlinemode",h.line_mode)
    
  TCL_SetVar("nbrow",string(size(h.data,1)))
  
   d="["+strcat(string(size(h.data)),'x')+" double array]"
  TCL_SetVar("curdata",d);
  
  TCL_SetVar("nbcolsegscolor",string(size(h.segs_color,2)))
  for i=1:size(h.segs_color,2)
    val= "segscolorVAL("+string(i)+")";
    TCL_EvalStr('set '+val+" "+string(h.segs_color(i)));
  end

  if (h.clip_box==[])
    TCL_SetVar("old_Xclipbox","")
    TCL_SetVar("old_Yclipbox","")
    TCL_SetVar("old_Wclipbox","")
    TCL_SetVar("old_Hclipbox","")
    TCL_SetVar("Xclipbox","")
    TCL_SetVar("Yclipbox","")
    TCL_SetVar("Wclipbox","")
    TCL_SetVar("Hclipbox","")
   else
    TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
    TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
    TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
    TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
    TCL_SetVar("Xclipbox",string(h.clip_box(1)))
    TCL_SetVar("Yclipbox",string(h.clip_box(2)))
    TCL_SetVar("Wclipbox",string(h.clip_box(3)))
    TCL_SetVar("Hclipbox",string(h.clip_box(4)))
  end
  TCL_SetVar("curclipstate",h.clip_state);

  TCL_EvalFile(SCI+'/tcl/ged/Segs.tcl')
endfunction


function ged_champ(h)
  global ged_handle; ged_handle=h
  TCL_SetVar("curvis",h.visible)
  TCL_SetVar("curcolored",h.colored)
  TCL_SetVar("ncolors",string(size(f.color_map,1)))
  TCL_SetVar("curarrowsize",string(h.arrow_size))
  TCL_SetVar("curthick",string(h.thickness))
  ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash"]; 
  TCL_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))

  d="["+strcat(string(size(h.data.x)),'x')+" double array]"
  TCL_SetVar("curdata_x",d);
  d="["+strcat(string(size(h.data.y)),'x')+" double array]"
  TCL_SetVar("curdata_y",d);
  d="["+strcat(string(size(h.data.fx)),'x')+" double array]"
  TCL_SetVar("curdata_fx",d);
  d="["+strcat(string(size(h.data.fy)),'x')+" double array]"
  TCL_SetVar("curdata_fy",d);

  if (h.clip_box==[])
    TCL_SetVar("old_Xclipbox","")
    TCL_SetVar("old_Yclipbox","")
    TCL_SetVar("old_Wclipbox","")
    TCL_SetVar("old_Hclipbox","")
    TCL_SetVar("Xclipbox","")
    TCL_SetVar("Yclipbox","")
    TCL_SetVar("Wclipbox","")
    TCL_SetVar("Hclipbox","")
   else
    TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
    TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
    TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
    TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
    TCL_SetVar("Xclipbox",string(h.clip_box(1)))
    TCL_SetVar("Yclipbox",string(h.clip_box(2)))
    TCL_SetVar("Wclipbox",string(h.clip_box(3)))
    TCL_SetVar("Hclipbox",string(h.clip_box(4)))
  end
  TCL_SetVar("curclipstate",h.clip_state);

  TCL_EvalFile(SCI+'/tcl/ged/Champ.tcl')
endfunction

function ged_fec(h)
  global ged_handle; ged_handle=h
  TCL_SetVar("curvis",h.visible)
  TCL_SetVar("zbmin",string(h.z_bounds(1)))
  TCL_SetVar("zbmax",string(h.z_bounds(2)))
//  TCL_SetVar("nbrow",string(size(h.data,1)))
//  TCL_SetVar("nbcol",string(size(h.data,2)))
//  TCL_SetVar("nbrowTri",string(size(h.triangles,1)))
//  TCL_SetVar("nbcolTri",string(size(h.triangles,2)))

  d="["+strcat(string(size(h.data)),'x')+" double array]"
  TCL_SetVar("curdata_data",d);
  d="["+strcat(string(size(h.triangles)),'x')+" double array]"
  TCL_SetVar("curdata_triangles",d);

//  if (h.clip_box==[])
//    TCL_SetVar("old_Xclipbox","")
//    TCL_SetVar("old_Yclipbox","")
//    TCL_SetVar("old_Wclipbox","")
//    TCL_SetVar("old_Hclipbox","")
//    TCL_SetVar("Xclipbox","")
//    TCL_SetVar("Yclipbox","")
//    TCL_SetVar("Wclipbox","")
//    TCL_SetVar("Hclipbox","")
//   else
//    TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
//    TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
//    TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
//    TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
//    TCL_SetVar("Xclipbox",string(h.clip_box(1)))
//    TCL_SetVar("Yclipbox",string(h.clip_box(2)))
//    TCL_SetVar("Wclipbox",string(h.clip_box(3)))
//    TCL_SetVar("Hclipbox",string(h.clip_box(4)))
//  end
//  TCL_SetVar("curclipstate",h.clip_state);

  TCL_EvalFile(SCI+'/tcl/ged/Fec.tcl')
endfunction


function ged_grayplot(h)
  global ged_handle; ged_handle=h
  TCL_SetVar("curvis",h.visible)
  TCL_SetVar("curdatamapping",h.data_mapping)
  
  
  d="["+strcat(string(size(h.data.x)),'x')+" double array]"
  TCL_SetVar("curdata_x",d);
  d="["+strcat(string(size(h.data.y)),'x')+" double array]"
  TCL_SetVar("curdata_y",d);
  d="["+strcat(string(size(h.data.z)),'x')+" double array]"
  TCL_SetVar("curdata_z",d);

  TCL_EvalFile(SCI+'/tcl/ged/Grayplot.tcl')

endfunction

function ged_matplot(h)
  global ged_handle; ged_handle=h
  TCL_SetVar("curvis",h.visible)

  d="["+strcat(string(size(h.data)),'x')+" double array]"
  TCL_SetVar("curdata",d);

  TCL_EvalFile(SCI+'/tcl/ged/Matplot.tcl')

endfunction


function ged_axis(h)
  global ged_handle; ged_handle=h
  TCL_SetVar("curvis",h.visible)
  TCL_SetVar("curseg",h.tics_segment)
  TCL_SetVar("curcolor",string(h.tics_color))
  TCL_SetVar("curticsstyle",h.tics_style)
  TCL_SetVar("curfontcolor",string(h.labels_font_color))
  TCL_SetVar("curfontsize",string(h.labels_font_size))
  TCL_SetVar("nbcolX",string(size(h.xtics_coord,2)))
  TCL_SetVar("nbcolY",string(size(h.ytics_coord,2)))
  TCL_SetVar("xticscoord",sci2exp(h.xtics_coord,0))
  TCL_SetVar("yticscoord",sci2exp(h.ytics_coord,0))
  TCL_SetVar("cursubtics",string(h.sub_tics))
  TCL_SetVar("curticslabel",sci2exp(h.tics_labels,0))
  TCL_SetVar("curticsdir",string(h.tics_direction))

  if (h.clip_box==[])
    TCL_SetVar("old_Xclipbox","")
    TCL_SetVar("old_Yclipbox","")
    TCL_SetVar("old_Wclipbox","")
    TCL_SetVar("old_Hclipbox","")
    TCL_SetVar("Xclipbox","")
    TCL_SetVar("Yclipbox","")
    TCL_SetVar("Wclipbox","")
    TCL_SetVar("Hclipbox","")
   else
    TCL_SetVar("old_Xclipbox",string(h.clip_box(1)))
    TCL_SetVar("old_Yclipbox",string(h.clip_box(2)))
    TCL_SetVar("old_Wclipbox",string(h.clip_box(3)))
    TCL_SetVar("old_Hclipbox",string(h.clip_box(4)))
    TCL_SetVar("Xclipbox",string(h.clip_box(1)))
    TCL_SetVar("Yclipbox",string(h.clip_box(2)))
    TCL_SetVar("Wclipbox",string(h.clip_box(3)))
    TCL_SetVar("Hclipbox",string(h.clip_box(4)))
  end
  TCL_SetVar("curclipstate",h.clip_state);

  TCL_EvalFile(SCI+'/tcl/ged/Axis.tcl')
endfunction


function [h,Axes]=ged_getobject(pt)
  h=[];Axes=[];
  f=get("current_figure");
  aold=get("current_axes")
  axes_array=f.children
  // assume that the lastly created objects
  // are at the beginning of the arrays of children
  // We can then select the last object
  // first in the loop.
  for k=1:size(axes_array,'*')
    Axes=axes_array(k)
    set("current_axes",Axes)
    h=ged_loop(Axes,pt)
    if h<>[] then break,end
  end
  set("current_axes",aold)
endfunction

function h=ged_loop(a,pt)

  h=[]
  minDist    = 0.01 ;
  minPixDist = 3    ;
  
  for ka=1:size(a,'*')
    ck=a(ka) ;
    select ck.type
      
      case "Polyline"
	xy=ck.data;
	d=Dist2polyline((xy(:,1)-Xmin)/Dx,(xy(:,2)-Ymin)/Dy,pts)
	if d < minDist then h=ck,return,end
	
      case "Rectangle"
	xy=ck.data;
	x0=xy(1);y0=xy(2);W=xy(3);H=xy(4);
	d=Dist2polyline((x0+[0,W,W,0]-Xmin)/Dx,(y0+[0,0,-H,-H]-Ymin)/Dy,pts)
	if d < minDist then h=ck,return,end
    
      case "Arc" 
	xy=ck.data;
	[xp,yp]=xchange(pt(1),pt(2),'i2f')
	//[dist, toto] = dist2Arc( [xp,yp] ./ [Dx,Dy], xy(1:2)./[Dx,Dy], xy(3)/Dx, xy(4)/Dy, xy(5) / 64., xy(6) / 64. ) ;
	dist = pixDist2Arc( [xp,yp], xy(1:2), xy(3), xy(4), xy(5) / 64., xy(6) / 64. ) ;
	if dist <= minPixDist then 
	  h=ck;
	  return;
	end
      
      case "Segs"
	xy=ck.data;
	xv=(matrix(xy(:,1),2,-1)-Xmin)/Dx
	yv=(matrix(xy(:,2),2,-1)-Ymin)/Dy
	for ks=1:size(xv,2)
	  d=Dist2polyline(xv(:,ks),yv(:,ks),pts)
	  if d < minDist then h=ck,return,end
	end

      case "Compound"
	h=ged_loop(ck.children,pt)
	if h<>[] then return,end
	
      case "Axes"
	xy=ck.data_bounds;
	[xp,yp]=xchange(pt(1),pt(2),'i2f')
	Xmin=xy(1,1);Ymin=xy(1,2),Dx=xy(2,1)-xy(1,1);Dy=xy(2,2)-xy(1,2);
	pts=[(xp-Xmin)/Dx (yp-Ymin)/Dy]
	d=Dist2polyline([0,1,1,0],[0,0,1,1],pts)
	if d < minDist then h=ck,return,end
	h=ged_loop([a.children(:);ck.x_label;ck.y_label;ck.z_label;ck.title],pt)
	if h<>[] then return,end
	
      case "Text"
	if is_in_text(ck,[xp;yp]) then
	  h=ck,
	  return,
	end
	
      case "Label"
	if is_in_text(ck,[xp;yp]) then
	  h=ck
	  return,
	end
	
      end
    end
endfunction
  
function r=is_in_text(h,xy)
  if h.type<>"Text" then r=%f,return,end
  if h.text_box_mode=='filled' then 
    r=(xy(1)>h.data(1)&xy(1)<h.data(1)+h.text_box(1))&(xy(2)>h.data(2)&xy(2)<h.data(2)+h.text_box(2))
  else
    r = stringbox(h);
    r=[r r(:,1)];
    r=and([xy(2) -xy(1)]*diff(r,1,2)+(r(1,1:$-1).*r(2,2:$)-r(1,2:$).*r(2,1:$-1))<0)
  end
endfunction

// compute the square of distance between a point and the ellipse 
// in 2D included in an axis aligned rectangle whose upper left 
// corner is upperLeft and its wifth and heigth is defined.
function [dist] = dist2Ellipse( point, upperLeft, width, heigth )
  width2  = width  / 2. ;
  heigth2 = heigth / 2. ;
  centerC = [ upperLeft(1) + width2, upperLeft(2) - heigth2 ] ; // center of the ellipse
  
  // clicked point in the circle frame
  pointC  = [ (point(1) - centerC(1)) / width2, (point(2) - centerC(2)) / heigth2 ] ;
  
  // get the vector between the point and the closest on the circle
  diffclose = ( 1 - 1 / norm( pointC ) ) * pointC ;
  //closest = pointC / sqrt( pointC(1) * pointC(1) + pointC(2) * pointC(2) ) ;
  
  // get the difference between the two
  //ffclose = pointC - closest ;
  
  // bring it back to the current frame value
  diffclose(1) = diffclose(1) * width2  ;
  diffclose(2) = diffclose(2) * heigth2 ;
  
  // get the distance with the closest point
  dist = norm( diffclose ) ;
  
endfunction

// compute the distance between a point and the arc 
// in 2D included in an axis aligned rectangle whose upper left 
// corner is upperLeft and its wifth and heigth is defined.
function [dist,diffClose] = dist2Arc( point, upperLeft, width, heigth, sector1, sector2 )


  if ( width == 0 | heigth == 0 ) then
    dist = %inf ;
    diffClose = [%inf,%inf];
    return ;
  end
  
  // convert the sector into radiant angle
  angle1 =  sector1            * %pi / 180. ;
  angle2 = (sector1 + sector2) * %pi / 180. ;
  
  width2  = width  / 2. ;
  heigth2 = heigth / 2. ;
  centerC = [ upperLeft(1) + width2, upperLeft(2) - heigth2 ] ; // center of the ellipse
  
  // clicked point in the circle frame
  pointC  = [ (point(1) - centerC(1)) / width2, (point(2) - centerC(2)) / heigth2 ] ;
  
  // get the projection of the clicked point on the circle
  closest = pointC / norm( pointC ) ;
  
  // now a quite tricky part. The aim is to find
  // if the closest point is in the drawing sector
  // ie if it is between bound1 and bound2 on the circle
  // maybe a eayer solution exists.
  
  // get the boundaries of the displayed angle
  // the closest point is not on the arc it is one of the two
  // boundaries
  bound1 = [cos(angle1),sin(angle1)] ;
  bound2 = [cos(angle2),sin(angle2)] ;
  
  // now get the vector of bissecting line between the two bounds
  // with the orientation toward the arc
  b2b1       = bound1 -  bound2 ;
  bissect(1) = -b2b1(2)         ;
  bissect(2) =  b2b1(1)         ;
  
  // get the position of the point along this axis
  side = closest(1) * bissect(1) + closest(2) * bissect(2) ;
  
  // get the position of one of the bound (same value for both)
  boundPos = bound1(1) * bissect(1) + bound1(2) * bissect(2) ;
  
  if side > boundPos  then
    // the closest point is on the arc
    diffClose = ( pointC - closest ) .* [width2,heigth2] ;
    // bring it back to the current frame value
    //diffclose = diffclose .* [width2,heigth2] ;
  
    // get the distance with the closest point
    dist = norm( diffClose ) ;
    
  else
    // the closest point is one of the bounds
    // return back to the real coordinates
    bound1 = centerC + bound1 .* [width2,heigth2];
    bound2 = centerC + bound2 .* [width2,heigth2];
    
    // get the minimum distance
    dist  = norm( bound1 - point ) ;
    dist2 = norm( bound2 - point ) ;
    if dist > dist2 then
      diffClose = bound1 - point ;
    else
      dist = dist2 ;
      diffClose = bound2 - point ;
    end
    //dist = min( norm( bound1 - point ), norm( bound2 - point ) ) ;
  end  
  
endfunction

// same as before but return the value in pixels
function dist = pixDist2Arc( point, upperLeft, width, heigth, sector1, sector2 )
  
  [dist, difference] = dist2Arc( point, upperLeft, width, heigth, sector1, sector2 ) ;
  // convert to pixels
  // get the length of the difference vector
  // we construct it by getting two points
  [origin(1),origin(2)] = xchange(0,0,'f2i');
  [extremity(1),extremity(2)] = xchange(difference(1),difference(2),'f2i');
  dist = norm( extremity - origin ) ;

endfunction

function [d,pt,ind]=Dist2polyline(xp,yp,pt)
// computes minimum distance from a point to a polyline
//d    minimum distance to polyline
//pt   coordinate of the polyline closest point
//ind  
//     if negative polyline closest point is a polyline corner:pt=[xp(-ind) yp(-ind)]
//     if positive pt lies on segment [ind ind+1]

// Copyright INRIA
x=pt(1)
y=pt(2)
xp=xp(:);yp=yp(:)
cr=4*sign((xp(1:$-1)-x).*(xp(1:$-1)-xp(2:$))+..
          (yp(1:$-1)-y).*(yp(1:$-1)-yp(2:$)))+..
    sign((xp(2:$)-x).*(xp(2:$)-xp(1:$-1))+..
          (yp(2:$)-y).*(yp(2:$)-yp(1:$-1)))

ki=find(cr==5) // index of segments for which projection fall inside
np=size(xp,'*')
if ki<>[] then
  //projection on segments
  x=[xp(ki) xp(ki+1)]
  y=[yp(ki) yp(ki+1)]
  dx=x(:,2)-x(:,1)
  dy=y(:,2)-y(:,1)
  d_d=dx.^2+dy.^2
  d_x=( dy.*(-x(:,2).*y(:,1)+x(:,1).*y(:,2))+dx.*(dx*pt(1)+dy*pt(2)))./d_d
  d_y=(-dx.*(-x(:,2).*y(:,1)+x(:,1).*y(:,2))+dy.*(dx*pt(1)+dy*pt(2)))./d_d
  xp=[xp;d_x]
  yp=[yp;d_y]
end
[d,k]=min(((xp-pt(1))).^2+((yp-pt(2))).^2) //distance with all points
d=sqrt(d)
pt(1)=xp(k)
pt(2)=yp(k)
if k>np then ind=ki(k-np),else ind=-k,end
endfunction

function ged_eventhandler(win,x,y,ibut)
//Copyright INRIA
//Author : Serge Steer 2002
  if ibut==-1 then return,end //ignore move
  if and(win<>winsid())| ibut==-1000 then 
    //window has been deleted by the user
    return
  end
  cur=gcf();scf(win) //make the window associated to the event active
  //disable the event handler not to execute new event before finishing this one
  seteventhandler("")  ; 

  global ged_handle;ged_handle=[]
  ged_handle=ged_getobject([x,y]);
  if ged_handle~=[] then //an object has been selected
    if or(ibut==[0 3 10]) then //left button --> edit properties
      tkged()
    elseif or(ibut==[2 5 12]) then //right button -->move
      [x,y]=xchange(x,y,'i2f')
      pos=[x,y]
      while %t then
	rep=xgetmouse(0,[%t %t])
	if rep(3)>0 then break,end
	
	move(ged_handle,rep(1:2)-pos)
	show_pixmap()
	pos=rep(1:2)
      end

    end
  end
  seteventhandler("ged_eventhandler") //enable the handler
  scf(cur) //reset current window
endfunction

 
function [ini,typs]=build_args(labels)
  n=size(labels,'*')
  ini=[]
  typs=list()
  for k=1:n
    typ=type(h(labels(k)))
    execstr(['if typ==10 then'
	     '   w=h.'+labels(k)
	     'else'
	     '   w=sci2exp(h.'+labels(k)+',0)'
	     'end'])
    ini=[ini;w]
    if typ==10 then
      typs($+1)="str";typs($+1)=-1
    else
      typs($+1)="vec";typs($+1)=1
    end
  end
endfunction

function GetSetValue(h)
  n=size(labels,'*')
  lhs='[ok,'+strcat('x'+string(1:n),',')+']'
  execstr(lhs+'=getvalue(t,labels,typs,ini)')
  if ok then
    for k=1:n
      execstr("if x'+string(k)+'<>h."+labels(k)+' then h.'+..
	      labels(k)+'=x'+string(k)+',end')
    end
  end
endfunction



function tkged()
global ged_handle
global ged_cur_fig_handle

h=ged_handle

// hierarchical viewer
TK_send_handles_list(ged_cur_fig_handle)
TCL_SetVar("curgedindex",string(Get_handle_pos_in_list(h)))

//color_map array for color sample display
f=getparfig(h);
for i=1:size(f.color_map,1)
  redname= "RED("+string(i)+")";
  TCL_EvalStr('set '+redname+" "+string(f.color_map(i,1)));
  grename= "GREEN("+string(i)+")";
  TCL_EvalStr('set '+grename+" "+string(f.color_map(i,2)));
  bluname= "BLUE("+string(i)+")";
  TCL_EvalStr('set '+bluname+" "+string(f.color_map(i,3)));
end

TCL_SetVar("msdos",string(MSDOS)) // to know the OS
TCL_SetVar("sciGedIsAlive","Alive") ; // to know wether ged is opened or not

select h.type
case "Polyline"
  ged_polyline(h)
case "Rectangle"
  ged_rectangle(h)
case "Axes"
  ged_axes(h)
case "Label" // for now the labels are inside the axes (F.Leray 06.12.05)
  ged_axes(h.parent)
  if (h == h.parent.x_label)
    TCL_EvalStr("Notebook:raise $uf.n X");
  elseif (h == h.parent.y_label)
    TCL_EvalStr("Notebook:raise $uf.n Y");
  elseif (h == h.parent.z_label)
    TCL_EvalStr("Notebook:raise $uf.n Z");
  elseif (h == h.parent.title)
    TCL_EvalStr("Notebook:raise $uf.n Title");
  end
case "Figure"
  ged_figure(h)
case "Compound"
  ged_Compound(h)
  
case "Plot3d"
  ged_plot3d(h)
case "Fac3d"
  ged_fac3d(h)
case "Text"
  ged_text(h)
case "Legend"
ged_legend(h)
case "Arc"
  ged_arc(h)
case "Segs"
  ged_segs(h)    
case "Champ"
  ged_champ(h)
case "Fec"
  ged_fec(h)
case "Grayplot"
  ged_grayplot(h)
case "Matplot"
  ged_matplot(h)
case "Axis"
  ged_axis(h)
end
endfunction
  
function setStyle(sty)
  global ged_handle; h=ged_handle
  h.polyline_style=find(sty==['interpolated','staircase', ...
		    'barplot','arrowed','filled' 'bar'])
endfunction
function setLineStyle(sty)
  global ged_handle; h=ged_handle
  h.line_style=find(sty==[ "solid" "dash" "dash dot" "longdash dot" ..
		    "bigdash dot" "bigdash longdash"])
  
endfunction
function setMarkStyle(sty)
  global ged_handle; h=ged_handle
  h.mark_style=find(sty==["dot" "plus" "cross" "star" "filled diamond" ..
  "diamond" "triangle up" "triangle down" "diamond plus" "circle" ..
  "asterisk" "square" "triangle right" "triangle left" "pentagram"])-1
endfunction
function setFontStyle(ftn)
  global ged_handle; h=ged_handle
  h.font_style=find(ftn==["Courier" "Symbol" "Times",..
		    "Times Italic" "Times Bold" "Times Bold Italic",..
		   "Helvetica"  "Helvetica Italic" "Helvetica Bold",..
		   "Helvetica Bold Italic"])-1
endfunction

function setLabelsFontStyle(label,ftn)
   global ged_handle; h=ged_handle
select label
case "t"
  h.title.font_style=find(ftn==["Courier" "Symbol" "Times",..
		    "Times Italic" "Times Bold" "Times Bold Italic",..
		   "Helvetica"  "Helvetica Italic" "Helvetica Bold",..
		   "Helvetica Bold Italic"])-1
case "x"
  h.x_label.font_style=find(ftn==["Courier" "Symbol" "Times",..
		    "Times Italic" "Times Bold" "Times Bold Italic",..
		   "Helvetica"  "Helvetica Italic" "Helvetica Bold",..
		   "Helvetica Bold Italic"])-1
case "y"
  h.y_label.font_style=find(ftn==["Courier" "Symbol" "Times",..
		    "Times Italic" "Times Bold" "Times Bold Italic",..
		   "Helvetica"  "Helvetica Italic" "Helvetica Bold",..
		   "Helvetica Bold Italic"])-1
case "z"
  h.z_label.font_style=find(ftn==["Courier" "Symbol" "Times",..
		    "Times Italic" "Times Bold" "Times Bold Italic",..
		   "Helvetica"  "Helvetica Italic" "Helvetica Bold",..
		   "Helvetica Bold Italic"])-1
end;
endfunction


// Axes data (data_bounds)
function setXdb(xmin, xmax)
  global ged_handle; h=ged_handle
  tmp=h.data_bounds;
  tmp(1,1)=xmin;
  tmp(2,1)=xmax;
  h.data_bounds=tmp;
//  tst=execstr('h.data_bounds=tmp','errcatch','n');
//  if tst<>0 then
//   disp 'Warning: X data_bounds must contain double'
//  end
endfunction


function setYdb(ymin, ymax)
  global ged_handle; h=ged_handle
  tmp=h.data_bounds;
  tmp(1,2)=ymin;
  tmp(2,2)=ymax;
  h.data_bounds=tmp;
//  tst=execstr('h.data_bounds=tmp','errcatch','n');
//  if tst<>0 then
//   disp 'Warning: Y data_bounds must contain double'
//  end
endfunction

function setZdb(zmin, zmax)
  global ged_handle; h=ged_handle
 select h.view
     case "2d"
      drawlater();
      h.view='3d';
      tmp=h.data_bounds;
      tmp(1,3)=zmin;
      tmp(2,3)=zmax;
      h.data_bounds=tmp;
      h.view='2d';
      drawnow();
//      tst=execstr('h.data_bounds=tmp','errcatch','n');
//      h.view='2d';
//      if tst<>0 then
//       disp 'Warning: Z data_bounds must contain double'
//      end
     case "3d"
      tmp=h.data_bounds;
      tmp(1,3)=zmin;
      tmp(2,3)=zmax;
      h.data_bounds=tmp;
//      tst=execstr('h.data_bounds=tmp','errcatch','n');
//      if tst<>0 then
//        disp 'Warning: Z data_bounds must contain double'
//      end
     end
endfunction


function LogtoggleX( tog)
 global ged_handle; h=ged_handle

 h.log_flags=tog+part(h.log_flags,2);


//tst=execstr("global h;h.log_flags=tog+part(h.log_flags,2)",'errcatch','n');

//if tst<>0 then
//   disp 'Warning: X bounds must be strictly positive'
//end
endfunction

function LogtoggleY( tog)
 global ged_handle; h=ged_handle

 h.log_flags=part(h.log_flags,1)+tog;


//tst=execstr("global h;h.log_flags=part(h.log_flags,1)+tog",'errcatch','n');

//if tst<>0 then
//   disp 'Warning: Y bounds must be strictly positive'
//end

endfunction


function LogtoggleZ( tog)
 global ged_handle; h=ged_handle

 h.log_flags=part(h.log_flags,1)+part(h.log_flags,2)+tog;


//tst=execstr("global h;h.log_flags=part(h.log_flags,1)+tog",'errcatch','n');

//if tst<>0 then
//   disp 'Warning: Z bounds must be strictly positive'
//end

endfunction



function [haxe] = getparaxe( h )

htmp = h;
haxe = [];
while htmp.type<>'Axes' do
  htmp = htmp.parent
end

haxe = htmp;
endfunction




// Rectangle data
function setXval(val)
  global ged_handle; h=ged_handle
  tmp=h.data;
  tmp(1)=val;
  tst=execstr('h.data=tmp','errcatch','n');
  if tst<>0 then
   disp 'Warning: Y data must contain double'
  end
endfunction

function setYval(val)
  global ged_handle; h=ged_handle
  tmp=h.data;
  tmp(2)=val;
  tst=execstr('h.data=tmp','errcatch','n');
  if tst<>0 then
   disp 'Warning: Y data must contain double'
  end
endfunction

function setZval(val)
  global ged_handle; h=ged_handle
  ax=getparaxe(h);
 select ax.view
     case "2d"
      drawlater();
      ax.view='3d';
      tmp=h.data;
      tmp(3)=val;
      tst=execstr('h.data=tmp','errcatch','n');
      ax.view='2d';
      drawnow();
      if tst<>0 then
       disp 'Warning: Z data must contain double'
      end
     case "3d"
      tmp=h.data;
      tmp(3)=val;
      tst=execstr('h.data=tmp','errcatch','n');
      if tst<>0 then
        disp 'Warning: Z data must contain double'
      end
     end
endfunction


function setWval(val)
  global ged_handle; h=ged_handle
 ax=getparaxe(h);
 select ax.view
     case "2d"
      drawlater();
      ax.view='3d';
      tmp=h.data;
      tmp(4)=val;
      tst=execstr('h.data=tmp','errcatch','n');
      ax.view='2d';
      drawnow();
      if tst<>0 then
       disp 'Warning: Width data must contain double'
      end
     case "3d"
      tmp=h.data;
      tmp(4)=val;
      tst=execstr('h.data=tmp','errcatch','n');
      if tst<>0 then
        disp 'Warning: Width data must contain double'
      end
     end
endfunction


function setHval(val)
  global ged_handle; h=ged_handle
 ax=getparaxe(h);
 select ax.view
     case "2d"
      drawlater();
      ax.view='3d';
      tmp=h.data;
      tmp(5)=val;
      tst=execstr('h.data=tmp','errcatch','n');
      ax.view='2d';
      drawnow();
      if tst<>0 then
       disp 'Warning: Height data must contain double'
      end
     case "3d"
      tmp=h.data;
      tmp(5)=val;
      tst=execstr('h.data=tmp','errcatch','n');
      if tst<>0 then
        disp 'Warning: Height data must contain double'
      end
     end
endfunction


// complement for Arc entity
function setA1val(val)
  global ged_handle; h=ged_handle
 ax=getparaxe(h);
 select ax.view
     case "2d"
      drawlater();
      ax.view='3d';
      tmp=h.data;
      tmp(6)=val;
      tst=execstr('h.data=tmp','errcatch','n');
      ax.view='2d';
      drawnow();
      if tst<>0 then
       disp 'Warning: Width data must contain double'
      end
     case "3d"
      tmp=h.data;
      tmp(6)=val;
      tst=execstr('h.data=tmp','errcatch','n');
      if tst<>0 then
        disp 'Warning: a1 data must contain double'
      end
     end
endfunction

function setA2val(val)
  global ged_handle; h=ged_handle
 ax=getparaxe(h);
 select ax.view
     case "2d"
      drawlater();
      ax.view='3d';
      tmp=h.data;
      tmp(7)=val;
      tst=execstr('h.data=tmp','errcatch','n');
      ax.view='2d';
      drawnow();
      if tst<>0 then
       disp 'Warning: Width data must contain double'
      end
     case "3d"
      tmp=h.data;
      tmp(7)=val;
      tst=execstr('h.data=tmp','errcatch','n');
      if tst<>0 then
        disp 'Warning: a2 data must contain double'
      end
     end
endfunction


//Fec
function setZb(min, max)
  global ged_handle; h=ged_handle
  tmp=h.z_bounds;
  tmp(1,1)=min;
  tmp(1,2)=max;
  tst=execstr('h.z_bounds=tmp','errcatch','n');
  if tst<>0 then
   disp 'Warning: X data_bounds must contain double'
  end
endfunction



////////////////////////////////
// Scilab Command Interface ////
////////////////////////////////
//Plot3d
function set3dtlistXYZ (X,Y,Z)
  global ged_handle; h=ged_handle
  ged_tmp_tlist = tlist(["3d","x","y","z"],X,Y,Z)
  h.data=ged_tmp_tlist;
  clear ged_tmp_tlist;
endfunction

function set3dtlistXYZC (X,Y,Z,COLOR)
  global ged_handle; h=ged_handle
  ged_tmp_tlist = tlist(["3d","x","y","z","color"],X,Y,Z,COLOR)
  h.data=ged_tmp_tlist;
  clear ged_tmp_tlist;
endfunction

function setGrayplottlist(X,Y,Z)
  global ged_handle; h=ged_handle
  ged_tmp_tlist = tlist(["grayplotdata","x","y","z"],X,Y,Z)
  h.data=ged_tmp_tlist;
  clear ged_tmp_tlist;
endfunction

//Champ
function setchamptlistXYFXFY (X,Y,FX,FY)
  global ged_handle; h=ged_handle
  ged_tmp_tlist = tlist(["champdata","x","y","fx","fy"],X,Y,FX,FY)
  h.data=ged_tmp_tlist;
  clear ged_tmp_tlist;
endfunction


///////////////////////////////
// Edit Data using sciGUI /////
///////////////////////////////

function EditData(TheData,datastring)
// TheData must be a real scalar or matrix
global ged_handle; h=ged_handle

// I declare ged_tmp ged_tmp_string WINDOW as global
global ged_tmp;
global ged_tmp_string;
global WINDOW;

ged_tmp_string = datastring;

ged_tmp=TheData;

WINDOW = GEDeditvar("ged_tmp")

endfunction


function CloseEditorSaveData()
// Called when closing data editor

global ged_handle; // To leave here because used when doing execstr(ged_tmp_string...
global ged_tmp;
global ged_tmp_string;
global WINDOW;

Nan = %nan // to avoid error message because of special Nan display

ged_tmp=GEDeditvar_get(WINDOW);

execstr(ged_tmp_string+"= ged_tmp");

clearglobal ged_tmp ged_tmp_string WINDOW
clear ged_tmp ged_tmp_string WINDOW

endfunction


function outvar=GEDeditvar_get(winId)
// Simple variable editor - import data
// This file is part of sciGUI toolbox
// Copyright (C) 2004 Jaime Urzua Grez
// mailto:jaime_urzua@yahoo.com
// rev. 0.1
  //
// This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.

//disp("Please wait...");
outvar=[];
base="sciGUITable(win,"+string(winId)+",data";
varType=evstr(TCL_GetVar(base+',type)'));
varni=evstr(TCL_GetVar(base+',ni)'));
varnj=evstr(TCL_GetVar(base+',nj)'));
for j=1:varnj,
	ww=[];
	for i=1:varni,
		q=TCL_GetVar(base+','+string(i)+','+string(j)+')');
		if (varType~=10) then
			if (varType==4) then
				if ((q=="T")|(q=="t")) then
					ww=[ww;%t];
				else
					ww=[ww;%f];
				end
			else
				ww=[ww;evstr(q)];
			end
		else
			ww=[ww;q];
		end
	end
	outvar=[outvar ww];
end
endfunction

function [WINID] = GEDeditvar(varargin)
// Simple Variable Editor
// This file is part of sciGUI toolbox
// Copyright (C) 2004 Jaime Urzua Grez 
// mailto:jaime_urzua@yahoo.com
// rev. 0.2 2004/06/24
//
// This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.
  
  sciGUI_init()
  
  [%_nams]=who('get');
  %_loc_type=type(varargin(1))
  if (%_loc_type~=10) then error(42), end
  %_in_list=find(%_nams==varargin(1));
  if (%_in_list==[]) then error(42), end
  execstr("%_loc_var="+varargin(1));
  %_loc_type=type(%_loc_var);
  %_allo=find([1;4;10]==%_loc_type);
  if (%_allo==[]) then error(42), end

  //disp("Please wait...");
  %_loc_nam=varargin(1);
  %_loc_ni=size(%_loc_var,1);
  %_loc_nj=size(%_loc_var,2);
  %_loc_type=string(%_loc_type);

  TCL_EvalStr("set EdVarLoc [GEDsciGUIEditVar -1]");

  %_winId=TCL_GetVar("EdVarLoc");

  TCL_SetVar("sciGUITable(win,"+%_winId+",data,name)",%_loc_nam);
  TCL_SetVar("sciGUITable(win,"+%_winId+",data,type)",string(%_loc_type));
  TCL_SetVar("sciGUITable(win,"+%_winId+",data,ni)",string(%_loc_ni));
  TCL_SetVar("sciGUITable(win,"+%_winId+",data,nj)",string(%_loc_nj));

  //
  // This is a slow manner to move the data ...
  // Anyone have a faster method than TCL_SetVar
  // I would like set some tcl variable like varname(pos_i,pos_j)
  //
  Nb_data=(%_loc_nj)*(%_loc_ni)
 // disp("Nb_data=")
 // disp(Nb_data);
 // winWB=waitbar('Loading data...');
 // tmp = 0;
 // waitbar(tmp,winWB);
 

  for %_j=1:%_loc_nj,
    for %_i=1:%_loc_ni,
      %_value=string(%_loc_var(%_i,%_j));
      %_varname="sciGUITable(win,"+%_winId+",data,"+string(%_i)+","+string(%_j)+")";
      TCL_SetVar(%_varname,%_value);
    end

  //tmp = ((%_j)*(%_loc_ni)) / Nb_data;
  //tmp = tmp *100; tmp = int(tmp); tmp = tmp /100;
  //disp("tmp dans for=")
  //disp(tmp)
  //waitbar(tmp,winWB);
  end


 // disp("%_winId=");
 // disp(%_winId);
 // disp("type(%_winId)=");
 // disp(type(%_winId));
 // disp("winWB=");
 // disp(winWB);
 // disp("type(winWB)=");
 // disp(type(winWB));
 // disp("AVANT DrawGrid");

 
  TCL_EvalStr("GEDsciGUIEditVarDrawGrid "+%_winId)
 
  WINID = %_winId;

endfunction


// for the ticks arrays Locations and Labels (inside TLIST)

function ged_tablo=GetTab(val,index)
//disp("ICI")
ged_tablo(index) =val
endfunction


function ged_tablo=GetTab2(val,index,ged_tablo)
//disp("ICI")
ged_tablo(index) =val
endfunction


function setTicksTList(XYZ,locations,labels)
global ged_handle;h= ged_handle;

TL=tlist(['ticks','locations','labels'],locations,labels);

if XYZ=='X'
 h.x_ticks=TL;
elseif XYZ=='Y'
 h.y_ticks=TL;
elseif XYZ=='Z'
 h.z_ticks=TL;
end

endfunction


// Is called by ged_axes 

function LoadTicks2TCL(h)
global ged_handle;ged_handle=h;

TCL_SetVar("Xaxes_visibleToggle",h.axes_visible(1))
TCL_SetVar("Yaxes_visibleToggle",h.axes_visible(2))
TCL_SetVar("Zaxes_visibleToggle",h.axes_visible(3))

TCL_SetVar("SubticksEntryX",string(h.sub_ticks(1)))
TCL_GetVar("SubticksEntryX")

// disp("h.sub_ticks(1) =")
// disp(h.sub_ticks(1));

TCL_SetVar("SubticksEntryY",string(h.sub_ticks(2)))

select h.view
case "2d" 
  drawlater(); // postpon the drawings due to switching from 2d to 3d mode (for example)
  // in order to know the complete data set (z data for axes...)
  h.view='3d'
  TCL_SetVar("SubticksEntryZ",string(h.sub_ticks(3)))
  h.view='2d'
  drawnow();
case "3d"
  TCL_SetVar("SubticksEntryZ",string(h.sub_ticks(3)))
end

TCL_SetVar("XautoticksToggle",h.auto_ticks(1))
TCL_SetVar("YautoticksToggle",h.auto_ticks(2))
TCL_SetVar("ZautoticksToggle",h.auto_ticks(3))

//ticks value: X axis
ticks = h.x_ticks;
sizeticks = size(ticks.locations,1);
TCL_SetVar("nbticks_x",string(sizeticks));
for i=1:sizeticks
  val= "LOCATIONS_X("+string(i)+")";
  TCL_EvalStr('set '+val+" "+string(ticks.locations(i)));
  val= "LABELS_X("+string(i)+")";
  TCL_EvalStr('set '+val+" {"+ticks.labels(i)+"}");
end

//ticks value: Y axis
ticks = h.y_ticks;
sizeticks = size(ticks.locations,1);
TCL_SetVar("nbticks_y",string(sizeticks));
for i=1:sizeticks
  val= "LOCATIONS_Y("+string(i)+")";
  TCL_EvalStr('set '+val+" "+string(ticks.locations(i)));
  val= "LABELS_Y("+string(i)+")";
  TCL_EvalStr('set '+val+" {"+ticks.labels(i)+"}");
end

//ticks value: Z axis
ticks = h.z_ticks;
sizeticks = size(ticks.locations,1);
TCL_SetVar("nbticks_z",string(sizeticks));
for i=1:sizeticks
  val= "LOCATIONS_Z("+string(i)+")";
  TCL_EvalStr('set '+val+" "+string(ticks.locations(i)));
  val= "LABELS_Z("+string(i)+")";
  TCL_EvalStr('set '+val+" {"+ticks.labels(i)+"}");
end

endfunction



function Subtickstoggle( tog, index)
 global ged_handle; h=ged_handle

 subticks=h.sub_ticks;
 subticks(index)=tog;
//disp("subticks vaut:")
//disp(subticks)

 h.sub_ticks = subticks;
endfunction


// when clicking on Ticks button : REload ticks is called

function ReLoadTicks2TCL(h)
  global ged_handle;ged_handle=h;

  TCL_SetVar("Xaxes_visibleToggle",h.axes_visible(1))
  TCL_SetVar("Yaxes_visibleToggle",h.axes_visible(2))
  TCL_SetVar("Zaxes_visibleToggle",h.axes_visible(3))

  ticks = h.x_ticks;
  sizeticks = size(ticks.locations,1);
  if (sizeticks <> 0)
   TCL_EvalStr("unset LOCATIONS_X");
   TCL_EvalStr("unset LABELS_X");
  end
  
  ticks = h.y_ticks;
  sizeticks = size(ticks.locations,1);
  if (sizeticks <> 0)
   TCL_EvalStr("unset LOCATIONS_Y");
   TCL_EvalStr("unset LABELS_Y");
  end
  
  ticks = h.z_ticks;
  sizeticks = size(ticks.locations,1);
  if (sizeticks <> 0)
   TCL_EvalStr("unset LOCATIONS_Z");
   TCL_EvalStr("unset LABELS_Z");
  end
  
  TCL_SetVar("SubticksEntryX",string(h.sub_ticks(1)))
//  TCL_GetVar("SubticksEntryX")
 
 // disp("h.sub_ticks(1) =")
 // disp(h.sub_ticks(1));

  TCL_SetVar("SubticksEntryY",string(h.sub_ticks(2)))

  select h.view
   case "2d" 
    drawlater();
    h.view='3d'
    TCL_SetVar("SubticksEntryZ",string(h.sub_ticks(3)))
    h.view='2d'
    drawnow();
   case "3d"
     TCL_SetVar("SubticksEntryZ",string(h.sub_ticks(3)))
   end

  TCL_SetVar("XautoticksToggle",h.auto_ticks(1))
  TCL_SetVar("YautoticksToggle",h.auto_ticks(2))
  TCL_SetVar("ZautoticksToggle",h.auto_ticks(3))

  //ticks value: X axis
  ticks = h.x_ticks;
  sizeticks = size(ticks.locations,1);
  TCL_SetVar("nbticks_x",string(sizeticks));
  for i=1:sizeticks
    val= "LOCATIONS_X("+string(i)+")";
      TCL_EvalStr('set '+val+" "+string(ticks.locations(i)));
//      disp("i vaut:");
//      disp(i);
    val= "LABELS_X("+string(i)+")";
      TCL_EvalStr('set '+val+" {"+ticks.labels(i)+"}");
  end

  //ticks value: Y axis
  ticks = h.y_ticks;
  sizeticks = size(ticks.locations,1);
  TCL_SetVar("nbticks_y",string(sizeticks));
  for i=1:sizeticks
    val= "LOCATIONS_Y("+string(i)+")";
      TCL_EvalStr('set '+val+" "+string(ticks.locations(i)));
     val= "LABELS_Y("+string(i)+")";
      TCL_EvalStr('set '+val+" {"+ticks.labels(i)+"}");
  end

 //ticks value: Z axis
  ticks = h.z_ticks;
  sizeticks = size(ticks.locations,1);
  TCL_SetVar("nbticks_z",string(sizeticks));
  for i=1:sizeticks
    val= "LOCATIONS_Z("+string(i)+")";
      TCL_EvalStr('set '+val+" "+string(ticks.locations(i)));
    val= "LABELS_Z("+string(i)+")";
      TCL_EvalStr('set '+val+" {"+ticks.labels(i)+"}");
  end

endfunction



function DestroyGlobals()
global ged_current_figure

if find(ged_current_figure==winsid()) then
  xset('window',ged_current_figure)
end 

// ged is closed
TCL_UnsetVar("sciGedIsAlive");

clearglobal ged_current_figure
clear ged_current_figure

// disp("PASSE PAR DestroyGlobals Scilab");
endfunction


function ged_delete_entity()
  [btn,xc,yc]=xclick()
   [xc,yc]=xchange(xc,yc,'f2i')
  h=ged_getobject([xc,yc])
  if h<>[] then delete(h),end
endfunction

function ged_move_entity()
  [btn,xc,yc]=xclick()
  pos=[xc,yc]
  [xc,yc]=xchange(xc,yc,'f2i')
  [r,ax]=ged_getobject([xc,yc])
  cur_ax=gca(),sca(ax)
  [xc,yc]=xchange(xc,yc,'i2f');pos=[xc,yc]
  if r==[] return,end
  f=gcf();pix=f.pixmap;f.pixmap='on'
  rep(3)=-1
  select r.type
  case 'Rectangle' then
    while rep(3)==-1 do
      rep=xgetmouse(0,[%t %t])
      r.data(1:2)= r.data(1:2)+(rep(1:2)-pos)
      pos=rep(1:2)
      show_pixmap()
    end 
  case 'Segs' then //Segment
    while rep(3)==-1 do
      rep=xgetmouse(0,[%t %t])
      r.data=r.data+ones(2,1)*(rep(1:2)-pos)
      pos=rep(1:2)
      show_pixmap()
    end 
  case 'Polyline' then //Polyline
    while rep(3)==-1 do
      rep=xgetmouse(0,[%t %t])
      r.data(:,1:2)=r.data(:,1:2)+ones(r.data(:,1))*(rep(1:2)-pos)
      pos=rep(1:2)
      show_pixmap()
    end 
   case 'Arc' then //Circle
    while rep(3)==-1 do
      rep=xgetmouse(0,[%t %t])
      r.data(1:2)= r.data(1:2)+(rep(1:2)-pos)
      pos=rep(1:2)
      show_pixmap()
    end 
  case 'Text' then
    while rep(3)==-1 do
      rep=xgetmouse(0,[%t %t])
      r.data= r.data+(rep(1:2)-pos)
      pos=rep(1:2)
      show_pixmap()
    end 
  case 'Label' then
    while rep(3)==-1 do
      rep=xgetmouse(0,[%t %t])
      r.position= r.position+(rep(1:2)-pos)
      r.auto_position = "off"
      pos=rep(1:2)
      show_pixmap()
    end 
    
  end
  sca(cur_ax)
  f.pixmap=stripblanks(pix)
endfunction

function ged_copy_entity()
  [btn,xc,yc]=xclick()
  [xc,yc]=xchange(xc,yc,'f2i')
  r=ged_getobject([xc,yc])
  if r==[] return,end
  twinkle(r,1);	
  save(TMPDIR+'/G_Clipboard',r)
  //make the axes containning the clicked point the current one
  sca(ged_select_axes(xc,yc))
endfunction


function ged_paste_entity()
  
  // check the file
  [info,err] = fileinfo(TMPDIR + '/G_Clipboard' ) ;
  
  if err <> 0 then
    return ;
  end
  
  // create the saved object
  load(TMPDIR+'/G_Clipboard') ;

//  a=gca();b=a.data_bounds;
//  move(r,[-1 1]*a.data_bounds/20)
endfunction

function axes =  ged_select_axes(x,y)
// x and y are pixel coord.
f=gcf()
nb_axes = size(f.children,'*') // for now Iconsider that children of a figure are of type Axes
axes_size = f.axes_size // given in pixels
axes_size = [axes_size axes_size];

for i=1:nb_axes
  axes = f.children(i);
  cur_axes_bounds = axes.axes_bounds;
  rect = cur_axes_bounds.*axes_size; // rectangle in pixels (margins inside)
  
  rect(3) = rect(3) + rect(1);
  rect(4) = rect(4) + rect(2);
  if (x>rect(1) & x<rect(3) & y>rect(2) & y<rect(4)) then
     return 
  end
end
axes=[]
endfunction
