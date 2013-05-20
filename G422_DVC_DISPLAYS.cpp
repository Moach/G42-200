

#include "G422.h"
#include "G422_MDL_DVC.h"
#include "G422_DVC.h"

#define EICAS_W 340
#define EICAS_H 410
#define EICAS_MW 170
#define EICAS_MH 205


inline void txtFltData(oapi::Sketchpad *skp, int x, int y, string st, double& val, int prcsn=2);
inline void txtFltData(oapi::Sketchpad *skp, int x, int y, double& val, int prcsn=2);



#define BLIT_PTARROW_R 56, 39, 10, 13
#define BLIT_PTARROW_L 56, 75, 10, 13
#define BLIT_PTARROW_U 55, 57, 13, 10





const UINT pnl_mgids[6] = {MGID_EICAS_C1, MGID_EICAS_C2, MGID_EICAS_P1, MGID_EICAS_P2, MGID_EICAS_E1, MGID_EICAS_E2};

void G422::selEicasMode(int eicasIdx, int mode)
{
	
	//  note on how this works:
	//		in order to allow saving the current screen more even when the screen is off, we use the negated mode numbers to flag that the screen is to go dark
	//      then, when turning it back on - we can use abs() to restore the last mode displayed -- therefore, we don't need a second set of variables! clever, huh?

	
	if (mode == 0) return; // there is no mode zero...
	
	
	static WORD vIdx[4] = {0,1,2,3};
	NTVERTEX vtx[4];
	
	
	if (mode >= 0)
	{
		float md_X = (float)( (mode-1) % 4 ) * .25f;
		float md_Y = floorf( (float)(mode-1) * .25f ) * .3f;
		
		vtx[0].tu = md_X;       vtx[0].tv = md_Y;
		vtx[1].tu = md_X;       vtx[1].tv = md_Y+.3f;
		vtx[2].tu = md_X+.25f; vtx[2].tv = md_Y+.3f;
		vtx[3].tu = md_X+.25f; vtx[3].tv = md_Y;
	} else
	{
		// power OFF -- negative
		vtx[0].tu = .925;   vtx[0].tv = .910;
		vtx[1].tu = .925;   vtx[1].tv = .999;
		vtx[2].tu = .999;	 vtx[2].tv = .999;
		vtx[3].tu = .999;   vtx[3].tv = .910;
	
	}
	
	GROUPEDITSPEC spc; //memset(&spc, 0, sizeof(GROUPEDITSPEC));
	spc.flags = GRPEDIT_VTXTEX; 
	spc.Vtx = vtx; 
	spc.vIdx = vIdx;
	spc.nVtx = 4;
	
	oapiEditMeshGroup(GetDevMesh(visual, mdlID_dvc), pnl_mgids[eicasIdx], &spc);
	
	int lMode = VC_eicas_screens[eicasIdx];
	VC_eicas_screens[eicasIdx] = mode;
	
	// disable redraw on invisible pages
	//
	drawEicas |= (1 << mode);
	
	if (lMode < 0) return; // irrelevant when toggling screens on from off state
	for (int i = 0; i<6; i++) // check for panels still displaying the page that was just closed
	{
		if (VC_eicas_screens[i] == lMode) return; // stop here if any are found
	}
	
	// otherwise, flag disused page OFF so it doesn't go on redrawing while invisible
	drawEicas &= ~(1 << lMode);
}






void G422::clbkMFDMode(int mfd, int mode)
{
	oapiVCTriggerRedrawArea(-1, VC_AREA_MFDKEYS);
	//
}



