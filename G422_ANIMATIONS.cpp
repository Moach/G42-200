
#include "G422.h"
#include "G422_MDL_EXT.h"
#include "G422_MDL_DVC.h"


// gears...
//
#define V3_GEAR_HATCH_R _V(2.3561, -2.37101, -4.12656)
#define V3_GEAR_STRUT_R _V(4.07604, -3.58362, -4.33409)
#define V3_GEAR_STRUT_L _V(-4.076, -3.58362, -4.33409)
#define V3_GEAR_HATCH_L _V(-2.356, -2.37101, -4.12656)
#define V3_GEAR_FRAME_L _V(-8.77, 1.1714, -4.00951)
#define V3_GEAR_FRAME_R _V( 8.77, 1.1714, -4.00951)


#define VHINGE_PVT_GEAR_DOOR_R _V( 6.25936, -1.91533, -7.21245), _V( 0.0742465, -0.000549689, 0.99724)
#define VHINGE_PVT_GEAR_DOOR_L _V(-6.25936, -1.91533, -7.21245), _V(-0.0742465, -0.000549689, 0.99724)

#define V3_GEAR_DOOR_C_FR _V( 0.731, -0.00371313, 39.192)
#define V3_GEAR_DOOR_C_FL _V(-0.731, -0.00371313, 39.192)
#define V3_GEAR_DOOR_C_AL _V(-0.609, -0.266117, 36.1358)
#define V3_GEAR_DOOR_C_AR _V( 0.609, -0.266117, 36.1358)

#define V3_GEAR_STRUT_C _V(-0.0740595, 0.186456, 34.6558)

#define VHINGE_PVT_TWDOOR_L _V( 0.302762, -0.153345, -34.3016), _V( 0.00629226, 0.0785203, -0.996893)
#define VHINGE_PVT_TWDOOR_R _V(-0.302762, -0.153345, -34.3016), _V(-0.00629226, 0.0785203, -0.996893)
#define V3_TAIL_WHEEL _V(0.0, 0.00187147, -34.4394)


// visor...
//
#define V3_VISOR_FRONT_OUT _V(0.630102, 3.05472, 41.3294)
#define V3_VISOR_AFT_IN _V(0.630099, -0.904581, 37.1685)


// canards...
//
#define V3_CANARD_DOOR_R _V(-1.6796, 2.68228, 35.7218)
#define V3_CANARD_WING_R _V( 1.327, 1.13419, 39.3921)
#define V3_CANARD_DOOR_L _V( 1.6796, 2.68228, 35.7218)
#define V3_CANARD_WING_L _V(-1.327, 1.13419, 39.3921)
#define VHINGE_PVT_CNRD_COVER_R _V(0.0, 1.23179, 38.7731), _V( 0.00691653, 0.994608, 0.103476)
#define VHINGE_PVT_CNRD_COVER_L _V(0.0, 1.23179, 38.7731), _V(-0.00691653, 0.994608, 0.103476)


// control surfaces...
//
#define VHINGE_CNRD_ACS_R _V( 1.76121, 1.15581, 38.7872), _V( 0.883175, 0.0406976, -0.467275)
#define VHINGE_CNRD_ACS_L _V(-1.76121, 1.15581, 38.7872), _V(-0.883175, 0.0406976, -0.467275)

#define VHINGE_PVT_RUDDER _V(0.0, 4.24414, -29.8109), _V(0.0, 0.983387, 0.181523)

#define V3_WING_ACS_INB_R _V( 6.73334, -0.64278, -16.9604)
#define V3_WING_ACS_INB_L _V(-6.73334, -0.64278, -16.9604)

#define VHINGE_PVT_ACS_R _V( 15.6702, -1.17023, -19.8428), _V( 0.985346, 2.09872e-005, 0.170567)
#define VHINGE_PVT_ACS_L _V(-15.6702, -1.17023, -19.8428), _V(-0.985346, 2.09872e-005, 0.170567)

// RCS doors
//
#define VHINGE_RCS_DOOR_R _V( 1.41042, 0.439932, 36.586), _V( 0.05531, -0.00197669, -0.998467)
#define VHINGE_RCS_DOOR_L _V(-1.41042, 0.439932, 36.586), _V(-0.05531, -0.00197669, -0.998467)


// cargo bay...
//

#define V3_BAY_DOOR_L _V(-2.119, 1.96815, 28.1852)
#define V3_BAY_DOOR_R _V( 2.119, 1.96815, 28.1852)

// bay antenna...
//
#define V3_ANT_DISH _V(-1.57284, 2.11666, 33.0636)



// inlet doors...
//
#define V3_MAIN_INTAKE_DOORS _V(0.0, 12.7809, 2.48512)

