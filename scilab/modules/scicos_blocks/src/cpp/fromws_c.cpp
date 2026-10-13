/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
 *  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#include <cmath>
#include <cstring>

#include "Helpers.hxx"

#include "context.hxx"
#include "types.hxx"
#include "double.hxx"
#include "int.hxx"

extern "C"
{
#include "dynlib_scicos_blocks.h"
#include "expandPathVariable.h"
#include "scicos_block4.h"
#include "scicos_evalhermite.h"
#include "scicos.h"
#include "sci_malloc.h"

#include "localization.h"
#include "charEncoding.h"

    SCICOS_BLOCKS_IMPEXP void fromws_c(scicos_block* block, int flag);
}

/*--------------------------------------------------------------------------*/
/* work struct for a block */
typedef struct
{
    int nPoints;
    int Hmat;
    int cnt1;
    int cnt2;
    int EVindex;
    int PerEVcnt;
    int firstevent;
    double* D;
    void* work;
    double* workt;
} fromwork_struct;
/*--------------------------------------------------------------------------*/
static int Mytridiagldltsolve(double* &dA, double* &lA, double* &B, int N)
{
    for (int j = 1; j <= N - 1; ++j)
    {
        double Temp = lA[j - 1];
        lA[j - 1] /= dA[j - 1];
        B[j] -= lA[j - 1] * B[j - 1];
        dA[j] -= Temp * lA[j - 1];
    }

    B[N - 1] /= dA[N - 1];
    for (int j = N - 2; j >= 0; --j)
    {
        B[j] = - lA[j] * B[j + 1] + B[j] / dA[j];
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_xcos_block;

/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void fromws_c(scicos_block* block, int flag)
{
    /* Retrieve dimensions of output port */
    int my    = GetOutPortRows(block, 1); /* Number of rows of the output */
    int ny    = GetOutPortCols(block, 1); /* Number of cols of the output */
    int ytype = GetOutType(block, 1);     /* Output type */

    /* Generic pointers */
    double *y_d, *y_cd, *ptr_d = nullptr, *ptr_T, *ptr_D;
    char *y_c, *ptr_c;
    unsigned char *y_uc, *ptr_uc;
    short int *y_s, *ptr_s;
    unsigned short int *y_us, *ptr_us;
    int *y_l, *ptr_l;
    unsigned int *y_ul, *ptr_ul;

    /* The struct pointer of the block */
    fromwork_struct** work = (fromwork_struct**) block->work;
    fromwork_struct* ptr = nullptr;

    int Fnlength = block->ipar[0];
    int Method   = block->ipar[1 + Fnlength];
    int ZC       = block->ipar[2 + Fnlength];
    int OutEnd   = block->ipar[3 + Fnlength];

    switch (flag)
    {
        case 4 :
        {
            /* Init */

            /*
             * the variable name is :
             *  - length prefix
             *  - utf8 string stored as int32 values (might not be 0-terminated)
             */
            int len = block->ipar[0];
            char* utf8_varname = (char*) MALLOC((len + 1) * sizeof(char));
            for (int i = 0; i < len; ++i)
            {
                utf8_varname[i] = static_cast<char>(block->ipar[1 + i]);
            }
            utf8_varname[len] = '\0';
            std::wstring FName(to_wide_string(utf8_varname));
            FREE(utf8_varname);
            
            auto* pIT = symbol::Context::getInstance()->get(symbol::Symbol(FName));
            if (pIT == nullptr)
            {
                Coserror(_("The '%s' variable does not exist.\n"), FName.c_str());
                return;
            }

            if (!pIT->isGenericType())
            {
                Coserror(_("The '%s' variable does not have fields.\n"), FName.c_str());
                return;
            }
            auto* pGT = pIT->getAs<types::GenericType>();

            types::InternalType* pITTime = nullptr;
            if(!pGT->extract(L"time", pITTime))
            {
                Coserror(_("The '%s.time' field does not exist.\n"), FName.c_str());
                return;
            }
            types::InternalType* pITValues = nullptr;
            if (!pGT->extract(L"values", pITValues))
            {
                Coserror(_("The '%s.values' field does not exist.\n"), FName.c_str());
                return;
            }

            if(!pITTime->isDouble())
            {
                Coserror(_("The '%s.time' field should be of double type.\n"));
                return;
            }
            auto* pdblTime = pITTime->getAs<types::Double>();
            if (pdblTime->isComplex())
            {
                Coserror(_("The '%s.time' field should not be complex.\n"));
                return;
            }
            int nPoints = pdblTime->getSize(); 

            if((ytype == SCSREAL_N || ytype == SCSCOMPLEX_N) && !pITValues->isDouble())
            {
                Coserror(_("The '%s.values' field should have double type.\n"));
                return;
            }
            else if (ytype == SCSINT8_N && !pITValues->isInt8())
            {
                Coserror(_("The '%s.values' field should have int8 type.\n"));
                return;
            }
            else if (ytype == SCSINT16_N && !pITValues->isInt16())
            {
                Coserror(_("The '%s.values' field should have int16 type.\n"));
                return;
            }
            else if (ytype == SCSINT32_N && !pITValues->isInt32())
            {
                Coserror(_("The '%s.values' field should have int32 type.\n"));
                return;
            }
            else if (ytype == SCSUINT8_N && !pITValues->isUInt8())
            {
                Coserror(_("The '%s.values' field should have uint8 type.\n"));
                return;
            }
            else if (ytype == SCSUINT16_N && !pITValues->isUInt16())
            {
                Coserror(_("The '%s.values' field should have uint16 type.\n"));
                return;
            }
            else if (ytype == SCSUINT32_N && !pITValues->isUInt32())
            {
                Coserror(_("The '%s.values' field should have uint32 type.\n"));
                return;
            }

            auto* pGTValues = pITValues->getAs<types::GenericType>();
            int* dims = pGTValues->getDimsArray();
            if (nPoints != dims[0])
            {
                Coserror(_("The '%s.time' and '%s.values' fields does not have the same first dimension (resp. %d and %d length).\n"), nPoints, dims[0]);
                return;
            }
            int mX = dims[1];
            int nX = 1;
            if (pGTValues->getDims() > 2)
            {
                nX = dims[2];
            }
            if (my != mX || ny != nX)
            {
                Coserror(_("Data dimensions are inconsistent:\n Variable size=[%d,%d] \n Block output size=[%d,%d].\n"), mX, nX, my, ny);
                return;
            }

            /* Allocation of the work structure of that block */

            *work = new fromwork_struct();
            ptr = *work;
            ptr->D = nullptr;
            ptr->workt = nullptr;
            ptr->work = nullptr;

            if (ytype == SCSREAL_N)
            {
                /* Real */
                types::Double* pdblValues = pGTValues->getAs<types::Double>();

                ptr->work = MALLOC((nPoints + 1) * mX * nX * sizeof(double));
                ptr_d = (double*) ptr->work;
                double* pDataReal = pdblValues->getReal();
                for (size_t j = 0; j < size_t(nPoints) * size_t(mX) * size_t(nX); ++j)
                    ptr_d[j] = pDataReal[j];
                ptr_d[nPoints * mX * nX] = 0;
            }
            else if (ytype == SCSCOMPLEX_N)
            {
                /* Complex */
                auto* pdblValues = pGTValues->getAs<types::Double>();
                
                ptr->work = MALLOC((nPoints + 1) * mX * nX * 2 * sizeof(double));
                ptr_d = (double*) ptr->work;
                std::memcpy(ptr_d, pdblValues->getReal(), nPoints * mX * nX * sizeof(double));
                ptr_d[nPoints * mX * nX] = 0;
                if (pdblValues->isComplex())
                {
                    std::memcpy(ptr_d + nPoints * mX * nX + 1, pdblValues->getImg(), nPoints * mX * nX * sizeof(double));
                }
                else
                {
                    std::memset(ptr_d + nPoints * mX * nX + 1, 0, nPoints * mX * nX * sizeof(double));
                }
                ptr_d[nPoints * mX * nX * 2 + 1] = 0;
            }
            else if (ytype == SCSINT8_N)
            {
                /* int8 case */
                ptr->work = CALLOC((nPoints + 1) * mX * nX, sizeof(char));
                ptr_c = (char*)ptr->work;
                std::memcpy(ptr_c, pGTValues->getAs<types::Int8>(), nPoints * mX * nX * sizeof(char));
                ptr_c[nPoints * mX * nX] = 0;
            }
            else if (ytype == SCSINT16_N)
            {
                /* int16 case */
                ptr->work = CALLOC((nPoints + 1) * mX * nX, sizeof(short int));
                ptr_s = (short int*) ptr->work;
                std::memcpy(ptr_s, pGTValues->getAs<types::Int16>(), nPoints * mX * nX * sizeof(short int));
                ptr_s[nPoints * mX * nX] = 0;
            }
            else if (ytype == SCSINT32_N)
            {
                /* int32 case */
                ptr->work = CALLOC((nPoints + 1) * mX * nX, sizeof(int));
                ptr_l = (int*) ptr->work;
                std::memcpy(ptr_l, pGTValues->getAs<types::Int32>(), nPoints * mX * nX * sizeof(int));
                ptr_l[nPoints * mX * nX] = 0;
            }
            else if (ytype == SCSUINT8_N)
            {
                /* uint8 case */
                ptr->work = CALLOC((nPoints + 1) * mX * nX, sizeof(unsigned char));
                ptr_uc = (unsigned char*) ptr->work;
                std::memcpy(ptr_uc, pGTValues->getAs<types::UInt8>(), nPoints * mX * nX * sizeof(unsigned char));
                ptr_uc[nPoints * mX * nX] = 0;
            }
            else if (ytype == SCSUINT16_N)
            {
                /* uint16 case */
                ptr->work = CALLOC((nPoints + 1) * mX * nX, sizeof(unsigned short int));
                ptr_us = (unsigned short int*) ptr->work;
                std::memcpy(ptr_us, pGTValues->getAs<types::UInt16>(), nPoints * mX * nX * sizeof(unsigned short int));
                ptr_us[nPoints * mX * nX] = 0;
            }
            else if (ytype == SCSUINT32_N)
            {
                /* uint32 case */
                ptr->work = CALLOC((nPoints + 1) * mX * nX, sizeof(unsigned int));
                ptr_ul = (unsigned int*) ptr->work;
                std::memcpy(ptr_ul, pGTValues->getAs<types::UInt32>(), nPoints * mX * nX * sizeof(unsigned short int));
                ptr_ul[nPoints * mX * nX] = 0;
            }

            /* Check Hmat */
            if (nX > 1)
            {
                ptr->Hmat = 1;
            }
            else
            {
                ptr->Hmat = 0;
            }

            ptr->workt = new double[nPoints + 1];
            ptr_T = (double*) ptr->workt;
            double* pTimeReal = pITTime->getAs<types::Double>()->getReal();
            for (size_t j = 0; j < size_t(nPoints); ++j)
                ptr_T[j] = pTimeReal[j];
            ptr_T[nPoints] = 0;

            /*================================*/
            /* Check for an increasing time data */
            for (int j = 0; j < nPoints - 1; ++j)
            {
                if (ptr_T[j] > ptr_T[j + 1])
                {
                    Coserror(_("The time vector should be an increasing vector.\n"));
                    *work = nullptr;
                    delete[] ptr->workt;
                    FREE(ptr->work);
                    delete ptr;
                    return;
                }
            }
            /*=================================*/
            if ((Method > 1) && (ytype == SCSREAL_N || ytype == SCSCOMPLEX_N) && (!ptr->Hmat))
            {
                /* double or complex */
                if (ytype == SCSREAL_N) /* real */
                {
                    ptr->D = new double[nPoints * mX + 1];
                }
                else /* complex */
                {
                    ptr->D = new double[2 * nPoints * mX + 1];
                }

                double* spline = new double[3 * nPoints - 2];

                double* A_d  = spline;
                double* A_sd = A_d  + nPoints;
                double* qdy  = A_sd + nPoints - 1;

                for (int j = 0; j < mX; ++j)
                {
                    /* real part */
                    for (int i = 0; i <= nPoints - 2; ++i)
                    {
                        A_sd[i] = 1 / (ptr_T[i + 1] - ptr_T[i]);
                        qdy[i]  = (ptr_d[i + 1 + j * nPoints] - ptr_d[i + j * nPoints]) * A_sd[i] * A_sd[i];
                    }

                    for (int i = 1; i <= nPoints - 2; ++i)
                    {
                        A_d[i] = 2 * (A_sd[i - 1] + A_sd[i]);
                        ptr->D[i + j * nPoints] = 3 * (qdy[i - 1] + qdy[i]);
                    }

                    if (Method == 2)
                    {
                        A_d[0] =  2 * A_sd[0];
                        ptr->D[0 + j * nPoints] = 3 * qdy[0];
                        A_d[nPoints - 1] =  2 * A_sd[nPoints - 2];
                        ptr->D[nPoints - 1 + j * nPoints] =  3 * qdy[nPoints - 2];
                        ptr->D[nPoints + j * nPoints] =  ptr->D[nPoints - 1 + j * nPoints];
                        double* res = &ptr->D[j * nPoints];
                        Mytridiagldltsolve(A_d, A_sd, res, nPoints);
                    }

                    if (Method == 3)
                    {
                        /*  s'''(x(2)-) = s'''(x(2)+) */
                        double r = A_sd[1] / A_sd[0];
                        A_d[0] = A_sd[0] / (1 + r);
                        ptr->D[j * nPoints] = ((3 * r + 2) * qdy[0] + r * qdy[1]) / ((1 + r) * (1 + r));
                        /*  s'''(x(n-1)-) = s'''(x(n-1)+) */
                        r = A_sd[nPoints - 3] / A_sd[nPoints - 2];
                        A_d[nPoints - 1] = A_sd[nPoints - 2] / (1 + r);
                        ptr->D[nPoints - 1 + j * nPoints] = ((3 * r + 2) * qdy[nPoints - 2] + r * qdy[nPoints - 3]) / ((1 + r) * (1 + r));
                        ptr->D[nPoints + j * nPoints] =  ptr->D[nPoints - 1 + j * nPoints];
                        double* res = &ptr->D[j * nPoints];
                        Mytridiagldltsolve(A_d, A_sd, res, nPoints);
                    }
                }

                if (ytype == SCSCOMPLEX_N)
                {
                    /* imag part */
                    for (int j = 0; j < mX; ++j)
                    {
                        for (int i = 0; i <= nPoints - 2; ++i)
                        {
                            A_sd[i] = 1 / (ptr_T[i + 1] - ptr_T[i]);
                            qdy[i]  = (ptr_d[nPoints + i + 1 + j * nPoints] - ptr_d[nPoints + i + j * nPoints]) * A_sd[i] * A_sd[i];
                        }

                        for (int i = 1; i <= nPoints - 2; ++i)
                        {
                            A_d[i] = 2 * (A_sd[i - 1] + A_sd[i]);
                            ptr->D[i + j * nPoints + nPoints] = 3 * (qdy[i - 1] + qdy[i]);
                        }

                        if (Method == 2)
                        {
                            A_d[0] =  2 * A_sd[0];
                            ptr->D[nPoints + 0 + j * nPoints] = 3 * qdy[0];
                            A_d[nPoints - 1] =  2 * A_sd[nPoints - 2];
                            ptr->D[nPoints + nPoints - 1 + j * nPoints] =  3 * qdy[nPoints - 2];
                            ptr->D[nPoints + nPoints + j * nPoints] =  ptr->D[nPoints + nPoints - 1 + j * nPoints];
                            double* res = &ptr->D[nPoints + j * nPoints];
                            Mytridiagldltsolve(A_d, A_sd, res, nPoints);
                        }

                        if (Method == 3)
                        {
                            /*  s'''(x(2)-) = s'''(x(2)+) */
                            double r = A_sd[1] / A_sd[0];
                            A_d[0] = A_sd[0] / (1 + r);
                            ptr->D[nPoints + j * nPoints] = ((3 * r + 2) * qdy[0] + r * qdy[1]) / ((1 + r) * (1 + r));
                            /*  s'''(x(n-1)-) = s'''(x(n-1)+) */
                            r = A_sd[nPoints - 3] / A_sd[nPoints - 2];
                            A_d[nPoints - 1] = A_sd[nPoints - 2] / (1 + r);
                            ptr->D[nPoints + nPoints - 1 + j * nPoints] = ((3 * r + 2) * qdy[nPoints - 2] + r * qdy[nPoints - 3]) / ((1 + r) * (1 + r));
                            ptr->D[nPoints + nPoints + j * nPoints] =  ptr->D[nPoints + nPoints - 1 + j * nPoints];
                            double* res = &ptr->D[nPoints + j * nPoints];
                            Mytridiagldltsolve(A_d, A_sd, res, nPoints);
                        }
                    }
                }

                delete[] spline;
            }
            /*===================================*/
            int cnt1 = nPoints - 1;
            int cnt2 = nPoints;
            for (int i = 0; i < nPoints; ++i)
            {
                /* finding the first positive time instant */
                if (ptr->workt[i] >= 0)
                {
                    cnt1 = i - 1;
                    cnt2 = i;
                    break;
                }
            }
            ptr->nPoints = nPoints;
            ptr->cnt1 = cnt1;
            ptr->cnt2 = cnt2;
            ptr->EVindex = 0;
            ptr->PerEVcnt = 0;
            ptr->firstevent = 1;
            break;
            /*******************************************************/
            /*******************************************************/
        }
        case 1 :
        {
            /* Output computation */

            /* Retrieve 'ptr' of the structure of the block */
            ptr = *work;
            int nPoints = ptr->nPoints;
            int cnt1 = ptr->cnt1;
            int cnt2 = ptr->cnt2;
            int EVindex = ptr->EVindex;
            int PerEVcnt = ptr->PerEVcnt;

            /* Get current simulation time */
            double t = get_scicos_time();
            double t1 = t, t2;

            double TNm1  = ptr->workt[nPoints - 1];
            double TP    = TNm1 - 0;

            int inow;
            if (ZC == 1)
            {
                /* Zero-crossing enabled */
                if (OutEnd == 2)
                {
                    if (PerEVcnt > 0)
                    {
                        // We ran out of value and OutEnd is 2 (Repeat)
                        // Use fake time within our range.
                        t -= (PerEVcnt) * TP;
                    }
                    inow = nPoints - 1;
                }
                else
                {
                    inow = nPoints + 1; // Arbitrary value more than nPoints, will be overwritten if needed.
                }
                for (int i = cnt1 ; i < nPoints; ++i)
                {
                    if (i == -1)
                    {
                        continue;
                    }
                    if (t <= ptr->workt[i])
                    {
                        if (t < ptr->workt[i])
                        {
                            inow = i - 1;
                        }
                        else
                        {
                            inow = i;
                        }
                        if (inow < cnt2)
                        {
                            cnt2 = inow;
                        }
                        else
                        {
                            cnt1 = cnt2;
                            cnt2 = inow;
                        }
                        break;
                    }
                }
            }
            else   /* Zero-crossing disabled */
            {
                if (OutEnd == 2)
                {
                    double r = 0;
                    if (TP != 0)
                    {
                        r = floor((t / TP));
                    }
                    t -= static_cast<int>(r) * TP;
                    inow = nPoints - 1;
                }
                else
                {
                    inow = nPoints + 1; // Arbitrary value more than nPoints, will be overwritten if needed.
                }
                // Look in time value table a range to have current time in.
                // Beware exact values.
                for (int i = 0 ; i < nPoints; ++i)
                {
                    if (t <= ptr->workt[i])
                    {
                        if (t < ptr->workt[i])
                        {
                            inow = i - 1;
                        }
                        else
                        {
                            inow = i;
                        }
                        break;
                    }
                }
            }

            ptr->cnt1 = cnt1;
            ptr->cnt2 = cnt2;
            ptr->EVindex = EVindex;
            ptr->PerEVcnt = PerEVcnt;

            /***************************/
            /* Hypermatrix case */
            if (ptr->Hmat)
            {
                for (int j = 0; j < my * ny; ++j)
                {
                    if (ytype == SCSREAL_N)
                    {
                        /* real case */
                        y_d = GetRealOutPortPtrs(block, 1);
                        ptr_d = (double*) ptr->work;

                        if (inow >= nPoints)
                        {
                            if (OutEnd == 0)
                            {
                                y_d[j] = 0; /* Outputs set to zero */
                            }
                            else if (OutEnd == 1)
                            {
                                y_d[j] = ptr_d[(nPoints - 1) * ny * my + j]; /* Hold outputs at the end */
                            }
                        }
                        else
                        {
                            if (inow < 0)
                            {
                                y_d[j] = 0;
                            }
                            else
                            {
                                y_d[j] = ptr_d[inow * ny * my + j];
                            }
                        }
                    }
                    else if (ytype == SCSCOMPLEX_N)
                    {
                        /* complex case */
                        y_d = GetRealOutPortPtrs(block, 1);
                        y_cd = GetImagOutPortPtrs(block, 1);
                        ptr_d = (double*) ptr->work;

                        if (inow >= nPoints)
                        {
                            if (OutEnd == 0)
                            {
                                y_d[j] = 0; /* Outputs set to zero */
                                y_cd[j] = 0; /* Outputs set to zero */
                            }
                            else if (OutEnd == 1)
                            {
                                y_d[j] = ptr_d[(nPoints - 1) * ny * my + j]; /* Hold outputs at the end */
                                y_cd[j] = ptr_d[nPoints * my * ny + (nPoints - 1) * ny * my + j]; /* Hold outputs at the end */
                            }
                        }
                        else
                        {
                            if (inow < 0)
                            {
                                y_d[j] = 0; /* Outputs set to zero */
                                y_cd[j] = 0; /* Outputs set to zero */
                            }
                            else
                            {
                                y_d[j] = ptr_d[inow * ny * my + j];
                                y_cd[j] = ptr_d[nPoints * my * ny + inow * ny * my + j];
                            }
                        }
                    }
                    else if (ytype == SCSINT8_N)
                    {
                        /* --------------------- int8 char  ----------------------------*/
                        y_c = Getint8OutPortPtrs(block, 1);
                        ptr_c = (char*) ptr->work;
                        if (inow >= nPoints)
                        {
                            if (OutEnd == 0)
                            {
                                y_c[j] = 0; /* Outputs set to zero */
                            }
                            else if (OutEnd == 1)
                            {
                                y_c[j] = ptr_c[(nPoints - 1) * ny * my + j]; /* Hold outputs at the end */
                            }
                        }
                        else
                        {
                            if (inow < 0)
                            {
                                y_c[j] = 0;
                            }
                            else
                            {
                                y_c[j] = ptr_c[inow * ny * my + j];
                            }
                        }
                    }
                    else if (ytype == SCSINT16_N)
                    {
                        /* --------------------- int16 short int ---------------------*/
                        y_s = Getint16OutPortPtrs(block, 1);
                        ptr_s = (short int*) ptr->work;
                        if (inow >= nPoints)
                        {
                            if (OutEnd == 0)
                            {
                                y_s[j] = 0; /* Outputs set to zero */
                            }
                            else if (OutEnd == 1)
                            {
                                y_s[j] = ptr_s[(nPoints - 1) * ny * my + j]; /* Hold outputs at the end */
                            }
                        }
                        else
                        {
                            if (inow < 0)
                            {
                                y_s[j] = 0;
                            }
                            else
                            {
                                y_s[j] = ptr_s[inow * ny * my + j];
                            }
                        }
                    }
                    else if (ytype == SCSINT32_N)
                    {
                        /* --------------------- int32 long ---------------------*/
                        y_l = Getint32OutPortPtrs(block, 1);
                        ptr_l = (int*) ptr->work;
                        if (inow >= nPoints)
                        {
                            if (OutEnd == 0)
                            {
                                y_l[j] = 0; /* Outputs set to zero */
                            }
                            else if (OutEnd == 1)
                            {
                                y_l[j] = ptr_l[(nPoints - 1) * ny * my + j]; /* Hold outputs at the end */
                            }
                        }
                        else
                        {
                            if (inow < 0)
                            {
                                y_l[j] = 0;
                            }
                            else
                            {
                                y_l[j] = ptr_l[inow * ny * my + j];
                            }
                        }
                    }
                    else if (ytype == SCSUINT8_N)
                    {
                        /*--------------------- uint8 uchar ---------------------*/
                        y_uc = Getuint8OutPortPtrs(block, 1);
                        ptr_uc = (unsigned char*) ptr->work;
                        if (inow >= nPoints)
                        {
                            if (OutEnd == 0)
                            {
                                y_uc[j] = 0; /* Outputs set to zero */
                            }
                            else if (OutEnd == 1)
                            {
                                y_uc[j] = ptr_uc[(nPoints - 1) * ny * my + j]; /* Hold outputs at the end */
                            }
                        }
                        else
                        {
                            if (inow < 0)
                            {
                                y_uc[j] = 0;
                            }
                            else
                            {
                                y_uc[j] = ptr_uc[inow * ny * my + j];
                            }
                        }
                    }
                    else if (ytype == SCSUINT16_N)
                    {
                        /* --------------------- uint16 ushort ---------------------*/
                        y_us = Getuint16OutPortPtrs(block, 1);
                        ptr_us = (unsigned short int*) ptr->work;
                        if (inow >= nPoints)
                        {
                            if (OutEnd == 0)
                            {
                                y_us[j] = 0; /* Outputs set to zero */
                            }
                            else if (OutEnd == 1)
                            {
                                y_us[j] = ptr_us[(nPoints - 1) * ny * my + j]; /* Hold outputs at the end */
                            }
                        }
                        else
                        {
                            if (inow < 0)
                            {
                                y_us[j] = 0;
                            }
                            else
                            {
                                y_us[j] = ptr_us[inow * ny * my + j];
                            }
                        }
                    }
                    else if (ytype == SCSINT32_N)
                    {
                        /* --------------------- uint32 ulong ---------------------*/
                        y_ul = Getuint32OutPortPtrs(block, 1);
                        ptr_ul = (unsigned int*) ptr->work;
                        if (inow >= nPoints)
                        {
                            if (OutEnd == 0)
                            {
                                y_ul[j] = 0; /* Outputs set to zero */
                            }
                            else if (OutEnd == 1)
                            {
                                y_ul[j] = ptr_ul[(nPoints - 1) * ny * my + j]; /* Hold outputs at the end */
                            }
                        }
                        else
                        {
                            if (inow < 0)
                            {
                                y_ul[j] = 0;
                            }
                            else
                            {
                                y_ul[j] = ptr_ul[inow * ny * my + j];
                            }
                        }
                    }
                } /* for j loop */
            }
            /****************************/
            /* Scalar or vectorial case */
            else
            {
                for (int j = 0; j < my; ++j)
                {
                    double y1, y2, d1, d2, h, dh, ddh, dddh;
                    if (ytype == SCSREAL_N || ytype == SCSCOMPLEX_N)
                    {
                        /*  If real or complex*/
                        y_d = GetRealOutPortPtrs(block, 1);
                        ptr_d = (double*) ptr->work;
                        ptr_D = (double*) ptr->D;

                        if (inow >= nPoints)
                        {
                            if (OutEnd == 0)
                            {
                                y_d[j] = 0.0; /* Outputs set to zero */
                            }
                            else if (OutEnd == 1)
                            {
                                y_d[j] = ptr_d[nPoints - 1 + (j) * nPoints]; /* Hold outputs at the end */
                            }
                        }
                        else if (Method == 0)
                        {
                            if (inow < 0)
                            {
                                y_d[j] = 0.0;
                            }
                            else
                            {
                                y_d[j] = ptr_d[inow + (j) * nPoints];
                            }
                        }
                        else if (Method == 1)
                        {
                            if (inow < 0)
                            {
                                inow = 0;
                            }
                            t1 = ptr->workt[inow];
                            t2 = ptr->workt[inow + 1];
                            y1 = ptr_d[inow + j * nPoints];
                            y2 = ptr_d[inow + 1 + j * nPoints];
                            y_d[j] = (y2 - y1) * (t - t1) / (t2 - t1) + y1;
                        }
                        else if (Method >= 2)
                        {
                            if (inow < 0)
                            {
                                inow = 0;
                            }
                            t1 = ptr->workt[inow];
                            t2 = ptr->workt[inow + 1];
                            y1 = ptr_d[inow + j * nPoints];
                            y2 = ptr_d[inow + 1 + j * nPoints];
                            d1 = ptr_D[inow + j * nPoints];
                            d2 = ptr_D[inow + 1 + j * nPoints];
                            scicos_evalhermite(&t, &t1, &t2, &y1, &y2, &d1, &d2, &h, &dh, &ddh, &dddh, &inow);
                            y_d[j] = h;
                        }
                    }
                    if (ytype == SCSCOMPLEX_N)
                    {
                        /*  -------------- complex ----------------------*/
                        y_cd = GetImagOutPortPtrs(block, 1);
                        if (inow >= nPoints)
                        {
                            if (OutEnd == 0)
                            {
                                y_cd[j] = 0.0; /* Outputs set to zero*/
                            }
                            else if (OutEnd == 1)
                            {
                                y_cd[j] = ptr_d[nPoints * my + nPoints - 1 + (j) * nPoints]; // Hold outputs at the end
                            }
                        }
                        else if (Method == 0)
                        {
                            if (inow < 0)
                            {
                                y_cd[j] = 0.0; /* Outputs set to zero */
                            }
                            else
                            {
                                y_cd[j] = ptr_d[nPoints * my + inow + (j) * nPoints];
                            }
                        }
                        else if (Method == 1)
                        {
                            if (inow < 0)
                            {
                                inow = 0;
                            }
                            /* Extrapolation for 0<t<X(0) */
                            t1 = ptr->workt[inow];
                            t2 = ptr->workt[inow + 1];
                            y1 = ptr_d[nPoints * my + inow + j * nPoints];
                            y2 = ptr_d[nPoints * my + inow + 1 + j * nPoints];
                            y_cd[j] = (y2 - y1) * (t - t1) / (t2 - t1) + y1;
                        }
                        else if (Method >= 2)
                        {
                            if (inow < 0)
                            {
                                inow = 0;
                            }
                            t1 = ptr->workt[inow];
                            t2 = ptr->workt[inow + 1];
                            y1 = ptr_d[inow + j * nPoints + nPoints];
                            y2 = ptr_d[inow + 1 + j * nPoints + nPoints];
                            d1 = ptr_D[inow + j * nPoints + nPoints];
                            d2 = ptr_D[inow + 1 + j * nPoints + nPoints];
                            scicos_evalhermite(&t, &t1, &t2, &y1, &y2, &d1, &d2, &h, &dh, &ddh, &dddh, &inow);
                            y_cd[j] = h;
                        }
                    }
                    else if (ytype == SCSINT8_N)
                    {
                        /* --------------------- int8 char  ----------------------------*/
                        y_c = Getint8OutPortPtrs(block, 1);
                        ptr_c = (char*) ptr->work;
                        /* y_c[j]=ptr_c[inow+(j)*nPoints]; */
                        if (inow >= nPoints)
                        {
                            if (OutEnd == 0)
                            {
                                y_c[j] = 0; /* Outputs set to zero */
                            }
                            else if (OutEnd == 1)
                            {
                                y_c[j] = ptr_c[nPoints - 1 + (j) * nPoints]; /* Hold outputs at the end */
                            }
                        }
                        else if (Method == 0)
                        {
                            if (inow < 0)
                            {
                                y_c[j] = 0;
                            }
                            else
                            {
                                y_c[j] = ptr_c[inow + (j) * nPoints];
                            }
                        }
                        else if (Method >= 1)
                        {
                            if (inow < 0)
                            {
                                inow = 0;
                            }
                            t1 = ptr->workt[inow];
                            t2 = ptr->workt[inow + 1];
                            y1 = (double)ptr_c[inow + j * nPoints];
                            y2 = (double)ptr_c[inow + 1 + j * nPoints];
                            y_c[j] = (char)((y2 - y1) * (t - t1) / (t2 - t1) + y1);
                        }
                    }       
                    else if (ytype == SCSINT16_N)
                    {
                        /* --------------------- int16 short ---------------------*/
                        y_s = Getint16OutPortPtrs(block, 1);
                        ptr_s = (short int*) ptr->work;
                        /* y_s[j]=ptr_s[inow+(j)*nPoints]; */
                        if (inow >= nPoints)
                        {
                            if (OutEnd == 0)
                            {
                                y_s[j] = 0; /* Outputs set to zero */
                            }
                            else if (OutEnd == 1)
                            {
                                y_s[j] = ptr_s[nPoints - 1 + (j) * nPoints]; // Hold outputs at the end
                            }
                        }
                        else if (Method == 0)
                        {
                            if (inow < 0)
                            {
                                y_s[j] = 0;
                            }
                            else
                            {
                                y_s[j] = ptr_s[inow + (j) * nPoints];
                            }
                        }
                        else if (Method >= 1)
                        {
                            if (inow < 0)
                            {
                                inow = 0;
                            }
                            t1 = ptr->workt[inow];
                            t2 = ptr->workt[inow + 1];
                            y1 = (double)ptr_s[inow + j * nPoints];
                            y2 = (double)ptr_s[inow + 1 + j * nPoints];
                            y_s[j] = (short int)((y2 - y1) * (t - t1) / (t2 - t1) + y1);
                        }
                    }       
                    else if (ytype == SCSINT32_N)
                    {
                        /* --------------------- int32 long ---------------------*/
                        y_l = Getint32OutPortPtrs(block, 1);
                        ptr_l = (int*) ptr->work;
                        /* y_l[j]=ptr_l[inow+(j)*nPoints]; */
                        if (inow >= nPoints)
                        {
                            if (OutEnd == 0)
                            {
                                y_l[j] = 0; /* Outputs set to zero */
                            }
                            else if (OutEnd == 1)
                            {
                                y_l[j] = ptr_l[nPoints - 1 + (j) * nPoints]; /* Hold outputs at the end */
                            }
                        }
                        else if (Method == 0)
                        {
                            if (inow < 0)
                            {
                                y_l[j] = 0;
                            }
                            else
                            {
                                y_l[j] = ptr_l[inow + (j) * nPoints];
                            }
                        }
                        else if (Method >= 1)
                        {
                            t1 = ptr->workt[inow];
                            t2 = ptr->workt[inow + 1];
                            y1 = (double)ptr_l[inow + j * nPoints];
                            y2 = (double)ptr_l[inow + 1 + j * nPoints];
                            y_l[j] = (int)((y2 - y1) * (t - t1) / (t2 - t1) + y1);
                        }
                    }       
                    else if (ytype == SCSUINT8_N)
                    {
                        /*--------------------- uint8 uchar ---------------------*/
                        y_uc = Getuint8OutPortPtrs(block, 1);
                        ptr_uc = (unsigned char*) ptr->work;
                        /* y_uc[j]=ptr_uc[inow+(j)*nPoints]; */
                        if (inow >= nPoints)
                        {
                            if (OutEnd == 0)
                            {
                                y_uc[j] = 0; /* Outputs set to zero */
                            }
                            else if (OutEnd == 1)
                            {
                                y_uc[j] = ptr_uc[nPoints - 1 + (j) * nPoints]; /* Hold outputs at the end */
                            }
                        }
                        else if (Method == 0)
                        {
                            if (inow < 0)
                            {
                                y_uc[j] = 0;
                            }
                            else
                            {
                                y_uc[j] = ptr_uc[inow + (j) * nPoints];
                            }
                        }
                        else if (Method >= 1)
                        {
                            t1 = ptr->workt[inow];
                            t2 = ptr->workt[inow + 1];
                            y1 = (double)ptr_uc[inow + j * nPoints];
                            y2 = (double)ptr_uc[inow + 1 + j * nPoints];
                            y_uc[j] = (unsigned char)((y2 - y1) * (t - t1) / (t2 - t1) + y1);
                        }
                    }       
                    else if (ytype == SCSUINT16_N)
                    {
                        /* --------------------- uint16 ushort int ---------------------*/
                        y_us = Getuint16OutPortPtrs(block, 1);
                        ptr_us = (unsigned short int*) ptr->work;
                        /* y_us[j]=ptr_us[inow+(j)*nPoints]; */
                        if (inow >= nPoints)
                        {
                            if (OutEnd == 0)
                            {
                                y_us[j] = 0; /* Outputs set to zero */
                            }
                            else if (OutEnd == 1)
                            {
                                y_us[j] = ptr_us[nPoints - 1 + (j) * nPoints]; /* Hold outputs at the end */
                            }
                        }
                        else if (Method == 0)
                        {
                            if (inow < 0)
                            {
                                y_us[j] = 0;
                            }
                            else
                            {
                                y_us[j] = ptr_us[inow + (j) * nPoints];
                            }
                        }
                        else if (Method >= 1)
                        {
                            t1 = ptr->workt[inow];
                            t2 = ptr->workt[inow + 1];
                            y1 = (double)ptr_us[inow + j * nPoints];
                            y2 = (double)ptr_us[inow + 1 + j * nPoints];
                            y_us[j] = (unsigned short int)((y2 - y1) * (t - t1) / (t2 - t1) + y1);
                        }
                    }       
                    else if (ytype == SCSUINT32_N)
                    {
                        /* --------------------- uint32 ulong ---------------------*/
                        y_ul = Getuint32OutPortPtrs(block, 1);
                        ptr_ul = (unsigned int*) ptr->work;
                        /* y_ul[j]=ptr_ul[inow+(j)*nPoints]; */
                        if (inow > nPoints)
                        {
                            if (OutEnd == 0)
                            {
                                y_ul[j] = 0; /* Outputs set to zero */
                            }
                            else if (OutEnd == 1)
                            {
                                y_ul[j] = ptr_ul[nPoints - 1 + (j) * nPoints]; /* Hold outputs at the end */
                            }
                        }
                        else if (Method == 0)
                        {
                            if (inow < 0)
                            {
                                y_ul[j] = 0;
                            }
                            else
                            {
                                y_ul[j] = ptr_ul[inow + (j) * nPoints];
                            }
                        }
                        else if (Method >= 1)
                        {
                            t1 = ptr->workt[inow];
                            t2 = ptr->workt[inow + 1];
                            y1 = (double)ptr_ul[inow + j * nPoints];
                            y2 = (double)ptr_ul[inow + 1 + j * nPoints];
                            y_ul[j] = (unsigned int)((y2 - y1) * (t - t1) / (t2 - t1) + y1);
                        }
                    }
                } /* for j loop */
            }
            /********************************************************************/
            break;
        }
        case 3 :
        {
            /* Event date computation */
            /* Retrieve 'ptr' of the structure of the block */
            ptr = *work;
            int nPoints = ptr->nPoints;
            int cnt1 = ptr->cnt1;
            int cnt2 = ptr->cnt2;
            int EVindex = ptr->EVindex;
            int PerEVcnt = ptr->PerEVcnt;

            /* Get current simulation time */
            //double t = get_scicos_time();

            double TNm1  = ptr->workt[nPoints - 1];
            double TP    = TNm1 - 0;

            int jfirst;
            if (ZC == 1)
            {
                /* Generate Events only if ZC is active */
                if ((Method == 1) || (Method == 0))
                {
                    /*-------------------------*/
                    if (ptr->firstevent == 1)
                    {
                        jfirst = nPoints - 1; /* Finding first positive time instant */
                        for (int j = 0; j < nPoints; ++j)
                        {
                            if (ptr->workt[j] > 0)
                            {
                                jfirst = j;
                                break;
                            }
                        }
                        block->evout[0] = ptr->workt[jfirst];
                        EVindex = jfirst;
                        ptr->EVindex = EVindex;
                        ptr->firstevent = 0;
                        return;
                    }
                    /*------------------------*/
                    int i = EVindex;
                    /*------------------------*/
                    if (i < nPoints - 1)
                    {
                        block->evout[0] = ptr->workt[i + 1] - ptr->workt[i];
                        EVindex = i + 1;
                    }
                    /*------------------------*/
                    if (i == nPoints - 1)
                    {
                        if (OutEnd == 2)
                        {
                            /*  Periodic */
                            cnt1 = -1;
                            cnt2 = 0;
                            PerEVcnt++; /* When OutEnd==2 (perodic output) */
                            jfirst = nPoints - 1; /* Finding first positive time instant */
                            for (int j = 0; j < nPoints; ++j)
                            {
                                if (ptr->workt[j] >= 0)
                                {
                                    jfirst = j;
                                    break;
                                }
                            }
                            block->evout[0] = ptr->workt[jfirst];
                            EVindex = jfirst;
                        }
                    }
                    /*-------------------------- */
                }
                else if (Method <= 3)
                {
                    if (ptr->firstevent == 1)
                    {
                        block->evout[0] = TP;
                        ptr->firstevent = 0;
                    }
                    else
                    {
                        if (OutEnd == 2)
                        {
                            block->evout[0] = TP;
                        }
                        PerEVcnt++;
                    }
                    cnt1 = -1;
                    cnt2 = 0;
                }
                ptr->cnt1 = cnt1;
                ptr->cnt2 = cnt2;
                ptr->EVindex = EVindex;
                ptr->PerEVcnt = PerEVcnt;
            }
            /***********************************************************************/
            break;
        }
        case 5 :
        {
            /* Finish */
            ptr = *work;
            if (ptr != nullptr)
            {
                if (ptr->D != nullptr)
                {
                    delete[] ptr->D;
                }
                if (ptr->work != nullptr)
                {
                    FREE(ptr->work);
                }
                if (ptr->workt != nullptr)
                {
                    delete[] ptr->workt;
                }
                delete ptr;
            }
            break;
            /***********************************************************************/
        }
        case 6 :
        {
            /* Re-init */

            // Finish then init
            fromws_c(block, 5);
            fromws_c(block, 4);
            break;
        }
        /*************************************************************************/
        default :
            return;
    }
}
