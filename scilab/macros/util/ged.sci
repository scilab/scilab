function ged(k,win)
  //xset, xget used because ged should handle both old and new style
  cur=xget('window')
  xset('window',win) 
  isold=get('figure_style')=='old'
  if isold&k>3 then 
    message('this menu does not apply to old style graphics')
    xset('window',cur)
    return
  end
  if k>3 then
    TK_EvalStr("set isgedinterp [interp exists ged]")
    if TK_GetVar("isgedinterp")=='0' then    
      TK_EvalStr("interp create ged")
      TK_EvalStr("load {'+gettklib()+'} Tk ged")
      TK_EvalStr("ged eval {wm withdraw .}")
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
    TK_send_handles_list(gcf())
    TK_SetVar("curgedindex",string(Get_handle_pos_in_list(gcf())))

    //color_map array for color sample display
    f=gcf();
    for i=1:size(f.color_map,1)
      redname= "RED("+string(i)+")";
      TK_EvalStr('set '+redname+" "+string(f.color_map(i,1)));
      grename= "GREEN("+string(i)+")";
      TK_EvalStr('set '+grename+" "+string(f.color_map(i,2)));
      bluname= "BLUE("+string(i)+")";
      TK_EvalStr('set '+bluname+" "+string(f.color_map(i,3)));
    end
    
    //ged_fontarray = ["Courier" "Symbol" "Times" "Times Italic" "Times Bold" "Times Bold Italic"  "Helvetica"  "Helvetica Italic" "Helvetica Bold" "Helvetica Bold Italic"];

    ged_figure(gcf())
    case 5 then //edit current axes
    // hierarchical viewer
    TK_send_handles_list(gcf())
    TK_SetVar("curgedindex",string(Get_handle_pos_in_list(gca())))

    //color_map array for color sample display
    f=gcf();
    for i=1:size(f.color_map,1)
      redname= "RED("+string(i)+")";
      TK_EvalStr('set '+redname+" "+string(f.color_map(i,1)));
      grename= "GREEN("+string(i)+")";
      TK_EvalStr('set '+grename+" "+string(f.color_map(i,2)));
      bluname= "BLUE("+string(i)+")";
      TK_EvalStr('set '+bluname+" "+string(f.color_map(i,3)));
    end

    ged_axes(gca())
    case 6 then //start Entity picker
    seteventhandler("ged_eventhandler")
    case 7 then //stop Entity picker
    seteventhandler("")
  end
  xset('window',cur)
endfunction


function curgedindex = Get_handle_pos_in_list(h)
handles = Get_handles_list(gcf())
 for i=1:size(handles,1)
  if(h==handles(i))
   curgedindex = i;
  end
 end
endfunction





function TK_send_handles_list(h)
iFig = 0;
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
iAxi = 0; // axis : entity created when using drawaxis method for example

f=getparfig(h);
handles = Get_handles_list(f)

TK_SetVar("ged_handle_list_size",string(size(handles,1)));

