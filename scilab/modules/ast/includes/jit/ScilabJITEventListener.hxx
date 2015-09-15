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

#ifndef __SCILAB_JIT_EVENT_LISTENER_HXX__
#define __SCILAB_JIT_EVENT_LISTENER_HXX__

#include "llvm/ExecutionEngine/JITEventListener.h"

namespace llvm
{
namespace object
{
class ObjectFile;
}
}

namespace jit
{
class ScilabJITEventListener : public llvm::JITEventListener
{

public:

#if defined(LLVM_VERSION_MAJOR) && LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR >= 7
    virtual void NotifyObjectEmitted(const llvm::object::ObjectFile & obj, const llvm::RuntimeDyld::LoadedObjectInfo & L) override;
#else
    virtual void NotifyObjectEmitted(const llvm::ObjectImage & obj) override;
#endif
};


} // namespace jit

#endif // __SCILAB_JIT_EVENT_LISTENER_HXX__
