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

#include "SUNDIALSManager.hxx"
#include "complexHelpers.hxx"

void SUNDIALSManager::computeMatrix(types::typed_list &in, functionKind what, SUNMatrix SUNMat_J)
{
    types::typed_list out;
    char errorMsg[256] = "";

    callClosing(what, in, {1}, out);

    // Double or sparse matrice expected
    if (out[0]->isDouble() || out[0]->isSparse())
    {
        if (out[0]->getAs<types::GenericType>()->getSize() != m_iSizeOfInput[what])
        {
            sprintf(errorMsg, _("%s: Wrong size for output argument #%d: expecting %d.\n"), m_pCallFunctionName[what], 1, m_iSizeOfInput[what]);
            throw ast::InternalError(errorMsg);
        }
    }
    else
    {
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Double or Sparse matrix expected.\n"), m_pCallFunctionName[what], 1);
        throw ast::InternalError(errorMsg);
    }

    if (SUNMat_J == NULL)
    {
        // call was a test to determine type of matrix
        m_typeOfOutput[what] = out[0]->getType();
        if (out[0]->isSparse())
        {
            m_iNonZeros[what] = out[0]->getAs<types::Sparse>()->nonZeros();
        }
    }
    else
    {
        copyMatrixToSUNMatrix(out[0], SUNMat_J, m_iNbEq, m_odeIsComplex);
    }

    out[0]->DecreaseRef();
    out[0]->killMe();
}

void SUNDIALSManager::computeFunction(types::typed_list &in, functionKind what, double *pdblOut, double *pdblOutExtra)
{
    types::typed_list out;
    types::Double* pDblOut[2] = {NULL,NULL};
    char errorMsg[256] = "";
    int iRetCount = pdblOutExtra != NULL ? 2 : 1;
    bool bOutputIsComplex = false;

    callClosing(what, in, {iRetCount}, out);

    // Double matrices expected

    for (int i=0; i < out.size(); i++)
    {
        if (out[i]->isDouble() == false)
        {
            sprintf(errorMsg, _("%s: Wrong type for output argument #%d: double expected.\n"), m_pCallFunctionName[what], i+1);
            throw ast::InternalError(errorMsg);
        }

        pDblOut[i] = out[i]->getAs<types::Double>();

        bOutputIsComplex |= pDblOut[i]->isComplex();
    
        if (m_iSizeOfInput[what] == -1)
        {
            m_iSizeOfInput[what] = pDblOut[i]->getSize();
        }
        else if (pDblOut[i]->getSize() != m_iSizeOfInput[what])
        {
            sprintf(errorMsg, _("%s: Wrong size for output argument #%d: expecting %d.\n"), m_pCallFunctionName[what], i+1, m_iSizeOfInput[what]);
            throw ast::InternalError(errorMsg);
        }
    }

    if (pdblOut == NULL) // pdblOut == NULL when testing if ode is complex or getting  m_iSizeOfInput[what]
    {
        // m_odeIsComplex can already be true because e.g. of initial condition
        m_odeIsComplex |= bOutputIsComplex;
    }
    else
    {
        if (bOutputIsComplex && !m_odeIsComplex)
        {
            sprintf(errorMsg, _("%s: Unexpected complex type output after initialization phase.\n"), m_pCallFunctionName[what]);
            throw ast::InternalError(errorMsg);            
        }
        copyRealImgToComplexVector(pDblOut[0]->get(), pDblOut[0]->getImg(), pdblOut, m_iSizeOfInput[what], m_odeIsComplex);
        if (out.size() == 2)
        {
            copyRealImgToComplexVector(pDblOut[1]->get(), pDblOut[1]->getImg(), pdblOutExtra, m_iSizeOfInput[what], m_odeIsComplex);
        }
    }

    for (int i=0; i < iRetCount; i++)
    {
        out[i]->DecreaseRef();
        out[i]->killMe();
    }
}

