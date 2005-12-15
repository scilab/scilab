//
//Author : F.Leray
//Copyright INRIA
//

function plot(varargin)
// Try to build a new better parser that could manage things like:
// plot(x,y,'X',1:10); // where X stands for Xdata (Matlab recognize
//it and treats it well...

[lhs,rhs]=argn(0);

isold=get('figure_style')=='old'

if isold
  T = varargin
  oldplot(T(:));
  return;
end


if ~rhs
	//LineSpec and PropertySpec examples:
	title_demo = [
		'';
		'Demo of plot()';
		'========================================';
		''];
	
	s_mat=["t=0:%pi/20:2*%pi;";
	"subplot(211)";
	"plot(t,sin(t),''ro-.'',t,cos(t),''cya+'',t,abs(sin(t)),''--mo'')";
	"subplot(212)";
	"plot([t ;t],[sin(t) ;cos(t)],''xdat'',[1:2])"];
	
	write(%io(2),title_demo);
	write(%io(2),s_mat);
	write(%io(2),' ');
	execstr(s_mat);
	return;
end



CurColor = 0; // current color used if no color specified via LineSpec
// nor PropertyName


ListArg = varargin;

//detect and set the current axes now:
if type(ListArg(1)) == 9
  hdle = ListArg(1);
  if (hdle.type == "Axes")
    sca(ListArg(1));
    ListArg(1) = null(); // remove this parameter from the list
  else
    warning("Handle should be an Axes handle")
    return;
  end
end


nv = size(ListArg)


T=[];
d=[];

typeOfPlot = 'plot';
provided_data = 2;

for k=1:nv
  T(k,1) = type(ListArg(k))
end

Ttmp=T;

for i=1:nv-1
  e=[];
  e=find(Ttmp(i,1)==1 & or(Ttmp(i+1,1)==[1,13,130])) // to accept double, macro function or primitive as second argument

  if (e<>[]) then
    d=[d i];
    Ttmp(i,1)  = 99; // Replace a known type by 99 (no meaning) to count it once only!
    Ttmp(i+1,1)= 99; // to avoid having (x1,y1,x2,y2) ->d=[1,2,3]
    // With this trick, d=[1,3];
  end

  //  disp("i=");
  //  disp(i);
  //  disp("d vaut:");
  //  disp(d)
end


if (d==[]) // No data couple found
  // Search for at least a single data , i.e.: plot(y)
  //  disp(T)
  //  disp(type(T(1,1)))

  if (T(1,1)==1 & ListArg(1)<>[]) then // case plot(SINGLE y,...)
    d = 1; 
    provided_data = 1;
    
    if (modulo(nv-d,2)<>0) then
      P1 = d+2 // Position of the first PropertyName field
    else
      P1 = d+1
    end
    
  else
    warning("Error inside input argument : no data");
    return;
  end
  
  //  disp("ICII--------------------")
  //  disp("d vaut:")
  //  disp(d);
  //  disp("P1=")
  //  disp(P1)
  
else
  
  // Some test to check wrong inputs
  //
  // 1. Test if 2 data couples (first : type==1, second : type=[1,13,130]) 
  // are at least separated by 2 indices
  if (d(2:$)-d(1:$-1)<2)
    warning("Error inside input argument !");
    return;
  end

  // 2. Test if no string couples happen before P1 (see below for P1 definition)
  for i=1:d($)
    e=[];
    e=find(Ttmp(i,1)==10 & Ttmp(i+1,1)==10)
    
    if (e<>[]) then
      warning("Error inside input argument : String argument is an unknown option.");
      return;
    end
  end


  
  if (modulo(nv-(d($)+1),2)<>0) then
    P1 = d($)+3 // Position of the first PropertyName field
  else
    P1 = d($)+2
  end
  
end

numplot = size(d,'*');

P = zeros(numplot,3); 
// P is a matrix storing the index of x, y and linespec
// if one of these indices is 0 => it does not exist
// (which is possible for x and linepsec, not for y)

//disp("T vaut:");
//disp(T)

if (provided_data == 2) then

  for k=1:size(d,'*')
    P(k,1:2) = d(k) +[0,1] // x,y index storage

    if (d(k)+2 < P1)
      if (T(d(k)+2,1)==10) then // LineSpec treatment
	P(k,3) = d(k)+2;
      end
    end
    //    disp(P);
  end
