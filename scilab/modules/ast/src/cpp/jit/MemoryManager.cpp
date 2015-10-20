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

#include "MemoryManager.hxx"
#include "BaseFunctions.hxx"
#include "BaseFunctionsMacros.h"
#include "faddeeva.h"
//#include "configvariable.hxx"

/*
  Un moyen de gerer les extern C++ (plutot que de les wrapper et de perdre du temps avec un call supplementaire
  c'est de mettre la fonction ds sam:
  pr foo::bar::toto(int x) on mets foo_bar_toto_S_i8 -> foo::bar::toto
 */
namespace jit
{

MemoryManager::SymbolsAddressMap MemoryManager::syms(init());

uint64_t MemoryManager::getSymbolAddress(const std::string & name)
{
    SymbolsAddressMap::const_iterator i = syms.find(name);
    if (i == syms.end())
    {
        const uint64_t ptr = SectionMemoryManager::getSymbolAddress(name);
        if (ptr)
        {
            addSymbol(name, ptr);
            return ptr;
        }
    }
    else
    {
        return i->second;
    }
}

uint8_t * MemoryManager::allocateCodeSection(uintptr_t Size, unsigned Alignment, unsigned SectionID, llvm::StringRef SectionName)
{
    //printf("Allocating code section: %s %ld %d %d\n", SectionName.data(), Size, Alignment, SectionID);
    return llvm::SectionMemoryManager::allocateCodeSection(Size, Alignment, SectionID, SectionName);
}

uint8_t * MemoryManager::allocateDataSection(uintptr_t Size, unsigned Alignment, unsigned SectionID, llvm::StringRef SectionName, bool isReadOnly)
{
    //printf("Allocating data section: %s %ld %d %d %d\n", SectionName.data(), Size, Alignment, SectionID, isReadOnly);
    return llvm::SectionMemoryManager::allocateDataSection(Size, Alignment, SectionID, SectionName, isReadOnly);
}


/*    char * aligned_new(size_t alignment, size_t size)
    {
	return aligned_alloc();
    }

    void MYDELETE(void * p)
    {
	std::cout << "DELETE=" << p << std::endl;
	delete[] (char*)p;
    }
*/

char * MYNEW(std::size_t size)
{
    std::cout << "NEW=" << size << std::endl;
    char * p =  new char[size];
    return p;
}

void MYDGEMM(char * n, char * n1, int * x_r, int * y_c, int * x_c, double * one, double * x, int * x_r_1, double * y, int * x_c_1, double * zero, double * o, int * x_r_2)
{
    std::cout << "MYDGEMM=" << *x_r << ":::" << *x_c << ":::" << *y_c << "::" << *one << "::" << *x_r_1 << "::" << *x_c_1 << "::" << *zero << "::" << *x_r_2 << ":::::" << n << ":::" << n1 << std::endl;

    //dgemm_(n, n1, x_r, y_c, x_c, one, x, x_r_1, y, x_c_1, zero, o, x_r_2);
}


MemoryManager::SymbolsAddressMap MemoryManager::init()
{
    MemoryManager::SymbolsAddressMap sam;
    sam.reserve(10000);

    // TODO: maybe put the mangled names for these two operators (Windows or Unix mangling)
    // I don't if llvm is able to make something if it knows that new is new !
#ifdef _MSC_VER
    addSymbol(sam, "delete", (void(*)(void *))operator delete[]);
#else
    addSymbol(sam, "delete", (void(*)(void *)noexcept)operator delete[]);
#endif
    addSymbol(sam, "new", (void * (*)(size_t))operator new[]);
    //addSymbol(sam, "new", (void * (*)(size_t))MYNEW);
    addSymbol(sam, "MyDGEMM", MYDGEMM);

    // TODO: maybe we should get directly the pointer of the ConfigVariable::m_iIeee
    addSymbol(sam, "getieee", ConfigVariable::getIeee);

    //addSymbol(sam, "delete", &MYDELETE);
    //addSymbol(sam, "new", &MYNEW);

    //addSymbol(sam, "add_SdMd", BaseFunctions::binSM<double, double, double, BaseFunctions::sum>);
    //addSymbol(sam, "add_MdSd", BaseFunctions::binMS<double, double, double, BaseFunctions::sum>);
    //addSymbol(sam, "add_MdMd", BaseFunctions::binMM<double, double, double, BaseFunctions::sum>);
    //addSymbol(sam, "breakOneq_MdSd", BaseFunctions::breakOnMS<false, double, double, BaseFunctions::eq>);
    //addSymbol(sam, "breakOneq_SdMd", BaseFunctions::breakOnSM<false, double, double, BaseFunctions::eq>);
    //addSymbol(sam, "breakOneq_MdMd", BaseFunctions::breakOnMM<false, double, double, BaseFunctions::eq>);
    //addSymbol(sam, "toBoolean_Md", BaseFunctions::breakOnM<false, double, BaseFunctions::asBool>);
    //addSymbol(sam, "sin_Md", BaseFunctions::vectorize<double, double, std::sin>);
    //addSymbol(sam, "cos_Md", BaseFunctions::vectorize<double, double, std::cos>);
    //addSymbol(sam, "exp_Md", BaseFunctions::vectorize<double, double, std::exp>);
    //addSymbol(sam, "sign_Md", BaseFunctions::vectorize<double, double, BaseFunctions::sign>);
    //addSymbol(sam, "rdiv_MdMd", (void (*)(double*, int64_t, int64_t, double*, int64_t, int64_t, double**))BaseFunctions::rdiv);
    //addSymbol(sam, "erfcx_Sd", (double (*)(double))Faddeeva::erfcx);
    addSymbol(sam, "powMat_MdSd", BaseFunctions::powMat);

    return sam;
}
}
