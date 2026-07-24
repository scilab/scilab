//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021-2022 - UTC - Stéphane MOTTELET
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// For more information, see the COPYING file which you should have received
//
//--------------------------------------------------------------------------

#ifndef _IDAMANAGER_HXX_
#define _IDAMANAGER_HXX_

#include "dynlib_differential_equations.h"

#include "OdeManager.hxx"

#include <idas/idas_impl.h>
#include <idas/idas_ls_impl.h>
#include <idas/idas.h>            /* prototypes for IDA fcts. and consts. */

extern "C"
{
    void SUN_chemres(int *n, double *t, double y[], double yd[], double r[]);
    void SUN_chemjac(int *n, double *t, double *cj, double y[], double yd[], double r[], double j[]);
    void SUN_chemevent(int *n,double *t,double *y, double *yd, int *ng, double *g, int *term, int *dir);
    int SUN_chemcb(int *n,double *t,double *y, double *yd, int *flag);
}

class DIFFERENTIAL_EQUATIONS_IMPEXP IDAManager final : public OdeManager
{
    public :

    IDAManager()
    {
        m_strSolver = "ida";
        m_wstrSolver = L"ida";
        m_staticFunctionMap[L"SUN_chemres"] = (dynlibFunPtr)SUN_chemres;
        m_staticFunctionMap[L"SUN_chemjac"] = (dynlibFunPtr)SUN_chemjac;
        m_staticFunctionMap[L"SUN_chemevent"] = (dynlibFunPtr)SUN_chemevent;
        m_staticFunctionMap[L"SUN_chemcb"] = (dynlibFunPtr)SUN_chemcb;
 
        m_defaultFunctionKind = RES;
 
        setUserData = IDASetUserData;
        setInitStep = IDASetInitStep;
        setMinStep = IDASetMinStep;
        setMaxStep = IDASetMaxStep;
        setMaxNumSteps = IDASetMaxNumSteps;
        setStopTime = IDASetStopTime;
        setMaxOrd = IDASetMaxOrd;
        getCurrentTime = IDAGetCurrentTime;
        getCurrentStep = IDAGetCurrentStep;
        getLastStep = IDAGetLastStep;
        getRootInfo = IDAGetRootInfo;
        setConstraints = IDASetConstraints;
        setVTolerances = IDASVtolerances;
        setQuadSVTolerances = IDAQuadSVtolerances;
        setQuadErrCon = IDASetQuadErrCon;
        getReturnFlagName = IDAGetReturnFlagName;
        getDky = IDAGetDky;

        toODEReturn.emplace(IDA_SUCCESS, ODE_SUCCESS);
        toODEReturn.emplace(IDA_TSTOP_RETURN, ODE_TSTOP_RETURN);
        toODEReturn.emplace(IDA_ROOT_RETURN, ODE_ROOT_RETURN);
        toODEReturn.emplace(IDA_CONV_FAIL, ODE_CONV_FAILURE);
        toODEReturn.emplace(IDA_TOO_MUCH_WORK, ODE_TOO_MUCH_WORK);
        toODEReturn.emplace(IDA_REP_RES_ERR, ODE_REP_RES_ERR);
        toODEReturn.emplace(IDA_WARNING, ODE_WARNING);

        fromODEReturn.emplace(ODE_SUCCESS, IDA_SUCCESS);
        fromODEReturn.emplace(ODE_TSTOP_RETURN, IDA_TSTOP_RETURN );
        fromODEReturn.emplace(ODE_ROOT_RETURN, IDA_ROOT_RETURN);
        fromODEReturn.emplace(ODE_CONV_FAILURE, IDA_CONV_FAIL);
        fromODEReturn.emplace(ODE_TOO_MUCH_WORK, IDA_TOO_MUCH_WORK);
        fromODEReturn.emplace(ODE_REP_RES_ERR, IDA_REP_RES_ERR);
        fromODEReturn.emplace(ODE_WARNING, IDA_WARNING);
     }

    ~IDAManager() {
        if (m_prob_mem != NULL)
        {
            if (m_prob_mem != NULL) IDAFree(&m_prob_mem);
            m_prob_mem = NULL;
        }
        if (m_NVArrayYS != NULL)
        {
            for (int i=0; i<getNbSensPar(); i++)
            {
                N_VDestroy(m_NVArrayYS[i]);
                N_VDestroy(m_NVArrayYpS[i]);
            }
            //N_VDestroyVectorArray_Serial(m_NVArrayYS, getNbSensPar());
            m_NVArrayYS = NULL;
        }
        if (m_NVectorYQ != NULL)
        {
            N_VDestroy(m_NVectorYQ);
        }
        SUNDIALSMANAGER_KILLME(m_pDblSensPar);
        SUNDIALSMANAGER_KILLME(m_pDblYS0);
        SUNDIALSMANAGER_KILLME(m_pDblYpS0);
        SUNDIALSMANAGER_KILLME(m_pDblYQ0);
    }

    void setPreviousManager(void *p)
    {
        m_prevManager = static_cast<IDAManager *>(p);
        m_odeIsExtension = true;
    }

    OdeManager *getPreviousManager()
    {
        return m_prevManager;
    }
    
    virtual bool isDAE()
    {
        return true;
    }

    types::Double *getYpOut()
    {
        // solution y' at user prescribed timesteps or at each internal step of the method
        return getArrayFromVectors(m_pDblY0, m_vecYpOut, m_dblVecTOut.size());
    }

