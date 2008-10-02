//  Scipad - programmer's editor and debugger for Scilab
//
//  Copyright (C) 2002 -      INRIA, Matthieu Philippe
//  Copyright (C) 2003-2006 - Weizmann Institute of Science, Enrico Segre
//  Copyright (C) 2004-2008 - Francois Vogel
//
//  Localization files ( in tcl/msg_files/) are copyright of the 
//  individual authors, listed in the header of each file
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file scipad/license.txt
//

function [svar,tysi,editable] = FormatStringsForWatch(var)
// Return three strings describing variable var in some way
// svar:
//   Conversion of variable var content into a single string (not a matrix
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
// Authors: François Vogel, 2004-2007, Enrico Segre 2007-2008

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
      svar = noedit_l+typeof(var)+" (type "+msprintf("%d",tvar)+")"+noedit_r;
      tysi = tysi + " " + LocalizeForScipad("(unsupported)")
      editable = "false";

    else
      // supported cases
      svar = emptystr();

      listpref = emptystr();  // this is to use the same code for lists, tlists and mlists
      if tvar == 16 then tvar = 15; listpref = "t"; end;
      if tvar == 17 then tvar = 15; listpref = "m"; end;

      // some common cathegorizations, factored out
      if or(tvar==[1 2 4 5 6 7 8 9 10]) then
        [nr,nc] = size(var);
        losi= LocalizeForScipad("size:") + " " + msprintf("%d",nr) + "x" + msprintf("%d",nc);
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
        varstr=MatrixToFullPrecisString(var)
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
          co = strcat(MatrixToFullPrecisString(coeff(var))," ");
          unknown = (varn(var));  // stripblanks is no more required since cvs 26 May 05
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
        svar = pre + "int" + msprintf("%d",nbits) + "(" + StringMatrixForWatch(MatrixToFullPrecisString(var))+ ")";
         
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
               LocalizeForScipad("size:") + " " + msprintf("%d",length(var)) + " " + LocalizeForScipad("elements");

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

function varstr=MatrixToFullPrecisString(var)
  //ancillary which replaces varstr=string(var), overcoming the 
  // following limitations (bugs 1317, 2784):
  // -output always guarantees full double precision accuracy,
  //  irrespectful of format(), but is as short as possible
  // -exponents larger than E100 are output in C, not in Fortran form
  // Performance is unfortunately worse than string() alone and possibly
  //  polynomial in the matrix size, despite the tour de force
  // See comments in bug 2784
  [nr,nc]=size(var)
  b=var(:);
  if b<>[] then
    if type(b)==8 | isreal(b) then //rely on short circuit, isreal() is not defined for int
      //there are cases (e.g. 1.1) in which %.17g gives a longer
      // form with trailing numerical noise. To see if the shorter
      // form is ok, compare them.
      s1=tokens(msprintf(" %.16g",b)); varstr=tokens(msprintf(" %.17g",b))
      t=(msscanf(nr,s1,"%lg")==msscanf(nr,varstr,"%lg"))
      // the comparison is ok even for inf, nan
      varstr(t)=s1(t);
      //recast inf, nan in input form
      varstr(isnan(b))="%nan"
      varstr(b==%inf)="%inf"
      varstr(b==-%inf)="-%inf"    
    else
    //the ancillary calls itself for real and imaginary part
      rvar=real(b); ivar=imag(b); aivar=abs(ivar)
      isign=sign(ivar); minusplus=["-","+"]; minusblank=["-",""]
      Re0=(rvar==0); Im0=(ivar==0); Im1=(aivar==1); Implus=((isign>0)+1)
      asign=minusplus(Implus)'
      aminus=minusblank(Implus)'
     //slightly inefficient to call MatrixToFullPrecisString twice to fill the initial
     // full array and then additionally to reduce it, but I don't want to break my head
      varstr=MatrixToFullPrecisString(rvar)+asign+MatrixToFullPrecisString(aivar)+"*%i"
      varstr(~Im1&Re0)=MatrixToFullPrecisString(ivar(~Im1&Re0))+"*%i"
      varstr(Im1&~Re0)=MatrixToFullPrecisString(rvar(Im1&~Re0))+asign(Im1&~Re0)+"%i"
      varstr(Im1&Re0)=aminus(Im1&Re0)+"%i"
      varstr(Im0)=MatrixToFullPrecisString(rvar(Im0))
      // complex numbers like a+%i*%inf, %inf+b*%i should be treated separately
      // Nan real part: note bug 2409: complex variables with %inf 
      //  imaginary part give isnan==%t
      // However, we have treated them above using separately Re and Im: ok anyway.  
    end
    varstr=matrix(varstr,nr,nc)
  else
    varstr=[]
  end
endfunction

