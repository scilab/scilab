function [svar,tysi] = FormatStringsForWatch(var)
// Return two strings describing variable var in some way
// svar:
//   Convertion of variable var content into a single string (not a matrix
//   of strings), taking into account the type of var.
//   The string svar is identical to what the user would have typed in the
//   Scilab shell to define var, apart from some extra characters needed to
//   pass the string to Tcl/Tk.
// tysi:
//   A string containing the type and size of var
// Both strings are used for the watch window of the debugger in Scipad.
// Author: François Vogel, 2004-2007

  unklabel   = "<?>"; // Warning: if this is changed it must be changed accordingly in db_init.tcl
  unsuptyp_l = "<<";  // Ditto
  unsuptyp_r = ">>";  // Ditto

  if execstr("tvar = type(var);","errcatch") <> 0 then
    warning(LocalizeForScipad(" what you try to watch is not supported by the debugger"));
    svar = unklabel;
    tysi = unklabel;

  else

    if and(tvar<>[1 2 4 5 6 8 9 10 11 13 14 15 16 17 128 129 130]) then
      // unsupported cases
      warning(LocalizeForScipad(" what you try to watch is of type ")...
          +typeof(var)...
          +LocalizeForScipad(" - this type is not supported by the debugger"));
      svar = unklabel;
      tysi = LocalizeForScipad("Type:") + " " + typeof(var) + " " + LocalizeForScipad("(unsupported)")

    else
      // supported cases
      svar = emptystr();
      tysi = emptystr();

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
        [nr,nc] = size(var);
        if isreal(var) then
          reco = LocalizeForScipad("real");
        else
          reco = LocalizeForScipad("complex");
        end
        tysi = LocalizeForScipad("Type:") + " " + typeof(var) + " (" + reco + "), " + ...
               LocalizeForScipad("size:") + " " + string(nr) + "x" + string(nc);

      case 2 then  // polynomial matrix
        if prod(size(var)) > 1 then
          svar = MatFormatStringsForWatch(var);
        else
          co = strcat(string(coeff(var))," ");
          unknown = stripblanks(varn(var));  // stripblanks is no more required since cvs 26 May 05
          svar = "poly(\[" + co + "\],\""" + unknown + "\"",\""coeff\"")";
        end
        [nr,nc] = size(var);
        if isreal(var) then
          reco = LocalizeForScipad("real");
        else
          reco = LocalizeForScipad("complex");
        end
        tysi = LocalizeForScipad("Type:") + " " + typeof(var) + " (" + reco + "), " + ...
               LocalizeForScipad("size:") + " " + string(nr) + "x" + string(nc);

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
        [nr,nc] = size(var);
        tysi = LocalizeForScipad("Type:") + " " + typeof(var) + ", " + ...
               LocalizeForScipad("size:") + " " + string(nr) + "x" + string(nc);

      case 5 then  // sparse matrix
        [ij,v,mn]=spget(var);
        ind = FormatStringsForWatch(ij);
        vec = FormatStringsForWatch(v);
        dim = FormatStringsForWatch(mn);
        svar = "sparse(" + ind + "," + vec + "," + dim + ")";
        [nr,nc] = size(var);
        if isreal(var) then
          reco = LocalizeForScipad("real");
        else
          reco = LocalizeForScipad("complex");
        end
        tysi = LocalizeForScipad("Type:") + " " + typeof(var) + " (" + reco + "), " + ...
               LocalizeForScipad("size:") + " " + string(nr) + "x" + string(nc);

      case 6 then  // boolean sparse matrix
        [ij,v,mn]=spget(var);
        ind = FormatStringsForWatch(ij);
        vec = FormatStringsForWatch(v);
        dim = FormatStringsForWatch(mn);
        svar = "sparse(" + ind + "," + vec + "," + dim + ")";
        [nr,nc] = size(var);
        tysi = LocalizeForScipad("Type:") + " " + typeof(var) + ", " + ...
               LocalizeForScipad("size:") + " " + string(nr) + "x" + string(nc);

      case 8 then  // 1, 2 or 4-bytes integer matrix (this works for 1 to 10-bytes int or uint)
        if prod(size(var)) > 1 then
          svar = MatFormatStringsForWatch(var);
          it = inttype(var);
          if it > 10 then it = it - 10; pre = "u"; else pre = emptystr(); end
          nbits = it*8;
          svar = pre + "int" + string(nbits) + "(" + svar + ")";
        else
          svar = string(var);
        end
        [nr,nc] = size(var);
        tysi = LocalizeForScipad("Type:") + " " + typeof(var) + ", " + ...
               LocalizeForScipad("size:") + " " + string(nr) + "x" + string(nc);

      case 9 then  // graphic handle, we aren't yet able to display the content
        svar = unsuptyp_l + LocalizeForScipad("graphic handle") + unsuptyp_r
        [nr,nc] = size(var);
        tysi = LocalizeForScipad("Type:") + " " + typeof(var) + ", " + ...
               LocalizeForScipad("size:") + " " + string(nr) + "x" + string(nc);
         
      case 10 then  // character string matrix
        if prod(size(var)) > 1 then
          svar = MatFormatStringsForWatch(var);
        else
          svar = strsubst(string(var),"\","\\");
          svar = strsubst(svar,"""","\""\""");
          svar = strsubst(svar,"''","''''");
          svar = strsubst(svar,"$","\$");
          svar = strsubst(svar,"{","\{");
          svar = strsubst(svar,"}","\}");
          svar = strsubst(svar,"[","\[");
          svar = strsubst(svar,"]","\]");
          svar = "\""" + svar + "\""";
        end
        [nr,nc] = size(var);
        tysi = LocalizeForScipad("Type:") + " " + typeof(var) + ", " + ...
               LocalizeForScipad("size:") + " " + string(nr) + "x" + string(nc);

      case 11 then  // uncompiled function
        Vars = macrovar(var);
        svar = unsuptyp_l + LocalizeForScipad("uncompiled function") + unsuptyp_r + ...
               "+\[" + strcat(Vars(2)',",") + "]=...(" + strcat(Vars(1)',",") + ")"
        tysi = LocalizeForScipad("Type:") + " " + typeof(var)

      case 13 then  // compiled function
        Vars = macrovar(var)
        svar = unsuptyp_l + LocalizeForScipad("compiled function") + unsuptyp_r + ...
               ":\[" + strcat(Vars(2)',",") + "]=...(" + strcat(Vars(1)',",") + ")"
        tysi = LocalizeForScipad("Type:") + " " + typeof(var)

      case 14 then  // library
        s = string(var)
        svar = unsuptyp_l + LocalizeForScipad("library") + unsuptyp_r + ...
               ":" + strcat(s(2:$),",")
        tysi = LocalizeForScipad("Type:") + " " + typeof(var)
        
      case 15 then  // list or tlist or mlist (types 16 and 17 changed into 15 above)
        if length(var) == 0 then svar = listpref + "list()";
        else
          defs = definedfields(var);
          for i = 1:length(var)
            if find(defs==i) <> [] then
              svar = svar + FormatStringsForWatch(getfield(i,var)) + ",";
            else
              svar = svar + unklabel + ",";
            end
          end
          svar = part(svar,1:length(svar)-1);
          svar = listpref + "list(" + svar + ")";
        end
        tysi = LocalizeForScipad("Type:") + " " + typeof(var) + ", " + ...
               LocalizeForScipad("size:") + " " + string(length(var)) + " " + LocalizeForScipad("elements");

      case 128 then  // pointer
        svar = unsuptyp_l + LocalizeForScipad("pointer") + unsuptyp_r
        tysi = LocalizeForScipad("Type:") + " " + typeof(var)

      case 129 then  // size implicit index?
        svar = unsuptyp_l + LocalizeForScipad("size implicit index") + unsuptyp_r
        tysi = LocalizeForScipad("Type:") + " " + typeof(var)

      case 130 then  // intrinsic function
        svar = unsuptyp_l + LocalizeForScipad("primitive") + unsuptyp_r
        tysi = LocalizeForScipad("Type:") + " " + typeof(var)


      end
    end
  end

endfunction


function [svar] = MatFormatStringsForWatch(var)
// Ancillary for FormatStringsForWatch
// Calls FormatStringsForWatch for each element of matrix var,
// and properly formats the resulting string as if the user would have
// typed it into the Scilab shell (apart from added \ chars)
  svar = emptystr();
  for i = 1:size(var,'r')
    oneline = emptystr();
    for j = 1:size(var,'c')
      oneline = oneline + FormatStringsForWatch(var(i,j)) + " ";
    end
    svar = svar + part(oneline,1:length(oneline)-1) + ";";
  end
  svar = part(svar,1:length(svar)-1);
  svar = "\[" + svar + "\]";
endfunction
