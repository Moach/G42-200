
#include "G422.h"
#include "G422_DVC.h"
#include "G422_MDL_EXT.h"
//


//bool initOne;
int G422::vslCount = 0;
G422_DRAWRES G422DrawRes;
map<UINT, int> VC_swIndexByMGID;

G422::G422(OBJHANDLE vsl_hndl, int fltModel) : VESSEL3 (vsl_hndl, fltModel)
{
	
	allSystemsReset = true;
	
	thrPos = 0.0;
	cgShiftRef = 0.0;
	VC_PoV_station = 0;
	burner_toggle  = 0;
	ramcaster_mode = 0;
	wingPos = 0;
	main_eng_mode = 0;
	thr_authority = 0;
	main_ign_sqnc = 1;
	prk_brake_mode = 0;
	gnd_steer_mode = 0;
	
	
	
	vcRdwT = 0.0;
	
	thrFX_X = 0.0;
	thrFX_Y = 0.0;
	
	fx_exhaustLvl  = 0.0;
	fx_contrailLvl = 0.0;
	
	memset(&nav_green, 0, sizeof(BEACONLIGHTSPEC)*8);
	//
	//
	
	memset(&engMain_L, 0, sizeof(RT66));
	memset(&engMain_R, 0, sizeof(RT66));
	memset(&engRamx,   0, sizeof(RAMCASTER));
	memset(&apu,       0, sizeof(APU));
	
	engMain_L.state = engMain_R.feed = RT66::SST_INOP;
	engRamx.state = RAMCASTER::ENG_INOP;
	apu.state = APU::ENG_INOP;
	
	engMain_L.feed = RT66::FUEL_OPEN;
	engMain_R.feed = RT66::FUEL_OPEN; // fuel lines always open 'til coding gets to that
	apu.feed       =  APU::FUEL_OPEN;
	engRamx.feed   = RAMCASTER::FUEL_OPEN;
	
	VC_eicas_screens[0] = -1;
	VC_eicas_screens[1] = -1;
	VC_eicas_screens[2] = -1;
	VC_eicas_screens[3] = -1;
	VC_eicas_screens[4] = -1;
	VC_eicas_screens[5] = -1;
	
	drawEicas = (1 << 5) | (1 << 7);
	
	if (!vslCount)
	{
		//G422DrawRes.bmpRes[0] = oapiCreateSurface(LOADBMP(BMP_RHTTAPE));
		
		G422DrawRes.mfdLabelsFont = oapiCreateFont(12, true, "FixedSys", FONT_BOLD);	

		G422DrawRes.brSet[0] = oapiCreateBrush(0x000000);
		G422DrawRes.brSet[1] = oapiCreateBrush(0x0000FF);
		G422DrawRes.brSet[2] = oapiCreateBrush(0x200000);
		G422DrawRes.brSet[3] = oapiCreateBrush(0xFFFFFF);

		G422DrawRes.spSet[0] = oapiCreatePen(1, 2, 0x00FF00);
		G422DrawRes.spSet[1] = oapiCreatePen(1, 2, 0xFFCC00);
		G422DrawRes.spSet[2] = oapiCreatePen(1, 2, 0x0000FF);
		G422DrawRes.spSet[3] = oapiCreatePen(1, 2, 0x00CCFF);
		G422DrawRes.spSet[4] = oapiCreatePen(1, 2, 0xFFFFFF);
		G422DrawRes.spSet[5] = oapiCreatePen(1, 2, 0x200000);

	
		for (int i=0; i<VC_SWITCH_COUNT; ++i)
		{
			//VCSwitchDef &swDef = VC_swDefs[i];
			VC_swIndexByMGID[ (VC_swDefs[i].mgid) ] = i; // map the array indices of the switches by their mesh IDs... makes them easier to find
			//
		}
		
		
		
		
		//initOne = true;
	}
	
	
	
	
	
	
	++vslCount;
}

G422::~G422()
{
	clearAnimations();
	oapiDestroySurface(vcRes_eicasSrc);
	//
	
	--vslCount;
	if (!vslCount)
	{
		oapiReleaseFont(G422DrawRes.mfdLabelsFont);
		
		oapiReleasePen(G422DrawRes.spSet[0]);
		oapiReleasePen(G422DrawRes.spSet[1]);
		oapiReleasePen(G422DrawRes.spSet[2]);
		oapiReleasePen(G422DrawRes.spSet[3]);
		oapiReleasePen(G422DrawRes.spSet[4]);
		oapiReleasePen(G422DrawRes.spSet[5]);
		
		oapiReleaseBrush(G422DrawRes.brSet[0]);
		oapiReleaseBrush(G422DrawRes.brSet[1]);
		oapiReleaseBrush(G422DrawRes.brSet[2]);
		oapiReleaseBrush(G422DrawRes.brSet[3]);
		
		oapiDestroySurface(G422DrawRes.bmpRes[0]);
	}
}


BMP thrFX_map;









