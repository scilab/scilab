function [currColOut,listOut]=addPlot3(X,Y,Z,c,m,ms,lt,win)

state=loadGraphicState(win);
cmap = state('colormap');

// Adds a 5-uple to the list of elementary plots to do

MAXCOL = 7;

currColOut=currentColor;

nc=size(X,2); // number of curves
if c==0
   c=zeros(nc,1);
   for k=1:nc
      c(k)=addcolor(cmap(currColOut,:));
      currColOut=currColOut+1;
      if currColOut > MAXCOL
        currColOut=1;
      end
   end  
else
   c=c(ones(nc,1));
end

listOut=list('plot3',X,Y,Z,c,m,ms,lt);

endfunction // End of addPlot3