bool G422::clbkVCRedrawEvent(int id, int ev, SURFHANDLE surf)
{
	//
	//
	//if (oapiCockpitMode() != COCKPIT_VIRTUAL) return false;
	
	
	
	
	oapi::Sketchpad* sp;
	static const int mfdUpdateLineup[4] = {MFD_LEFT, MFD_USER1, MFD_RIGHT, MFD_USER2};
	
	
	switch (id)
	{
		//default:
		case VC_AREA_MFDKEYS:
		
			sp = oapiGetSketchpad(surf);
			if (sp == NULL){
				return false;
			}
			
			//strcpy(oapiDebugString(), "redraw code is running...");
			
			sp->SetFont(G422DrawRes.mfdLabelsFont);
			//sp->SetBackgroundMode(oapi::Sketchpad::BkgMode::BK_OPAQUE);
			//sp->SetBackgroundColor(0x000000);
			sp->SetTextColor(0xFFCC00);
			sp->SetTextAlign(oapi::Sketchpad::CENTER,  oapi::Sketchpad::BASELINE);
			//
			{
				int x = 0;
				
				for (UINT i = 0; i<4; ++i)
				{
					//
					for (UINT j = 0; j<6; ++j)
					{
						const char* label = oapiMFDButtonLabel(mfdUpdateLineup[i], j);
						if (label == NULL) continue;
						
						sp->Text(25 + x, 32 + (j*50), label, strlen(label));
						//
					}
					
					x += 50;
					for (UINT j = 6; j<12; ++j)
					{
						const char* label = oapiMFDButtonLabel(mfdUpdateLineup[i], j);
						if (label == NULL) continue;

						sp->Text(25 + x , 32 + ((j-6)*50), label, strlen(label));
						//
					}
					
					x += 50;
				}
				
				//////////////////////////////////////////////////////////////////////////
				
				x = 0;
				int y = 300;
				
				for (UINT i = 0; i<12; ++i)
				{
					const char* label = oapiMFDButtonLabel(MFD_USER3, i);
					if (label != NULL) 
						sp->Text(25 + x, 32 + y, label, strlen(label));
					
					y+=50; 	
					if (y > 450)
					{
						x += 50;
						y = 300;
					}
				}
				
				for (UINT i = 0; i<12; ++i)
				{
					const char* label = oapiMFDButtonLabel(MFD_USER4, i);
					if (label != NULL) 
						sp->Text(25 + x, 32 + y, label, strlen(label));

					y+=50; 	
					if (y > 450)
					{
						x += 50;
						y = 300;
					}
				}
			
			}
			oapiReleaseSketchpad(sp);
			
		return true;
		
		
		case VC_AREA_EICAS_ALL:
		{	
			
			//
			//
			
			sp = oapiGetSketchpad(surf); 
			if (sp == NULL){
				return false;
			}
			
			
			sp->SetFont(MFDS_FONT);
			sp->SetTextColor(0xFFFFFF);
			sp->SetTextAlign(oapi::Sketchpad::LEFT,  oapi::Sketchpad::TOP);
			//
			
			
			sp->SetPen(PEN_AMBER);
			//
			
			
			//
			//////////////////////////////////////////////////////////////////////////
			//
			//
			//     ramcaster flight-envelope display
		
			if (drawEicas & (1<<7))
			{
				
				
				if (thrFX_Y > 0.0001 && thrFX_Y < 1.0)
				{
					if (thrFX_X > 0.0001 && thrFX_X < 1.0)
					{
						sp->MoveTo(RDW_RAMXFPR_X + thrFX_X*RDW_RAMXFPR_W, RDW_RAMXFPR_Y);
						sp->LineTo(RDW_RAMXFPR_X + thrFX_X*RDW_RAMXFPR_W, RDW_RAMXFPR_Y+RDW_RAMXFPR_H);

					}
				
					sp->MoveTo(RDW_RAMXFPR_X,               RDW_RAMXFPR_Y + thrFX_Y*RDW_RAMXFPR_H);
					sp->LineTo(RDW_RAMXFPR_X+RDW_RAMXFPR_W, RDW_RAMXFPR_Y + thrFX_Y*RDW_RAMXFPR_H);
				}
			
				
			}
			
			//
			// GEN PCT gauges...
			if (drawEicas & (1<<5))
			{
				double fdat = engMain_L.genPct * 120.0;
				txtFltData(sp, 32, 365, fdat, 1); txtFltData(sp, 108, 365, fdat, 1);
			
				double thr = engMain_L.genPct*PI*1.25;
				double dx = cos(thr); double dy = sin(thr);
				sp->MoveTo(RDW_GENPCT_XL + dx*RDW_GENPCT_R1, RDW_GENPCT_Y + dy*RDW_GENPCT_R1);
				sp->LineTo(RDW_GENPCT_XL + dx*RDW_GENPCT_R2, RDW_GENPCT_Y + dy*RDW_GENPCT_R2);
				
				sp->MoveTo(RDW_GENPCT_XR + dx*RDW_GENPCT_R1, RDW_GENPCT_Y + dy*RDW_GENPCT_R1);
				sp->LineTo(RDW_GENPCT_XR + dx*RDW_GENPCT_R2, RDW_GENPCT_Y + dy*RDW_GENPCT_R2);
				
				

				// EPR gauges
				fdat = engMain_L.epr * PI + PI*.75;
				dx = cos(fdat); dy = sin(fdat);

				sp->MoveTo(202 + dx*12, 360 + dy*12);
				sp->LineTo(202 + dx*18, 360 + dy*18);

				sp->MoveTo(202 + dx*12, 398 + dy*12);
				sp->LineTo(202 + dx*18, 398 + dy*18);
				
				fdat = engMain_L.epr * 155.0;
				txtFltData(sp, 205, 362, fdat, 1); txtFltData(sp, 205, 400, fdat, 1);


				
				// thr indicators
				//
				thr = engMain_L.thr*PI*1.25;
				dx = cos(thr); dy = sin(thr);
				
				sp->SetPen(PEN_WHITE);
				sp->MoveTo(RDW_GENPCT_XL + dx*RDW_GENPCT_R2, RDW_GENPCT_Y + dy*RDW_GENPCT_R2);
				sp->LineTo(RDW_GENPCT_XL + dx*RDW_GENPCT_R3, RDW_GENPCT_Y + dy*RDW_GENPCT_R3);

				sp->MoveTo(RDW_GENPCT_XR + dx*RDW_GENPCT_R2, RDW_GENPCT_Y + dy*RDW_GENPCT_R2);
				sp->LineTo(RDW_GENPCT_XR + dx*RDW_GENPCT_R3, RDW_GENPCT_Y + dy*RDW_GENPCT_R3);
			
			
			
				// fuel flow
				//
				fdat = GetPropellantFlowrate(fuel_main_allTanks) * .5;
				txtFltData(sp, 32, 440, fdat, 1); txtFltData(sp, 108, 440, fdat, 1); // FFL
				
				// fuel pressure
				//
				fdat = engMain_L.fuelPrs * 100.0; txtFltData(sp,  32, 475, fdat, 1); // FSP
				fdat = engMain_R.fuelPrs * 100.0; txtFltData(sp, 108, 475, fdat, 1); 
				
				
				
				fdat = engMain_L.oxyPrs * 100.0; txtFltData(sp, 200, 472, fdat, 1); 
				fdat = engMain_R.oxyPrs * 100.0; txtFltData(sp, 200, 582, fdat, 1);

				fdat = GetPropellantFlowrate(fuel_oxy) * .5;
				txtFltData(sp, 200, 435, fdat, 1); txtFltData(sp, 200, 550, fdat, 1);

				
				if (engMain_L.state == RT66::SST_RUN_EXT) //   turbine+afterburner mode
				{
					fdat = GetThrusterLevel(RT66_gasGen_thgr[0]) * GetThrusterMax(RT66_gasGen_thgr[0]) + 
						   GetThrusterLevel(RT66_burner_thgr[0]) * GetThrusterMax(RT66_burner_thgr[0]);
				} 
				else if (engMain_L.state == RT66::SST_RUN_INT) //      rocket mode
				{  
					
					fdat = GetThrusterLevel(RT66_rocket_thgr[0]) * GetThrusterMax(RT66_rocket_thgr[0]);
				}
				
				//
				txtFltData(sp, 32, 590, fdat, 1); txtFltData(sp, 108, 590, fdat, 1); // TTR
			}
		
			if (drawEicas & (1<<7))
			{
		
			// RAMCASTER DISPLAY
			//
			
			//if (engRamx.state == RAMCASTER::ENG_RUNLO || engRamx.state == RAMCASTER::ENG_RUNHI)
			
				double fdat = GetThrusterLevel(ramcaster) * GetThrusterMax(ramcaster);
				txtFltData(sp, 595, 590, fdat, 1);
				
				fdat = GetPropellantFlowrate(fuel_main_allTanks);
				txtFltData(sp, 533, 562, fdat, 1);
				
				fdat = engRamx.fuelPrs * 100.0;
				txtFltData(sp, 533, 590, fdat, 1);
				
				fdat = engRamx.epr * 225.0;
				txtFltData(sp, 719, 590, fdat, 1);
				
				switch (engRamx.state)
				{
					case RAMCASTER::ENG_RUNLO:
						sp->Text(690, 330, "MODE: LO", 8);
					break;
					case RAMCASTER::ENG_RUNHI:
						sp->Text(690, 330, "MODE: HI", 8);
					break;
					default:
						sp->Text(700, 330, "INOP", 4);
					break;
				}
				
			 //else
			//{
				
				//sp->Text(595, 590, "00.0", 4);
				//sp->Text(533, 562, "00.0", 4);
				
			
			//}
			
			
			}
			
		
			//
			///////////////////////////////////////////////////////////////////////   //////////////////////////////////////////////////////////////////////////
			//     
			///////////////////////////////////////////////////////////////////////   //////////////////////////////////////////////////////////////////////////
			//
			//												FUEL RESERVES PANEL
		
			
			
			
			
			
			// main
			//
			double flLvl = GetPropellantMass(fuel_main_allTanks);
			txtFltData(sp, FLD_FUEL_MAIN_X, FLD_FUEL_MAIN_Y, " ", flLvl);
			
			double flMax = GetPropellantMaxMass(fuel_main_allTanks);
			double flPct = (flLvl / flMax) *PI*1.45;
			//
			double dX = cos(flPct); double dY = sin(flPct);
			sp->SetPen((flPct > PI*.3)? PEN_AMBER : PEN_RED);
			sp->MoveTo(114 + dX*20, 978 + dY*20); sp->LineTo(114 + dX*25, 978 + dY*25);
			
			
			// oxidizer
			//
			flLvl = GetPropellantMass(fuel_oxy);
			txtFltData(sp, FLD_FUEL_OXY_X, FLD_FUEL_OXY_Y, " ", flLvl);
			
			flMax = GetPropellantMaxMass(fuel_oxy);	
			flPct = (flLvl / flMax) *PI*1.45;
			dX = cos(flPct); dY = sin(flPct);
			sp->SetPen((flPct > PI*.3)? PEN_AMBER : PEN_RED);
			sp->MoveTo(40 + dX*20, 978 + dY*20); sp->LineTo(40 + dX*25, 978 + dY*25);
			

			// systems monopropellant
			//
			flLvl = GetPropellantMass(fuel_sys);
			txtFltData(sp, FLD_FUEL_ASF_X, FLD_FUEL_ASF_Y, " ", flLvl);
			
			flMax = GetPropellantMaxMass(fuel_sys);
			flPct = (flLvl / flMax) *PI*1.45;
			dX = cos(flPct); dY = sin(flPct);
			sp->SetPen((flPct > PI*.3)? PEN_AMBER : PEN_RED);
			sp->MoveTo(185 + dX*20, 978 + dY*20); sp->LineTo(185 + dX*25, 978 + dY*25);
			
			
			
			
				
			
			
			

			//	ACS CONTROLS (wings position indicator)
			
			
			if (drawEicas & (1<<9))
			{
			
				if ((wingTipFthr->mp_status | wingTipWvrd->mp_status) & MP_MOVING) // any of the two hinge actuators moving
				{
					sp->SetPen(PEN_RED);
					
					flPct = (wingTipWvrd->pos - wingTipFthr->pos) * PI * .5;
					dX = cos(flPct); dY = sin(flPct);
					
					sp->MoveTo(54 - int(dX * 5 ), 680 + int(dY*5 ));
					sp->LineTo(54 - int(dX * 18), 680 + int(dY*18));
					sp->MoveTo(76 + int(dX * 5 ), 680 + int(dY*5 ));
					sp->LineTo(76 + int(dX * 18), 680 + int(dY*18));
					
				} else
				{
					sp->SetPen(PEN_WHITE);
					
					if (wingTipFthr->pos + wingTipWvrd->pos == 0.0)
					{
						dX = 1.0; dY = 0.0;
					} else
					{
						dX = 0.0;
						dY = (wingTipFthr->pos > wingTipWvrd->pos)? 1.0 : -1.0;
					}
					
					
					sp->MoveTo(54 - int(dX * 5 ), 680 + int(dY*5 ));
					sp->LineTo(54 - int(dX * 18), 680 + int(dY*18));
					sp->MoveTo(76 + int(dX * 5 ), 680 + int(dY*5 ));
					sp->LineTo(76 + int(dX * 18), 680 + int(dY*18));
				
				}
			}
			
			
			
			//
			//
			oapiReleaseSketchpad(sp);
			
			
			//
			//
			
		//	oapiBlt(surf, vcRsc_reht_tape, 12, 495-rhtLvl, 0, 86-rhtLvl, 10, rhtLvl);
		//	oapiBlt(surf, vcRsc_reht_tape, 88, 495-rhtLvl, 0, 86-rhtLvl, 10, rhtLvl);
			
			if (drawEicas & (1<<5))
			{
				switch (inltDoors->mp_status)
				{
				
					case MP_LOW_DETENT:
						oapiBlt(surf, vcRes_eicasSrc,  24,333, 70, 27, 48, 23); //oapiBlt(surf, surf,  24,333, 470, 958, 48, 23);
						oapiBlt(surf, vcRes_eicasSrc, 100,333, 70, 27, 48, 23); //oapiBlt(surf, surf, 100,333, 470, 958, 48, 23);
					break;
					case MP_HI_DETENT:
						oapiBlt(surf, vcRes_eicasSrc,  24,333, 70, 3, 48, 23); //oapiBlt(surf, surf,  24,333, 470, 935, 48, 23);
						oapiBlt(surf, vcRes_eicasSrc, 100,333, 70, 3, 48, 23); //oapiBlt(surf, surf, 100,333, 470, 935, 48, 23);
					break;
					default:
						oapiBlt(surf, vcRes_eicasSrc,  24,333, 70, 49, 48, 23); //oapiBlt(surf, surf,  24,333, 470, 980, 48, 23);
						oapiBlt(surf, vcRes_eicasSrc, 100,333, 70, 49, 48, 23); //oapiBlt(surf, surf, 100,333, 470, 980, 48, 23);
					
				}
				
				if (engMain_L.state == RT66::SST_RUN_INT)
				{
				
					int engLvl = GetEngineLevel(ENGINE_MAIN) * 164;
					
					if (engLvl < 82)
					{
						oapiBlt(surf, vcRes_eicasSrc, 169,595-engLvl, 13, 90-engLvl, 8, engLvl);//oapiBlt(surf, surf, 169,595-engLvl, 411, 1022-engLvl, 8, engLvl);
						oapiBlt(surf, vcRes_eicasSrc, 180,595-engLvl, 34, 90-engLvl, 8, engLvl);//oapiBlt(surf, surf, 180,595-engLvl, 432, 1022-engLvl, 8, engLvl);
						
						
					} else
					{
						oapiBlt(surf, vcRes_eicasSrc, 169, 513, 13, 8, 8, 82); //oapiBlt(surf, surf, 169, 513, 411, 940, 8, 82);
						oapiBlt(surf, vcRes_eicasSrc, 180, 513, 34, 8, 8, 82); //oapiBlt(surf, surf, 180, 513, 432, 940, 8, 82);
						
						engLvl -= 82;
						oapiBlt(surf, vcRes_eicasSrc, 169, 513-engLvl, 21, 90-engLvl, 8, engLvl); //oapiBlt(surf, surf, 169, 513-engLvl, 420, 1022-engLvl, 8, engLvl);
						oapiBlt(surf, vcRes_eicasSrc, 180, 513-engLvl, 42, 90-engLvl, 8, engLvl); //oapiBlt(surf, surf, 180, 513-engLvl, 441, 1022-engLvl, 8, engLvl);
					
					}
					
				} else if (burner_toggle)	
				{
					int engLvl = (GetEngineLevel(ENGINE_MAIN) - .8) * 85 * 5;
					//
					oapiBlt(surf, vcRes_eicasSrc, 10, 495-engLvl, 0, 90-engLvl, 2, engLvl);
					oapiBlt(surf, vcRes_eicasSrc, 86, 495-engLvl, 0, 90-engLvl, 2, engLvl);

					int rhtLvl = GetThrusterLevel(RT66_burner_thgr[0]) * 85;
					//
					oapiBlt(surf, vcRes_eicasSrc, 12, 495-rhtLvl, 2, 90-rhtLvl, 8, rhtLvl); 
					oapiBlt(surf, vcRes_eicasSrc, 88, 495-rhtLvl, 2, 90-rhtLvl, 8, rhtLvl); 
				}
			}
			
			
			
			if (drawEicas & (1<<9))
			{
				
				
				oapiBlt(surf, vcRes_eicasSrc, 209, 700+(int)(60*GetControlSurfaceLevel(AIRCTRL_ELEVATORTRIM)), BLIT_PTARROW_R);
				
				
				
				oapiBlt(surf, vcRes_eicasSrc, 115, 676+(int)(26*GetControlSurfaceLevel(AIRCTRL_ELEVATOR)), BLIT_PTARROW_R);
				oapiBlt(surf, vcRes_eicasSrc, 182, 676+(int)(26*GetControlSurfaceLevel(AIRCTRL_ELEVATOR)), BLIT_PTARROW_L);
				
				
				oapiBlt(surf, vcRes_eicasSrc, 88,  857+(int)(26*GetControlSurfaceLevel(AIRCTRL_ELEVATOR)), BLIT_PTARROW_R);
				oapiBlt(surf, vcRes_eicasSrc, 209, 857+(int)(26*GetControlSurfaceLevel(AIRCTRL_ELEVATOR)), BLIT_PTARROW_L);


				oapiBlt(surf, vcRes_eicasSrc, 132,  850-(int)(26*GetControlSurfaceLevel(AIRCTRL_AILERON)), BLIT_PTARROW_R);
				oapiBlt(surf, vcRes_eicasSrc, 166,  850+(int)(26*GetControlSurfaceLevel(AIRCTRL_AILERON)), BLIT_PTARROW_L);
				
				oapiBlt(surf, vcRes_eicasSrc, 149+(int)(26*GetControlSurfaceLevel(AIRCTRL_RUDDER)), 895,  BLIT_PTARROW_U);
				
				
				switch(landingGears->mp_status) // gear indicators
				{
					case MP_LOW_DETENT:
						oapiBlt(surf, vcRes_eicasSrc, 20, 718,   162, 0,   94, 39);
					
					break;
					case MP_HI_DETENT:
						oapiBlt(surf, vcRes_eicasSrc, 20, 718,   162, 76,   94, 39);
					
					break;
					default:
						oapiBlt(surf, vcRes_eicasSrc, 20, 718,   162, 38,   94, 39);
						
				}
				
			
			}
			
			
		}
		
		
		return true;
	}
	
	
	//
	//
	return false;
	
}

