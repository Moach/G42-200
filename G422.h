
#define STRICT

#ifndef G422_MODULE
#define G422_MODULE


#include "Orbitersdk.h"

#include "OrbiterSoundSDK35.h"
#include "EasyBmp\EasyBMP.h"


#include <math.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>

#include "resource.h"

using namespace std;

extern HINSTANCE thisDLL;

#define LOAD_BMP(id) (LoadBitmap (thisDLL, MAKEINTRESOURCE (id)))




#define OUT cout <<
#define ENT << endl



#define _V0 _V(0.0,0.0,0.0)

#define _X _V(1.0,0.0,0.0)
#define _Y _V(0.0,1.0,0.0)
#define _Z _V(0.0,0.0,1.0)

#define _Xn _V(-1.0,0.0,0.0)
#define _Yn _V(0.0,-1.0,0.0)
#define _Zn _V(0.0,0.0,-1.0)


#define SCALAR_ANGLE 0.15915494309189533577



#define NML_AGL_PRS 101.4e3

#define EMPTY_MASS 86e3

//#define MASTER_WINGLIFT 0.25
#define WAVERIDER_FACTOR 0.025 // was .055
#define WINGFTHR_LIFTDMP 0.525

#define MAXLIFT_GROUNDEFFECT 0.85
#define MAXHEIG_GROUNDEFFECT 35.0
#define MAXHINV_GROUNDEFFECT 2.8571428571429e-002


#define ENGINE_RAMP_SCALAR  0.42
#define ENGINE_IDLE_EXTPCT  0.05
#define ENGINE_IDLE_INTPCT  0.12

//                       thrust rating for turbine + afterburner is the same as for full rocket mode
#define MAXTHRUST_MAIN_GEN 68e4
#define MAXTHRUST_MAIN_AFB 82e4
//
#define MAXTHRUST_MAIN_RKT 150e4


#define MAXTHRUST_RAMX_LO 350e4//282e4
#define MAXTHRUST_RAMX_HI 365e4//265e4

#define MAXTRHUST_RCS 5500.0
#define MAXTHRUST_OMS 65800.0


#define ISPMAX_MAIN_GEN  41500
#define ISPMAX_MAIN_AFB  13500

#define ISPMAX_MAIN_RKT  7800
#define ISPMIN_MAIN_RKT  7300

#define ISPMAX_RAMXLO   16700
#define ISPMAX_RAMXHI   32000

#define ISPMAX_RCS 6100
#define ISPMAX_OMS 6800

#define ISPMIN_RCS 1100
#define ISPMIN_OMS 1000


#define MAXFUEL_MAIN_ALL 205e3

#define MAXFUEL_OXY 85000.0
#define MAXFUEL_SYS 16000.0


#define OXYFUEL_RATIO	1.15



#define APU_FUEL_RATE .035




/*
#define MAXFUEL_TANK_L 12000.0
#define MAXFUEL_TANK_F 125000.0
#define MAXFUEL_TANK_A 114000.0
#define MAXFUEL_TANK_R 12000.0
*/


#define RCS_FX_LSCALE 2.25
#define RCS_FX_WSCALE 0.20
#define OMS_FX_LSCALE 5.4
#define OMS_FX_WSCALE 0.26




#define SFX_ENGINERUN_MAIN_EXT 1
#define SFX_ENGINERUN_MAIN_INT 2

#define SFX_ENGINERUN_RAMX_LO 3
#define SFX_ENGINERUN_RAMX_HI 4
#define SFX_RAMCASTER_START   5
#define SFX_RAMCASTER_TRST_UP 6

#define SFX_APU_START 7
#define SFX_APU_RUN   8
#define SFX_APU_STOP  9

#define SFX_FUELPUMP_MAIN 10
#define SFX_FUELPUMP_RAMX 12
#define SFX_FUELPUMP_OXY  13
#define SFX_FUELPUMP_RCS  14
#define SFX_FUELPUMP_OMS  15
#define SFX_FUELPUMP_APU  16

#define SFX_GEARS 17
#define SFX_VSRUP 18
#define SFX_VSRDN 19
#define SFX_CNRDS 20

#define SFX_WIND_OPEN 21
#define SFX_WIND_CLSD 22

#define SFX_VC_POP   23
#define SFX_VC_FLICK 24
#define SFX_VC_TICK  25
#define SFX_VC_BEEP  26
#define SFX_VC_BLIP  27
#define SFX_VC_CLAK  28
#define SFX_VC_AFLIK 29
#define SFX_VC_SLACK 30

