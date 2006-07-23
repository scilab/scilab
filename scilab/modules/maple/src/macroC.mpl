
####################################################################
#
#                    CAPOLSINI Patrick
#        INRIA - Universite de Nice - Sophia Antipolis
#
#                MACRO GENERATEUR DE CODE C
#                         C CODE GENERATION
#
#                                      1992
#
#             Version 1.0 for Maple  V
#
#           Comments and bug report are expected and welcome 
#                     capolsin@safir.unice.fr
#                     capolsin@sophia.inria.fr
#
#           Future extensions : ANSI C code generation
#
####################################################################
# Updated to Maple 6 and above by Claude Gomez
# claude.gomez@inria.fr
# 3 March 2004
####################################################################

####################################################################
#  C package functions redefinitions
####################################################################

# ENTER : Maple expression
# IMPRESSION : C corresponding code
`m_C/expression` := proc() local i,k,p,s,t,e;                  #modif

option `Copyright 1990 by the University of Waterloo`;
#e := args[1];
e := `m_C/ConvertAndCheck`(args[1]);                         #modif
if type(e, integer) then `codegen/C/write`(convert(e, name)) #modif
elif type(e, float) or
     (e = Pi or e = E or e = gamma or e = Catalan) then 
     `codegen/C/write`(`codegen/C/float`(evalf(e)))
elif type(e, `Boolean`) then          # modif
        `m_C/Bool`(e);
elif type(e, symbol) then `codegen/C/write`(e); ## modif
elif type(e,complex) then
 p:=convert(e,name); i:=SearchText(convert(I,name),p);
 `codegen/C/write`(cat(substring(p,1..i-1),`(sqrt(-1))`,substring(p,i+1..length(p))));
elif type(e,`*`) then

        if op(1,e) + 1 = 0 then
                `codegen/C/write`(`-`);
                p := nops(e) = 2 and `codegen/C/precedence`(op(2,e)) <= 50;
                if p then `codegen/C/write`(`(`) fi;
                `m_C/expression`(subsop(1=1,e));
                if p then `codegen/C/write`(`)`) fi;
                RETURN()
        elif type(op(1,e),fraction) and
                (op(1,op(1,e)) = 1 or op(1,op(1,e)) = -1) then
                if op(1,e) < 0 then `codegen/C/write`(`-`) fi;
                p := nops(e) = 2 and `codegen/C/precedence`(op(2,e)) <= 50;
                if p then `codegen/C/write`(`(`) fi;
                `m_C/expression`(subsop(1=1,e));
                if p then `codegen/C/write`(`)`) fi;
                `codegen/C/write`(`/`||(op(2,op(1,e))));
                RETURN()
        fi;

        p := `codegen/C/precedence`(e,1);
        if p then `codegen/C/write`(`(`) fi;
        `m_C/expression`(op(1,e));
        if p then `codegen/C/write`(`)`) fi;
        for k from 2 to nops(e) do
                t := op(k,e);
                if type(t,`^`) and type(op(2,t),numeric)
                        and sign(op(2,t)) = -1
                then s := op(1,t)^(-op(2,t)); `codegen/C/write`(`/`)
                else s := t; `codegen/C/write`(`*`)
                fi;
                p := `codegen/C/precedence`(s) < `codegen/C/precedence`(e) or
        type(t,name^integer) and op(2,t) < -1 and op(2,t) > -4;
                if p then `codegen/C/write`(`(`) fi;
                `m_C/expression`(s);
                if p then `codegen/C/write`(`)`) fi;
        od

elif type(e,`^`) then

        if type(op(2,e),rational) and op(2,e) < 0 then
                p := `codegen/C/precedence`(op(1,e)) < 70 or
        type(e,name^integer) and op(2,e) > -4 and op(2,e) < -1;
                `codegen/C/write`(`1/`);
                if p then `codegen/C/write`(`(`) fi;
                `m_C/expression`(op(1,e)^(-op(2,e)));
                if p then `codegen/C/write`(`)`) fi;
        elif type(e,name^integer) and op(2,e) < 4 then
                # See C/precedence
                 `m_C/expression`(op(1,e));
                `codegen/C/write`(`*`);
                 `m_C/expression`(op(1,e));
                if op(2,e) > 2 then `codegen/C/write`( cat(`*`,op(1,e)) ) fi;
        elif type(op(2,e),fraction) and op(2,op(2,e))=2 then
                s := op(1,e)^op(1,op(2,e));
                `m_C/expression`('sqrt'(s));    
        else
                `codegen/C/write`(`pow(`);
                `m_C/expression`(op(1,e));
                `codegen/C/write`('`,`');
                `m_C/expression`(evalf(op(2,e)));
                `codegen/C/write`('`)`');
        fi

elif type(e,`+`) then
      
        p := `codegen/C/precedence`(e,1);
        if p then `codegen/C/write`(`(`) fi;
        `m_C/expression`(op(1,e));
        if p then `codegen/C/write`(`)`) fi;
        for k from 2 to nops(e) do
                if not (type(op(k,e),numeric) and op(k,e) < 0 or
                        type(op(k,e),`*`) and type(op(1,op(k,e)),numeric)
                        and sign(op(1,op(k,e))) = -1)
                then `codegen/C/write`(`+`) fi;
                p := `codegen/C/precedence`(e,k);
                if p then `codegen/C/write`(`(`) fi;
                `m_C/expression`(op(k,e));
                if p then `codegen/C/write`(`)`) fi;
        od
elif type(e, affect) then                      #modif
        `m_C/expression`(op(1, e));
        `codegen/C/write`(cat(` `, 
                      substring(op(0, e), 2..length(op(0,e))),
                      ` `));
	`m_C/expression`(op(2, e));

#elif type(e, `Boolean`) then                  # modif
#        `m_C/Bool`(e);

elif type(e,function) and type(op(0,e),name) then
#        e := `m_C/ConvertAndCheck`(e);           #modif
        if op(0,e) = 'RootOf' then
                ERROR(`refusing to convert RootOf's. \
You must substitute all RootOf's for the root that you want. \
The library routine fsolve may be useful for computing approximations to them`)
        fi;

        if type(e,'ln(GAMMA(anything))') then
                RETURN( `m_C/expression`(lnGAMMA(op(1,op(1,e)))) )
        fi;
        if op(0, e) = `irem` then                 #modif
          `m_C/expression`(op(1, e));
          `codegen/C/write`(`%`);
          `m_C/expression`(op(2, e));
	  RETURN(NULL);
        fi;
        p := `codegen/C/function_name`(op(0,e),nops(e));
        if not type(p,name) then
                RETURN( `m_C/expression`( p(op(e)) ) )
        fi;
        `m_C/expression`( `codegen/C/function_name`(op(0,e),nops(e)) );
        `codegen/C/write`(`(`);
        if nops(e) > 0 then
            `m_C/expression`(op(1,e));
            for k from 2 to nops(e) do
                `codegen/C/write`(`,`);
                `m_C/expression`(op(k,e));
            od;
        fi;
        `codegen/C/write`(`)`);

elif type(e,indexed) and type(op(0,e),name) then
        `codegen/C/write`(op(0, e));
        for k in e do                  
           `codegen/C/write`(`[`);
           `m_C/expression`(k+shift);   # add value of "shift" to indices
           `codegen/C/write`(`]`);
        od;  

elif type(e,fraction) then
        `m_C/expression`(op(1,e));
        `codegen/C/write`('`/`');
        `m_C/expression`(op(2,e));


else ERROR(`unable to translate`,e) 
fi

end:


`m_C/ConvertAndCheck` := proc(x) local f,a,dummy;
	option `Copyright 1990 by the University of Waterloo`;
	if type(x, {name,numeric,complex}) then x
	elif type(x, function) then
		f := op(0,x);
		a := map( procname, [op(x)] );
		if f = 'csc' then 1/sin(op(a))
		elif f = 'sec' then 1/cos(op(a))
		elif f = 'cot' then 1/tan(op(a))
		elif f = 'csch' then 1/sinh(op(a))
		elif f = 'sech' then 1/cosh(op(a))
		elif f = 'coth' then 1/tanh(op(a))
		elif f = 'arccsc' then 1/arcsin(op(a))
		elif f = 'arcsec' then 1/arccos(op(a))
		elif f = 'arccot' then 1/arctan(op(a))
		elif f = 'arccsch' then 1/arcsinh(op(a))
		elif f = 'arcsech' then 1/arccosh(op(a))
		elif f = 'arccoth' then 1/arctanh(op(a))
		else subs(dummy=f,dummy(op(a)))
		fi
	else map( procname, x )
	fi
end:

