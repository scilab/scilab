// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Fabrice Leray
// Copyright (C) 2010 - DIGITEO - Pierre Lando <pierre.lando@scilab.org>
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function opt1 = getColorIndex(ColorString)

[lhs,rhs]=argn(0)

if rhs<>1 then
  error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "getColorIndex", 1));
end

if type(ColorString)<>10 then
  error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), "getColorIndex", 1));
end

opt1=-1;
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
