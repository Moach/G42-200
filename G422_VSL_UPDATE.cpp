

#include "G422.h"
#include "G422_DVC.h"
#include "G422_MDL_EXT.h"
#include "G422_MDL_DVC.h"



inline bool MovingPart::operate(double& dt)
{
	switch (mp_status)
	{
		case MP_MOVING:
			//strcpy(oapiDebugString(), "moving...");

			pos = pos + (rate * dt);

			if (pos >= 1.0)
			{
				pos = 1.0;
				mp_status = MP_HI_DETENT;
				vsl->clbkGeneric(VMSG_MPSTOP, sysID, this);
			}
			vsl->SetAnimation (anim_idx,  pos);

		return true;

		case MP_REVERSING:
			//strcpy(oapiDebugString(), "reversing...");
			pos = pos - (rate * dt);

			if (pos <= 0.0)
			{
				pos = 0.0;
				mp_status = MP_LOW_DETENT;
				vsl->clbkGeneric(VMSG_MPSTOP, sysID, this);
			}
			vsl->SetAnimation (anim_idx, pos);

		return true;
	}

	//
	return false;
}











VECTOR3 vF, vW, vA; // define some static stuff for use below...



//
void G422::clbkPreStep(double simt, double dT, double mjd)
{
	//
	//SetAnimation(anim_InletDoors, GetThrusterLevel (main_eng_arr[0])); // just checking...
	//SetPropellantMass(dummyFuel, 1);
	//double alt;
	//oapiGetAltitude(GetHandle(), &alt);

	/*
	if (!allSystemsReset) // no need to run up all systems every time, but they do need refreshing once in a while
	{
		allSystemsReset = true; // no need to refresh again, at first
		systemsReset();         // but something might need to call for another pass...
	}
	*/
	
	// get flight envelope coords for engine reference lookup table
	// 
	
	
	thrFX_X = GetMachNumber() * .05;
	thrFX_Y = 1.0 - (GetAltitude() * 12.5e-6);
	
	
	
	//
	//
	//
	//
	
	simEngines(dT, apu);
	//
	simEngines(dT, engMain_L);
	simEngines(dT, engMain_R);
	//
	simEngines(dT, engRamx);
	//	
	simEngines(dT, rcoms);
	
	
	
	if (engMain_L.state == RT66::SST_RUN_EXT)
	{
		// not quite sure how or why this works.... got to it by tweaking around with plot-o-matic...
		//
		double alt = GetAltitude() * .00001;
		fx_contrailLvl = max(0, ((alt-.028) * alt) * ((.18-alt)*300.0) * engMain_L.epr);
		fx_exhaustLvl = engMain_L.epr;
		
	} else
		fx_contrailLvl = 0.0;
		fx_exhaustLvl  = 0.0;
	
	
	
	SetAnimation(an_dvc_thr,  GetEngineLevel(ENGINE_MAIN));
	SetAnimation(an_dvc_ramx, GetEngineLevel(ENGINE_HOVER));
	
	//
	//if (GetADCtrlMode())
	//{
		SetAnimation(an_dvc_csPitch, (GetControlSurfaceLevel(AIRCTRL_ELEVATOR)* -.5) + .5);
		SetAnimation(an_dvc_csRoll,  (GetControlSurfaceLevel(AIRCTRL_AILERON) *  .5) + .5);		
	//} else
	/*{
		VECTOR3 rotCtrl;
		GetAttitudeRotLevel(rotCtrl);
		SetAnimation(an_dvc_csPitch, (rotCtrl.x * -.5) + .5);
		SetAnimation(an_dvc_csRoll,  (rotCtrl.z *  .5) + .5);
	}
	*/
	
	/*
	//
	
	//
	// ok, lets check that ADI ball thingy....
	
	MATRIX3 vslRot; GetRotationMatrix(vslRot);
	VECTOR3 vslVec; 

	static UINT ADI = MGID_ADI_BALL;
	
	MATRIX3 vslRot; 
	GetRotationMatrix(vslRot);
	
	VECTOR3 flightVec;
	oapiGetFocusAirspeedVector(flightVec);
	
	flightVec = mul(flightVec, vslRot);
	
	VECTOR3 adiSpinAxis;
	
		//MGROUP_ROTATE adi_spin(mdlID_dvc, &ADI, 1, V3)
		
	
*/

/*
	SetAnimation(an_dvc_adi_A, .5 +       (GetBank()*SCALAR_ANGLE));
	SetAnimation(an_dvc_adi_B, .5 + ( GetSlipAngle()*SCALAR_ANGLE));
 	SetAnimation(an_dvc_adi_C, .5 + (     GetPitch()*SCALAR_ANGLE));
	*/
	//
	
		
	
	SetAnimation(an_acs_cnrd, (canards->pos >= 1.0)? ((GetControlSurfaceLevel(AIRCTRL_ELEVATOR)* -.5) + .5) : .5);
	
	SetAnimation(an_acs_elvr, (GetControlSurfaceLevel(AIRCTRL_ELEVATOR) * -.5) + .5);
	
	SetAnimation(an_acs_rddr, (GetControlSurfaceLevel(AIRCTRL_RUDDER)   * -.5) + .5);	
	
	SetAnimation(an_acs_alrn, (GetControlSurfaceLevel(AIRCTRL_AILERON)  * -.5) + .5);
	
	//
	// 
	
	if (visor->pos < 1.0) // visor open (down)
	{
		//
		double dprs = GetDynPressure();
		
		//PlayVesselWave3(SFXID, SFX_WIND_OPEN, NOLOOP, int(GetDrag()*0.00035), 22050 + int(GetDrag()*0.0005));
		
		if (dprs > .1) 
			PlayVesselWave3(SFXID, SFX_WIND_OPEN, LOOP, min(240, int(dprs * 11.0e-3)), 11025 + int(22050e-4*dprs) );
	}
	
	// int(GetDrag()*0.0004), 22050 + int(GetDrag()*0.002)
	
	// operate moving parts
	//
	landingGears->operate(dT);
		   visor->operate(dT);
	     canards->operate(dT);
	   inltDoors->operate(dT);
	   ramxDoors->operate(dT);
	    bayDoors->operate(dT);
	         rcs->operate(dT);
	
	// wings can only pivot one way if the other is at low-detent
	if (wingTipFthr->pos <= 0.0) wingTipWvrd->operate(dT);
	if (wingTipWvrd->pos <= 0.0) wingTipFthr->operate(dT);
		
	// light positions must update accordingly as well...
	//
	if (wingTipFthr->pos > 0.0 && wingTipFthr->pos < 1.0 // when necessary...
	||  wingTipWvrd->pos > 0.0 && wingTipWvrd->pos < 1.0)
	{
		*nav_red.pos   = wingTipLightsL[0];
		*stb_l.pos     = wingTipLightsL[1];
		*nav_green.pos = wingTipLightsR[0];
		*stb_r.pos     = wingTipLightsR[1];
		
	}


	


	if (GroundContact())
	{
		// ground steerign by nosewyheel
		//
		double steer = GetControlSurfaceLevel(AIRCTRL_RUDDER);
		double speed = GetAirspeed();
		
		if (speed < 32.0) SetAnimation(an_dvc_tiller, .5-(steer*.5));
		else SetAnimation(an_dvc_tiller, .5);
		
		if (speed < 85.0 && speed > .3 && abs(steer) > 0.01 && GetPitch() < 0.0)
		{
			if (speed < 25.0) // slow-moving mode... constant steer
			{
				VECTOR3 rVel; GetAngularVel(rVel);
				rVel.y -= speed * steer * .032;
				SetAngularVel(rVel);
				//
				
			} else
			{
				//             speed-attenuation mode (prevents oversteer)
				
				VECTOR3 rVel; GetAngularVel(rVel); 
				rVel.y -= speed * steer * .032 * max(0.0, 1.0-(speed*.015));
				SetAngularVel(rVel);
				
			}
		}
	}
/*
	// gauge general-purpose things...
	//
	GetForceVector(vF);
	GetWeightVector(vW);
	vslMass = oapiGetMass(GetHandle());
	//
	//Catch missing data bug
	if(vslMass > 0)
	{
		vA = (vF-vW)/vslMass;

		Gver = vA.y / 9.81; // get G forces!
		Gfwd = vA.z / 9.81;
		Glat = vA.x / 9.81;
	}*/
	
	/*
	
	// trigger redraws
	//
	if (vcRdwT >= 0.12)
	{
		vcRdwT = 0.0;
		oapiVCTriggerRedrawArea(-1, VC_AREA_EICAS_ALL);
	} else
		vcRdwT += dT;
*/
	//oapiVCTriggerRedrawArea(-1, VC_AREA_FUELRES);

	//const VECTOR3 eyePos = V3_VC_EYEPOINT - (vA * .001);
	//SetCameraOffset();
	//SetCameraDefaultDirection(_V(0,0,1), vA.x * -.1);

}