// ramcaster doors and slats...
//
#define V3_RAMX_DOOR _V(0.0, -1.74215, 21.057)
#define V3_RAMX_LOWER_SLAT _V(0.0, -3.90982, 8.84999)
#define V3_RAMX_UPPER_SLAT _V(0.0, -8.47985, 21.0105)


// waverider wingtips...
//
#define VHINGE_PVT_WRTIP_R _V( 14.6442, -1.35756, -22.2521), _V(-0.0258672, 0.00946881, 0.999621)
#define VHINGE_PVT_WRTIP_L _V(-14.6442, -1.35756, -22.2521), _V( 0.0258672, 0.00946881, 0.999621)
#define VHINGE_PVT_WRMID_R _V( 12.1279, -0.991496, -20.6341), _V( 0.00270178, -0.00566354, 0.99998)
#define VHINGE_PVT_WRMID_L _V(-12.1279, -0.991496, -20.6341), _V(-0.00270178, -0.00566354, 0.99998)


// DVC stuff...
//
//

// ramcaster throttle lever
#define V3_RAMX_THR _V(-0.00373119, 2.17513, 38.8959)

// wing pos lever
#define V3_WING_POS_LEVER _V(-0.00373119, 2.16365, 38.7879)

// stick and throttle unit
#define V3_CTRL_STICK _V(0.595641, 2.19661, 38.8979)
#define V3_THROTTLE_UNIT _V(0.0989042, 1.85421, 38.9147)


// nosewheel tiller
#define POSDIR_GND_STEER_TILLER _V(0.71939, 2.2042, 38.7936), _V(0.0, -0.991703, 0.128547)



// ADI ball thingy (thanks Martin for the awesome texture!!)
#define V3_ADI_BALL _V(-3.05176e-005, 2.79184, 39.08)
#define V3_ADI_ROLLAXIS _V(0.0, -0.573357, -0.819306)