else
  // we are in the case where: plot(SINGLE y,... x not specified
  // or plot(handle,SINGLE y,...
  P(1,1) = 0; // no x specified
P(1,2) = d;

//pause;

if (d+1 < P1)
  if (T(d+1,1)==10) then // LineSpec treatment
    P(1,3) = d+1;
  end
end
end

//disp("P1 vaut:");
//disp(P1);



// delay the drawing commands
// smart drawlater
current_figure=gcf();
cur_draw_mode = current_figure.immediate_drawing;
current_figure.immediate_drawing = 'off';


//Now, we plot the decomposed plots one by one with their own linespec
// provided_data = 2 : x and y are provided

//disp("P=")
//disp(P)

//disp("ListArg=")
//disp(ListArg)

FinalAgreg=[]; // Final Compound containing all the new created plots.

//for i=numplot:-1:1
for i=1:numplot
  
  //default values
  Marker=[];
  MarkerSize=1;
  Color=[];
  LineStyle=1;
  Line = %F;
  Marker = %F;

  if (provided_data == 2) then
    
    if (type(ListArg(P(i,2))) == 13 | type(ListArg(P(i,2))) == 130)
      // A function (macro or primitive) is given. We need to build the vector or matrix.
      sizefirstarg = size(ListArg(P(i,1)));
      fonction = ListArg(P(i,2));
      firstarg = ListArg(P(i,1));
      tmp = [];
      for ii=1:sizefirstarg(1,2)
	for jj=1:sizefirstarg(1,1)
	  tmp(jj,ii) = fonction(firstarg(jj,ii));
	end
      end
      ListArg(P(i,2)) = tmp;
    end

    [X,Y] = checkXYPair(typeOfPlot,ListArg(P(i,1)),ListArg(P(i,2)),current_figure,cur_draw_mode)
  else
    if or(size(ListArg(P(1,2)))==1)  // If this is a vector
      X=1:length(ListArg(P(1,2))); // insert an abcsissa vector of same length,
    else                                  // if this is a matrix,
      X=1:size(ListArg(P(1,2)),1); // insert an abcsissa vector with 
    end
    [X,Y] = checkXYPair(typeOfPlot,X,ListArg(P(1,2)),current_figure,cur_draw_mode)
  end

  // Case if 'Xdata', 'Ydata' or 'Zdata' have been set in (PropertyName,Propertyvalue) couples
  // must be taken into account now
  
  // P1 is the position of the first PropertyName field.
  Property = P1;
  
  while (Property <= nv-1)
    PropertyName  = ListArg(Property);
    PropertyValue = ListArg(Property+1);
    
    // Xdata can ONLY be a vector (cf. Matlab help)
    PName = getPlotPropertyName(PropertyName,current_figure,cur_draw_mode);
    if (PName == 'xdata')
      
      if (type(PropertyValue)<>1 | and(size(PropertyValue)<>1))
	warning("Xdata value must be a column or row vector.");
	ResetFigureDDM(current_figure, cur_draw_mode);
	return;
      else
	PropertyValue = PropertyValue(:); // force
	if or(size(X))==1  // If X is a vector (inevitably a column vector because checkXYPair always returns a column vector)
	  X = PropertyValue; // X is replaced by PropertyValue
	  [X,Y] = checkXYPair(typeOfPlot,X,Y,current_figure,cur_draw_mode)
	else // X is a matrix
	  if size(PropertyValue,'*') == size(X,1)
	    for j=1:size(PropertyValue,'*')
	      X(j,:) = PropertyValue(j,1);
	    end
	  else
	    str='plot : incompatible dimensions in input arguments';
	    warning(str);
	    ResetFigureDDM(current_figure, cur_draw_mode);
	  end
	end
      end
      
      // Ydata ONLY be a vector (contrary to what is said by the Matlab help)
    elseif (PName == 'ydata')
      
      if (type(PropertyValue)<>1 | and(size(PropertyValue)<>1))
	warning("Ydata value must be a column or row vector.");
	ResetFigureDDM(current_figure, cur_draw_mode);
	return;
      else
	PropertyValue = PropertyValue(:); // force
	if or(size(Y))==1  // If Y is a vector (inevitably a column vector because checkXYPair always returns a column vector)
	  Y = PropertyValue; // Y is replaced by PropertyValue
	  [X,Y] = checkXYPair(typeOfPlot,X,Y,current_figure,cur_draw_mode)
	else // Y is a matrix
	  if size(PropertyValue,'*') == size(Y,1)
	    for j=1:size(PropertyValue,'*')
	      Y(j,:) = PropertyValue(j);
	    end
	  else
	    str='plot : incompatible dimensions in input arguments';
	    warning(str);
	    ResetFigureDDM(current_figure, cur_draw_mode);
	  end
	end
	
      end
      
      // Zdata will be treated after plot building
    end
    
    Property = Property+2;
  end
  

  
  //Now we have an array P [numplot x 3] containing indices pointing on T for :
  // - x (<>0 if existing)
  // - y
  // - linespec (<>0 if existing)
  // for each plot passed in argument
  //       x | y | linespec
  //       ----------------
  //plot1   0|i1 |0    <=> plot(y)
  //plot2  i2|i3 |0    <=> plot(x,y)
  //plot3  i4|i5 |i6   <=> plot(x,y,LINESPEC)
  //...
  
  
  
  if (P(i,3)<>0) then // if we have a line spec <=> index <> 0
    [Color,Line,LineStyle,Marker,MarkerStyle,MarkerSize,fail] = getLineSpec(ListArg(P(i,3)),current_figure,cur_draw_mode); 
  end

  
  // The plot is made now :
  
  //  pause;
  err = execstr('plot2d(X,Y)','errcatch','m');
  
  if err <> 0
    mprintf("Error %d : in plot2d called by plot",err);
    ResetFigureDDM(current_figure, cur_draw_mode);
    return;
  end
  
  agreg=gce();  // when using plot2d, we always have an Compound as the current entity

  FinalAgreg = [agreg FinalAgreg];

  if Color==[]
    DefaultColor = %T;
  else
    DefaultColor = %F; 
  end

  for ii=size(agreg.children,'*'):-1:1
    e=agreg.children(ii); // we apply linespec to the lines

    // Color treatment : if no color specified by LineSpec nor PropertyName
    // Set the default color to the curve
    if DefaultColor == %T
      [Color,CurColor] = setDefaultColor(CurColor);
    end


    //    disp('CurColor=')
    //    disp(CurColor);
    //    disp('Color=')
    //    disp(Color)

    //    disp('Line=');
    //    disp(Line);

    if (Marker == %T)
      e.mark_style=MarkerStyle;
      e.mark_mode ='on';
      e.mark_foreground = Color;
      e.mark_style=MarkerStyle;
      e.mark_size=MarkerSize;
    else
      e.mark_mode ='off'
    end

    if (Line == %T)
      e.line_mode='on';
      e.foreground = Color;
      e.line_style = LineStyle;
    else
      e.line_mode='off'
    end

    if (Line == %F & Marker ==%F) // no linespec nor PropertyName set
      e.line_mode='on';
      e.foreground = Color;
      e.line_style = LineStyle;
    end

  end
