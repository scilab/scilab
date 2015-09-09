/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __BASE_FUNCTIONS_MACROS2_H__
#define __BASE_FUNCTIONS_MACROS2_H__

#define __VECTORIZE__(NAME, CPPNAME, ShortTL, BaseTL, BaseTO) void NAME##_M##ShortTL(const BaseTL * __restrict__ x, const int64_t x_r, const int64_t x_c, BaseTO ** __restrict__ o) { jit::BaseFunctions::vectorize<BaseTL, BaseTO, CPPNAME>(x, x_r, x_c, o); }

#define __CUMULATE__(NAME, CPPNAME, ShortTL, BaseTL, BaseTO) BaseTL NAME##_M##ShortTL(const BaseTL * x, const int64_t x_r, const int64_t x_c) { return jit::BaseFunctions::cumulate<BaseTL, CPPNAME>(x, x_r, x_c); }

#define __CUMULATEIN__(NAME, CPPNAME, ShortTL, BaseTL, BaseTO) void NAME##_M##ShortTL(const BaseTL * x, const int64_t x_r, const int64_t x_c, BaseTL * o) { jit::BaseFunctions::cumulateIn<BaseTL, CPPNAME>(x, x_r, x_c, o); }

#define __BINARY__SM(NAME, CPPNAME, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) void NAME##_S##ShortTL##M##ShortTR(const BaseTL x, const BaseTR * __restrict__ y, const int64_t y_r, const int64_t y_c, BaseTO ** __restrict__ o) { jit::BaseFunctions::binSM<BaseTL, BaseTR, BaseTO, CPPNAME>(x, y, y_r, y_c, o); }

#define __BINARY__MS(NAME, CPPNAME, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) void NAME##_M##ShortTL##S##ShortTR(const BaseTL * __restrict__ x, const int64_t x_r, const int64_t x_c, const BaseTR y, BaseTO ** __restrict__ o) { jit::BaseFunctions::binMS<BaseTL, BaseTR, BaseTO, CPPNAME>(x, x_r, x_c, y, o); }

#define __BINARY__MM(NAME, CPPNAME, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) void NAME##_M##ShortTL##M##ShortTR(const BaseTL * __restrict__ x, const int64_t x_r, const int64_t x_c, const BaseTR * __restrict__ y, const int64_t y_r, const int64_t y_c, BaseTO ** __restrict__ o) { jit::BaseFunctions::binMM<BaseTL, BaseTR, BaseTO, CPPNAME>(x, x_r, x_c, y, y_r, y_c, o); }

#define __BINARY__UU(NAME, CPPNAME, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) void NAME##_U##ShortTL##U##ShortTR(const BaseTL * __restrict__ x, const int64_t x_r, const int64_t x_c, const BaseTR * __restrict__ y, const int64_t y_r, const int64_t y_c, BaseTO ** __restrict__ o) { jit::BaseFunctions::binUU<BaseTL, BaseTR, BaseTO, CPPNAME>(x, x_r, x_c, y, y_r, y_c, o); }

#define __UNARY__M(NAME, CPPNAME, ShortTL, BaseTL, BaseTO) void NAME##_M##ShortTL(const BaseTL * x, const int64_t x_r, const int64_t x_c, BaseTO ** __restrict__ o) { jit::BaseFunctions::unaryM<BaseTL, BaseTO, CPPNAME>(x, x_r, x_c, o); }

#define __BREAKON__SM(NAME, CPPNAME, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) int32_t breakOn_##NAME##_S##ShortTL##M##ShortTR(const BaseTL x, const BaseTR * __restrict__ y, const int64_t y_r, const int64_t y_c) { return jit::BaseFunctions::breakOnSM<false, BaseTL, BaseTR, CPPNAME>(x, y, y_r, y_c); }

#define __BREAKON__MS(NAME, CPPNAME, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) int32_t breakOn_##NAME##_M##ShortTL##S##ShortTR(const BaseTL * __restrict__ x, const int64_t x_r, const int64_t x_c, const BaseTR y) { return jit::BaseFunctions::breakOnMS<false, BaseTL, BaseTR, CPPNAME>(x, x_r, x_c, y); }

#define __BREAKON__MM(NAME, CPPNAME, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) int32_t breakOn_##NAME##_M##ShortTL##M##ShortTR(const BaseTL * __restrict__ x, const int64_t x_r, const int64_t x_c, const BaseTR * __restrict__ y, const int64_t y_r, const int64_t y_c) { return jit::BaseFunctions::breakOnMM<false, BaseTL, BaseTR, CPPNAME>(x, x_r, x_c, y, y_r, y_c); }


