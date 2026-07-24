/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2021 - 2023 - UTC - Stéphane MOTTELET
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "OdeManager.hxx"
#include "complexHelpers.hxx"

// Static helpers

int OdeManager::function_t_Y1_Y2(functionKind what, sunrealtype t, N_Vector N_Vector1, N_Vector N_Vector2, void *pManager)
{
    OdeManager *manager = static_cast<OdeManager *>(pManager);
    functionAPI fAPI = manager->getFunctionAPI(what);
    double *pdbl = N_VGetArrayPointer(N_Vector2);

    if (fAPI == SCILAB_CALLABLE)
    {
        types::typed_list in;
        manager->callOpening(what, in, t, N_VGetArrayPointer(N_Vector1));
        manager->computeFunction(in, what, N_VGetArrayPointer(N_Vector2));
    }
    else if (fAPI == SUNDIALS_DLL)
    {
        dynlibFunPtr pFunc = manager->getEntryPointFunction(what);
        return ((SUN_DynFun)pFunc)(t, N_Vector1, N_Vector2, (void *)manager->getPdblSinglePar(what));
    }
    for (int k = 0; k < N_VGetLength(N_Vector2); k++, pdbl++)
    {
        if (!std::isfinite(*pdbl))
        {
            // return a SUNDIALS recoverable error
            return 1;
        }
    }
    return 0;
}

int OdeManager::function_t_Y1_Y2_Y3(functionKind what, sunrealtype t, N_Vector N_Vector1, N_Vector N_Vector2, N_Vector N_Vector3, void *pManager)
{
    OdeManager *manager = static_cast<OdeManager *>(pManager);
    functionAPI fAPI = manager->getFunctionAPI(what);
    double *pdbl = N_VGetArrayPointer(N_Vector3);

    if (fAPI == SCILAB_CALLABLE)
    {
        types::typed_list in;
        manager->callOpening(what, in, t, N_VGetArrayPointer(N_Vector1), N_VGetArrayPointer(N_Vector2));
        manager->computeFunction(in, what, N_VGetArrayPointer(N_Vector3));
    }
    else if (fAPI == SUNDIALS_DLL)
    {
        dynlibFunPtr pFunc = manager->getEntryPointFunction(what);
        return ((SUN_DynRes)pFunc)(t, N_Vector1, N_Vector2, N_Vector3, manager->getPdblSinglePar(what));
    }
    for (int k = 0; k < N_VGetLength(N_Vector3); k++, pdbl++)
    {
        if (!std::isfinite(*pdbl))
        {
            // return a SUNDIALS recoverable error
            return 1;
        }
    }
    return 0;
}

void OdeManager::errHandler(int line, const char *func, const char *file, const char *msg, SUNErrCode code, void *pManager, SUNContext sunctx)
{
    OdeManager *manager = static_cast<OdeManager *>(pManager);
    manager->solverErrHandler(code, msg);
}

int OdeManager::rhsFunction(sunrealtype t, N_Vector N_VectorY, N_Vector N_VectorYDot, void *pManager)
{
    return function_t_Y1_Y2(RHS, t, N_VectorY, N_VectorYDot, pManager);
}

int OdeManager::rhsFunctionStiff(sunrealtype t, N_Vector N_VectorY, N_Vector N_VectorYDot, void *pManager)
{
    return function_t_Y1_Y2(SRHS, t, N_VectorY, N_VectorYDot, pManager);
}

int OdeManager::resFunction(sunrealtype t, N_Vector N_VectorY, N_Vector N_VectorYp, N_Vector N_VectorRes, void *pManager)
{
    return function_t_Y1_Y2_Y3(RES, t, N_VectorY, N_VectorYp, N_VectorRes, pManager);
}

int OdeManager::eventFunction(sunrealtype t, N_Vector N_VectorY, sunrealtype *pdblOut, void *pManager)
{
    OdeManager *manager = static_cast<OdeManager *>(pManager);
    functionKind what = EVENTS;
    functionAPI fAPI = manager->getFunctionAPI(what);

    if (fAPI == SCILAB_CALLABLE)
    {
        types::typed_list in;
        manager->callOpening(what, in, t, N_VGetArrayPointer(N_VectorY));
        manager->computeFunction(in, what, pdblOut);
    }
    else if (fAPI == SUNDIALS_DLL)
    {
        dynlibFunPtr pFunc = manager->getEntryPointFunction(what);
        return  ((SUN_DynEvent)pFunc)(t, N_VectorY, pdblOut, manager->getPdblSinglePar(what));
    }
    return 0;
}