void G422::setupAnimations()
{
	//
	//
	
	// landing gears...
	//
	UINT an_lg = CreateAnimation(0.0);
	
	static UINT lgTrucks[8] = {MGID_GEAR_TRUCK_L, MGID_GEAR_STRUT_L, MGID_GEAR_TRUCK_R, MGID_GEAR_STRUT_R, 
								MGID_GEAR_TRUCK_C, MGID_GEAR_STRUT_C, MGID_GEAR_FRAME_L, MGID_GEAR_FRAME_R};
								
	static UINT lgDoors[8]  = {MGID_GEAR_DOOR_L, MGID_GEAR_HATCH_L, MGID_GEAR_DOOR_R, MGID_GEAR_HATCH_R,		
								MGID_GEAR_DOOR_C_AL, MGID_GEAR_DOOR_C_AR, MGID_GEAR_DOOR_C_FL, MGID_GEAR_DOOR_C_FR};
								
	static UINT tailWheel[3] = {MGID_TAIL_WHEEL, MGID_TAILWHEEL_DOOR_R, MGID_TAILWHEEL_DOOR_L};
						
						
	static MGROUP_ROTATE mgRot_lgdoor_op1_L(mdlID_ext, lgDoors,   1, VHINGE_PVT_GEAR_DOOR_L, (float)(-125 * RAD)); // open main doors fully
	static MGROUP_ROTATE mgRot_lgdoor_op1_R(mdlID_ext, lgDoors+2, 1, VHINGE_PVT_GEAR_DOOR_R, (float)( 125 * RAD));
	//
	AddAnimationComponent(an_lg, 0.0, 0.3, &mgRot_lgdoor_op1_L); 
	AddAnimationComponent(an_lg, 0.0, 0.3, &mgRot_lgdoor_op1_R);
	
	
	static MGROUP_ROTATE mgRot_lgdoor_op1_CFL(mdlID_ext, lgDoors+6,   1, V3_GEAR_DOOR_C_FL, _V(0,0,1), (float)(-90 * RAD)); // open fore-center doors
	static MGROUP_ROTATE mgRot_lgdoor_op1_CFR(mdlID_ext, lgDoors+7,   1, V3_GEAR_DOOR_C_FR, _V(0,0,1), (float)( 90 * RAD));
	//
	AddAnimationComponent(an_lg, 0.0, 0.2, &mgRot_lgdoor_op1_CFL);
	AddAnimationComponent(an_lg, 0.0, 0.2, &mgRot_lgdoor_op1_CFR);
	
	static MGROUP_ROTATE mgRot_lgdoor_CAL(mdlID_ext, lgDoors+4,   1, V3_GEAR_DOOR_C_AL, _V(0,0,1), (float)(-90 * RAD)); // open aft-center doors
	static MGROUP_ROTATE mgRot_lgdoor_CAR(mdlID_ext, lgDoors+5,   1, V3_GEAR_DOOR_C_AR, _V(0,0,1), (float)( 90 * RAD));
	//
	AddAnimationComponent(an_lg, 0.0, 0.2, &mgRot_lgdoor_CAL);
	AddAnimationComponent(an_lg, 0.0, 0.2, &mgRot_lgdoor_CAR);
	
	
	static MGROUP_ROTATE mgRot_lgbogie_L(mdlID_ext, lgTrucks,   2, V3_GEAR_STRUT_L, _V(0,0, 1), (float)( 145*RAD)); // extend  main gears
	static MGROUP_ROTATE mgRot_lgbogie_R(mdlID_ext, lgTrucks+2, 2, V3_GEAR_STRUT_R, _V(0,0, 1), (float)(-145*RAD));
	static MGROUP_ROTATE mgRot_lghatch_L(mdlID_ext, lgDoors+1,  1, V3_GEAR_HATCH_L, _V(0,0, 1), (float)( 30*RAD));  // sync with lower hatches
	static MGROUP_ROTATE mgRot_lghatch_R(mdlID_ext, lgDoors+3,  1, V3_GEAR_HATCH_R, _V(0,0, 1), (float)(-30*RAD));
	static MGROUP_ROTATE mgRot_lgframe_L(mdlID_ext, lgTrucks+6,  1, V3_GEAR_FRAME_L, _V(0,0, 1), (float)(-15*RAD));  // and frames as well...
	static MGROUP_ROTATE mgRot_lgframe_R(mdlID_ext, lgTrucks+7,  1, V3_GEAR_FRAME_R, _V(0,0, 1), (float)( 15*RAD));
	//
	AddAnimationComponent(an_lg, 0.2, 0.8, &mgRot_lgbogie_L); 
	AddAnimationComponent(an_lg, 0.2, 0.8, &mgRot_lgbogie_R);
	AddAnimationComponent(an_lg, 0.0, 0.8, &mgRot_lgframe_L); 
	AddAnimationComponent(an_lg, 0.0, 0.8, &mgRot_lgframe_R);
	AddAnimationComponent(an_lg, 0.10, 0.8, &mgRot_lghatch_L);
	AddAnimationComponent(an_lg, 0.10, 0.8, &mgRot_lghatch_R);
	
	static MGROUP_ROTATE mgRot_lgbogie_C(mdlID_ext, lgTrucks+4, 2, V3_GEAR_STRUT_C, _V(1,0, 0), (float)(95*RAD)); // extend nose gear
	//
	AddAnimationComponent(an_lg, 0.1, 0.8, &mgRot_lgbogie_C);
	
	static MGROUP_ROTATE mgRot_lgdoor_op2_CFL(mdlID_ext, lgDoors+6,   1, V3_GEAR_DOOR_C_FL, _V(0,0,1), (float)( 90 * RAD)); // close fore-center doors
	static MGROUP_ROTATE mgRot_lgdoor_op2_CFR(mdlID_ext, lgDoors+7,   1, V3_GEAR_DOOR_C_FR, _V(0,0,1), (float)(-90 * RAD));
	//
	AddAnimationComponent(an_lg, 0.5, 0.7, &mgRot_lgdoor_op2_CFL);
	AddAnimationComponent(an_lg, 0.5, 0.7, &mgRot_lgdoor_op2_CFR);
	
	static MGROUP_ROTATE mgRot_lgdoor_op2_L(mdlID_ext, lgDoors,   1, VHINGE_PVT_GEAR_DOOR_L, (float)( 80 * RAD)); // close doors half-way
	static MGROUP_ROTATE mgRot_lgdoor_op2_R(mdlID_ext, lgDoors+2, 1, VHINGE_PVT_GEAR_DOOR_R, (float)(-80 * RAD));
	//
	AddAnimationComponent(an_lg, 0.75, 1.0, &mgRot_lgdoor_op2_L);
	AddAnimationComponent(an_lg, 0.75, 1.0, &mgRot_lgdoor_op2_R);
	
	
	// lastly, that little tail-strike wheel
	static MGROUP_ROTATE mgRot_tswdoor_L(mdlID_ext, tailWheel+1, 1, VHINGE_PVT_TWDOOR_L, (float)(-90 * RAD));
	static MGROUP_ROTATE mgRot_tswdoor_R(mdlID_ext, tailWheel+2, 1, VHINGE_PVT_TWDOOR_R, (float)( 90 * RAD));
	static MGROUP_ROTATE mgRot_tailwheel(mdlID_ext, tailWheel, 1, V3_TAIL_WHEEL, _X,  (float)(-65 * RAD));
	//
	AddAnimationComponent(an_lg, 0.65, 0.8, &mgRot_tswdoor_L);
	AddAnimationComponent(an_lg, 0.65, 0.8, &mgRot_tswdoor_R);
	AddAnimationComponent(an_lg, 0.70, 0.9, &mgRot_tailwheel);
	
	//
	landingGears = new MovingPart(an_lg, 0.1, MP_HI_DETENT, 1.0, this, G422_SYSID_LGRS);
	//
	//
	
	
	
	
	// visor
	//
	UINT an_vsr = CreateAnimation(0.0);
	
	static UINT vsrFore[3] = {MGID_VISOR_FRONT_IN, MGID_VISOR_FRONT_OUT, MGID_VISOR_EXT_FORE};
	static UINT vsrAft[5]  = {MGID_VISOR_AFT_IN, MGID_VISOR_WINDOW_FRAME, MGID_VISOR_MECH, MGID_VISOR_EXT_AFT, MGID_VISOR_EXT_WINDOWS};
	
	static MGROUP_ROTATE mgRot_vsrFore_ext(mdlID_ext, vsrFore+2, 1, V3_VISOR_FRONT_OUT, _V(1,0,0), (float)(10 * RAD));
	static MGROUP_ROTATE mgRot_vsrFore_dvc(mdlID_dvc, vsrFore,   2, V3_VISOR_FRONT_OUT, _V(1,0,0), (float)(10 * RAD));
	//
	AddAnimationComponent(an_vsr, 0.0, 0.4, &mgRot_vsrFore_ext);
	AddAnimationComponent(an_vsr, 0.0, 0.4, &mgRot_vsrFore_dvc);
	
	static MGROUP_ROTATE mgRot_vsrAft_ext(mdlID_ext, vsrAft+3, 2, V3_VISOR_AFT_IN, _V(1,0,0), (float)(-15 * RAD));
	static MGROUP_ROTATE mgRot_vsrAft_dvc(mdlID_dvc, vsrAft,   3, V3_VISOR_AFT_IN, _V(1,0,0), (float)(-15 * RAD));
	//
	AddAnimationComponent(an_vsr, 0.3, 1.0, &mgRot_vsrAft_ext);
	AddAnimationComponent(an_vsr, 0.3, 1.0, &mgRot_vsrAft_dvc);
	
	//
	visor = new MovingPart(an_vsr, 0.15, MP_LOW_DETENT, 0.0, this, G422_SYSID_VISR);
	
	
	
	



	// RCS FWD door
	//
	
	UINT an_rcs = CreateAnimation(0.0);
	static UINT f_rcs[4] = {MGID_RCS_DOOR_L, MGID_RCS_THRUSTERS_L, MGID_RCS_DOOR_R, MGID_RCS_THRUSTERS_R};
	static MGROUP_ROTATE mgRot_rcsL(mdlID_ext, f_rcs,   2, VHINGE_RCS_DOOR_L, (float)( 75*RAD));
	static MGROUP_ROTATE mgRot_rcsR(mdlID_ext, f_rcs+2, 2, VHINGE_RCS_DOOR_R, (float)(-75*RAD));
	AddAnimationComponent(an_rcs, 0.0, 1.0, &mgRot_rcsL);
	AddAnimationComponent(an_rcs, 0.0, 1.0, &mgRot_rcsR);
	//
	rcs = new MovingPart(an_rcs, 0.3, MP_LOW_DETENT, 0.0, this, G422_SYSID_RCSDR);
	
	
	
	// inlet doors...
	//
	UINT an_inlt = CreateAnimation(0.0);
	static UINT inlet = MGID_MAIN_INTAKE_DOORS;
	static MGROUP_ROTATE mgRot_inletDoors(mdlID_ext, &inlet,   1, V3_MAIN_INTAKE_DOORS, _V(1,0,0), (float)(-10*RAD)); // close doors
	AddAnimationComponent(an_inlt, 0.1, 0.9, &mgRot_inletDoors);
	//
	inltDoors = new MovingPart(an_inlt, 0.3, MP_HI_DETENT, 1.0, this, G422_SYSID_INLT);





	// ramcaster doors
	//
	UINT an_ramx = CreateAnimation(0.0);
	static UINT rcDoors[3] = {MGID_RAMX_DOOR, MGID_RAMX_LOWER_SLAT, MGID_RAMX_UPPER_SLAT};
	static MGROUP_ROTATE mgRot_rcDoor(mdlID_ext, rcDoors,   1,    V3_RAMX_DOOR,       _V(1,0,0), (float)( 10*RAD));
	static MGROUP_ROTATE mgRot_rcLwrSlat(mdlID_ext, rcDoors+1, 1, V3_RAMX_LOWER_SLAT, _V(1,0,0), (float)( 12.5*RAD));
	AddAnimationComponent(an_ramx, 0.1, 0.9,  &mgRot_rcDoor);
	AddAnimationComponent(an_ramx, 0.15, 0.4, &mgRot_rcLwrSlat);
	//
	ramxDoors = new MovingPart(an_ramx, 0.25, MP_LOW_DETENT, 0.0, this, G422_SYSID_RAMX);


	


	
	
	// canards
	//
	UINT an_cnrd = CreateAnimation(0.0);
	static UINT cnrds[6] = {MGID_CANARD_WING_L, MGID_CANARD_COVER_L, MGID_CANARD_DOOR_L, MGID_CANARD_WING_R,  MGID_CANARD_COVER_R, MGID_CANARD_DOOR_R};
	
	static MGROUP_ROTATE mgRot_cnrdDoor_op1_L(mdlID_ext, cnrds+2,   1, V3_CANARD_DOOR_L, _V(0,0,1), (float)( 5*RAD)); // open doors
	static MGROUP_ROTATE mgRot_cnrdDoor_op1_R(mdlID_ext, cnrds+5,   1, V3_CANARD_DOOR_R, _V(0,0,1), (float)(-5*RAD));
	static MGROUP_ROTATE mgRot_cnrdCover_L(mdlID_ext, cnrds+1, 1, VHINGE_PVT_CNRD_COVER_L, (float)( 25*RAD)); // swing in fore covers
	static MGROUP_ROTATE mgRot_cnrdCover_R(mdlID_ext, cnrds+4, 1, VHINGE_PVT_CNRD_COVER_R, (float)(-25*RAD));
	//
	AddAnimationComponent(an_cnrd, 0.0, 0.1, &mgRot_cnrdDoor_op1_L);
	AddAnimationComponent(an_cnrd, 0.0, 0.1, &mgRot_cnrdDoor_op1_R);
	AddAnimationComponent(an_cnrd, 0.0, 0.1, &mgRot_cnrdCover_L);
	AddAnimationComponent(an_cnrd, 0.0, 0.1, &mgRot_cnrdCover_R);
	
	static MGROUP_ROTATE mgRot_cnrdWings_L(mdlID_ext, cnrds,   1, V3_CANARD_WING_L, _V(0,1,0), (float)( 55*RAD)); // swing out canards
	static MGROUP_ROTATE mgRot_cnrdWings_R(mdlID_ext, cnrds+3, 1, V3_CANARD_WING_R, _V(0,1,0), (float)(-55*RAD));
	
	//
	AddAnimationComponent(an_cnrd, 0.1, 0.9, &mgRot_cnrdWings_L);
	AddAnimationComponent(an_cnrd, 0.1, 0.9, &mgRot_cnrdWings_R);
	
	
	static MGROUP_ROTATE mgRot_cnrdDoor_op2_L(mdlID_ext, cnrds+2,   1, V3_CANARD_DOOR_L, _V(0,0,1), (float)(-5*RAD)); // close doors
	static MGROUP_ROTATE mgRot_cnrdDoor_op2_R(mdlID_ext, cnrds+5,   1, V3_CANARD_DOOR_R, _V(0,0,1), (float)( 5*RAD));
	//
	AddAnimationComponent(an_cnrd, 0.9, 1.0, &mgRot_cnrdDoor_op2_L);
	AddAnimationComponent(an_cnrd, 0.9, 1.0, &mgRot_cnrdDoor_op2_R);
	
	canards = new MovingPart(an_cnrd, 0.12, MP_HI_DETENT, 1.0, this, G422_SYSID_CNRD);
	
	
	
	
	
	// cargo bay doors
	//
	UINT an_cgoBay = CreateAnimation(0.0);
	static UINT cgoBay[5] = {MGID_BAY_DOOR_L, MGID_BAY_DOOR_INT_L, MGID_BAY_DOOR_R, MGID_BAY_DOOR_INT_R, MGID_ANT_DISH};
	
	static MGROUP_ROTATE mgRot_bayDoor_L(mdlID_ext, cgoBay,   2, V3_BAY_DOOR_L, _Z, (float)( 180.0*RAD));
	static MGROUP_ROTATE mgRot_bayDoor_R(mdlID_ext, cgoBay+2, 2, V3_BAY_DOOR_R, _Z, (float)(-180.0*RAD));
	static MGROUP_ROTATE mgRot_antenna (mdlID_ext, cgoBay+4, 1, V3_ANT_DISH,	  _Y, (float)( 130.0*RAD));
	
	AddAnimationComponent(an_cgoBay, 0.01, 0.47, &mgRot_bayDoor_L);
	AddAnimationComponent(an_cgoBay, 0.49, 0.95, &mgRot_bayDoor_R);
	AddAnimationComponent(an_cgoBay, 0.96, 1.00, &mgRot_antenna);
	
	bayDoors = new MovingPart(an_cgoBay, 0.01, MP_LOW_DETENT, 0.0, this);
	
	
	
	
	

	//  wingtips (acually there's two animations for those)
	//

	static UINT wings[4] = {MGID_WING_WRDROP_TIP_L, MGID_WING_FTHR_L, MGID_WING_WRDROP_TIP_R, MGID_WING_FTHR_R};



	UINT an_wingsUp = CreateAnimation(0.0);
	static MGROUP_ROTATE mgROt_wup_L(mdlID_ext, wings+1, 1, VHINGE_PVT_WRMID_L, (float)(-35*RAD));
	static MGROUP_ROTATE mgROt_wup_R(mdlID_ext, wings+3, 1, VHINGE_PVT_WRMID_R, (float)( 35*RAD));
	//
	ANIMATIONCOMPONENT_HANDLE wingPar_L = AddAnimationComponent(an_wingsUp, 0.0, 1.0, &mgROt_wup_L);
	ANIMATIONCOMPONENT_HANDLE wingPar_R = AddAnimationComponent(an_wingsUp, 0.0, 1.0, &mgROt_wup_R);


	UINT an_wingsDown = CreateAnimation(0.0);
	//static MGROUP_ROTATE mgROt_wdn_L(mdlID_ext, wings,   2, VHINGE_PVT_WRTIP_L, (float)( 25*RAD));
	//static MGROUP_ROTATE mgROt_wdn_R(mdlID_ext, wings+3, 2, VHINGE_PVT_WRTIP_R, (float)(-25*RAD));
	mgRot_wtL = new MGROUP_ROTATE(mdlID_ext, wings,   1, VHINGE_PVT_WRTIP_L, (float)( 25*RAD));
	mgRot_wtR = new MGROUP_ROTATE(mdlID_ext, wings+2, 1, VHINGE_PVT_WRTIP_R, (float)(-25*RAD));	
	//
	ANIMATIONCOMPONENT_HANDLE acsPar_L = AddAnimationComponent(an_wingsDown, 0.0, 1.0, mgRot_wtL, wingPar_L);
	ANIMATIONCOMPONENT_HANDLE acsPar_R = AddAnimationComponent(an_wingsDown, 0.0, 1.0, mgRot_wtR, wingPar_R);
	
	
	
	wingTipLightsL[0] = V3_LGT_NAV_RED;     wingTipLightsL[1] = V3_LGT_STROBE_L;
	wingTipLightsR[0] = V3_LGT_NAV_GREEN;   wingTipLightsR[1] = V3_LGT_STROBE_R;
	//
	mgRot_wtLgtsL = new MGROUP_ROTATE(LOCALVERTEXLIST, MAKEGROUPARRAY(wingTipLightsL),   2, VHINGE_PVT_WRTIP_L, (float)( 25*RAD));
	mgRot_wtLgtsR = new MGROUP_ROTATE(LOCALVERTEXLIST, MAKEGROUPARRAY(wingTipLightsR), 2,   VHINGE_PVT_WRTIP_R, (float)(-25*RAD));	
	//
	AddAnimationComponent(an_wingsDown, 0.0, 1.0, mgRot_wtLgtsL, wingPar_L);
	AddAnimationComponent(an_wingsDown, 0.0, 1.0, mgRot_wtLgtsR, wingPar_R);
		

	

	wingTipWvrd = new MovingPart(an_wingsDown, 0.15, MP_LOW_DETENT, 0.0, this);
	wingTipFthr = new MovingPart(an_wingsUp,   0.15, MP_LOW_DETENT, 0.0, this);
	//
	
	
	
	
	
	
	
	
	// control surfaces...
	//
	
	
	
	an_acs_cnrd = CreateAnimation(0.5);
	static MGROUP_ROTATE mgRot_cnrdACS_L(mdlID_ext, cnrds,   1, VHINGE_CNRD_ACS_L, (float)(-60*RAD ));
	static MGROUP_ROTATE mgRot_cnrdACS_R(mdlID_ext, cnrds+3, 1, VHINGE_CNRD_ACS_R, (float)( 60*RAD ));
	
	AddAnimationComponent(an_acs_cnrd, 0.0, 1.0, &mgRot_cnrdACS_L);
	AddAnimationComponent(an_acs_cnrd, 0.0, 1.0, &mgRot_cnrdACS_R);
	

	an_acs_rddr = CreateAnimation(0.5);
	static UINT rddr = MGID_RUDDER;
	static MGROUP_ROTATE mgRot_rddr(mdlID_ext, &rddr, 1, VHINGE_PVT_RUDDER, (float)(60*RAD));
	AddAnimationComponent(an_acs_rddr, 0.0,  1.0, &mgRot_rddr);
	

	an_acs_alrn = CreateAnimation(0.5);
	static UINT ailerons[2] = {MGID_WING_ACS_INB_L, MGID_WING_ACS_INB_R};
	static MGROUP_ROTATE mgRot_alrnACS_L(mdlID_ext, ailerons,   1, V3_WING_ACS_INB_L, _X, (float)( 50*RAD ));
	static MGROUP_ROTATE mgRot_alrnACS_R(mdlID_ext, ailerons+1, 1, V3_WING_ACS_INB_R, _X, (float)(-50*RAD ));
	AddAnimationComponent(an_acs_alrn, 0.0, 1.0, &mgRot_alrnACS_L);
	AddAnimationComponent(an_acs_alrn, 0.0, 1.0, &mgRot_alrnACS_R);
		

	an_acs_elvr = CreateAnimation(0.5);

	static UINT elevators[2] = {MGID_WING_ACS_OUT_L, MGID_WING_ACS_OUT_R};
	mgRot_acs_evL = new MGROUP_ROTATE(mdlID_ext, elevators,    1, VHINGE_PVT_ACS_L, (float)( 50*RAD));
	mgRot_acs_evR = new MGROUP_ROTATE(mdlID_ext, elevators+1,  1, VHINGE_PVT_ACS_R, (float)(-50*RAD));	
	AddAnimationComponent(an_acs_elvr, 0.0, 1.0, mgRot_acs_evL, acsPar_L);
	AddAnimationComponent(an_acs_elvr, 0.0, 1.0, mgRot_acs_evR, acsPar_R);




	
	
	
	/*
	// DVC crap!
	//
	
	// ADI ball
	//
	
	// this is a toughie - pay attention to gimbal ordering now...
	//
	an_dvc_adi_A = CreateAnimation(0.5);
	an_dvc_adi_B = CreateAnimation(0.5);
	an_dvc_adi_C = CreateAnimation(0.5);
	//
	static UINT ADI[2] = {MGID_ADI_BALL, MGID_ADI_RING};
	ANIMATIONCOMPONENT_HANDLE gimbal;
	//
	static MGROUP_ROTATE mgRot_ADI_gimbal(mdlID_dvc, ADI, 0, V3_ADI_BALL, V3_ADI_ROLLAXIS, (float)(PI*2.0));
	gimbal = AddAnimationComponent(an_dvc_adi_A, 0.0, 1.0, &mgRot_ADI_gimbal);
	
    mgRot_ADI_G1 = new MGROUP_ROTATE(mdlID_dvc, ADI, 0, V3_ADI_BALL, _Y, (float)(PI*2.0));
    gimbal = AddAnimationComponent(an_dvc_adi_B, 0.0, 1.0, mgRot_ADI_G1, gimbal);
    
	mgRot_ADI_G2 = new MGROUP_ROTATE(mdlID_dvc, ADI, 1, V3_ADI_BALL, _Xn, (float)(PI*2.0));
	AddAnimationComponent(an_dvc_adi_C, 0.0, 1.0, mgRot_ADI_G2, gimbal);
	
	//
	//
*/
// 	
	
	// ramx throttle
	an_dvc_ramx = CreateAnimation(0.0);
	static UINT vcRamxThr[4] = {MGID_RAMX_THR_GRIP, MGID_THR_RAMX_DETENT, MGID_RAMX_THR_FLAP,  MGID_RAMX_THR_STICK};
	static MGROUP_ROTATE mgRot_ramxThr(mdlID_dvc, vcRamxThr, 4, V3_RAMX_THR, _X, (float)(45*RAD));
	AddAnimationComponent(an_dvc_ramx, 0.0, 1.0, &mgRot_ramxThr);
	
	// ctrl stick
	//
	an_dvc_csPitch = CreateAnimation(0.0);
	an_dvc_csRoll  = CreateAnimation(0.0);
	static UINT ctrlStick[3] = {MGID_CTRL_STICK, MGID_STICK_BOOT_1, MGID_STICK_BOOT_2};
	//
	static MGROUP_ROTATE mgRot_stickPitch(mdlID_dvc, ctrlStick,   1, V3_CTRL_STICK, _X, (float)(30*RAD));
	static MGROUP_ROTATE mgRot_boot1Pitch(mdlID_dvc, ctrlStick+1, 1, V3_CTRL_STICK, _X, (float)(20*RAD));
	static MGROUP_ROTATE mgRot_boot2Pitch(mdlID_dvc, ctrlStick+2, 1, V3_CTRL_STICK, _X, (float)(15*RAD));
	AddAnimationComponent(an_dvc_csPitch, 0.0, 1.0, &mgRot_stickPitch);
	AddAnimationComponent(an_dvc_csPitch, 0.0, 1.0, &mgRot_boot1Pitch);
	AddAnimationComponent(an_dvc_csPitch, 0.0, 1.0, &mgRot_boot2Pitch);
	//
	static MGROUP_ROTATE mgRot_stickRoll(mdlID_dvc, ctrlStick,   1, V3_CTRL_STICK, _Zn, (float)(30*RAD));
	static MGROUP_ROTATE mgRot_boot1Roll(mdlID_dvc, ctrlStick+1, 1, V3_CTRL_STICK, _Zn, (float)(20*RAD));
	static MGROUP_ROTATE mgRot_boot2Roll(mdlID_dvc, ctrlStick+2, 1, V3_CTRL_STICK, _Zn, (float)(15*RAD));
	AddAnimationComponent(an_dvc_csRoll, 0.0, 1.0, &mgRot_stickRoll);
	AddAnimationComponent(an_dvc_csRoll, 0.0, 1.0, &mgRot_boot1Roll);
	AddAnimationComponent(an_dvc_csRoll, 0.0, 1.0, &mgRot_boot2Roll);
	
	// throttle unit
	//
	an_dvc_thr = CreateAnimation(0.0);
	static UINT thrUnit[1] = {MGID_THROTTLE_UNIT};
	static MGROUP_ROTATE mgRot_thrUnit(mdlID_dvc, thrUnit, 1, V3_THROTTLE_UNIT, _X, (float)(15*RAD));
	AddAnimationComponent(an_dvc_thr, 0.0, 1.0, &mgRot_thrUnit);
	
	// wing lever
	//
	an_dvc_wpos = CreateAnimation(0.0);
	static UINT vcWPos[1] = {MGID_WING_POS_LEVER};
	static MGROUP_ROTATE mgRot_wPosLvr(mdlID_dvc, vcWPos, 1, V3_WING_POS_LEVER, _X, (float)(65*RAD));
	AddAnimationComponent(an_dvc_wpos, 0.0, 1.0, &mgRot_wPosLvr);

	// nosewheel tiller
	//
	an_dvc_tiller =  CreateAnimation(0.5);
	static UINT vcTiller[1] = {MGID_GND_STEER_TILLER};
	static MGROUP_ROTATE mgRot_tiller(mdlID_dvc, vcTiller, 1, POSDIR_GND_STEER_TILLER, (float)(170*RAD));
	AddAnimationComponent(an_dvc_tiller, 0.0, 1.0, &mgRot_tiller);
	
	
	
	// dvc switches... in bulk!
	//
	for (int i = 0; i<VC_SWITCH_COUNT; ++i)
	{
		VCSwitchDef &swDef = VC_swDefs[i];
		VCSwitch &vcsw = VCSwitches[i];
		//
		vcsw.pos  = swDef.init_pos;
		vcsw.mgid = swDef.mgid;
		vcsw.anID = CreateAnimation(0.0);
		vcsw.mgRot = new MGROUP_ROTATE(mdlID_dvc, &(swDef.mgid), 1, swDef.axisPos, swDef.axisDir, (float)(80*RAD));
		
		AddAnimationComponent(vcsw.anID, 0.0, 1.0, vcsw.mgRot);
		
		switch(vcsw.pos)
		{
			case SW3_UP:
			case SW2_UP:
				SetAnimation(vcsw.anID, 1.0);
				break;

			case SW3_MID:
				SetAnimation(vcsw.anID, 0.5);
				break;

			case SW3_DOWN:
			case SW2_DOWN:
				SetAnimation(vcsw.anID, 0.0);
			break;
		};
	}
	

}


