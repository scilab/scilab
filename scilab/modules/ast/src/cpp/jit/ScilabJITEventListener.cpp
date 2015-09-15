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

#include <memory>
#include <iostream>

#include "ScilabJITEventListener.hxx"
#include "MemoryManager.hxx"

#if defined(LLVM_VERSION_MAJOR) && LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR >= 7
#include "llvm/Object/ObjectFile.h"
#else
#include "llvm/Object/ObjectFile.h"
#include "llvm/DebugInfo.h"
#include "llvm/DebugInfo/DIContext.h"
#include "llvm/Support/Memory.h"
#endif

namespace jit
{
#if defined(LLVM_VERSION_MAJOR) && LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR >= 7
void ScilabJITEventListener::NotifyObjectEmitted(const llvm::object::ObjectFile & obj, const llvm::RuntimeDyld::LoadedObjectInfo & L)
{

}

#else
void ScilabJITEventListener::NotifyObjectEmitted(const llvm::ObjectImage & obj)
{
    std::cout << "Object Emitted:" << /*obj.getImageName().data() << */std::endl;
    std::unique_ptr<llvm::DIContext> dicontext(llvm::DIContext::getDWARFContext(obj.getObjectFile()));
    llvm::error_code err;
    for (llvm::object::symbol_iterator i = obj.begin_symbols(), end = obj.end_symbols(); i != end; i.increment(err))
    {
        llvm::object::SymbolRef::Type type;
        if (!i->getType(type) && type == llvm::object::SymbolRef::ST_Function)
        {
            llvm::StringRef name;
            uint64_t addr, size;
            if (!i->getName(name) && !i->getAddress(addr) && !i->getSize(size))
            {
                //MemoryManager::addSymbol(name.str(), addr);

                llvm::DILineInfoTable lines = dicontext->getLineInfoForAddressRange(addr, size, llvm::DILineInfoSpecifier::FunctionName | llvm::DILineInfoSpecifier::FileLineInfo | llvm::DILineInfoSpecifier::AbsoluteFilePath);
                std::cout << "Name:" << name.data() << ", Addr:" << (void *)(intptr_t)addr << ", Size:" << size << "." << std::endl;
                for (int j = 0; j < lines.size(); ++j)
                {
                    std::cout << lines[j].second.getFileName() << ": "
                              << lines[j].second.getLine() << ", "
                              << lines[j].second.getFunctionName() << ": "
                              << lines[j].first << "." << std::endl;
                }
            }
        }
    }
}
#endif


} // namespace jit