for i=1:size(handles,1)
 SelObject="SELOBJECT("+string(i)+")";
 hand = handles(i);
 select  hand.type
   case "Figure"
    iFig = iFig+1;
    figname= "Figure("+string(iFig)+")";
    TK_EvalStr('set '+SelObject+" "+figname);
   case "Axes"
    iAxe = iAxe+1;
    axename= "Axes("+string(iAxe)+")";
    TK_EvalStr('set '+SelObject+" "+axename);
   case "Agregation"
    iAgr = iAgr+1;
    agrname= "Agregation("+string(iAgr)+")";
    TK_EvalStr('set '+SelObject+" "+agrname);
   case "Polyline"
    iPol = iPol+1;
    polname= "Polyline("+string(iPol)+")";
    TK_EvalStr('set '+SelObject+" "+polname);
   case "Plot3d"
    iPl3 = iPl3+1;
    pl3name= "Plot3d("+string(iPl3)+")";
    TK_EvalStr('set '+SelObject+" "+pl3name);
   case "Fac3d"
    iFac = iFac+1;
    Facname= "Fac3d("+string(iFac)+")";
    TK_EvalStr('set '+SelObject+" "+Facname);
   case "Rectangle"
    iRec = iRec+1;
    Recname= "Rectangle("+string(iRec)+")";
    TK_EvalStr('set '+SelObject+" "+Recname);
   case "Text"
    iTex = iTex+1;
    Texname= "Text("+string(iTex)+")";
    TK_EvalStr('set '+SelObject+" "+Texname);
   case "Legend"
    iLeg = iLeg+1;
    Legname= "Legend("+string(iLeg)+")";
    TK_EvalStr('set '+SelObject+" "+Legname);
   case "Arc"
    iArc = iArc+1;
    Arcname= "Arc("+string(iArc)+")";
    TK_EvalStr('set '+SelObject+" "+Arcname);
   case "Segs"
    iSeg = iSeg+1;
    Segname= "Segs("+string(iSeg)+")";
    TK_EvalStr('set '+SelObject+" "+Segname);
   case "Champ"
    iCha = iCha+1;
    Chaname= "Champ("+string(iCha)+")";
    TK_EvalStr('set '+SelObject+" "+Chaname);
   case "Fec"
    iFec = iFec+1;
    Fecname= "Fec("+string(iFec)+")";
    TK_EvalStr('set '+SelObject+" "+Fecname);
   case "Grayplot"
    iGra = iGra+1;
    Graname= "Grayplot("+string(iGra)+")";
    TK_EvalStr('set '+SelObject+" "+Graname);
   case "Axis"
    iAxi = iAxi+1;
    Axiname= "Axis("+string(iAxi)+")";
    TK_EvalStr('set '+SelObject+" "+Axiname);
  end
end
endfunction


//function h=Get_handle_from_index(index)
function Get_handle_from_index(index)
   global ged_handle;
   hl = Get_handles_list(gcf());

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

psonstmp = h.children(1);
//pause
while((psonstmp <>[]) & ((i) <=size(psonstmp.parent.children,1)))
  i = i+1;
  ged_handle_out = [ged_handle_out;  psonstmp];
  List_handles(psonstmp);
//  disp("Processus recursif RETOUR")
//  disp("    ici 1");
//  pause
  if((i) <=size(psonstmp.parent.children,1)) then
    psonstmp = psonstmp.parent.children(i);
   else
    psonstmp=[];
  end
//  disp("    ici 2");
//  pause
end

endfunction



function ged_agregation(h)
  global ged_handle;ged_handle=h;
  TK_SetVar("curvis",h.visible)
  TK_EvalFile(SCI+'/tcl/ged/Agregation.tcl')
endfunction


function ged_figure(h)
  global ged_handle;ged_handle=h;

  TK_SetVar("background",string(h.background))
  TK_SetVar("rotation_style",h.rotation_style)
  TK_SetVar("figure_name",h.figure_name)
  TK_SetVar("figure_id",string(h.figure_id))
  TK_SetVar("figure_xposition",string(h.figure_position(1)))
  TK_SetVar("figure_yposition",string(h.figure_position(2)))
  TK_SetVar("figure_xsiz",string(h.figure_size(1)))
  TK_SetVar("figure_ysiz",string(h.figure_size(2)))
  TK_SetVar("figure_xaxesiz",string(h.axes_size(1)))
  TK_SetVar("figure_yaxesiz",string(h.axes_size(2)))
  TK_SetVar("bcolor",string(h.background))
  TK_SetVar("ncolors",string(size(h.color_map,1)))
  TK_SetVar("curvis",h.visible)
  TK_SetVar("curpix",h.pixmap)
  TK_SetVar("curpdm",h.pixel_drawing_mode)
  TK_SetVar("curautoresize",h.auto_resize)
  TK_SetVar("currotation_style",h.rotation_style)
  TK_EvalFile(SCI+'/tcl/ged/Figure.tcl')
endfunction