void G422::clbkSetClassCaps(FILEHANDLE cfg)
{
	//
	
	SetEmptyMass(EMPTY_MASS);
	SetSize(75);
	
	SetVisibilityLimit(1e-3, 2e-4);
	SetClipRadius(35);	
		
	/*  
	//	DISUSED -- legacy bmp lookup setup for the engines
		
		
	if(!thrFX_map.ReadFromFile("Config\\G422\\ThrFX.bmp"))
	{
		MessageBox(NULL, "'thrFX.bmp' file missing!\n should be in Config/G422 folder, but there it ain't...\n can't load G42 now :P", "G42-200 :: EPIC FAIL", MB_OK | MB_ICONERROR);
		return;
	}
	*/
	
	
	mdl_ext = oapiLoadMeshGlobal("G422\\G422_ext");
	mdl_dvc = oapiLoadMeshGlobal("G422\\G422_dvc");	
	mdl_vc_L = oapiLoadMeshGlobal("G422\\G422_dvc_L");	
	mdl_vc_R = oapiLoadMeshGlobal("G422\\G422_dvc_R");	
	
	mdlID_ext  =  AddMesh(mdl_ext);  SetMeshVisibilityMode (mdlID_ext,  MESHVIS_EXTERNAL); 
	mdlID_dvc  =  AddMesh(mdl_dvc);  SetMeshVisibilityMode (mdlID_dvc,  MESHVIS_VC);	    
	mdlID_dvcR =  AddMesh(mdl_vc_R); SetMeshVisibilityMode (mdlID_dvcR, MESHVIS_VC);	 
	mdlID_dvcL =  AddMesh(mdl_vc_L); SetMeshVisibilityMode (mdlID_dvcL, MESHVIS_NEVER);	 
	
	
	static SURFHANDLE     rcsTex = oapiRegisterExhaustTexture("exhaust_atrcs");
	static SURFHANDLE exhaustTex[4];
	exhaustTex[0] = oapiRegisterExhaustTexture("DGa2\\exhaust_main");
	exhaustTex[1] = oapiRegisterExhaustTexture("DGa2\\exhaust_ramx");
	exhaustTex[2] = oapiRegisterExhaustTexture("DGa2\\exhaust_oxy");
	exhaustTex[3] = oapiRegisterExhaustTexture("DGa2\\exhaust_oms");
	
	
	
	
	
	SetTouchdownPoints (V3_TOUCHDOWN_C, V3_TOUCHDOWN_L, V3_TOUCHDOWN_R);
	
	//
	SetAlbedoRGB (_V(0.998,0.998,0.998));
	SetGravityGradientDamping (20.0);
	SetCrossSections (_V(575, 1505, 90));
	SetSurfaceFrictionCoeff (0.03, 0.25);
	SetMaxWheelbrakeForce (8e5);
	SetPMI (_V(85.0, 105.0, 45.0));  // _V(22.5,21.1,21.7)     _V(85.0, 105.0, 45.0)
	SetRotDrag (_V(0.85, 0.82, 0.75)); // _V(0.85, 0.80, 0.72)
	//
	
	EnableTransponder(true);
	//SetNosewheelSteering(true);
	//
	//
	
	SetDockParams(V3_DOCKING_PORT, _Y, _Zn);
	att_bay_1 = CreateAttachment(false, V3_BAY_ATTACH_001, _Y, _Z, "bay");
	
	
	static VECTOR3 nav_lgts_pos[7] =
	{
		 V3_LGT_NAV_GREEN, V3_LGT_NAV_RED, V3_LGT_STROBE_L, V3_LGT_STROBE_R, V3_LGT_STROBE_T, V3_LGT_BCN_1, V3_LGT_BCN_2
	};
	static VECTOR3 nav_lgts_col[3] = {{0.5, 1.0, 0.5}, {1.0, 0.5, 0.5}, {1.0, 1.0, 1.0}};
	
	nav_green.shape = BEACONSHAPE_DIFFUSE;     nav_red.shape = BEACONSHAPE_DIFFUSE;  nav_white.shape = BEACONSHAPE_DIFFUSE;
	nav_green.pos = nav_lgts_pos;			   nav_red.pos = nav_lgts_pos+1;		  nav_white.pos =  nav_lgts_pos+4;
	nav_green.col = nav_lgts_col;			   nav_red.col = nav_lgts_col+1;		  nav_white.col =  nav_lgts_col+2;
	nav_green.size = .25;					   nav_red.size = .25;					  nav_white.size = .25;
	nav_green.falloff = .2;				       nav_red.falloff = .2;				  nav_white.falloff = .2;
	nav_green.period = 0.0;				       nav_red.period = 0.0;				  nav_white.period = 0.0;	
	nav_green.active = false;				   nav_red.active = false;				  nav_white.active = false;
	AddBeacon(&nav_green);					   AddBeacon(&nav_red);					  AddBeacon(&nav_white);
	
	
	
	stb_l.shape = BEACONSHAPE_STAR;         stb_r.shape = BEACONSHAPE_STAR;     stb_t.shape = BEACONSHAPE_STAR;
	stb_l.pos = nav_lgts_pos+2;			   	stb_r.pos = nav_lgts_pos+3;			 stb_t.pos = nav_lgts_pos+4;
	stb_l.col = nav_lgts_col+2;			   	stb_r.col = nav_lgts_col+2;			 stb_t.col = nav_lgts_col+2;
	stb_l.size = 0.5;					   	stb_r.size = 0.5;					 stb_t.size = 0.5;
	stb_l.falloff = .6;                   	stb_r.falloff = .6;			    	 stb_t.falloff = .6;
	stb_l.period = 1.0;                    	stb_r.period = 1.0;					 stb_t.period = 1.0;
	stb_l.duration = .05;				   	stb_r.duration = .05;				 stb_t.duration = .05;
	stb_l.tofs = 0.0;                      	stb_r.tofs = 0.0;					 stb_t.tofs = 0.2;
	stb_l.active = false;					stb_r.active = false;				 stb_t.active = false;
	AddBeacon(&stb_l);						AddBeacon(&stb_r);					 AddBeacon(&stb_t);
	
	
	bcn1.shape = BEACONSHAPE_DIFFUSE; bcn2.shape = BEACONSHAPE_DIFFUSE;
	bcn1.pos = nav_lgts_pos+5;		  bcn2.pos = nav_lgts_pos+6;
	bcn1.col = nav_lgts_col+1;		  bcn2.col = nav_lgts_col+1;
	bcn1.size = .3;					  bcn2.size = .3;
	bcn1.falloff = .2;				  bcn2.falloff = .2;
	bcn1.period = 1.5;				  bcn2.period = 1.5;
	bcn1.duration = .2;				  bcn2.duration = .2;
	bcn1.tofs = 0.5;				  bcn2.tofs = 1.0;
	bcn1.active = false;			  bcn2.active = false;
	AddBeacon(&bcn1);				  AddBeacon(&bcn2);
	
	
	
	//
	//
	/*
	static PARTICLESTREAMSPEC contrail_main = {
		0, 1.0, .85, 120, 0.05, 10.0, 8, 3.0, PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_PLIN, -1.0, 25.0,
		PARTICLESTREAMSPEC::ATM_PLIN, 0.5, 0.15
	};
	static PARTICLESTREAMSPEC contrail_ramx = {
		0, 2.5, .95, 120, 0.03, 10.0, 5, 3.0, PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_PLIN, -1.0, 25.0,
		PARTICLESTREAMSPEC::ATM_PLIN, 0.15, 0.05
	};*/
	
	static PARTICLESTREAMSPEC fx_contrail_long = {
		0, 1.0, .85, 120, 0.05, 30.0, 8, 3.0, PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_LIN, 0.0, 1.0,
		PARTICLESTREAMSPEC::ATM_FLAT, 1.0, 1.0, NULL
	};
	
	static PARTICLESTREAMSPEC fx_jetgas_burn = {
		0, 1.0, .85, 120, 0.05, 2.5, 8, 3.0, PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_LIN, 0.0, 1.0,
		PARTICLESTREAMSPEC::ATM_FLAT, 1.0, 1.0, NULL
	};

/*	AddParticleStream(&fx_contrail_long, _V(-10, 0.0, -50), _V(0.0, 0.0,-1.0), &fx_contrailLvl);
	AddParticleStream(&fx_contrail_long, _V( 10, 0.0, -50), _V(0.0, 0.0,-1.0), &fx_contrailLvl);
	AddParticleStream(&fx_jetgas_burn, V3_EXHAUST_MAIN_L, _V(0.0, 0.0, -1.0), &fx_exhaustLvl);
	AddParticleStream(&fx_jetgas_burn, V3_EXHAUST_MAIN_R, _V(0.0, 0.0, -1.0), &fx_exhaustLvl);
*/
	
	//
	fuel_main_allTanks = CreatePropellantResource(MAXFUEL_MAIN_ALL, MAXFUEL_MAIN_ALL, 1);
	fuel_oxy = CreatePropellantResource(MAXFUEL_OXY, MAXFUEL_OXY, 1);
	fuel_sys = CreatePropellantResource(MAXFUEL_SYS, MAXFUEL_SYS, 1);
	//
	//
	
	dummyFuel = CreatePropellantResource(1,1,1);
	dummyThruster     = CreateThruster(_V(0,0,0), _V(0,0,1), 0, dummyFuel, 1, 1, 1);
	dummyThrAirbrakes = CreateThruster(_V(0,0,0), _V(0,0,1), 0, dummyFuel, 1, 1, 1);
	dummyHover        = CreateThruster(_V(0,0,0), _V(0,0,1), 0, dummyFuel, 1, 1, 1);
	//
	controller_thgr = CreateThrusterGroup(&dummyThruster, 1, THGROUP_MAIN); // this will serve to read the inputs from the throttle

	//
	CreateThrusterGroup(&dummyThrAirbrakes, 1, THGROUP_RETRO);
	CreateThrusterGroup(&dummyHover, 1,	THGROUP_HOVER);
	
	
	// main engines!
	//
	
	RT66_gasGen_thgr[0] = engMain_L.th_gasGen =  CreateThruster(_V(0,0,0), _V(0,0,1), MAXTHRUST_MAIN_GEN, fuel_main_allTanks);
	RT66_gasGen_thgr[1] = engMain_R.th_gasGen =  CreateThruster(_V(0,0,0), _V(0,0,1), MAXTHRUST_MAIN_GEN, fuel_main_allTanks);
//	CreateThrusterGroup(RT66_gasGen_thgr, 2, THGROUP_USER);
	
	RT66_burner_thgr[0] = engMain_L.th_burner =  CreateThruster(_V(0,0,0), _V(0,0,1), MAXTHRUST_MAIN_AFB, fuel_main_allTanks);
	RT66_burner_thgr[1] = engMain_R.th_burner =  CreateThruster(_V(0,0,0), _V(0,0,1), MAXTHRUST_MAIN_AFB, fuel_main_allTanks);
//	CreateThrusterGroup(RT66_burner_thgr, 2, THGROUP_USER);
	
	SetEngineLevel(ENGINE_MAIN, 0.0);
	thrPos = 0.0;
	
	
	AddExhaust(RT66_burner_thgr[0], 15, 1.5, V3_EXHAUST_MAIN_L, _V(0,0,-1), exhaustTex[0]);
	AddExhaust(RT66_burner_thgr[1], 15, 1.5, V3_EXHAUST_MAIN_R, _V(0,0,-1), exhaustTex[0]);
	//AddExhaustStream(RT66_gasGen_thgr[0], _V( 10, 0, -50), &contrail_main);
	//AddExhaustStream(RT66_gasGen_thgr[0], _V(-10, 0, -50), &contrail_main);
	
	
	// rocket mode!
	//
	RT66_rocket_thgr[0] = engMain_L.th_rocket =  CreateThruster(_V(0,0,0), _V(0,0,1), MAXTHRUST_MAIN_RKT, fuel_main_allTanks, ISPMAX_MAIN_RKT, ISPMIN_MAIN_RKT);
	RT66_rocket_thgr[1] = engMain_R.th_rocket =  CreateThruster(_V(0,0,0), _V(0,0,1), MAXTHRUST_MAIN_RKT, fuel_main_allTanks, ISPMAX_MAIN_RKT, ISPMIN_MAIN_RKT);
	AddExhaust(RT66_rocket_thgr[0], 18, 1.2, V3_EXHAUST_MAIN_L, _V(0,0,-1), exhaustTex[2]);
	AddExhaust(RT66_rocket_thgr[1], 18, 1.2, V3_EXHAUST_MAIN_R, _V(0,0,-1), exhaustTex[2]);
	CreateThrusterGroup(RT66_rocket_thgr, 2, THGROUP_USER);
	
	// ramcaster!
	//
	ramcaster = engRamx.th_ramx = CreateThruster(_V(0,0,0), _V(0,0,1), 1, fuel_main_allTanks);
	
	
	AddExhaust(ramcaster, 16, 1.5, V3_RAMX_EXHAUST_001, _V(0,0,-1), exhaustTex[1]);
	AddExhaust(ramcaster, 18, 1.6, V3_RAMX_EXHAUST_002, _V(0,0,-1), exhaustTex[1]);
	AddExhaust(ramcaster, 16, 1.5, V3_RAMX_EXHAUST_003, _V(0,0,-1), exhaustTex[1]);
	AddExhaust(ramcaster, 15, 1.4, V3_RAMX_EXHAUST_004, _V(0,0,-1), exhaustTex[1]);
	AddExhaust(ramcaster, 15, 1.4, V3_RAMX_EXHAUST_005, _V(0,0,-1), exhaustTex[1]);
	//AddExhaustStream(ramcaster, _V(0, -1, -30), &contrail_ramx);
	
	
	// OMS!
	//
	oms_thgr[0] = CreateThruster(_V(0,0,0), _V(0,0,1), MAXTHRUST_OMS, fuel_sys, ISPMAX_OMS,  ISPMIN_OMS);
	oms_thgr[1] = CreateThruster(_V(0,0,0), _V(0,0,1), MAXTHRUST_OMS, fuel_sys, ISPMAX_OMS,  ISPMIN_OMS);
	AddExhaust(oms_thgr[0], 10, .6, V3_EXHAUST_OMS_L, _V(0,0,-1), exhaustTex[3]);
	AddExhaust(oms_thgr[1], 10, .6, V3_EXHAUST_OMS_R, _V(0,0,-1), exhaustTex[3]);
	
	
	wingLift = CreateAirfoil3 (LIFT_VERTICAL, _V(0,0,0), VLiftCoeff, 0, 32, 850, 2.1);
	// wing and body lift+drag components

	CreateAirfoil3 (LIFT_HORIZONTAL, _V(0,0,-22), HLiftCoeff, 0, 12, 180, 2.2);
	// vertical stabiliser and body lift and drag components




	CreateControlSurface3 (AIRCTRL_ELEVATOR,     4.0, 1.5, _V(0,0,-25.0), AIRCTRL_AXIS_AUTO);
	CreateControlSurface3 (AIRCTRL_ELEVATORTRIM, 4.0, 1.5, _V(0,0,-25.0), AIRCTRL_AXIS_AUTO);


	CreateControlSurface3 (AIRCTRL_AILERON, 4.0, 1.5, _V( 22.5,0,-25), AIRCTRL_AXIS_AUTO);
	CreateControlSurface3 (AIRCTRL_AILERON, 4.0, 1.5, _V(-22.5,0,-25), AIRCTRL_AXIS_AUTO);
	//
	
	CreateControlSurface3 (AIRCTRL_RUDDER, 4.5, 1.5,  _V(   0,0,-30.0), AIRCTRL_AXIS_AUTO);
	//
	
	
	
	// and now, the RCS thrusters
	// 
	


	//
	th_rcs_pitchUp[0] = th_rcs_vertUp[0]   = CreateThruster (_V(0,0, 40), _V(0,  1,0), MAXTRHUST_RCS, fuel_sys, ISPMAX_RCS); // raise bow 
	th_rcs_pitchUp[1] = th_rcs_vertDown[1] = CreateThruster (_V(0,0,-40), _V(0, -1,0), MAXTRHUST_RCS, fuel_sys, ISPMAX_RCS); // drop stern
	//
	th_rcs_pitchDn[0] = th_rcs_vertDown[0] = CreateThruster (_V(0,0, 40), _V(0, -1,0), MAXTRHUST_RCS, fuel_sys, ISPMAX_RCS); // drop bow
	th_rcs_pitchDn[1] = th_rcs_vertUp[1]   = CreateThruster (_V(0,0,-40), _V(0,  1,0), MAXTRHUST_RCS, fuel_sys, ISPMAX_RCS); // raise stern

	//
	
	
	AddExhaust (th_rcs_pitchUp[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSF_DN_1, _V(-.1,-1,0), rcsTex); // raise bow
	AddExhaust (th_rcs_pitchUp[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSF_DN_2, _V(-.1,-1,0), rcsTex);
	AddExhaust (th_rcs_pitchUp[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSF_DN_3, _V(-.1,-1,0), rcsTex);
	AddExhaust (th_rcs_pitchUp[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSF_DN_4, _V( .1,-1,0), rcsTex);
	AddExhaust (th_rcs_pitchUp[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSF_DN_5, _V( .1,-1,0), rcsTex);
	AddExhaust (th_rcs_pitchUp[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSF_DN_6, _V( .1,-1,0), rcsTex);
	
	AddExhaust (th_rcs_pitchUp[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_UP_1, _V(0,1,0), rcsTex); // drop stern
	AddExhaust (th_rcs_pitchUp[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_UP_2, _V(0,1,0), rcsTex);
	AddExhaust (th_rcs_pitchUp[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_UP_3, _V(0,1,0), rcsTex);
	AddExhaust (th_rcs_pitchUp[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_UP_4, _V(0,1,0), rcsTex);
	AddExhaust (th_rcs_pitchUp[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_UP_5, _V(0,1,0), rcsTex);
	AddExhaust (th_rcs_pitchUp[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_UP_6, _V(0,1,0), rcsTex);
	AddExhaust (th_rcs_pitchUp[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_UP_7, _V(0,1,0), rcsTex);
	AddExhaust (th_rcs_pitchUp[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_UP_8, _V(0,1,0), rcsTex);
	
	AddExhaust (th_rcs_pitchDn[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_DN_1, _V(-.1,-1,0), rcsTex); // raise stern
	AddExhaust (th_rcs_pitchDn[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_DN_2, _V(-.1,-1,0), rcsTex);
	AddExhaust (th_rcs_pitchDn[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_DN_3, _V(-.1,-1,0), rcsTex);
	AddExhaust (th_rcs_pitchDn[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_DN_4, _V( .1,-1,0), rcsTex);
	AddExhaust (th_rcs_pitchDn[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_DN_5, _V( .1,-1,0), rcsTex);
	AddExhaust (th_rcs_pitchDn[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_DN_6, _V( .1,-1,0), rcsTex);
	
	AddExhaust (th_rcs_pitchDn[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSF_UP_1, _V(0,1,0), rcsTex); // drop bow
	AddExhaust (th_rcs_pitchDn[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSF_UP_2, _V(0,1,0), rcsTex);
	AddExhaust (th_rcs_pitchDn[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSF_UP_3, _V(0,1,0), rcsTex);
	AddExhaust (th_rcs_pitchDn[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSF_UP_4, _V(0,1,0), rcsTex);
	
	
	


	////////////////////////

	th_rcs_yawLeft[0]  = th_rcs_horLeft[0]  = CreateThruster (_V(0,0, 40), _V(-1, 0,0), MAXTRHUST_RCS * .75, fuel_sys, ISPMAX_RCS);  // bow left
	th_rcs_yawLeft[1]  = th_rcs_horRight[1] = CreateThruster (_V(0,0,-40), _V( 1, 0,0), MAXTRHUST_RCS * .75, fuel_sys, ISPMAX_RCS);  // stern right
	//																										  
	th_rcs_yawRight[0] = th_rcs_horRight[0] = CreateThruster (_V(0,0, 40), _V( 1, 0,0), MAXTRHUST_RCS * .75, fuel_sys, ISPMAX_RCS);  // bow right
	th_rcs_yawRight[1] = th_rcs_horLeft[1]  = CreateThruster (_V(0,0,-40), _V(-1, 0,0), MAXTRHUST_RCS * .75, fuel_sys, ISPMAX_RCS);  // stern left
	
	
	
	
	
	AddExhaust (th_rcs_yawLeft[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSF_RT_1, _V(1,0,0), rcsTex); // bow left
	AddExhaust (th_rcs_yawLeft[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSF_RT_2, _V(1,0,0), rcsTex);
	AddExhaust (th_rcs_yawLeft[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSF_RT_3, _V(1,0,0), rcsTex);
	
	AddExhaust (th_rcs_yawLeft[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_LF_1, _V(-1,0,0), rcsTex); // stern right
	AddExhaust (th_rcs_yawLeft[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_LF_2, _V(-1,0,0), rcsTex);
	AddExhaust (th_rcs_yawLeft[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_LF_3, _V(-1,0,0), rcsTex);
	
	AddExhaust (th_rcs_yawRight[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSF_LF_1, _V(-1,0,0), rcsTex); // bow right
	AddExhaust (th_rcs_yawRight[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSF_LF_2, _V(-1,0,0), rcsTex);
	AddExhaust (th_rcs_yawRight[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSF_LF_3, _V(-1,0,0), rcsTex);
	
	AddExhaust (th_rcs_yawRight[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_RT_1, _V(1,0,0), rcsTex); // stern left
	AddExhaust (th_rcs_yawRight[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_RT_2, _V(1,0,0), rcsTex);
	AddExhaust (th_rcs_yawRight[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSA_RT_3, _V(1,0,0), rcsTex);
	
	
	/////////////////////////////

	th_rcs_fwd[0]  = CreateThruster (_V(0,0,0),  _V(0,0, 1), MAXTRHUST_RCS * 2.5, fuel_sys, ISPMAX_RCS); // fore-aft RCS is intentionally overpowered wrt the other sets...
	th_rcs_back[0] = CreateThruster (_V(0,0,0),  _V(0,0,-1), MAXTRHUST_RCS * 2.5, fuel_sys, ISPMAX_RCS); // this is to make "braking" easier, since there aren't any beefy retro thrusters aboard

	

	//
	AddExhaust (th_rcs_back[0], 2.75, 0.5, V3_RCSF_RFW_1, _V(0,0,1), rcsTex);
	AddExhaust (th_rcs_back[0], 2.75, 0.5, V3_RCSF_RFW_2, _V(0,0,1), rcsTex);
	AddExhaust (th_rcs_back[0], 2.75, 0.5, V3_RCSF_RFW_3, _V(0,0,1), rcsTex);
	AddExhaust (th_rcs_back[0], 2.75, 0.5, V3_RCSF_LFW_1, _V(0,0,1), rcsTex);
	AddExhaust (th_rcs_back[0], 2.75, 0.5, V3_RCSF_LFW_2, _V(0,0,1), rcsTex);
	AddExhaust (th_rcs_back[0], 2.75, 0.5, V3_RCSF_LFW_3, _V(0,0,1), rcsTex);

	AddExhaust (th_rcs_fwd[0], 2.75, 0.5, V3_RCSA_BK_1, _V(0,0,-1), rcsTex);
	AddExhaust (th_rcs_fwd[0], 2.75, 0.5, V3_RCSA_BK_2, _V(0,0,-1), rcsTex);
	AddExhaust (th_rcs_fwd[0], 2.75, 0.5, V3_RCSA_BK_3, _V(0,0,-1), rcsTex);
	AddExhaust (th_rcs_fwd[0], 2.75, 0.5, V3_RCSA_BK_4, _V(0,0,-1), rcsTex);
	AddExhaust (th_rcs_fwd[0], 2.75, 0.5, V3_RCSA_BK_5, _V(0,0,-1), rcsTex);
	AddExhaust (th_rcs_fwd[0], 2.75, 0.5, V3_RCSA_BK_6, _V(0,0,-1), rcsTex);


	///////////////////////////

	th_rcs_bankRight[0] = CreateThruster (_V( 10,0,0), _V(0,  1, 0),  MAXTRHUST_RCS  * 2.0, fuel_sys, ISPMAX_RCS);   // raise port
	th_rcs_bankRight[1] = CreateThruster (_V(-10,0,0), _V(0, -1, 0),  MAXTRHUST_RCS  * 2.0, fuel_sys, ISPMAX_RCS);   // drop starboard

	th_rcs_bankLeft[0] = CreateThruster (_V( 10,0,0), _V(0, -1, 0),  MAXTRHUST_RCS  * 2.0, fuel_sys, ISPMAX_RCS);   // drop port
	th_rcs_bankLeft[1] = CreateThruster (_V(-10,0,0), _V(0,  1, 0),  MAXTRHUST_RCS  * 2.0, fuel_sys, ISPMAX_RCS);   // raise starboard

	

	AddExhaust (th_rcs_bankLeft[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSR_UP_001, _V(0,1,0), rcsTex); // drop right
	AddExhaust (th_rcs_bankLeft[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSR_UP_002, _V(0,1,0), rcsTex);
	AddExhaust (th_rcs_bankLeft[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSR_UP_003, _V(0,1,0), rcsTex);
	AddExhaust (th_rcs_bankLeft[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSR_UP_004, _V(0,1,0), rcsTex);

	AddExhaust (th_rcs_bankLeft[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSL_DN_001, _V(0,-1,0), rcsTex); // raise left
	AddExhaust (th_rcs_bankLeft[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSL_DN_002, _V(0,-1,0), rcsTex);
	AddExhaust (th_rcs_bankLeft[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSL_DN_003, _V(0,-1,0), rcsTex);
	AddExhaust (th_rcs_bankLeft[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSL_DN_004, _V(0,-1,0), rcsTex);
	
	AddExhaust (th_rcs_bankRight[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSR_DN_001, _V(0,-1,0), rcsTex); // raise right
	AddExhaust (th_rcs_bankRight[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSR_DN_002, _V(0,-1,0), rcsTex);
	AddExhaust (th_rcs_bankRight[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSR_DN_003, _V(0,-1,0), rcsTex);
	AddExhaust (th_rcs_bankRight[0], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSR_DN_004, _V(0,-1,0), rcsTex);
				
	AddExhaust (th_rcs_bankRight[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSL_UP_001, _V(0,1,0), rcsTex); // drop left
	AddExhaust (th_rcs_bankRight[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSL_UP_002, _V(0,1,0), rcsTex);
	AddExhaust (th_rcs_bankRight[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSL_UP_003, _V(0,1,0), rcsTex);
	AddExhaust (th_rcs_bankRight[1], RCS_FX_LSCALE, RCS_FX_WSCALE, V3_RCSL_UP_004, _V(0,1,0), rcsTex);
	
	//
	//
	setupAnimations();
	
}


void G422::clbkPostCreation()
{
	SFXID = ConnectToOrbiterSoundDLL3( GetHandle() );

	//
	//
	SoundOptionOnOff3(SFXID, PLAYCOUNTDOWNWHENTAKEOFF, false);
	SoundOptionOnOff3(SFXID, PLAYMAINTHRUST, false);
	SoundOptionOnOff3(SFXID, PLAYHOVERTHRUST, false);
	
	
	ReplaceStockSound3(SFXID, "Sound\\G422\\amb.wav",         REPLACE_AIR_CONDITIONNING);
//	ReplaceStockSound3(SFXID, "Sound\\G422\\RCSfire.wav",    REPLACE_RCS_THRUST_ATTACK);
//	ReplaceStockSound3(SFXID, "Sound\\G422\\RCSsustain.wav", REPLACE_RCS_THRUST_SUSTAIN);
	
	RequestLoadVesselWave3(SFXID, SFX_GEARS, "Sound\\G422\\gears.wav",        INTERNAL_ONLY);
	RequestLoadVesselWave3(SFXID, SFX_CNRDS, "Sound\\G422\\canards.wav",      INTERNAL_ONLY);
	RequestLoadVesselWave3(SFXID, SFX_VSRUP, "Sound\\G422\\visor_raise.wav", INTERNAL_ONLY);
	RequestLoadVesselWave3(SFXID, SFX_VSRDN, "Sound\\G422\\visor_lower.wav", INTERNAL_ONLY);
	
	RequestLoadVesselWave3(SFXID, SFX_FUELPUMP_MAIN, "Sound\\G422\\fuelpump_main.wav", INTERNAL_ONLY);
	RequestLoadVesselWave3(SFXID, SFX_FUELPUMP_OXY,  "Sound\\G422\\fuelpump_oxy.wav",  INTERNAL_ONLY);
	RequestLoadVesselWave3(SFXID, SFX_FUELPUMP_APU,  "Sound\\G422\\fuelpump_apu.wav",  INTERNAL_ONLY);
	RequestLoadVesselWave3(SFXID, SFX_FUELPUMP_RAMX, "Sound\\G422\\fuelpump_ramx.wav", INTERNAL_ONLY);
	RequestLoadVesselWave3(SFXID, SFX_FUELPUMP_OMS, "Sound\\G422\\fuelpump_oms.wav", INTERNAL_ONLY);
	RequestLoadVesselWave3(SFXID, SFX_FUELPUMP_RCS, "Sound\\Vessel\\aircond.wav", INTERNAL_ONLY);
	
	RequestLoadVesselWave3(SFXID, SFX_APU_START, "Sound\\G422\\apu_start.wav", INTERNAL_ONLY);
	RequestLoadVesselWave3(SFXID, SFX_APU_RUN,   "Sound\\G422\\apu_run.wav",   INTERNAL_ONLY);
	RequestLoadVesselWave3(SFXID, SFX_APU_STOP,  "Sound\\G422\\apu_stop.wav",  INTERNAL_ONLY);
	
	RequestLoadVesselWave3(SFXID, SFX_VC_POP,   "Sound\\G422\\cockpit\\pop.wav",   INTERNAL_ONLY);
	RequestLoadVesselWave3(SFXID, SFX_VC_FLICK, "Sound\\G422\\cockpit\\flick.wav", INTERNAL_ONLY);
	RequestLoadVesselWave3(SFXID, SFX_VC_AFLIK, "Sound\\G422\\cockpit\\auto_flipback.wav", INTERNAL_ONLY);
	RequestLoadVesselWave3(SFXID, SFX_VC_BEEP,  "Sound\\G422\\cockpit\\beep.wav",  INTERNAL_ONLY);
	RequestLoadVesselWave3(SFXID, SFX_VC_BLIP,  "Sound\\G422\\cockpit\\blip.wav",  INTERNAL_ONLY);
	RequestLoadVesselWave3(SFXID, SFX_VC_CLAK,  "Sound\\G422\\cockpit\\clamp.wav", INTERNAL_ONLY);
	RequestLoadVesselWave3(SFXID, SFX_VC_TICK,  "Sound\\G422\\cockpit\\tick.wav",  INTERNAL_ONLY);
	RequestLoadVesselWave3(SFXID, SFX_VC_SLACK, "Sound\\G422\\cockpit\\shclack.wav",  INTERNAL_ONLY);
	
	RequestLoadVesselWave3(SFXID, SFX_WINGMTR, "Sound\\G422\\wing_motor.wav",  INTERNAL_ONLY);
	
	RequestLoadVesselWave3(SFXID, SFX_WIND_CLSD, "Sound\\G422\\wind_vsr_up.wav",   INTERNAL_ONLY);
	RequestLoadVesselWave3(SFXID, SFX_WIND_OPEN, "Sound\\G422\\wind_vsr_down.wav", INTERNAL_ONLY);
	
	RequestLoadVesselWave3(SFXID, SFX_JETROAR, "Sound\\G422\\roar.wav",  BOTHVIEW_FADED_FAR);
	RequestLoadVesselWave3(SFXID, SFX_RCTROAR, "Sound\\G422\\main_burn_rct.wav",    BOTHVIEW_FADED_FAR);
	RequestLoadVesselWave3(SFXID, SFX_AFTERBURNER, "Sound\\G422\\afterburner.wav",  BOTHVIEW_FADED_FAR);
	RequestLoadVesselWave3(SFXID, SFX_OMSBURN, "Sound\\Vessel\\roll.wav",  BOTHVIEW_FADED_FAR);
	//
	RequestLoadVesselWave3(SFXID, SFX_ENGINERUN_MAIN_EXT,   "Sound\\G422\\main_run_gen.wav",    BOTHVIEW_FADED_MEDIUM);
	RequestLoadVesselWave3(SFXID, SFX_ENGINESTART_MAIN_EXT, "Sound\\G422\\main_start_gen.wav",  BOTHVIEW_FADED_MEDIUM);
	RequestLoadVesselWave3(SFXID, SFX_ENGINESTOP_MAIN_EXT,  "Sound\\G422\\main_stop_gen.wav",   BOTHVIEW_FADED_MEDIUM);
	
	RequestLoadVesselWave3(SFXID, SFX_ENGINESTART_MAIN_INT,  "Sound\\G422\\main_start_rct.wav", BOTHVIEW_FADED_MEDIUM);
	RequestLoadVesselWave3(SFXID, SFX_ENGINERUN_MAIN_INT,    "Sound\\G422\\main_run_rct.wav",   BOTHVIEW_FADED_MEDIUM);
	RequestLoadVesselWave3(SFXID, SFX_ENGINESTOP_MAIN_INT,   "Sound\\G422\\main_stop_rct.wav",  BOTHVIEW_FADED_MEDIUM);
	
	RequestLoadVesselWave3(SFXID, SFX_ENGINERUN_RAMX_LO, "Sound\\G422\\ramcaster_run_lo.wav",   BOTHVIEW_FADED_MEDIUM);
	RequestLoadVesselWave3(SFXID, SFX_ENGINERUN_RAMX_HI, "Sound\\G422\\ramcaster_run_hi.wav",   BOTHVIEW_FADED_MEDIUM);
	RequestLoadVesselWave3(SFXID, SFX_RAMCASTER_TRST_UP, "Sound\\G422\\ramcaster_switch_up.wav",   BOTHVIEW_FADED_MEDIUM);
	RequestLoadVesselWave3(SFXID, SFX_RAMCASTER_START,   "Sound\\G422\\ramcaster_engage_nmnl.wav", BOTHVIEW_FADED_MEDIUM);
	RequestLoadVesselWave3(SFXID, SFX_RAMXBURN,          "Sound\\G422\\ramcaster_dry.wav", BOTHVIEW_FADED_MEDIUM);
	
	
	//
	oapiVCTriggerRedrawArea(-1, VC_AREA_MFDKEYS);
	oapiVCTriggerRedrawArea(-1, VC_AREA_EICAS_ALL);
	//oapiVCTriggerRedrawArea(-1, VC_AREA_FUELRES);

	SetAnimation(an_dvc_wpos, wingTipFthr->getToggleState()? 1.0 : wingTipWvrd->getToggleState()? 0.0 : .5);
	
	//6
	CreateVariableDragElement(&(landingGears->pos), 8.5, _V(0.0, -3.8, 0.0));
	CreateVariableDragElement(&(canards->pos),      1.6, _V(0.0,  2.0, 35.0));
	CreateVariableDragElement(&(inltDoors->pos),    3.0, _V(0.0,  -0.8, -2.0));
	CreateVariableDragElement(&(ramxDoors->pos),    2.5, _V(0.0,  -2.2,  2.0));
	
	
	
	
	//canards->sysReset = false; // gotta update this to get that control surface working
	
	allSystemsReset = false;
}





void G422::clbkVisualCreated(VISHANDLE vis, int refcount)
{
	visual = vis;
	//

	for (int i=0; i<6; i++)
	{
		selEicasMode(i, VC_eicas_screens[i]);
		//
	}
	
	
}