

#include "G422.h"
#include "G422_DVC.h"
#include "G422_MDL_DVC.h"

//
//



// this helps make things less ugly
//
inline bool CFGCheckTag(char* line, const char* tag, int *cfgData)
{
	int len = strlen(tag);
	if (strnicmp (line, tag, len)) return false;

	sscanf (line+len, "%d", cfgData);
	return true;
}
//





void G422::clbkLoadStateEx (FILEHANDLE scn, void *status)
{
	char *lineChars;
	int cfgLoadInt;
	
	
	while (oapiReadScenario_nextline (scn, lineChars)) {


			  if (CFGCheckTag(lineChars, "GEARS",       &cfgLoadInt))   landingGears->loadCfgState(cfgLoadInt);
		else if (CFGCheckTag(lineChars, "CNRDS",       &cfgLoadInt))       canards->loadCfgState(cfgLoadInt);
		else if (CFGCheckTag(lineChars, "VISOR",       &cfgLoadInt))         visor->loadCfgState(cfgLoadInt);
		else if (CFGCheckTag(lineChars, "INLET_MAIN", &cfgLoadInt))     inltDoors->loadCfgState(cfgLoadInt);
		else if (CFGCheckTag(lineChars, "INLET_RAMX", &cfgLoadInt))     ramxDoors->loadCfgState(cfgLoadInt);
		else if (CFGCheckTag(lineChars, "WING_WVRD", &cfgLoadInt))    wingTipWvrd->loadCfgState(cfgLoadInt);
		else if (CFGCheckTag(lineChars, "WING_FTHR", &cfgLoadInt))    wingTipFthr->loadCfgState(cfgLoadInt);
		else if (CFGCheckTag(lineChars, "CGO_BAY",   &cfgLoadInt))       bayDoors->loadCfgState(cfgLoadInt);
		else if (CFGCheckTag(lineChars, "RCSDOORS",  &cfgLoadInt))           rcs->loadCfgState(cfgLoadInt);
		
		else if (CFGCheckTag(lineChars, "MAIN_ENG_MD", &cfgLoadInt))  main_eng_mode = cfgLoadInt;
		else if (CFGCheckTag(lineChars, "BURNER_TGGL", &cfgLoadInt))  burner_toggle = cfgLoadInt;
		else if (CFGCheckTag(lineChars, "RAMX_ENG_MD", &cfgLoadInt)) ramcaster_mode = cfgLoadInt;
		else if (CFGCheckTag(lineChars, "THR_AUTH_MD", &cfgLoadInt))  thr_authority = cfgLoadInt;		
		
		else if (!strnicmp (lineChars, "VC_SWS", 6))
		{
			for (int i =0; i< VC_SWITCH_COUNT; ++i)
			{

				VCSwitches[i].setLoadState(lineChars[i+7], this);
			}
			
		}
		
		else if (!strnicmp (lineChars, "ENG_MAIN_", 9))
		{
			 if (lineChars[9] == 'L') // load state for left main engine
			 {
				sscanf(lineChars+10, "%d%d%lf%lf%lf%lf%lf",
					&engMain_L.state, &engMain_L.feed, &engMain_L.thr, &engMain_L.genPct,
					&engMain_L.epr, &engMain_L.fuelPrs, &engMain_L.oxyPrs);
				
			 } else // load state for right  main engine
			 {
				sscanf(lineChars+10, "%d%d%lf%lf%lf%lf%lf",
					&engMain_R.state, &engMain_R.feed, &engMain_R.thr, &engMain_R.genPct,
					&engMain_R.epr, &engMain_R.fuelPrs, &engMain_R.oxyPrs);

				
			 }
		}
		else if (!strnicmp (lineChars, "ENG_RAMX", 8))
		{
			sscanf(lineChars+8, "%d%d%lf%lf%lf", 
				&engRamx.state, &engRamx.feed, &engRamx.thr, &engRamx.epr, &engRamx.fuelPrs); 
			//cueEngines(engRamx, (RAMCASTER::SIMSTATE) engRamx.state);
		}
		else if (!strnicmp (lineChars, "ENG_APU",7))
		{
			sscanf(lineChars+7, "%d%d%lf%lf%lf", 
				&apu.state, &apu.feed, &apu.pwrPct, &apu.fuelPrs, &apu.fuelFlow); 
		}
		else if (!strnicmp (lineChars, "VC_EICAS",8))
		{
			sscanf(lineChars+8, "%d%d%d%d%d%d", 
				VC_eicas_screens+5, VC_eicas_screens+4, VC_eicas_screens, VC_eicas_screens+1, VC_eicas_screens+2, VC_eicas_screens+3); 
		}
		else 
			ParseScenarioLineEx (lineChars, status);

	}

	//oapiVCTriggerRedrawArea(-1, VC_AREA_MFDKEYS);

}