##############################
# New function 
##############################
# ENTER : A Maple boolean expression or OR, AND, NOT
# EXIT : C corresponding code
`m_C/Bool` := proc(e)	
local i;

    if type(e, function) then
      if op(0, e) = 'OR' then
        `codegen/C/write`(`(`);
        for i to nops(e)-1 do
           `m_C/expression`(op(i, e));
           `codegen/C/write`(` || `);
        od;
        `m_C/expression`(op(nops(e), e));
        `codegen/C/write`(`)`);
      elif op(0, e) = 'AND' then
        `codegen/C/write`(`(`);
	for i to nops(e)-1 do
           `m_C/expression`(op(i, e));
           `codegen/C/write`(` && `);
        od;
        `m_C/expression`(op(nops(e), e));
	`codegen/C/write`(`)`);
      elif op(0, e) = 'NOT' then
        `codegen/C/write`(`!`);
        `m_C/expression`(op(e));
      fi;   
    elif type(e, `=`) then
      `codegen/C/write`(`(`);             
      `m_C/expression`(op(1, e));
      `codegen/C/write`(` == `);
      `m_C/expression`(op(2, e));
      `codegen/C/write`(`)`);
    elif type(e, `<`) then            
      `codegen/C/write`(`(`);
      `m_C/expression`(op(1, e));
      `codegen/C/write`(` < `);
      `m_C/expression`(op(2, e));
      `codegen/C/write`(`)`);
    elif type(e, `<=`) then           
      `codegen/C/write`(`(`);
      `m_C/expression`(op(1, e));
      `codegen/C/write`(` <= `);
      `m_C/expression`(op(2, e));
      `codegen/C/write`(`)`);
    elif type(e, `<>`) then           
      `codegen/C/write`(`(`);
      `m_C/expression`(op(1, e));
      `codegen/C/write`(` != `);
      `m_C/expression`(op(2, e));
      `codegen/C/write`(`)`);
    elif type(e, `and`) then
      `codegen/C/write`(`(`);
      `m_C/expression`(op(1, e));
      `codegen/C/write`(` && `);
      `m_C/expression`(op(2, e));
      `codegen/C/write`(`)`);
    elif type(e, `or`) then
      `codegen/C/write`(`(`);
      `m_C/expression`(op(1, e));
      `codegen/C/write`(` || `);
      `m_C/expression`(op(2, e));
      `codegen/C/write`(`)`);
    elif type(e, `not`) then
      `codegen/C/write`(`!`);
      `m_C/expression`(op(e));
    elif e = true then
       `codegen/C/write`(`1`);
    elif e = false then
       `codegen/C/write`(`0`);
    fi;
end:

################################################################
#                Expansion with optimized option 
################################################################
# ENTER : array, equalC or &= and optimized option 
# EXIT : single or single sequence or expression sequence
`m_C/expand` := proc(l)
local statseq, statseq2, l_vars, x, i;

    if type(l, array) then
      statseq := [codegen[optimize](l)];
    else
      if op(1, l) = `equalC` then       # l=[equalC, ...]
        x := `&=`(op(2, l), op(3, l));
      elif type(l, list(affect)) then   # l=[&?=(...), &?=(...), ...]
        x := convert(l, array);         # an array to use optimize
      fi;
      statseq := [codegen[optimize](x)];
    fi;
    l_vars := [];
    statseq2 := [];
    for i to nops(statseq) do
         if type(statseq[i],indexed = affect) then       # t[i] = &?=...
           statseq2 := [statseq2[], [op(2, statseq[i])]];
         elif type(statseq[i], indexed = anything) then    # t[i] = ...
           statseq2 := [statseq2[], [convert(statseq[i], `&=`)]]; 
         elif type(statseq[i], {optvar = equation,            # t0 = t1=...
			      optvar = affect}) then           # t0 = &?=...
           statseq2 := [statseq2[], [op(2, statseq[i])]];
         elif type(statseq[i], optvar = anything) then     # t0 = ...
           l_vars := [l_vars[], op(1, statseq[i])];
           statseq2 := [statseq2[], [convert(statseq[i], `&=`)]];
         fi;
    od;

    if assigned(autodeclare) and l_vars <> [] then
      [declarem, autodeclare, l_vars], op(statseq2);
    else
      op(statseq2);
    fi;
end:

###################################################################
#                  macroC : basic procedures
###################################################################

# ENTER : NULL
# SIDE EFFECT : initialize global variables
`init_genC` := proc()
    global optimized, precision, autodeclare, shift;
    optimized := false;
    precision := single;
    autodeclare := 'autodeclare';
    shift := -1;
    NULL;
end:

# ENTER : macroC expressions list
# IMPRESSION : C corresponding code
genC := proc()
global `codegen/C/filename`;
local  old_Digits, old_quiet, old_labelling, old_screenwidth, res;

# Patch `C/arrayexpand` and `optimize/array`
#`optimize/array` := eval(`macroC/optimize/array`);
#`C/arrayexpand` := eval(`macroC/C/arrayexpand`);

    old_Digits := Digits;
    old_quiet := interface(quiet);
    old_labelling := interface(labelling);
    old_screenwidth := interface(screenwidth);
    interface(quiet=true);
    interface(labelling=false);
    interface(screenwidth=80);
    if assigned(precision) then
      if precision = `single` then
        Digits := 7;                 
      elif precision = `double` then
        Digits := 16;
      else
        interface(quiet=old_quiet);
        interface(labelling=old_labelling);
        ERROR(`Bad value for precision`, precision);
      fi;
    fi;

    if nargs = 1 and                        # genC(l)
       type(args[1], {list, list(list)}) then
      `codegen/C/filename`:=terminal;
      res := traperror(`macroC/genC`(args[1], 0));
    elif nargs = 2 and                      # genC(ind, l)
         type(args[2], {list, list(list)}) then
      `codegen/C/filename`:=terminal;
      res := traperror(`macroC/genC`(args[2], args[1]));
    elif nargs = 2 and                      # genC(l, filename=toto)
         type(args[1], {list, list(list)}) and
         type(args[2], `=`) then      
      `codegen/C/filename`:=convert(op(2, args[2]), name);           
      res := traperror(`macroC/genC`(args[1], 0));
    elif nargs = 3 and                      # genC(ind, l, filename=toto)
         type(args[2], {list, list(list)}) and
         type(args[3], `=`) then
      `codegen/C/filename`:=convert(op(2, args[3]), name);           
      res := traperror(`macroC/genC`(args[2], args[1]));
    else 
      res := traperror(ERROR(`Bad number or type of arguments`));
    fi;
    if res = lasterror then 
      print(lasterror);
    fi;
    interface(quiet=old_quiet);
    interface(labelling=old_labelling);
    interface(screenwidth=old_screenwidth);
    Digits := old_Digits;
    NULL;   
end:

# ENTER : a macro/single list or a unique macro/single
# EXIT : a single sequence
# macros expansion
`macroC/expand` := proc(l)

    if l = [] then RETURN(l) fi;
    if type(l, list) then
      if (op(1, l) = `equalC` or type(op(1, l), affect)) and
           assigned(optimized) and optimized then   # optimize option
        `m_C/expand`(l);      
      elif op(1, l) = `matrixm` then
        `macroC/matrixm`(op(2..nops(l), l));
      elif `macroC/macro`[op(1, l)] = true then       # a macro
        op(map(`macroC/expand`, [`macroC/`||(op(1, l))(op(2..nops(l), l))]));
      else                         # a single or macro/single list
        map(`macroC/expand`, l);      
      fi;
    else
      l;
    fi;
end:

# Extract declarations
# ENTER : * l    : a single/macro list
# EXIT : a macro/single sequence in decl paranmeter
# SIDE EFFECT : declarem macros of a block in decl
`macroC/extract_decl` := proc(l, decl)

    if l = NULL or l = [] then RETURN(l) fi;
    if type(l, list) then
      if op(1, l) = `declarem` then   # a [declarem, ...]
        decl := [op(eval(decl)), subsop(1=`declareC`, l)];
        RETURN(NULL);
      elif `macroC/macro`[op(1, l)] = true then        # a macro
        `macroC/extract_decl`(`macroC/expand`(l), 'decl');
      elif op(1, l) = `blockC` then   # Do not go down through blocks
        RETURN(l);
      else
        map(`macroC/extract_decl`, l, 'decl');
      fi;
    else
      l;
    fi;
end:

# ENTER : a macroC instructions list and an indentation 
# IMPRESSION : C corresponding code (after macros expansion)
`macroC/genC` := proc(l, ind)
global  `codegen/C/buffer`;
local i, l_single;
    `codegen/C/buffer` := '``';
    l_single := [`macroC/expand`(l)];
    for i in l_single do
       `macroC/mac`(i, ind);
    od;
end:

# ENTER : a single list or expression list
# IMPRESSION :  C corresponding code
`macroC/mac` := proc(l, ind)
local f;
      if l = [] then                  # write a white line
        `codegen/C/writeln`(`macroC/indent`(ind));
      elif type(l, list(list)) then
        map(`macroC/mac`, l, ind);
      else
        f := `macroC/`||(op(1, l));    # a single
        if type(f, procedure) then      
          f(ind, op(2..nops(l), l));
        else		    	      # an instruction
          `codegen/C/write`(`macroC/indent`(ind));
          `m_C/expression`(op(l));
          `codegen/C/writeln`(`;`);
        fi;
      fi;
end:

# ENTER : an integer
# EXIT : a string containing n white spaces
`macroC/indent` := proc(n)
    cat(` `$n);
end:

# ENTER : an expression list
# IMPRESSION : translated expressions separated by commas
`macroC/enumeration` := proc()
local i;

    if nargs > 1 then
      for i from 2 to nargs - 1 do
         `m_C/expression`(args[i]);
         `codegen/C/write`(`,`);
      od;
      `m_C/expression`(args[nargs]);
    fi;
end:

# [{C]
# IMPRESSION : {
`macroC/{C` :=proc(ind)
    `codegen/C/writeln`(cat(`macroC/indent`(ind+2), `{`));
end:

# [}C]
# IMPRESSION : }
`macroC/}C` :=proc(ind)
    `codegen/C/writeln`(cat(`macroC/indent`(ind+2), `}`));
end:

# [spaceC, n]
# IMPRESSION : n spaces
`macroC/spaceC` := proc(ind, n)
    `codegen/C/write`(`macroC/indent`(n));
end:

# MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO
# [blockm, l]
# ENTER : see above
# EXIT : a single sequence
`macroC/macro`[`blockm`] := true:
`macroC/blockm` := proc(l)
    [`{C`], [blockC, l], [`}C`];
end:

# ENTER : a macroC instruction list
# IMPRESSION : corresponding code between { and }
#  optional macro declarations at the beginning of the block
`macroC/single`[`blockC`] := true:
`macroC/blockC` := proc(ind, l)
local decl, l1;

    decl := [];
    l1 := `macroC/extract_decl`(l, 'decl');
    `macroC/mac`(decl, ind+2);
    `macroC/mac`(l1, ind+2);
end:

# New &= operator to represent assignment
# ENTER : operator's arguments
# EXIT : a maple expression (function)
`&=` := proc()
local e, var, q, res, x;
options remember;

    var := args[1];
    e := args[2];
    if type(e, `+`) and 
       (member(var, [op(e)]) or member(-var, [op(e)])) then
      res := op(2, op(solve(e = var+x, {x})));
      if res = 1 then
        RETURN(cat(var, `++`));
      elif res = -1 then
        RETURN(cat(var, `--`));
      else
        RETURN(`&+=`(var, res));
      fi;
    elif type(e, `*`) and divide(e, var, 'q') then
       RETURN(`&*=`(var, q));
    fi;
    'procname'(var, e);
end:

# ENTER : an expression
# EXIT : true if a &=, &+=, &-=, &*=, &/=,
# &%=, &>>=, &<<=, &&=, &^=, &|= function
`type/affect` := proc(e)
    type(e, function) and member(op(0, e), 
                                 {`&=`, `&+=`, `&-=`, `&*=`, `&/=`,`&%=`,
                                  `&>>=`,`&<<=`, `&&=`, `&^=`, `&|=`});
end:

# ENTER : an expression
# EXIT : true if expression is a maple boolean or OR, NOT, AND
`type/Boolean` := proc(e)
    type(e, `boolean`) or 
    (type(e, function) and member(op(0, e), {OR, AND, NOT}));
end:

# ENTER : anequation
# EXIT : a &=
`convert/&=` := proc(x)
    if not(type(x, equation)) then
      ERROR(`Unable to convert`);
    fi;
    `&=`(op(1, x), op(2, x));
end:

# to detect a variable generated by optimize
# ENTER : a name
# EXIT : true if the name begins with a "t" and is followed by a number
`type/optvar` := proc(s)
    type(s, name) and substring(s, 1..1) = `t` and 
    member(substring(s, 2..2), {`0`,`1`,`2`,`3`,`4`,`5`,`6`,`7`,`8`,`9`});
end:

# ENTER : a string
# EXIT : string between " "
`macroC/string` := proc(s)
    cat(` "`, convert(s, name), `" `);
end:
    
#######################################
#               comments
#######################################
# [commentC, comment]
# ENTER : see above
# IMPRESSION : string between /* and */
`macroC/single`[`commentC`] := true:
`macroC/commentC` := proc(ind, message)

    if length(message) = 0 then 
      `codegen/C/writeln`();
    else 
      `codegen/C/writeln`(cat(`macroC/indent`(ind), `/*`, 
                      convert(message, name), `*/`));
    fi;
end:

##################################################
#          declarations
##################################################
# [declareC, <static, automatic, extern, ...>, type, [vars]]
# ENTER : see above
# IMPRESSION : varaibales declarations according to the specified types
# to declare "byte field" ==> maple string : [declareC,
#      `unsigned int`, [`field : 4`]]
`macroC/single`[`declareC`] := true:
`macroC/declareC` := proc()
local l, ind;

    ind := args[1];
    if nargs = 4 then            # 2 types
      `codegen/C/write`(cat(`macroC/indent`(ind), args[2], ` `, args[3], ` `));
      l := args[4];
    elif nargs = 3 then          # 1 type
      `codegen/C/write`(cat(`macroC/indent`(ind), args[2], ` `));
      l := args[3];
    else                         # no type
      `codegen/C/write`(`macroC/indent`(ind));
      l := args[2];
    fi;
    `macroC/enumeration`(ind, op(l));
    `codegen/C/writeln`(`;`);
end:

# [declarem, <static, automatic, extern, ...>, type, [vars]]
# same behavior as [declareC, ...]
`macroC/declarem` := proc()

    `macroC/declareC`(args);
end:

# [structC, <name>, [[type1, [names]], [type2, [names]], ... ],
#                    <[ident1, ident2, ... ]>]
# ENTER : see above
# IMPRESSION : a C struct
# NB : to define an extern or static ==> a maple string including the word
#      static ==> [structC, `static toto`, ...]
`macroC/single`[`structC`] := true:
`macroC/structC` := proc()
local ind, struct, l_vars;

    ind := args[1];
    if type(args[2], name) then
      if nargs = 4 then              # a name and a vars list
        struct := args[3];
        l_vars := args[4];
      else                           # a name and no vars
        struct := args[3];
        l_vars := [];
      fi;
      `codegen/C/writeln`(cat(`macroC/indent`(ind), `struct `, args[2]));
    else 
      if nargs = 3 then             # no name and a vars list
        struct := args[2];
        l_vars := args[3];
      else	  		    # no name and no vars list (C error)
        struct := args[2];
        l_vars := [];
      fi;
      `codegen/C/writeln`(cat(`macroC/indent`(ind), `struct `));
    fi;
    `codegen/C/writeln`(cat(`macroC/indent`(ind+2), `{`)); # struct block
    `macroC/mac`(map(proc(x) [declareC, op(x)] end, struct), ind+4);
    if l_vars <> [] then                           # end of struct and  vars
      `codegen/C/write`(cat(`macroC/indent`(ind+2), `} `));
      `macroC/enumeration`(ind, op(l_vars));
      `codegen/C/writeln`(`;`);
    else
      `codegen/C/writeln`(cat(`macroC/indent`(ind+2), `};`));
    fi;
end:

# [unionC, <name>, [[type1, [names]], [type2, [names]], ... ],
#                 <[ident1, ident2, ... ]>]
# ENTER : see above
# IMPRESSION : a union definition
# NB : for an extern or static ==> a string including the word 
#      static ==> [unionC, `static toto`, ...]
`macroC/single`[`unionC`] := true:
`macroC/unionC` := proc()
local ind, struct, l_vars;

    ind := args[1];
    if type(args[2], name) then
      if nargs = 4 then              # a name and a vars list
        struct := args[3];
        l_vars := args[4];
      else                           # a name and no vars
        struct := args[3];
        l_vars := [];
      fi;
      `codegen/C/writeln`(cat(`macroC/indent`(ind), `union `, args[2]));
    else 
      if nargs = 3 then             # no name and a vars list
        struct := args[2];
        l_vars := args[3];
      else	  		    # no name and no vars (C error)
        struct := args[2];
        l_vars := [];
      fi;
      `codegen/C/writeln`(cat(`macroC/indent`(ind), `union `));
    fi;
    `codegen/C/writeln`(cat(`macroC/indent`(ind+2), `{`)); # union block
    `macroC/mac`(map(proc(x) [declareC, op(x)] end, struct), ind+4);
    if l_vars <> [] then                           # union end and vars
      `codegen/C/write`(cat(`macroC/indent`(ind+2), `} `));
      `macroC/enumeration`(ind, op(l_vars));
      `codegen/C/writeln`(`;`);
    else
      `codegen/C/writeln`(cat(`macroC/indent`(ind+2), `};`));
    fi;
