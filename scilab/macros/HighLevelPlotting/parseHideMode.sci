function hide = parseHideMode(typeOfPlot,parseList)

//
// Parsing function for the 'hidden' property 
//

if length(parseList) == 1
   error(sprintf('%s : missing hide mode ',typeOfPlot));
end

if ~(typeOfPlot=='mesh' | typeOfPlot=='trimesh')
   error(sprintf('%s : hidden property not allowed',typeOfPlot));
end

select type(parseList(2))
case 10 //  a string
   select parseList(2)
   case 'yes'
      hide=%T;
   case 'no'
      hide=%F;
   case 'off'
      hide=%F;
   else
      error(sprintf('%s : unknown hide mode ''%s''',typeOfPlot,parseList(2)));
   end
else
   error(sprintf('%s : missing hide mode ',typeOfPlot));
end




