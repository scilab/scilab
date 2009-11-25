// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Calixte Denizet
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 
function str = prettyprint(a, fmt, delim, processByElement, isWrapped)
//a is a Scilab variable
//fmt is the format, if omitted 'latex' is used by default, it can be 'latex', 'tex' or 'mathml'.
//processByElement is a boolean to indicate if the resulting matrix must be converted into a single string.
//isWrapped is a boolean to indicate if the result must be wrapped inside delimiters ('$' for latex and tex or nothing for mathml) to be used with xstring or xtitle 
// delim is a string indicating the delimiter to use for the resulting matrix, it's only used if isWrapped is true. The delimiter can be '(', '{', '[', '|', '||' or ')'
//If the type of a is not handled, then an error is returned. If a user wants to handle the type foo when exporting with latex, he must define the function foo2latex.
 
  nargs = argn(2);
 
  select nargs
  case 0 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected."),"prettyprint",1,4));
  case 1 then
    fmt='latex';
    delim = '(';
    processByElement = %F;
    isWrapped = %T;
  case 2 then
    if type(fmt) <> 10 then
      error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"prettyprint",2));
    end
    delim = '(';
    processByElement = %F;
    isWrapped = %T;    
  case 3 then
    if type(fmt) <> 10 then 
      error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"prettyprint",2));
    end
    if type(delim) <> 10 | (delim <> '(' &  delim <> '[' & delim <> '|' & delim <> '||' & delim <> '{' & delim <> '') then
      error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', ''%s'', ''%s'' or ''%s'' expected.\n"),"prettyprint",3,"(","[","|","||","{",""));
    end
    processByElement = %F;          
    isWrapped = %T;
  case 4 then
    if type(fmt) <> 10 then 
      error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"prettyprint",2));
    end
    if type(delim) <> 10 | (delim <> '(' &  delim <> '[' & delim <> '|' & delim <> '||' & delim <> '{' & delim <> '') then
      error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', ''%s'', ''%s'' or ''%s'' expected.\n"),"prettyprint",3,"(","[","|","||","{",""));
    end
    if type(processByElement) <> 4 then 
      error(msprintf(gettext("%s: Wrong type for argument #%d: Boolean expected.\n"),"prettyprint",4));
    end
    isWrapped = %T;
  case 5
    if type(fmt) <> 10 then 
      error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"prettyprint",2));
    end
    if type(delim) <> 10 | (delim <> '(' &  delim <> '[' & delim <> '|' & delim <> '||' & delim <> '{' & delim <> '') then
      error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', ''%s'', ''%s'' or ''%s'' expected.\n"),"prettyprint",3,"(","[","|","||","{",""));
    end      
    if type(processByElement) <> 4 then
      error(msprintf(gettext("%s: Wrong type for argument #%d: Boolean expected.\n"),"prettyprint",4));
    end
    if type(isWrapped) <> 4 then
      error(msprintf(gettext("%s: Wrong type for argument #%d: Boolean expected.\n"),"prettyprint",5));
    end
  else
    error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected"),"prettyprint",1,4));    
  end
  try
    execstr('[plus,minus,img,op,cp,ow,cw,d2s]=' + fmt + 'conf()');
  catch 
    error(msprintf(gettext("%s: Wrong export format: %s"),"prettyprint",fmt));
  end
  
  typ = type(a);
        
  select typ
  //Scalar type : real or complex
  case 1 then
    if norm(imag(a)) <= %eps * norm(real(a)) then
      str = comp2str(real(a),plus,minus,img,d2s);
    else
      str = comp2str(a,plus,minus,img,d2s);
    end
  //Polynomial type
  case 2 then
    x = varn(a);
    C = coeff(a);
    [m,n] = size(a);
    maxd = max(degree(a));
    str = emptystr(a);
    for i = 0:maxd do
      execstr('expo=' + fmt + 'exp(x,i)');
      A = C(1:m,(1 + i * n):((i + 1) * n));
      if norm(imag(A)) <= %eps * norm(real(A)) then
        str = str + comp2coef(real(A),expo,plus,minus,img,op,cp,d2s);
      else
        str = str + comp2coef(A,expo,plus,minus,img,op,cp,d2s);
      end        
    end
    L = length(plus);
    for i = 1:m do
      for j = 1:n do
        if part(str(i,j),1:L) == plus then
          str(i,j) = part(str(i,j),L + 1:length(str(i,j)));
        end
      end
    end
    //The null polynomial is represented by '' so we must replace it by '0'
    str(str == '') = d2s(0);
  //Boolean type
  case 4 then
    str = 'F' + emptystr(a);
    str(a) = 'T';
    execstr('str=' + fmt + 'exp(str,1)');
  //Int type  
  case 8 then
    str = d2s(a);
  //String type
  case 10 then
    str = a;
  //Tlist type
  case 16 then
    t = a(1);
    select t(1)
    //Rationnal type
    case 'r' then
      num = prettyprint(a('num'),fmt,'(',%T,%F);
      den = prettyprint(a('den'),fmt,'(',%T,%F);
      execstr('str=rational2' + fmt + '(num,den)');
    //Linear state space type
    case 'lss' then
      execstr('str=lss2' + fmt + '(a)');
    else
      str = unknown_type(t(1),a,fmt);
      return;
    end      
  case 17  then
    a1=getfield(1,a);
    select a1(1)
    //Cell type
    case 'ce' then
      dim = double(a.dims);
      L = length(dim);
      if L >= 3 then
        str = unknown_type('ce',a,fmt);
        return;
      end
      str = emptystr(dim(1),dim(2));
      for i = 1:dim(1) do
        for j = 1:dim(2) do
          str(i,j) = prettyprint(a(i,j).entries,fmt,delim,%F,%F);
        end
      end
    else
      str = unknown_type(a1(1),a,fmt);
      return;
    end
  else
    str = unknown_type(typeof(a),a,fmt);
    return;
  end
  
  [m,n] = size(a);
  if m*n <> 1 & ~processByElement then
    execstr('str=' + fmt + 'matrix(str,''' + delim + ''')');
  end

  if isWrapped then
    str = ow + str + cw;
  end
endfunction

//This function gives different elements of configuration
//plus is '+', minus is '-', img is sqrt(-1), op and cp are opening ang closing parenthesis (useful to generate complex coeff in polynomials), ow and cw are opening and closing delimiters used to wrap the expression to be displayed with xstring or xtitle, d2s is the function used to convert double into mathml. 
function [plus,minus,img,op,cp,ow,cw,d2s] = mathmlconf()
  plus = '<mo>+</mo>';
  minus = '<mo>-</mo>';
  img = '<mi>i</mi>';
  op = '<mfenced separator=""""><mrow>';
  cp = '</mrow></mfenced>';
  ow = '';
  cw = '';
  d2s = mathmldbl2str;
endfunction  

//This function generates var^n (useful for polynomials)
function str = mathmlexp(var,n)
  if n >= 2 then
    str = '<msup><mi>' + var + '</mi><mn>' + string(n) + '</mn></msup>';
  elseif n == 1 then
    str = '<mi>' + var + '</mi>';
  else
    str = '';
  end
endfunction 

//This function creates a fraction with the given numerator and denominator  
function str = rational2mathml(num,den)
  str = '<mfrac><mrow>' + num + '</mrow><mrow>' + den + '</mrow></mfrac>';
endfunction

//This function converts a double into a mathml string
//Negative and positive are handled and the scientific notation too
function str = mathmldbl2str(x)
  str = strsubst(string(x),'-','<mo>-</mo><mn>');
  if strindex(str,'D') <> [] then
    str = strsubst(str,'+','<mo>+</mo><mn>');
    str = strsubst(str,'D','</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow>');
    if x > 0 then
      str = '<mn>' + str;
    end
    str = str + '</mn></mrow></msup>';
  elseif x >= 0 then
    str = '<mn>' + str + '</mn>';
  else
    str = str + '</mn>';
  end
endfunction  

//This function generates a matrix with the given delimiter
function str = mathmlmatrix(mat,delim)
  if argn(2) == 1 then
    delim = '(';
  end
  select delim
  case '[' then
    com = '<mfenced open=""["" close=""]"">';
  case '(' then
    com = '<mfenced open=""("" close="")"">';
  case '|' then
    com = '<mfenced open=""&#x2223;"" close=""&#x2223;"">';
  case '' then
    com = '<mfenced>';
  case '{' then
    com = '<mfenced open=""{"" close=""}"">';
  case '||' then
    com = '<mfenced open=""&#x2225;"" close=""&#x2225;"">';
  else
    com = '<mfenced open=""("" close="")"">';
  end
  [m,n] = size(mat);
  str = com + '<mtable>';
  for i=1:m do
    str = str + '<mtr><mtd>' + strcat(mat(i,:),'</mtd><mtd>') + '</mtd></mtr>';
  end
  str = str + '</mtable></mfenced>';
endfunction

//This function handles the syslin
function str = lss2mathml(sys)
  if sys(7) == 'c' then 
    der = '<mover><mi>X</mi><mo>.</mo></mover>';
  else
    der = '<mover><mi>X</mi><mo>+</mo></mover>'
  end
  str = '<mfenced close="""" open=""{""><mtable columnalign=""right center left""><mtr><mtd>' + der + '</mtd><mtd><mo>=</mo></mtd><mtd>' + prettyprint(sys(2),'mathml','(',%F,%F) + '<mi>X</mi><mo>+</mo>' + prettyprint(sys(3),'mathml','(',%F,%F) + '<mi>U</mi></mtd></mtr><mtr><mtd><mi>Y</mi></mtd><mtd><mo>=</mo></mtd><mtd>' + prettyprint(sys(4),'mathml','(',%F,%F) + '<mi>X</mi></mtd></mtr>';
  try 
    if norm(sys(5),1) == 0 then
      str = str + '</mtable></mfenced>';
    else
      str = str + '<mo>+</mo>' + prettyprint(sys(5),'mathml','(',%F,%F) + '</mtable></mfenced>';
    end
  catch
    str = str + '<mo>+</mo>' + prettyprint(sys(5),'mathml','(',%F,%F) + '</mtable></mfenced>';
  end
endfunction    

function [plus,minus,img,op,cp,ow,cw,d2s] = latexconf()
  plus = '+';
  minus = '-';
  img = 'i';
  op = '(';
  cp = ')';
  ow = '$';
  cw = '$';
  d2s = latexdbl2str;
endfunction

function str = latexexp(var,n)
  if n >= 2 then
    str = var + '^{' + string(n) + '}';
  elseif n == 1 then
    str = var;
  else
    str = '';
  end
endfunction

function str = rational2latex(num,den)
  str = '{\frac{' + num + '}{' + den + '}}'
endfunction

function str = latexdbl2str(x)
  str = string(x);
  if strindex(str,'D') <> [] then
    str = strsubst(str,'D','\times10^{');
    str = str + '}';
  end
endfunction  

function str = lss2latex(sys)
  if sys(7) == 'c' then 
    der = '\dot{X}';
  else
    der = '\stackrel{+}{X}'
  end
  str = '{\left\{\begin{array}{rcl}' + der + '&=&' + prettyprint(sys(2),'latex','(',%F,%F) + ' X+' + prettyprint(sys(3),'latex','(',%F,%F) + 'U\cr Y &=& ' + prettyprint(sys(4),'latex','(',%F,%F) + ' X ';
  try 
    if norm(sys(5),1) == 0 then
      str = str + '\end{array}\right.}';
    else
      str = str + ' + ' + prettyprint(sys(5),'latex','(',%F,%F) + '\end{array}\right.}';
    end
  catch
    str = str + ' + ' + prettyprint(sys(5),'latex','(',%F,%F) + '\end{array}\right.}';
  end
endfunction
        
function str = latexmatrix(mat,delim)
  if argn(2) == 1 then
    delim = '(';
  end
  select delim
  case '[' then
    com = 'bmatrix';
  case '(' then
    com = 'pmatrix';
  case '|' then
    com = 'vmatrix';
  case '' then
    com = 'matrix';
  case '{' then
    com = 'Bmatrix';
  case '||' then
    com = 'Vmatrix';
  else
    com = 'pmatrix';
  end
  [m,n] = size(mat);
  str = '{\begin{' + com + '}';
  if m*n == 0 then
    str = str + '\ ';
  else
    for i=1:m do
      str = str + strcat(mat(i,:),'&') + '\cr ';
    end
  end
  str = str + '\end{' + com + '}}';
endfunction  

function [plus,minus,img,op,cp,ow,cw,d2s] = texconf()
  [plus,minus,img,op,cp,ow,cw,d2s] = latexconf()
endfunction

function str = texexp(var,n)
  str = latexexp(var,n);
endfunction

function str = rational2tex(num,den)
  str = '{{' + num + '}\over{' + den + '}}'
endfunction

function str = texdbl2str(x)
  str = string(x);
  if strindex(str,'D') <> [] then
    str = strsubst(str,'D','\times10^{');
    str = str + '}';
  end
endfunction  

function str = lss2tex(sys)
  if sys(7) == 'c' then 
    der = '\dot{X}';
  else
    der = '{\buildrel + \over X}'
  end
  str = '{\left\{\eqalign{' + der + ' &= ' + prettyprint(sys(2),'tex','(',%F,%F) + ' X+' + prettyprint(sys(3),'tex','(',%F,%F) + 'U\cr Y &= ' + prettyprint(sys(4),'tex','(',%F,%F) + ' X ';
  try 
    if norm(sys(5),1) == 0 then
      str = str + '}\right.}';
    else
      str = str + ' + ' + prettyprint(sys(5),'tex','(',%F,%F) + '}\right.}';
    end
  catch
    str = str + ' + ' + prettyprint(sys(5),'tex','(',%F,%F) + '}\right.}';
  end
endfunction
        
function str = texmatrix(mat,delim)
  if argn(2) == 1 then
    delim = '(';
  end
  select delim
  case '[' then
    com = 'bmatrix';
  case '(' then
    com = 'pmatrix';
  case '|' then
    com = 'vmatrix';
  case '' then
    com = 'matrix';
  case '{' then
    com = 'Bmatrix';
  case '||' then
    com = 'Vmatrix';
  else
    com = 'pmatrix';
  end
  [m,n] = size(mat);
  str = '{\' + com + '{';
  if m*n == 0 then
    str = str + '\ ';
  else
    for i=1:m do
      str = str + strcat(mat(i,:),'&') + '\cr ';
    end
  end
  str = str + '}}';
endfunction   

//This function converts a complex (or a double) into a string in using the function d2s (double2string) fixed by mathmlconf or latexconf.
function str = comp2str(z,plus,minus,img,d2s)
  [m,n] = size(z);
  if m*n <> 1 then
    str = emptystr(z);
    for i=1:m do
      for j=1:n do
        str(i,j) = comp2str(z(i,j),plus,minus,img,d2s);
      end
    end
    return;
  end
  re = real(z);
  im = imag(z);
  if re == 0 & im == 0 then
    str = d2s(0);
    return;
  end;
  if re == 0 then
    if im == 1 then
      str = img;
    elseif im == -1 then
      str = minus + img;
    else
      str = d2s(im) + img;
    end
  elseif im == 0 then
    if re > 0 then
      str = d2s(re);
    else
      str = d2s(re);
    end
  elseif im > 0 then
    if im == 1 then
      str = d2s(re) + plus + img;
    else
      str = d2s(re) + plus + d2s(im) + img;
    end
  else
    if im == -1 then
      str = d2s(re) + minus + img;
    else
      str = d2s(re) + d2s(im) + img;
    end
  end
endfunction

//This function convert a complex into a coefficient (parenthesis are eventually added around the coeff, if var=='' (i.e. var^0), no parenthesis are added.
function str = comp2coef(z,var,plus,minus,img,op,cp,d2s)
  [m,n] = size(z);
  if m*n <> 1 then
    str = emptystr(z);
    for i=1:m do
      for j=1:n do
        str(i,j) = comp2coef(z(i,j),var,plus,minus,img,op,cp,d2s);
      end
    end
    return;
  end  
  re = real(z);
  im = imag(z);
  if var <> '' then 
    if re <> 0 & im <> 0 then
      str = plus + op + comp2str(z,plus,minus,img,d2s) + cp + var;
    elseif re == 0 & im == 0 then
      str = '';
    elseif re == 1 & im == 0 then
      str = plus + var;
    elseif re == -1 & im == 0 then
      str = minus + var;
    elseif re > 0 | im > 0 then
      str = plus + comp2str(z,plus,minus,img,d2s) + var;
    else
      str = comp2str(z,plus,minus,img,d2s) + var;
    end
  else
    if re == 0 & im == 0 then
      str = ''; 
    else
      str = comp2str(z,plus,minus,img,d2s);
    end
  end
endfunction

function str = unknown_type(typ,a,fmt)
  try
    execstr('str=' + typ + '2' + fmt + '(a)');
  catch
    error(msprintf(gettext("%s: Type %s is not handled : Define the function %s2%s."),"prettyprint",typ,typ,fmt))
  end
endfunction 
 
=======
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Calixte Denizet
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 
function str = prettyprint(a, fmt, delim, processByElement, isWrapped)
//a is a Scilab variable
//fmt is the format, if omitted 'latex' is used by default, it can be 'latex', 'tex' or 'mathml'.
//processByElement is a boolean to indicate if the resulting matrix must be converted into a single string.
//isWrapped is a boolean to indicate if the result must be wrapped inside delimiters ('$' for latex and tex or nothing for mathml) to be used with xstring or xtitle 
// delim is a string indicating the delimiter to use for the resulting matrix, it's only used if isWrapped is true. The delimiter can be '(', '{', '[', '|', '||' or ')'
//If the type of a is not handled, then an error is returned. If a user wants to handle the type foo when exporting with latex, he must define the function foo2latex.
 
  nargs = argn(2);
 
  select nargs
  case 0 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected."),"prettyprint",1,4));
  case 1 then
    fmt='latex';
    delim = '(';
    processByElement = %F;
    isWrapped = %T;
  case 2 then
    if type(fmt) <> 10 then
      error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"prettyprint",2));
    end
    delim = '(';
    processByElement = %F;
    isWrapped = %T;    
  case 3 then
    if type(fmt) <> 10 then 
      error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"prettyprint",2));
    end
    if type(delim) <> 10 | (delim <> '(' &  delim <> '[' & delim <> '|' & delim <> '||' & delim <> '{' & delim <> '') then
      error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', ''%s'', ''%s'' or ''%s'' expected.\n"),"prettyprint",3,"(","[","|","||","{",""));
    end
    processByElement = %F;          
    isWrapped = %T;
  case 4 then
    if type(fmt) <> 10 then 
      error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"prettyprint",2));
    end
    if type(delim) <> 10 | (delim <> '(' &  delim <> '[' & delim <> '|' & delim <> '||' & delim <> '{' & delim <> '') then
      error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', ''%s'', ''%s'' or ''%s'' expected.\n"),"prettyprint",3,"(","[","|","||","{",""));
    end
    if type(processByElement) <> 4 then 
      error(msprintf(gettext("%s: Wrong type for argument #%d: Boolean expected.\n"),"prettyprint",4));
    end
    isWrapped = %T;
  case 5
    if type(fmt) <> 10 then 
      error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"prettyprint",2));
    end
    if type(delim) <> 10 | (delim <> '(' &  delim <> '[' & delim <> '|' & delim <> '||' & delim <> '{' & delim <> '') then
      error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', ''%s'', ''%s'' or ''%s'' expected.\n"),"prettyprint",3,"(","[","|","||","{",""));
    end      
    if type(processByElement) <> 4 then
      error(msprintf(gettext("%s: Wrong type for argument #%d: Boolean expected.\n"),"prettyprint",4));
    end
    if type(isWrapped) <> 4 then
      error(msprintf(gettext("%s: Wrong type for argument #%d: Boolean expected.\n"),"prettyprint",5));
    end
  else
    error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected"),"prettyprint",1,4));    
  end
  try
    execstr('[plus,minus,img,op,cp,ow,cw,d2s]=' + fmt + 'conf()');
  catch 
    error(msprintf(gettext("%s: Wrong export format: %s"),"prettyprint",fmt));
  end
  
  typ = type(a);
        
  select typ
  //Scalar type : real or complex
  case 1 then
    if norm(imag(a)) <= %eps * norm(real(a)) then
      str = comp2str(real(a),plus,minus,img,d2s);
    else
      str = comp2str(a,plus,minus,img,d2s);
    end
  //Polynomial type
  case 2 then
    x = varn(a);
    C = coeff(a);
    [m,n] = size(a);
    maxd = max(degree(a));
    str = emptystr(a);
    for i = 0:maxd do
      execstr('expo=' + fmt + 'exp(x,i)');
      A = C(1:m,(1 + i * n):((i + 1) * n));
      if norm(imag(A)) <= %eps * norm(real(A)) then
        str = str + comp2coef(real(A),expo,plus,minus,img,op,cp,d2s);
      else
        str = str + comp2coef(A,expo,plus,minus,img,op,cp,d2s);
      end        
    end
    L = length(plus);
    for i = 1:m do
      for j = 1:n do
        if part(str(i,j),1:L) == plus then
          str(i,j) = part(str(i,j),L + 1:length(str(i,j)));
        end
      end
    end
    //The null polynomial is represented by '' so we must replace it by '0'
    str(str == '') = d2s(0);
  //Boolean type
  case 4 then
    str = 'F' + emptystr(a);
    str(a) = 'T';
    execstr('str=' + fmt + 'exp(str,1)');
  //Int type  
  case 8 then
    str = d2s(a);
  //String type
  case 10 then
    str = a;
  //Tlist type
  case 16 then
    t = a(1);
    select t(1)
    //Rationnal type
    case 'r' then
      num = prettyprint(a('num'),fmt,'(',%T,%F);
      den = prettyprint(a('den'),fmt,'(',%T,%F);
      execstr('str=rational2' + fmt + '(num,den)');
    //Linear state space type
    case 'lss' then
      execstr('str=lss2' + fmt + '(a)');
    else
      str = unknown_type(t(1),a,fmt);
      return;
    end      
  case 17  then
    a1=getfield(1,a);
    select a1(1)
    //Cell type
    case 'ce' then
      dim = double(a.dims);
      L = length(dim);
      if L >= 3 then
        str = unknown_type('ce',a,fmt);
        return;
      end
      str = emptystr(dim(1),dim(2));
      for i = 1:dim(1) do
        for j = 1:dim(2) do
          str(i,j) = prettyprint(a(i,j).entries,fmt,delim,%F,%F);
        end
      end
    else
      str = unknown_type(a1(1),a,fmt);
      return;
    end
  else
    str = unknown_type(typeof(a),a,fmt);
    return;
  end
  
  [m,n] = size(a);
  if m*n <> 1 & ~processByElement then
    execstr('str=' + fmt + 'matrix(str,''' + delim + ''')');
  end

  if isWrapped then
    str = ow + str + cw;
  end
endfunction

//This function gives different elements of configuration
//plus is '+', minus is '-', img is sqrt(-1), op and cp are opening ang closing parenthesis (useful to generate complex coeff in polynomials), ow and cw are opening and closing delimiters used to wrap the expression to be displayed with xstring or xtitle, d2s is the function used to convert double into mathml. 
function [plus,minus,img,op,cp,ow,cw,d2s] = mathmlconf()
  plus = '<mo>+</mo>';
  minus = '<mo>-</mo>';
  img = '<mi>i</mi>';
  op = '<mfenced separator=""""><mrow>';
  cp = '</mrow></mfenced>';
  ow = '';
  cw = '';
  d2s = mathmldbl2str;
endfunction  

//This function generates var^n (useful for polynomials)
function str = mathmlexp(var,n)
  if n >= 2 then
    str = '<msup><mi>' + var + '</mi><mn>' + string(n) + '</mn></msup>';
  elseif n == 1 then
    str = '<mi>' + var + '</mi>';
  else
    str = '';
  end
endfunction 

//This function creates a fraction with the given numerator and denominator  
function str = rational2mathml(num,den)
  str = '<mfrac><mrow>' + num + '</mrow><mrow>' + den + '</mrow></mfrac>';
endfunction

//This function converts a double into a mathml string
//Negative and positive are handled and the scientific notation too
function str = mathmldbl2str(x)
  str = strsubst(string(x),'-','<mo>-</mo><mn>');
  if strindex(str,'D') <> [] then
    str = strsubst(str,'+','<mo>+</mo><mn>');
    str = strsubst(str,'D','</mn><mo>&#x00D7;</mo><msup><mn>10</mn><mrow>');
    if x > 0 then
      str = '<mn>' + str;
    end
    str = str + '</mn></mrow></msup>';
  elseif x >= 0 then
    str = '<mn>' + str + '</mn>';
  else
    str = str + '</mn>';
  end
endfunction  

//This function generates a matrix with the given delimiter
function str = mathmlmatrix(mat,delim)
  if argn(2) == 1 then
    delim = '(';
  end
  select delim
  case '[' then
    com = '<mfenced open=""["" close=""]"">';
  case '(' then
    com = '<mfenced open=""("" close="")"">';
  case '|' then
    com = '<mfenced open=""&#x2223;"" close=""&#x2223;"">';
  case '' then
    com = '<mfenced>';
  case '{' then
    com = '<mfenced open=""{"" close=""}"">';
  case '||' then
    com = '<mfenced open=""&#x2225;"" close=""&#x2225;"">';
  else
    com = '<mfenced open=""("" close="")"">';
  end
  [m,n] = size(mat);
  str = com + '<mtable>';
  for i=1:m do
    str = str + '<mtr><mtd>' + strcat(mat(i,:),'</mtd><mtd>') + '</mtd></mtr>';
  end
  str = str + '</mtable></mfenced>';
endfunction

//This function handles the syslin
function str = lss2mathml(sys)
  if sys(7) == 'c' then 
    der = '<mover><mi>X</mi><mo>.</mo></mover>';
  else
    der = '<mover><mi>X</mi><mo>+</mo></mover>'
  end
  str = '<mfenced close="""" open=""{""><mtable columnalign=""right center left""><mtr><mtd>' + der + '</mtd><mtd><mo>=</mo></mtd><mtd>' + prettyprint(sys(2),'mathml','(',%F,%F) + '<mi>X</mi><mo>+</mo>' + prettyprint(sys(3),'mathml','(',%F,%F) + '<mi>U</mi></mtd></mtr><mtr><mtd><mi>Y</mi></mtd><mtd><mo>=</mo></mtd><mtd>' + prettyprint(sys(4),'mathml','(',%F,%F) + '<mi>X</mi></mtd></mtr>';
  try 
    if norm(sys(5),1) == 0 then
      str = str + '</mtable></mfenced>';
    else
      str = str + '<mo>+</mo>' + prettyprint(sys(5),'mathml','(',%F,%F) + '</mtable></mfenced>';
    end
  catch
    str = str + '<mo>+</mo>' + prettyprint(sys(5),'mathml','(',%F,%F) + '</mtable></mfenced>';
  end
endfunction    

function [plus,minus,img,op,cp,ow,cw,d2s] = latexconf()
  plus = '+';
  minus = '-';
  img = 'i';
  op = '(';
  cp = ')';
  ow = '$';
  cw = '$';
  d2s = latexdbl2str;
endfunction

function str = latexexp(var,n)
  if n >= 2 then
    str = var + '^{' + string(n) + '}';
  elseif n == 1 then
    str = var;
  else
    str = '';
  end
endfunction

function str = rational2latex(num,den)
  str = '{\frac{' + num + '}{' + den + '}}'
endfunction

function str = latexdbl2str(x)
  str = string(x);
  if strindex(str,'D') <> [] then
    str = strsubst(str,'D','\times10^{');
    str = str + '}';
  end
endfunction  

function str = lss2latex(sys)
  if sys(7) == 'c' then 
    der = '\dot{X}';
  else
    der = '\stackrel{+}{X}'
  end
  str = '{\left\{\begin{array}{rcl}' + der + '&=&' + prettyprint(sys(2),'latex','(',%F,%F) + ' X+' + prettyprint(sys(3),'latex','(',%F,%F) + 'U\cr Y &=& ' + prettyprint(sys(4),'latex','(',%F,%F) + ' X ';
  try 
    if norm(sys(5),1) == 0 then
      str = str + '\end{array}\right.}';
    else
      str = str + ' + ' + prettyprint(sys(5),'latex','(',%F,%F) + '\end{array}\right.}';
    end
  catch
    str = str + ' + ' + prettyprint(sys(5),'latex','(',%F,%F) + '\end{array}\right.}';
  end
endfunction
        
function str = latexmatrix(mat,delim)
  if argn(2) == 1 then
    delim = '(';
  end
  select delim
  case '[' then
    com = 'bmatrix';
  case '(' then
    com = 'pmatrix';
  case '|' then
    com = 'vmatrix';
  case '' then
    com = 'matrix';
  case '{' then
    com = 'Bmatrix';
  case '||' then
    com = 'Vmatrix';
  else
    com = 'pmatrix';
  end
  [m,n] = size(mat);
  str = '{\begin{' + com + '}';
  if m*n == 0 then
    str = str + '\ ';
  else
    for i=1:m do
      str = str + strcat(mat(i,:),'&') + '\cr ';
    end
  end
  str = str + '\end{' + com + '}}';
endfunction  

function [plus,minus,img,op,cp,ow,cw,d2s] = texconf()
  [plus,minus,img,op,cp,ow,cw,d2s] = latexconf()
endfunction

function str = texexp(var,n)
  str = latexexp(var,n);
endfunction

function str = rational2tex(num,den)
  str = '{{' + num + '}\over{' + den + '}}'
endfunction

function str = texdbl2str(x)
  str = string(x);
  if strindex(str,'D') <> [] then
    str = strsubst(str,'D','\times10^{');
    str = str + '}';
  end
endfunction  

function str = lss2tex(sys)
  if sys(7) == 'c' then 
    der = '\dot{X}';
  else
    der = '{\buildrel + \over X}'
  end
  str = '{\left\{\eqalign{' + der + ' &= ' + prettyprint(sys(2),'tex','(',%F,%F) + ' X+' + prettyprint(sys(3),'tex','(',%F,%F) + 'U\cr Y &= ' + prettyprint(sys(4),'tex','(',%F,%F) + ' X ';
  try 
    if norm(sys(5),1) == 0 then
      str = str + '}\right.}';
    else
      str = str + ' + ' + prettyprint(sys(5),'tex','(',%F,%F) + '}\right.}';
    end
  catch
    str = str + ' + ' + prettyprint(sys(5),'tex','(',%F,%F) + '}\right.}';
  end
endfunction
        
function str = texmatrix(mat,delim)
  if argn(2) == 1 then
    delim = '(';
  end
  select delim
  case '[' then
    com = 'bmatrix';
  case '(' then
    com = 'pmatrix';
  case '|' then
    com = 'vmatrix';
  case '' then
    com = 'matrix';
  case '{' then
    com = 'Bmatrix';
  case '||' then
    com = 'Vmatrix';
  else
    com = 'pmatrix';
  end
  [m,n] = size(mat);
  str = '{\' + com + '{';
  if m*n == 0 then
    str = str + '\ ';
  else
    for i=1:m do
      str = str + strcat(mat(i,:),'&') + '\cr ';
    end
  end
  str = str + '}}';
endfunction   

//This function converts a complex (or a double) into a string in using the function d2s (double2string) fixed by mathmlconf or latexconf.
function str = comp2str(z,plus,minus,img,d2s)
  [m,n] = size(z);
  if m*n <> 1 then
    str = emptystr(z);
    for i=1:m do
      for j=1:n do
        str(i,j) = comp2str(z(i,j),plus,minus,img,d2s);
      end
    end
    return;
  end
  re = real(z);
  im = imag(z);
  if re == 0 & im == 0 then
    str = d2s(0);
    return;
  end;
  if re == 0 then
    if im == 1 then
      str = img;
    elseif im == -1 then
      str = minus + img;
    else
      str = d2s(im) + img;
    end
  elseif im == 0 then
    if re > 0 then
      str = d2s(re);
    else
      str = d2s(re);
    end
  elseif im > 0 then
    if im == 1 then
      str = d2s(re) + plus + img;
    else
      str = d2s(re) + plus + d2s(im) + img;
    end
  else
    if im == -1 then
      str = d2s(re) + minus + img;
    else
      str = d2s(re) + d2s(im) + img;
    end
  end
endfunction

//This function convert a complex into a coefficient (parenthesis are eventually added around the coeff, if var=='' (i.e. var^0), no parenthesis are added.
function str = comp2coef(z,var,plus,minus,img,op,cp,d2s)
  [m,n] = size(z);
  if m*n <> 1 then
    str = emptystr(z);
    for i=1:m do
      for j=1:n do
        str(i,j) = comp2coef(z(i,j),var,plus,minus,img,op,cp,d2s);
      end
    end
    return;
  end  
  re = real(z);
  im = imag(z);
  if var <> '' then 
    if re <> 0 & im <> 0 then
      str = plus + op + comp2str(z,plus,minus,img,d2s) + cp + var;
    elseif re == 0 & im == 0 then
      str = '';
    elseif re == 1 & im == 0 then
      str = plus + var;
    elseif re == -1 & im == 0 then
      str = minus + var;
    elseif re > 0 | im > 0 then
      str = plus + comp2str(z,plus,minus,img,d2s) + var;
    else
      str = comp2str(z,plus,minus,img,d2s) + var;
    end
  else
    if re == 0 & im == 0 then
      str = ''; 
    else
      str = comp2str(z,plus,minus,img,d2s);
    end
  end
endfunction

function str = unknown_type(typ,a,fmt)
  try
    execstr('str=' + typ + '2' + fmt + '(a)');
  catch
    error(msprintf(gettext("%s: Type %s is not handled : Define the function %s2%s."),"prettyprint",typ,typ,fmt))
  end
endfunction