#define SFX_WINGMTR 31

#define SFX_ENGINESTART_MAIN_EXT 32
#define SFX_ENGINESTART_MAIN_INT 33

#define SFX_ENGINESTOP_MAIN_EXT 34
#define SFX_ENGINESTOP_MAIN_INT 35

#define SFX_AFTERBURNER 36
#define SFX_JETROAR 37
#define SFX_RCTROAR 38

#define SFX_OMSBURN 39
#define SFX_RAMXBURN 40


enum VCSwitchState
{
	SW3_DOWN, SW3_MID, SW3_UP, // 3-stop switches
	SW2_DOWN, SW2_UP           // 2-stop switches
};

//
struct VCSwitch
{
	UINT anID;
	UINT mgid;
	//
	
	VCSwitchState pos;
	MGROUP_ROTATE *mgRot;
	
	// check SIM OPS for these...
	//
	bool flick(bool upDn, VESSEL3 *vslRef);
	void setPos(VCSwitchState newPos, VESSEL3 *vslRef);
	
	char getSaveState();
	void setLoadState(char &cs, VESSEL3* vsl);
};


typedef struct {  // this allows us to statically define all switches in a list-like fashion, then instantiate
                  // independent VCSwitch structs for new G42-class vessels
	UINT mgid;
	//int swSetRef, swSetID;
	VCSwitchState init_pos;
	VECTOR3 refPos, axisPos, axisDir;
	//
	
	//int swIdx;
	
} VCSwitchDef;

#define VC_SWITCH_COUNT 50 // number of switches in VC (so far)
extern VCSwitchDef VC_swDefs[VC_SWITCH_COUNT]; // see DVC SETUP

extern map<UINT, int> VC_swIndexByMGID; // defined by the first vessel created, see VSL_SETUP
//



//
enum MP_STATE 
{
	MP_MOVING = 1, MP_LOW_DETENT = 2, MP_HI_DETENT = 4, MP_REVERSING = 3, MP_INOP = 0
};

// vessel generic messages
//
#define VMSG_MPSTRT 1010
#define VMSG_MPSTOP 1011
#define VMSG_ENGRST 1012
//


//
//
struct MovingPart  // implemented in G422_AUX
{
	    UINT anim_idx;
	  double pos;
	  double rate;
	     int sysID;    // id for use in callbacks
	    bool sysReset; // will be set to false whever a callback is fired, reset to true after updating systems logic
	    
	MP_STATE mp_status; //
	VESSEL3* vsl;

	MovingPart(UINT idx, double rt, MP_STATE st, double stPos = 0.0, VESSEL3* vslRef = NULL, int sID = 0)
	{
		anim_idx = idx;
		pos = stPos;
		rate = rt;
		mp_status = st;
		sysID = sID;
		vsl = vslRef;
		sysReset = true;
		//
		if(vsl)
			vsl->SetAnimation(idx, stPos);
	}

	inline bool operate(double& dt); // except this one - see SIM UPDATE
	int toggle();
	int toggle(bool b);
	void toDetent(int p);
	bool getToggleState();
	//
	
	// and these two - see scenario RW
	void loadCfgState(int state);
	int getCfgState();
};



//////////////////////////////////////////////////////////////////////////

// engine shit...

extern BMP thrFX_map;

struct FXMapRef
{
	double channelRed;
	double channelGreen;
	double channelBlue;
};

// this is all defined in SIM_DYNAMICS (for now)
//
void   FXMapAll(FXMapRef &fxr, BMP &tex, double u, double v);
double FXMapRedChannel(BMP &tex, double u, double v);
double FXMapGreenChannel(BMP &tex, double u, double v);
double FXMapBlueChannel(BMP &tex, double u, double v);
//

extern struct G422_DRAWRES
{

	bool def;
	
	SURFHANDLE bmpRes[1];
	
	oapi::Font *mfdLabelsFont;
	//
	oapi::Brush *brSet[8];
	oapi::Pen   *spSet[8];


} G422DrawRes;




//////////////////////////////////////////////////////////////////////////

//   SYSTEM ID's

// some of these may have cue specifiers to work with... 
// 
#define G422_SYSID_LGRS  1
#define G422_SYSID_CNRD  2
#define G422_SYSID_VISR  3
#define G422_SYSID_RAMX  4
#define G422_SYSID_INLT  5
#define G422_SYSID_RCOMS 6
#define G422_SYSID_PKBRK 7
#define G422_SYSID_RCSDR 8


