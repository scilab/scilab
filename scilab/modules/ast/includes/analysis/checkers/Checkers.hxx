/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2015 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

// This file has been generated, so don't modify it by hand !!

#ifndef __CHECKERS_HXX__
#define __CHECKERS_HXX__

#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "TIType.hxx"
#include "tools.hxx"

namespace analysis
{
class Checkers
{
    struct __NName
    {
        const unsigned short nargs;
        const std::wstring name;

        //__NName(unsigned short n, const char * _name) : nargs(n), name(_name) { }
        __NName(unsigned short n, const std::wstring & _name) : nargs(n), name(_name) { }
        inline bool operator==(const __NName & R) const
        {
            return nargs == R.nargs && name == R.name;
        }
    };

    struct __Infos
    {
        const unsigned short pos;
        const bool elementwise;
        const bool notconst;
        const bool jitted;

        __Infos(const unsigned short _pos, const bool _ew, const bool _nc, const bool _j) : pos(_pos), elementwise(_ew), notconst(_nc), jitted(_j) { }
    };

    struct __Hasher
    {
        inline std::size_t operator()(const __NName & nn) const
        {
            return tools::hash_combine(std::hash<std::wstring>()(nn.name), nn.nargs);
        }
    };

    typedef std::unordered_map<__NName, __Infos, __Hasher> CheckersMap;
    static CheckersMap map;
    static TIType (*ARGS0[6])(GVN &);
    static TIType (*ARGS1[54])(GVN &, const TIType &);
    static TIType (*ARGS2[10])(GVN &, const TIType &, const TIType &);
    static TIType (*ARGS3[1])(GVN &, const TIType &, const TIType &, const TIType &);

public:

    static TIType check(GVN & gvn, const std::wstring & name, const std::vector<TIType> & args);
    static bool isElementWise(const std::wstring & name, unsigned short nargs = 1);
    static bool isJitted(const std::wstring & name, unsigned short nargs = 1);
    static bool isConst(const std::wstring & name, unsigned short nargs = 1);

private:

    static CheckersMap initMap();

public:

    static TIType check_____dotpower____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_rand(GVN & gvn);
    static TIType check_rand(GVN & gvn, const TIType & in0);
    static TIType check_rand(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_____ge____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_____or____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_____power____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_____not____(GVN & gvn, const TIType & in0);
    static TIType check_isreal(GVN & gvn, const TIType & in0);
    static TIType check_isreal(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_____transp____(GVN & gvn, const TIType & in0);
    static TIType check_abs(GVN & gvn, const TIType & in0);
    static TIType check_cosh(GVN & gvn, const TIType & in0);
    static TIType check_typeof(GVN & gvn, const TIType & in0);
    static TIType check_int16(GVN & gvn, const TIType & in0);
    static TIType check_iconvert(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_gettext(GVN & gvn, const TIType & in0);
    static TIType check_gettext(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_____neq____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_____and____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_tan(GVN & gvn, const TIType & in0);
    static TIType check_cumsum(GVN & gvn, const TIType & in0);
    static TIType check_size(GVN & gvn, const TIType & in0);
    static TIType check_size(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_____andand____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_asin(GVN & gvn, const TIType & in0);
    static TIType check_____dottransp____(GVN & gvn, const TIType & in0);
    static TIType check_or(GVN & gvn, const TIType & in0);
    static TIType check_or(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_____eq____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_log(GVN & gvn, const TIType & in0);
    static TIType check_floor(GVN & gvn, const TIType & in0);
    static TIType check_int(GVN & gvn, const TIType & in0);
    static TIType check_fix(GVN & gvn, const TIType & in0);
    static TIType check_sqrt(GVN & gvn, const TIType & in0);
    static TIType check_uint16(GVN & gvn, const TIType & in0);
    static TIType check_____le____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_zeros(GVN & gvn);
    static TIType check_zeros(GVN & gvn, const TIType & in0);
    static TIType check_zeros(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_ls(GVN & gvn);
    static TIType check_ls(GVN & gvn, const TIType & in0);
    static TIType check_max(GVN & gvn, const TIType & in0);
    static TIType check_uint32(GVN & gvn, const TIType & in0);
    static TIType check_int64(GVN & gvn, const TIType & in0);
    static TIType check_prod(GVN & gvn, const TIType & in0);
    static TIType check_type(GVN & gvn, const TIType & in0);
    static TIType check_sin(GVN & gvn, const TIType & in0);
    static TIType check_round(GVN & gvn, const TIType & in0);
    static TIType check_____krontimes____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_int32(GVN & gvn, const TIType & in0);
    static TIType check_____gt____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_inttype(GVN & gvn, const TIType & in0);
    static TIType check_exp(GVN & gvn, const TIType & in0);
    static TIType check_atan(GVN & gvn, const TIType & in0);
    static TIType check_____dotrdiv____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_imag(GVN & gvn, const TIType & in0);
    static TIType check_uint8(GVN & gvn, const TIType & in0);
    static TIType check_ceil(GVN & gvn, const TIType & in0);
    static TIType check_uint64(GVN & gvn, const TIType & in0);
    static TIType check_____lt____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_sinm(GVN & gvn, const TIType & in0);
    static TIType check_isinf(GVN & gvn, const TIType & in0);
    static TIType check_____sub____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_sinh(GVN & gvn, const TIType & in0);
    static TIType check_toc(GVN & gvn);
    static TIType check_isscalar(GVN & gvn, const TIType & in0);
    static TIType check_sign(GVN & gvn, const TIType & in0);
    static TIType check_int8(GVN & gvn, const TIType & in0);
    static TIType check_____rdivide____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_matrix(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_matrix(GVN & gvn, const TIType & in0, const TIType & in1, const TIType & in2);
    static TIType check_____oror____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_cos(GVN & gvn, const TIType & in0);
    static TIType check_and(GVN & gvn, const TIType & in0);
    static TIType check_and(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_____unaryminus____(GVN & gvn, const TIType & in0);
    static TIType check_tanh(GVN & gvn, const TIType & in0);
    static TIType check_____add____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_isnan(GVN & gvn, const TIType & in0);
    static TIType check_imult(GVN & gvn, const TIType & in0);
    static TIType check_____times____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_argn(GVN & gvn);
    static TIType check_argn(GVN & gvn, const TIType & in0);
    static TIType check_cumprod(GVN & gvn, const TIType & in0);
    static TIType check_erf(GVN & gvn, const TIType & in0);
    static TIType check_ones(GVN & gvn);
    static TIType check_ones(GVN & gvn, const TIType & in0);
    static TIType check_ones(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_erfc(GVN & gvn, const TIType & in0);
    static TIType check_min(GVN & gvn, const TIType & in0);
    static TIType check_sum(GVN & gvn, const TIType & in0);
    static TIType check_____ldivide____(GVN & gvn, const TIType & in0, const TIType & in1);
    static TIType check_acos(GVN & gvn, const TIType & in0);
    static TIType check_____dottimes____(GVN & gvn, const TIType & in0, const TIType & in1);
};

} // namespace analysis

#endif // __CHECKERS_HXX__