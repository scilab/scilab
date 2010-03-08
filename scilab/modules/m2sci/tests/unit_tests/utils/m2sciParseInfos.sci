function [sizeInfo, typeInfo, propInfo] = m2sciParseInfos(strInfos)
parts = tokens(strInfos, "|");

if size(parts, "*") == 3 then
  // The name is not given
  sizeInfo = msscanf(parts(1), "%d %d");
  typeInfo = parts(2);
  propInfo = parts(3);
else
  // The name is given
  sizeInfo = msscanf(parts(2), "%d %d");
  typeInfo = parts(3);
  propInfo = parts(4);
  
end
endfunction
