

//
//
#include "G422.h"
#include "G422_MDL_DVC.h"
#include "G422_DVC.h"



bool G422::clbkVCMouseEvent(int id, int ev, VECTOR3 &p)
{
	
	int ctrlSet = HIWORD(id);
	int ctrlID  = LOWORD(id);
	
	//static const int mfdInputLineup[4] = {MFD_LEFT, MFD_RIGHT, MFD_USER1, MFD_USER2};
	
	switch(ctrlSet)
	{
		case VC_CTRLSET_MFDK:
		{
			// 
			bool op = oapiProcessMFDButton(ctrlID / 12, ctrlID % 12, ev);
			static bool hold = false;
			if (!hold)
			{
				PlayVesselWave3(SFXID, SFX_VC_POP);
				//PlayVesselWave3(SFXID, op? SFX_VC_BEEP : SFX_VC_POP);
				hold = true;
			} else
			{
				if (ev & PANEL_MOUSE_UP)
				{ 
					hold = false;
				//	
				}
			}
			
		}
		return true;
		
		
		case VC_CTRLSET_MFDC:
			//
			
			switch(ctrlID % 3)
			{
				case 0:  // select keys...
					PlayVesselWave3(SFXID, SFX_VC_POP);
					oapiSendMFDKey(ctrlID / 3, OAPI_KEY_F1);
				return true;
				
				case 1: // power buttons...
					PlayVesselWave3(SFXID, SFX_VC_POP);
					oapiSendMFDKey(ctrlID / 3, OAPI_KEY_ESCAPE);
				return true;
				
				case 2: // menu keys...
					PlayVesselWave3(SFXID, SFX_VC_POP);
					oapiSendMFDKey(ctrlID / 3, OAPI_KEY_GRAVE);
				return true;
			}
			
			
			
		
		case VC_CTRLSET_EICAS_C1:
		case VC_CTRLSET_EICAS_C2:
		{	
			UINT btn = (UINT)(p.x*6.0) + ((p.y > .5)? 7 : 1);
			
			PlayVesselWave3(SFXID, SFX_VC_POP);
			selEicasMode((ctrlSet == VC_CTRLSET_EICAS_C1)? 0 : 1, btn);
			
			return true;
		}	
		
		
		case VC_CTRLSET_EICAS_P1L:
		case VC_CTRLSET_EICAS_P1R:
		{
			UINT btn = (UINT)(p.y*6.0) + ((ctrlSet == VC_CTRLSET_EICAS_P1L)? 1 : 7);
			
			PlayVesselWave3(SFXID, SFX_VC_POP);
			selEicasMode(2, btn);
		
			return true;
		
		}
		case VC_CTRLSET_EICAS_P2L:
		case VC_CTRLSET_EICAS_P2R:
		{
			UINT btn = (UINT)(p.y*6.0) + ((ctrlSet == VC_CTRLSET_EICAS_P2L)? 1 : 7);

			PlayVesselWave3(SFXID, SFX_VC_POP);
			selEicasMode(3, btn);

			return true;

		}

		case VC_CTRLSET_EICAS_E1L:
		case VC_CTRLSET_EICAS_E1R:
		{
			UINT btn = (UINT)(p.y*6.0) + ((ctrlSet == VC_CTRLSET_EICAS_E1R)? 1 : 7);

			PlayVesselWave3(SFXID, SFX_VC_POP);
			selEicasMode(4, btn);

			return true;

		}
		case VC_CTRLSET_EICAS_E2L:
		case VC_CTRLSET_EICAS_E2R:
		{
			UINT btn = (UINT)(p.y*6.0) + ((ctrlSet == VC_CTRLSET_EICAS_E2R)? 1 : 7);

			PlayVesselWave3(SFXID, SFX_VC_POP);
			selEicasMode(5, btn);

			return true;

		}
	
		
		
		case VC_CTRLSET_EICAS_PWR:
		{
			PlayVesselWave3(SFXID, SFX_VC_POP);
			selEicasMode(ctrlID, (ev == PANEL_MOUSE_LBDOWN)? abs(VC_eicas_screens[ctrlID]) : -abs(VC_eicas_screens[ctrlID]));
			return true;
			
		}
		
		
		
		
		
		
		
		
			
			
		case VC_CTRLSET_SWITCHES:
		{
			//strcpy(oapiDebugString(), "tick...");
			//
			VCSwitch &sw = VCSwitches[ctrlID];
			
			if (ev & (PANEL_MOUSE_LBUP | PANEL_MOUSE_RBUP)) // spring-loaded switches revert when released
			{
				switch (sw.mgid)
				{
					case MGID_SW3_RAMX_IGN:
					case MGID_SW3_EMAIN_IGN:
					case MGID_SW3_APU_PACK_A:
					case MGID_SW3_APU_PACK_B:
						// the above switches are spring-loaded only on top position...
						if (sw.pos != SW3_UP) return false;
						
					case MGID_SW3_EMAIN_MODE:
					case MGID_SW3_RAMX_MODE:
						// 
						sw.setPos(SW3_MID, this);
						PlayVesselWave3(SFXID, SFX_VC_TICK);
					return true;
				}
				
				return false;
			}
			
			
			
			
			if( !sw.flick(ev == PANEL_MOUSE_LBDOWN, this) )
			{
				PlayVesselWave3(SFXID, SFX_VC_POP);
				return false;
			}
			
			
			PlayVesselWave3(SFXID, SFX_VC_FLICK);
			//sprintf(oapiDebugString(), "mesh ID = %d | ctrl ID = %d", sw.mgid, ctrlID);
			
			
		//	allSystemsReset = false;
			
			switch (sw.mgid)
			{
				// flight controls..
				//
					
				case MGID_SW3_GEAR:
					if (sw.pos == SW3_UP)   landingGears->toggle(false); // gears are backwards (false means retract)
					if (sw.pos == SW3_DOWN) landingGears->toggle(true);
				return true;
				
				case MGID_SW3_CANARD:
					if (sw.pos == SW3_UP)   canards->toggle(true);
					if (sw.pos == SW3_DOWN) canards->toggle(false);
				return true;
				
				case MGID_SW3_VISOR:
					if (sw.pos == SW3_UP)   visor->toggle(true);
					if (sw.pos == SW3_DOWN) visor->toggle(false);
				return true;
				
				case MGID_SW3_RCS:
					if (sw.pos == SW3_UP)   rcs->toggle(true);
					if (sw.pos == SW3_DOWN) rcs->toggle(false);
				return true;
				/*
				case MGID_SW3_BAY:
					if (sw.pos == SW3_UP)   bayDoors->toggle(true);
					if (sw.pos == SW3_DOWN) bayDoors->toggle(false);
				return true;
				*/
				
				// main engines...
				//
				
				case MGID_SW3_EMAIN_MODE: // mode switch
					if (sw.pos == SW3_UP)    { cueEngines(engMain_L, RT66::ENG_INLT_OPEN);    cueEngines(engMain_R, RT66::ENG_INLT_OPEN);   }//inltDoors->toggle(false);
					if (sw.pos == SW3_DOWN)  { cueEngines(engMain_L, RT66::ENG_INLT_CLSE);    cueEngines(engMain_R, RT66::ENG_INLT_CLSE);   }//inltDoors->toggle(true);
				return true;
				
				case MGID_SW3_EMAIN_REHEAT:
					if (sw.pos == SW2_UP) burner_toggle = 1;
					else				   burner_toggle = 0;
				return true;
				
				
				// this is a bit kludge-ish... the ideal thing would be to simulate turbine RPM or something
				// but for now this should kinda do the trick....
				//
				case MGID_SW3_EMAIN_IGN:
					//if (sw.pos == SW3_UP && main_eng_mode   == 0)	main_eng_mode = 1;
					//if (sw.pos == SW3_DOWN && main_eng_mode == 1) main_eng_mode = 0;
					if (sw.pos == SW3_UP)   { cueEngines(engMain_L, RT66::ENG_SPOOLUP);    cueEngines(engMain_R, RT66::ENG_SPOOLUP);   }
					if (sw.pos == SW3_DOWN) { cueEngines(engMain_L, RT66::ENG_SPOOLDOWN);  cueEngines(engMain_R, RT66::ENG_SPOOLDOWN); }
					
				return true;
				
				case MGID_SW3_EMAIN_BRNRIGN:
					if (sw.pos == SW3_UP  ) main_ign_sqnc = 2;
					if (sw.pos == SW3_MID ) main_ign_sqnc = 1;
					if (sw.pos == SW3_DOWN) main_ign_sqnc = 0;
				return true;
				
				
				case MGID_SW3_RAMX_IGN:
					if (sw.pos == SW3_UP)    cueEngines(engRamx, RAMCASTER::ENG_START);//ramcaster_mode = 1;
					if (sw.pos == SW3_DOWN)  cueEngines(engRamx, RAMCASTER::ENG_STOP );//ramcaster_mode = 0;
				return true;
				
				
				case MGID_SW3_RAMX_DOOR:
					if (sw.pos == SW3_UP)    ramxDoors->toggle(true);
					if (sw.pos == SW3_DOWN)  ramxDoors->toggle(false);
				return true;
				
				case MGID_SW3_RAMX_MODE:
					if (!ramcaster_mode) return true;  // off...
					//
					if (sw.pos == SW3_UP)    cueEngines(engRamx, RAMCASTER::ENG_RUNHI);
					if (sw.pos == SW3_DOWN)  cueEngines(engRamx, RAMCASTER::ENG_RUNLO);
				return true;
				

				case MGID_SW2_THR_AUTH:
					if (sw.pos == SW2_UP)    thr_authority = 0;
					if (sw.pos == SW2_DOWN)  thr_authority = 1;
				return true;
				
				
				
				case MGID_SW2_MSFEED_R:
					if (sw.pos == SW2_UP)    
					{
						engMain_R.feed |= RT66::FUEL_PUMP;
						PlayVesselWave3(SFXID, SFX_FUELPUMP_MAIN, LOOP);
					}
					if (sw.pos == SW2_DOWN)
					{
						engMain_R.feed &= ~RT66::FUEL_PUMP;
						if (VCSwitches[VC_swIndexByMGID[MGID_SW2_MSFEED_L]].pos == SW2_DOWN) 
							StopVesselWave3(SFXID, SFX_FUELPUMP_MAIN);
					}
				return true;	
				case MGID_SW2_MSFEED_L:
					if (sw.pos == SW2_UP)    
					{
						engMain_L.feed |= RT66::FUEL_PUMP;
						PlayVesselWave3(SFXID, SFX_FUELPUMP_MAIN, LOOP);
					}
					if (sw.pos == SW2_DOWN)
					{
						engMain_L.feed &= ~RT66::FUEL_PUMP;
						if (VCSwitches[VC_swIndexByMGID[MGID_SW2_MSFEED_R]].pos == SW2_DOWN) 
							StopVesselWave3(SFXID, SFX_FUELPUMP_MAIN);
					}
				return true;	


				case MGID_SW2_MSFEED_C:
					if (sw.pos == SW2_UP)    
					{
					//	engRamx.feed |= RAMCASTER::FUEL_PUMP;
						PlayVesselWave3(SFXID, SFX_FUELPUMP_RAMX, LOOP);
					}
					if (sw.pos == SW2_DOWN)
					{
					//	engRamx.feed &= ~RAMCASTER::FUEL_PUMP;
						StopVesselWave3(SFXID, SFX_FUELPUMP_RAMX);
					}
				return true;
				
				
				
				case MGID_SW3_OXYFEED_R:
					if (sw.pos == SW2_UP)    
					{
						engMain_R.feed |= RT66::OXY_PUMP;
						PlayVesselWave3(SFXID, SFX_FUELPUMP_OXY, LOOP);
					}
					if (sw.pos == SW2_DOWN)
					{
						engMain_R.feed &= ~RT66::OXY_PUMP;
						if (VCSwitches[VC_swIndexByMGID[MGID_SW3_OXYFEED_L]].pos == SW2_DOWN) 
							StopVesselWave3(SFXID, SFX_FUELPUMP_OXY);
					}
					return true;	
				case MGID_SW3_OXYFEED_L:
					if (sw.pos == SW2_UP)    
					{
						engMain_L.feed |= RT66::OXY_PUMP;
						PlayVesselWave3(SFXID, SFX_FUELPUMP_OXY, LOOP);
					}
					if (sw.pos == SW2_DOWN)
					{
						engMain_L.feed &= ~RT66::OXY_PUMP;
						if (VCSwitches[VC_swIndexByMGID[MGID_SW3_OXYFEED_R]].pos == SW2_DOWN) 
							StopVesselWave3(SFXID, SFX_FUELPUMP_OXY);
					}
					return true;	
				
				
				case MGID_SW2_SYSFEED_RCS:
					if (sw.pos == SW2_UP)    PlayVesselWave3(SFXID, SFX_FUELPUMP_APU, LOOP);
					if (sw.pos == SW2_DOWN)  StopVesselWave3(SFXID, SFX_FUELPUMP_APU);
				return true;	
				
				
				
				case MGID_SW3_APU_PACK_A:
				//case MGID_SW3_APU_PACK_B:
				
					if (sw.pos == SW3_UP)    cueEngines(apu, APU::ENG_START);
					if (sw.pos == SW3_DOWN)  cueEngines(apu, APU::ENG_STOP);
				return true;	
				
				
				case MGID_SW3_STBYIGN_OMS:
					if (sw.pos == SW2_UP)    cueSystems(G422_SYSID_RCOMS, RCOMS::CUE_OMS_STBY, 1);
					if (sw.pos == SW2_DOWN)  cueSystems(G422_SYSID_RCOMS, RCOMS::CUE_OMS_STBY, 0);
				return true;
				case MGID_SW3_STBYIGN_RCS:
					if (sw.pos == SW2_UP)    cueSystems(G422_SYSID_RCOMS, RCOMS::CUE_RCS_STBY, 1);
					if (sw.pos == SW2_DOWN)  cueSystems(G422_SYSID_RCOMS, RCOMS::CUE_RCS_STBY, 0);
				return true;
				
				
				
				case MGID_SW2_APU_RAMX:
					if (sw.pos == SW2_UP)    
					{
						engRamx.feed |= RAMCASTER::FUEL_PUMP;
					//	PlayVesselWave3(SFXID, SFX_FUELPUMP_RAMX, LOOP);
					}
					if (sw.pos == SW2_DOWN)
					{
						engRamx.feed &= ~RAMCASTER::FUEL_PUMP;
					//	StopVesselWave3(SFXID, SFX_FUELPUMP_RAMX);
					}
				
				
				return true;
				
				
				
				case MGID_SW2_LGT_NAV:
					if (sw.pos == SW2_UP)   nav_white.active = nav_red.active = nav_green.active = true;
					if (sw.pos == SW2_DOWN) nav_white.active = nav_red.active = nav_green.active = false;
				return true;	
				
				case MGID_SW2_LGT_STB:
					if (sw.pos == SW2_UP)   stb_t.active = stb_l.active = stb_r.active = true;
					if (sw.pos == SW2_DOWN) stb_t.active = stb_l.active = stb_r.active = false;
				return true;	
				
				case MGID_SW2_LGT_BCN:
					if (sw.pos == SW2_UP)   bcn1.active = bcn2.active = true;
					if (sw.pos == SW2_DOWN) bcn1.active = bcn2.active = false;
				return true;	
				
				
			}
			
			return true; // whatever else, just flick it! (faux switch)
		};
		
		
		
		case VC_CTRLSET_WPOSLVR:
			//	
			//
			if (ev == PANEL_MOUSE_LBDOWN)
			{
				if (wingPos < 1) 
					++wingPos;
				
			} else
			{
				if (wingPos > -1) 
					--wingPos;
			
			}
			
			PlayVesselWave3(SFXID, SFX_VC_CLAK);
			StopVesselWave3(SFXID, SFX_WINGMTR);
			//sprintf(oapiDebugString(), "wing-pos :: %d", wingPos);
			
			switch (wingPos)
			{
				case 0:
					wingTipWvrd->toggle(false);
					wingTipFthr->toggle(false);
					SetAnimation(an_dvc_wpos, 0.5);
					oapiVCSetAreaClickmode_Spherical((VC_CTRLSET_WPOSLVR << 16), V3_VC_WPOS_MID, .035);
					PlayVesselWave3(SFXID, SFX_WINGMTR);
				break;
				case -1:
					wingTipWvrd->toggle(true);
					SetAnimation(an_dvc_wpos, 0.0);
					oapiVCSetAreaClickmode_Spherical((VC_CTRLSET_WPOSLVR << 16), V3_VC_WPOS_AFT, .035);
					PlayVesselWave3(SFXID, SFX_WINGMTR);
				break;
				case 1:
					wingTipFthr->toggle(true);
					SetAnimation(an_dvc_wpos, 1.0);
					oapiVCSetAreaClickmode_Spherical((VC_CTRLSET_WPOSLVR << 16), V3_VC_WPOS_FWD, .035);
					PlayVesselWave3(SFXID, SFX_WINGMTR);
				break;
			}
			
				
		return true;
		
		case VC_CTRLSET_MOVESEAT:
			//
			clbkConsumeBufferedKey(OAPI_KEY_TAB, true, 0);
			
		return true;
		
		default:
		return false;
	}
	
}



