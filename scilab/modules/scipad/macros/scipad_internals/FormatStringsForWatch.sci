function [svar,tysi,editable] = FormatStringsForWatch(var)
// Return three strings describing variable var in some way
// svar:
//   Convertion of variable var content into a single string (not a matrix
//   of strings), taking into account the type of var.
//   The string svar is identical to what the user would have typed in the
//   Scilab shell to define var, apart from some extra characters needed to
//   pass the string to Tcl/Tk.
// tysi:
//   A string containing the type and size of var
// editable:
//   A string being "true" if var can be edited by the user in the Scipad
//   debugger, and "false" otherwise
// All this is used for the watch window of the debugger in Scipad.
// Authors: François Vogel, 2004-2007, Enrico Segre 2007

  unklabel = "<?>"; // Warning: if this is changed it must be changed accordingly in db_init.tcl
  noedit_l = "<<";
  noedit_r = ">>";

  if execstr("tvar = type(var);","errcatch") <> 0 then
    warning(LocalizeForScipad(" what you try to watch is not supported by the debugger"));
    svar = unklabel;
    tysi = unklabel;
    editable = "false";

  else

    tysi = LocalizeForScipad("Type:") + " " + typeof(var) 
    
    if and(tvar<>[1 2 4 5 6 7 8 9 10 11 13 14 15 16 17 128 129 130]) then
      // unsupported cases
      warning(LocalizeForScipad(" what you try to watch is of type ")...
          +typeof(var)...
          +LocalizeForScipad(" - this type is not supported by the debugger"));
      svar = noedit_l+typeof(var)+" (type "+string(tvar)+")"+noedit_r;
      tysi = tysi + " " + LocalizeForScipad("(unsupported)")
      editable = "false";

    else
      // supported cases
      svar = emptystr();

      listpref = emptystr();  // this is to use the same code for lists, tlists and mlists
      if tvar == 16 then tvar = 15; listpref = "t"; end;
      if tvar == 17 then tvar = 15; listpref = "m"; end;

      // some common cathegorizatizations, factored out
      if or(tvar==[1 2 4 5 6 7 8 9 10]) then
        [nr,nc] = size(var);
        losi= LocalizeForScipad("size:") + " " + string(nr) + "x" + string(nc);
      end

      if or(tvar==[1 2 5]) then
        if isreal(var) then
          reco = LocalizeForScipad("real");
        else
          reco = LocalizeForScipad("complex");
        end
      end

      if or(tvar==[5 6 7]) then
        [ij,v,mn]=spget(var);
        ind = FormatStringsForWatch(ij);
        vec = FormatStringsForWatch(v);
        dim = FormatStringsForWatch(mn);
      end

      if or(tvar==[4 6 8 9 10]) then
        tysi = tysi + ", " + losi
      end

      editable = "true";
      if or(tvar==[9 11 13 14 128 129 130]) then
          editable = "false";
      end

      //case by case
      select tvar

      case 1 then  // real or complex matrix
        varstr = string(var);
        if isreal(var) then
          varstr=FormatInfNanForWatch(var,varstr)
        else
          // note bug 2409: there is no complex variable with inf imaginary part
          // which ~isnan
          a=isnan(var); b=imag(var(a))
          varstr(a)="%nan+%i*"+FormatInfNanForWatch(b,string(b))
          a=isinf(var); b=real(var(a));
          varstr(a)=FormatInfNanForWatch(b,string(b))+"+%i*"+string(imag(var(a)))
          // note "+%i*"+string(imag(var(a))) and not string(%i*imag(var(a)))
          // because string(0*%i)=="0"
        end
        svar=StringMatrixForWatch(varstr)
        tysi = tysi + " (" + reco + "), " + losi

      case 2 then  // polynomial matrix
        [n,m]=size(var)
        if n*m > 1 then
          //here we do have a growing string - I don't know how to do better
          varstr=emptystr(n,m)
          for i=1:n; for j=1:m
            varstr(i,j)=FormatStringsForWatch(var(i,j))
          end; end
          svar=StringMatrixForWatch(varstr)
        else
          co = strcat(string(coeff(var))," ");
          unknown = stripblanks(varn(var));  // stripblanks is no more required since cvs 26 May 05
          svar = "poly(\[" + co + "\],\""" + unknown + "\"",\""coeff\"")";
        end
        tysi = tysi + " (" + reco + "), " + losi

      case 4 then  // boolean matrix
        a=["%f" "%t"]; varstr=matrix(a(1+var),size(var));
        svar=StringMatrixForWatch(varstr)

      case 5 then  // sparse matrix
        svar = "sparse(" + ind + "," + vec + "," + dim + ")";
        tysi = tysi + " (" + reco + "), " + losi

      case 6 then  // boolean sparse matrix
        svar = "sparse(" + ind + "," + vec + "," + dim + ")";

      case 7 then  // Matlab sparse matrix
        svar = "mtlb_sparse(sparse(" + ind + "," + vec + "," + dim + "))";
        // isreal does not work with a Matlab sparse (%msp_isreal is not defined)
        // so use a detour
        if isreal(sparse(ij,v,mn)) then
          reco = LocalizeForScipad("real");
        else
          reco = LocalizeForScipad("complex");
        end
        tysi = tysi + " (" + reco + "), " + losi

      case 8 then  // 1, 2 or 4-bytes integer matrix (this works for 1 to 10-bytes int or uint)
        it = inttype(var);
        if it > 10 then it = it - 10; pre = "u"; else pre = emptystr(); end
        nbits = it*8;
        svar = pre + "int" +string(nbits)+ "(" + StringMatrixForWatch(string(var))+ ")";

      case 9 then  // graphic handle, we aren't yet able to display the content
        svar = noedit_l + LocalizeForScipad("graphic handle") + noedit_r

      case 10 then  // character string matrix
        svar = strsubst(var,"\","\\");
        svar = strsubst(svar,"[","\[");
        svar = strsubst(svar,"]","\]");
        svar = strsubst(svar,"""","\""\""");
        svar = StringMatrixForWatch("\"""+svar+"\""")
        svar = strsubst(svar,"''","''''");
        svar = strsubst(svar,"$","\$");
        svar = strsubst(svar,"{","\{");
        svar = strsubst(svar,"}","\}");

      case 11 then  // uncompiled function
        Vars = macrovar(var);
        svar = noedit_l + LocalizeForScipad("uncompiled function") + noedit_r + ...
               ":\[" + strcat(Vars(2)',",") + "]=...(" + strcat(Vars(1)',",") + ")"

      case 13 then  // compiled function
        Vars = macrovar(var)
        svar = noedit_l + LocalizeForScipad("compiled function") + noedit_r + ...
               ":\[" + strcat(Vars(2)',",") + "]=...(" + strcat(Vars(1)',",") + ")"

      case 14 then  // library
        s = string(var)
        svar = noedit_l + LocalizeForScipad("library") + noedit_r + ...
               ":" + strcat(s(2:$),",")

      case 15 then  // list or tlist or mlist (types 16 and 17 changed into 15 above)
        if length(var) == 0 then svar = listpref + "list()";
        else
          defs = definedfields(var);
          for i = 1:length(var)
            if find(defs==i) <> [] then
              [e_svar,e_tysi,e_edit] = FormatStringsForWatch(getfield(i,var));
              svar = svar + e_svar + ",";
              // a list is non editable as soon as one of its elements
              // (possibly itself nested in a sublist) is non editable
              if e_edit=="false" then editable = "false"; end
            else
              svar = svar + unklabel + ",";
            end
          end
          svar = part(svar,1:length(svar)-1);
          svar = listpref + "list(" + svar + ")";
        end
        tysi = tysi + ", " + ...
               LocalizeForScipad("size:") + " " + string(length(var)) + " " + LocalizeForScipad("elements");

      case 128 then  // pointer, e.g. a=rand(5,5);b=rand(5,1);A=sparse(a);[h,rk]=lufact(A);typeof(h), type(h)
        svar = noedit_l + LocalizeForScipad("pointer") + noedit_r

      case 129 then  // size implicit index, e.g. index=2:$; typeof(index), type(index)
        svar = noedit_l + LocalizeForScipad("size implicit index") + noedit_r + " " + sci2exp(var)

      case 130 then  // intrinsic function
        svar = noedit_l + LocalizeForScipad("primitive") + noedit_r

      end
    end
  end

endfunction

// Ancillaries for FormatStringsForWatch

function varstr = FormatInfNanForWatch(var,varstr)
// ancillary to replace Inf, Nan in real variables
// var and svar should be consistent! (svar=string(var))
      varstr(isnan(var))="%nan"
      varstr(var==%inf)="%inf"
      varstr(var==-%inf)="-%inf"
endfunction

function svar=StringMatrixForWatch(varstr)
// properly formats the resulting string matrix as if the user would have
// typed it into the Scilab shell (apart from added \ chars)
  if prod(size(varstr)) > 1 then
    if size(varstr,1)==1 then
      svar="\["+strcat(varstr," ")+"\]"
    elseif size(varstr,2)==1 then
      svar="\["+strcat(varstr,"; ")+"\]"
    else
      svar="\[" + strcat([varstr(1:$-1,1:$-1)+" " varstr(1:$-1,$)+"; "]') + ..
           strcat(varstr($,:)," ")+"\]";
    end
  elseif varstr == [] then
    svar = "\[\]";
  else
    svar = varstr;
  end
endfunction
