// ------------------------------------
// Allan CORNET
// INRIA 2007
// ------------------------------------
// Warning : order !!
// list of  : filename , module
listmacrostoload = ['fileparts','fileio';
  'getshell','core';
  'pathconvert','fileio';
  'stripblanks','string';
  'listfiles','fileio'
  'unix_g','io';
  'OS_Version','core';
  '%c_a_c','overloading';
  'basename','fileio';
  'mputl','fileio';
  'genlib','functions';
  ];

dim = size(listmacrostoload);

for i = 1:dim(1)
 clear(listmacrostoload(i,1));
 exec(SCI+'/modules/'+listmacrostoload(i,2)+'/macros/'+listmacrostoload(i,1)+'.sci');
end
clear i;
clear dim;
// ------------------------------------