//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021-2022 - UTC - Stéphane MOTTELET
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// For more information, see the COPYING file which you should have received
//
//--------------------------------------------------------------------------

#ifndef _ODEMANAGER_HXX_
#define _ODEMANAGER_HXX_

#include "dynlib_differential_equations.h"

#include "SUNDIALSManager.hxx"
#include "SUNDIALSBridge.hxx"

class DIFFERENTIAL_EQUATIONS_IMPEXP OdeManager : public SUNDIALSManager
{
    public :

    OdeManager()
    {
        m_iSizeOfInput[EVENTS] = -1;
        m_dblOptT0 = NAN;
        m_dblInitialStep = NAN;
        m_iNbEvents = 0;
    }

    virtual ~OdeManager() {
        if (m_N_VectorYp != NULL) N_VDestroy(m_N_VectorYp);
        if (m_N_VectorYTemp != NULL) N_VDestroy(m_N_VectorYTemp);
        if (m_MASS != NULL) SUNMatDestroy(m_MASS);
        if (m_MASS_LS != NULL) SUNLinSolFree(m_MASS_LS);
        if (m_TempSUNMat != NULL) SUNMatDestroy(m_TempSUNMat);
        SUNDIALSMANAGER_KILLME(m_pDblInterpBasisVectors);
        SUNDIALSMANAGER_KILLME(m_pDblTSpan);
    };

    enum solverTaskCode {ODE_NORMAL, ODE_ONE_STEP};
    enum solverReturnCode {ODE_SUCCESS=0, ODE_TSTOP_RETURN, ODE_ROOT_RETURN,
        ODE_CONV_FAILURE, ODE_TOO_MUCH_WORK, ODE_REPTD_RHSFUNC_ERR, ODE_REP_RES_ERR, ODE_WARNING};

    std::map<int, solverReturnCode> toODEReturn;
    std::map<solverReturnCode,int> fromODEReturn;

    types::Double *getTOut()
    {
        // time at the begining of each internal step of the method
        types::Double *pDblTOut = new types::Double(1,m_dblVecTOut.size());
        std::copy(m_dblVecTOut.begin(), m_dblVecTOut.end(), pDblTOut->get());
        return pDblTOut;
    }

    types::Double *getYOut()
    {
        // solution y at user prescribed timesteps or at each internal step of the method
        return getArrayFromVectors(m_pDblY0, m_vecYOut, m_dblVecTOut.size());
    }

    types::Double *getIndexEvent()
    {
        if (m_dblVecEventTime.size() == 0)
        {
           return types::Double::Empty(); 
        }
        else
        {
            types::Double *pDbl = new types::Double(m_iNbEvents, m_dblVecIndexEvent.size());
            for (auto i = 0; i < m_dblVecIndexEvent.size(); i++)
            {
                std::copy(m_dblVecIndexEvent[i].begin(), m_dblVecIndexEvent[i].end(), pDbl->get() + i*m_iNbEvents);
            }
            return pDbl;
        }
    }

    types::Double *getTEvent()
    {
        if (m_dblVecEventTime.size() == 0)
        {
           return types::Double::Empty(); 
        }
        else
        {
            types::Double *pDbl = new types::Double(1,m_dblVecEventTime.size());
            std::copy(m_dblVecEventTime.begin(), m_dblVecEventTime.end(), pDbl->get());
            return pDbl;
        }         
    }

    types::Double *getYEvent()
    {
        if (m_dblVecEventTime.size() == 0)
        {
           return types::Double::Empty(); 
        }
        else
        {
        return getArrayFromVectors(m_pDblY0, m_dblVecYEvent,m_dblVecEventTime.size());
        }
    }

    std::vector<double> getStepVector()
    {
        return m_dblVecStep;
    }
    std::vector<double> getCurrTimeVector()
    {
        return m_dblVecCurrTime;
    }

    int getNbEvents()
    {
        return m_iNbEvents;
    }

    bool getErrProj()
    {
        return m_bErrProj;
    }

    types::Double *getInterpBasis()
    {
        return m_pDblInterpBasisVectors;
    }

    std::vector<int> getInterpBasisIndex()
    {
        return m_indexInterpBasis;
    }
 
    SUNMatrix getTempSunMatrix()
    {
        return m_TempSUNMat;
    }
 
