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

// This file has been generated, so don't modify it by hand !!

#ifndef __CHECKERS_HXX__
#define __CHECKERS_HXX__

#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "TIType.hxx"

#include "check_rand.hxx"
#include "check_int.hxx"
#include "check_transp.hxx"
#include "check_dothypot.hxx"
#include "check_cosh.hxx"
#include "check_hypot.hxx"
#include "check_tan.hxx"
#include "check_cumsum.hxx"
#include "check_asin.hxx"
#include "check_isnan.hxx"
#include "check_sub.hxx"
#include "check_floor.hxx"
#include "check_dottimes.hxx"
#include "check_fix.hxx"
#include "check_sqrt.hxx"
#include "check_uint16.hxx"
#include "check_add.hxx"
#include "check_abs.hxx"
#include "check_zeros.hxx"
#include "check_max.hxx"
#include "check_uint32.hxx"
#include "check_int64.hxx"
#include "check_prod.hxx"
#include "check_sin.hxx"
#include "check_krontimes.hxx"
#include "check_int32.hxx"
#include "check_dottransp.hxx"
#include "check_int16.hxx"
#include "check_exp.hxx"
#include "check_atan.hxx"
#include "check_dotrdiv.hxx"
#include "check_uint8.hxx"
#include "check_ceil.hxx"
#include "check_sinm.hxx"
#include "check_isinf.hxx"
#include "check_sinh.hxx"
#include "check_int8.hxx"
#include "check_log.hxx"
#include "check_uint64.hxx"
#include "check_cos.hxx"
#include "check_tanh.hxx"
#include "check_min.hxx"
#include "check_times.hxx"
#include "check_cumprod.hxx"
#include "check_erf.hxx"
#include "check_ones.hxx"
#include "check_erfc.hxx"
#include "check_sum.hxx"
#include "check_acos.hxx"
#include "check_round.hxx"

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
            inline bool operator==(const __NName & R) const { return nargs == R.nargs && name == R.name; }
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
            inline std::size_t operator()(const __NName & nn) const { return (std::hash<std::wstring>()(nn.name) << 4) ^ nn.nargs; }
        };

        typedef std::unordered_map<__NName, __Infos, __Hasher> CheckersMap;
        static CheckersMap map;
        static TIType (*ARGS0[3])(GVN &);
        static TIType (*ARGS1[40])(GVN &, const TIType &);
        static TIType (*ARGS2[5])(GVN &, const TIType &, const TIType &);

    public:
        
        static TIType check(GVN & gvn, const std::wstring & name, const std::vector<TIType> & args)
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
                }
            }
            
            return TIType(gvn);
        }

        static bool isElementWise(const std::wstring & name, unsigned short nargs = 1)
        {
            CheckersMap::const_iterator i = map.find(__NName(nargs, name));
            if (i != map.end())
            {
                return i->second.elementwise;
            }
            return false;
        }

        static bool isJitted(const std::wstring & name, unsigned short nargs = 1)
        {
            CheckersMap::const_iterator i = map.find(__NName(nargs, name));
            if (i != map.end())
            {
                return i->second.jitted;
            }
            return false;
        }

    private:

        static CheckersMap initMap()
        {
            CheckersMap map;
            map.emplace(__NName(0, L"rand"), __Infos(0, false, true, false));
            map.emplace(__NName(0, L"zeros"), __Infos(1, false, false, false));
            map.emplace(__NName(0, L"ones"), __Infos(2, false, false, false));
            map.emplace(__NName(1, L"rand"), __Infos(0, false, true, false));
            map.emplace(__NName(1, L"int"), __Infos(1, true, false, true));
            map.emplace(__NName(1, L"cosh"), __Infos(2, true, false, true));
            map.emplace(__NName(1, L"tan"), __Infos(3, true, false, true));
            map.emplace(__NName(1, L"cumsum"), __Infos(4, true, false, true));
            map.emplace(__NName(1, L"asin"), __Infos(5, true, false, false));
            map.emplace(__NName(1, L"isnan"), __Infos(6, true, false, true));
            map.emplace(__NName(1, L"floor"), __Infos(7, true, false, true));
            map.emplace(__NName(1, L"fix"), __Infos(8, true, false, true));
            map.emplace(__NName(1, L"sqrt"), __Infos(9, true, false, true));
            map.emplace(__NName(1, L"uint16"), __Infos(10, true, false, true));
            map.emplace(__NName(1, L"abs"), __Infos(11, true, false, true));
            map.emplace(__NName(1, L"zeros"), __Infos(12, false, false, false));
            map.emplace(__NName(1, L"max"), __Infos(13, false, false, true));
            map.emplace(__NName(1, L"uint32"), __Infos(14, true, false, true));
            map.emplace(__NName(1, L"int64"), __Infos(15, true, false, true));
            map.emplace(__NName(1, L"prod"), __Infos(16, false, false, true));
            map.emplace(__NName(1, L"sin"), __Infos(17, true, false, true));
            map.emplace(__NName(1, L"int32"), __Infos(18, true, false, true));
            map.emplace(__NName(1, L"int16"), __Infos(19, true, false, true));
            map.emplace(__NName(1, L"exp"), __Infos(20, true, false, true));
            map.emplace(__NName(1, L"atan"), __Infos(21, true, false, false));
            map.emplace(__NName(1, L"uint8"), __Infos(22, true, false, true));
            map.emplace(__NName(1, L"ceil"), __Infos(23, true, false, true));
            map.emplace(__NName(1, L"sinm"), __Infos(24, false, false, false));
            map.emplace(__NName(1, L"isinf"), __Infos(25, true, false, true));
            map.emplace(__NName(1, L"sinh"), __Infos(26, true, false, true));
            map.emplace(__NName(1, L"int8"), __Infos(27, true, false, true));
            map.emplace(__NName(1, L"log"), __Infos(28, true, false, false));
            map.emplace(__NName(1, L"uint64"), __Infos(29, true, false, true));
            map.emplace(__NName(1, L"cos"), __Infos(30, true, false, true));
            map.emplace(__NName(1, L"tanh"), __Infos(31, true, false, true));
            map.emplace(__NName(1, L"min"), __Infos(32, false, false, true));
            map.emplace(__NName(1, L"cumprod"), __Infos(33, true, false, true));
            map.emplace(__NName(1, L"erf"), __Infos(34, true, false, true));
            map.emplace(__NName(1, L"ones"), __Infos(35, false, false, false));
            map.emplace(__NName(1, L"erfc"), __Infos(36, true, false, true));
            map.emplace(__NName(1, L"sum"), __Infos(37, false, false, true));
            map.emplace(__NName(1, L"acos"), __Infos(38, true, false, false));
            map.emplace(__NName(1, L"round"), __Infos(39, true, false, true));
            map.emplace(__NName(2, L"rand"), __Infos(0, false, true, false));
            map.emplace(__NName(2, L"dothypot"), __Infos(1, true, false, true));
            map.emplace(__NName(2, L"hypot"), __Infos(2, false, false, true));
            map.emplace(__NName(2, L"zeros"), __Infos(3, false, false, false));
            map.emplace(__NName(2, L"ones"), __Infos(4, false, false, false));           
            ARGS0[0] = &check_rand;
            ARGS0[1] = &check_zeros;
            ARGS0[2] = &check_ones;
            ARGS1[0] = &check_rand;
            ARGS1[1] = &check_int;
            ARGS1[2] = &check_cosh;
            ARGS1[3] = &check_tan;
            ARGS1[4] = &check_cumsum;
            ARGS1[5] = &check_asin;
            ARGS1[6] = &check_isnan;
            ARGS1[7] = &check_floor;
            ARGS1[8] = &check_fix;
            ARGS1[9] = &check_sqrt;
            ARGS1[10] = &check_uint16;
            ARGS1[11] = &check_abs;
            ARGS1[12] = &check_zeros;
            ARGS1[13] = &check_max;
            ARGS1[14] = &check_uint32;
            ARGS1[15] = &check_int64;
            ARGS1[16] = &check_prod;
            ARGS1[17] = &check_sin;
            ARGS1[18] = &check_int32;
            ARGS1[19] = &check_int16;
            ARGS1[20] = &check_exp;
            ARGS1[21] = &check_atan;
            ARGS1[22] = &check_uint8;
            ARGS1[23] = &check_ceil;
            ARGS1[24] = &check_sinm;
            ARGS1[25] = &check_isinf;
            ARGS1[26] = &check_sinh;
            ARGS1[27] = &check_int8;
            ARGS1[28] = &check_log;
            ARGS1[29] = &check_uint64;
            ARGS1[30] = &check_cos;
            ARGS1[31] = &check_tanh;
            ARGS1[32] = &check_min;
            ARGS1[33] = &check_cumprod;
            ARGS1[34] = &check_erf;
            ARGS1[35] = &check_ones;
            ARGS1[36] = &check_erfc;
            ARGS1[37] = &check_sum;
            ARGS1[38] = &check_acos;
            ARGS1[39] = &check_round;
            ARGS2[0] = &check_rand;
            ARGS2[1] = &check_dothypot;
            ARGS2[2] = &check_hypot;
            ARGS2[3] = &check_zeros;
            ARGS2[4] = &check_ones;

            return map;
        }
    };

} // namespace analysis

#endif // __CHECKERS_HXX__