function ged_axes(h)
  global ged_handle;ged_handle=h;
  TK_SetVar("Lmargins",string(h.margins(1)));
  TK_SetVar("Rmargins",string(h.margins(2)));
  TK_SetVar("Tmargins",string(h.margins(3)));
  TK_SetVar("Bmargins",string(h.margins(4)));
  ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash"]; 
  TK_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))
  if((h.clip_state<>'clipgrf') & (h.clip_state<>'on'))
    h.clip_state='clipgrf';
    TK_SetVar("old_Xclipbox",string(h.clip_box(1)))
    TK_SetVar("old_Yclipbox",string(h.clip_box(2)))
    TK_SetVar("old_Wclipbox",string(h.clip_box(3)))
    TK_SetVar("old_Hclipbox",string(h.clip_box(4)))
    TK_SetVar("Xclipbox",string(h.clip_box(1)))
    TK_SetVar("Yclipbox",string(h.clip_box(2)))
    TK_SetVar("Wclipbox",string(h.clip_box(3)))
    TK_SetVar("Hclipbox",string(h.clip_box(4)))
    h.clip_state='off';
   else
    TK_SetVar("old_Xclipbox",string(h.clip_box(1)))
    TK_SetVar("old_Yclipbox",string(h.clip_box(2)))
    TK_SetVar("old_Wclipbox",string(h.clip_box(3)))
    TK_SetVar("old_Hclipbox",string(h.clip_box(4)))
    TK_SetVar("Xclipbox",string(h.clip_box(1)))
    TK_SetVar("Yclipbox",string(h.clip_box(2)))
    TK_SetVar("Wclipbox",string(h.clip_box(3)))
    TK_SetVar("Hclipbox",string(h.clip_box(4)))
  end
  TK_SetVar("curclipstate",h.clip_state);
  TK_SetVar("curautoclear",h.auto_clear);
  TK_SetVar("curautoscale",h.auto_scale);
  //TK_SetVar("curfillmode",h.fill_mode);
  TK_SetVar("curalpharotation",string(h.rotation_angles(1)))
  TK_SetVar("curthetarotation",string(h.rotation_angles(2)))
  ged_fontarray = ["Courier" "Symbol" "Times" "Times Italic"...
	 "Times Bold" "Times Bold Italic"  "Helvetica"  "Helvetica  Italic"...
	 "Helvetica Bold" "Helvetica Bold Italic"];
  TK_SetVar("Xlabelpos",h.x_location)
  TK_SetVar("Ylabelpos",h.y_location)
  TK_SetVar("Xlabelfontstyle",ged_fontarray(h.x_label.font_style+1))
  TK_SetVar("Ylabelfontstyle",ged_fontarray(h.y_label.font_style+1))
  TK_SetVar("Zlabelfontstyle",ged_fontarray(h.z_label.font_style+1))
  TK_SetVar("TITLEfontstyle",ged_fontarray(h.title.font_style+1))
  TK_SetVar("fontstyle",ged_fontarray(h.font_style+1))
  TK_SetVar("msdos",string(MSDOS))
  TK_SetVar("xlabel",h.x_label.text)
  TK_SetVar("ylabel",h.y_label.text)
  TK_SetVar("zlabel",h.z_label.text)
  TK_SetVar("tlabel",h.title_label.text)
  TK_SetVar("xlabel_foreground",string(h.x_label.foreground))
  TK_SetVar("ylabel_foreground",string(h.y_label.foreground))
  TK_SetVar("zlabel_foreground",string(h.z_label.foreground))
  TK_SetVar("titlelabel_foreground",string(h.title.foreground))
  TK_SetVar("xlabel_fontsize",string(h.x_label.font_size))
  TK_SetVar("ylabel_fontsize",string(h.y_label.font_size))
  TK_SetVar("zlabel_fontsize",string(h.z_label.font_size))
  TK_SetVar("titlelabel_fontsize",string(h.title.font_size))
  TK_SetVar("ncolors",string(size(f.color_map,1)))
  TK_SetVar("fcolor",string(h.foreground))
  TK_SetVar("bcolor",string(h.background))
  TK_SetVar("curthick",string(h.thickness))
  TK_SetVar("curvis",h.visible)
  TK_SetVar("curfontsize",string(h.font_size))
  TK_SetVar("curfontcolor",string(h.font_color))
  TK_SetVar("visToggle",h.axes_visible)
  TK_SetVar("limToggle",h.tight_limits)
  TK_SetVar("isoToggle",h.isoview)
  TK_SetVar("cubToggle",h.cube_scaling)
  TK_SetVar("viewToggle",h.view)
  TK_SetVar("boxToggle",h.box)
  TK_SetVar("xToggle",part(h.log_flags,1))
  TK_SetVar("yToggle",part(h.log_flags,2))
  TK_SetVar("xGrid",string(h.grid(1)))
  TK_SetVar("yGrid",string(h.grid(2)))

  
  select h.view
    case "2d" 
    h.view='3d'
    TK_SetVar("old_curalpharotation",string(h.rotation_angles(1)))
    TK_SetVar("old_curthetarotation",string(h.rotation_angles(2)))
    TK_SetVar("zGrid",string(h.grid(3)))
    TK_SetVar("dbxmin",string(h.data_bounds(1,1)))
    TK_SetVar("dbymin",string(h.data_bounds(1,2)))
    TK_SetVar("dbzmin",string(h.data_bounds(1,3)))
    TK_SetVar("dbxmax",string(h.data_bounds(2,1)))
    TK_SetVar("dbymax",string(h.data_bounds(2,2)))
    TK_SetVar("dbzmax",string(h.data_bounds(2,3)))
    h.view='2d'
    case "3d"
    TK_SetVar("zGrid",string(h.grid(3)))
    TK_SetVar("dbxmin",string(h.data_bounds(1,1)))
    TK_SetVar("dbymin",string(h.data_bounds(1,2)))
    TK_SetVar("dbzmin",string(h.data_bounds(1,3)))
    TK_SetVar("dbxmax",string(h.data_bounds(2,1)))
    TK_SetVar("dbymax",string(h.data_bounds(2,2)))
    TK_SetVar("dbzmax",string(h.data_bounds(2,3)))
  end
  TK_EvalFile(SCI+'/tcl/ged/Axes.tcl')