end:

# [enumC, <name>, [var1, var2, ...], <[ident1, ident2, ...]>]
# ENTER : see above
# IMPRESSION : a enumerated type definition
`macroC/single`[`enumC`] := true:
`macroC/enumC` := proc()
local ind, ens, l_vars;

    ind := args[1];
    if type(args[2], name) then
      if nargs = 4 then             # a name and a vars list
        ens := args[3];
        l_vars := args[4];
      else                          # a name and no vars
        ens := args[3];
        l_vars := [];
      fi;
      `codegen/C/write`(cat(`macroC/indent`(ind), `enum `, args[2], ` {`));
    else
      if nargs = 3 then             # no name and a vars list
        ens := args[2];
        l_vars := args[3];
      else                          # no name and no vars 
        ens := args[2];
        l_vars := [];
      fi;
      `codegen/C/write`(cat(`macroC/indent`(ind), `enum `, `{`));
    fi;
    `macroC/enumeration`(ind, op(ens));  # enum block
    `codegen/C/write`(`} `);
    `macroC/enumeration`(ind, op(l_vars));
    `codegen/C/writeln`(`;`);
end:

# [typedefC, type, ident] or
# [typedefC, definition struct, <ident>]
# ENTER : see above
# IMPRESSION : new type definition either by a simple manner or by
#             a union, a struct or an enum
`macroC/single`[`typedefC`] := true:
`macroC/typedefC` := proc(ind, ty, ident)

    if type(ty, name) and type(ident, name) then
      `codegen/C/writeln`(cat(`macroC/indent`(ind), 
                      `typedef `, ty, ` `, ident, `;`));
    else
      `codegen/C/write`(cat(`macroC/indent`(ind), `typedef `));
      `macroC/mac`([op(ty), [ident]], ind);
    fi;
end:

#####################################################################
#     preprocessor commands : include, define, undef, ....
#####################################################################
# [includeC, filename]
# ENTER : see above
# IMPRESSION : file inclusion of "filename"(surrounded by <> or "") 
`macroC/single`[`includeC`] := true:
`macroC/includeC` := proc(ind, nom)

    `codegen/C/writeln`(cat(`# include `, convert(nom, name)));
end:

# [defineC, ident, var]
# ENTER : see above
# IMPRESSION : global varaible assignment
`macroC/single`[`defineC`] := true:
`macroC/defineC` := proc(ind, nom, expr)

    `codegen/C/write`(cat(`# define `, convert(nom, name), ` `));
    `m_C/expression`(expr);
    `codegen/C/writeln`();
end:

# [undefC, ident]
# ENTER : see above
# IMPRESSION : desassignment
`macroC/single`[`undefC`] := true:
`macroC/undefC` := proc(ind, nom)

    `codegen/C/writeln`(cat(`# undef `, convert(nom, name)));
end:

# [ifdefC, ident]
# ENTER : see above
# IMPRESSION : global variables assignment test
`macroC/single`[`ifdefC`] := true:
`macroC/ifdefC` := proc(ind, nom)

    `codegen/C/writeln`(cat(`# ifdef `, convert(nom, name)));
end:

# [#elseC, ident]
# ENTER : see above
# IMPRESSION :  # else
`macroC/single`[`#elseC`] := true:
`macroC/#elseC` := proc(ind)

    `codegen/C/writeln`(cat(`# else `));
end:

# [#endifC, ident]
# ENTER : see above
# IMPRESSION :  # endif
`macroC/single`[`#endifC`] := true:
`macroC/#endifC` := proc(ind)

    `codegen/C/writeln`(cat(`# endif `));
end:

# MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO
# [ifdefm, ident, [actions1], <[actions2]>]
# ENTER : see above
# EXIT : a single sequence
`macroC/macro`[`ifdefm`] := true:
`macroC/ifdefm` := proc(nom, l1, l2)

    if nargs = 2 then
      [ifdefC, nom], l1, [`#endifC`];
    elif nargs = 3 then
      [ifdefC, nom], l1, [`#elseC`], l2, [`#endifC`];
    fi;
end:

# [ifndefC, ident]
# ENTER : see above
# IMPRESSION : global variables non assignment test
`macroC/single`[`ifndefC`] := true:
`macroC/ifndefC` := proc(ind, nom)

    `codegen/C/writeln`(cat(`# ifndef `, convert(nom, name)));
end:

# MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO
# [ifndefm, ident, [actions1], <[actions2]>] 
# ENTER : see above
# EXIT : single sequence 
`macroC/macro`[`ifndefm`] := true:
`macroC/ifndefm` := proc(nom, l1, l2)

    if nargs = 2 then
      [ifndefC, nom], l1, [`#endifC`];
    elif nargs = 3 then
      [ifndefC, nom], l1, [`#elseC`], l2, [`#endifC`];
    fi;
end:

# [ifC, expr]
# ENTER : see above
# IMPRESSION : expression test
`macroC/single`[`ifC`] := true:
`macroC/ifC` := proc(ind, expr)

    `codegen/C/write`(`# if `);
    `m_C/expression`(expr);
    `codegen/C/writeln`(` `);
end:

# [elifC, expr]
# ENTER : see above
# IMPRESSION : # elif expr
`macroC/single`[`elifC`] := true:
`macroC/elifC` := proc(ind, expr)

    `codegen/C/write`(`# elif `);
    `m_C/expression`(expr);
    `codegen/C/writeln`(` `);
end:

# MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO
# [elifm, expr, [actions]]
# ENTER : see above
# EXIT : single sequence
`macroC/macro`[`elifm`] := true:
`macroC/elifm` := proc(expr, l)

    [[elifC, expr], l];
end:

# MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO
# [ifm, expr, [actions1], <[actions2]>] 
# ENTER : see above
# EXIT : single sequence
# NB : for #elif,  [actionsi] has to be a [elifm, ...]
`macroC/macro`[`ifm`] := true:
`macroC/ifm` := proc(expr, l1, l2)

    if nargs = 2 then
      [ifC, expr], l1, [`#endifC`];
    elif nargs = 3 then
      [ifC, expr], l1, [`#elseC`], l2, [`#endifC`];
    fi;
end:

# [lineC, number, <string>]
# ENTER : see above
# IMPRESSION : #line number <filename>
`macroC/single`[`lineC`] := true:
`macroC/lineC` := proc(ind, nb, chaine)

    if nargs = 2 then
      `codegen/C/writeln`(cat(`# line `, nb));
    else
      `codegen/C/writeln`(cat(`# line `, nb, `macroC/string`(chaine)));
    fi;
end:

# [errorC, string]
# ENTER : see above
# IMPRESSION : #error string
`macroC/single`[`errorC`] := true:
`macroC/errorC` := proc(ind, chaine)

    `codegen/C/writeln`(cat(`# error`, `macroC/string`(chaine)));
end:

# [pragmaC, string]
# ENTER : see above
# IMPRESSION : #pragma string
`macroC/single`[`pragmaC`] := true:
`macroC/pragmaC` := proc(ind, chaine)

    `codegen/C/writeln`(cat(`# pragma `, chaine));
end:

#####################################
#         assignment
#####################################
# [equalC, ident, expression]
# ENTER : see above
# IMPRESSION : corresponding assignement
`macroC/single`[`equalC`] := true:
`macroC/equalC` := proc(ind, nom, e)

    `codegen/C/write`(`macroC/indent`(ind));
    `m_C/expression`(`&=`(nom, e));
    `codegen/C/writeln`(`;`);
end:

#####################################
#           matrix handling
#####################################
# MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO
# [matrixm, ident, array-name]
# [matrixm, ident, array-operators]
# ENTER : see above
# EXIT : a single sequence
`macroC/macro`[`matrixm`] := true:     
`macroC/matrixm` := proc(nom, e)
local x;

    x := subs(_dummy = nom, proc() local _dummy; _dummy end)();
    if type(e, 'name') and type(e, 'array') then
      assign(x, op(e));
    else
      assign(x, e);
    fi;
    if assigned(optimized) and optimized then
      `m_C/expand`(x);
    else
      op(map(proc(x) [convert(x, `&=`)] end, [`codegen/C/arrayexpand`(x)]));
    fi;
end:

###############################################
#               functions
###############################################
# [callC, nom, [parameters]]
# ENTER : see above
# IMPRESSION : function call with parameters
`macroC/single`[`callC`] := true:
`macroC/callC` := proc(ind, nom, l)

    `m_C/expression`(cat(`macroC/indent`(ind), nom)(op(l)));
    `codegen/C/writeln`(`;`);