int OdeManager::eventFunctionImpl(sunrealtype t, N_Vector N_VectorY, N_Vector N_VectorYp, sunrealtype *pdblOut, void *pManager)
{
    OdeManager *manager = static_cast<OdeManager *>(pManager);
    functionKind what = EVENTS;
    functionAPI fAPI = manager->getFunctionAPI(what);

    if (fAPI == SCILAB_CALLABLE)
    {
        types::typed_list in;
        manager->callOpening(what, in, t, N_VGetArrayPointer(N_VectorY), N_VGetArrayPointer(N_VectorYp));
        manager->computeFunction(in, what, pdblOut);
    }
    else  if (fAPI == SUNDIALS_DLL)
    {
        dynlibFunPtr pFunc = manager->getEntryPointFunction(what);
        return  ((SUN_DynImplEvent)pFunc)(t, N_VectorY, N_VectorYp, pdblOut, manager->getPdblSinglePar(what));
    }
    return 0;
}

int OdeManager::jacFunction(sunrealtype t, N_Vector N_VectorY, N_Vector N_VectorFy, SUNMatrix SUNMat_J, void *pManager, N_Vector tmp1, N_Vector tmp2, N_Vector tmp3)
{
    OdeManager *manager = static_cast<OdeManager *>(pManager);
    functionKind what = JACY;
    functionAPI fAPI = manager->getFunctionAPI(what);

    if (fAPI == SCILAB_CALLABLE)
    {
        types::typed_list in;
        manager->callOpening(what, in, t, N_VGetArrayPointer(N_VectorY));
        manager->computeMatrix(in, what, SUNMat_J);        
    }
    else if (fAPI == SUNDIALS_DLL)
    {
        dynlibFunPtr pFunc = manager->getEntryPointFunction(what);
        return ((SUN_DynJacFun)pFunc)(t, N_VectorY, N_VectorFy, SUNMat_J, manager->getPdblSinglePar(what), tmp1, tmp2, tmp3);
    }
    else if (fAPI == CONSTANT)
    {
        copyMatrixToSUNMatrix(manager->getConstantFunction(what), SUNMat_J, manager->getNEq(), manager->isComplex());
    }
    return 0;
}


int OdeManager::jacResFunction(sunrealtype t, sunrealtype c, N_Vector N_VectorY, N_Vector N_VectorYp, N_Vector N_VectorR,
                   SUNMatrix SUNMat_J, void *pManager, N_Vector tmp1, N_Vector tmp2, N_Vector tmp3)
{
    OdeManager *manager = static_cast<OdeManager *>(pManager);
    functionKind what = JACYYP;
    functionAPI fAPI = manager->getFunctionAPI(what);
    
    if (fAPI == SCILAB_CALLABLE)
    {
        types::typed_list in;
        manager->callOpening(what, in, t, N_VGetArrayPointer(N_VectorY), N_VGetArrayPointer(N_VectorYp));
        in.push_back(new types::Double(c));
        manager->computeMatrix(in, what, SUNMat_J);
    }
    else if (fAPI == SUNDIALS_DLL)
    {
        dynlibFunPtr pFunc = manager->getEntryPointFunction(what);
        return ((SUN_DynJacRes)pFunc)(t, c, N_VectorY, N_VectorYp, N_VectorR, SUNMat_J, manager->getPdblSinglePar(what), tmp1, tmp2, tmp3);
    }
    else if (fAPI == CONSTANT)
    {
        types::InternalType *pIJYYP = manager->getConstantFunction(what);
        // manager->getTempSUNMatrix() is supposed to contain pIJY converted to SUNMatrix
        // below we set SUNMat_J to dR/dy + c*dR/dyp
        copyMatrixToSUNMatrix(pIJYYP, SUNMat_J, manager->getNEq(), manager->isComplex());
        SUNMatScaleAdd(c, SUNMat_J, manager->getTempSunMatrix());        
    }
    return 0;
}