    int (*setUserData)(void *,void *);
    int (*setInitStep)(void *, sunrealtype);
    int (*setMinStep)(void *, sunrealtype);
    int (*setMaxStep)(void *, sunrealtype);
    int (*setMaxNumSteps)(void *, long int);
    int (*setStopTime)(void *, sunrealtype);
    int (*setMaxOrd)(void *, int);
    int (*getCurrentTime)(void *, double *);
    int (*getCurrentStep)(void *, double *);
    int (*getRootInfo)(void *, int *);
    int (*setConstraints)(void *, N_Vector);
    int (*setVTolerances)(void *, sunrealtype, N_Vector);
    int (*setQuadSVTolerances)(void *, sunrealtype, N_Vector);
    int (*setQuadErrCon)(void *, int);
    int (*getDky)(void *, sunrealtype, int, N_Vector);
    int (*getSens)(void *, sunrealtype *, N_Vector *);
    int (*getSensDky)(void *, sunrealtype, int, N_Vector *);
    int (*getLastStep)(void *, double *);
    char *(*getReturnFlagName)(long int);

    // SUNDIALSManager methods implemented in OdeManager 
    void callOpening(functionKind what, types::typed_list &in, double t, double *pdblY = NULL, double *pdblYp = NULL);
    void parseFunctionFromOption(types::optional_list &opt, const wchar_t * _pwstLabel, functionKind what);
    void parseMatrices(types::typed_list &in);
    void parseOptions(types::optional_list &opt);
    void init();
    void solve();
    types::Double *parseInitialCondition(types::typed_list &in, int iStart);

    // New OdeManager specific methods
    int intermediateCallback(sunrealtype t, int iFlag, N_Vector N_VectorY, N_Vector N_VectorYp);
    void setupEvents(types::optional_list &opt);
    void createSolutionOutput(types::typed_list &out);
    types::Double *createYOut(types::Double *pDblTemplate, int iNbOut, int iSizeTSpan, bool bFlat = false);
    types::Double *getArrayFromVectors(types::Double *pDblTemplate, std::vector<std::vector<double>> &m_vecY, size_t iTSpanSize);
    void solverErrHandler(int error_code, const char *msg);

    // static methods
    static int function_t_Y1_Y2(functionKind what, sunrealtype t, N_Vector N_Vector1, N_Vector N_Vector2, void *pManager);
    static int function_t_Y1_Y2_Y3(functionKind what, sunrealtype t, N_Vector N_Vector1, N_Vector N_Vector2, N_Vector N_Vector3, void *pManager);
    static int eventFunction(sunrealtype t, N_Vector N_VectorY, sunrealtype *pdblOut, void *pManager);
    static int eventFunctionImpl(sunrealtype t, N_Vector N_VectorY, N_Vector N_VectorYp, sunrealtype *pdblOut, void *pManager);
    static int rhsFunction(sunrealtype t, N_Vector N_VectorY, N_Vector N_VectorYDot, void *pManager);
    static int rhsFunctionStiff(sunrealtype t, N_Vector N_VectorY, N_Vector N_VectorYDot, void *pManager);
    static int jacFunction(sunrealtype t, N_Vector N_VectorY, N_Vector N_VectorFy, SUNMatrix SUNMat_J, void *pManager, 
        N_Vector tmp1, N_Vector tmp2, N_Vector tmp3);
    static int resFunction(sunrealtype t, N_Vector N_VectorY, N_Vector N_VectorYp, N_Vector N_VectorRes, void *pManager);
    static int jacResFunction(sunrealtype t, sunrealtype c, N_Vector N_VectorY, N_Vector N_VectorYp, N_Vector N_VectorR,
        SUNMatrix SUNMat_J, void *pManager, N_Vector tmp1, N_Vector tmp2, N_Vector tmp3);
    static int massFunction(sunrealtype t, SUNMatrix SUNMat_M, void *pManager, N_Vector tmp1, N_Vector tmp2, N_Vector tmp3);
    static int colPackJac(sunrealtype t, N_Vector N_VectorY, N_Vector N_VectorYp, SUNMatrix SUNMat_J, void *pManager,
         N_Vector tmp1, N_Vector tmp2, N_Vector tmp3);
    static void errHandler(int line, const char *func, const char *file, const char *msg, SUNErrCode err_code, void *err_user_data, SUNContext sunctx);