end:

# [functionC, <type>, nom, [[type1, [params1]], [type2, [params2]], ...]]
# ENTER : see above
# IMPRESSION : C code for function declaration
`macroC/single`[`functionC`] := true:
`macroC/functionC` := proc()
local i, params, ind, typ, nom, l_params;

    ind := args[1];
    if nargs = 4 then           # with type
      typ := cat(args[2], ` `);
      nom := args[3];
      l_params := args[4];
    elif nargs = 3 then         # no type
      typ := NULL;
      nom := args[2];
      l_params := args[3];
    fi;
    `codegen/C/write`(cat(`macroC/indent`(ind), typ, nom, `(`)); # function heading
    if l_params <> [] then 
      params := map(proc(x) local param, i;         # parameters list
                      if type(x, indexed) then
                        param := op(0, x);
                      else 
                        param := x;
                      fi;
		      for i while (substring(param, 1..1) = `*`) do
                        param := substring(param, 2..length(param));
                      od;
                      param; 
                    end, 
                    map(proc(x) op(op(2, x)) end, l_params));
      `macroC/enumeration`(ind, op(params));
    fi;
    `codegen/C/writeln`(`)`);
    for i in l_params do          # parameters declaration
       `macroC/declareC`(ind, i[1], i[2]);
    od;
end:

# MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO
# [functionm, type, name, [[type1, [params_type1]], 
#                         [type2, [params_type2]], ...],
#		         [instructions]]
# ENTER : see above
# EXIT : single sequence 
`macroC/macro`[`functionm`] := true:
`macroC/functionm` := proc()

    [functionC, args[1..(nargs-1)]], [blockm, args[nargs]];
end:

###############################################
#          Loop exits and labels
###############################################
# [returnC, <expression>]
# ENTER : see above
# IMPRESSION : C code for return
`macroC/single`[`returnC`] := true:
`macroC/returnC` := proc()

    if nargs = 1 then
      `codegen/C/writeln`(cat(`macroC/indent`(args[1]), `return;`));
    else
      `codegen/C/write`(cat(`macroC/indent`(args[1]), `return (`));
      `m_C/expression`(args[2]);
      `codegen/C/writeln`(`);`);
    fi;
end:

# [breakC]
# ENTER : see above
# IMPRESSION : C code for break
`macroC/single`[`breakC`] := true:
`macroC/breakC` := proc (ind)

   `codegen/C/writeln`(cat(`macroC/indent`(ind), `break;`));
end:

# [continueC]
# ENTER : see above
# IMPRESSION : C code for continue
`macroC/single`[`continueC`] := true:
`macroC/continueC` := proc (ind)

   `codegen/C/writeln`(cat(`macroC/indent`(ind), `continue;`));
end: 

# [gotoC, ident]
# ENTER : see above
# IMPRESSION : C code for goto
`macroC/single`[`gotoC`] := true:
`macroC/gotoC` := proc (ind, ident)

   `codegen/C/writeln`(cat(`macroC/indent`(ind), `goto `, ident, `;`));
end: 

# [labelC, ident]
# ENTER : see above
# IMPRESSION : C code C a label definition
`macroC/single`[`labelC`] := true:
`macroC/labelC` := proc(ind, ident)

    `codegen/C/writeln`(cat(`macroC/indent`(ind), ident, `:`));
end:

# MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO
# [labelm, ident, l]
# ENTER : see above
# EXIT :  a single sequence
`macroC/macro`[`labelm`] := true:
`macroC/labelm` := proc(ident, l)
local res;

    res := [labelC, ident];
    if type(l, list(list)) then          # macros list
      res := res, [blockm, l];
    elif type(l, list) and               
         assigned(optimized) and 
         optimized then         
      res := res, [blockm, [l]];
    else                	       # macro or expression
      res := res, [spaceC, 2], l;
    fi;
    res;
end:

#####################################################
#             Iterations : for, while, do while
#####################################################
# [forC, expr1, expr2, expr3]
# ENTER : see above
# IMPRESSION : C code C for for loop
# NB : <expri> may be `` ==> for(;;)
`macroC/single`[`forC`] := true:
`macroC/forC` := proc(ind, expr1, expr2, expr3)

    `codegen/C/write`(cat(`macroC/indent`(ind), `for`, `(`));
    `m_C/expression`(expr1);
    `codegen/C/write`(`;`);
    `m_C/expression`(expr2);
    `codegen/C/write`(`;`);
    `m_C/expression`(expr3);
    `codegen/C/writeln`(`)`);
end:

# MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO
# [form, expr1, expr2, expr3, [action]]
# ENTER : see above
# EXIT : a single sequence
`macroC/macro`[`form`] := true:
`macroC/form` := proc(expr1, expr2, expr3, l)
local res;

    res := [forC, expr1, expr2, expr3];
    if type(l, list(list)) then          # macros list 
      res := res, [blockm, l];
    elif type(l, list) and               
         assigned(optimized) and 
         optimized then         
      res := res, [blockm, [l]];
    else                	       # macro or expression
      res := res, [spaceC, 2], l;
    fi;
    res;
end:

# [whileC, expr]
# ENTER : see above
# IMPRESSION : C code C for while loop
`macroC/single`[`whileC`] := true:
`macroC/whileC` := proc(ind, expr)

    `codegen/C/write`(cat(`macroC/indent`(ind), `while`, `(`));
    `m_C/expression`(expr);
    `codegen/C/writeln`(`)`);
end:

# [doC]
# ENTER : see above
# IMPRESSION : C code for do ... while loop
`macroC/single`[`doC`] := true:
`macroC/doC` := proc(ind)

    `codegen/C/writeln`(cat(`macroC/indent`(ind), `do`));
end:

# MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO
# [whilem, expr, [action]]
# ENTER : see above
# EXIT : a single sequence
`macroC/macro`[`whilem`] := true:
`macroC/whilem` := proc(expr, l)
local res;

    res := [whileC, expr];
    if type(l, list(list)) then          # macros list 
      res := res, [blockm, l];
    elif type(l, list) and               
         assigned(optimized) and 
         optimized then         
      res := res, [blockm, [l]];    
    else                	       # macro or expression
      res := res, [spaceC, 2], l;
    fi;
    res;
end:

# MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO
# [dowhilem, [action], expr]
# ENTER : see above
# EXIT : a single sequence
`macroC/macro`[`dowhilem`] := true:
`macroC/dowhilem` := proc(expr, l)
local res;

    res := [doC];
    if type(l, list(list)) then          # macros list
      res := res, [blockm, l];
    elif type(l, list) and               
         assigned(optimized) and 
         optimized then         
      res := res, [blockm, [l]];
    else                	       # macro or expression
      res := res, [spaceC, 2], l;
    fi;
    res, [whileC, expr];
end:

##########################################################
#             Les tests conditionnels : if, switch, case
##########################################################

# [if_thenC, expr]
# ENTER : see above
# IMPRESSION : C code for if instruction
`macroC/single`[`if_thenC`] := true:
`macroC/if_thenC` := proc(ind, expr)

    `codegen/C/write`(cat(`macroC/indent`(ind), `if`, `(`));
    `m_C/expression`(expr);
    `codegen/C/writeln`(`)`); 
end:

# MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO
# [if_thenm, expr, [action]]
# ENTER : see above
# EXIT : a single sequence
`macroC/macro`[`if_thenm`] := true:
`macroC/if_thenm` := proc(expr, l)
local res;

    res := [if_thenC, expr];
    if type(l, list(list)) then          # macros list
      res := res, [blockm, l];
    elif type(l, list) and               
         assigned(optimized) and 
         optimized then         
      res := res, [blockm, [l]];
    else                	       # macro or expression
      res := res, [spaceC, 2], l;
    fi;
    res;
end:

# [elseC]
# ENTER : see above
# IMPRESSION : else
`macroC/single`[`elseC`] := true:
`macroC/elseC` := proc(ind)

    `codegen/C/writeln`(cat(`macroC/indent`(ind), `else `));
end:

# MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO
# [if_then_elsem, expr, [actions1], [actions2]]
# ENTER : see above
# EXIT : a single sequence
`macroC/macro`[`if_then_elsem`] := true:
`macroC/if_then_elsem` := proc(expr, l1, l2)
local res;

    res := `macroC/if_thenm`(expr, l1), [elseC];
    if type(l2, list(list)) then          # macros list
      res := res, [blockm, l2];
    elif type(l2, list) and
         assigned(optimized) and 
         optimized then         
      res := res, [blockm, [l2]];
    else                	       # macro or expression
      res := res, [spaceC, 2], l2;
    fi;
    res;
end:

