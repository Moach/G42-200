

#include "G422.h"

//
//             from stock DG - blame dr. Martin
// ==============================================================
// Airfoil coefficient functions
// Return lift, moment and zero-lift drag coefficients as a
// function of angle of attack (alpha or beta)
// ==============================================================

// 1. vertical lift component (wings and body) | alpha



void VLiftCoeff (VESSEL *v, double aoa, double M, double Re, void *context, double *cl, double *cm, double *cd)
{
	int i;

	const int nabsc = 9;
	static const double AOA[nabsc] =      {-180*RAD, -60*RAD, -30*RAD, -2*RAD, 15*RAD, 20*RAD, 25*RAD, 60*RAD, 180*RAD};
	for (i = 0; i < nabsc-1 && AOA[i+1] < aoa; i++);
	double f = (aoa-AOA[i]) / (AOA[i+1]-AOA[i]);


	static const double CL_sbsc[nabsc]  = {       0,       0,    -0.5,     -0.1,    0.8,     0.72,     0.35,      0,       0};
	static const double CM_sbsc[nabsc]  = {       0,       0,   0.014, 0.0042, -0.006, -0.008,  -0.010,     .004,       0};
	
	

	*cl = CL_sbsc[i] + (CL_sbsc[i+1]-CL_sbsc[i]) * f;  // aoa-dependent lift coefficient
	*cm = CM_sbsc[i] + (CM_sbsc[i+1]-CM_sbsc[i]) * f;  // aoa-dependent moment coefficient
	
	


	
	
	double wingPos = (static_cast<G422*>( v ))->wingTipWvrd->pos;
	if (wingPos > 0.01)
	{
		*cl = (*cl) * (wingPos * ( 6.0* (M+2.0) )) * WAVERIDER_FACTOR;
		//
	}
	
	wingPos = (static_cast<G422*>( v ))->wingTipFthr->pos;
	if (wingPos > 0.01)
	{
		*cl = (*cl) * (1.0 - wingPos * WINGFTHR_LIFTDMP);
		
	}
	

	double saoa = sin(aoa);
	double pd = 0.015 + 0.4*saoa*saoa;  // profile drag
	
	double wd = oapiGetWaveDrag (M, 0.75, 1.0, 1.1, 0.04);
	*cd = pd + oapiGetInducedDrag (*cl, 1.5, 0.7) + wd;
	// profile drag + (lift-)induced drag + transonic/supersonic wave (compressibility) drag
	
	// ground effect lift
	//
	double agl = v->GetAltitude(); // prehaps this could be more precise
	//
	if (agl < MAXHEIG_GROUNDEFFECT && M > 0.1)
	{
		double gndFX = 1-((agl) * MAXHINV_GROUNDEFFECT);
		*cl += ((gndFX * gndFX * saoa) * MAXLIFT_GROUNDEFFECT);
	}
	
	
}

// 2. horizontal lift component (vertical stabilisers and body) | beta

void HLiftCoeff (VESSEL *v, double beta, double M, double Re, void *context, double *cl, double *cm, double *cd)
{
	int i;

	const int nabsc = 8;
	static const double BETA[nabsc] = {-180*RAD,-135*RAD,-90*RAD,-45*RAD,45*RAD,90*RAD,135*RAD,180*RAD};
	static const double CL[nabsc]   = {       0,    +0.3,      0,   -0.3,  +0.3,     0,   -0.3,      0};

	for (i = 0; i < nabsc-1 && BETA[i+1] < beta; i++);
	*cl = CL[i] + (CL[i+1]-CL[i]) * (beta-BETA[i]) / (BETA[i+1]-BETA[i]);

	*cm = 0.0;
	*cd = 0.015 + oapiGetInducedDrag (*cl, 1.5, 0.6) + oapiGetWaveDrag (M, 0.75, 1.0, 1.1, 0.04);
}



void RAMX_FxModel(VESSEL *vessel, G422::RAMCASTER &ramx)
{
	// very much shameless ripoff from DG code... no re-inventing the wheel for us this time....
	//     -- thanks again, Martin!
	
	const OBJHANDLE hBody = vessel->GetAtmRef();
	const ATMCONST *atm = (hBody ? oapiGetPlanetAtmConstants (hBody) : 0);

	if (atm) // atmospheric parameters available
	{ 

		double M, Fs, T0, Td, Tb, Tb0, Te, p0, pd, D, rho, cp, v0, ve, tr, lvl, dma, dmf, precov, dmafac;
		const double eps = 1e-4;
		const double dma_scale = 2.7e-4;

		M   = vessel->GetMachNumber();                     // Mach number
		T0  = vessel->GetAtmTemperature();                 // freestream temperature
		p0  = vessel->GetAtmPressure();                    // freestream pressure
		rho = vessel->GetAtmDensity();                     // freestream density
		cp  = atm->gamma * atm->R / (atm->gamma-1.0);      // specific heat (pressure)
		v0  = M * sqrt (atm->gamma * atm->R * T0);         // freestream velocity
		tr  = (1.0 + 0.5*(atm->gamma-1.0) * M*M);          // temperature ratio
		Td  = T0 * tr;                                     // diffuser temperature
		pd  = p0 * pow (Td/T0, atm->gamma/(atm->gamma-1.0)); // diffuser pressure
		precov = max (0.0, 1.0-0.075*pow (max(M,1.0)-1.0, 1.35)); // pressure recovery
		dmafac = dma_scale*precov*pd;
		
	
	}
	
}









