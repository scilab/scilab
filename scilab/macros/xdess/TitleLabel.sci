function TitleLabel(varargin)
// Copyright INRIA
// TITLELABEL function
// is used by the functions title, xlabel, ylabel, zlabel
// F.Belahcene

[lhs,rhs]=argn(0);

isold=get('figure_style')=='old'

if isold
  disp("Not supported under old graphic style");
  return;
end

//CurColor = 0; // current color used if no color specified via LineSpec
// nor PropertyName

ListArg = varargin;
titlelabel = ListArg(1);
ListArg(1) = null();

//detect and set the current axes now:
if type(ListArg(1)) == 9
  hdle = ListArg(1);
  if (hdle.type == "Axes")
    sca(ListArg(1));
    ListArg(1) = null(); // remove this parameter from the list
  else
    disp("Handle should be an Axes handle")
    return;
  end
end

monaxe = gca();

nv = size(ListArg);
// detect and set the string now:
if  type(ListArg(1))== 10 & modulo(nv,2) == 1 then
  st = ListArg(1);
  execstr("monaxe."+ titlelabel +".text"+"=st")
  ListArg(1) = null();
  nv=nv-1 
else
 error("wrong input argument")
end

T=[];

//given_data = 2;

for k=1:nv
  T(k,1) = type(ListArg(k))
end

given_data = 0;
P1 = 0;

for i=1:nv
  if T(i) == 1
    given_data = given_data +1;
  else
    P1 = i;  // Position of the first PropertyName field
    break;
  end
end


// delay the drawing commands
// smart drawlater
current_figure=gcf();
cur_draw_mode = current_figure.immediate_drawing;
current_figure.immediate_drawing = 'off';


// set some defaults here
// current_figure=gcf(); // already init. before
// current_figure.color_map=jetcolormap(64); // bad choice -> init must be done somewhere else.

colormap_size = size(current_figure.color_map,1);

///////////////////////////////////
//Global Property treatment      //
//PropertyName and PropertyValue //
///////////////////////////////////

// P1 is the position of the first PropertyName field.
Property = P1;

current_surface = gce(); // get the newly created fac3d

current_titlelabel=get(monaxe,titlelabel)
monaxe.axes_visible = ["on","on","on"]
//current_surface.mark_size_unit='point';

while ((Property <> 0) & (Property <= nv-1))
  setTitleLabelProperty(ListArg(Property),ListArg(Property+1),current_titlelabel,current_figure,cur_draw_mode)
  //setLabelProperty(ListArg(Property),ListArg(Property+1),monaxe.title,current_figure,cur_draw_mode)
  Property = Property+2;
end

//postponed drawings are done now !
// smart drawnow
ResetFigureDDM(current_figure, cur_draw_mode);


endfunction

// Reset the Default Drawing Mode (DDM) of the figure
// immediate_drawing is set to its input value.
function ResetFigureDDM(cur_figure, cur_draw_mode)

if type(cur_figure == 9)
  if cur_figure.type == "Figure"
    cur_figure.immediate_drawing = cur_draw_mode;
  else
    disp("Error in ResetFigureDDM : input argument must be a figure graphic handle");
    return;
  end
end

endfunction