// things that can be done to systems
//
#define G422_SYSOP_GENERIC 0
#define G422_SYSOP_ENABLE  1
#define G422_SYSOP_DISABLE 2
#define G422_SYSOP_RESET   3
#define G422_SYSOP_ARM     4
#define G422_SYSOP_SAFE    5



/////////////////////////////////////////////// G422 VESSEL3 CLASS //////////////////////////////////////////////////////////

//extern bool initOne; // we use this to have the first G42 instance set up global stuff
//                   which further instances needn't trouble doing again...
//




struct G422 : VESSEL3
{
	//
	G422 (OBJHANDLE vsl_hndl, int fltModel);
	~G422 ();
	
	static int vslCount; 
	
	MESHHANDLE mdl_ext;
	MESHHANDLE mdl_dvc;
	MESHHANDLE mdl_vc_R;
	MESHHANDLE mdl_vc_L;
	
	UINT mdlID_ext, mdlID_dvc, mdlID_dvcR, mdlID_dvcL;
	
	
	SURFHANDLE vcTex_mfds;
	SURFHANDLE vcTex_eicas;
//	SURFHANDLE vcRsc_rckt_tape, vcRsc_reht_tape;
	
	SURFHANDLE vcRes_eicasSrc;
	
	//
	//
	
	PROPELLANT_HANDLE fuel_main_allTanks;
	PROPELLANT_HANDLE fuel_main_L, fuel_main_F, fuel_main_A, fuel_main_R;
	PROPELLANT_HANDLE fuel_sys, fuel_oxy, dummyFuel; 
	
	THRUSTER_HANDLE dummyThruster, dummyHover, dummyThrAirbrakes;
	
	THRUSTER_HANDLE RT66_gasGen_thgr[2];
	THRUSTER_HANDLE RT66_burner_thgr[2];
	THRUSTER_HANDLE RT66_rocket_thgr[2];
	THRUSTER_HANDLE ramcaster;
	THRUSTER_HANDLE oms_thgr[2];
	

	THRUSTER_HANDLE th_rcs_pitchUp[2], th_rcs_pitchDn[2],  th_rcs_vertUp[2], th_rcs_vertDown[2];
	THRUSTER_HANDLE th_rcs_yawLeft[2], th_rcs_yawRight[2], th_rcs_horLeft[2], th_rcs_horRight[2];
	THRUSTER_HANDLE th_rcs_fwd[1], th_rcs_back[1];
	THRUSTER_HANDLE th_rcs_bankRight[2], th_rcs_bankLeft[2];

	THGROUP_HANDLE master_thgr, controller_thgr;
	THGROUP_HANDLE rcs_fwd_thgr, rcs_r1_thgr, rcs_r2_thgr;
	
	AIRFOILHANDLE wingLift;
	
	CTRLSURFHANDLE acsCndrs;
	
	ATTACHMENTHANDLE att_bay_1;
	
	double fx_contrailLvl;
	double fx_exhaustLvl;
	
	BEACONLIGHTSPEC nav_green, nav_red, nav_white, stb_l, stb_r, stb_t, bcn1, bcn2;
	
	
	
	
	MovingPart *landingGears;
	MovingPart *visor;
	MovingPart *canards;
	MovingPart *rcs;
	
	MovingPart *inltDoors;
	MovingPart *ramxDoors;
	
	MovingPart *wingTipWvrd;
	MovingPart *wingTipFthr;
	MGROUP_ROTATE *mgRot_wtL, *mgRot_wtR, *mgRot_wtLgtsL, *mgRot_wtLgtsR; 
	MGROUP_ROTATE *mgRot_acs_evL, *mgRot_acs_evR; 	
	
	VECTOR3 wingTipLightsL[2];
	VECTOR3 wingTipLightsR[2];
	
	
	MovingPart *bayDoors;
	MovingPart *radiators;
	
	int VC_eicas_screens[6];
	int drawEicas;
	
	int VC_PoV_station; // 0: pilot 1: F/0 (for now...)
	
	VCSwitch VCSwitches[VC_SWITCH_COUNT]; // that's right, load 'em up...
	