void G422::clbkSaveState (FILEHANDLE scn)
{
	VESSEL2::clbkSaveState(scn);
	//
	
	//
	// alright, first the moving parts... those should be mostly readable...
	
	//
	oapiWriteScenario_int(scn, "GEARS", landingGears->getCfgState());
	oapiWriteScenario_int(scn, "CNRDS",      canards->getCfgState());
	oapiWriteScenario_int(scn, "VISOR",        visor->getCfgState());
	oapiWriteScenario_int(scn, "RCSDOORS",      rcs->getCfgState());
	
	oapiWriteScenario_int(scn, "INLET_MAIN",        inltDoors->getCfgState());
	oapiWriteScenario_int(scn, "INLET_RAMX",        ramxDoors->getCfgState());
	
	oapiWriteScenario_int(scn, "WING_WVRD",        wingTipWvrd->getCfgState());
	oapiWriteScenario_int(scn, "WING_FTHR",        wingTipFthr->getCfgState());
	
	oapiWriteScenario_int(scn, "CGO_BAY",   bayDoors->getCfgState());
	
	
	// now, simulation things....
	oapiWriteScenario_int(scn, "MAIN_ENG_MD",  main_eng_mode);
	oapiWriteScenario_int(scn, "BURNER_TGGL",  burner_toggle);
	oapiWriteScenario_int(scn, "RAMX_ENG_MD", ramcaster_mode);
	oapiWriteScenario_int(scn, "THR_AUTH_MD",  thr_authority);
	
	// engines crap...
	
	char scnData[256]; 
	
	sprintf(scnData, "%d %d %0.4f %0.4f %0.4f %0.4f %0.4f", 
	 engMain_L.state, engMain_L.feed, engMain_L.thr, engMain_L.genPct, engMain_L.epr, engMain_L.fuelPrs, engMain_L.oxyPrs); 
	oapiWriteScenario_string(scn, "ENG_MAIN_L", scnData); // left main engine
	
	sprintf(scnData, "%d %d %0.4f %0.4f %0.4f %0.4f %0.4f",
	 engMain_R.state, engMain_R.feed, engMain_R.thr, engMain_R.genPct, engMain_R.epr, engMain_R.fuelPrs, engMain_R.oxyPrs); 
	oapiWriteScenario_string(scn, "ENG_MAIN_R", scnData); // right main engine
	
	sprintf(scnData, "%d %d %0.4f %0.4f %0.4f", 
	 engRamx.state, engRamx.feed, engRamx.thr, engRamx.epr, engRamx.fuelPrs); 
	oapiWriteScenario_string(scn, "ENG_RAMX", scnData); // ramcaster
	
	
	sprintf(scnData, "%d %d %0.4f %0.4f %0.4f", apu.state, apu.feed, apu.pwrPct, apu.fuelPrs, apu.fuelFlow); 
	oapiWriteScenario_string(scn, "ENG_APU", scnData); // APU

	// ok... now onto VC stuff
	//
	
	sprintf(scnData, "%d %d %d %d %d %d", 
		VC_eicas_screens[5], VC_eicas_screens[4], VC_eicas_screens[0], VC_eicas_screens[1], VC_eicas_screens[2], VC_eicas_screens[3]); 
	oapiWriteScenario_string(scn, "VC_EICAS", scnData); // EICAS SCREENS
	
	
	
	// seat position
	//oapiWriteScenario_int(scn, "VC_SEAT", VC_PoV_station);
	
	
	// this might come out a little unreadable now... but we gotta save the state for ALL those switches
	//
	
	char vcSwsState[VC_SWITCH_COUNT+1];
	for (int i = 0; i< VC_SWITCH_COUNT; ++i)
	{
		
		vcSwsState[i] = VCSwitches[i].getSaveState();
	}
	vcSwsState[VC_SWITCH_COUNT] = '\x00'; // set null terminator
	//
	oapiWriteScenario_string(scn, "VC_SWS", vcSwsState);
	
	
	// ok, i think that's it for now...
	
}





int MovingPart::getCfgState()
{

	if ((mp_status ==  MP_REVERSING || mp_status == MP_LOW_DETENT))
	{
		return 1;
	}
	if((mp_status == MP_MOVING || mp_status == MP_HI_DETENT))
	{
		return 2;
	}

}



void MovingPart::loadCfgState(int state)
{

	if (state == 2)
	{
		mp_status = MP_HI_DETENT;
		pos = 1.0;
		vsl->SetAnimation (anim_idx, pos);
		return;
	}

	if (state == 1)
	{
		mp_status = MP_LOW_DETENT;
		pos = 0.0;
		vsl->SetAnimation (anim_idx, pos);
		return;
	}
	
	sysReset = false;
	vsl->clbkGeneric(VMSG_MPSTOP, sysID, this);
}



char VCSwitch::getSaveState()
{
	switch (pos)
	{
		case SW3_DOWN: return '-';
		case SW3_MID:  return '=';
		case SW3_UP:   return '+';
		case SW2_UP:   return '^';
		case SW2_DOWN: return 'v';
	}
	
}

void VCSwitch::setLoadState(char &cs, VESSEL3* vsl)
{
	switch (cs)
	{
		case '-': return setPos(SW3_DOWN, vsl);
		case '=': return setPos(SW3_MID,  vsl);
		case '+': return setPos(SW3_UP,   vsl);
		case '^': return setPos(SW2_UP,   vsl);
		case 'v': return setPos(SW2_DOWN, vsl);
	}
	
}