void G422::clearAnimations()
{
	delete landingGears;
	delete visor;
	delete canards;
	delete inltDoors;
	delete ramxDoors;
	delete wingTipWvrd;
	delete wingTipFthr;
	delete bayDoors;
	delete rcs;
	
	delete mgRot_wtL;
	delete mgRot_wtR;
	delete mgRot_acs_evL;
	delete mgRot_acs_evR;
	
	for (int i = 0; i<VC_SWITCH_COUNT; ++i)
	{
		VCSwitch &vcsw = VCSwitches[i];
		delete vcsw.mgRot;
	}
}



/*

void VCSwitch::defineAnimation(VESSEL3 *vslRef, UINT mdlID, const VECTOR3 _aPos, const VECTOR3 _aDir)
{
	
	mgRot = new MGROUP_ROTATE(mdlID, &mgid, 1, _aPos, _aDir, (float)(80*RAD));
	anID = vslRef->CreateAnimation(0.0);
	vslRef->AddAnimationComponent(anID, 0.0, 1.0, mgRot);
	
	switch(pos)
	{
		case SW3_UP:
		case SW2_UP:
			vslRef->SetAnimation(anID, 1.0);
		break;

		case SW3_MID:
			vslRef->SetAnimation(anID, 0.5);
		break;

		case SW3_DOWN:
		case SW2_DOWN:
			vslRef->SetAnimation(anID, 0.0);
		break;
	}

}

*/