int SUNDIALSManager::colPackJac(sunrealtype t, sunrealtype c, N_Vector N_VectorY, N_Vector N_VectorYp, N_Vector N_VectorR,
                   SUNMatrix SUNMat_J, void *pManager, N_Vector tmp1, N_Vector tmp2, N_Vector tmp3)
{
    // compute Sparse Jacobian approximation using ColPack
    SUNDIALSManager *manager = static_cast<SUNDIALSManager *>(pManager);
    spCompJacobian *spJacEngine = manager->getColPackEngine();

    double **ppdblProd = spJacEngine->getProducts();

    for (int j=0; j < spJacEngine->getNbSeeds(); j++)
    {
        // Does Jac(Y)*seed[j] -> tmp1 using internal SUNDIALS engine
        manager->DQJtimes(t, N_VectorY, N_VectorYp, N_VectorR, manager->getNVectorSeeds()[j], tmp1, c, tmp2, tmp3);
        // copy tmp1 in ppdblProd ColPack array (array of pointers on lines)
        double *pdblTmp1 = N_VGetArrayPointer(tmp1);
        for (int i=0; i<manager->getNRealEq(); i++)
        {
           ppdblProd[i][j] =  pdblTmp1[i]; 
        }
    }

    // recover Jacobian from Jac(Y)*seed[j] products
    spJacEngine->recover();

    // copy template to output Jacobian SUNMat_J (only index terms matter here)
    SUNMatCopy(manager->getSUNMATPattern(),SUNMat_J);
    // copy nonzero terms
    spJacEngine->recoverMatrix(SM_DATA_S(SUNMat_J));
    // double *pdbl = SM_DATA_S(SUNMat_J);
    // for (int i=0; i<SM_NNZ_S(SUNMat_J); i++)
    // {
    //     sciprint("%g\n",pdbl[i]);
    // }

    return 0;                      
}

void SUNDIALSManager::callOpening(functionKind what, types::typed_list &in, double *pdblY)
{
    types::Double* pDblY = NULL;
    // keep native dimensions of Y0
    pDblY = m_pDblY0->clone();
    if (pdblY != NULL)
    {
        copyComplexVectorToDouble(pdblY, pDblY->get(), pDblY->getImg(), m_iNbEq, m_odeIsComplex);
    }
    in.push_back(pDblY);
}

void SUNDIALSManager::callClosing(functionKind what, types::typed_list &in, std::vector<int> iRetCount, types::typed_list &out)
{
    types::optional_list opt;
    char errorMsg[256] = "";

    if (m_pCallFunction[what] == NULL)
    {
        sprintf(errorMsg, "Error, m_pCallFunction[%d] is NULL !",(int)what);
        throw ast::InternalError(errorMsg);
    }

    // optional user input parameters
    for (auto pIn : m_pParameters[what])
    {
        in.push_back(pIn);
    }
    for (auto pIn : in)
    {
        pIn->IncreaseRef();
    }

    try
    {
        // new std::wstring(L"") is deleted in destructor of ast::CommentExp
        ConfigVariable::clearLastError();
        m_pCallFunction[what]->invoke(in, opt, iRetCount[iRetCount.size()-1], out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch(ast::InternalError& ie)
    {
        for (auto pIn :  m_pParameters[what])
        {
            pIn->DecreaseRef();
            pIn->killMe();
        }
        for (auto pOut :  out)
        {
            pOut->DecreaseRef();
            pOut->killMe();
        }
        if (ConfigVariable::getLastErrorFunction() == L"")
        {
            // necessary for errors such as "Wrong number of input arguments"
            std::wostringstream ostr;
            ostr << m_pCallFunctionName[what] << L": " <<  ie.GetErrorMessage();
            throw ast::InternalError(ostr.str());
        }
        else
        {
            char* pStr = wide_string_to_UTF8(ie.GetErrorMessage().c_str());
            sprintf(errorMsg, "%s", pStr);
            FREE(pStr);
            throw ast::InternalError(errorMsg);
        }
    }

    for (auto pOut : out) // safety IncreaseRef
    {
        pOut->IncreaseRef();
    }

    for (auto pIn : in)
    {
        pIn->DecreaseRef();
        pIn->killMe();
    }

    if (iRetCount.size() == 1)
    {
        if (out.size() != iRetCount[0])
        {

            for (auto pOut : out) // safety DecreaseRef
            {
                pOut->DecreaseRef();
                pOut->killMe();
            }

            sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), m_pCallFunctionName[what], iRetCount[0]);
            throw ast::InternalError(errorMsg);
        }
    }
    else if (iRetCount.size() == 2)
    {
        if (out.size() < iRetCount[0] || out.size() > iRetCount[1])
        {
            for (auto pOut : out) // safety DecreaseRef
            {
                pOut->DecreaseRef();
                pOut->killMe();
            }
            sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), m_pCallFunctionName[what], iRetCount[0],iRetCount[1]);
            throw ast::InternalError(errorMsg);
        }
    }
}

