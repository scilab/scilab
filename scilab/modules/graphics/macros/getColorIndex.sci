//
//Author : F.Leray
//Copyright INRIA
//

function opt1 = getColorIndex(ColorString)

str = convstr(ColorString);
// 1) COLORS

//Colors
Table      = ['red' 'green' 'blue' 'cyan' 'magenta' 'yellow' 'black' ..
	'k' 'white'     'none' 'auto' 'flat' 'interp']
//ColorVal = ['red' 'green' 'blue' 'cyan' 'magenta' 'yellow' 'black' ..
//      'black' 'white' 'none' 'auto' 'flat' 'interp']
//                                      |       |
//                                  only used with 'surf' command

//color management
//difficulty here since we have to allow every kind of writings:
//i.e:
//magenta can be set by: 'm' 'ma' 'mag' 'mage' 'magen' 'magent' or at least 'magenta'
//

opt1=-1;
str  = convstr(PropertyValue);

k=find(part(Table,1:length(str))==str);

c1 = part(str,1); // We get the first character

if (k == [] | (size(k,'*') > 1 & c1 <> 'b'))
  warning("Error in Color property : bad argument specified");
  return;
end

if c1=='b' // special case here : we have to distinguish between black and blue colors
  c2 = part(str,2);
  if (c2 == 'l')
    c3 = part(str,3);
    if (c3 == 'a')
      k=7; // k is set to black color
    else  
      k=3; // k is set to blue color
    end
  else
    k=3; // k is set to blue color
  end
end

opt = Table(k);
for i=1:length(str)
  if part(opt,i) <> part(str,i)
    break;
  end
end

opt1 = k;

str = part(str,i:length(str));

if (size(opt1,'*') > 1)  // unreachable case normally
  warning("Error in Property specification : too much properties specified")
  return;
end


endfunction
