function [currColOut,listeOut]=addPlot(X,Y,c,m,ms,lt,win)

// Adds a quadruple to the list of elementary plots to do

MAXCOL = 7;

listeOut=liste;

currColOut=currentColor;

if c==0
   c=currColOut;
   currColOut=currColOut+1;
   if currColOut > MAXCOL
     currColOut=1;
   end  
end

listeOut(length(listeOut)+1)=list(X,Y,c,m,ms,lt);

// End of addPlot