void SUNDIALSManager::parseFunction(types::InternalType* pIn, functionKind what, int iStart)
{
    char errorMsg[256];

    if (pIn->isCallable())
    {
        m_pCallFunction[what] = pIn->getAs<types::Callable>();
        m_functionAPI[what] = SCILAB_CALLABLE;
    }
    else if (pIn->isList())
    {
         types::List *pList = pIn->getAs<types::List>();
     
        types::InternalType *pIFirst = pList->getSize() > 0 ? pList->get(0) : NULL;
        if (pIFirst != NULL && pIFirst->isCallable())
        {
            m_pCallFunction[what] = pIFirst->getAs<types::Callable>();
            m_functionAPI[what] = SCILAB_CALLABLE;
            
            for (int i=1; i<pList->getSize(); i++)
            {
                // mandatory IncreaseRef !
                pList->get(i)->IncreaseRef();
                m_pParameters[what].push_back(pList->get(i));
            }
        }
        else if (pIFirst != NULL && pIFirst->isString())
        {
            types::String *pStr = pIFirst->getAs<types::String>();
            wchar_t* pwstr = pStr->get(0);
            if (m_staticFunctionMap.find(pwstr) != m_staticFunctionMap.end())
            {
                m_pEntryPointFunction[what] = m_staticFunctionMap[pwstr];
            }
            else
            {
                ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(pwstr);
                if (func == NULL)
                {
                    sprintf(errorMsg,_("%s: unable to find entry point %ls.\n"), getSolverName().c_str(), pwstr);
                    throw ast::InternalError(errorMsg);
                }
                m_pEntryPointFunction[what] = func->functionPtr;
            }
            if (pList->getSize() > 2)
            {
                sprintf(errorMsg,_("%s: only one parameter is allowed.\n"), getSolverName().c_str());
                throw ast::InternalError(errorMsg);
            }
            if (pList->get(1)->isDouble() == false || pList->get(1)->getAs<types::Double>()->isComplex())
            {
                sprintf(errorMsg,_("%s: parameter must be a real matrix.\n"), getSolverName().c_str());
                throw ast::InternalError(errorMsg);
            }
            // mandatory IncreaseRef !
            pList->get(1)->IncreaseRef();
            m_pParameters[what].push_back(pList->get(1));
            // api detection
            m_functionAPI[what] = SUNDIALS_DLL;
        }
        else
        {
            sprintf(errorMsg,_("%s: first element of parameter %s should be a function or a string.\n"), getSolverName().c_str(), pstrArgName[what]);
            throw ast::InternalError(errorMsg);
        }
    }
    else if (pIn->isString())
    {
        types::String *pStr = pIn->getAs<types::String>();
        wchar_t* pwstr = pStr->get(0);
        if (m_staticFunctionMap.find(pwstr) != m_staticFunctionMap.end())
        {
            m_pEntryPointFunction[what] = m_staticFunctionMap[pwstr];
        }
        else
        {
            ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(pwstr);
            if (func == NULL)
            {
                sprintf(errorMsg,_("%s: unable to find entry point %ls.\n"), getSolverName().c_str(), pwstr);
                throw ast::InternalError(errorMsg);
            }
            m_pEntryPointFunction[what] = func->functionPtr;
        }
        // api detection
        m_functionAPI[what] = SUNDIALS_DLL;
    }
    else if ((what == JACY || what == MASS) && (pIn->isDouble() || pIn->isSparse()))
    {
        if (pIn->getAs<types::GenericType>()->getSize() != m_iSizeOfInput[what])
        {
            sprintf(errorMsg, _("%s: parameter %s should be a Double or Sparse matrix of size %d.\n"), getSolverName().c_str(), pstrArgName[what], m_iSizeOfInput[what]);
            throw ast::InternalError(errorMsg);
        }
        pIn->IncreaseRef();
        m_pIConstFunction[what] = pIn;
        m_typeOfOutput[what] = pIn->getType();
        if (pIn->isSparse())
        {
            m_iNonZeros[what] = pIn->getAs<types::Sparse>()->nonZeros();
        }
        m_functionAPI[what] = CONSTANT;
    }
    else if (what == JACYYP && pIn->isCell())
    {
        types::Cell *pCell = pIn->getAs<types::Cell>();
        types::InternalType *pI0 = NULL;
        types::InternalType *pI1 = NULL;
        if (pCell->getSize() == 2)
        {
            pI0 = pCell->get(0);
            pI1 = pCell->get(1);
            if ((pI0->isDouble() && pI1->isDouble()) || (pI0->isSparse() && pI1->isSparse()))
            {
                if (pI0->getAs<types::GenericType>()->getSize() == m_iSizeOfInput[JACY]
                    && pI1->getAs<types::GenericType>()->getSize() == m_iSizeOfInput[JACY])
                {
                    m_pIConstFunction[JACY] = pI0;
                    m_pIConstFunction[JACYYP] = pI1;
                    m_typeOfOutput[JACY] = pI0->getType(); // Only this one will be tested
                    m_typeOfOutput[JACYYP] = pI0->getType(); // set by security
                }
                if (pI0->isSparse())
                {
                    int iNnz = std::max(pI0->getAs<types::Sparse>()->nonZeros(),pI1->getAs<types::Sparse>()->nonZeros());
                    m_iNonZeros[JACY]=iNnz;
                    m_iNonZeros[JACYYP]=iNnz;
                }
            }
            m_functionAPI[JACY] = CONSTANT;
            m_functionAPI[JACYYP] = CONSTANT;
        }
        if (m_pIConstFunction[JACY] == NULL || m_pIConstFunction[JACYYP] == NULL)
        {
            pCell->DecreaseRef();
            sprintf(errorMsg, _("%s: parameter %s should be a cell with two Double or Sparse matrices of size %d x %d.\n"), getSolverName().c_str(), pstrArgName[what], m_iNbEq, m_iNbEq);
            throw ast::InternalError(errorMsg);
        }
        pCell->IncreaseRef();
        pI0->IncreaseRef();
        pI1->IncreaseRef();
    }
    else
    {
        if (what == JACYYP)
        {
            sprintf(errorMsg, _("%s: parameter %s should be a matrix, a cell, a string, a function or a list.\n"), getSolverName().c_str(), pstrArgName[what]);
        }
        else if (iStart >= 0)
        {
            sprintf(errorMsg, _("%s: Wrong type for input argument %d: a string, a function or a list is expected.\n"), getSolverName().c_str(), iStart+1);
        }
        else
        {
            sprintf(errorMsg, _("%s: parameter %s should be a string, a function or a list.\n"), getSolverName().c_str(), pstrArgName[what]);            
        }
        throw ast::InternalError(errorMsg);
    }

    if (m_pCallFunction[what] != NULL)
    {
        m_pCallFunctionName[what] = wide_string_to_UTF8(m_pCallFunction[what]->getName().c_str());
    }
    
    // must be set only if no exception has occured (they can be catched in gateway)
    m_bHas[what] = true;
}

