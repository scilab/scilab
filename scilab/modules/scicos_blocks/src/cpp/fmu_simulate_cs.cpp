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

    SCICOS_BLOCKS_IMPEXP void fmu_simulate_cs(scicos_block *block, scicos_flag flag);
}

SCICOS_BLOCKS_IMPEXP void fmu_simulate_cs(scicos_block *block, scicos_flag flag)
{
    size_t i = 0;
    char *pathName = NULL, *identName = NULL, *guid = NULL;
    int *sciInputTypes = NULL, *sciOutputTypes = NULL;
    fmiStatus status;
    // main structure
    workFMU *structFmu = nullptr;
    FmuCoSimulation *fmu = nullptr;
    fmiCallbackFunctions functions;
    double time = 0, dt = 0;
    fmiValueReference *sciRealRefs = NULL, *sciIntRefs = NULL, *sciBoolRefs = NULL;
    fmiValueReference *sciInputRefs = NULL, *sciOutputRefs = NULL;
    fmiReal *rpar = NULL, *boolValue = NULL;
    fmiInteger *ipar = NULL;
    size_t realRefsSize, intRefsSize, boolRefsSize, outRefsSize, inpRefsSize;
    fmiString fmuLocation = NULL, mimeType = NULL;
    fmiReal timeout = 1000, step = 0, tStart = 0, tStop = 0;
    fmiBoolean visible = fmiFalse, interactive = fmiFalse;
    fmiBoolean loggingOn = fmiFalse, updateStep = fmiTrue;
    fmiBoolean StopTimeDefined = fmiFalse;

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

    if (flag == Initialization)
    {
        // sciprint("\n");
        // sciprint("*****Initialization of block [%d] Time [%f] \n",get_block_number(), get_scicos_time());
        pathName = Getint8OparPtrs(block, 10); // path to 
        identName = Getint8OparPtrs(block, 11); // identifier
        guid = Getint8OparPtrs(block, 12); // guid
        loggingOn = *Getint8OparPtrs(block, 13); // logger

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
        structFmu->fmuCS1 = new FmuCoSimulation();
        fmu = structFmu->fmuCS1;
        if (fmu->create(pathName, identName) == false)
        {
            //sciprint(fmu_wrap_error(), "fmu_simulate_cs");
            free_instance(block, -3, "fmu_wrap_co_simulation_new"); // internal error
            return;
        }
        // set fmiType
        structFmu->type = workFMU::CoSimulation;
        structFmu->version = workFMU::fmiV1;

        // helper functions
        functions.logger = fmu_callback_logger;
        functions.allocateMemory = calloc;
        functions.freeMemory = free;
        functions.stepFinished = NULL;

        // instantiate slave
        structFmu->instance = fmu->fmiInstantiateSlave(identName, guid, fmuLocation, mimeType, timeout, visible, interactive, functions, loggingOn);
        if (structFmu->instance == NULL)
        {
            free_instance(block, -3, "fmiInstantiateSlave");
            return;
        }
        // set debug logging
        if (loggingOn)
        {
            status = fmu->fmiSetDebugLogging(structFmu->instance, loggingOn);
            if (status > fmiWarning)
            {
                free_instance(block, -3, "fmiSetDebugLogging");
                return;
            }
        }

        // set start values
        // real
        if (realRefsSize > 0)
        {
            if (((structFmu->data.coSimulationV1.realRefs) = (fmiValueReference*)scicos_malloc(realRefsSize * sizeof(fmiValueReference))) == NULL)
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
                structFmu->data.coSimulationV1.realRefs[i] = sciRealRefs[i];
            }
            // set real values
            status = fmu->fmiSetReal((structFmu->instance), structFmu->data.coSimulationV1.realRefs, realRefsSize, rpar);
            if (status > fmiDiscard)
            {
                free_instance(block, -3, "fmiSetReal");
                return;
            }
        }
        // integers
        if (intRefsSize > 0)
        {
            if (((structFmu->data.coSimulationV1.intRefs) = (fmiValueReference*)scicos_malloc(intRefsSize * sizeof(fmiValueReference))) == NULL)
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
                structFmu->data.coSimulationV1.intRefs[i] = sciIntRefs[i];
            }
            // set integer values
            status = fmu->fmiSetInteger((structFmu->instance), structFmu->data.coSimulationV1.intRefs, intRefsSize, ipar);
            if (status > fmiWarning)
            {
                free_instance(block, -3, "fmiSetInteger");
                return;
            }
        }
        // boolean
        if (boolRefsSize > 0)
        {
            if (((structFmu->data.coSimulationV1.boolValue) = (fmiBoolean*)scicos_malloc(boolRefsSize * sizeof(fmiBoolean))) == NULL)
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
                        structFmu->data.coSimulationV1.boolValue[i] = fmiFalse;
                    }
                    else
                    {
                        structFmu->data.coSimulationV1.boolValue[i] = fmiTrue;
                    }
                }
                else
                {
                    if (boolValue[i] == 1)
                    {
                        structFmu->data.coSimulationV1.boolValue[i] = fmiTrue;
                    }
                    else
                    {
                        structFmu->data.coSimulationV1.boolValue[i] = fmiFalse;
                    }
                }
            }
            if (((structFmu->data.coSimulationV1.boolRefs) = (fmiValueReference*)scicos_malloc(boolRefsSize * sizeof(fmiValueReference))) == NULL)
            {
                free_instance(block, -16, "scicos_malloc");
                return;
            }
            for (i = 0; i < boolRefsSize; i++)
            {
                structFmu->data.coSimulationV1.boolRefs[i] = sciBoolRefs[i];
            }
            // set boolean values
            status = fmu->fmiSetBoolean((structFmu->instance), structFmu->data.coSimulationV1.boolRefs, boolRefsSize, structFmu->data.coSimulationV1.boolValue);
            if (status > fmiWarning)
            {
                free_instance(block, -3, "fmiSetBoolean");
                return;
            }
        }

        // initialize slave
        tStart = get_scicos_time();
        // StopTimeDefined == fmiFalse, tStop == 0
        status = fmu->fmiInitializeSlave(structFmu->instance, tStart, StopTimeDefined, tStop);
        if (status > 1)
        {
            free_instance(block, -3, "fmiInitializeSlave");
            return;
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
        // sciprint("*****Output update block [%d] Time [%f] \n",get_block_number(), get_scicos_time());
        if ((structFmu = (workFMU*)(*block->work)) == NULL)
        {
            return;
        }
        fmu = structFmu->fmuCS1;
        time = get_scicos_time();

        dt = get_scicos_time() - structFmu->data.coSimulationV1.oldTime;
        // save previous time
        structFmu->data.coSimulationV1.oldTime = get_scicos_time();

        if ((dt == 0 || dt < 0) && time != 0)
        {
            updateStep = fmiFalse;
        }
        else if (time == 0)
        {
            step = get_scicos_time();
        }
        else
        {
            step = dt;
        }

        if (updateStep)
        {
            if (fmiSet_values(block, flag, 0, sciInputTypes, &block->insz[2*block->nin], inpRefsSize, sciInputRefs, block->inptr) > fmiWarning)
            {
                free_instance(block, -3, "fmiSet_values");
                return;
            }

            // do step
            status = fmu->fmiDoStep((structFmu->instance), time - step, step, fmiTrue);
            if (status > fmiDiscard)
            {
                free_instance(block, -3, "fmiDoStep");
                return;
            }
            
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
        // sciprint("*****Ending of block [%d] Time [%f] \n", get_block_number(), get_scicos_time());
        if ((structFmu = (workFMU*)(*block->work)) == NULL)
        {
            return;
        }
        fmu = structFmu->fmuCS1;

        // terminate simulation
        status = fmu->fmiTerminateSlave(structFmu->instance);
        if (status > fmiWarning)
        {
            free_instance(block, -3, "fmiTerminateSlave");
            return;
        }

        // freeing of instance
        free_instance(block, 0, "fmu_simulate_cs");
    }
}
