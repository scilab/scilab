#include "CruiseControl_CruiseControl_interface.h"
const int  rt_version = Srtv62;

const char* _SCSIM_CheckSum = "650af783d6fd2c034c866c25779cfe43";
const char* _SCSIM_SmuTypesCheckSum = "28e18c1b393c1cd143584174bcc1ba87";

/*******************************
 * Validity
 *******************************/
int valid(void * pHandle) {
	return 1;
}
int notvalid(void * pHandle) {
	return 0;
}

/*******************************
 * Simulation context
 *******************************/
inC_CruiseControl_CruiseControl inputs_ctx;
static inC_CruiseControl_CruiseControl inputs_ctx_restore;
static inC_CruiseControl_CruiseControl inputs_ctx_execute;
outC_CruiseControl_CruiseControl outputs_ctx;
static outC_CruiseControl_CruiseControl outputs_ctx_restore;

/* separate_io: inputs instanciation */

/* separate_io: outputs instanciation */

static void _SCSIM_RestoreInterface(void) {
	inputs_ctx.On = inputs_ctx_restore.On;
	inputs_ctx.Off = inputs_ctx_restore.Off;
	inputs_ctx.Resume = inputs_ctx_restore.Resume;
	inputs_ctx.Set = inputs_ctx_restore.Set;
	inputs_ctx.QuickAccel = inputs_ctx_restore.QuickAccel;
	inputs_ctx.QuickDecel = inputs_ctx_restore.QuickDecel;
	inputs_ctx.Accel = inputs_ctx_restore.Accel;
	inputs_ctx.Brake = inputs_ctx_restore.Brake;
	inputs_ctx.Speed = inputs_ctx_restore.Speed;
	outputs_ctx = outputs_ctx_restore;

	/* separate_io: outputs restore */
}

static void _SCSIM_ExecuteInterface(void) {
	pSimulator->m_pfnAcquireValueMutex(pSimulator);
	inputs_ctx_execute.On = inputs_ctx.On;
	inputs_ctx_execute.Off = inputs_ctx.Off;
	inputs_ctx_execute.Resume = inputs_ctx.Resume;
	inputs_ctx_execute.Set = inputs_ctx.Set;
	inputs_ctx_execute.QuickAccel = inputs_ctx.QuickAccel;
	inputs_ctx_execute.QuickDecel = inputs_ctx.QuickDecel;
	inputs_ctx_execute.Accel = inputs_ctx.Accel;
	inputs_ctx_execute.Brake = inputs_ctx.Brake;
	inputs_ctx_execute.Speed = inputs_ctx.Speed;
	pSimulator->m_pfnReleaseValueMutex(pSimulator);
}

extern void _SCSIM_Mapping_Create();

/*******************************
 * Cyclic function encapsulation
 *******************************/
void SimInit(void) {
	/* Context initialization */
	_SCSIM_Mapping_Create();
	_SCSIM_RestoreInterface();
#ifdef EXTENDED_SIM
	BeforeSimInit();
#endif /* EXTENDED_SIM */
	CruiseControl_reset_CruiseControl(&outputs_ctx);
#ifdef EXTENDED_SIM
	AfterSimInit();
#endif /* EXTENDED_SIM */
}

#ifdef EXTENDED_SIM
int GraphicalInputsConnected = 1;
#endif /* EXTENDED_SIM */
int SimStep(void) {
#ifdef EXTENDED_SIM
	if (GraphicalInputsConnected)
		BeforeSimStep();
#endif /* EXTENDED_SIM */
	_SCSIM_ExecuteInterface();
	CruiseControl_CruiseControl(&inputs_ctx_execute, &outputs_ctx);
#ifdef EXTENDED_SIM
	AfterSimStep();
#endif /* EXTENDED_SIM */
	return 1;
}

void SimStop(void) {
#ifdef EXTENDED_SIM
	ExtendedSimStop();
#endif /* EXTENDED_SIM */
}

int SsmGetDumpSize(void) {
	int nSize = 0;
	nSize += sizeof(inC_CruiseControl_CruiseControl);

/* separate_io: add (not in ctx) inputs size */

/* separate_io: add (not in ctx) outputs size */
	nSize += sizeof(outC_CruiseControl_CruiseControl);
#ifdef EXTENDED_SIM
	nSize += ExtendedGetDumpSize();
#endif /* EXTENDED_SIM */
	return nSize;
}

void SsmGatherDumpData(char * pData) {
	char* pCurrent = pData;
	memcpy(pCurrent, &inputs_ctx, sizeof(inC_CruiseControl_CruiseControl));
	pCurrent += sizeof(inC_CruiseControl_CruiseControl);

	/* separate_io: dump (not in ctx) inputs */

	/* separate_io: dump (not in ctx) outputs */
	memcpy(pCurrent, &outputs_ctx, sizeof(outC_CruiseControl_CruiseControl));
	pCurrent += sizeof(outC_CruiseControl_CruiseControl);
#ifdef EXTENDED_SIM
	ExtendedGatherDumpData(pCurrent);
#endif /* EXTENDED_SIM */
}

void SsmRestoreDumpData(const char * pData) {
	const char* pCurrent = pData;
	memcpy(&inputs_ctx, pCurrent, sizeof(inC_CruiseControl_CruiseControl));
	pCurrent += sizeof(inC_CruiseControl_CruiseControl);

	/* separate_io: restore (not in ctx) inputs */

	/* separate_io: restore (not in ctx) outputs */
	memcpy(&outputs_ctx, pCurrent, sizeof(outC_CruiseControl_CruiseControl));
	pCurrent += sizeof(outC_CruiseControl_CruiseControl);
#ifdef EXTENDED_SIM
	ExtendedRestoreDumpData(pCurrent);
#endif /* EXTENDED_SIM */
}

const char * SsmGetCheckSum() {
	return _SCSIM_CheckSum;
}

const char * SsmGetSmuTypesCheckSum() {
	return _SCSIM_SmuTypesCheckSum;
}

void SsmUpdateValues(void) {
#ifdef EXTENDED_SIM
	UpdateValues();
#endif /* EXTENDED_SIM */
}

void SsmConnectExternalInputs(int bConnect) {
#ifdef EXTENDED_SIM
	GraphicalInputsConnected = bConnect;
#endif /* EXTENDED_SIM */
}