std::wstring SUNDIALSManager::setLinearSolver(functionKind what, N_Vector NV_work, SUNMatrix & SUN_A, SUNLinearSolver & SUN_LS)
{
    char errorMsg[256] = "";
    if (m_iVecBand[what].size()>0)
    {
        // Band solver, create band SUNMatrix for use in linear solves
        if (m_odeIsComplex)
        {
            // when ode is complex, upper and lower bandwidth are larger due to (re,im) interlacing
            SUN_A = SUNBandMatrix(m_iNbRealEq, 2*m_iVecBand[what][0]+1, 2*m_iVecBand[what][1]+1, m_sunctx);
        }
        else
        {
            SUN_A = SUNBandMatrix(m_iNbEq, m_iVecBand[what][0], m_iVecBand[what][1], m_sunctx);
        }
        // Create dense SUNLinSol_LapackBand object
        SUN_LS = SUNLinSol_LapackBand(NV_work, SUN_A, m_sunctx);
    }
    else if (m_iNonZeros[what] >= 0)
    {
        // Jacobian is sparse
        int iStorage = CSC_MAT;
        if (m_functionAPI[what] == SCILAB_CALLABLE
            || m_functionAPI[what] == CONSTANT
            ||  m_wstrSparseFormat[what] == L"CSR")
        {
            // Scilab uses Compressed Row Format (CSR)
            iStorage = CSR_MAT;
        }
        // convert sparsity pattern of user matrix to internal ColPack compressed row format:
        if (m_pIPattern[what] != NULL && m_pIPattern[what]->isSparse())
        {
            // set to compressed Row Format (CSR) because pattern is a Scilab Sparse matrix
            iStorage = CSR_MAT;

            // create ColPack Jacobian engine
            m_spJacEngine = new spCompJacobian(m_wstrSolver);
            m_spJacEngine->setPattern(m_pIPattern[what]);
            // do coloring of the graph
            if (m_spJacEngine->init() == false)
            {
                sprintf(errorMsg, _("%s: m_spJacEngine->init() failed\n"), getSolverName().c_str());
                throw ast::InternalError(errorMsg);
            }
            // copy seed vectors to N_Vector array, once for all
            m_N_Vector_seeds  = N_VCloneVectorArray(m_spJacEngine->getNbSeeds(), m_N_VectorY);
            for (int j=0; j < m_spJacEngine->getNbSeeds(); j++)
            {
                double *pdblSeed = N_VGetArrayPointer(m_N_Vector_seeds[j]);
                for (int i=0; i<getNRealEq();i++)
                {
                    pdblSeed[i] = m_spJacEngine->getSeeds()[i][j];
                }
            }
            // copy sparse pattern to SUNMatrix m_SUNMat_pattern template for later reuse in bridge function computing Jacobian
            if (m_odeIsComplex)
            {
                m_SUNMat_pattern = SUNSparseMatrix(m_iNbRealEq, m_iNbRealEq, 4*m_iNonZeros[what], iStorage, m_sunctx);
            }
            else
            {
                m_SUNMat_pattern = SUNSparseMatrix(m_iNbEq, m_iNbEq, m_iNonZeros[what], iStorage, m_sunctx);
            }
            // below call does a complimentary pattern transformation when ode is complex
            copyMatrixToSUNMatrix(m_pIPattern[what], m_SUNMat_pattern, m_iNbEq, m_odeIsComplex);
        }
        // Sparse KLU solver, create sparse SUNMatrix for use in linear solves
        if (m_odeIsComplex)
        {
            // when ode is complex, the number of nonzeros is multiplied by 4 because of (re,im) interlacing
            SUN_A = SUNSparseMatrix(m_iNbRealEq, m_iNbRealEq, 4*m_iNonZeros[what], iStorage, m_sunctx);
        }
        else
        {
            SUN_A = SUNSparseMatrix(m_iNbEq, m_iNbEq, m_iNonZeros[what], iStorage, m_sunctx);
        }
        // Create SUNLinearSolver object
        SUN_LS = SUNLinSol_KLU(NV_work, SUN_A, m_sunctx);
        return L"sparse";
    }
    else if (m_wstrLinSolver==L"DENSE" || m_wstrLinSolver==L"NONE")
    {
        // Default dense solver, create dense SUNMatrix for use in linear solves
        SUN_A = SUNDenseMatrix(m_iNbRealEq, m_iNbRealEq, m_sunctx);
        // Create dense SUNLinSol_LapackDense object
        SUN_LS = SUNLinSol_LapackDense(NV_work, SUN_A, m_sunctx);
    }
    else
    {
        // iterative linear solvers
        if (m_wstrLinSolver==L"CG")
        {
            //Preconditioned Conjugate Gradient (symmetric Jacobian is a must !)
            SUN_LS = SUNLinSol_PCG(NV_work, m_iPrecondType[m_wstrPrecondType], m_iLinSolMaxIters, m_sunctx);
        }
        else if (m_wstrLinSolver==L"BCGS")
        {
            SUN_LS = SUNLinSol_SPBCGS(NV_work, m_iPrecondType[m_wstrPrecondType], m_iLinSolMaxIters, m_sunctx);        
        }
        else if (m_wstrLinSolver==L"FGMR")
        {
            SUN_LS = SUNLinSol_SPFGMR(NV_work, m_iPrecondType[m_wstrPrecondType], m_iLinSolMaxIters, m_sunctx);                
        }
        else if (m_wstrLinSolver==L"GMR")
        {
            SUN_LS = SUNLinSol_SPGMR(NV_work, m_iPrecondType[m_wstrPrecondType], m_iLinSolMaxIters, m_sunctx);                 
        }
        else if (m_wstrLinSolver==L"TFQMR")
        {
            SUN_LS = SUNLinSol_SPTFQMR(NV_work, m_iPrecondType[m_wstrPrecondType], m_iLinSolMaxIters, m_sunctx);                         
        }        
    }
    
    return m_wstrLinSolver;
}