end


///////////////////////////////////
//Global Property treatment      //
//PropertyName and PropertyValue //
///////////////////////////////////


//disp("Start Global Property treatment")

// Those properties will be applied to Agreg children
Agreg = glue(FinalAgreg(1:$))

k=find(Agreg.children.type=="Compound")

while (k<>[])
  k=k(1);
  unglue(Agreg.children(k));
  k=find(Agreg.children.type=="Compound")
end


//disp("Agreg=")
//disp(Agreg);


// P1 is the position of the first PropertyName field.
Property = P1;

//disp("JE SUIS LA...");

Curves = Agreg.children
//Curves(:,1) = Curves(:,$:-1:1);

// set mark_size_unit to 'point' for all the curves
Curves.mark_size_unit='point';

while (Property <= nv-1)
  setPlotProperty(ListArg(Property),ListArg(Property+1),Curves,current_figure,cur_draw_mode)
  
  Property = Property+2;
end





//disp("PUIS LA...");


//disp("End Global Property treatment")



//postponed drawings are done now !
// smart drawnow
ResetFigureDDM(current_figure, cur_draw_mode)

endfunction




// Reset the Default Drawing Mode (DDM) of the figure
// immediate_drawing is set to its input value.
function ResetFigureDDM(cur_figure, cur_draw_mode)

if type(cur_figure) == 9
  if cur_figure.type == "Figure"
    cur_figure.immediate_drawing = cur_draw_mode;
  else
    warning("Error in ResetFigureDDM : input argument must be a figure graphic handle");
    return;
  end
end

endfunction