endfunction

function ged_rectangle(h)
  global ged_handle; ged_handle=h

  f=h;while stripblanks(f.type)<>"Figure" then f=f.parent,end
  ax=h;while stripblanks(ax.type)<>"Axes" then ax=ax.parent,end
  TK_SetVar("ncolors",string(size(f.color_map,1)))
  TK_SetVar("curcolor",string(h.foreground))
  TK_SetVar("curthick",string(h.thickness))
  TK_SetVar("curvis",h.visible)
  ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash"]; 
  TK_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))
  ged_markstylearray=["dot" "plus" "cross" "star" "diamond fill" "diamond" "triangle up" "triangle down" "trefle" "circle"];
  TK_SetVar("curmarkstyle",ged_markstylearray(h.mark_style+1))
  TK_SetVar("curmarkmode",h.mark_mode)
  // Rectangle data
   select ax.view
    case "2d" 
    ax.view='3d'
    TK_SetVar("Xval",string(h.data(1)))
    TK_SetVar("Yval",string(h.data(2)))
    TK_SetVar("Zval",string(h.data(3)))
    TK_SetVar("Wval",string(h.data(4)))
    TK_SetVar("Hval",string(h.data(5)))
    ax.view='2d'
    case "3d"
    TK_SetVar("Xval",string(h.data(1)))
    TK_SetVar("Yval",string(h.data(2)))
    TK_SetVar("Zval",string(h.data(3)))
    TK_SetVar("Wval",string(h.data(4)))
    TK_SetVar("Hval",string(h.data(5)))
  end
  TK_EvalFile(SCI+'/tcl/ged/Rectangle.tcl')
endfunction

