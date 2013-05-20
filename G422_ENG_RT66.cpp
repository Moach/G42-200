


#include "G422.h"
#include "G422_MDL_EXT.h"






void G422::cueEngines(RT66 &eng, RT66::SIMSTATE sst)
{
	if (eng.state == sst) return;
	//clbkGeneric(VMSG_ENGRST, 0, NULL);
	//
	switch ( sst )
	{

	case RT66::ENG_SPOOLUP:
		//
		if (eng.state != RT66::SST_CUT && eng.state != RT66::SST_INOP) return; // yeah, can't start if not stopped first...

		if (inltDoors->getToggleState()) // int mode!
		{
			return cueEngines(eng, RT66::SST_STARTRCKT); // start as rocket!
			//
		}

	case RT66::SST_STARTGEN:
		//
		eng.state = RT66::SST_STARTGEN;


		return;


	case RT66::ENG_SPOOLDOWN:
		//
		if (eng.state == RT66::SST_RUN_INT) return cueEngines(eng, RT66::SST_STOPRCKT); // stop as rocket when running as such....
		if (eng.state != RT66::SST_RUN_EXT && eng.state != RT66::SST_STARTGEN) return; // otherwise, still can't stop what's not running

	case RT66::SST_STOPGEN:
		//
		eng.state = RT66::SST_STOPGEN;
		eng.epr = 0.0;
		eng.thr = 0.0;

		SetThrusterLevel(eng.th_burner, 0.0);
		SetThrusterLevel(eng.th_gasGen, 0.0);
		//SetThrusterLevel(eng.th_rocket, 0.0);

		StopVesselWave3(SFXID, SFX_JETROAR);
		StopVesselWave3(SFXID, SFX_AFTERBURNER);
		StopVesselWave3(SFXID, SFX_ENGINESTART_MAIN_EXT);
		//StopVesselWave3(SFXID, SFX_ENGINESTART_MAIN_INT);
		//PlayVesselWave3(SFXID, SFX_ENGINESTOP_MAIN_EXT);

		return;

	case RT66::SST_RUN_EXT:
		//
		StopVesselWave3(SFXID, SFX_ENGINESTART_MAIN_EXT);


		break;

	case RT66::SST_CUT:
		//
		eng.thr = 0.0;
		eng.epr = 0.0;
		eng.genPct = 0.0;


		SetThrusterLevel(eng.th_burner, 0.0);
		SetThrusterLevel(eng.th_gasGen, 0.0);
		SetThrusterLevel(eng.th_rocket, 0.0);

		break;




	case RT66::ENG_INLT_CLSE:
		switch (eng.state)
		{
		default: return; // not allowed if engines are running (or staring)
		case RT66::SST_CUT:  case RT66::SST_FAIL: case RT66::SST_INOP:
		case RT66::SST_STOPGEN: case RT66::SST_STOPRCKT:
			//
			inltDoors->toggle(true);
		}
		return;
	case RT66::ENG_INLT_OPEN:
		switch (eng.state)
		{
		default: return;
		case RT66::SST_CUT:  case RT66::SST_FAIL: case RT66::SST_INOP:
		case RT66::SST_STOPGEN: case RT66::SST_STOPRCKT:
			//
			inltDoors->toggle(false);
		}

		return;





	case RT66::SST_STARTRCKT:

		//
		//PlayVesselWave3(SFXID, SFX_ENGINESTART_MAIN_INT);

		break;


	case RT66::SST_RUN_INT:

		StopVesselWave3(SFXID, SFX_ENGINESTART_MAIN_INT);
		//

		break;


	case RT66::SST_STOPRCKT:

		if (eng.state != RT66::SST_RUN_INT) return; // likewise, can't stop what's not running
		eng.epr = 0.0;

		StopVesselWave3(SFXID, SFX_ENGINERUN_MAIN_INT);
		StopVesselWave3(SFXID, SFX_ENGINESTART_MAIN_INT);
		StopVesselWave3(SFXID, SFX_RCTROAR );

		SetThrusterLevel(eng.th_rocket, 0.0);


		break;



	case RT66::SST_INOP:


		break;



	default:
		break;

	}
	eng.state = sst;
}







