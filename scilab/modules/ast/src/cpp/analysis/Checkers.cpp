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

#include "checkers/Checkers.hxx"

namespace analysis
{
TIType (*Checkers::ARGS0[6])(GVN &);
TIType (*Checkers::ARGS1[54])(GVN &, const TIType &);
TIType (*Checkers::ARGS2[10])(GVN &, const TIType &, const TIType &);
TIType (*Checkers::ARGS3[1])(GVN &, const TIType &, const TIType &, const TIType &);

Checkers::CheckersMap Checkers::map = Checkers::initMap();

TIType Checkers::check(GVN & gvn, const std::wstring & name, const std::vector<TIType> & args)
{
    const unsigned short size = args.size();
    CheckersMap::const_iterator i = map.find(__NName(size, name));
    if (i != map.end())
    {
        // TODO : remove array of function pointers and replace by a switch statement
        switch (size)
        {
            case 0:
                return ARGS0[i->second.pos](gvn);
            case 1:
                return ARGS1[i->second.pos](gvn, args[0]);
            case 2:
                return ARGS2[i->second.pos](gvn, args[0], args[1]);
            case 3:
                return ARGS3[i->second.pos](gvn, args[0], args[1], args[2]);
        }
    }

    return TIType(gvn);
}

bool Checkers::isElementWise(const std::wstring & name, unsigned short nargs)
{
    CheckersMap::const_iterator i = map.find(__NName(nargs, name));
    if (i != map.end())
    {
        return i->second.elementwise;
    }
    return false;
}

bool Checkers::isJitted(const std::wstring & name, unsigned short nargs)
{
    CheckersMap::const_iterator i = map.find(__NName(nargs, name));
    if (i != map.end())
    {
        return i->second.jitted;
    }
    return false;
}

bool Checkers::isConst(const std::wstring & name, unsigned short nargs)
{
    CheckersMap::const_iterator i = map.find(__NName(nargs, name));
    if (i != map.end())
    {
        return !i->second.notconst;
    }
    return false;
}

Checkers::CheckersMap Checkers::initMap()
{
    CheckersMap map;
    map.emplace(__NName(0, L"rand"), __Infos(0, false, true, false));
    map.emplace(__NName(0, L"zeros"), __Infos(1, false, false, false));
    map.emplace(__NName(0, L"ls"), __Infos(2, false, true, false));
    map.emplace(__NName(0, L"toc"), __Infos(3, false, true, false));
    map.emplace(__NName(0, L"argn"), __Infos(4, false, false, false));
    map.emplace(__NName(0, L"ones"), __Infos(5, false, false, false));
    map.emplace(__NName(1, L"rand"), __Infos(0, false, true, false));
    map.emplace(__NName(1, L"isreal"), __Infos(1, false, false, false));
    map.emplace(__NName(1, L"abs"), __Infos(2, true, false, true));
    map.emplace(__NName(1, L"cosh"), __Infos(3, true, false, true));
    map.emplace(__NName(1, L"typeof"), __Infos(4, false, false, false));
    map.emplace(__NName(1, L"int16"), __Infos(5, true, false, true));
    map.emplace(__NName(1, L"gettext"), __Infos(6, false, true, false));
    map.emplace(__NName(1, L"tan"), __Infos(7, true, false, true));
    map.emplace(__NName(1, L"cumsum"), __Infos(8, true, false, true));
    map.emplace(__NName(1, L"size"), __Infos(9, false, false, false));
    map.emplace(__NName(1, L"asin"), __Infos(10, true, false, false));
    map.emplace(__NName(1, L"or"), __Infos(11, false, false, false));
    map.emplace(__NName(1, L"log"), __Infos(12, true, false, false));
    map.emplace(__NName(1, L"floor"), __Infos(13, true, false, true));
    map.emplace(__NName(1, L"int"), __Infos(14, true, false, true));
    map.emplace(__NName(1, L"fix"), __Infos(15, true, false, true));
    map.emplace(__NName(1, L"sqrt"), __Infos(16, true, false, true));
    map.emplace(__NName(1, L"uint16"), __Infos(17, true, false, true));
    map.emplace(__NName(1, L"zeros"), __Infos(18, false, false, false));
    map.emplace(__NName(1, L"ls"), __Infos(19, false, true, false));
    map.emplace(__NName(1, L"max"), __Infos(20, false, false, true));
    map.emplace(__NName(1, L"uint32"), __Infos(21, true, false, true));
    map.emplace(__NName(1, L"int64"), __Infos(22, true, false, true));
    map.emplace(__NName(1, L"prod"), __Infos(23, false, false, true));
    map.emplace(__NName(1, L"type"), __Infos(24, false, false, false));
    map.emplace(__NName(1, L"sin"), __Infos(25, true, false, true));
    map.emplace(__NName(1, L"round"), __Infos(26, true, false, true));
    map.emplace(__NName(1, L"int32"), __Infos(27, true, false, true));
    map.emplace(__NName(1, L"inttype"), __Infos(28, false, false, false));
    map.emplace(__NName(1, L"exp"), __Infos(29, true, false, true));
    map.emplace(__NName(1, L"atan"), __Infos(30, true, false, true));
    map.emplace(__NName(1, L"imag"), __Infos(31, false, false, false));
    map.emplace(__NName(1, L"uint8"), __Infos(32, true, false, true));
    map.emplace(__NName(1, L"ceil"), __Infos(33, true, false, true));
    map.emplace(__NName(1, L"uint64"), __Infos(34, true, false, true));
    map.emplace(__NName(1, L"sinm"), __Infos(35, false, false, false));
    map.emplace(__NName(1, L"isinf"), __Infos(36, true, false, true));
    map.emplace(__NName(1, L"sinh"), __Infos(37, true, false, true));
    map.emplace(__NName(1, L"isscalar"), __Infos(38, false, false, false));
    map.emplace(__NName(1, L"sign"), __Infos(39, true, false, true));
    map.emplace(__NName(1, L"int8"), __Infos(40, true, false, true));
    map.emplace(__NName(1, L"cos"), __Infos(41, true, false, true));
    map.emplace(__NName(1, L"and"), __Infos(42, false, false, false));
    map.emplace(__NName(1, L"tanh"), __Infos(43, true, false, true));
    map.emplace(__NName(1, L"isnan"), __Infos(44, true, false, true));
    map.emplace(__NName(1, L"imult"), __Infos(45, false, false, false));
    map.emplace(__NName(1, L"argn"), __Infos(46, false, false, false));
    map.emplace(__NName(1, L"cumprod"), __Infos(47, true, false, true));
    map.emplace(__NName(1, L"erf"), __Infos(48, true, false, true));
    map.emplace(__NName(1, L"ones"), __Infos(49, false, false, false));
    map.emplace(__NName(1, L"erfc"), __Infos(50, true, false, true));
    map.emplace(__NName(1, L"min"), __Infos(51, false, false, true));
    map.emplace(__NName(1, L"sum"), __Infos(52, false, false, true));
    map.emplace(__NName(1, L"acos"), __Infos(53, true, false, false));
    map.emplace(__NName(2, L"rand"), __Infos(0, false, true, false));
    map.emplace(__NName(2, L"isreal"), __Infos(1, false, false, false));
    map.emplace(__NName(2, L"iconvert"), __Infos(2, false, false, false));
    map.emplace(__NName(2, L"gettext"), __Infos(3, false, true, false));
    map.emplace(__NName(2, L"size"), __Infos(4, false, false, false));
    map.emplace(__NName(2, L"or"), __Infos(5, false, false, false));
    map.emplace(__NName(2, L"zeros"), __Infos(6, false, false, false));
    map.emplace(__NName(2, L"matrix"), __Infos(7, false, false, false));
    map.emplace(__NName(2, L"and"), __Infos(8, false, false, false));
    map.emplace(__NName(2, L"ones"), __Infos(9, false, false, false));
    map.emplace(__NName(3, L"matrix"), __Infos(0, false, false, false));
    ARGS0[0] = &check_rand;
    ARGS0[1] = &check_zeros;
    ARGS0[2] = &check_ls;
    ARGS0[3] = &check_toc;
    ARGS0[4] = &check_argn;
    ARGS0[5] = &check_ones;
    ARGS1[0] = &check_rand;
    ARGS1[1] = &check_isreal;
    ARGS1[2] = &check_abs;
    ARGS1[3] = &check_cosh;
    ARGS1[4] = &check_typeof;
    ARGS1[5] = &check_int16;
    ARGS1[6] = &check_gettext;
    ARGS1[7] = &check_tan;
    ARGS1[8] = &check_cumsum;
    ARGS1[9] = &check_size;
    ARGS1[10] = &check_asin;
    ARGS1[11] = &check_or;
    ARGS1[12] = &check_log;
    ARGS1[13] = &check_floor;
    ARGS1[14] = &check_int;
    ARGS1[15] = &check_fix;
    ARGS1[16] = &check_sqrt;
    ARGS1[17] = &check_uint16;
    ARGS1[18] = &check_zeros;
    ARGS1[19] = &check_ls;
    ARGS1[20] = &check_max;
    ARGS1[21] = &check_uint32;
    ARGS1[22] = &check_int64;
    ARGS1[23] = &check_prod;
    ARGS1[24] = &check_type;
    ARGS1[25] = &check_sin;
    ARGS1[26] = &check_round;
    ARGS1[27] = &check_int32;
    ARGS1[28] = &check_inttype;
    ARGS1[29] = &check_exp;
    ARGS1[30] = &check_atan;
    ARGS1[31] = &check_imag;
    ARGS1[32] = &check_uint8;
    ARGS1[33] = &check_ceil;
    ARGS1[34] = &check_uint64;
    ARGS1[35] = &check_sinm;
    ARGS1[36] = &check_isinf;
    ARGS1[37] = &check_sinh;
    ARGS1[38] = &check_isscalar;
    ARGS1[39] = &check_sign;
    ARGS1[40] = &check_int8;
    ARGS1[41] = &check_cos;
    ARGS1[42] = &check_and;
    ARGS1[43] = &check_tanh;
    ARGS1[44] = &check_isnan;
    ARGS1[45] = &check_imult;
    ARGS1[46] = &check_argn;
    ARGS1[47] = &check_cumprod;
    ARGS1[48] = &check_erf;
    ARGS1[49] = &check_ones;
    ARGS1[50] = &check_erfc;
    ARGS1[51] = &check_min;
    ARGS1[52] = &check_sum;
    ARGS1[53] = &check_acos;
    ARGS2[0] = &check_rand;
    ARGS2[1] = &check_isreal;
    ARGS2[2] = &check_iconvert;
    ARGS2[3] = &check_gettext;
    ARGS2[4] = &check_size;
    ARGS2[5] = &check_or;
    ARGS2[6] = &check_zeros;
    ARGS2[7] = &check_matrix;
    ARGS2[8] = &check_and;
    ARGS2[9] = &check_ones;
    ARGS3[0] = &check_matrix;

    return map;
}

}