#define ______ADD_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__SM(add, jit::BaseFunctions::sum, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______ADD_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MS(add, jit::BaseFunctions::sum, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______ADD_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MM(add, jit::BaseFunctions::sum, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______ADD_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__UU(add, jit::BaseFunctions::sum, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______SUB_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__SM(sub, jit::BaseFunctions::sub, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______SUB_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MS(sub, jit::BaseFunctions::sub, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______SUB_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MM(sub, jit::BaseFunctions::sub, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______SUB_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__UU(sub, jit::BaseFunctions::sub, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______UNARYMINUS_____M__(ShortTL, BaseTL, BaseTO) __UNARY__M(unaryminus, jit::BaseFunctions::opposite, ShortTL, BaseTL, BaseTO)

#define ______TIMES_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__SM(times, jit::BaseFunctions::prod, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______TIMES_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MS(times, jit::BaseFunctions::prod, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______TIMES_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) void times_M##ShortTL##M##ShortTR(const BaseTL * __restrict__ x, const int64_t x_r, const int64_t x_c, const BaseTR * __restrict__ y, const int64_t y_r, const int64_t y_c, BaseTO ** __restrict__ o) { jit::BaseFunctions::timesMM<BaseTL, BaseTR, BaseTO>(x, x_r, x_c, y, y_r, y_c, o); }
#define ______TIMES_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) 

#define ______DOTTIMES_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__SM(dottimes, jit::BaseFunctions::prod, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______DOTTIMES_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MS(dottimes, jit::BaseFunctions::prod, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______DOTTIMES_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MM(dottimes, jit::BaseFunctions::prod, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______DOTTIMES_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__UU(dottimes, jit::BaseFunctions::prod, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______DOTPOWER_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__SM(dotpower, jit::BaseFunctions::pow, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______DOTPOWER_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MS(dotpower, jit::BaseFunctions::pow, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______DOTPOWER_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MM(dotpower, jit::BaseFunctions::pow, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______DOTPOWER_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__UU(dotpower, jit::BaseFunctions::pow, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______DOTRDIV_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__SM(dotrdiv, jit::BaseFunctions::rdiv, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______DOTRDIV_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MS(dotrdiv, jit::BaseFunctions::rdiv, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______DOTRDIV_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MM(dotrdiv, jit::BaseFunctions::rdiv, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______DOTRDIV_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__UU(dotrdiv, jit::BaseFunctions::rdiv, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______NEQ_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__SM(neq, jit::BaseFunctions::ne, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) 
#define ______NEQ_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MS(neq, jit::BaseFunctions::ne, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______NEQ_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MM(neq, jit::BaseFunctions::ne, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______NEQ_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__UU(neq, jit::BaseFunctions::ne, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______EQ_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__SM(eq, jit::BaseFunctions::eq, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__SM(eq, jit::BaseFunctions::eq, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______EQ_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MS(eq, jit::BaseFunctions::eq, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)__BREAKON__MS(eq, jit::BaseFunctions::eq, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______EQ_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MM(eq, jit::BaseFunctions::eq, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__MM(eq, jit::BaseFunctions::eq, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______EQ_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__UU(eq, jit::BaseFunctions::eq, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______LE_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__SM(le, jit::BaseFunctions::le, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__SM(le, jit::BaseFunctions::le, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______LE_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MS(le, jit::BaseFunctions::le, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__MS(le, jit::BaseFunctions::le, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______LE_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MM(le, jit::BaseFunctions::le, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__MM(le, jit::BaseFunctions::le, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______LE_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__UU(le, jit::BaseFunctions::le, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______GT_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__SM(gt, jit::BaseFunctions::gt, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__SM(gt, jit::BaseFunctions::gt, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______GT_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MS(gt, jit::BaseFunctions::gt, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__MS(gt, jit::BaseFunctions::gt, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______GT_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MM(gt, jit::BaseFunctions::gt, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__MM(gt, jit::BaseFunctions::gt, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______GT_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__UU(gt, jit::BaseFunctions::gt, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______LT_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__SM(lt, jit::BaseFunctions::lt, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__SM(lt, jit::BaseFunctions::lt, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______LT_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MS(lt, jit::BaseFunctions::lt, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__MS(lt, jit::BaseFunctions::lt, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______LT_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MM(lt, jit::BaseFunctions::lt, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__MM(lt, jit::BaseFunctions::lt, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______LT_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__UU(lt, jit::BaseFunctions::lt, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______GE_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__SM(ge, jit::BaseFunctions::ge, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__SM(ge, jit::BaseFunctions::ge, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______GE_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MS(ge, jit::BaseFunctions::ge, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__MS(ge, jit::BaseFunctions::ge, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______GE_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MM(ge, jit::BaseFunctions::ge, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__MM(ge, jit::BaseFunctions::ge, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______GE_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__UU(ge, jit::BaseFunctions::ge, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______AND_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__SM(and, jit::BaseFunctions::booland, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__SM(and, jit::BaseFunctions::booland, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______AND_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MS(and, jit::BaseFunctions::booland, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__MS(and, jit::BaseFunctions::booland, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______AND_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MM(and, jit::BaseFunctions::booland, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__MM(and, jit::BaseFunctions::booland, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______AND_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__UU(and, jit::BaseFunctions::booland, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______ANDAND_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__SM(andand, jit::BaseFunctions::booland, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__SM(andand, jit::BaseFunctions::booland, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______ANDAND_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MS(andand, jit::BaseFunctions::booland, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__MS(andand, jit::BaseFunctions::booland, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______ANDAND_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MM(andand, jit::BaseFunctions::booland, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__MM(andand, jit::BaseFunctions::booland, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______ANDAND_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__UU(andand, jit::BaseFunctions::booland, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______OR_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__SM(or, jit::BaseFunctions::boolor, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__SM(or, jit::BaseFunctions::boolor, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______OR_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MS(or, jit::BaseFunctions::boolor, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__MS(or, jit::BaseFunctions::boolor, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______OR_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MM(or, jit::BaseFunctions::boolor, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__MM(or, jit::BaseFunctions::boolor, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______OR_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__UU(or, jit::BaseFunctions::boolor, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______OROR_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__SM(oror, jit::BaseFunctions::boolor, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__SM(oror, jit::BaseFunctions::boolor, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______OROR_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MS(oror, jit::BaseFunctions::boolor, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__MS(oror, jit::BaseFunctions::boolor, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______OROR_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__MM(oror, jit::BaseFunctions::boolor, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BREAKON__MM(oror, jit::BaseFunctions::boolor, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______OROR_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__UU(oror, jit::BaseFunctions::boolor, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______NOT_____M__(ShortTL, BaseTL, BaseTO) __UNARY__M(not, jit::BaseFunctions::boolneg, ShortTL, BaseTL, BaseTO)