int OdeManager::massFunction(sunrealtype t, SUNMatrix SUNMat_M, void *pManager, N_Vector tmp1, N_Vector tmp2, N_Vector tmp3)
{
    OdeManager *manager = static_cast<OdeManager *>(pManager);
    functionKind what = MASS;
    functionAPI fAPI = manager->getFunctionAPI(what);

    if (fAPI == SCILAB_CALLABLE)
    {
        types::typed_list in;
        in.push_back(new types::Double(t));
        manager->computeMatrix(in, what, SUNMat_M);
    }
    else if (fAPI == SUNDIALS_DLL)
    {
        dynlibFunPtr pFunc = manager->getEntryPointFunction(what);
        return ((SUN_DynMass)pFunc)(t, SUNMat_M, manager->getPdblSinglePar(what), tmp1, tmp2, tmp3);
    }
    else if (fAPI == CONSTANT)
    {
        copyMatrixToSUNMatrix(manager->getConstantFunction(what), SUNMat_M, manager->getNEq(), manager->isComplex());
    }
    return 0;
}

int OdeManager::colPackJac(sunrealtype t, N_Vector N_VectorY, N_Vector N_VectorYp, SUNMatrix SUNMat_J, void *pManager, N_Vector tmp1, N_Vector tmp2, N_Vector tmp3)
{
    return SUNDIALSManager::colPackJac(t, 0, N_VectorY, N_VectorYp, NULL, SUNMat_J, pManager, tmp1, tmp2, tmp3);
}

// Dynamic helpers

int OdeManager::intermediateCallback(sunrealtype t, int iFlag, N_Vector N_VectorY, N_Vector N_VectorYp)
{
    functionKind what = INTCB;
    functionAPI fAPI = getFunctionAPI(what);
    char errorMsg[256];

    if (fAPI == SCILAB_CALLABLE)
    {
        types::typed_list in;
        types::typed_list out;
        bool bTerm;
     
        callOpening(what, in, t, N_VGetArrayPointer(N_VectorY), isDAE() ? N_VGetArrayPointer(N_VectorYp) : NULL);
        in.push_back(new types::String(wstrCbState[iFlag].c_str()));
        in.push_back(getStats());        
        callClosing(what, in, {1}, out);
        // Scalar boolean expected
        if (out[0]->isBool() == false || out[0]->getAs<types::Bool>()->getSize() != 1)
        {
            sprintf(errorMsg, _("%s: Wrong type for output argument #%d: scalar boolean expected.\n"), getFunctionName(what), 1);
            throw ast::InternalError(errorMsg);
        }
        bTerm = out[0]->getAs<types::Bool>()->get(0);
        out[0]->DecreaseRef();
        out[0]->killMe();

        return (int) bTerm;
    }
    else if (fAPI == SUNDIALS_DLL)
    {
        dynlibFunPtr pFunc = getEntryPointFunction(what);
        if (isDAE())
        {
            return ((SUN_DynImplCallBack)pFunc)(t, iFlag, N_VectorY, N_VectorYp, getPdblSinglePar(what));
        }
        else
        {
            return ((SUN_DynCallBack)pFunc)(t, iFlag, N_VectorY, getPdblSinglePar(what));     
        }
    }
    return 0;
}

types::Double *OdeManager::createYOut(types::Double *m_pDblYtempl, int iNbOut, int iSizeTSpan, bool bFlat)
{
    types::Double *pDblYOut;
    int iDimsYtempl = m_pDblYtempl->getDims();

    if (bFlat == false && m_pDblYtempl->getCols() > 1)
    {
        // create a new arrray of native dimensions of m_pDblYtempl + one extra dimension for time
        int *piDimsArrayYtempl = m_pDblYtempl->getDimsArray();
        int *piDimsArrayYOut = new int[iDimsYtempl+1];
        for (int i=0; i<iDimsYtempl; i++)
        {
            piDimsArrayYOut[i] = piDimsArrayYtempl[i];
        }
        piDimsArrayYOut[iDimsYtempl] = iSizeTSpan;
        pDblYOut = new types::Double(iDimsYtempl+1, piDimsArrayYOut, m_odeIsComplex);
        delete[] piDimsArrayYOut;
    }
    else if (m_pDblYtempl->isVector() && bFlat == false)
    {
        pDblYOut = new types::Double(m_pDblYtempl->getSize(), iSizeTSpan, m_odeIsComplex);        
    }
    else
    {
        pDblYOut = new types::Double(iNbOut, iSizeTSpan, m_odeIsComplex);
    }
    return pDblYOut;
}

