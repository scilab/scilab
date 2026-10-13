//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Vladislav Trubkin
// Copyright (C) - 2017 - ESI Group - Clement DAVID
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

#include "fmu_wrapper.hxx"

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include "sciprint.h"
#include "scicos.h"
#include "dynlib_scicos_blocks.h"
#include "scicos_block4.h"
#include "scicos_malloc.h"
#include "scicos_free.h"

#include "fmu_sim_utility.h"

    SCICOS_BLOCKS_IMPEXP void fmu_simulate_me(scicos_block *block, scicos_flag flag);
}

SCICOS_BLOCKS_IMPEXP void fmu_simulate_me(scicos_block *block, scicos_flag flag)
{
    unsigned int i = 0;
    char *pathName = NULL, *identName = NULL, *guid = NULL;
    double *sciTolerance = NULL;
    int *sciInputTypes = NULL, *sciOutputTypes = NULL;
    unsigned int *sciNumberOfStates = NULL, *sciNumberOfEvents = NULL;
    fmiStatus status;
    workFMU *structFmu = (workFMU*)(*block->work);
    FmuModelExchange *fmu = nullptr;
    fmiCallbackFunctions functions;
    double time = 0;
    fmiValueReference *sciRealRefs = NULL, *sciIntRefs = NULL, *sciBoolRefs = NULL;
    fmiValueReference *sciInputRefs = NULL, *sciOutputRefs = NULL;
    fmiReal *rpar = NULL, *boolValue = NULL;
    fmiBoolean loggingOn = fmiFalse;
    fmiInteger *ipar = NULL;
    size_t realRefsSize, intRefsSize, boolRefsSize, outRefsSize, inpRefsSize;

    // return if an fmu has not been loaded
    if (block->nopar == 0)
    {
        return;
    }

    // real values
    // refs are stored as [refs..., negated alias flags...]
    sciRealRefs = Getuint32OparPtrs(block, 6); // real refs
    realRefsSize = GetOparSize(block, 6, 1) * GetOparSize(block, 6, 2) / 2; // real refs size
    rpar = GetRparPtrs(block); // real values
    // integer values
    sciIntRefs = Getuint32OparPtrs(block, 7);
    intRefsSize = GetOparSize(block, 7, 1) * GetOparSize(block, 7, 2) / 2;
    ipar = GetIparPtrs(block);
    // boolean values
    sciBoolRefs = Getuint32OparPtrs(block, 8);
    boolRefsSize = GetOparSize(block, 8, 1) * GetOparSize(block, 8, 2) / 2;
    boolValue = GetRealOparPtrs(block, 9); // boolean values
    // inputs
    sciInputRefs = Getuint32OparPtrs(block, 2);
    inpRefsSize = GetOparSize(block, 2, 1) * GetOparSize(block, 2, 2);
    sciInputTypes = Getint32OparPtrs(block, 3);
    // outputs
    sciOutputRefs = Getuint32OparPtrs(block, 4);
    outRefsSize = GetOparSize(block, 4, 1) * GetOparSize(block, 4, 2);
    sciOutputTypes = Getint32OparPtrs(block, 5);
    // states and events
    sciNumberOfStates = Getuint32OparPtrs(block, 14);
    sciNumberOfEvents = Getuint32OparPtrs(block, 15);

    if (flag == Initialization)
    {
        // sciprint("\n");
        // sciprint("*****Initialization block [%d] Time [%f] \n",get_block_number(), get_scicos_time());
        pathName = Getint8OparPtrs(block, 10); // path to
        identName = Getint8OparPtrs(block, 11); // identifier
        guid = Getint8OparPtrs(block, 12); // guid
        loggingOn = *Getint8OparPtrs(block, 13); // logger
        sciTolerance = GetRealOparPtrs(block, 16); // relative tolerance

        // memory allocation for the main structure
        if ((*(block->work) = (workFMU*)scicos_malloc(sizeof(workFMU))) == NULL)
        {
            set_block_error(-16); // memory allocation error
            *(block->work) = NULL;
            return;
        }
        structFmu = (workFMU*)(*block->work);
        memset(structFmu, 0x0, sizeof(workFMU));

        // load library and create an instance
        structFmu->fmuME1 = new FmuModelExchange();
        fmu = structFmu->fmuME1;
        if (fmu->create(pathName, identName) == false)
        {
            //sciprint(fmu_wrap_error(), "fmu_simulate_me");
            free_instance(block, -3, "fmu_wrap_model_exchange_new"); // internal error
            return;
        }
        // set fmiType
        structFmu->type = workFMU::ModelExchange;
        structFmu->version = workFMU::fmiV1;

        // calling helper functions
        functions.logger = fmu_callback_logger;
        functions.allocateMemory = calloc;
        functions.freeMemory = free;
        functions.stepFinished = NULL;
        // instantiate model
        structFmu->instance = fmu->fmiInstantiateModel(identName, guid, functions, loggingOn);
        if (structFmu->instance == NULL)
        {
            free_instance(block, -3, "fmiInstantiateModel");
            return;
        }
        // set debug logging
        if (loggingOn)
        {
            status = fmu->fmiSetDebugLogging(structFmu->instance, fmiTrue);
            if (status > 1)
            {
                free_instance(block, -3, "fmiSetDebugLogging");
                return;
            }
        }
        // set start time
        status = fmu->fmiSetTime((structFmu->instance), get_scicos_time());
        if (status > 1)
        {
            free_instance(block, -3, "fmiSetTime");
            return;
        }
        // set start values
        // real
        if (realRefsSize > 0)
        {
            if (((structFmu->data.modelExchangeV1.realRefs) = (fmiValueReference*)scicos_malloc(realRefsSize * sizeof(fmiValueReference))) == NULL)
            {
                free_instance(block, -16, "scicos_malloc");
                return;
            }
            for (i = 0; i < realRefsSize; i++)
            {
                // negated alias
                if (sciRealRefs[realRefsSize + i] > 0)
                {
                    rpar[i] = -rpar[i];
                }
                structFmu->data.modelExchangeV1.realRefs[i] = sciRealRefs[i];
            }
            // set real values
            status = fmu->fmiSetReal((structFmu->instance), structFmu->data.modelExchangeV1.realRefs, realRefsSize, rpar);
            if (status > fmiDiscard)
            {
                free_instance(block, -3, "fmiSetReal");
                return;
            }
        }
        // integers
        if (intRefsSize > 0)
        {
            if (((structFmu->data.modelExchangeV1.intRefs) = (fmiValueReference*)scicos_malloc(intRefsSize * sizeof(fmiValueReference))) == NULL)
            {
                free_instance(block, -16, "scicos_malloc");
                return;
            }
            for (i = 0; i < intRefsSize; i++)
            {
                // negated alias
                if (sciIntRefs[intRefsSize + i] > 0)
                {
                    ipar[i] = -ipar[i];
                }
                structFmu->data.modelExchangeV1.intRefs[i] = sciIntRefs[i];
            }
            // set integer values
            status = fmu->fmiSetInteger((structFmu->instance), structFmu->data.modelExchangeV1.intRefs, intRefsSize, ipar);
            if (status > fmiWarning)
            {
                free_instance(block, -3, "fmiSetInteger");
                return;
            }
        }
        // boolean
        if (boolRefsSize > 0)
        {
            if (((structFmu->data.modelExchangeV1.boolValue) = (fmiBoolean*)scicos_malloc(boolRefsSize * sizeof(fmiBoolean))) == NULL)
            {
                free_instance(block, -16, "scicos_malloc");
                return;
            }
            for (i = 0; i < boolRefsSize; i++)
            {
                if (sciBoolRefs[boolRefsSize + i] > 0)
                {
                    // negated alias
                    if (boolValue[i] == 1)
                    {
                        structFmu->data.modelExchangeV1.boolValue[i] = fmiFalse;
                    }
                    else
                    {
                        structFmu->data.modelExchangeV1.boolValue[i] = fmiTrue;
                    }
                }
                else
                {
                    if (boolValue[i] == 1)
                    {
                        structFmu->data.modelExchangeV1.boolValue[i] = fmiTrue;
                    }
                    else
                    {
                        structFmu->data.modelExchangeV1.boolValue[i] = fmiFalse;
                    }
                }
            }
            if (((structFmu->data.modelExchangeV1.boolRefs) = (fmiValueReference*)scicos_malloc(boolRefsSize * sizeof(fmiValueReference))) == NULL)
            {
                free_instance(block, -16, "scicos_malloc");
                return;
            }
            for (i = 0; i < boolRefsSize; i++)
            {
                structFmu->data.modelExchangeV1.boolRefs[i] = sciBoolRefs[i];
            }
            // set boolean values
            status = fmu->fmiSetBoolean((structFmu->instance), structFmu->data.modelExchangeV1.boolRefs, boolRefsSize, structFmu->data.modelExchangeV1.boolValue);
            if (status > fmiWarning)
            {
                free_instance(block, -3, "fmiSetBoolean");
                return;
            }
        }
        // initialize model
        if (sciTolerance[0])
        {
            status = fmu->fmiInitialize((structFmu->instance), fmiTrue, 1.0E-06, &structFmu->data.modelExchangeV1.eventInfo);
            if (status > fmiWarning)
            {
                free_instance(block, -3, "fmiInitialize");
                return;
            }
        }
        else
        {
            status = fmu->fmiInitialize((structFmu->instance), fmiFalse, 0.0, &structFmu->data.modelExchangeV1.eventInfo);
            if (status > fmiWarning)
            {
                free_instance(block, -3, "fmiInitialize");
                return;
            }
        }

        if (sciNumberOfStates[0] > 0)
        {
            structFmu->data.modelExchangeV1.states = (double*)scicos_malloc(sciNumberOfStates[0] * sizeof(double));
            structFmu->data.modelExchangeV1.nominalStates = (double*)scicos_malloc(sciNumberOfStates[0] * sizeof(double));
            structFmu->data.modelExchangeV1.derivatives = (double*)scicos_malloc(sciNumberOfStates[0] * sizeof(double));
            structFmu->data.modelExchangeV1.refsStates = (fmiValueReference*) scicos_malloc(sciNumberOfStates[0] * sizeof(fmiValueReference));
            if (!structFmu->data.modelExchangeV1.states || !structFmu->data.modelExchangeV1.nominalStates || !structFmu->data.modelExchangeV1.derivatives || !structFmu->data.modelExchangeV1.refsStates)
            {
                free_instance(block, -16, "scicos_malloc");
                return;
            }
        }

        status = fmu->fmiGetContinuousStates((structFmu->instance), structFmu->data.modelExchangeV1.states, sciNumberOfStates[0]);
        if (status > 1)
        {
            free_instance(block, -3, "fmiGetContinuousStates");
            return;
        }
        for (i = 0; i < sciNumberOfStates[0]; i++)
        {
            block->x[i] = structFmu->data.modelExchangeV1.states[i];
        }

        status = fmu->fmiGetStateValueReferences((structFmu->instance), structFmu->data.modelExchangeV1.refsStates, sciNumberOfStates[0]);
        if (status > 1)
        {
            free_instance(block, -3, "fmiGetStateValueReferences");
            return;
        }

        if (sciNumberOfEvents[0] > 0)
        {
            structFmu->data.modelExchangeV1.events = (double*)scicos_malloc(sciNumberOfEvents[0] * sizeof(double));
            structFmu->data.modelExchangeV1.tmpevents = (double*)scicos_malloc(sciNumberOfEvents[0] * sizeof(double));
            if (!structFmu->data.modelExchangeV1.events || !structFmu->data.modelExchangeV1.tmpevents)
            {
                free_instance(block, -16, "scicos_malloc");
                return;
            }
            status = fmu->fmiGetEventIndicators((structFmu->instance), structFmu->data.modelExchangeV1.events, sciNumberOfEvents[0]);
            if (status > fmiDiscard)
            {
                free_instance(block, -3, "fmiGetEventIndicators");
                return;
            }
        }
        // retrieve solution at time = 0.00
        if (fmiGet_values(block, sciOutputTypes, &block->outsz[2*block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmiWarning)
        {
            free_instance(block, -3, "fmiGet_values");
            return;
        }
    }

    else if (flag == DerivativeState)
    {
        // sciprint("\n");
        // sciprint("*****Derivative State block [%d] at time: %f\n",get_block_number(), get_scicos_time());
        if ((structFmu = (workFMU*)(*block->work)) == NULL)
        {
            return;
        }
        fmu = structFmu->fmuME1;

        structFmu->data.modelExchangeV1.callEventUpdate = fmiFalse;

        // set time
        time = get_scicos_time();
        status = fmu->fmiSetTime((structFmu->instance), time);
        if (status > fmiWarning)
        {
            free_instance(block, -3, "fmiSetTime");
            return;

        }

        // set inputs
        if (fmiSet_values(block, flag, 1, sciInputTypes, &block->insz[2*block->nin], inpRefsSize, sciInputRefs, block->inptr) > fmiWarning)
        {
            free_instance(block, -3, "fmiSet_values");
            return;
        }
        for (i = 0; i < sciNumberOfStates[0]; i++)
        {
            structFmu->data.modelExchangeV1.states[i] = block->x[i];
        }
        status = fmu->fmiSetContinuousStates((structFmu->instance), structFmu->data.modelExchangeV1.states, sciNumberOfStates[0]);
        if (status > fmiDiscard)
        {
            free_instance(block, -3, "fmiSetContinuousStates");
            return;
        }
        status = fmu->fmiCompletedIntegratorStep((structFmu->instance), &structFmu->data.modelExchangeV1.callEventUpdate);
        if (status > fmiWarning)
        {
            free_instance(block, -3, "fmiCompletedIntegratorStep");
            return;
        }
        status = fmu->fmiGetDerivatives((structFmu->instance), structFmu->data.modelExchangeV1.derivatives, sciNumberOfStates[0]);
        if (status > fmiDiscard)
        {
            free_instance(block, -3, "fmiGetDerivatives");
            return;
        }
        for (i = 0; i < sciNumberOfStates[0]; i++)
        {
            block->xd[i] = structFmu->data.modelExchangeV1.derivatives[i];
        }
        if (fmiGet_values(block, sciOutputTypes, &block->outsz[2*block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmiWarning)
        {
            free_instance(block, -3, "fmiGet_values");
            return;
        }
    }

    else if (flag == OutputUpdate)
    {
        // sciprint("\n");
        // sciprint("*****Output update block [%d] at time: %f\n", get_block_number(), get_scicos_time());
        if ((structFmu = (workFMU*)(*block->work)) == NULL)
        {
            return;
        }
        fmu = structFmu->fmuME1;

        if (!structFmu->data.modelExchangeV1.eventInfo.terminateSimulation)
        {
            structFmu->data.modelExchangeV1.timeEvent = fmiFalse;
            structFmu->data.modelExchangeV1.stateEvent = fmiFalse;

            time = get_scicos_time();
            status = fmu->fmiSetTime((structFmu->instance), time);
            if (status > fmiWarning)
            {
                free_instance(block, -3, "fmiSetTime");
                return;
            }
            if (fmiSet_values(block, flag, 0, sciInputTypes, &block->insz[2*block->nin], inpRefsSize, sciInputRefs, block->inptr) > fmiWarning)
            {
                free_instance(block, -3, "fmiSet_values");
                return;
            }
            if (sciNumberOfEvents[0] > 0)
            {
                for (i = 0; i < sciNumberOfEvents[0]; i++)
                {
                    structFmu->data.modelExchangeV1.tmpevents[i] = structFmu->data.modelExchangeV1.events[i];
                }
                status = fmu->fmiGetEventIndicators((structFmu->instance), structFmu->data.modelExchangeV1.events, sciNumberOfEvents[0]);
                if (status > fmiDiscard)
                {
                    free_instance(block, -3, "fmiGetEventIndicators");
                    return;
                }
            }

            structFmu->data.modelExchangeV1.timeEvent = (structFmu->data.modelExchangeV1.eventInfo.upcomingTimeEvent) && (structFmu->data.modelExchangeV1.eventInfo.nextEventTime <= time);

            for (i = 0; i < sciNumberOfEvents[0]; i++)
            {
                if (structFmu->data.modelExchangeV1.tmpevents[i] * structFmu->data.modelExchangeV1.events[i] < 0)
                {
                    structFmu->data.modelExchangeV1.stateEvent = fmiTrue;
                    break;
                }
            }

            if (structFmu->data.modelExchangeV1.timeEvent || structFmu->data.modelExchangeV1.stateEvent || structFmu->data.modelExchangeV1.callEventUpdate)
            {
                structFmu->data.modelExchangeV1.eventInfo.iterationConverged = fmiFalse;
                status = fmu->fmiEventUpdate((structFmu->instance), fmiFalse, &structFmu->data.modelExchangeV1.eventInfo);
                if (status > fmiWarning)
                {
                    free_instance(block, -3, "fmiEventUpdate");
                    return;
                }
                if (structFmu->data.modelExchangeV1.eventInfo.iterationConverged)
                {
                    if (fmiGet_values(block, sciOutputTypes, &block->outsz[2*block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmiWarning)
                    {
                        free_instance(block, -3, "fmiGet_values");
                        return;
                    }
                }
                if (structFmu->data.modelExchangeV1.eventInfo.stateValuesChanged)
                {
                    status = fmu->fmiGetContinuousStates((structFmu->instance), structFmu->data.modelExchangeV1.states, sciNumberOfStates[0]);
                    if (status > fmiWarning)
                    {
                        free_instance(block, -3, "fmiGetContinuousStates");
                        return;
                    }
                    for (i = 0; i < sciNumberOfStates[0]; i++)
                    {
                        block->x[i] = structFmu->data.modelExchangeV1.states[i];
                    }
                }
                if (structFmu->data.modelExchangeV1.eventInfo.stateValueReferencesChanged)
                {
                    status = fmu->fmiGetNominalContinuousStates((structFmu->instance), structFmu->data.modelExchangeV1.nominalStates, sciNumberOfStates[0]);
                    if (status > fmiWarning)
                    {
                        free_instance(block, -3, "fmiGetNominalContinuousStates");
                        return;
                    }
                }
            }

            // get outputs
            if (fmiGet_values(block, sciOutputTypes, &block->outsz[2*block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmiWarning)
            {
                free_instance(block, -3, "fmiGet_values");
                return;
            }
        }
    }
    else if (flag == Ending)
    {
        // sciprint("\n");
        // sciprint("*****Ending block [%d] \n", get_block_number());
        if ((structFmu = (workFMU*)(*block->work)) == NULL)
        {
            return;
        }
        fmu = structFmu->fmuME1;

        time = get_scicos_time();
        status = fmu->fmiSetTime((structFmu->instance), time);
        if (status > fmiWarning)
        {
            free_instance(block, -3, "fmiSetTime");
            return;
        }

        // retrieve outputs at final time; the FMU is unusable after fmiTerminate
        if (fmiGet_values(block, sciOutputTypes, &block->outsz[2*block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmiWarning)
        {
            free_instance(block, -3, "fmiGet_values");
            return;
        }

        // terminate simulation
        status = fmu->fmiTerminate(structFmu->instance);
        if (status > fmiWarning)
        {
            free_instance(block, -3, "fmiTerminate");
            return;
        }

        // freeing of instance
        free_instance(block, 0, "fmu_simulate_me");
        return;
    }
}