//#define ______TRANSP_____M__(ShortTL, BaseTL, BaseTO) addSymbol(sam, "transp_M_" #ShortTL, jit::BaseFunctions::oTransp<BaseTL>);

#define __ABS_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(abs, jit::BaseFunctions::abs, ShortTL, BaseTL, BaseTO)
#define __COS_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(cos, std::cos, ShortTL, BaseTL, BaseTO)
#define __SIN_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(sin, std::sin, ShortTL, BaseTL, BaseTO)
#define __TAN_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(tan, std::tan, ShortTL, BaseTL, BaseTO)
#define __TANH_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(tanh, std::tanh, ShortTL, BaseTL, BaseTO)
#define __ATAN_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(atan, std::atan, ShortTL, BaseTL, BaseTO)
#define __COSH_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(cosh, std::cosh, ShortTL, BaseTL, BaseTO)
#define __SINH_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(sinh, std::sinh, ShortTL, BaseTL, BaseTO)
#define __EXP_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(exp, std::exp, ShortTL, BaseTL, BaseTO)
#define __ERF_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(erf, std::erf, ShortTL, BaseTL, BaseTO)
#define __ERFC_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(erfc, std::erfc, ShortTL, BaseTL, BaseTO)
#define __SIGN_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(sign, jit::BaseFunctions::sign, ShortTL, BaseTL, BaseTO)
#define __GAMMA_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(gamma, std::gamma, ShortTL, BaseTL, BaseTO)
#define __GAMMALN_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(gammaln, std::lgamma, ShortTL, BaseTL, BaseTO)
#define __CEIL_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(ceil, jit::BaseFunctions::ceil, ShortTL, BaseTL, BaseTO)
#define __FLOOR_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(floor, jit::BaseFunctions::floor, ShortTL, BaseTL, BaseTO)
#define __ROUND_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(round, jit::BaseFunctions::round, ShortTL, BaseTL, BaseTO)
#define __INT_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(int, jit::BaseFunctions::trunc, ShortTL, BaseTL, BaseTO)
#define __FIX_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(fix, jit::BaseFunctions::trunc, ShortTL, BaseTL, BaseTO)
#define __INT8_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(int8, jit::BaseFunctions::cast, ShortTL, BaseTL, BaseTO)
#define __INT16_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(int16, jit::BaseFunctions::cast, ShortTL, BaseTL, BaseTO)
#define __INT32_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(int32, jit::BaseFunctions::cast, ShortTL, BaseTL, BaseTO)
#define __INT64_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(int64, jit::BaseFunctions::cast, ShortTL, BaseTL, BaseTO)
#define __UINT8_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(uint8, jit::BaseFunctions::cast, ShortTL, BaseTL, BaseTO)
#define __UINT16_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(uint16, jit::BaseFunctions::cast, ShortTL, BaseTL, BaseTO)
#define __UINT32_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(uint32, jit::BaseFunctions::cast, ShortTL, BaseTL, BaseTO)
#define __UINT64_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(uint64, jit::BaseFunctions::cast, ShortTL, BaseTL, BaseTO)
//#define __ISINF_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(isinf, tools::isInfinite, ShortTL, BaseTL, BaseTO)
//#define __ISNAN_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(isnan, tools::isNaN, ShortTL, BaseTL, BaseTO)
#define __MAX_M__(ShortTL, BaseTL, BaseTO) __CUMULATE__(max, jit::BaseFunctions::max, ShortTL, BaseTL, BaseTO)
#define __MIN_M__(ShortTL, BaseTL, BaseTO) __CUMULATE__(min, jit::BaseFunctions::min, ShortTL, BaseTL, BaseTO)
#define __SUM_M__(ShortTL, BaseTL, BaseTO) __CUMULATE__(sum, jit::BaseFunctions::sum, ShortTL, BaseTL, BaseTO)
#define __PROD_M__(ShortTL, BaseTL, BaseTO) __CUMULATE__(prod, jit::BaseFunctions::prod, ShortTL, BaseTL, BaseTO)
#define __CUMSUM_M__(ShortTL, BaseTL, BaseTO) __CUMULATEIN__(cumsum, jit::BaseFunctions::sum, ShortTL, BaseTL, BaseTO)
#define __CUMPROD_M__(ShortTL, BaseTL, BaseTO) __CUMULATEIN__(cumprod, jit::BaseFunctions::prod, ShortTL, BaseTL, BaseTO)