# [caseC, expr, [actions]]
# ENTER : see above
# IMPRESSION : C code C for case instruction
`macroC/single`[`caseC`] := true:
`macroC/caseC` := proc(ind, expr, l)

    `codegen/C/write`(cat(`macroC/indent`(ind), `case `));
    `m_C/expression`(expr);
    `codegen/C/writeln`(` :`);
    if type(l, list(list)) then         # macros list
       `macroC/mac`(l, ind+2); 
    else                		# macro or expression
      `macroC/mac`(l, ind+2);
    fi;
end:

# [defaultC, [actions]]
# ENTER : see above
# IMPRESSION : C code for default: instruction
`macroC/single`[`defaultC`] := true:
`macroC/defaultC` := proc(ind, l)

    `codegen/C/writeln`(cat(`macroC/indent`(ind), `default :`));
    if type(l, list(list)) then               # macros list
       `macroC/mac`(l, ind+2);
    else                                      # macro or e
      `macroC/mac`(l, ind+2);
    fi;
end:

# [switchC, expr]
# ENTER : see above
# IMPRESSION : switch(expr)
`macroC/single`[`switchC`] := true:
`macroC/switchC` := proc(ind, expr)

    `codegen/C/write`(cat(`macroC/indent`(ind), `switch (`));
    `m_C/expression`(expr);
    `codegen/C/writeln`(`)`);
end:

# MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO
# [switchm, expr, [caseC, case1, [actions]], ..., [defaultC, [actions]]
# ENTER : see above
# EXIT : a single sequence
`macroC/macro`[`switchm`] := true:
`macroC/switchm` := proc(expr, l)

    [switchC, expr], [blockm, l];
end:


##################################################################
#
#                    SOME ROUTINES
#
##################################################################

###############################################
#            input/ouput
##############################################
# [fopenC, pointeur-name, filename, status]
# ENTER : see above
# IMPRESSION : C code for a file opening
`macroC/single`[`fopenC`] := true:
`macroC/fopenC` := proc(ind, np, nf, s)

    `codegen/C/write`(cat(`macroC/indent`(ind), np, ` = `));
    `codegen/C/writeln`(cat(`fopen`, `(`, `macroC/string`(nf), `,`, `macroC/string`(s), `)`, `;`));
end:

# [fcloseC, pointeur-name]
# ENTER : see above
# IMPRESSION : C code for a file closing
`macroC/single`[`fcloseC`] := true:
`macroC/fcloseC` := proc(ind, np)

    `codegen/C/writeln`(cat(`macroC/indent`(ind), `fclose`, `(`, np, `)`, `;`)); 
end:

# MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO
# [fopenm, pointeur-name, filename, status, [instructions]]
# ENTER : see above
# EXIT : single sequence
`macroC/macro`[`fopenm`] := true:
`macroC/fopenm` := proc(np, nf, s, l)

    [fopenC, np, nf, s], l, [fcloseC, np];
end:


##################################################################
#                    Maple bugs with arrays
##################################################################
# gives back all indices eventhough options are present
indices2 := proc(A)
local evalA, i, j;

    if op(1, eval(A)) = NULL then
      indices(A);
    else
      evalA := [op(eval(A))];
      if type(evalA[3], `..`) then # a matrix
        seq(seq([i,j],j=1..op(2, evalA[3])),i=1..op(2, evalA[2]));
      else                         # a vector
        seq([i], i=1..op(2, evalA[2]));
      fi;
    fi;
end:

# To avoid maple's bug with optioned array 
# "indices2" in spite of "indices"
`macroC/C/arrayexpand` := proc(A)
option `Copyright 1989 by the University of Waterloo`;
        if not (type(A,'name') and type(A,'array')) then RETURN(A) fi;
        op( map( proc(x,A) local dummy;
                subs( dummy=op(x), 'A[dummy]' ) = A[op(x)] end,
                [indices2(A)], A ) )                      #modif
end:

# To avoid maple's bug with optioned array 
# "indices2" in spite of "indices"
`macroC/optimize/array` := proc(a) local k, n, last, inds, res, stat, stsq, A;
    option `Copyright 1990 by the University of Waterloo`;
    if not type(a,name) then ERROR(`named array expected`) fi;
    inds := [indices2(a)];
    stat := A( seq(a[op(inds[k])], k = 1..nops(inds)) );
    stsq := [codegen[optimize](stat)];
    n := nops(stsq);
    last := op(n,stsq);
    if not type(last,equation) then ERROR(`bug in optimize`) fi;
    last := op(2,last);
    for k to nops(last) do res[k] := A[op(inds[k])] = op(k,last) od;
    res := subs(A=a, [seq(res[k], k = 1..nops(last))]);
    stsq[1..n-1], op(res)
end:

#####################################################################
#                 convert problems
#
#        Claude Gomez's changes (macrofort share package)
#
#####################################################################
`convert/string` := proc(x)
if nargs=0 then ``
elif nargs>1 then ``||(`convert/string`(x))||`,`||(`convert/string`(
	args[2..nargs] ) )
else
	if type(x,'symbol') then x
	elif type(x,'integer') then ``||x
	elif type(x,'rational') then
		``||(`convert/string`(op(1,x)))||`/`||(`convert/string`(op(2,x)))
	elif type(x,'float') then
        ``||(_convertnamefloat(x))
	elif type(x,`^`) then `_convertstring^`(x)
	elif type(x,`*`) then `_convertstring*`(x)
	elif type(x,`+`) then `_convertstring+`(x)
	elif type(x,'list') then `[`||(`convert/string`(op(x)))||`]`
	elif type(x,'set') then `{`||(`convert/string`(op(x)))||`}`
	elif type(x,'indexed') then ``||(`convert/string`(op(0,x)))||
		`[`||(`convert/string`(op(x)))||`]`
	elif type(x,'function') then ``||(op(0,x))||`(`||(`convert/string`(op(x)))||`)`
	elif type(x,`=`) then ``||(`convert/string`(op(1,x)))||`=`||
		(`convert/string`(op(2,x)))
	elif type(x,`..`) then ``||(`convert/string`(op(1,x)))||`..`||
		(`convert/string`(op(2,x)))
	elif type(x,`<`) then ``||(`convert/string`(op(1,x)))||`<`||
		(`convert/string`(op(2,x)))
	elif type(x,`<=`) then ``||(`convert/string`(op(1,x)))||`<=`||
		(`convert/string`(op(2,x)))
	elif type(x,`<>`) then ``||(`convert/string`(op(1,x)))||`<>`||
		(`convert/string`(op(2,x)))
	elif type(x,`and`) then `(`||(`convert/string`(op(1,x)))||` and `||
		(`convert/string`(op(2,x)))||`)`
	elif type(x,`or`) then `(`||(`convert/string`(op(1,x)))||` or `||
		(`convert/string`(op(2,x)))||`)`
	elif type(x,`not`) then ` not `||(`convert/string`(op(1,x)))
	elif type(x,`.`) then ``||(`convert/string`(op(1,x)))||`.`||
		(`convert/string`(op(2,x)))
	else
		ERROR(`unable to convert to string`)
	fi
fi
end:

_convertnamefloat := proc( f )
local old, s;
option `Copyright 1989 by the University of Waterloo`;
old := Digits;
Digits := max( old, length( trunc(f) ) + 1 );
if f < 0 then
	s := `-`||(trunc(-f))||`.`||(_convertnamefrac(frac(-f)));
else
	s := ``||(trunc(f))||`.`||(_convertnamefrac(frac(f)));
fi;
Digits := old;
RETURN( s );
end:

_convertnamefrac := proc(r)
option `Copyright 1989 by the University of Waterloo`;
if r = 0 then ``
elif r >= 0.1 then ``||(trunc(10*r))||(_convertnamefrac(10*r-trunc(10*r)))
else `0`|| (_convertnamefrac(10*r)) fi
end:

`_convertstring^` := proc(x)
	local p;
	p := op(2,x);
	if type(p,'name') or (type(p,'integer') and p>0) then
	  ``||(`convert/string`(op(1,x)))||`^`||p
	else ``||(`convert/string`(op(1,x)))||`^(`||(`convert/string`(p))||`)`
	fi
end:

`_convertstring*` := proc(x)
	local i,t,p;
	t := `(`||(`convert/string`(op(1,x)));
	if nops(x) = 2 then
		p := op(2,x);
		if type(p,`^`) and op(2,p) = -1 then
		  ``||(t)||`/`||(`convert/string`(op(1,p)))||`)`
		else ``||(t)||`*`||(`convert/string`(p))||`)`
		fi;
	else for i from 2 to nops(x) do
		t := ``||(t)||`*`||(`convert/string`(op(i,x)))
	      od;
	      ``||t||`)`
	fi
end:

