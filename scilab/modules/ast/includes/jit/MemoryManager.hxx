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

#ifndef __MEMORY_MANAGER_HXX__
#define __MEMORY_MANAGER_HXX__

#include <cstdint>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "llvm/ExecutionEngine/SectionMemoryManager.h"

#ifdef _MSC_VER
#define __SCILABJIT_CAST_FUN__(x) (uint64_t)(void (*)())(x)
#else
#define __SCILABJIT_CAST_FUN__(x) (uint64_t)(x)
#endif

namespace jit
{

    class MemoryManager : public llvm::SectionMemoryManager
    {
	typedef std::unordered_map<std::string, uint64_t> SymbolsAddressMap;
	
	MemoryManager(const MemoryManager &) = delete;
	void operator=(const MemoryManager &) = delete;
	
	static SymbolsAddressMap syms;
	static SymbolsAddressMap init();
	
    public:

	MemoryManager() : llvm::SectionMemoryManager() { }

	virtual ~MemoryManager() { }
	virtual uint64_t getSymbolAddress(const std::string & name);


	virtual uint8_t *allocateCodeSection(uintptr_t Size, unsigned Alignment, unsigned SectionID, llvm::StringRef SectionName);
	virtual uint8_t *allocateDataSection(uintptr_t Size, unsigned Alignment, unsigned SectionID, llvm::StringRef SectionName, bool isReadOnly);
	
	template<typename T>
	inline static void addSymbol(const std::string & name, T address)
	    {
		syms.emplace(name, __SCILABJIT_CAST_FUN__(address));
	    }

	inline static void addSymbol(const std::string & name, uint64_t address)
	    {
		syms.emplace(name, address);
	    }

	template<typename T>
	inline static void addSymbol(const std::string && name, T address)
	    {
		syms.emplace(name, __SCILABJIT_CAST_FUN__(address));
	    }

	inline static std::size_t getMapSize()
	    {
		return syms.size();
	    }

    private:

	template<typename T>
	inline static void addSymbol(SymbolsAddressMap & map, const std::string & name, T address)
	    {
		map.emplace(name, __SCILABJIT_CAST_FUN__(address));
	    }

	template<typename T>
	inline static void addSymbol(SymbolsAddressMap & map, const std::string && name, T address)
	    {
		map.emplace(name, __SCILABJIT_CAST_FUN__(address));
	    }
    };
    
} // namespace jit

#endif // __MEMORY_MANAGER_HXX__