// rand is a special call so no need to define it
#define __RAND_M__(ShortTL, BaseTL, BaseTO)

#define __MODULO_SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(modulo, jit::BaseFunctions::modulo, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define __MODULO_MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(modulo, jit::BaseFunctions::modulo, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define __MODULO_MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(modulo, jit::BaseFunctions::modulo, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define __MODULO_UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(modulo, jit::BaseFunctions::modulo, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

// NOT IMPLEMENTED FOR NOW !!
#define ______TRANSP_____M__(ShortTL, BaseTL, BaseTO)
#define ______DOTTRANSP_____M__(ShortTL, BaseTL, BaseTO)
#define ______KRONTIMES_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______KRONTIMES_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______KRONTIMES_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______KRONTIMES_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______LDIVIDE_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______LDIVIDE_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______LDIVIDE_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______LDIVIDE_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______RDIVIDE_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______RDIVIDE_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______RDIVIDE_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______RDIVIDE_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______POWER_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______POWER_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______POWER_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______POWER_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define __ZEROS_M__(ShortTL, BaseTL, BaseTO)
#define __ZEROS_SS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define __RAND_M__(ShortTL, BaseTL, BaseTO)
#define __RAND_SS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define __ONES_M__(ShortTL, BaseTL, BaseTO)
#define __ONES_SS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define __ASIN_M__(ShortTL, BaseTL, BaseTO)
#define __ACOS_M__(ShortTL, BaseTL, BaseTO)
#define __SQRT_M__(ShortTL, BaseTL, BaseTO)
#define __LOG_M__(ShortTL, BaseTL, BaseTO)
#define __SINM_M__(ShortTL, BaseTL, BaseTO)
#define __ISINF_M__(ShortTL, BaseTL, BaseTO)
#define __ISNAN_M__(ShortTL, BaseTL, BaseTO)
#define __HYPOT_SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define __HYPOT_MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define __HYPOT_MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define __HYPOT_UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define __SIZE_M__(ShortTL, BaseTL, BaseTO)
#define __TYPE_M__(ShortTL, BaseTL, BaseTO)
#define __INTTYPE_M__(ShortTL, BaseTL, BaseTO)
#define __IMAG_M__(ShortTL, BaseTL, BaseTO)
#define __ISSCALAR_M__(ShortTL, BaseTL, BaseTO)
#define __AND_M__(ShortTL, BaseTL, BaseTO)
#define __AND_MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define __OR_M__(ShortTL, BaseTL, BaseTO)
#define __OR_MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define __IMULT_M__(ShortTL, BaseTL, BaseTO)
#define __ISREAL_M__(ShortTL, BaseTL, BaseTO)
#define __ISREAL_MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define __ARGN_S__(ShortTL, BaseTL, BaseTO)
#define __MATRIX_MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define __MATRIX_MSS__(ShortT1, BaseT1, ShortT2, BaseT2, ShortT3, BaseT3, BaseTO)

#endif // __BASE_FUNCTIONS_MACROS2_H__