function ged_polyline(h)
    global ged_handle; ged_handle=h
    f=h;while stripblanks(f.type)<>"Figure" then f=f.parent,end
    TK_SetVar("ncolors",string(size(f.color_map,1)))
    TK_SetVar("curcolor",string(h.foreground))
    TK_SetVar("curthick",string(h.thickness))
    TK_SetVar("curvis",h.visible)

    ged_polylinestylearray=["interpolated" "staircase" "barplot" "arrowed" "filled"];
    TK_SetVar("curpolylinestyle",ged_polylinestylearray(max(h.polyline_style,1)))
    ged_linestylearray=["solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash"];
    TK_SetVar("curlinestyle",ged_linestylearray(max(h.line_style,1)))
    ged_markstylearray=["dot" "plus" "cross" "star" "diamond fill" "diamond" "triangle up" "triangle down" "trefle" "circle"];
    TK_SetVar("curmarkstyle",ged_markstylearray(h.mark_style+1))
    TK_SetVar("curmarkmode",h.mark_mode)
    TK_SetVar("nbrow",string(size(h.data,1)))
    // pass the data matrix
    for i=1:size(h.data,1)
      val= "polyVAL("+string(i)+",1)";
      TK_EvalStr('set '+val+" "+string(h.data(i,1)));
      val= "polyVAL("+string(i)+",2)";
      TK_EvalStr('set '+val+" "+string(h.data(i,2)));
      if(get(getparaxe(h),'view') == '3d')
	val= "polyVAL("+string(i)+",3)";
	TK_EvalStr('set '+val+" "+string(h.data(i,3)));
      end
    end
    select get(getparaxe(h),'view')
      case "2d"
      TK_SetVar("nbcol",string(2));
      case "3d"
      TK_SetVar("nbcol",string(3));
    end
    TK_EvalFile(SCI+'/tcl/ged/Polyline.tcl')
endfunction


function ged_plot3d(h)
  global ged_handle; ged_handle=h
  TK_SetVar("curvis",h.visible)
  disp("Warning: This type of entity is not yet implemented in the Graphic Editor")
  TK_EvalFile(SCI+'/tcl/ged/NYI.tcl')
endfunction


function ged_fac3d(h)
  global ged_handle; ged_handle=h
  TK_SetVar("curvis",h.visible)
  disp("Warning: This type of entity is not yet implemented in the Graphic Editor")
endfunction


function ged_text(h)
  global ged_handle; ged_handle=h
  TK_SetVar("curvis",h.visible)
  disp("Warning: This type of entity is not yet implemented in the Graphic Editor")
  TK_EvalFile(SCI+'/tcl/ged/NYI.tcl')
  TK_EvalFile(SCI+'/tcl/ged/NYI.tcl')
endfunction


function ged_legend(h)
  global ged_handle; ged_handle=h
  TK_SetVar("curvis",h.visible)
  disp("Warning: This type of entity is not yet implemented in the Graphic Editor")
  TK_EvalFile(SCI+'/tcl/ged/NYI.tcl')
endfunction


function ged_arc(h)
  global ged_handle; ged_handle=h
  TK_SetVar("curvis",h.visible)
  disp("Warning: This type of entity is not yet implemented in the Graphic Editor")
  TK_EvalFile(SCI+'/tcl/ged/NYI.tcl')
endfunction


function ged_segs(h)
  global ged_handle; ged_handle=h
  TK_SetVar("curvis",h.visible)
  disp("Warning: This type of entity is not yet implemented in the Graphic Editor")
  TK_EvalFile(SCI+'/tcl/ged/NYI.tcl')
endfunction


function ged_champ(h)
  global ged_handle; ged_handle=h
  TK_SetVar("curvis",h.visible)
  disp("Warning: This type of entity is not yet implemented in the Graphic Editor")
  TK_EvalFile(SCI+'/tcl/ged/NYI.tcl')
endfunction

function ged_fec(h)
  global ged_handle; ged_handle=h
  TK_SetVar("curvis",h.visible)
  disp("Warning: This type of entity is not yet implemented in the Graphic Editor")
  TK_EvalFile(SCI+'/tcl/ged/NYI.tcl')
endfunction


function ged_grayplot(h)
  global ged_handle; ged_handle=h
  TK_SetVar("curvis",h.visible)
  disp("Warning: This type of entity is not yet implemented in the Graphic Editor")
  TK_EvalFile(SCI+'/tcl/ged/NYI.tcl')
endfunction


function ged_axis(h)
  global ged_handle; ged_handle=h
  TK_SetVar("curvis",h.visible)
  disp("Warning: This type of entity is not yet implemented in the Graphic Editor")
  TK_EvalFile(SCI+'/tcl/ged/NYI.tcl')
endfunction