	VISHANDLE visual;
	
	
	/*inline VCSwitch& getVCSW(UINT mgid)
	{
		int swIndex = VC_swIndexByMGID[mgid];
		return (VCSwitches[swIndex]);
	};*/
	
	
	// individual animations for VC (not simple switches)
	//
	UINT an_dvc_ramx;
	UINT an_dvc_wpos;
	UINT an_dvc_thr;
	UINT an_dvc_csPitch;
	UINT an_dvc_csRoll;
	UINT an_dvc_tiller;
	
	/*
	// gimbals (innermost out)
	UINT an_dvc_adi_C, an_dvc_adi_B, an_dvc_adi_A;
	MGROUP_ROTATE *mgRot_ADI_G1, *mgRot_ADI_G2;
	*/
	// aerodynamic control surfaces
	//
	UINT an_acs_cnrd;
	UINT an_acs_rddr;
	UINT an_acs_alrn;
	UINT an_acs_elvr;
	
	bool allSystemsReset; // set to false to bring about a refresh of all systems upon next step (disused)
	
	
	double thrPos; // gauged throttle position from controller dummy
	double deltaT; // master sim delta-time record for use in whatever functions....
	double vcRdwT; // time since last VC redraw...
	
	double thrFX_X, thrFX_Y; // scanline positions for thruster performance lookup table
	
	
	int SFXID; // for use with orbiter-sound
	
	
	int thrAuthority; // master selected throttle authority mode :: 1 = main 2 = OMS 3 = both
	
	int main_eng_mode; // main engine mode selector 0: off 1:ext 2: int 
	int main_ign_sqnc; // main ignition sequence selector 0: passive 1:nominal 2: ullage-burn 
	int burner_toggle; // main engine afterburners 0: off 1: on (both engines)
	
	int ramcaster_mode; // ramx mode 0: off 1:low 2:hi
	
	int thr_authority; // 0: main engines 1: OMS
	
	int prk_brake_mode; // 0: off, 1: set
	int gnd_steer_mode; // 0: off, 1: limited, 2:full
	
	// mind you - we're not really dealing with ACTUAL CG_shifting here - instead, we just throw off the center-of-lift
	//            to the opposite side, as that's the only effect a pilot would perceive (after PBW and FMC sort out everything else)
	//               plus, the ShiftCG functions are bugged to boot -- too much fuss for a non-apparent aspect of the sim...
	//  then we can cut a few corners and get down to what matters only...
	//
	// "ref" is where you want the CG to be, 
	// "pos" is where it really is,
	// "dyn" is how your flight-envelope throws it off...
	//
	double cgShiftRef, cgShiftPos, cgShiftDyn; 
	
	//
	int  wingPos; // -1 = fuly down 0 = flat  1 = fully up
	
	double Gfwd, Gver, Glat, vslMass;
	
	struct RT66
	{
		bool busy;
		
		enum FEED_CFG {FUEL_OPEN = 1, FUEL_PUMP = 2, OXY_OPEN = 4, OXY_PUMP = 8}; int feed;
		enum SIMSTATE {SST_INOP, SST_CUT, ENG_SPOOLUP, SST_STARTGEN, SST_STARTRCKT, SST_RUN_EXT, ENG_INLT_CLSE,
					   ENG_INLT_OPEN, SST_RUN_INT, ENG_SPOOLDOWN, SST_STOPGEN, SST_STOPRCKT, SST_FAIL}; int state;
		
		double fuelPrs, oxyPrs; // fuel and oxy lines pressure
		
		double thr; // throttle command position
		double genPct; // general percent compressor/turbopump RPM
		//
		
		double spr, epr, mct; // stream pressure rating, effective pressure rating,  mean compressor temperature
		
		THRUSTER_HANDLE th_gasGen, th_burner, th_rocket;
		
	} engMain_L, engMain_R;
	
	
	struct RAMCASTER
	{
		bool busy;
		
		enum FEED_CFG {FUEL_OPEN = 1, FUEL_PUMP = 2}; int feed;
		enum SIMSTATE {ENG_INOP, ENG_START, ENG_RUNLO, ENG_UPCAST, ENG_RUNHI, ENG_DOWNCAST, ENG_STOP, ENG_FAIL}; int state;
		double thr;
		double fuelPrs;
		
		double spr, epr, ett, idt;
		//float
		//THGROUP_HANDLE tgr_ramx;
		THRUSTER_HANDLE th_ramx;
	} engRamx;
	
	
	struct APU
	{
		bool busy;
		
