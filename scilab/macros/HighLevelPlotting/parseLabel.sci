function [label] = parseLabel(funcName, labelType, parseList)

//
// Parsing label for the 'xlabel,ylabel,zlabel' properties 
//

if length(parseList) == 1
   error(sprintf('%s : missing %s ',funcName, labelType));
end

select type(parseList(2))
case 10 //  a string
  label = parseList(2);
else
   error(sprintf('%s : missing %s ',funcName, labelType));
end




