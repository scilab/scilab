function ged(k,win)
  //xset, xget used because ged should handle both old and new style
  cur=xget('window')
  xset('window',win) 
  if  get('figure_style')=='old' then 
    message('Edit menu does not apply to old style graphics')
    unsetmenu(win,'Edit')
    xset('window',cur)
    return
  end
  TK_EvalStr("set isgedinterp [interp exists ged]")
  if TK_GetVar("isgedinterp")=='0' then    
    TK_EvalStr("interp create ged")
    TK_EvalStr("load {'+gettklib()+'} Tk ged")
    TK_EvalStr("ged eval {wm withdraw .}")
  end

  select k
    case 1 then //edit current figure properties
    ged_figure(gcf())
    //to be done
    case 2 then //edit current axes
    ged_axes(gca())
    case 3 then //start Entity picker
    seteventhandler("ged_eventhandler")
    case 4 then //stop Entity picker
    seteventhandler("")
  end
  xset('window',cur)
endfunction




function ged_figure(h)
  global ged_handle;ged_handle=h;
  message('to be defined')
endfunction

function ged_axes(h)
  global ged_handle;ged_handle=h;
  f=h.parent
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
    TK_SetVar("dbxmax",string(h.data_bounds(2,1)))
    TK_SetVar("dbymax",string(h.data_bounds(2,2)))
    TK_SetVar("dbzmax",string(h.data_bounds(2,3)))
  end
  TK_EvalFile(SCI+'/tcl/ged/Axes.tcl')
endfunction

function ged_rectangle(h)
  global ged_handle; ged_handle=h
  f=h;while stripblanks(f.type)<>"Figure" then f=f.parent,end
  TK_SetVar("ncolors",string(size(f.color_map,1)))
  TK_SetVar("curcolor",string(h.foreground))
  TK_SetVar("curthick",string(h.thickness))
  TK_SetVar("curvis",h.visible)
  TK_SetVar("curstyle",string(h.line_style))
  TK_EvalFile(SCI+'/tcl/ged/Rectangle.tcl')
endfunction

function ged_polyline(h)
    global ged_handle; ged_handle=h
    f=h;while stripblanks(f.type)<>"Figure" then f=f.parent,end
    TK_SetVar("ncolors",string(size(f.color_map,1)))
    TK_SetVar("curcolor",string(h.foreground))
    TK_SetVar("curthick",string(h.thickness))
    TK_SetVar("curvis",h.visible)
    TK_SetVar("curstyle",string(h.polyline_style))
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

  //color_map array for color sample display
  f=getparfig(h);
  for i=1:size(f.color_map,1)
    redname= "RED("+string(i)+")";
    TK_EvalStr('set '+redname+" "+string(int(255*f.color_map(i,1))));
    grename= "GREEN("+string(i)+")";
    TK_EvalStr('set '+grename+" "+string(int(255*f.color_map(i,2))));
    bluname= "BLUE("+string(i)+")";
    TK_EvalStr('set '+bluname+" "+string(int(255*f.color_map(i,3))));
  end

  select h.type
    case "Polyline"
     ged_polyline(h)

    case "Rectangle"
    ged_rectangle(h)
    case "Axes"
    ged_axes(h)
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