// --------------------------------------------------------------
// Process direct key events
// --------------------------------------------------------------
int G422::clbkConsumeDirectKey (char *kstate)
{

	return 0;
}

// --------------------------------------------------------------
// Process buffered key events
// --------------------------------------------------------------

//
int G422::clbkConsumeBufferedKey (DWORD key, bool down, char *kstate)
{
	
	if (kstate)
	{
	
		//
		if (KEYMOD_SHIFT(kstate)) // SHIFT KEYS
		{
			
			if (!down) return 0;
			
			switch (key)
			{
				case OAPI_KEY_R:
				
					if (burner_toggle)
					{
						VCSwitches[VC_swIndexByMGID[MGID_SW3_EMAIN_REHEAT]].setPos(SW2_DOWN, this);
						burner_toggle = 0;
					} else
					{
						VCSwitches[VC_swIndexByMGID[MGID_SW3_EMAIN_REHEAT]].setPos(SW2_UP, this);
						burner_toggle = 1;
					}


				return 1;
			
			}
		
			return 0;
		}
		
		if (KEYMOD_CONTROL(kstate)) // CTRL KEYS
		{
			if (!down) return 0;
			
			switch (key)
			{
				case OAPI_KEY_PERIOD:
				case OAPI_KEY_COMMA:
//					cueSystems(G422_SYSID_PKBRK); // toggle
					
				return 1;
			}
			
			
			return 0;
		}
		
		if (KEYMOD_ALT(kstate)) // ALT KEYS
		{
		
		
		
			return 0;
		}
	
	}
	
	// no mod keys...
	//
	
	if (!down) return 0;
	
	
	switch (key)
	{

		case OAPI_KEY_G:
			landingGears->toggle();
			VCSwitches[VC_swIndexByMGID[MGID_SW3_GEAR]].setPos(landingGears->getToggleState()? SW3_DOWN : SW2_UP, this);
		return 1;

		case OAPI_KEY_V:
			visor->toggle();
			VCSwitches[VC_swIndexByMGID[MGID_SW3_VISOR]].setPos(visor->getToggleState()? SW2_UP : SW3_DOWN, this);
		return 1;

		case OAPI_KEY_N:
			canards->toggle();
			VCSwitches[VC_swIndexByMGID[MGID_SW3_CANARD]].setPos(canards->getToggleState()? SW2_UP : SW3_DOWN, this);
		return 1;
	
		
		case OAPI_KEY_B:
			bayDoors->toggle();
			
		return 1;
		/*
		case OAPI_KEY_W:
			if (!KEYMOD_SHIFT(kstate)) return 0;
			wingTipWvrd->toggle();
			
		return 1;
		*/
		
		case OAPI_KEY_TAB:
		{
			
			if (VC_PoV_station == 0)
			{
				VC_PoV_station = 1;  //											engineer side
				
				SetMeshVisibilityMode (mdlID_dvcR, MESHVIS_NEVER);	 
				SetMeshVisibilityMode (mdlID_dvcL, MESHVIS_VC );	
				
				//                   LEAN FORWARD (FMC)                     LEAN LEFT (WINDOW)                  LEAN RIGHT (OVERHEAD)
				SetCameraMovement( _V(0.1, -.15, 0.1), -12*RAD, -10*RAD,		_V(-0.15, -.02, 0), 35*RAD, 0*RAD,		_V(0.22, -0.17, .1), -15*RAD, 60*RAD);
				//
				SetCameraOffset (V3_VC_POV_FO);
				
				VECTOR3 vDir = _V(0.0, -.2, 1); normalise(vDir);
				SetCameraDefaultDirection(vDir);
				oapiCameraSetCockpitDir(0.0, -5*RAD);
				
				oapiVCSetAreaClickmode_Spherical((VC_CTRLSET_MOVESEAT << 16), _V( 1, 2.25,  38.7), .6);
				
			} else
			{
				VC_PoV_station = 0; //											 pilot side
				SetMeshVisibilityMode (mdlID_dvcR, MESHVIS_VC );	 
				SetMeshVisibilityMode (mdlID_dvcL, MESHVIS_NEVER );	

				SetCameraMovement( _V(0,-.05, 0.1), 0.0, -20*RAD,    	_V(-0.20, -0.1, 0), 15*RAD, 45*RAD,		_V(0.15, -.02, 0), -35*RAD, 0);
				//
				SetCameraOffset (V3_VC_POV_PILOT);
				
				oapiVCSetAreaClickmode_Spherical((VC_CTRLSET_MOVESEAT << 16), _V(-1, 2.25,  38.7), .6);
				
				VECTOR3 vDir = _V(0.0, -.15, 1); normalise(vDir);
				SetCameraDefaultDirection(vDir);
				oapiCameraSetCockpitDir(0.0, 0.0);
			};
			
			// force a ctrl+alt+down keypress so the view returns to "unleaned"
			/*
			keybd_event(VK_LCONTROL,0,0,0); keybd_event(VK_LMENU,0,0,0);
			keybd_event(VK_DOWN,0,0,0);     keybd_event(VK_DOWN,0,2,0);
			keybd_event(VK_LMENU,0,2,0);    keybd_event(VK_LCONTROL,0,2,0); */
		}
		
		break;
		
		case OAPI_KEY_MINUS:
		
		//	ShiftCG(_V(0.0, 0.0, -0.1));
			cgShiftRef += 0.1;
			EditAirfoil(wingLift, 0x01, _V(0.0, 0.0, cgShiftRef), NULL, 0,0,0);
		
		break;
		
		
		case OAPI_KEY_EQUALS:
		
		//	ShiftCG(_V(0.0, 0.0, 0.1));
			cgShiftRef -= 0.1;
			EditAirfoil(wingLift, 0x01, _V(0.0, 0.0, cgShiftRef), NULL, 0,0,0);
		
		break;
		
	
		default:
		return 0;
	}


	return 0;
}