function h=ged_getobject(pt)
  h=[]

   f=get("current_figure");
   aold=get("current_axes")
   axes_array=f.children
   for k=1:size(axes_array,'*')
     Axes=axes_array(k)
     set("current_axes",Axes)
     h=ged_loop(Axes)
     if h<>[] then return,end
   end
endfunction

function h=ged_loop(a)
  h=[]
  for ka=1:size(a,'*')
    ck=a(ka)
    select ck.type
      case "Polyline"
      xy=ck.data;
      d=Dist2polyline((xy(:,1)-Xmin)/Dx,(xy(:,2)-Ymin)/Dy,pts)
      if d<0.005 then h=ck,return,end
      case "Rectangle"
      xy=ck.data;
      x0=xy(1);y0=xy(2);W=xy(3);H=xy(4);
      d=Dist2polyline((x0+[0,W,W,0]-Xmin)/Dx,(y0+[0,0,-H,-H]-Ymin)/Dy,pts)
      if d<0.005 then h=ck,return,end
      case "Arc" 
      xy=ck.data;
      x0=xy(1,1);y0=xy(1,2);RX=xy(2,1)/2;RY=xy(2,2)/2;a1=xy(3,1);a2=xy(3,2);
      d=abs(((pt(1)-(x0+RX))./RX)^2+((pt(2)-(y0-RY))./RY)^2-1)
      if d<0.005 then h=ck,return,end
      case "Segs"
      xy=ck.data;
      xv=matrix(xy(:,1),2,-1)
      yv=matrix(xy(:,2),2,-1)
      dx=(xv(1,:)-xv(2,:))
      dy=(yv(1,:)-yv(2,:))
      d_d=dx.^2+dy.^2
      case "Agregation"
      h=ged_loop(ck.children)
      if h<>[] then return,end
      case "Axes"
      xy=ck.data_bounds;
      [xp,yp]=xchange(pt(1),pt(2),'i2f')
      Xmin=xy(1,1);Ymin=xy(1,2),Dx=xy(2,1)-xy(1,1);Dy=xy(2,2)-xy(1,2);
      pts=[(xp-Xmin)/Dx (yp-Ymin)/Dy]
      d=Dist2polyline([0,1,1,0],[0,0,1,1],pts)
      if d<0.005 then h=ck,return,end
      h=ged_loop(Axes.children)
      if h<>[] then return,end
    end
  end
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

  if or(win==winsid()) then //does the windows still exists

//    seteventhandler("")  
  else //window has been deleted by an asynchronous xdel()
    return
  end

  if ibut<0 then 
    if ibut==-1000 then //the window has been closed by the window manager
      return
    end
 //   seteventhandler("ged_eventhandler"),
    return,
  end
//  seteventhandler("")  
  global ged_handle;ged_handle=[]
  cur=gcf();scf(win)
  ged_handle=ged_getobject([x,y])
  scf(cur)

  if ged_handle~=[] then


    if ibut==0 then
      //Set(h)
      tkged()
    elseif ibut==2 then
      [x,y]=xchange(x,y,'i2f')
      pos=[x,y]
      while %t then
	rep=xgetmouse()
	if rep(3)>0 then break,end
	
	move(ged_handle,rep(1:2)-pos)
	pos=rep(1:2)
      end
    elseif ibut==3 then
      delete(ged_handle)
    end
  end
//  seteventhandler("ged_eventhandler")  
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
  h=ged_handle

  // hierarchical viewer
  TK_send_handles_list(gcf())
  TK_SetVar("curgedindex",string(Get_handle_pos_in_list(h)))

  //color_map array for color sample display
  f=getparfig(h);
  for i=1:size(f.color_map,1)
    redname= "RED("+string(i)+")";
    TK_EvalStr('set '+redname+" "+string(f.color_map(i,1)));
    grename= "GREEN("+string(i)+")";
    TK_EvalStr('set '+grename+" "+string(f.color_map(i,2)));
    bluname= "BLUE("+string(i)+")";
    TK_EvalStr('set '+bluname+" "+string(f.color_map(i,3)));
  end

  select h.type
    case "Polyline"
     ged_polyline(h)
    case "Rectangle"
     ged_rectangle(h)
    case "Axes"
     ged_axes(h)
    case "Figure"
     ged_figure(h)
    case "Agregation"
     ged_agregation(h)

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
    case "Axis"
     ged_axis(h)
  end
