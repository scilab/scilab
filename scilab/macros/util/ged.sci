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
//  [x,y]=xchange(x,y,'i2f')
  ged_handle=ged_getobject([x,y])


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

function ged_men()
  addmenu(xget("window'),'Edit',list(2,'ged_edit'))
endfunction

function ged(win)
  if argn(2)<1 then
    win=xget("window")
  end
  old=xget("window")
  xset("window",win)
  if get("figure_style")=="new" then
    seteventhandler("ged_eventhandler")
  end
  
  TK_EvalStr("set isgedinterp [interp exists ged]")
  if TK_GetVar("isgedinterp")=='0' then    
    TK_EvalStr("interp create ged")
    TK_EvalStr("load {'+gettklib()+'} Tk ged")
    TK_EvalStr("ged eval {wm withdraw .}")
  end
  xset("window",old)
endfunction


  
function Set(h)
  if size(h,'*')>1 then
    error('vector of handle is not allowed')
  else
    t='Handle of type ""'+h.type+'"" with properties:'
    select h.type
      case "Polyline"
      labels=[ "polyline_style"
	       "thickness"
	       "line_style"
	       "fill_mode"
	       "foreground"
	       "visible"
	       "clip_state"
	       "clip_box"]
      [ini,typs]=build_args(labels)
//      k=find(labels=="data")-1
//      typs(2*k+1)="mat";typs(2*k+2)=[-1,2]
      k=find(labels=="clip_box")-1
      typs(2*k+2)=4
      GetSetValue(h)

      case "Patch"
      case "Agregation"
      case "Axes"
      case "Legend"
      case "Rectangle"
      case "Figure"
      case "Grayplot"
      case "Matplot"
      case "Fec"
      case "Segs"
      case "Plot3d"
      case "Plot3d1" 
      case "Fac3d" 
      case "Fac3d2" 
      case "Fac3d3" 
    end
  end
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
  //color_map array for color sample display
  f=gcf();
  for i=1:size(get(gcf(),'color_map'),1)
    redname= "RED("+string(i)+")";
    TK_EvalStr('set '+redname+" "+string(int(255*f.color_map(i,1))));
    grename= "GREEN("+string(i)+")";
    TK_EvalStr('set '+grename+" "+string(int(255*f.color_map(i,2))));
    bluname= "BLUE("+string(i)+")";
    TK_EvalStr('set '+bluname+" "+string(int(255*f.color_map(i,3))));
  end

  select ged_handle.type
    case "Polyline"
    TK_SetVar("ncolors",string(size(get(gcf(),'color_map'),1)))
    TK_SetVar("curcolor",string(h.foreground))
    TK_SetVar("curthick",string(h.thickness))
    TK_SetVar("curvis",h.visible)
    TK_SetVar("curstyle",string(h.polyline_style))
    TK_EvalFile(SCI+'/tcl/ged/Polyline.tcl')
    case "Rectangle"
    TK_SetVar("ncolors",string(size(get(gcf(),'color_map'),1)))
    TK_SetVar("curcolor",string(h.foreground))
    TK_SetVar("curthick",string(h.thickness))
    TK_SetVar("curvis",h.visible)
    TK_SetVar("curstyle",string(h.line_style))
    TK_EvalFile(SCI+'/tcl/ged/Rectangle.tcl')
    case "Axes"
    TK_SetVar("xlabel",h.x_label.text)
    TK_SetVar("ylabel",h.y_label.text)
    TK_SetVar("zlabel",h.z_label.text)
    TK_SetVar("xlabel_foreground",string(h.x_label.foreground))
    TK_SetVar("ylabel_foreground",string(h.y_label.foreground))
    TK_SetVar("zlabel_foreground",string(h.z_label.foreground))
    TK_SetVar("titlelabel_foreground",string(h.title.foreground))
    TK_SetVar("xlabel_fontsize",string(h.x_label.font_size))
    TK_SetVar("ylabel_fontsize",string(h.y_label.font_size))
    TK_SetVar("zlabel_fontsize",string(h.z_label.font_size))
    TK_SetVar("titlelabel_fontsize",string(h.title.font_size))
    TK_SetVar("ncolors",string(size(get(gcf(),'color_map'),1)))
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
    TK_SetVar("zGrid","-1")
     case "3d"
    TK_SetVar("zGrid",string(h.grid(3)))
   end
    TK_EvalFile(SCI+'/tcl/ged/Axes.tcl')
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


function updatecolor(index)

fcol=get(gcf(),'color_map');
COL= int(255*fcol(index,1)) ;
TK_SetVar("RED",string(COL));

COL= int(255*fcol(index,2)) ;
TK_SetVar("GREEN",string(COL));

COL= int(255*fcol(index,3)) ;
TK_SetVar("BLUE",string(COL));

endfunction
