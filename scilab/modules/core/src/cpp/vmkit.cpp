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

#include "vmkit.hxx"

#ifdef VMKIT_ENABLED


namespace VMKitScilab
{
void ScilabThread::setArgs(ScilabEngineInfo* _pSEI)
{
    this->pSEI = _pSEI;
}

void ScilabThread::setret(int ret)
{
    this->ret = ret;
}

ScilabEngineInfo* ScilabThread::getArgs()
{
    return pSEI;
}

int ScilabThread::getret()
{
    return ret;
}

ScilabThread::ScilabThread (ScilabVM* vm) : vmkit::MutatorThread()
{
    MyVM = (vmkit::VirtualMachine*)vm;
}

ScilabVM* ScilabThread::vm()
{
    return (ScilabVM *)MyVM;
}

void ScilabThread::execute()
{
    StartScilabEngine(pSEI);
    setret(RunScilabEngine(pSEI));
    StopScilabEngine(pSEI);
}

void ScilabVM::mainStart(ScilabThread* thread)
{
    ScilabVM* vm = thread->vm();
    printf("\nVMKitThread: Create\n\n");

    thread->execute();

    vm->setret(thread->getret());

    printf("\nVMKitThread: Exit\n\n");
    vm->exit();
}

void ScilabVM::runApplication(ScilabEngineInfo* _pSEI)
{
    ScilabThread * mainThread = new ScilabThread(this);

    mainThread->setArgs(_pSEI);

    mainThread->start((void (*)(vmkit::Thread *))mainStart);
}

int ScilabVM::getret()
{
    return ret;
}

void ScilabVM::setret(int ret)
{
    this->ret = ret;
}

}
#endif