void SUNDIALSManager::parseMatrixPattern(types::optional_list &opt, const wchar_t * _pwstLabel, functionKind what)
{
    char errorMsg[256];
    int iNonZeros;
    types::InternalType *pI;
    // if (opt.find(_pwstLabel) == opt.end())
    // {
    //     if (m_odeIsExtension)
    //     {
    //         m_pIPattern[what] =  getPreviousManager()->m_pIPattern[what]; // can be NULL
    //     }
    //     return;
    // }
    if (opt.find(_pwstLabel) == opt.end())
    {
        return;
    }
    pI = opt[_pwstLabel];
    if (pI->isDouble())
    {
        types::Double *pDbl = pI->getAs<types::Double>();
        if (pDbl->getCols() == 2)
        {
            for (int i=0; i<pDbl->getRows(); i++)
            {
                int iRow = pDbl->get(i,0);
                int iCol = pDbl->get(i,1);
                if (iRow < 1 || iCol > m_iNbEq || iCol < 1 || iCol > m_iNbEq)
                {
                    sprintf(errorMsg,_("%s: invalid value in option %ls at row %d.\n"), getSolverName().c_str(), _pwstLabel, i+1);
                    throw ast::InternalError(errorMsg);
                }
            }
            iNonZeros = pDbl->getRows();
        }
        else
        {
            sprintf(errorMsg, _("%s: Wrong size for option \"%ls\": a Double matrix with %d columns is expected.\n"), getSolverName().c_str(), _pwstLabel, 2);
            throw ast::InternalError(errorMsg);
        }
    }
    else if (pI->isSparse())
    {
        types::Sparse *pSp = pI->getAs<types::Sparse>();
        if (pSp->getRows() != m_iNbEq || pSp->getCols() != m_iNbEq)
        {
            sprintf(errorMsg, _("%s: Wrong size for option \"%ls\": a %d x %d Sparse matrix is expected.\n"), getSolverName().c_str(), _pwstLabel, m_iNbEq, m_iNbEq);
            throw ast::InternalError(errorMsg);
        }
        iNonZeros = pSp->nonZeros();
    }
    else
    {
        sprintf(errorMsg, _("%s: Wrong type for option \"%ls\": a Double or Sparse matrix is expected.\n"), getSolverName().c_str(), _pwstLabel);
        throw ast::InternalError(errorMsg);
    }
    m_pIPattern[what] = pI;
    m_pIPattern[what]->IncreaseRef();
    m_wstrSparseFormat[what] = L"CSR";
    m_iNonZeros[what] = iNonZeros;
    opt.erase(_pwstLabel);
}

int SUNDIALSManager::DQJtimes(sunrealtype tt, N_Vector yy, N_Vector yp, N_Vector rr,
                  N_Vector v, N_Vector Jv, sunrealtype c_j,
                  N_Vector work1, N_Vector work2)
{
    return 1;
}