bool G422::clbkDrawHUD(int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp)
{

	
	

	if (VC_PoV_station) return true; // anything other than pilot seat gets no HUD
	
	
	//
	// draw the default HUD
	VESSEL3::clbkDrawHUD(mode, hps, skp);


	//
	//VESSEL *vsl = oapiGetFocusInterface();
	
	double surfRds = oapiGetSize(GetSurfaceRef());
	double mjdref  = oapiGetSimMJD();
	
	ELEMENTS elms;
	GetElements(elms,  mjdref);

	
	
	
	
/*
	HDC hDC = skp->GetDC();

	// show gear deployment status
	if (landingGears->mp_status != MP_LOW_DETENT) {
		int d = hps->Markersize/2;
		int cx = hps->CX, cy = hps->CY;

		if (landingGears->mp_status != MP_HI_DETENT)
		{

			TextOut (hDC,cx-15, cy-40, TEXT("GEAR"),4);
			TextOut (hDC,cx-20, cy-25, TEXT("UNSAFE"),6);

		} else
		{
			Rectangle (hDC, cx-d/2, cy-d*5, cx+d/2, cy-d*4);
			Rectangle (hDC, cx-d*3, cy-d*2, cx-d*2, cy-d);
			Rectangle (hDC, cx+d*2, cy-d*2, cx+d*3, cy-d);
		}		
	}
*/
	
	if (oapiCockpitMode() == COCKPIT_VIRTUAL) 
	{
		
		double fltData;
		//
		
		switch(mode)
		{
			case HUD_SURFACE:
				
				fltData =  GetMachNumber(); txtFltData (skp, 5, 50, TEXT("MACH "), fltData);
				fltData = GetDynPressure(); txtFltData (skp, 5, 60, TEXT("DynP "), fltData);
				fltData = GetAtmPressure(); txtFltData (skp, hps->W - 68, 50, TEXT("StaP "), fltData);
				
				fltData = GetSlipAngle() * DEG;  txtFltData (skp, hps->W - 100, hps->H - 30, TEXT("SLIP  "), fltData);
				fltData = GetAOA() * DEG;        txtFltData (skp, hps->W - 100, hps->H - 40, TEXT("AoA  "),  fltData);

				//TextOut (hDC,0,hps->H - 25, pilotInterface.engineLine, strlen(pilotInterface.engineLine));
				
			break;

			case HUD_DOCKING:
				//
				//skp->Text(hps->W-60,10, TEXT("::CLSD::"),8);



			break;	

			case HUD_ORBIT:
				GetApDist(fltData); fltData-=surfRds;    txtFltData (skp,5,20, TEXT("ApA "), fltData);			
				GetPeDist(fltData); fltData-=surfRds;    txtFltData (skp,5,30, TEXT("PeA "), fltData);

				txtFltData (skp, 5,40, TEXT("Ecc "), elms.e);			
				txtFltData (skp, 5,50, TEXT("SmA "), elms.a);


				txtFltData (skp,(hps->W)- 80, 30, TEXT("Inc "), elms.i);
				txtFltData (skp,(hps->W)- 80, 40, TEXT("LAN "), elms.theta);

			break;

		}



		if(GetNavmodeState (NAVMODE_KILLROT))
			skp->Text(hps->W-150, hps->H-13, TEXT("|   KILLROT   |"), 15);

		if(GetNavmodeState (NAVMODE_PROGRADE))
			skp->Text(hps->W-150, hps->H-13, TEXT("|   PROGRDE   |"), 15);

		if(GetNavmodeState (NAVMODE_RETROGRADE))
			skp->Text(hps->W-150, hps->H-13, TEXT("|   RETROGD   |"), 15);

		if(GetNavmodeState (NAVMODE_NORMAL))
			skp->Text(hps->W-150, hps->H-13, TEXT("|   NORMAL+   |"), 15);

		if(GetNavmodeState (NAVMODE_ANTINORMAL))
			skp->Text(hps->W-150, hps->H-13, TEXT("|   NORMAL-   |"), 15);

		if(GetNavmodeState (NAVMODE_HLEVEL))
			skp->Text(hps->W-150, hps->H-13, TEXT("|   WINGLVL   |"), 15);

		if(GetNavmodeState (NAVMODE_HOLDALT))
			skp->Text(hps->W-150, hps->H-13, TEXT("|   HOLDALT   |"), 15);
		


		// show RCS / OMS mode
		switch (rcoms.state)
		{
			case RCOMS::SYS_ALLSTBY:
			case RCOMS::SYS_RCSTBY:
				switch (GetAttitudeMode()) 
				{
					case RCS_ROT:
						skp->Text(0, hps->H-13, TEXT("RCS ROT"), 7);
					break;
					case RCS_LIN:
						skp->Text(0, hps->H-13, TEXT("RCS LIN"), 7);
					break;
				}
				
			if (!thr_authority || rcoms.state != RCOMS::SYS_ALLSTBY) break;		
			case RCOMS::SYS_OMSTBY:
				skp->Text(55, hps->H-13, TEXT("|  OMS"), 6);
				
				
			break;
		}
		
		if (prk_brake_mode)
		{
			skp->Text(5, hps->H-37, TEXT("PK BRAKES"), 9);
		
		}
		
		// show main throttle position
		//
		fltData = GetThrusterGroupLevel(THGROUP_MAIN) * 100;  
		if (burner_toggle)
			txtFltData (skp, 5, hps->H-25, (fltData>80)? TEXT("Th*") : TEXT("Th "), fltData, 1);
		else
			txtFltData (skp, 5, hps->H-25, TEXT("Th "), fltData, 1);
		
	}

	return true;
}




//
//
inline void txtFltData(oapi::Sketchpad *skp, int x, int y, string st, double& val, int prcsn)
{

	ostringstream strm;

	//
	if (abs(val) < 1000)
		strm << st << fixed << setprecision(prcsn) << val;	
	else if (abs(val) < 1000000)
		strm << st << fixed << setprecision(prcsn) << (val/1000) << TEXT("K");
	else
		strm << st << fixed << setprecision(prcsn) << (val/1000000) << TEXT("M");

	st = strm.str();

	//
	//TextOut(hDC, x,y, st.c_str(), st.length());
	skp->Text(x ,y, st.c_str(), st.length());
}

inline void txtFltData(oapi::Sketchpad *skp, int x, int y, double& val, int prcsn)
{

	ostringstream strm;

	//
	if (abs(val) < 1000)
		strm << fixed << setprecision(prcsn) << val;	
	else if (abs(val) < 1000000)
		strm << fixed << setprecision(prcsn) << (val/1000) << TEXT("K");
	else
		strm << fixed << setprecision(prcsn) << (val/1000000) << TEXT("M");

	string st = strm.str();

	//
	//TextOut(hDC, x,y, st.c_str(), st.length());
	skp->Text(x ,y, st.c_str(), st.length());
}