void OdeManager::createSolutionOutput(types::typed_list &out)
{
    // return a MList of "_odeSolution" type with method solution at internal steps
    // and a pointer to the OdeManager object. Extraction on this MList calls %_odeSolution_e and allows
    // to compute solution at arbitrary value of time by using method dense interpolant.

    auto addFields = getAdditionalFields();
    auto addEventFields = getAdditionalEventFields();
    int iNbEventFields = m_iNbEvents > 0 ? 3+addEventFields.size() : 0; 
    int iNbFields = 9 + addFields.size() + iNbEventFields + (m_iRetCount == 1 ? 3 : 0);
    int k = 0;

    types::MList *pObj = new types::MList();
    types::String *pStr = new types::String(1,iNbFields);
    
    pStr->set(k++,L"_odeSolution");
    pStr->set(k++,L"solver");
    pStr->set(k++,L"method");
    pStr->set(k++,L"interpolation");
    pStr->set(k++,L"linearSolver");
    pStr->set(k++,L"nonLinearSolver");
    pStr->set(k++,L"rtol");
    pStr->set(k++,L"atol");

    if (m_iRetCount == 1)
    {
        pStr->set(k++,L"t");
        pStr->set(k++,L"y");
    }

    for (auto it : addFields)
    {
        pStr->set(k++,it.first.c_str());            
    }
    if (m_iNbEvents > 0)
    {
        pStr->set(k++,L"te");
        pStr->set(k++,L"ye");
        for (auto it : addEventFields)
        {
            pStr->set(k++,it.first.c_str());            
        }
        pStr->set(k++,L"ie");
    }
    
    if (m_iRetCount == 1)
    {
        pStr->set(k++,L"manager");
    }

    pStr->set(k++,L"stats");

    k = 0;
    pObj->set(k++,pStr);
    pObj->set(k++,new types::String(getSolverName().c_str()));
    pObj->set(k++,new types::String(getMethodName().c_str()));
    pObj->set(k++,new types::String(getInterpolationMethod().c_str()));
    pObj->set(k++,new types::String(m_wstrLinSolver.c_str()));
    pObj->set(k++,new types::String(m_wstrNonLinSolver.c_str()));
    pObj->set(k++,new types::Double(m_dblRtol));
    pObj->set(k++,getATol());
    
    if (m_iRetCount == 1)
    {
        pObj->set(k++,getTOut());
        pObj->set(k++,getYOut());
    }

    for (auto it : addFields)
    {
        pObj->set(k++,it.second);            
    }
    if (m_iNbEvents > 0)
    {
        pObj->set(k++, getTEvent());
        pObj->set(k++, getYEvent());
        for (auto it : addEventFields)
        {
            pObj->set(k++,it.second);            
        }
        pObj->set(k++, getIndexEvent());
    }
    if (m_iRetCount == 1)
    {
        pObj->set(k++,new types::Pointer((void *)this));
    }

    pObj->set(k++, getStats());

    out.push_back(pObj);
}

types::Double *OdeManager::getArrayFromVectors(types::Double *m_pDblYtempl, std::vector<std::vector<double>> &m_vecY, size_t iTSpanSize)
{
    types::Double *pDblY = createYOut(m_pDblYtempl, m_iNbEq, (int) iTSpanSize);
    int iSize = m_vecY[0].size();
    for (auto i = 0; i < m_vecY.size(); i++)
    {
        copyComplexVectorToRealImg(m_vecY[i].data(), pDblY, i, iSize);
    }
    return pDblY;
}

int OdeManager::getBasisDimensionAtIndex(int iIndex)
{
    return m_indexInterpBasis[iIndex] -  m_indexInterpBasis[iIndex - 1];        
}

double *OdeManager::getBasisAtIndex(int iIndex)
{
    return m_pDblInterpBasisVectors->get() + m_indexInterpBasis[iIndex-1]*m_pDblInterpBasisVectors->getRows();
}




