		enum FEED_CFG {FUEL_OPEN = 1, FUEL_PUMP = 2}; int feed;
		enum SIMSTATE {ENG_INOP, ENG_RUN, ENG_START, ENG_STOP}; int state;
		double pwrPct;
		double fuelPrs, fuelFlow;
		
	} apu;
	
	
	
	
	// from here on these are systems
	//
	/*
	struct SUBSYSTEM_BASE
	{
		bool online;
		
		bool checkFeeds(double &elecBus, double &hydBus, double &prsBus);
		void operate(double %simDt, double &coolingLine);
		
		bool cueState(int cue);
		
	};
	*/
	
	
	struct RCOMS // reaction control and orbital maneuvering systems
	{
		enum SYS_FEED  {FUEL_OM_OPEN = 1, FUEL_OM_PUMP = 2, FUEL_RC_OPEN = 4, FUEL_RC_PUMP = 8}; int feed;
		enum SIMSTATE {SYS_INOP, SYS_RCSTBY, SYS_OMSTBY, SYS_ALLSTBY, SYS_OFF}; int state;
		enum CUE_SPEC {CUE_RCS_STBY, CUE_OMS_STBY, CUE_RCS_OPCL};
		//
		
		double fuelPrsOms, fuelPrsRcs, fuelFlowOms, fuelFlowRcs;
		double omsThr;
		
		THRUSTER_HANDLE th_oms;
	} rcoms;
	
	
	
	
	
	
	
	
	//
	// functions and whatnot -- definitions are found in files respectively marked
	// 
	
	//
	//
	
	// VESSEL SETUP
	//
	void setupAnimations();
	void clearAnimations();
	//
	void clbkSetClassCaps (FILEHANDLE cfg);
	void clbkPostCreation (void);
	//
	void clbkVisualCreated(VISHANDLE vis, int refcount);
	
	// SCENARIO READ/WRITE
	//
	void clbkLoadStateEx (FILEHANDLE scn, void *status);
	void clbkSaveState (FILEHANDLE scn);
	
	
	// DVC SETUP
	//
	bool clbkLoadVC(int id);
	
	
	// DVC DISPLAYS
	//
	void clbkMFDMode(int mfd, int mode);
	//
	bool clbkVCRedrawEvent(int id, int ev, SURFHANDLE surf);
	bool clbkDrawHUD (int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp);
	
	void selEicasMode(int pnlID, int mode);
	
	
	
	// SIM CONTROLS
	//
	bool clbkVCMouseEvent(int id, int ev, VECTOR3 &p);
	int clbkConsumeDirectKey (char *kstate);
	int clbkConsumeBufferedKey (DWORD key, bool down, char *kstate);
	
	
	// VESSEL UPDATE
	//
	void clbkPreStep(double simt, double simdt, double mjd);
	
	int clbkGeneric(int msgid, int prm, void *context);
	int systemsReset(int mode = 0, int spec = 0);
	int enginesReset();
	
	
	// SIM CUES
	//
	void cueEngines(RT66 &eng, RT66::SIMSTATE sst);
	void cueEngines(RAMCASTER &eng, RAMCASTER::SIMSTATE sst);
	void cueEngines(APU &eng,  APU::SIMSTATE sst);
	//
	void cueSystems(int sysID, int spec = 0, int op = 0, void *ctx = NULL);
	//
	
	// SIM DYNAMICS
	//
	void simEngines(double &dT, RT66 &eng);
	void simEngines(double &dT, RAMCASTER &eng);
	void simEngines(double &dT, APU &eng);
	void simEngines(double &dT, RCOMS &eng);
	
	
	
	// SIM OPERATIONALS (OPS)
	//
	
	
	
	
};






// global lift functions (also in SIM DYNAMICS)
//
void VLiftCoeff (VESSEL *v, double aoa, double M, double Re, void *context, double *cl, double *cm, double *cd);
void HLiftCoeff (VESSEL *v, double beta, double M, double Re, void *context, double *cl, double *cm, double *cd);






// utility things


#define clamp(mn, x, mx) ( max((mn), min((mx), (x))) )

void StringExplode(string str, string separator, vector<string>* results);
void StringTrimWS(string &str);
void StringToLower(string &str);
int parseInt(string &str);




inline double sdRandom(int x)
{

	x = (x<<13) ^ x;
	return ( 1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
	//
}


inline void filter( double &crt, double &tgt, double smth )
{
	//
	crt += ((tgt - crt) * smth);
}








#endif