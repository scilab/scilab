#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../../..
SCIDIR1=..\..\..

include ../../../Makefile.incl.mak

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = m2skernellib
NAM = SCI/macros/m2sci/kernel

TLIST_CREATE = Cste.sci Equal.sci Funcall.sci Infer.sci M2scivar.sci Operation.sci Rhs.sci Type.sci Variable.sci

TLIST_e = %cste_e.sci %funcall_e.sci %m2scivar_e.sci %operatio_e.sci %variable_e.sci

TLIST_6 = %funcall_6.sci %operatio_6.sci %variable_6.sci

INSER = %ce_i_variable.sci %ce_i_operatio.sci \
	%infer_i_st.sci  %infer_i_s.sci \
	%l_i_cste.sci %l_i_funcall.sci %l_i_operatio.sci %l_i_variable.sci \
	%s_i_variable.sci %s_i_operatio.sci \
	%st_i_operatio.sci %st_i_variable.sci \
	%type_i_cste.sci %type_i_funcall.sci %type_i_operatio.sci %type_i_variable.sci 

KERNEL = allunknown.sci clause2sci.sci convert2double.sci \
	 default_trad.sci equal2sci.sci expression2sci.sci first_non_singleton.sci funcall2sci.sci getvar2sci.sci \
	 getoperands.sci getrhs.sci gettempvar.sci get_unknown.sci i_notation.sci infer2txt.sci insert.sci instruction2sci.sci \
	 isacomment.sci isanmfile.sci is_a_scalar.sci is_a_vector.sci is_complex.sci \
	 isdefinedvar.sci is_empty.sci isinstring.sci is_real.sci m2sci_info.sci \
	 m2sci.sci m2sci_syntax.sci mtlbtree2sci.sci multi_fun_file.sci no_equiv.sci \
	 not_a_scalar.sci not_a_vector.sci not_empty.sci operation2sci.sci \
	 repl_poss.sci replace_brackets.sci sci_generic.sci sci_m2scideclare.sci sciparam.sci set_infos.sci \
	 update_instr_list.sci updatevarslist.sci

MACROS= $(TLIST_CREATE) $(TLIST_e) $(TLIST_6) $(INSER) $(KERNEL) 


include ../../Make.lib.mak
