function [svar] = FormatStringsForDebugWatch(varargin)
// Converts input variable into a single string (not a matrix of strings),
// taking into account its type.
// This is used for the watch window of the debugger in Scipad.
// The output string is identical to what the user would have typed in the
// Scilab shell, apart from some extra characters needed to pass the string
// to TCL/TK.
// Author: François Vogel, 2004

  if argn(2) == 0 then error(39); else var = varargin(1); end

  tvar = type(var);

  if tvar<>1 & tvar<>2 & tvar<>4 & tvar<>5 & tvar<>6 & tvar<>8 & tvar<>10 & tvar<>15 & tvar<>16 & tvar<>17 then
    // unsupported cases
    error("type not handled (input argument is of type "+typeof(var)+")");

  else
    // supported cases
    svar = emptystr();

    if tvar == 6 then tvar = 5; end;  // this is to use the same code for sparse matrices and boolean sparse matrices

    listpref = emptystr();  // this is to use the same code for lists, tlists and mlists
    if tvar == 16 then tvar = 15; listpref = "t"; end;
    if tvar == 17 then tvar = 15; listpref = "m"; end;

    select tvar

    case 1 then  // real or complex matrix
      varstr = string(var);
      if prod(size(var)) > 1 then
        for i = 1:size(varstr,'r')
          oneline = strcat(varstr(i,:)," ");
          svar = svar + oneline + ";";
        end
        svar = part(svar,1:length(svar)-1);
        svar = "\[" + svar + "\]";
      elseif var == [] then
        svar = "\[\]";
      else
        svar = varstr;
      end

    case 2 then  // polynomial matrix
      if prod(size(var)) > 1 then
        svar = MatFormatStringsForDebugWatch(var);
      else
        co = strcat(string(coeff(var))," ");
        if degree(var) > 0 then
          // <TODO> search for the unknown could be improved using varn()
          varstr = string(var);
          poltok = tokens(varstr(2));
          unknown = part(poltok($),length(poltok($)));
        else
          unknown = "x";
        end
        svar = "poly(\[" + co + "\],\""" + unknown + "\"",\""coeff\"")";
      end

    case 4 then  // boolean matrix
      varstr = string(var);
      if prod(size(var)) > 1 then
        for i = 1:size(varstr,'r')
          oneline = strcat(varstr(i,:)," %");
          svar = svar + "%" + oneline + ";";
        end
        svar = part(svar,1:length(svar)-1);
        svar = "\[" + svar + "\]";
      else
        svar = "%" + varstr;
      end

    case 5 then  // sparse matrix, or boolean sparse matrix (type 6 changed into 5 above)
      [ij,v,mn]=spget(var);
      ind = FormatStringsForDebugWatch(ij);
      vec = FormatStringsForDebugWatch(v);
      dim = FormatStringsForDebugWatch(mn);
      svar = "sparse(" + ind + "," + vec + "," + dim + ")";

    case 8 then  // 1, 2 or 4-bytes integer matrix (this works for 1 to 10-bytes int or uint)
      if prod(size(var)) > 1 then
        svar = MatFormatStringsForDebugWatch(var);
        it = inttype(var);
        if it > 10 then it = it - 10; pre = "u"; else pre = emptystr(); end
        nbits = it*8;
        svar = pre + "int" + string(nbits) + "(" + svar + ")";
      else
        svar = string(var);
      end

    case 10 then  // character string matrix
      if prod(size(var)) > 1 then
        svar = MatFormatStringsForDebugWatch(var);
      else
        svar = strsubst(string(var),"\","\\");
        svar = "\""" + strsubst(svar,"''","''''") + "\""";
      end

    case 15 then  // list or tlist or mlist (types 16 and 17 changed into 15 above)
      if length(var) == 0 then svar = listpref + "list()";
      else
        for i = 1:length(var)
          // <TODO> search for the undefined items could be improved using definedfields()
          ie = execstr("getfield(i,var)","errcatch");  // catch the undefined items
          if ie <> 117 then
            svar = svar + FormatStringsForDebugWatch(getfield(i,var)) + ",";
          else
            svar = svar + "\""Undefined\"",";  // Well, this actually defines this element as a string
                                               // <TODO> find a solution for having a properly undefined element instead
          end
        end
        svar = part(svar,1:length(svar)-1);
        svar = listpref + "list(" + svar + ")";
      end

    end
    
  end

endfunction


function [svar] = MatFormatStringsForDebugWatch(var)
// Ancillary for FormatStringsForDebugWatch
// Calls FormatStringsForDebugWatch for each element of matrix var,
// and properly formats the resulting string as if the user would have
// typed it into the Scilab shell (apart from added \ chars)
  svar = emptystr();
  for i = 1:size(var,'r')
    oneline = emptystr();
    for j = 1:size(var,'c')
      oneline = oneline + FormatStringsForDebugWatch(var(i,j)) + " ";
    end
    svar = svar + part(oneline,1:length(oneline)-1) + ";";
  end
  svar = part(svar,1:length(svar)-1);
  svar = "\[" + svar + "\]";
endfunction