    // virtual methods
    virtual int getMaxNargin()
    {
        return 3;
    }
    virtual int getMinNargin()
    {
        return 3;
    }
    virtual std::wstring getMethodName()
    {
        return m_wstrMethod;
    }
    virtual std::wstring getInterpolationMethod()
    {
        return L"native";
    }
    virtual bool computeIC(char *)
    {
        return false;
    }
    virtual std::vector<std::pair<std::wstring,types::Double *>> getAdditionalFields()
    {
        return {};
    }
    virtual std::vector<std::pair<std::wstring,types::Double *>> getAdditionalEventFields()
    {
        return {};
    }
    virtual void saveAdditionalStates()
    {}
    virtual void saveAdditionalStates(double dblTime)
    {}
    virtual void saveAdditionalEventStates(double dblTime)
    {}
    virtual int getNbSensPar()
    {
        return 0;
    }
    virtual bool hasSensFeature()
    {
        return false;
    }
    virtual bool hasQuadFeature()
    {
        return false;
    }

    virtual int getBasisDimensionAtIndex(int iIndex);
    virtual double *getBasisAtIndex(int iIndex);

    // pure OdeManager virtual methods
    virtual void setPreviousManager(void *) = 0;
    virtual OdeManager *getPreviousManager() = 0;
    virtual void parseMethodAndOrder(types::optional_list &opt) = 0;
    virtual bool initialize(char *) = 0;
    virtual bool setEventFunction() = 0;
    virtual bool setSolverAndJacobian(char *errorMsg) = 0;
    virtual int getInterpBasisSize() = 0;
    virtual solverReturnCode doStep(double dblFinalTime, double *pdblTime, solverTaskCode iKind) = 0;
    virtual void saveInterpBasisVectors() = 0;
    virtual std::vector<std::wstring> getAvailableMethods() = 0;
    virtual int getMaxMethodOrder(std::wstring wstrMethod) = 0;
    virtual void getInterpVectors(double *pdblNS, int iOrderPlusOne, int iIndex, double dblt0, double dblTUser, 
                                  double dblStep, double *pdblVect, double *pdblVectd) = 0;

    protected :
 
    std::wstring m_wstrMethod;
  
    N_Vector m_N_VectorYp = NULL;
    N_Vector m_N_VectorYTemp = NULL;
    N_Vector m_NVectorYQ = NULL;

    SUNMatrix m_MASS = NULL;
    SUNMatrix m_TempSUNMat = NULL;
    SUNLinearSolver m_MASS_LS = NULL;
    SUNNonlinearSolver m_NLSsens = NULL;
    types::Double *m_pDblTSpan = NULL;
    types::Double *m_pDblYp0 = NULL;
    types::Double *m_pDblTOut = NULL;
    types::Double *m_pDblYpOut = NULL;
    types::Double *m_pDblSensPar = NULL;
    types::Double *m_pDblYS0 = NULL;
    types::Double *m_pDblYpS0 = NULL;
    types::Double *m_pDblYQ0 = NULL;

    std::vector<double> m_dblVecTypicalPar;
    std::vector<double> m_dblVecQuadAtol;
    std::vector<double> m_dblVecQuadRtol;

    std::vector<double> m_dblVecStep;
    std::vector<double> m_dblVecCurrTime;
    std::vector<int> m_iVecOrder;
    std::vector<int> m_iVecInterpBasisSize;
    std::vector<std::vector<double>> m_vecYOut;
 
    std::vector<int> m_iVecEventIsTerminal;
    std::vector<int> m_iVecEventDirection;
    std::vector<int> m_iVecIsAlgebraic;

    std::vector<int> m_iVecSensParIndex;

    std::vector<std::vector<double>> m_dblVecYEvent;
    std::vector<std::vector<int>> m_dblVecIndexEvent;
    std::vector<double> m_dblVecEventTime;

    std::wstring m_wstrCalcIc;
    std::wstring m_wstrSensCorrStep;
    // continuous formula vectors
    // Nordsieck vectors for CVODE, Phi/Psi vectors for IDA
    std::vector<int> m_indexInterpBasis;
    std::list<std::vector<double>> interpBasisVectorList;
    types::Double *m_pDblInterpBasisVectors = NULL;
 
    bool m_bSuppressAlg = false;
    bool m_odeIsExtension = false;
    bool m_bErrProj = false;
    bool m_bStabLimDet = false;
    bool m_bSensErrCon = false;
    bool m_bQuadErrCon = false;

    double m_dblT0;
    double m_dblOptT0;
    double m_dblCurrentStep;
    double m_dblInitialStep;
    double m_dblMaxStep;
    double m_dblMinStep;
    double m_dblQuadRtol;

    int m_iMaxOrder;
    int m_iLastOrder;
    int m_iMaxNumSteps;
    int m_iNbEvents;
    int m_iNRefine;
    int m_iNbQuad;
    int m_iNbRealQuad;
 
    void *m_prob_mem = NULL;
};

#endif
