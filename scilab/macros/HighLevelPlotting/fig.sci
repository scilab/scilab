function varargout=fig(varargin) 

global myWorld

cmap=[];

if length(myWorld)==0
   graphinit();
end

global myWorld

newWin=%F;

[lhs,rhs]=argn(0);
if rhs==0           // put the "first avalaible" window number at the
  if winsid()==[]   // top of the argument list
     winNum=0;
     newWin=%T;
  else
     winNum=max(winsid())+1;
     newWin=%T;
  end
  argList=list(winNum)
else
   argList=varargin;
   if (type(argList(1))~=1)
      if winsid()==[]   // top of the argument list
         winNum=0;
	 newWin=%T;
       else
         winNum=max(winsid())+1;
       end
       argList(0)=winNum;
   end
end

if newWin
   state=loadDefaultGraphicState();
   saveGraphicState(state,winNum);
end

while length(argList)

   if (type(argList(1))==1) // The argument seems to be a window number

      if length(argList(1)) ~= 1
         error('fig : figure number must be an integer scalar')
      end
      winNum=int(argList(1));
      state=loadGraphicState(winNum); // get the graphic state of the window
      if or(winsid()==winNum) // if winNum is the number of an existing window
         scf(winNum); // activate the window
         dontClear=%T;
      else
         scf(winNum); // create the window
		 newWin=%T;
      end
      argList(1)=null();

   elseif (type(argList(1))==10) // If this argument is a string

      select argList(1) // Try to identify a property name

      case 'background'

         state('background')=parseColor('fig','background',argList);
         argList(1)=null(); argList(1)=null();

      case 'foreground'

         state('foreground')=parseColor('fig','foreground',argList);
         argList(1)=null(); argList(1)=null();

      case 'frameColor'

         state('frameColor')=parseColor('fig','frameColor',argList);
         argList(1)=null(); argList(1)=null();

      case 'colormap' // the user wants to change the RGB colormap

	 cmap=parseColormap('fig',argList);
         argList(1)=null(); argList(1)=null();
         state('RGBcolormap')=cmap;
	 saveGraphicState(state,winNum);

	case 'margin' // margin of plot box
		state('margin')=parseMargin('fig',argList);
		argList(1)=null();argList(1)=null();
		saveGraphicState(state,winNum);
      else
          str=sprintf('fig : %s is an unknown property name',argList(1));
          error(str)

      end // select argList(1)

   else
 
      str=sprintf('fig : argument %d has not the expected type',argNumber);
      error(str);

   end // if type(argList(1))

end // while length(argList)

winH=gcf();
imm_draw = winH.immediate_drawing;
winH.immediate_drawing='off';	

if cmap~=[]
	RGBmap=state('RGBcolormap');
	n1=size(state('colormap'),1);
	n2=size(RGBmap,1);
	finalColormap=[state('colormap');RGBmap];
	state('RGBcolormaptable')=n1+1:n1+n2;
	winH.color_map=finalColormap;
	fbg=state('frameColor')*255;
	winH.background=color(fbg(1),fbg(2),fbg(3));
end

saveGraphicState(state,winNum);

if newWin
	clearWindow(winNum,'fig')
end

varargout(1)=winNum;

winH.immediate_drawing=imm_draw; // <=> smarter than drawlater


// end of fig
