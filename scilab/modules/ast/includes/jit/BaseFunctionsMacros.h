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

#ifndef __BASE_FUNCTIONS_MACROS_H__
#define __BASE_FUNCTIONS_MACROS_H__

#define __VECTORIZE__(NAME, CPPNAME, ShortTL, BaseTL, BaseTO) addSymbol(sam, "" #NAME "_M_" #ShortTL, &BaseFunctions::vectorize<BaseTL, BaseTO, CPPNAME>);
#define __CUMULATE__(NAME, CPPNAME, ShortTL, BaseTL, BaseTO) addSymbol(sam, "" #NAME "_M_" #ShortTL, BaseFunctions::cumulate<BaseTL, CPPNAME>);
#define __CUMULATEIN__(NAME, CPPNAME, ShortTL, BaseTL, BaseTO) addSymbol(sam, "" #NAME "_M_" #ShortTL, BaseFunctions::cumulateIn<BaseTL, CPPNAME>);
#define __BINARY__(NAME, CPPNAME, __SM__, __MS__, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) addSymbol(sam, "" #NAME "_" #__SM__ "_" #ShortTL "_" #__MS__ "_" #ShortTR, BaseFunctions::bin##__SM__##__MS__<BaseTL, BaseTR, BaseTO, CPPNAME>);
#define __UNARY__(NAME, CPPNAME, __SM__, ShortTL, BaseTL, BaseTO) addSymbol(sam, "" #NAME "_" #__SM__ "_" #ShortTL, BaseFunctions::unary##__SM__<BaseTL, BaseTO, CPPNAME>);

#define ______ADD_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(add, BaseFunctions::sum, S, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______ADD_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(add, BaseFunctions::sum, M, S, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______ADD_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(add, BaseFunctions::sum, M, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______ADD_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(add, BaseFunctions::sum, U, U, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______SUB_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(sub, BaseFunctions::sub, S, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______SUB_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(sub, BaseFunctions::sub, M, S, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______SUB_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(sub, BaseFunctions::sub, M, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______SUB_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(sub, BaseFunctions::sub, U, U, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______UNARYMINUS_____M__(ShortTL, BaseTL, BaseTO) __UNARY__(unaryminus, BaseFunctions::opposite, M, ShortTL, BaseTL, BaseTO)

#define ______TIMES_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(times, BaseFunctions::prod, S, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______TIMES_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(times, BaseFunctions::prod, M, S, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______TIMES_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) addSymbol(sam, "times_M_" #ShortTL "_M_" #ShortTR, BaseFunctions::timesMM<BaseTL, BaseTR, BaseTO>);
#define ______TIMES_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(times, BaseFunctions::prod, U, U, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______DOTTIMES_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(dottimes, BaseFunctions::prod, S, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______DOTTIMES_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(dottimes, BaseFunctions::prod, M, S, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______DOTTIMES_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(dottimes, BaseFunctions::prod, M, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______DOTTIMES_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(dottimes, BaseFunctions::prod, U, U, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______DOTPOWER_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(dotpower, BaseFunctions::pow, S, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______DOTPOWER_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(dotpower, BaseFunctions::pow, M, S, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______DOTPOWER_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(dotpower, BaseFunctions::pow, M, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______DOTPOWER_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(dotpower, BaseFunctions::pow, U, U, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______DOTRDIV_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(dotrdiv, BaseFunctions::rdiv, S, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______DOTRDIV_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(dotrdiv, BaseFunctions::rdiv, M, S, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______DOTRDIV_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(dotrdiv, BaseFunctions::rdiv, M, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______DOTRDIV_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(dotrdiv, BaseFunctions::rdiv, U, U, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______NEQ_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(neq, BaseFunctions::ne, S, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______NEQ_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(neq, BaseFunctions::ne, M, S, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______NEQ_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(neq, BaseFunctions::ne, M, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______NEQ_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(neq, BaseFunctions::ne, U, U, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______EQ_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(eq, BaseFunctions::eq, S, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______EQ_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(eq, BaseFunctions::eq, M, S, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______EQ_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(eq, BaseFunctions::eq, M, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______EQ_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(eq, BaseFunctions::eq, U, U, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______LE_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(le, BaseFunctions::le, S, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______LE_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(le, BaseFunctions::le, M, S, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______LE_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(le, BaseFunctions::le, M, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______LE_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(le, BaseFunctions::le, U, U, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______GT_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(gt, BaseFunctions::gt, S, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______GT_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(gt, BaseFunctions::gt, M, S, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______GT_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(gt, BaseFunctions::gt, M, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______GT_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(gt, BaseFunctions::gt, U, U, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______LT_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(lt, BaseFunctions::lt, S, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______LT_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(lt, BaseFunctions::lt, M, S, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______LT_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(lt, BaseFunctions::lt, M, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______LT_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(lt, BaseFunctions::lt, U, U, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______GE_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(ge, BaseFunctions::ge, S, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______GE_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(ge, BaseFunctions::ge, M, S, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______GE_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(ge, BaseFunctions::ge, M, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______GE_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(ge, BaseFunctions::ge, U, U, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______AND_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(and, BaseFunctions::booland, S, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______AND_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(and, BaseFunctions::booland, M, S, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______AND_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(and, BaseFunctions::booland, M, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______AND_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(and, BaseFunctions::booland, U, U, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______ANDAND_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(andand, BaseFunctions::booland, S, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______ANDAND_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(andand, BaseFunctions::booland, M, S, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______ANDAND_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(andand, BaseFunctions::booland, M, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______ANDAND_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(andand, BaseFunctions::booland, U, U, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______OR_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(or, BaseFunctions::boolor, S, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______OR_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(or, BaseFunctions::boolor, M, S, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______OR_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(or, BaseFunctions::boolor, M, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______OR_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(or, BaseFunctions::boolor, U, U, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______OROR_____SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(oror, BaseFunctions::boolor, S, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______OROR_____MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(oror, BaseFunctions::boolor, M, S, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______OROR_____MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(oror, BaseFunctions::boolor, M, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define ______OROR_____UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(oror, BaseFunctions::boolor, U, U, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

