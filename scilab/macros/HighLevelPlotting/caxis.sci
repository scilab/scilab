function [vect]=caxis(varargin) 

[lhs,rhs]=argn(0);

if rhs==0
 
   state=loadGraphicState(gcw());
   vect=state('caxis');
   return;
      
elseif rhs==1
   state=loadGraphicState(gcw());
   if type(varargin(1))==1
      if length(varargin(1))==2
         state('caxis')=varargin(1)(:)';
	 state('caxisMode')='manual';
      else
         error('caxis : vector must have the form [cmin cmax]');
      end
   elseif type(varargin(1))==10
      select varargin(1)
      case 'auto'
         state('caxisMode')='auto';
	 state('caxis')=[]
      case 'manual'
         if state('caxis')~=[]
	    state('caxisMode')='manual';
	 else
	    error('caxis : cannot set caxis to manual mode, caxis is empty');
	 end
      else
         error('caxis : unknown caxis mode');
      end
   end
else
   error('caxis : too many input arguments');      

end

vect=state('caxis');
saveGraphicState(state,gcw());
