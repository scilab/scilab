function [axisStyle,axisRatio,axisVect,axisTightX,axisTightY] = parseAxis(funcName, parseList, aO,aE, aV, aTX,aTY)

//
// Parsing function for the 'axis' property 
//

axisTightX=aTX;
axisTightY=aTY;
axisEqual=aE;
axisVect=aV;
axisStyle=aO;

if length(parseList) == 1
   error(sprintf('%s : missing axis limits or mode ',funcName));
end

select type(parseList(2))
case 10 //  a string
   select parseList(2)
   case 'tightX'
      axisTightX=%T;
   case 'tightY'
      axisTightY=%T;
   case 'tight'
      axisTightX=%T;
      axisTightY=%T;
   case 'equal'
      axisRatio='equal';
   case 'vis3D'
      axisRatio='vis3d';
   case 'vis3d'
      axisRatio='vis3d';
   case 'off'
      axisStyle='off';
   case 'box'
      axisStyle='box';
   case 'trihedral'
      axisStyle='trihedral';
   case 'normal'
      axisStyle='normal';
   case 'left'
      axisStyle='normal';
   case 'right'
      axisStyle='right';
   case 'center'
      axisStyle='center';
   case 'origin'
      axisStyle='origin';
   else
      error(sprintf('%s : unknown axis mode ''%s''',funcName,parseList(2)));
   end
case 1 // a matrix (must be a 4 or 6 element vector)
   if funcName=='plot3' | funcName=='mesh' | funcName=='surf' | funcName=='surfl' | funcName=='trisurf'...
   | funcName=='trisurfl' | funcName=='trimesh'
      if (length(parseList(2))==6) & or(size(parseList(2))==1)
         axisVect=parseList(2)
      else
         error(sprintf('%s : axis limits must be a 6 element vector',funcName));
      end
   elseif (length(parseList(2))==4) & or(size(parseList(2))==1)
      axisVect=parseList(2)
      axisTightX=%T;
      axisTightY=%T;
   else
      error(sprintf('%s : axis limits must be a 4 element vector',funcName));
   end
else
   error(sprintf('%s : missing axis limits or mode ',funcName));
end