void G422::simEngines(double &dT, RT66 &eng)
{
	if (eng.state == RT66::SST_INOP) return; // no simulation needed


	if (eng.feed & RT66::FUEL_PUMP) // simulate fuel line pressure from pumps
	{
		if (eng.fuelPrs < 1.0) {
			double deltaPrs = .22 * dT;
			eng.fuelPrs = min(eng.fuelPrs + deltaPrs, 1.0);
		}
	} else
	{
		if (eng.fuelPrs >  0.0) {
			double deltaPrs = -.12 * (1.0 - eng.genPct * .5) * dT;
			eng.fuelPrs = max(eng.fuelPrs + deltaPrs, 0.0);
		}
	}

	if (eng.feed & RT66::OXY_PUMP) // simulate oxidizer line pressure from pumps
	{
		if (eng.oxyPrs < 1.0) {
			double deltaPrs = .22 * dT;
			eng.oxyPrs = min(eng.oxyPrs + deltaPrs, 1.0);
		}
	} else
	{
		if (eng.oxyPrs >  0.0) {
			double deltaPrs = -.12 * (1.0 - eng.genPct * .5) * dT;
			eng.oxyPrs = max(eng.oxyPrs + deltaPrs, 0.0);
		}
	}

	if (GetPropellantMass(fuel_main_allTanks) <= 0.1) eng.fuelPrs = 0.0;
	if (GetPropellantMass(fuel_oxy) <= 0.1) eng.oxyPrs = 0.0;

	switch(eng.state)
	{
	default:
	case RT66::SST_CUT:
	case RT66::SST_FAIL:
		//eng.epr = eng.thr = 0.0;


		return;


	case RT66::SST_STARTGEN:
		//
		PlayVesselWave3(SFXID, SFX_ENGINESTART_MAIN_EXT, NOLOOP, 220);
		eng.genPct += dT * 0.04854368932038834951 * ENGINE_IDLE_EXTPCT; //   1 / 20.6 secs -- engine spoolup time must match sound fx length

		if (eng.genPct >= ENGINE_IDLE_EXTPCT) cueEngines(eng, RT66::SST_RUN_EXT); // running!

		return;

	case RT66::SST_RUN_EXT:
		{

			//	double engPerf = FXMapRedChannel(thrFX_map, thrFX_X, thrFX_Y);

			//                       dynP :      0     4      8       12     16     20    24      28    32     36     40     44
			static double engFx_subs[12] = {0.92, 0.94, 0.97, .9825, .9992, .995,  .952,   .83,  .68,  .54,  .39,   .32 };
			static double engFx_sprs[12] = {0.50, 0.65, 0.83, .9885, .9998, 1.0,  .9987,   .884,  .753,  .425,  .28,   .14 };
			double *engFx = (GetMachNumber() >= 1.0)? engFx_sprs : engFx_subs; // very blunt, yet effective sonic transition

			//                       Mach :       0     .5      1    1.5     2    2.5     3       3.5 
			static double engFx_mach[8] = {0.974, 0.995, .998,  1.0, .975, .822,  .585,   .12};


			double DpFx = GetDynPressure() * 2.27272727e-5;
			//
			int perfRefLo = floor(DpFx * 11.0);   perfRefLo = (perfRefLo > 11)? 11 : (perfRefLo < 0)? 0 : perfRefLo;
			int perfRefHi =  ceil(DpFx * 11.0);   perfRefHi = (perfRefHi > 11)? 11 : (perfRefHi < 0)? 0 : perfRefHi;
			//
			double delta = engFx[perfRefHi] - engFx[perfRefLo];
			double engPerf = engFx[perfRefLo] + delta * ((DpFx*11.0 - (double)perfRefLo) * 0.0909090909);
		


			double MnFx = GetMachNumber() * 0.285714286;
			//
			perfRefLo = floor(MnFx * 7.0);  perfRefLo = (perfRefLo > 7)? 7 : (perfRefLo < 0)? 0 : perfRefLo;
			perfRefHi =  ceil(MnFx * 7.0);  perfRefHi = (perfRefHi > 7)? 7 : (perfRefHi < 0)? 0 : perfRefHi;
			//
			delta = engFx_mach[perfRefHi] - engFx_mach[perfRefLo];
			engPerf *= engFx_mach[perfRefLo] + delta * ((MnFx*7.0 - (double)perfRefLo) * 0.142857143);



			//
			//
			
			
			if (burner_toggle)
			{
				eng.thr = ENGINE_IDLE_EXTPCT + (GetEngineLevel(ENGINE_MAIN) * (1.2-ENGINE_IDLE_EXTPCT) * eng.fuelPrs);
				
				if (eng.thr > 1.0) // burners, engage!
				{
					
					double rhtLvl = max(0.0, (eng.thr - 1.0)*5.0);
					SetThrusterLevel(eng.th_burner, rhtLvl);
					PlayVesselWave3(SFXID, SFX_AFTERBURNER, LOOP, int(255 * rhtLvl));
					
					eng.thr = 1.0;
				
				} else
				{
					StopVesselWave3(SFXID, SFX_AFTERBURNER);
					SetThrusterLevel(eng.th_burner, 0.0);
					
				}
				
				
			} else
			{
				eng.thr = ENGINE_IDLE_EXTPCT + (GetEngineLevel(ENGINE_MAIN) * (1.0-ENGINE_IDLE_EXTPCT) * eng.fuelPrs);
				
				StopVesselWave3(SFXID, SFX_AFTERBURNER);
				SetThrusterLevel(eng.th_burner, 0.0);
				
			}
			
			eng.genPct += ((eng.thr - eng.genPct) * ENGINE_RAMP_SCALAR * dT);
			eng.epr += (eng.genPct * engPerf - eng.epr) * dT * ENGINE_RAMP_SCALAR;




			if (eng.fuelPrs/*eng.epr*/ <= 0.01) 
				return cueEngines(eng, RT66::SST_STOPGEN); // dry stall

			SetThrusterIsp(eng.th_gasGen,    ISPMAX_MAIN_GEN * eng.epr);
			SetThrusterMax0(eng.th_gasGen, MAXTHRUST_MAIN_GEN * eng.epr);
			SetThrusterLevel(eng.th_gasGen, eng.genPct);

			PlayVesselWave3(SFXID, SFX_ENGINERUN_MAIN_EXT, LOOP, 220/* + int(eng.epr*25)*/, 22050 + int(18000*(eng.genPct-ENGINE_IDLE_EXTPCT)));
			PlayVesselWave3(SFXID, SFX_JETROAR, LOOP, int(255 * eng.epr));

			
			SetThrusterIsp(eng.th_burner,    ISPMAX_MAIN_AFB * eng.epr);
			SetThrusterMax0(eng.th_burner, MAXTHRUST_MAIN_AFB * eng.epr);

			/*

			if (burner_toggle)  // reheat ON! (this is probably a bad idea... a proper variable should be used here)
			{
				//double rhtLvl = (max(eng.thr - .8, 0.0) * 5.0) * eng.epr;
			

				

			} else
			{
				
			}*/
		}
		return;

	case RT66::SST_STOPGEN:
		//
		if (eng.genPct > ENGINE_IDLE_EXTPCT)
		{
			eng.genPct -= eng.genPct * ENGINE_RAMP_SCALAR * dT;
			//

			PlayVesselWave3(SFXID, SFX_ENGINERUN_MAIN_EXT, NOLOOP, 220, 22050 + int(18000*(eng.genPct-ENGINE_IDLE_EXTPCT)));

		} else
		{
			PlayVesselWave3(SFXID, SFX_ENGINESTOP_MAIN_EXT, NOLOOP, 220);
			eng.genPct -= dT * 0.07407407407407407407 * ENGINE_IDLE_EXTPCT; // 1 / 13.5 secs -- same deal...

			if (eng.genPct <= 0.0)
			{
				//
				cueEngines(eng, RT66::SST_CUT); // off!
			}
		}

		return;




	case RT66::SST_STARTRCKT:

		PlayVesselWave3(SFXID, SFX_ENGINESTART_MAIN_INT, NOLOOP, 220);
		eng.genPct += dT * 0.04444444444444444444 * ENGINE_IDLE_INTPCT; //   1 / 22.5 secs -- engine spoolup time must match sound fx length

		if (eng.genPct >= ENGINE_IDLE_INTPCT) 
			cueEngines(eng, RT66::SST_RUN_INT); // running!
		return;


	case RT66::SST_RUN_INT:
		{
			// engines are shut if throttle gets fully closed	(or when they run out of fuel)
			if (eng.fuelPrs*eng.oxyPrs <= 0.01 || GetEngineLevel(ENGINE_MAIN) < 0.001) return cueEngines(eng, RT66::SST_STOPRCKT);

			eng.thr = ENGINE_IDLE_INTPCT + (GetEngineLevel(ENGINE_MAIN) * (1-ENGINE_IDLE_INTPCT) * eng.fuelPrs * eng.oxyPrs);
			eng.genPct += ((eng.thr - eng.genPct) * .52 * dT);
			eng.epr = eng.genPct * eng.thr;

			SetThrusterLevel(eng.th_rocket, eng.epr);
			//
			PlayVesselWave3(SFXID, SFX_ENGINERUN_MAIN_INT, LOOP, 220 + int(eng.epr * 15.0), 22050 + int(eng.epr * 11025.0));
			PlayVesselWave3(SFXID, SFX_RCTROAR, LOOP, int(eng.thr*255));


			double flwRate = GetPropellantFlowrate(fuel_main_allTanks);
			double oxyMass = GetPropellantMass(fuel_oxy);
			//

			// we've halved the propellant flowrate (wrt the legacy model) since now this bit runs for each of the two engines
			SetPropellantMass(fuel_oxy, max(0.0, oxyMass - ( flwRate * .5 * dT * OXYFUEL_RATIO )));

		}
		return;

	case RT66::SST_STOPRCKT:
		if (eng.genPct > ENGINE_IDLE_INTPCT)
		{
			eng.genPct -= eng.genPct * ENGINE_RAMP_SCALAR * dT;
			//

			PlayVesselWave3(SFXID, SFX_ENGINERUN_MAIN_INT, NOLOOP, 220, 22050 + int(18000*(eng.genPct-ENGINE_IDLE_INTPCT)));

		} else
		{
			PlayVesselWave3(SFXID, SFX_ENGINESTOP_MAIN_INT, NOLOOP, 220);
			eng.genPct -= dT * .1 * ENGINE_IDLE_INTPCT; // 1 / 10 secs -- same deal...

			if (eng.genPct <= 0.0)
			{
				//
				cueEngines(eng, RT66::SST_CUT); // off!
			}
		}
		return;

	}

	//
	//


}