#############################################
#    My own changes to Claude Gomez's changes
#############################################
`_convertstring+` := proc(x)
        local i,t;
        t := `(`||(`convert/string`(op(1,x)));
        for i from 2 to nops(x) do
           if type(op(i, x), `*`) and op(1, op(i, x)) = -1 then
             t := ``||(t)||`-`||(`convert/string`(op(2, op(i,x))))
           elif op(i, x) = -1 then
             t := ``||(t)||`-1`
           else
             t := ``||(t)||`+`||(`convert/string`(op(i,x)))
           fi;
        od;
        ``||t||`)`
end:

# Initialisation
init_genC():

####################################################################
#                   help file
####################################################################
#/usr/local/mapleV/bin.ds3100/helptomaple help/text/macroC < macroC.help > toto
`help/text/macroC` := TEXT(
`HELP FOR: The macroC package for the generation of C code within Maple`,
`   `,
`FUNCTIONS: init_genC  - initialization routine`,
`           genC       - C code generation`,
`   `,
`CALLING SEQUENCE:`,
`   init_genC();`,
`   genC(<indent>, l, <filename=foo>); `,
`   `,
`PARAMETERS:`,
`   indent - (optional) integer `,
`   l      - list of macro describing the program`,
`   foo    - (optional) a filename`,
`   `,
`SYNOPSIS:   `,
`- The syntax and use of this package is as close as possible as the`,
`  macrofort package written by Claude Gomez (INRIA - France).`,
`   `,
`- The procedure init_genC initialize the global variables "precision",`,
`  "optimized" and "autodeclare".`,
`   `,
`- The global variable "precision" can be assigned either to single`,
`  (default) or double and indicates the number of significant digits`,
`  for the floating point variables. `,
`   `,
`- The global variable "shift" can be assigned an integer (default -1)`,
`  which indicates the value to add to array indices (see example).`,
`   `,
`- The global variable optimized can be set to "false" (default) or`,
`  "true". It indicates that common subexpressions research is performed.`,
`  The resulting sequence of assignments uses the global names t0, t1, ...`,
`   `,
`- When using the global variable optimized, intermediate variables t1,`,
`  ... may have been used but not declared within the program. To avoid`,
`  this problem, macroC can handle the declaration of the intermediate`,
`  variables used. The user have to set the global variable autodeclare`,
`  (default not assigned) to a name representing the type of the`,
`  variables t1 ... (see example)`,
`   `,
`- The genC procedure generates the C code. The optional parameter`,
`  indent specifies the amount of blank spaces at the beginning of each`,
`  line of code. If indent is ommited, the value 0 is set by default.`,
`  An optional last parameter of the form filename = foo can be`,
`  specified to direct the output to the file foo.`,
`  The parameter l is a list made of macroC single or macro instructions`,
`  describing the C program. A macroC single or macro expression is a`,
`  list where the first element is a keyword describing the C statement`,
`  (generally  the C instruction name followed by a C for a single`,
`  instruction or a m for a macro instruction) and parameters`,
`  (eventually optional) for the instruction. `,
`  Parameters given between < > are optional.`,
`   `,
`Single and Macro instructions             C code`,
`=======================================================================`,
`   `,
`Preprocessor`,
`============`,
`[includeC,file_name]		--> # include file_name`,
`[defineC,ident, var]		--> # define ident expr`,
`[undefC,ident]			--> # undefine ident`,
`[ifdefC,ident]			--> # ifdef ident`,
`[ifndefC,ident]			--> # ifndef ident`,
`[ifC,expr]			--> # if expr`,
`[elifC,expr]			--> # elif expr`,
`[lineC,num,<file_name>]		--> # line num <"file_name">`,
`[errorC,string]			--> # error "string"`,
`[pragmaC,string]		--> # pragma "string"`,
`   `,
`[ifdefm,ident,[actions], `,
`         <[actions]>]	    	--> # ifdef ident`,
`				    action1;`,
`                                    action2;`,
`				       ...`,
`				    # else`,
`				    action3;`,
`				    action4;`,
`				       ...`,
`				    # endif`,
`[ifndefm,ident,[actions], `,
`          <[actions]>]	    	--> # ifndef ident`,
`				    action1;`,
`				    action2;`,
`				       ...`,
`				    # else`,
`				    action3;`,
`				    action4;`,
`				       ...`,
`				    # endif`,
`[ifm,expr,[actions], `,
`      <[actions]>]     		--> # if expr`,
`				    action1;`,
`				    # else`,
`				    action2;`,
`				    action3;`,
`				       ...`,
`				    # endif`,
`   `,
`[elifm,expr,[actions]]		--> # elif expr`,
`				    action1;`,
`				    action2;`,
`				       ...`,
`Declarations`,
`============`,
`[declareC,<type>,type,[vars]]   --> <type> type var1,var2,...;`,
`[structC,<name>,[[type1,[vars]], `,
`                 [type2,[vars]], `,
`                 ... ],`,
`                <[ident1,...]>] --> struct <name> `,
`                                      {`,
`                                         type1 var1,var2,...;`,
`                                         type2 var3,var4,...;`,
`                                      } <ident1,ident2,...>;`,
`[unionC,<name>,[[type1,[vars]], `,
`                [type2,[vars]], `,
`                ... ],`,
`               <[ident1,...]>]  --> union <name> `,
`                                      {`,
`                                         type1 var1,var2,...;`,
`                                         type2 var3,var4,...;`,
`                                      } <ident1,ident2,...>;`,
`   `,
`[enumC,<name>,[vars], `,
`              <[ident1,...]>]   --> enum <name> {var1,...} <ident1,...>;`,
`[typedefC,type,ident]		--> typedef type ident;`,
`[typedefC,struct-union-enum,`,
`          <ident>]		--> typedef struct-union-enum`,
`                                      {`,
`				      	...`,
`				      } <ident>;`,
`   `,
`[declarem,<type>,type,[vars]]   --> <type> type var1,var2,...;`,
`   `,
`Misc   `,
`====   `,
`[]                              -->            (blank line)`,
`[commentC,<string>]		--> /*string*/ `,
`[equalC,ident,expr]		--> ident = expr;`,
`[callC,name,[vars]]		--> name(var1,var2,...);`,
`[functionC,<type>,name, `,
`            [[type1,[vars]], `,
`             [type2,[vars]],`,
`	     ...]]   		--> <type> name(var1,var2,var3,var4)`,
`				    type1 var1,var2;`,
`				    type2 var3,var4;`,
`[matrixm,ident,array]		--> ident[i][j] = array[i,j];`,
`				             ...`,
`[functionm,<type>,name, `,
`            [[type1,[vars]], `,
`             [type2,[vars]],`,
`	     ...],`,
`            [actions]] 		--> <type> name(var1,var2,var3,var4)`,
`				    type1 var1,var2;`,
`				    type2 var3,var4;`,
`				      {`,
`				        action1;`,
`					action2;`,
`					   ...`,
`				      }`,
`   `,
`Conditions   `,
`==========   `,
`[if_thenC,expr]			--> if(expr)`,
`[caseC,expr,[actions]]		--> case expr :`,
`                                      action1;`,
`                                      action2;`,
`                                        ...`,
`[defaultC,[actions]]		--> default :`,
`                                      action1;`,
`                                      action2;`,
`                                        ...  `,
`   `,
`[if_thenm,expr,[actions]]	--> if(expr)`,
`				      {`,
`				        action1;`,
`					action2;`,
`					   ...`,
`				      }`,
`[if_then_elsem,expr,[actions], `,
`               [actions]]	 --> if(expr)`,
`				       {`,
`				         action1;`,
`					 action2;`,
`					    ...`,
`				       }`,
`				     else`,
`				       {`,
`				         action3;`,
`					 action4;`,
`					   ...`,
`				       }`,
`[switchm,expr,`,
`    [[caseC,case,[actions]], `,
`            ...`,
`     [defaultC,[actions]]]]	--> switch (expr)`,
`				      {`,
`				        case case1 :`,
`					  action1;`,
`					  action2;`,
`					     ...`,
`					default :`,
`				          action3;`,
`				          action4;`,
`					     ...`,
`				      }`,
`   `,
`Iterations   `,
`==========   `,
`[forC,expr1,expr2,expr3]	--> for(expr1;expr2;expr3)`,
`[whileC,expr]			--> while(expr)`,
`[form,expr1,expr2,expr3,`,
`      [actions]]		--> for(expr1;expr2;expr3)`,
`				      {`,
`				        action1;`,
`				        action2;`,
`					   ...`,
`				      }`,
`[whilem,expr,[actions]]		--> while(expr)`,
`				      {`,
`				        action1;`,
`					action2;`,
`					   ...`,
`				      }`,
`[dowhilem,expr,[actions]]	--> do`,
`				      {`,
`				        action1;`,
`					action2;`,
`				      }`,
`				    while (expr);`,
`   `,
`Exits and labels`,
`================`,
`[returnC,<expr>]		--> return(<expr>);`,
`[breakC]			--> break;`,
`[continueC]			--> continue;`,
`[gotoC,ident]			--> goto ident;`,
`[labelC,ident]			--> ident:`,
`   `,
`[labelm,ident,[actions]]	--> ident:`,
`				      {`,
`				        action1;`,
`					action2;`,
`					   ...`,
`				      }`,
`   `,
`Input - output`,
`==============`,
`[fopenC,pt_name,file_name,`,
`        status]			--> pt_name = fopen( "file_name" , "status" );`,
`[fcloseC,pt_name]		--> fclose(pt_name);`,
`[fopenm,pt_name,file_name,`,
`        status, [actions]]	--> pt_name = fopen( "file_name" , "status" );`,
`				    action1;`,
`				    action2;`,
`				    fclose(pt_name);`,
`   `,
`   `,
`- The declarem macro has exactly the same meaning as the declareC`,
`  single instruction but can be put anywhere within a block.`,
`   `,
`- All array indices are shifted by default but this may be customized`,
`  by the global variable "shift" (see examples below).`,
`   `,
`- A "string" function is provided wich returns its argument between "`,
`  and ". This can be very usefull for C functions dealing with`,
`  strings.   `,
`   `,
`- MacroC arguments are Maple names and have the same evaluation rules`,
`  at toplevel.`,
`   `,
`- Special Maple function name (csc, cot, sech, ...) are translated`,
`  into the C corresponding expression.`,
`   `,
`- An assignment operator is provided : ``&=``. This operator will`,
`  translate any assignment into the correct C syntax using new operators`,
`  as ``&+=``, ``&*=``, ``++`` or ``--`` (see examples).`,
`   `,
`- Boolean relations (``=``, ``<>``, ``<``, ``<=``) can be used with Maple`,
`  syntax and will be translated into C appropriate syntax. Logical`,
`  expressions (``and``, ``or``, ``not``) will be translated too but after`,
`  Maple boolean evaluation (such that (a<>b) or (a<d) is evaluated to`,
`  true) ! To avoid this problem we provide three prefixed operators`,
`  ``OR``, ``AND`` and ``NOT`` such that the previous expression will be`,
`  translated into the correct C statement.`,
`   `,
`   `,
`EXAMPLES:   `,
`> read ``macroC``;`,
`> mat:=array(1..3,1..3, [[csh(x), ln(x), Pi],[arccot(a), f(b), 4.56],`,
`> [g(x^45+1), 3/x^2, a-b]]);`,
`   `,
`                             [   csh(x)    ln(x)    Pi  ]`,
`                             [                          ]`,
`                             [  arccot(a)   f(b)   4.56 ]`,
`                             [                          ]`,
`                      mat := [    45          3         ]`,
`                             [ g(x   + 1)   ----  a - b ]`,
`                             [                2         ]`,
`                             [               x          ]`,
`   `,
`# Indices are shifted by default`,
`> genC([matrixm, toto, mat]);`,
`toto[1][0] = 1/atan(a);`,
`toto[1][1] = f(b);`,
`toto[1][2] = 0.456E1;`,
`toto[2][0] = g(pow(x,45)+1);`,
`toto[2][1] = 3/(x*x);`,
`toto[2][2] = a-b;`,
`toto[0][0] = csh(x);`,
`toto[0][1] = log(x);`,
`toto[0][2] = 0.3141593E1;`,
`   `,
`# Unshifted indices`,
`> shift:=0:`,
`   `,
`> genC([matrixm, toto, mat]);`,
`toto[3][1] = g(pow(x,45.0)+1);`,
`toto[3][2] = 3/(x*x);`,
`toto[3][3] = a-b;`,
`toto[1][2] = log(x);`,
`toto[1][3] = 0.3141593E1;`,
`toto[2][1] = 1/atan(a);`,
`toto[2][2] = f(b);`,
`toto[2][3] = 0.456E1;`,
`toto[1][1] = csh(x);`,
`   `,
`> string(``%lf\\n``);`,
`   `,
`                                     "%lfn"`,
`   `,
`> genC([equalC, bool, (a<>b) or (a<d)]);`,
`bool = 1;   `,
`   `,
`> genC([equalC, bool, OR(a<>b, a<d)]);`,
`bool = ((a != b) || (a < d));`,
`   `,
`> l := [   `,
`> [includeC, ``<math.h>``],`,
`> [includeC, ``<stdio.h>``],`,
`> [commentC, ````],`,
`> [commentC, `` Cholesky method ``],`,
`> [commentC, ````],`,
`> [functionm, void, cholesky, [[double, [``**M``, ``**res``]],`,
`>                              [int, n]],`,
`>   [ [form, &=(j,0), j<=n-1,&=(j,j+1),`,
`>          [[equalC, tmp, 0.0],`,
`>           [form, &=(k,0), k<=j-1,&=(k,k+1),`,
`>                [&=(tmp, tmp+(res[j+1,k+1]^2))]`,
`>           ],`,
`>           [equalC, res[j+1,j+1], sqrt(M[j+1,j+1]-tmp)],`,
`>           [form, &=(i,j+1), i<=n-1, &=(i,i+1),`,
`>                [[equalC, tmp, 0.0],`,
`>                 [form, &=(k,0), k<=j-1, &=(k,k+1),`,
`>                      [&=(tmp, tmp+(res[j+1,k+1]*res[i+1,k+1]))]`,
`>                 ],`,
`>                 [equalC, res[i+1,j+1], (M[j+1,i+1]-tmp)/res[j+1,j+1]],`,
`>                 [equalC, res[j+1,i+1], res[i+1,j+1]]`,
`>                ]`,
`>           ]]`,
`>     ],   `,
`>     [declarem, int, [i,j,k]],`,
`>     [declarem, double, [tmp]]`,
`>   ]]]:   `,
`   `,
`> genC(4, l);`,
`   `,
`# include <math.h>`,
`# include <stdio.h>`,
`   `,
`    /* Cholesky method */`,
`   `,
`    void cholesky(M,res,n)`,
`    double **M,**res;`,
`    int n;   `,
`      {   `,
`      int i,j,k;`,
`      double tmp;`,
`      for(j = 0;(j <= n-1);j++)`,
`        {   `,
`        tmp = 0;`,
`        for(k = 0;(k <= j-1);k++)`,
`          tmp += pow(res[j][k],2);`,
`        res[j][j] = sqrt(M[j][j]-tmp);`,
`        for(i = j+1;(i <= n-1);i++)`,
`          {`,
`          tmp = 0;`,
`          for(k = 0;(k <= j-1);k++)`,
`            tmp += res[j][k]*res[i][k];`,
`          res[i][j] = (M[j][i]-tmp)/res[j][j];`,
`          res[j][i] = res[i][j];`,
`          }`,
`        }   `,
`      }   `,
`   `,
`   `,
`> genC([equalC, PI, Pi]);`,
`PI = 0.3141593E1;`,
`   `,
`> precision := double:`,
`> genC([equalC, PI, Pi]);`,
`PI = 0.3141592653589793E1;`,
`   `,
`> genC([equalC,a,expand((1+cos(x))^10)]);`,
`a = 1+10*cos(x)+45*pow(cos(x),2)+120*pow(cos(x),3)+210*pow(cos(x),4)+252*pow(`,
`cos(x),5)+210*pow(cos(x),6)+120*pow(cos(x),7)+45*pow(cos(x),8)+10*pow(cos(x),9)`,
`+pow(cos(x),10);`,
`   `,
`> optimized :=true:`,
`> genC(4, [equalC,a,expand((1+cos(x))^10)]);`,
`    t1 = cos(x);`,
`    t3 = t1*t1;`,
`    t5 = t3*t1;`,
`    t7 = t3*t3;`,
`    t16 = t7*t7;`,
`    a = 1+10*t1+45*t3+120*t5+210*t7+252*t7*t1+210*t7*t3+120*t7*t5+45*t16+10*t16*t1+t16*t3;`,
`> autodeclare:=``double``:`,
`> genC(4, [equalC,a,expand((1+cos(x))^10)]);`,
`    double t1,t3,t5,t7,t16;`,
`    t1 = cos(x);`,
`    t3 = t1*t1;`,
`    t5 = t3*t1;`,
`    t7 = t3*t3;`,
`    t16 = t7*t7;`,
`    a = 1+10*t1+45*t3+120*t5+210*t7+252*t7*t1+210*t7*t3+120*t7*t5+45*t16+10*t16`,
`*t1+t16*t3;`,
`   `,
`   `,
`SEE ALSO: C, optimize, macrofort (share library)`,
`   `,
`   `,
`   `,
`   `,
`   `
):

####################################################################