    types::Double *getYpEvent()
    {
        // yp at time of event
        return getArrayFromVectors(m_pDblY0, m_vecYpEvent, m_dblVecEventTime.size());
    }

    types::Double *getYSOut()
    {
        // sensitivity of y at user prescribed timesteps or at each internal step of the method
        return getArrayFromVectors(m_pDblYS0, m_vecYSOut, m_dblVecTOut.size());
    }

    types::Double *getYpSOut()
    {
        // sensitivity of y' at user prescribed timesteps or at each internal step of the method
        return getArrayFromVectors(m_pDblYpS0, m_vecYpSOut, m_dblVecTOut.size());
    }

    types::Double *getYQOut()
    {
        // pure quadrature variable at user prescribed timesteps or at each internal step of the method
        return getArrayFromVectors(m_pDblYQ0, m_vecYQOut, m_dblVecTOut.size());
    }

    types::Double *getYSEvent()
    {
        // sensitivity of y at time of event
        return getArrayFromVectors(m_pDblYS0, m_vecYSEvent, m_dblVecEventTime.size());
    }

    types::Double *getYpSEvent()
    {
        // sensitivity of y' at time of event
        return getArrayFromVectors(m_pDblYpS0, m_vecYpSEvent, m_dblVecEventTime.size());
    }


    std::vector<std::pair<std::wstring,types::Double *>> getAdditionalFields();
    std::vector<std::pair<std::wstring,types::Double *>> getAdditionalEventFields();
    void saveAdditionalStates();
    void saveAdditionalStates(double dblTime);
    void saveAdditionalEventStates(double dblTime);

    bool setMaxOrder();
    bool computeIC(char *errorMsg);
    std::wstring getDefaultNonLinSolver();
    std::vector<std::wstring> getAvailableNonLinSolvers();
    int getMaxNargin()
    {
        return 4;
    }
    int getMinNargin()
    {
        return 4;
    }
    void parseMethodAndOrder(types::optional_list &opt);
    bool create();
    bool initialize(char *errorMsg);
    // void errHandler(int error_code, const char *module, const char *function, char *msg);
    bool setEventFunction();
    bool setSolverAndJacobian(char *errorMsg);
    int getInterpBasisSize();
    solverReturnCode doStep(double dblFinalTime, double *pdblTime, solverTaskCode iKind);
    void saveInterpBasisVectors();
    void getInterpVectors(double *pdblNS, int iOrderPlusOne, int iIndex, double dblt0, double dblTUser, double dblStep, double *pdblVect, double *pdblVectd);

    // static methods
    static int colPackJac(sunrealtype t, sunrealtype c, N_Vector N_VectorY, N_Vector N_VectorYp, N_Vector N_VectorR, SUNMatrix SUNMat_J, void *pManager, 
        N_Vector tmp1, N_Vector tmp2, N_Vector tmp3);

    types::Struct *getStats();

    int DQJtimes(sunrealtype tt, N_Vector yy, N_Vector yp, N_Vector rr,
                  N_Vector v, N_Vector Jv, sunrealtype c_j,
                  N_Vector work1, N_Vector work2) final;

    // static methods
    static int sensRes(int Ns, sunrealtype t, N_Vector N_VectorY, N_Vector N_VectorYp, N_Vector resval, N_Vector *yS, N_Vector *ySdot, N_Vector *resvalS,
        void *pManager, N_Vector tmp1, N_Vector tmp2, N_Vector tmp3);
    static int quadratureRhs(sunrealtype t, N_Vector N_VectorY, N_Vector N_VectorYdot, N_Vector N_VectorYQdot, void *pManager);


    std::vector<std::wstring> getAvailableMethods()
    {
        std::vector<std::wstring> availableMethods = {L"BDF"};
        return availableMethods;
    }

    int getMaxMethodOrder(std::wstring wstrMethod)
    {
        return 5;
    }

    bool hasQuadFeature()
    {
        return true;
    }

    bool hasSensFeature()
    {
        return true;
    }

    bool computeSens()
    {
        return m_pDblSensPar != NULL;
    }

    int getNbSensPar()
    {
        return m_pDblSensPar == NULL ? 0 : (m_iVecSensParIndex.size()==0 ? m_pDblSensPar->getSize() : m_iVecSensParIndex.size());
    }

    std::vector<std::wstring> getAvailablePrecondType()
    {
        std::vector<std::wstring> available = {L"NONE",L"LEFT"};
        return available;
    }

    std::wstring getDefaultPrecondType()
    {
        if  (m_iPrecBand.size() > 0)
        {
            return L"LEFT";
        }
        else
        {
            return L"NONE";            
        }
    }

    private :

    IDAManager* m_prevManager = NULL;

    std::vector<std::vector<double>> m_vecYpOut;
    std::vector<std::vector<double>> m_vecYpEvent;

    N_Vector *m_NVArrayYS = NULL;
    N_Vector *m_NVArrayYpS = NULL;

    std::vector<std::vector<double>> m_vecYQOut;

    std::vector<std::vector<double>> m_vecYSOut;
    std::vector<std::vector<double>> m_vecYSEvent;
    std::vector<std::vector<double>> m_vecYpSOut;
    std::vector<std::vector<double>> m_vecYpSEvent;

    long int m_incStat[9] = {0,0,0,0,0,0,0,0,0};
};

#endif