#define ______NOT_____M__(ShortTL, BaseTL, BaseTO) __UNARY__(not, BaseFunctions::boolneg, M, ShortTL, BaseTL, BaseTO)

#define ______TRANSP_____M__(ShortTL, BaseTL, BaseTO) addSymbol(sam, "transp_M_" #ShortTL, BaseFunctions::oTransp<BaseTL>);

#define __ABS_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(abs, BaseFunctions::abs, ShortTL, BaseTL, BaseTO)
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
#define __SIGN_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(sign, BaseFunctions::sign, ShortTL, BaseTL, BaseTO)
#define __GAMMA_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(gamma, std::gamma, ShortTL, BaseTL, BaseTO)
#define __GAMMALN_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(gammaln, std::lgamma, ShortTL, BaseTL, BaseTO)
#define __CEIL_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(ceil, BaseFunctions::ceil, ShortTL, BaseTL, BaseTO)
#define __FLOOR_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(floor, BaseFunctions::floor, ShortTL, BaseTL, BaseTO)
#define __ROUND_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(round, BaseFunctions::round, ShortTL, BaseTL, BaseTO)
#define __INT_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(int, BaseFunctions::trunc, ShortTL, BaseTL, BaseTO)
#define __FIX_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(fix, BaseFunctions::trunc, ShortTL, BaseTL, BaseTO)
#define __INT8_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(int8, BaseFunctions::cast, ShortTL, BaseTL, BaseTO)
#define __INT16_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(int16, BaseFunctions::cast, ShortTL, BaseTL, BaseTO)
#define __INT32_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(int32, BaseFunctions::cast, ShortTL, BaseTL, BaseTO)
#define __INT64_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(int64, BaseFunctions::cast, ShortTL, BaseTL, BaseTO)
#define __UINT8_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(uint8, BaseFunctions::cast, ShortTL, BaseTL, BaseTO)
#define __UINT16_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(uint16, BaseFunctions::cast, ShortTL, BaseTL, BaseTO)
#define __UINT32_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(uint32, BaseFunctions::cast, ShortTL, BaseTL, BaseTO)
#define __UINT64_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(uint64, BaseFunctions::cast, ShortTL, BaseTL, BaseTO)
//#define __ISINF_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(isinf, tools::isInfinite, ShortTL, BaseTL, BaseTO)
//#define __ISNAN_M__(ShortTL, BaseTL, BaseTO) __VECTORIZE__(isnan, tools::isNaN, ShortTL, BaseTL, BaseTO)
#define __MAX_M__(ShortTL, BaseTL, BaseTO) __CUMULATE__(max, BaseFunctions::max, ShortTL, BaseTL, BaseTO)
#define __MIN_M__(ShortTL, BaseTL, BaseTO) __CUMULATE__(min, BaseFunctions::min, ShortTL, BaseTL, BaseTO)
#define __SUM_M__(ShortTL, BaseTL, BaseTO) __CUMULATE__(sum, BaseFunctions::sum, ShortTL, BaseTL, BaseTO)
#define __PROD_M__(ShortTL, BaseTL, BaseTO) __CUMULATE__(prod, BaseFunctions::prod, ShortTL, BaseTL, BaseTO)
#define __CUMSUM_M__(ShortTL, BaseTL, BaseTO) __CUMULATEIN__(cumsum, BaseFunctions::sum, ShortTL, BaseTL, BaseTO)
#define __CUMPROD_M__(ShortTL, BaseTL, BaseTO) __CUMULATEIN__(cumprod, BaseFunctions::prod, ShortTL, BaseTL, BaseTO)

#define __MODULO_SM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(modulo, BaseFunctions::modulo, S, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define __MODULO_MS__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(modulo, BaseFunctions::modulo, M, S, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define __MODULO_MM__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(modulo, BaseFunctions::modulo, M, M, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)
#define __MODULO_UU__(ShortTL, BaseTL, ShortTR, BaseTR, BaseTO) __BINARY__(modulo, BaseFunctions::modulo, U, U, ShortTL, BaseTL, ShortTR, BaseTR, BaseTO)

// NOT IMPLEMENTED FOR NOW !!
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

#endif // __BASE_FUNCTIONS_MACROS_H__