FXMapRef fxRef;

#define HEX_2_SCALAR 0.00392156862745098039


double FXMapRedChannel(BMP &tex, double u, double v) 
{
	u = clamp(0.0, u, 1.0) * 256; v = clamp(0.0, v, 1.0) * 256;
	int x = floor(u);
	int y = floor(v);
	double u_ratio = u - x;
	double v_ratio = v - y;
	double u_opposite = 1 - u_ratio;
	double v_opposite = 1 - v_ratio;

	double p00, p10, p01, p11;

	p00 = double(tex(x,  y)->Red) * HEX_2_SCALAR;   p10 = double(tex(x+1,  y)->Red) * HEX_2_SCALAR;
	p01 = double(tex(x+1,y)->Red) * HEX_2_SCALAR;   p11 = double(tex(x+1,y+1)->Red) * HEX_2_SCALAR;

	return (p00 * u_opposite  + p10   * u_ratio) * v_opposite + (p01 * u_opposite  + p11 * u_ratio) * v_ratio;
}


double FXMapGreenChannel(BMP &tex, double u, double v) 
{
	u = clamp(0.0, u, 1.0) * 256; v = clamp(0.0, v, 1.0) * 256;
	int x = floor(u);
	int y = floor(v);
	double u_ratio = u - x;
	double v_ratio = v - y;
	double u_opposite = 1 - u_ratio;
	double v_opposite = 1 - v_ratio;

	double p00, p10, p01, p11;

	p00 = double(tex(x,  y)->Green) * HEX_2_SCALAR;   p10 = double(tex(x+1,  y)->Green) * HEX_2_SCALAR;
	p01 = double(tex(x+1,y)->Green) * HEX_2_SCALAR;   p11 = double(tex(x+1,y+1)->Green) * HEX_2_SCALAR;

	return (p00 * u_opposite  + p10   * u_ratio) * v_opposite + (p01 * u_opposite  + p11 * u_ratio) * v_ratio;
}


double FXMapBlueChannel(BMP &tex, double u, double v) 
{
	u = clamp(0.0, u, 1.0) * 256; v = clamp(0.0, v, 1.0) * 256;
	int x = floor(u);
	int y = floor(v);
	double u_ratio = u - x;
	double v_ratio = v - y;
	double u_opposite = 1 - u_ratio;
	double v_opposite = 1 - v_ratio;

	double p00, p10, p01, p11;

	p00 = double(tex(x,  y)->Blue) * HEX_2_SCALAR;   p10 = double(tex(x+1,  y)->Blue) * HEX_2_SCALAR;
	p01 = double(tex(x+1,y)->Blue) * HEX_2_SCALAR;   p11 = double(tex(x+1,y+1)->Blue) * HEX_2_SCALAR;

	return (p00 * u_opposite  + p10   * u_ratio) * v_opposite + (p01 * u_opposite  + p11 * u_ratio) * v_ratio;
}





void FXMapAll(FXMapRef &fxr, BMP &tex, double u, double v) 
{
	u = clamp(0.0, u, 1.0) * 256; v = clamp(0.0, v, 1.0) * 256;
	int x = floor(u);
	int y = floor(v);
	double u_ratio = u - x;
	double v_ratio = v - y;
	double u_opposite = 1 - u_ratio;
	double v_opposite = 1 - v_ratio;

	double p00, p10, p01, p11;

	p00 = double(tex(x,  y)->Red) * HEX_2_SCALAR;   p10 = double(tex(x+1,  y)->Red) * HEX_2_SCALAR;
	p01 = double(tex(x+1,y)->Red) * HEX_2_SCALAR;   p11 = double(tex(x+1,y+1)->Red) * HEX_2_SCALAR;

	fxr.channelRed = (p00 * u_opposite  + p10   * u_ratio) * v_opposite + (p01 * u_opposite  + p11 * u_ratio) * v_ratio;


	p00 = double(tex(x,  y)->Green) * HEX_2_SCALAR;   p10 = double(tex(x+1,  y)->Green) * HEX_2_SCALAR;
	p01 = double(tex(x+1,y)->Green) * HEX_2_SCALAR;   p11 = double(tex(x+1,y+1)->Green) * HEX_2_SCALAR;

	fxr.channelGreen = (p00 * u_opposite  + p10   * u_ratio) * v_opposite + (p01 * u_opposite  + p11 * u_ratio) * v_ratio;


	p00 = double(tex(x,  y)->Blue) * HEX_2_SCALAR;   p10 = double(tex(x+1,  y)->Blue) * HEX_2_SCALAR;
	p01 = double(tex(x+1,y)->Blue) * HEX_2_SCALAR;   p11 = double(tex(x+1,y+1)->Blue) * HEX_2_SCALAR;

	fxr.channelBlue = (p00 * u_opposite  + p10   * u_ratio) * v_opposite + (p01 * u_opposite  + p11 * u_ratio) * v_ratio;

}


