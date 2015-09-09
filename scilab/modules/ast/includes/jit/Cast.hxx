/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __CAST_HXX__
#define __CAST_HXX__

#include "JITVisitor.hxx"

namespace jit
{

    class Cast
    {

    public:

        template<typename T>
        inline static llvm::Value * cast(llvm::Value * v, const bool sgn, JITVisitor & jit)
            {
                return v->getType()->isIntegerTy() ? __Cast<T, std::is_integral<T>::value, true>::get(v, sgn, jit) : __Cast<T, std::is_integral<T>::value, false>::get(v, sgn, jit);
            }

        /**
         * Promote value according to Scilab's rules:
         *  - int_X+int_Y => int_max(X,Y)
         *  - uint_X+int_Y => uint_max(X,Y)
         *  - int_X+uint_Y => uint_max(X,Y)
         *  - uint_X+uint_Y => uint_max(X,Y)
         *  - double+(u)int_X => (u)int_X
         */
        inline static void promoteScilabValue(const bool lsign, llvm::Value *& l, const bool rsign, llvm::Value *& r, llvm::IRBuilder<> & builder)
            {
                llvm::Type * lt = l->getType();
                llvm::Type * rt = r->getType();

                if (lt != rt)
                {
                    if (lt->isIntegerTy())
                    {
                        if (rt->isIntegerTy())
                        {
                            if (lt->getIntegerBitWidth() > rt->getIntegerBitWidth())
                            {
                                r = rsign ? builder.CreateSExt(r, lt) : builder.CreateZExt(r, lt);
                            }
                            else
                            {
                                l = lsign ? builder.CreateSExt(l, rt) : builder.CreateZExt(l, rt);
                            }
                        }
                        else
                        {
                            r = lsign ? builder.CreateFPToSI(r, lt) : builder.CreateFPToUI(r, lt);
                        }
                    }
                    else
                    {
                        if (rt->isIntegerTy())
                        {
                            l = rsign ? builder.CreateFPToSI(l, rt) : builder.CreateFPToUI(l, rt);
                        }
                        else
                        {
                            if (lt->getPrimitiveSizeInBits() > rt->getPrimitiveSizeInBits())
                            {
                                r = builder.CreateFPExt(r, lt);
                            }
                            else
                            {
                                l = builder.CreateFPExt(l, rt);
                            }
                        }
                    }
                }
            }

	inline static void castInt64(llvm::Value *& v, JITVisitor & jit)
	    {
		if (v->getType() == jit.getTy<int64_t>())
		{
		    v = jit.getBuilder().CreateUIToFP(v, jit.getTy<double>());
		}
	    }

	inline static void castInt1(llvm::Value *& v, JITVisitor & jit)
	    {
		if (v->getType() == jit.getTy<bool>())
		{
		    v = jit.getBuilder().CreateZExt(v, jit.getTy<int32_t>());
		}
	    }

    private:

        // First bool is for if typename is integral
        // Second is for if v is integral
        template<typename, bool, bool>
        struct __Cast
        {
            inline static llvm::Value * get(llvm::Value * v, JITVisitor & jit);
        };

        template<typename T>
        struct __Cast<T, true, true>
        {
            inline static llvm::Value * get(llvm::Value * v, const bool sgn, JITVisitor & jit)
                {
                    return jit.getBuilder().CreateIntCast(v, jit.getTy<T>(), std::is_signed<T>::value);
                }
        };

        template<typename T>
        struct __Cast<T, true, false>
        {
            inline static llvm::Value * get(llvm::Value * v, const bool sgn, JITVisitor & jit)
                {
                    // Workaround for bug: http://llvm.org/bugs/show_bug.cgi?id=21130
                    // TODO: remove it when it will be fixed in llvm trunk
                    llvm::Type * ty = jit.getTy<int64_t>();
                    llvm::Value * i = std::is_signed<T>::value ? jit.getBuilder().CreateFPToSI(v, ty) : jit.getBuilder().CreateFPToUI(v, ty);
                    return jit.getBuilder().CreateIntCast(i, jit.getTy<T>(), std::is_signed<T>::value);
                }
        };

        template<typename T>
        struct __Cast<T, false, true>
        {
            inline static llvm::Value * get(llvm::Value * v, const bool sgn, JITVisitor & jit)
                {
                    return sgn ? jit.getBuilder().CreateSIToFP(v, jit.getTy<T>()) : jit.getBuilder().CreateUIToFP(v, jit.getTy<T>());
                }
        };

        template<typename T>
        struct __Cast<T, false, false>
        {
            inline static llvm::Value * get(llvm::Value * v, const bool sgn, JITVisitor & jit)
                {
                    return jit.getBuilder().CreateFPCast(v, jit.getTy<T>());
                }
        };

    };

    // try to find the Scilab type in operation
    template<bool, bool, typename, typename>
    struct __ScilabType_Helper;

    template<bool, typename, typename>
    struct __ScilabTypeII_Helper;

    template<bool, typename, typename>
    struct __GreaterType_Helper;

    template<typename, typename>
    struct __GreaterType;

    template<typename T, typename U>
    struct __GreaterType_Helper<true, T, U>
    {
        typedef T type;
    };

    template<typename T, typename U>
    struct __GreaterType_Helper<false, T, U>
    {
        typedef U type;
    };

    template<typename T, typename U>
    struct __GreaterType
    {
        typedef typename __GreaterType_Helper<sizeof(T) >= sizeof(U), T, U>::type type;
    };

    template<typename T, typename U>
    struct __ScilabTypeII_Helper<true, T, U>
    {
        typedef typename std::make_unsigned<typename __GreaterType<T, U>::type>::type type;
    };

    template<typename T, typename U>
    struct __ScilabTypeII_Helper<false, T, U>
    {
        typedef typename __GreaterType<T, U>::type type;
    };

    template<typename T, typename U>
    struct __ScilabTypeII
    {
        typedef typename __ScilabTypeII_Helper<std::is_unsigned<T>::value || std::is_unsigned<U>::value, T, U>::type type;
    };

    template<typename T, typename U>
    struct __ScilabType_Helper<true, true, T, U>
    {
        typedef typename __ScilabTypeII<T, U>::type type;
    };

    template<typename U>
    struct __ScilabType_Helper<true, true, bool, U>
    {
        typedef U type;
    };

    template<typename T>
    struct __ScilabType_Helper<true, true, T, bool>
    {
        typedef T type;
    };

    template<>
    struct __ScilabType_Helper<true, true, bool, bool>
    {
        typedef bool type;
    };

    template<typename T, typename U>
    struct __ScilabType_Helper<true, false, T, U>
    {
        typedef T type;
    };

    template<typename U>
    struct __ScilabType_Helper<true, false, bool, U>
    {
        typedef U type;
    };

    template<typename T, typename U>
    struct __ScilabType_Helper<false, true, T, U>
    {
        typedef U type;
    };

    template<typename T>
    struct __ScilabType_Helper<false, true, T, bool>
    {
        typedef T type;
    };

    template<typename T, typename U>
    struct __ScilabType_Helper<false, false, T, U>
    {
        typedef T type;
    };

    template<typename T, typename U>
    struct ScilabType
    {
        typedef typename __ScilabType_Helper<std::is_integral<T>::value, std::is_integral<U>::value, T, U>::type type;
    };

    template<typename T>
    struct is_pure_integral
    {
	const static bool value = std::is_integral<T>::value && !std::is_same<T, bool>::value;
    };

} // namespace jit

#endif // __CAST_HXX__