int G422::clbkGeneric(int msgid = 0, int prm = 0, void *context = NULL)
{
	switch (msgid)
	{
	
		case VMSG_ENGRST: // engines reset
			
			
			// reset all engines!
			//

			SetThrusterLevel(ramcaster, 0.0);
			SetThrusterLevel(oms_thgr[0], 0.0);
			SetThrusterLevel(oms_thgr[1], 0.0);

			SetThrusterLevel(RT66_gasGen_thgr[0], 0.0);
			SetThrusterLevel(RT66_gasGen_thgr[1], 0.0);

			SetThrusterLevel(RT66_burner_thgr[0], 0.0);
			SetThrusterLevel(RT66_burner_thgr[1], 0.0);

			SetThrusterLevel(RT66_rocket_thgr[0], 0.0);
			SetThrusterLevel(RT66_rocket_thgr[1], 0.0);
			
			
			
		return 0;
		
		
		
		
		case VMSG_MPSTRT:  // moving part started moving

			switch ( prm )
			{
				case G422_SYSID_LGRS:
					
					//
					if (landingGears->pos >= 1.0)
					{
						SetTouchdownPoints (V3_CRASHLAND_C,V3_CRASHLAND_L, V3_CRASHLAND_R);
						SetSurfaceFrictionCoeff (0.12, 0.16);
						SetMaxWheelbrakeForce (0.0);
				//		SetNosewheelSteering(false);
						//
						PlayVesselWave3(SFXID, SFX_GEARS);
					}
					
					landingGears->sysReset = true;
					//
					
				break;


				case G422_SYSID_CNRD:

					if (canards->pos >= 1.0) // canards retracted or retracting...
					{
						DelControlSurface(acsCndrs);
						
					}
					
					PlayVesselWave3(SFXID, SFX_CNRDS);
					canards->sysReset = true;


				break;


				case G422_SYSID_VISR:
					//
					if (visor->pos <= 1.0) PlayVesselWave3(SFXID, SFX_VSRDN);
					else                    PlayVesselWave3(SFXID, SFX_VSRUP);

				break;
				
				
				case G422_SYSID_INLT: // inlet doors moving...
					//
					
				break;
				
				
				default:
				break;

			}

		//	allSystemsReset = false;

		return 0;
	
	
		case VMSG_MPSTOP:  // moving part stopped moving
		
			switch ( prm )
			{
				case G422_SYSID_LGRS:
				
					if (landingGears->pos >= 1.0) // gears are down and locked
					{
						SetTouchdownPoints (V3_TOUCHDOWN_C, V3_TOUCHDOWN_L, V3_TOUCHDOWN_R);
						SetSurfaceFrictionCoeff (0.03, 0.25);
						SetMaxWheelbrakeForce (6e5);
				//		SetNosewheelSteering(true);
						//
						
						VCSwitches[VC_swIndexByMGID[MGID_SW3_GEAR]].setPos(SW3_MID, this);
						
					}
					landingGears->sysReset = true;
				
				
				break;
				
				
				case G422_SYSID_CNRD:
				
					if (canards->pos >= 1.0) // canards are deployed!
					{
						//strcpy(oapiDebugString(), "canards deployed!");
						acsCndrs = CreateControlSurface3(AIRCTRL_ELEVATOR, 5.0, 2.0, _V(0.0, 2.0, 35.0), AIRCTRL_AXIS_XNEG);
						
						VCSwitches[VC_swIndexByMGID[MGID_SW3_CANARD]].setPos(SW3_MID, this);
					}
					canards->sysReset = true;
					StopVesselWave3(SFXID, SFX_CNRDS);
				
				
				break;
				
				
				case G422_SYSID_VISR:
				
					VCSwitches[VC_swIndexByMGID[MGID_SW3_VISOR]].setPos(SW3_MID, this);
					StopVesselWave3(SFXID, SFX_VSRDN);
					StopVesselWave3(SFXID, SFX_VSRUP);
					if (visor->pos >= 1.0) StopVesselWave3(SFXID, SFX_WIND_OPEN);
				
				break;
				
				
				case G422_SYSID_RAMX:
					
					PlayVesselWave3(SFXID, SFX_VC_AFLIK);
					VCSwitches[VC_swIndexByMGID[MGID_SW3_RAMX_DOOR]].setPos(SW3_MID, this);
					//
					// hover engines are assigned to ramcasters upon doors open - this prevents it being "used" by MFD's as if it really was a "hover" engine
					if (ramxDoors->getToggleState())
					{
						//engRamx.tgr_ramx = CreateThrusterGroup(&(engRamx.th_ramx), 1, THGROUP_HOVER);
						cueEngines(engRamx, RAMCASTER::ENG_STOP);
					}
					else
					{
						//DelThrusterGroup(engRamx.tgr_ramx, THGROUP_HOVER);
						cueEngines(engRamx, RAMCASTER::ENG_INOP);
					}
				break;
				
				case G422_SYSID_RCSDR:
					
					
					
					
				break;
				
				
				default:
				break;
					
			}
			
		//	allSystemsReset = false;
			
		return 0;
		
		case VMSG_LUAINSTANCE:
		
		
		return 0;
		
		case VMSG_LUAINTERPRETER:
		
		
		return 0;
		
		
		
		default:
		return 0;
		
	}
	
}


/*
//////////////////////////////////////////////////////////////////////////
//								ALL SYSTEMS RESET								  //
//////////////////////////////////////////////////////////////////////////

//
//
// not sure why this would need any parameters right now... but let's leave them in, just in case



int G422::systemsReset(int mode, int spec)
{
	
	//
	// ok, here we do the things that don't need checking every frame, but need updating nonetheless...
	//strcpy(oapiDebugString(), "all systems reset!");
	
	
	
	
	
	return 0;
}*/