function [az,el]=parseView(typeOfPlot,argList,azin,elin);
az=azin;
el=elin;
if length(argList)>=2
 if type(argList(2))==1
   p=argList(2);
   if size(p,1)==1
      p=p';
   end
   [li,co]=size(p);
   if li==2
      az=p(1,:);
      el=p(2,:);
   elseif li==3
      ez=zeros(1,co);
      al=zeros(1,co);
      for k=1:co
        el(k)=acos(p(3,k)/norm(p(:,k)))/%pi*180;
        az(k)=atan(p(2,k),p(1,k))/%pi*180;
      end
   else
      error(sprintf('%s : view specification must be a 2 or 3-vector',typeOfPlot))
   end
 else
   error(sprintf('%s : view specification must be a vector',typeOfPlot))
 end
else
    error(sprintf('%s : missing view vector',typeOfPlot))
end

endfunction // end of parseView
