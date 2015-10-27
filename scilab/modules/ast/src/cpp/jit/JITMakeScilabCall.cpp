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

#include "JITScalars.hxx"
#include "JITArrayofs.hxx"
#include "JITVisitor.hxx"
#include "Cast.hxx"

namespace jit
{

void JITVisitor::makeCallFromScilab(const uint64_t functionId, const types::typed_list & in, types::typed_list & out)
{
    auto info_it = info.find(functionId);
    const analysis::TITypeSignatureTuple & outSignature = info_it->second.getOutSignature();

    if (!info_it->second.getWrapper())
    {
        const analysis::TITypeSignatureTuple & inSignature = info_it->second.getInSignature();
        const std::string _id = std::to_string(functionId);
        const std::string wrapper = _id;
        llvm::Module & M = *new llvm::Module("JIT" + std::to_string(id++), context);
        engine->addModule(std::unique_ptr<llvm::Module>(&M));
        M.setDataLayout(*engine->getDataLayout());
        M.setTargetTriple(target->getTargetTriple().str());
        llvm::Type * argsTy[] = { int64PtrTy };
        llvm::Function * toCall = getFunction(info_it->second.getName());
        llvm::FunctionType * ftype = llvm::FunctionType::get(voidTy, argsTy, /* isVarArgs */ false);
        llvm::Function * function = static_cast<llvm::Function *>(M.getOrInsertFunction(wrapper, ftype));
        llvm::BasicBlock * BB = llvm::BasicBlock::Create(context, "", function);
        std::vector<llvm::Value *> args;
        builder.SetInsertPoint(BB);

        uint64_t i = 0;
        llvm::Function::arg_iterator ai = function->arg_begin();
        for (const auto & s : inSignature.getTypes())
        {
            llvm::Value * x = builder.CreateGEP(ai, getConstant<int64_t>(i++));
            x = builder.CreateAlignedLoad(x, sizeof(uint64_t));
            if (s.scalar)
            {
                switch (s.type)
                {
                    case analysis::TIType::DOUBLE:
                    {
                        args.emplace_back(builder.CreateBitCast(x, dblTy));
                        break;
                    }
                    case analysis::TIType::COMPLEX:
                    {
                        args.emplace_back(builder.CreateBitCast(x, dblTy));
                        x = builder.CreateGEP(ai, getConstant<int64_t>(i++));
                        x = builder.CreateAlignedLoad(x, sizeof(uint64_t));
                        args.emplace_back(builder.CreateBitCast(x, dblTy));
                        break;
                    }
                    case analysis::TIType::BOOLEAN:
                    {
                        args.emplace_back(builder.CreateTrunc(x, int32Ty));
                        break;
                    }
                    case analysis::TIType::INT8:
                    {
                        args.emplace_back(builder.CreateTrunc(x, int8Ty));
                        break;
                    }
                    case analysis::TIType::INT16:
                    {
                        args.emplace_back(builder.CreateTrunc(x, int16Ty));
                        break;
                    }
                    case analysis::TIType::INT32:
                    {
                        args.emplace_back(builder.CreateTrunc(x, int32Ty));
                        break;
                    }
                    case analysis::TIType::INT64:
                    {
                        break;
                    }
                    case analysis::TIType::UINT8:
                    {
                        args.emplace_back(builder.CreateTrunc(x, int8Ty));
                        break;
                    }
                    case analysis::TIType::UINT16:
                    {
                        args.emplace_back(builder.CreateTrunc(x, int16Ty));
                        break;
                    }
                    case analysis::TIType::UINT32:
                    {
                        args.emplace_back(builder.CreateTrunc(x, int32Ty));
                        break;
                    }
                    case analysis::TIType::UINT64:
                    {
                        break;
                    }
                    default:
                        break;
                }
            }
            else
            {
                switch (s.type)
                {
                    case analysis::TIType::DOUBLE:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, dblPtrTy));
                        break;
                    }
                    case analysis::TIType::COMPLEX:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, dblPtrTy));
                        x = builder.CreateGEP(ai, getConstant<int64_t>(i++));
                        x = builder.CreateAlignedLoad(x, sizeof(uint64_t));
                        args.emplace_back(builder.CreateIntToPtr(x, dblPtrTy));
                        break;
                    }
                    case analysis::TIType::BOOLEAN:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, int32PtrTy));
                        break;
                    }
                    case analysis::TIType::INT8:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, int8PtrTy));
                        break;
                    }
                    case analysis::TIType::INT16:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, int16PtrTy));
                        break;
                    }
                    case analysis::TIType::INT32:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, int32PtrTy));
                        break;
                    }
                    case analysis::TIType::INT64:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, int64PtrTy));
                        break;
                    }
                    case analysis::TIType::UINT8:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, int8PtrTy));
                        break;
                    }
                    case analysis::TIType::UINT16:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, int16PtrTy));
                        break;
                    }
                    case analysis::TIType::UINT32:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, int32PtrTy));
                        break;
                    }
                    case analysis::TIType::UINT64:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, int64PtrTy));
                        break;
                    }
                    default:
                        break;
                }

                x = builder.CreateGEP(ai, getConstant<int64_t>(i++));
                x = builder.CreateAlignedLoad(x, sizeof(uint64_t));
                args.emplace_back(x); // rows
                x = builder.CreateGEP(ai, getConstant<int64_t>(i++));
                x = builder.CreateAlignedLoad(x, sizeof(uint64_t));
                args.emplace_back(x); // cols
                x = builder.CreateGEP(ai, getConstant<int64_t>(i++));
                x = builder.CreateAlignedLoad(x, sizeof(uint64_t));
                args.emplace_back(x); // refc
            }
        }

        for (const auto & s : outSignature.getTypes())
        {
            llvm::Value * x = builder.CreateGEP(ai, getConstant<int64_t>(i++));
            x = builder.CreateAlignedLoad(x, sizeof(uint64_t));
            if (s.scalar)
            {
                switch (s.type)
                {
                    case analysis::TIType::DOUBLE:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, dblPtrTy));
                        break;
                    }
                    case analysis::TIType::COMPLEX:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, dblPtrTy));
                        x = builder.CreateGEP(ai, getConstant<int64_t>(i++));
                        x = builder.CreateAlignedLoad(x, sizeof(uint64_t));
                        args.emplace_back(builder.CreateIntToPtr(x, dblPtrTy));
                        break;
                    }
                    case analysis::TIType::BOOLEAN:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, int32PtrTy));
                        break;
                    }
                    case analysis::TIType::INT8:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, int8PtrTy));
                        break;
                    }
                    case analysis::TIType::INT16:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, int16PtrTy));
                        break;
                    }
                    case analysis::TIType::INT32:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, int32PtrTy));
                        break;
                    }
                    case analysis::TIType::INT64:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, int64PtrTy));
                        break;
                    }
                    case analysis::TIType::UINT8:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, int8PtrTy));
                        break;
                    }
                    case analysis::TIType::UINT16:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, int16PtrTy));
                        break;
                    }
                    case analysis::TIType::UINT32:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, int32PtrTy));
                        break;
                    }
                    case analysis::TIType::UINT64:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, int64PtrTy));
                        break;
                    }
                    default:
                        break;
                }
            }
            else
            {
                switch (s.type)
                {
                    case analysis::TIType::DOUBLE:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, llvm::PointerType::getUnqual(dblPtrTy)));
                        break;
                    }
                    case analysis::TIType::COMPLEX:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, llvm::PointerType::getUnqual(dblPtrTy)));
                        x = builder.CreateGEP(ai, getConstant<int64_t>(i++));
                        x = builder.CreateAlignedLoad(x, sizeof(uint64_t));
                        args.emplace_back(builder.CreateIntToPtr(x, llvm::PointerType::getUnqual(dblPtrTy)));
                        break;
                    }
                    case analysis::TIType::BOOLEAN:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, llvm::PointerType::getUnqual(int32PtrTy)));
                        break;
                    }
                    case analysis::TIType::INT8:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, llvm::PointerType::getUnqual(int8PtrTy)));
                        break;
                    }
                    case analysis::TIType::INT16:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, llvm::PointerType::getUnqual(int16PtrTy)));
                        break;
                    }
                    case analysis::TIType::INT32:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, llvm::PointerType::getUnqual(int32PtrTy)));
                        break;
                    }
                    case analysis::TIType::INT64:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, llvm::PointerType::getUnqual(int64PtrTy)));
                        break;
                    }
                    case analysis::TIType::UINT8:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, llvm::PointerType::getUnqual(int8PtrTy)));
                        break;
                    }
                    case analysis::TIType::UINT16:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, llvm::PointerType::getUnqual(int16PtrTy)));
                        break;
                    }
                    case analysis::TIType::UINT32:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, llvm::PointerType::getUnqual(int32PtrTy)));
                        break;
                    }
                    case analysis::TIType::UINT64:
                    {
                        args.emplace_back(builder.CreateIntToPtr(x, llvm::PointerType::getUnqual(int64PtrTy)));
                        break;
                    }
                    default:
                        break;
                }

                x = builder.CreateGEP(ai, getConstant<int64_t>(i++));
                x = builder.CreateAlignedLoad(x, sizeof(uint64_t));
                args.emplace_back(builder.CreateIntToPtr(x, int64PtrTy)); // rows
                x = builder.CreateGEP(ai, getConstant<int64_t>(i++));
                x = builder.CreateAlignedLoad(x, sizeof(uint64_t));
                args.emplace_back(builder.CreateIntToPtr(x, int64PtrTy)); // cols
                x = builder.CreateGEP(ai, getConstant<int64_t>(i++));
                x = builder.CreateAlignedLoad(x, sizeof(uint64_t));
                args.emplace_back(builder.CreateIntToPtr(x, int64PtrTy)); // refc
            }
        }

        builder.CreateCall(toCall, args);
        builder.CreateRetVoid();

        //M.dump();

        engine->generateCodeForModule(&M);
        info_it->second.setWrapper(reinterpret_cast<void *>(engine->getFunctionAddress(wrapper)));

        function->removeFromParent();
        delete function;
    }

    std::vector<uint64_t> args;

    for (auto pIT : in)
    {
        if (pIT->isGenericType())
        {
            types::GenericType * pGT = static_cast<types::GenericType *>(pIT);
            if (pGT->isScalar())
            {
                switch (pGT->getType())
                {
                    case types::InternalType::ScilabInt8:
                    {
                        args.emplace_back(Cast::bit_cast(static_cast<types::Int8 *>(pGT)->get(0)));
                        break;
                    }
                    case types::InternalType::ScilabUInt8:
                    {
                        args.emplace_back(Cast::bit_cast(static_cast<types::UInt8 *>(pGT)->get(0)));
                        break;
                    }
                    case types::InternalType::ScilabInt16:
                    {
                        args.emplace_back(Cast::bit_cast(static_cast<types::Int16 *>(pGT)->get(0)));
                        break;
                    }
                    case types::InternalType::ScilabUInt16:
                    {
                        args.emplace_back(Cast::bit_cast(static_cast<types::UInt16 *>(pGT)->get(0)));
                        break;
                    }
                    case types::InternalType::ScilabInt32:
                    {
                        args.emplace_back(Cast::bit_cast(static_cast<types::Int32 *>(pGT)->get(0)));
                        break;
                    }
                    case types::InternalType::ScilabUInt32:
                    {
                        args.emplace_back(Cast::bit_cast(static_cast<types::UInt32 *>(pGT)->get(0)));
                        break;
                    }
                    case types::InternalType::ScilabInt64:
                    {
                        args.emplace_back(Cast::bit_cast(static_cast<types::Int64 *>(pGT)->get(0)));
                        break;
                    }
                    case types::InternalType::ScilabUInt64:
                    {
                        args.emplace_back(Cast::bit_cast(static_cast<types::UInt64 *>(pGT)->get(0)));
                        break;
                    }
                    case types::InternalType::ScilabDouble:
                        if (pGT->isComplex())
                        {
                            args.emplace_back(Cast::bit_cast(static_cast<types::Double *>(pGT)->getReal()[0]));
                            args.emplace_back(Cast::bit_cast(static_cast<types::Double *>(pGT)->getImg()[0]));
                        }
                        else
                        {
                            args.emplace_back(Cast::bit_cast(static_cast<types::Double *>(pGT)->get(0)));
                        }
                        break;
                    case types::InternalType::ScilabBool:
                    {
                        args.emplace_back(Cast::bit_cast(static_cast<types::Bool *>(pGT)->get(0)));
                        break;
                    }
                    default:
                        break;
                }
            }
            else
            {
                switch (pGT->getType())
                {
                    case types::InternalType::ScilabInt8:
                    {
                        args.emplace_back(Cast::bit_cast(static_cast<types::Int8 *>(pGT)->get()));
                        break;
                    }
                    case types::InternalType::ScilabUInt8:
                    {
                        args.emplace_back(Cast::bit_cast(static_cast<types::UInt8 *>(pGT)->get()));
                        break;
                    }
                    case types::InternalType::ScilabInt16:
                    {
                        args.emplace_back(Cast::bit_cast(static_cast<types::Int16 *>(pGT)->get()));
                        break;
                    }
                    case types::InternalType::ScilabUInt16:
                    {
                        args.emplace_back(Cast::bit_cast(static_cast<types::UInt16 *>(pGT)->get()));
                        break;
                    }
                    case types::InternalType::ScilabInt32:
                    {
                        args.emplace_back(Cast::bit_cast(static_cast<types::Int32 *>(pGT)->get()));
                        break;
                    }
                    case types::InternalType::ScilabUInt32:
                    {
                        args.emplace_back(Cast::bit_cast(static_cast<types::UInt32 *>(pGT)->get()));
                        break;
                    }
                    case types::InternalType::ScilabInt64:
                    {
                        args.emplace_back(Cast::bit_cast(static_cast<types::Int64 *>(pGT)->get()));
                        break;
                    }
                    case types::InternalType::ScilabUInt64:
                    {
                        args.emplace_back(Cast::bit_cast(static_cast<types::UInt64 *>(pGT)->get()));
                        break;
                    }
                    case types::InternalType::ScilabDouble:
                        if (pGT->isComplex())
                        {
                            args.emplace_back(Cast::bit_cast(static_cast<types::Double *>(pGT)->get()));
                            args.emplace_back(Cast::bit_cast(static_cast<types::Double *>(pGT)->getImg()));
                        }
                        else
                        {
                            args.emplace_back(Cast::bit_cast(static_cast<types::Double *>(pGT)->get()));
                        }
                        break;
                    case types::InternalType::ScilabBool:
                    {
                        args.emplace_back(Cast::bit_cast(static_cast<types::Bool *>(pGT)->get()));
                        break;
                    }
                    default:
                        break;
                }
                int32_t _r = pGT->getRows();
                uint64_t r = Cast::bit_cast(_r);
                args.emplace_back(r);
                args.emplace_back(Cast::bit_cast(pGT->getCols()));
                args.emplace_back(Cast::bit_cast(pGT->getRef()));
            }
        }
    }

    std::vector<OutContainer> llvmOuts;
    llvmOuts.reserve(outSignature.getTypes().size());

    for (const auto s : outSignature.getTypes())
    {
        llvmOuts.emplace_back(s.type);
        OutContainer & oc = llvmOuts.back();
        if (s.scalar)
        {
            switch (s.type)
            {
                case analysis::TIType::DOUBLE:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.dbl));
                    break;
                }
                case analysis::TIType::COMPLEX:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.cpx[0]));
                    args.emplace_back(Cast::bit_cast(&oc.data.cpx[1]));
                    break;
                }
                case analysis::TIType::BOOLEAN:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.boolean));
                    break;
                }
                case analysis::TIType::INT8:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.i8));
                    break;
                }
                case analysis::TIType::INT16:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.i16));
                    break;
                }
                case analysis::TIType::INT32:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.i32));
                    break;
                }
                case analysis::TIType::INT64:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.i64));
                    break;
                }
                case analysis::TIType::UINT8:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.ui8));
                    break;
                }
                case analysis::TIType::UINT16:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.ui16));
                    break;
                }
                case analysis::TIType::UINT32:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.ui32));
                    break;
                }
                case analysis::TIType::UINT64:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.ui64));
                    break;
                }
                default:
                    break;
            }
            oc.rows = 1;
            oc.cols = 1;
        }
        else
        {
            switch (s.type)
            {
                case analysis::TIType::DOUBLE:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.ptr));
                    break;
                }
                case analysis::TIType::COMPLEX:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.cpx_ptr[0]));
                    args.emplace_back(Cast::bit_cast(&oc.data.cpx_ptr[1]));
                    break;
                }
                case analysis::TIType::BOOLEAN:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.ptr));
                    break;
                }
                case analysis::TIType::INT8:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.ptr));
                    break;
                }
                case analysis::TIType::INT16:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.ptr));
                    break;
                }
                case analysis::TIType::INT32:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.ptr));
                    break;
                }
                case analysis::TIType::INT64:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.ptr));
                    break;
                }
                case analysis::TIType::UINT8:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.ptr));
                    break;
                }
                case analysis::TIType::UINT16:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.ptr));
                    break;
                }
                case analysis::TIType::UINT32:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.ptr));
                    break;
                }
                case analysis::TIType::UINT64:
                {
                    args.emplace_back(Cast::bit_cast(&oc.data.ptr));
                    break;
                }
                default:
                    break;
            }

            args.emplace_back(Cast::bit_cast(&oc.rows));
            args.emplace_back(Cast::bit_cast(&oc.cols));
            args.emplace_back(Cast::bit_cast(&oc.refcount));
        }
    }


    //std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    reinterpret_cast<void (*)(uint64_t *)>(info_it->second.getWrapper())(&args[0]);
    //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //double duration = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-9;
    //std::wcerr << "Exec time=" << duration << " s." << std::endl;

    std::vector<OutContainer>::iterator i = llvmOuts.begin();
    for (const auto s : outSignature.getTypes())
    {
        types::InternalType * pIT = nullptr;
        OutContainer & oc = *(i++);

        if (s.scalar)
        {
            switch (s.type)
            {
                case analysis::TIType::DOUBLE:
                {
                    pIT = new types::Double(oc.data.dbl);
                    break;
                }
                case analysis::TIType::COMPLEX:
                {
                    pIT = new types::Double(oc.data.cpx[0], oc.data.cpx[1]);
                    break;
                }
                case analysis::TIType::BOOLEAN:
                {
                    pIT = new types::Bool(oc.data.boolean);
                    break;
                }
                case analysis::TIType::INT8:
                {
                    pIT = new types::Int8((char)oc.data.i8);
                    break;
                }
                case analysis::TIType::INT16:
                {
                    pIT = new types::Int16(oc.data.i16);
                    break;
                }
                case analysis::TIType::INT32:
                {
                    pIT = new types::Int32(oc.data.i32);
                    break;
                }
                case analysis::TIType::INT64:
                {
                    pIT = new types::Int64((long long)oc.data.i64);
                    break;
                }
                case analysis::TIType::UINT8:
                {
                    pIT = new types::UInt8(oc.data.ui8);
                    break;
                }
                case analysis::TIType::UINT16:
                {
                    pIT = new types::UInt16(oc.data.ui16);
                    break;
                }
                case analysis::TIType::UINT32:
                {
                    pIT = new types::UInt32(oc.data.ui32);
                    break;
                }
                case analysis::TIType::UINT64:
                {
                    pIT = new types::UInt64((unsigned long long)oc.data.ui64);
                    break;
                }
                default:
                    break;
            }
        }
        else
        {
            switch (s.type)
            {
                case analysis::TIType::DOUBLE:
                {
                    pIT = new types::Double(oc.rows, oc.cols, reinterpret_cast<double *>(oc.data.ptr));
                    break;
                }
                case analysis::TIType::COMPLEX:
                {
                    pIT = new types::Double(oc.rows, oc.cols, reinterpret_cast<double *>(oc.data.cpx_ptr[0]), reinterpret_cast<double *>(oc.data.cpx_ptr[1]));
                    break;
                }
                case analysis::TIType::BOOLEAN:
                {
                    pIT = new types::Bool(oc.rows, oc.cols, reinterpret_cast<int32_t *>(oc.data.ptr));
                    break;
                }
                case analysis::TIType::INT8:
                {
                    pIT = new types::Int8(oc.rows, oc.cols, reinterpret_cast<char *>(oc.data.ptr));
                    break;
                }
                case analysis::TIType::INT16:
                {
                    pIT = new types::Int16(oc.rows, oc.cols, reinterpret_cast<int16_t *>(oc.data.ptr));
                    break;
                }
                case analysis::TIType::INT32:
                {
                    pIT = new types::Int32(oc.rows, oc.cols, reinterpret_cast<int32_t *>(oc.data.ptr));
                    break;
                }
                case analysis::TIType::INT64:
                {
                    pIT = new types::Int64(oc.rows, oc.cols, reinterpret_cast<long long *>(oc.data.ptr));
                    break;
                }
                case analysis::TIType::UINT8:
                {
                    pIT = new types::UInt8(oc.rows, oc.cols, reinterpret_cast<uint8_t *>(oc.data.ptr));
                    break;
                }
                case analysis::TIType::UINT16:
                {
                    pIT = new types::UInt16(oc.rows, oc.cols, reinterpret_cast<uint16_t *>(oc.data.ptr));
                    break;
                }
                case analysis::TIType::UINT32:
                {
                    pIT = new types::UInt32(oc.rows, oc.cols, reinterpret_cast<uint32_t *>(oc.data.ptr));
                    break;
                }
                case analysis::TIType::UINT64:
                {
                    pIT = new types::UInt64(oc.rows, oc.cols, reinterpret_cast<unsigned long long *>(oc.data.ptr));
                    break;
                }
                default:
                    break;
            }
        }

        out.emplace_back(pIT);
    }
}
}
