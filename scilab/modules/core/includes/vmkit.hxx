/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013-2013 - LIP6 - Peter Senna Tschudin
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C" {
    #include "machine.h"
    #include "InitScilab.h"
}

#include "MutatorThread.h"
#include "vmkit/System.h"
#include "vmkit/Locks.h"
#include "vmkit/Thread.h"
#include "vmkit/VirtualMachine.h"


namespace VMKitScilab
{
class ScilabVM;

class ScilabThread : public vmkit::MutatorThread
{
private:
    ScilabEngineInfo* pSEI;
    int ret;

public:
    void setArgs(ScilabEngineInfo* _pSEI);
    void setret(int ret);
    ScilabEngineInfo* getArgs();
    int getret();

    ScilabThread (ScilabVM* vm);
    ScilabVM* vm();
    void execute();
};

class ScilabVM : public vmkit::VirtualMachine
{
private:
    int ret;

public:
    vmkit::BumpPtrAllocator Alloc;
    vmkit::CompiledFrames **frames;

    ScilabVM (vmkit::BumpPtrAllocator& Alloc, vmkit::CompiledFrames** frames) : VirtualMachine(Alloc, frames) {}
    ~ScilabVM() {}

    virtual void startCollection() {}
    virtual void endCollection() {}
    virtual void scanWeakReferencesQueue(word_t closure) {}
    virtual void scanSoftReferencesQueue(word_t closure) {}
    virtual void scanPhantomReferencesQueue(word_t closure) {}
    virtual void scanFinalizationQueue(word_t closure) {}
    virtual void addFinalizationCandidate(gc* obj) {}
    virtual void finalizeObject(gc* res) {}
    virtual void traceObject(gc* obj, word_t closure) {}
    virtual void setType(gc* header, void* type) {}
    virtual void setType(void* header, void* type) {}
    virtual void* getType(gc* obj) {}
    virtual size_t getObjectSize(gc* obj) {}
    virtual const char* getObjectTypeName(gc* obj) {}
    virtual bool isCorruptedType(gc* header) {}
    virtual void printMethod(vmkit::FrameInfo* FI, word_t ip, word_t addr) {}
    virtual void invokeEnqueueReference(gc* res) {}
    virtual void clearObjectReferent(gc* ref) {}
    virtual gc** getObjectReferentPtr(gc* _obj) {}
    virtual void setObjectReferent(gc* _obj, gc* val) {}

    virtual void runApplication(int argc, char** argv) {}

    virtual void runApplication(ScilabEngineInfo *_pSEI);

    virtual void nullPointerException() {}
    virtual void stackOverflowError() {}

    static void mainStart(ScilabThread* thread);
    int getret();
    void setret(int ret);
};
}