endfunction
function setStyle(sty)
  global ged_handle; h=ged_handle
  h.polyline_style=find(sty==['interpolated','staircase', ...
		    'barplot','arrowed','filled'])
endfunction
function setLineStyle(sty)
  global ged_handle; h=ged_handle
  h.line_style=find(sty==[ "solid" "dash" "dash dot" "longdash dot" ..
		    "bigdash dot" "bigdash longdash"])
  
endfunction
function setMarkStyle(sty)
  global ged_handle; h=ged_handle
  h.mark_style=find(sty==["dot" "plus" "cross" "star" "diamond fill" ..
                    "diamond" "triangle up" "triangle down" "trefle" "circle"])-1
   
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
  tst=execstr('h.data_bounds=tmp','errcatch','n');
  if tst<>0 then
   disp 'Warning: X data_bounds must contain double'
  end
endfunction


function setYdb(ymin, ymax)
  global ged_handle; h=ged_handle
  tmp=h.data_bounds;
  tmp(1,2)=ymin;
  tmp(2,2)=ymax;
  tst=execstr('h.data_bounds=tmp','errcatch','n');
  if tst<>0 then
   disp 'Warning: Y data_bounds must contain double'
  end
endfunction

function setZdb(zmin, zmax)
  global ged_handle; h=ged_handle
 select h.view
     case "2d"
      h.view='3d';
      tmp=h.data_bounds;
      tmp(1,3)=zmin;
      tmp(2,3)=zmax;
      tst=execstr('h.data_bounds=tmp','errcatch','n');
      h.view='2d';
      if tst<>0 then
       disp 'Warning: Z data_bounds must contain double'
      end
     case "3d"
      tmp=h.data_bounds;
      tmp(1,3)=zmin;
      tmp(2,3)=zmax;
      tst=execstr('h.data_bounds=tmp','errcatch',n');
      if tst<>0 then
        disp 'Warning: Z data_bounds must contain double'
      end
     end
endfunction


function LogtoggleX( tog)
 global ged_handle; h=ged_handle
tst=execstr("global h;h.log_flags=tog+part(h.log_flags,2)",'errcatch','n');

if tst<>0 then
   disp 'Warning: X bounds must be strictly positive'
end

endfunction

function LogtoggleY( tog)
 global ged_handle; h=ged_handle
tst=execstr("global h;h.log_flags=part(h.log_flags,1)+tog",'errcatch','n');

if tst<>0 then
   disp 'Warning: Y bounds must be strictly positive'
end

endfunction



function  hfig= getparfig( h )

htmp = h;
hfig= []
while htmp.type<>'Figure' do
  htmp=htmp.parent
end

hfig = htmp;
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
      ax.view='3d';
      tmp=h.data;
      tmp(3)=val;
      tst=execstr('h.data=tmp','errcatch','n');
      ax.view='2d';
      if tst<>0 then
       disp 'Warning: Z data must contain double'
      end
     case "3d"
      tmp=h.data;
      tmp(3)=val;
      tst=execstr('h.data=tmp','errcatch',n');
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
      ax.view='3d';
      tmp=h.data;
      tmp(4)=val;
      tst=execstr('h.data=tmp','errcatch','n');
      ax.view='2d';
      if tst<>0 then
       disp 'Warning: Width data must contain double'
      end
     case "3d"
      tmp=h.data;
      tmp(4)=val;
      tst=execstr('h.data=tmp','errcatch',n');
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
      ax.view='3d';
      tmp=h.data;
      tmp(5)=val;
      tst=execstr('h.data=tmp','errcatch','n');
      ax.view='2d';
      if tst<>0 then
       disp 'Warning: Height data must contain double'
      end
     case "3d"
      tmp=h.data;
      tmp(5)=val;
      tst=execstr('h.data=tmp','errcatch',n');
      if tst<>0 then
        disp 'Warning: Height data must contain double'
      end
     end
endfunction

