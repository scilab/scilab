/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string.h>

#include "AnalysisVisitor.hxx"
#include "JITVisitor.hxx"

#include "parser.hxx"
#include "functions_gw.hxx"
#include "execvisitor.hxx"
#include "mutevisitor.hxx"
#include "debugvisitor.hxx"
#include "printvisitor.hxx"
#include "visitor_common.hxx"
#include "scilabWrite.hxx"
#include "scilabexception.hxx"
#include "configvariable.hxx"
#include "context.hxx"

#include <iostream>
#include <fstream>
#include <string>

extern "C"
{
#include "sci_malloc.h"
#include "os_wcsicmp.h"
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "os_swprintf.h"
}

#ifdef _MSC_VER
#pragma comment(lib, "LLVMLinker.lib")
#pragma comment(lib, "LLVMNVPTXCodeGen.lib")
#pragma comment(lib, "LLVMNVPTXDesc.lib")
#pragma comment(lib, "LLVMNVPTXInfo.lib")
#pragma comment(lib, "LLVMObjCARCOpts.lib")
#pragma comment(lib, "LLVMObject.lib")
#pragma comment(lib, "LLVMOption.lib")
#pragma comment(lib, "LLVMPowerPCAsmParser.lib")
#pragma comment(lib, "LLVMPowerPCAsmPrinter.lib")
#pragma comment(lib, "LLVMPowerPCCodeGen.lib")
#pragma comment(lib, "LLVMPowerPCDesc.lib")
#pragma comment(lib, "LLVMPowerPCInfo.lib")
#pragma comment(lib, "LLVMR600AsmPrinter.lib")
#pragma comment(lib, "LLVMR600CodeGen.lib")
#pragma comment(lib, "LLVMR600Desc.lib")
#pragma comment(lib, "LLVMR600Info.lib")
#pragma comment(lib, "LLVMRuntimeDyld.lib")
#pragma comment(lib, "LLVMScalarOpts.lib")
#pragma comment(lib, "LLVMSelectionDAG.lib")
#pragma comment(lib, "LLVMSparcCodeGen.lib")
#pragma comment(lib, "LLVMSparcDesc.lib")
#pragma comment(lib, "LLVMSparcInfo.lib")
#pragma comment(lib, "LLVMSupport.lib")
#pragma comment(lib, "LLVMSystemZAsmParser.lib")
#pragma comment(lib, "LLVMSystemZAsmPrinter.lib")
#pragma comment(lib, "LLVMSystemZCodeGen.lib")
#pragma comment(lib, "LLVMSystemZDesc.lib")
#pragma comment(lib, "LLVMSystemZDisassembler.lib")
#pragma comment(lib, "LLVMSystemZInfo.lib")
#pragma comment(lib, "LLVMTableGen.lib")
#pragma comment(lib, "LLVMTarget.lib")
#pragma comment(lib, "LLVMTransformUtils.lib")
#pragma comment(lib, "LLVMVectorize.lib")
#pragma comment(lib, "LLVMX86AsmParser.lib")
#pragma comment(lib, "LLVMX86AsmPrinter.lib")
#pragma comment(lib, "LLVMX86CodeGen.lib")
#pragma comment(lib, "LLVMX86Desc.lib")
#pragma comment(lib, "LLVMX86Disassembler.lib")
#pragma comment(lib, "LLVMX86Info.lib")
#pragma comment(lib, "LLVMX86Utils.lib")
#pragma comment(lib, "LLVMXCoreAsmPrinter.lib")
#pragma comment(lib, "LLVMXCoreCodeGen.lib")
#pragma comment(lib, "LLVMXCoreDesc.lib")
#pragma comment(lib, "LLVMXCoreDisassembler.lib")
#pragma comment(lib, "LLVMXCoreInfo.lib")
#pragma comment(lib, "LLVMipa.lib")
#pragma comment(lib, "LLVMipo.lib")
#pragma comment(lib, "LTO.lib")
#pragma comment(lib, "gtest.lib")
#pragma comment(lib, "gtest_main.lib")
#pragma comment(lib, "LLVMAArch64AsmParser.lib")
#pragma comment(lib, "LLVMAArch64AsmPrinter.lib")
#pragma comment(lib, "LLVMAArch64CodeGen.lib")
#pragma comment(lib, "LLVMAArch64Desc.lib")
#pragma comment(lib, "LLVMAArch64Disassembler.lib")
#pragma comment(lib, "LLVMAArch64Info.lib")
#pragma comment(lib, "LLVMAArch64Utils.lib")
#pragma comment(lib, "LLVMARMAsmParser.lib")
#pragma comment(lib, "LLVMARMAsmPrinter.lib")
#pragma comment(lib, "LLVMARMCodeGen.lib")
#pragma comment(lib, "LLVMARMDesc.lib")
#pragma comment(lib, "LLVMARMDisassembler.lib")
#pragma comment(lib, "LLVMARMInfo.lib")
#pragma comment(lib, "LLVMAnalysis.lib")
#pragma comment(lib, "LLVMAsmParser.lib")
#pragma comment(lib, "LLVMAsmPrinter.lib")
#pragma comment(lib, "LLVMBitReader.lib")
#pragma comment(lib, "LLVMBitWriter.lib")
#pragma comment(lib, "LLVMCodeGen.lib")
#pragma comment(lib, "LLVMCore.lib")
#pragma comment(lib, "LLVMCppBackendCodeGen.lib")
#pragma comment(lib, "LLVMCppBackendInfo.lib")
#pragma comment(lib, "LLVMDebugInfo.lib")
#pragma comment(lib, "LLVMExecutionEngine.lib")
#pragma comment(lib, "LLVMHexagonAsmPrinter.lib")
#pragma comment(lib, "LLVMHexagonCodeGen.lib")
#pragma comment(lib, "LLVMHexagonDesc.lib")
#pragma comment(lib, "LLVMHexagonInfo.lib")
#pragma comment(lib, "LLVMIRReader.lib")
#pragma comment(lib, "LLVMInstCombine.lib")
#pragma comment(lib, "LLVMInstrumentation.lib")
#pragma comment(lib, "LLVMInterpreter.lib")
#pragma comment(lib, "LLVMJIT.lib")
#pragma comment(lib, "LLVMLTO.lib")
#pragma comment(lib, "LLVMLinker.lib")
#pragma comment(lib, "LLVMMC.lib")
#pragma comment(lib, "LLVMMCDisassembler.lib")
#pragma comment(lib, "LLVMMCJIT.lib")
#pragma comment(lib, "LLVMMCParser.lib")
#pragma comment(lib, "LLVMMSP430AsmPrinter.lib")
#pragma comment(lib, "LLVMMSP430CodeGen.lib")
#pragma comment(lib, "LLVMMSP430Desc.lib")
#pragma comment(lib, "LLVMMSP430Info.lib")
#pragma comment(lib, "LLVMMipsAsmParser.lib")
#pragma comment(lib, "LLVMMipsAsmPrinter.lib")
#pragma comment(lib, "LLVMMipsCodeGen.lib")
#pragma comment(lib, "LLVMMipsDesc.lib")
#pragma comment(lib, "LLVMMipsDisassembler.lib")
#pragma comment(lib, "LLVMMipsInfo.lib")
#pragma comment(lib, "LLVMNVPTXAsmPrinter.lib")

#endif


using namespace std;
using namespace types;
using namespace ast;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_jit(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    ast::Exp * pExp = 0;

    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "jit" , 1);
        return Function::Error;
    }

    if (!in[0]->isString() || in[0]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "jit" , 1);
        return Function::Error;
    }

    Parser parser;
    parser.parse(in[0]->getAs<types::String>()->get(0));
    if (parser.getExitStatus() != Parser::Succeded)
    {
        char* pst = wide_string_to_UTF8(parser.getErrorMessage());
        Scierror(999, "%s", pst);
        FREE(pst);
        return Function::Error;
    }

    pExp = parser.getTree();

    if (!pExp)
    {
        return Function::Error;
    }

    analysis::AnalysisVisitor analysis;
    pExp->accept(analysis);
    analysis.print_info();

    ast::DebugVisitor debugMe;
    pExp->accept(debugMe);

    jit::JITVisitor jit(analysis);
    try
    {
        pExp->accept(jit);
    }
    catch (ast::ScilabError & se)
    {
        jit.dump();
        throw;
    }

    jit.run();

    delete pExp;

    return Function::OK;
}
