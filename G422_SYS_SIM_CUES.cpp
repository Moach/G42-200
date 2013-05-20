
//
//

#include "G422.h"
#include "G422_MDL_EXT.h"
//









void G422::cueSystems(int sysID, int cue, int op, void *ctx)
{
	//
	switch (sysID)
	{
		case G422_SYSID_RCOMS:
			//
			//
			switch (cue)
			{
				
				case RCOMS::CUE_OMS_STBY:
					//
					if (op)
						rcoms.state = (rcoms.state == RCOMS::SYS_RCSTBY)?  RCOMS::SYS_ALLSTBY : RCOMS::SYS_OMSTBY;
					else
						rcoms.state = (rcoms.state == RCOMS::SYS_ALLSTBY)?  RCOMS::SYS_RCSTBY : RCOMS::SYS_OFF;
				return;
				
				case RCOMS::CUE_RCS_STBY:
				
					if (op)
					{
						rcoms.state = (rcoms.state == RCOMS::SYS_OMSTBY)?  RCOMS::SYS_ALLSTBY : RCOMS::SYS_RCSTBY;
						
						//
						CreateThrusterGroup (th_rcs_pitchUp, 2, THGROUP_ATT_PITCHUP);
						CreateThrusterGroup (th_rcs_pitchDn, 2, THGROUP_ATT_PITCHDOWN);
						CreateThrusterGroup (th_rcs_vertUp,  2, THGROUP_ATT_UP);
						CreateThrusterGroup (th_rcs_vertDown,2, THGROUP_ATT_DOWN);
						//
						CreateThrusterGroup (th_rcs_yawLeft,  2,  THGROUP_ATT_YAWLEFT);
						CreateThrusterGroup (th_rcs_yawRight, 2,  THGROUP_ATT_YAWRIGHT);
						CreateThrusterGroup (th_rcs_horLeft,  2,  THGROUP_ATT_LEFT);
						CreateThrusterGroup (th_rcs_horRight, 2,  THGROUP_ATT_RIGHT);
						//
						CreateThrusterGroup (th_rcs_fwd,  1, THGROUP_ATT_FORWARD);
						CreateThrusterGroup (th_rcs_back, 1, THGROUP_ATT_BACK);
						//
						CreateThrusterGroup (th_rcs_bankRight, 2, THGROUP_ATT_BANKLEFT); // orbiter banking seems backwards... :P
						CreateThrusterGroup (th_rcs_bankLeft,  2, THGROUP_ATT_BANKRIGHT);// notice the switch...
					
						
					}else
					{
						rcoms.state = (rcoms.state == RCOMS::SYS_ALLSTBY)?  RCOMS::SYS_OMSTBY : RCOMS::SYS_OFF;
						//
						DelThrusterGroup(THGROUP_ATT_PITCHUP);
						DelThrusterGroup(THGROUP_ATT_PITCHDOWN);
						DelThrusterGroup(THGROUP_ATT_UP);
						DelThrusterGroup(THGROUP_ATT_DOWN);
						//
						DelThrusterGroup(THGROUP_ATT_YAWLEFT);
						DelThrusterGroup(THGROUP_ATT_YAWRIGHT);
						DelThrusterGroup(THGROUP_ATT_LEFT);
						DelThrusterGroup(THGROUP_ATT_RIGHT);
						//
						DelThrusterGroup(THGROUP_ATT_FORWARD);
						DelThrusterGroup(THGROUP_ATT_BACK);
						//
						DelThrusterGroup(THGROUP_ATT_BANKLEFT); 
						DelThrusterGroup(THGROUP_ATT_BANKRIGHT);
					}	
				return;
				
				case RCOMS::CUE_RCS_OPCL:
				return;
			}
		
		return;
		
		case G422_SYSID_PKBRK:
			
			switch(op)
			{
				default:
					//  toggle
					if (prk_brake_mode) 
					return cueSystems(sysID,0,G422_SYSOP_DISABLE);
					
				case G422_SYSOP_ENABLE:
					
					prk_brake_mode = 1;
					SetWheelbrakeLevel(1.0);
						
				break;
				
			
				case G422_SYSOP_DISABLE:
				
					prk_brake_mode = 0;
					SetWheelbrakeLevel(0.0);
					
				break;
			}
			
			
			 
		return;
		
		
		
		default:
		return;
	}
}





void G422::simEngines(double &dT, RCOMS &eng)
{
	if (eng.state == RCOMS::SYS_INOP) return;
	//


	switch (eng.state)
	{

	case RCOMS::SYS_ALLSTBY:
	case RCOMS::SYS_OMSTBY:
		//
		eng.omsThr = (thr_authority)? GetEngineLevel(ENGINE_MAIN) : 0.0;
		//

		SetThrusterLevel(oms_thgr[0], eng.omsThr);
		SetThrusterLevel(oms_thgr[1], eng.omsThr);
		PlayVesselWave3(SFXID, SFX_OMSBURN, NOLOOP, int(225 * eng.omsThr));

		if (eng.state == RCOMS::SYS_OMSTBY) return;
	case RCOMS::SYS_RCSTBY:
		//


		return;


	}
}





