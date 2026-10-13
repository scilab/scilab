//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Vladislav Trubkin
// Copyright (C) - 2017 - ESI Group - Clement DAVID
// Copyright (C) - 2024 - Dassault Systèmes - Clement DAVID
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

    SCICOS_BLOCKS_IMPEXP void fmu2_simulate_cs(scicos_block *block, scicos_flag flag);
}

SCICOS_BLOCKS_IMPEXP void fmu2_simulate_cs(scicos_block *block, scicos_flag flag)
{
    char *pathName = NULL, *resourceLocation = NULL, *identName = NULL, *guid = NULL;
    int *sciInputTypes = NULL, *sciOutputTypes = NULL;
    fmi2Status status;
    // main structure
    workFMU *structFmu;
    Fmu2CoSimulation *fmu;
    fmi2CallbackFunctions functions { fmu2_callback_logger, calloc, free, NULL, block};
    double time = 0, dt = 0;
    uint32_t *sciInputRefs = NULL, *sciOutputRefs = NULL;
    size_t outRefsSize, inpRefsSize;
    fmi2Boolean loggingOn = fmi2False;
    fmiReal tStart = 0, tStop = 0;
    fmiBoolean StopTimeDefined = fmiFalse;

    // return if an fmu has not been loaded
    if (block->nopar == 0)
    {
        return;
    }

    // settings
    pathName = Getint8OparPtrs(block, 1); // path to the lib
    resourceLocation = Getint8OparPtrs(block, 10); // fmuResourceLocation
    identName = Getint8OparPtrs(block, 11); // identifier
    guid = Getint8OparPtrs(block, 12); // guid
    loggingOn = *Getint8OparPtrs(block, 13); // logger

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

        // memory allocation for the main structure
        if ((*(block->work) = (workFMU*)scicos_malloc(sizeof(workFMU))) == NULL)
        {
            set_block_error(-16); // memory allocation error
            *(block->work) = NULL;
            return;
        }
        structFmu = (workFMU*)(*block->work);
        // clear the structFmu->fmuCS2 and structFmu->data
        memset(structFmu, 0x0, sizeof(workFMU));

        // load library and create an instance
        structFmu->fmuCS2 = new Fmu2CoSimulation();
        fmu = structFmu->fmuCS2;
        if (fmu->create(pathName, identName) == false)
        {
            //sciprint(fmu_wrap_error(), "fmu_simulate_cs");
            free_instance(block, -3, "Fmu2CoSimulation"); // internal error
            return;
        }
        // set fmiType
        structFmu->type = workFMU::CoSimulation;
        structFmu->version = workFMU::fmiV2;

        // instantiate slave
        structFmu->instance = fmu->fmi2Instantiate(identName, fmi2CoSimulation, guid, resourceLocation, &functions, fmi2True, loggingOn);
        if (structFmu->instance == NULL)
        {
            free_instance(block, -3, "fmi2Instantiate");
            return;
        }
        structFmu->data.coSimulationV2.state = structFmu->data.coSimulationV2.Instantiated;
        // set debug logging
        if (loggingOn)
        {
            status = fmu->fmi2SetDebugLogging(structFmu->instance, loggingOn, 0, NULL);
            if (status > fmi2Warning)
            {
                free_instance(block, -3, "fmi2SetDebugLogging");
                return;
            }
        }

        // setup experiment
        tStart = get_scicos_time();
        // StopTimeDefined == fmiFalse, tStop == 0
        status = fmu->fmi2SetupExperiment(structFmu->instance, fmiFalse, 0, tStart, StopTimeDefined, tStop);
        if (status > 1)
        {
            free_instance(block, -3, "fmi2SetupExperiment");
            return;
        }

        // set start values for variables with initial="exact" (as default or preset)
        status = (fmi2Status) fmiSet_parameters(block, flag);
        if(status > 1)
        {
            free_instance(block, -3, "fmiSet_parameters");
            return;
        }

        // enter initialization mode
        status = fmu->fmi2EnterInitializationMode(structFmu->instance);
        if (status > 1)
        {
            free_instance(block, -3, "fmi2EnterInitializationMode");
            return;
        }
        structFmu->data.coSimulationV2.state = structFmu->data.coSimulationV2.InitializationMode;
    }
    
    // synchronize inputs, outputs
    if (flag == Initialization || flag == ReInitialization)
    {    
        // TODO: use the start value from the xml file, this will fix the valid enumeration value problem

        // set inputs
        if (fmiSet_values(block, flag, 0, sciInputTypes, &block->insz[2*block->nin], inpRefsSize, sciInputRefs, block->inptr) > fmi2Warning)
        {
            free_instance(block, -3, "fmiSet_values");
            return;
        }

        // set outputs
        if (fmiGet_values(block, sciOutputTypes, &block->outsz[2*block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmi2Warning)
        {
            free_instance(block, -3, "fmiGet_values");
            return;
        }
    }

    if (flag == OutputUpdate)
    {
        // sciprint("\n");
        // sciprint("*****Output update block [%d] Time [%f] \n",get_block_number(), get_scicos_time());
        if ((structFmu = (workFMU*)(*block->work)) == NULL)
        {
            return;
        }
        fmu = structFmu->fmuCS2;

        if (structFmu->data.coSimulationV2.state == structFmu->data.coSimulationV2.InitializationMode)
        {
            // Exit initialization mode after ReInitialization fix-point iteration
            status = fmu->fmi2ExitInitializationMode(structFmu->instance);
            if (status > 1)
            {
                free_instance(block, -3, "fmi2ExitInitializationMode");
                return;
            }
            structFmu->data.coSimulationV2.state = structFmu->data.coSimulationV2.StepCompleted;
        }
        time = get_scicos_time();
        dt = time - structFmu->data.coSimulationV2.oldTime;

        if (dt > 0)
        {
            // copy Scicos inputs to fmiSet variables
            if (fmiSet_values(block, flag, 0, sciInputTypes, &block->insz[2*block->nin], inpRefsSize, sciInputRefs, block->inptr) > fmi2Warning)
            {
                free_instance(block, -3, "fmiSet_values");
                return;
            }

            // do step
            status = fmu->fmi2DoStep((structFmu->instance), structFmu->data.coSimulationV2.oldTime, dt, fmiTrue);
            if (status > fmi2Discard)
            {
                return;
            }
            structFmu->data.coSimulationV2.oldTime = time;

            // copy fmiGet variables to Scicos outputs
            if (fmiGet_values(block, sciOutputTypes, &block->outsz[2*block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmi2Warning)
            {
                return;
            }
        }
    }

    if (flag == Ending)
    {
        // sciprint("\n");
        // sciprint("*****Ending of block [%d] Time [%f] \n", get_block_number(), get_scicos_time());
        if ((structFmu = (workFMU*)(*block->work)) == NULL)
        {
            return;
        }
        fmu = structFmu->fmuCS2;

        if (structFmu->data.coSimulationV2.state > structFmu->data.coSimulationV2.InitializationMode)
        {
            // copy fmiGet variables to Scicos outputs
            if (fmiGet_values(block, sciOutputTypes, &block->outsz[2*block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmi2Warning)
            {
                return;
            }

            // terminate simulation
            status = fmu->fmi2Terminate(structFmu->instance);
            if (status > fmi2Warning)
            {
                free_instance(block, -3, "fmi2Terminate");
                return;
            }
        }

        // freeing of instance
        fmu->fmi2FreeInstance(structFmu->instance);
        free_instance(block, 0, "fmu_simulate_cs");
    }
}
