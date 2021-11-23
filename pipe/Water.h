#ifndef WATER_H
#define WATER_H

#include <exception>
#include <functional>
#include <string>
#include <cstring>
/*

Water - CAPE-OPEN steam/water property package
Copyright (C) 2010-2021 Jasper van Baten

You can use this source code provided that
 - its use is for academic teaching or academic research purposes, and
 - its use is non-commercial, and
 - you will not distribute COM objects that use any of the CLSIDs in Water_DLL.h
 - you properly acknowledge the author

For any other use, please contact jasper@amsterchem.com

For more information, see LICENSE.txt

*/

/*

The Water object implements the industrial water and steam IAPWS (http://www.iapws.org/)
97 specification. Specifically:

 - all equations for regions 1-5 from

   Revised Release on the IAPWS Industrial Formulation 1997 for the Thermodynamic Properties of Water and Steam (August 2007)

 - backward V(P,T) for region 3 from

   Supplementary Release on Backward Equations for Specific Volume as a Function of Pressure and Temperature v(p,T) for Region 3 of the IAPWS Industrial Formulation 1997 for the Thermodynamic Properties of Water and Steam (July 2005)

 - backward T(H,P) and T(S,P) from

   Supplementary Release on Backward Equations p(h,s) for Region 3, Equations as a Function of h and s for the Region Boundaries, and an Equation Tsat(h,s) for Region 4 of the IAPWS Industrial Formulation 1997 for the Thermodynamic Properties of Water and Steam (September 2004)

 - Viscosity correlation from

   Release on the IAPWS Formulation 2008 for the Viscosity of Ordinary Water Substance (September 2008)

   (in accordance with guidelines for industrial use: without near-critical correction)

 - Thermal conductivity correlation from

   Revised Release on the IAPWS Formulation 1985 for the Thermal Conductivity of Ordinary Water Substance (September 2008)

 - Surface tension correlation from

   Release: "Surface Tension of Ordinary Water Substance" (September 1994)

 - ideal gas Cp correlation from

 - Brent solver based on oleg's netlib version

 - various state solvers.

 To use:

 Calculate properties of vapor and liquid phase at given T,P:

   //liquid entropy calculation
   Water w;
   Water.SetStateTPX(T,P,TPLIQUID);
   s=w.entropy();

 Calculate flash equations

   Water w;
   Water.SetStateTPX(T,P,TPAUTO);
   vaporFraction=w.GetQuality();

   Water w;
   Water.SetStatePH(P,H);
   temperature=w.GetTemperature();
   vaporFraction=w.GetQuality();

   (etc)

   or by direct use of PSat(T) and TSat(P) for
   TVF and PVF flashes

*/

#include <cmath>
#include <cfloat>
void ASSERT(bool condition,char const* description) ;

//main water / steam calculation class
class Water {

	//units are : K, MPa, kJ, kg

	class WaterException : public std::exception {

		static const int maxText=1024;
		char txt[maxText];

		public:

		WaterException(const char *reason) {
			int ln=(int)strlen(reason);
			if (ln<maxText) {
				#ifdef _MSC_VER
				strcpy_s(txt,maxText,reason);
				#else
				strcpy(txt,reason);
				#endif
			} else {
				memcpy(txt,reason,maxText-1);
				txt[maxText-1]=0;
			}
		}

		WaterException(const std::string &reason) {
			if (reason.size()<maxText) {
				#ifdef _MSC_VER
				strcpy_s(txt,maxText,reason.c_str());
				#else
				strcpy(txt,reason.c_str());
				#endif
			} else {
				memcpy(txt,reason.c_str(),maxText-1);
				txt[maxText-1]=0;
			}
		}

		WaterException(const char *reason,WaterException &other) {
			int ln=(int)strlen(reason);
			if (ln<maxText) {
				#ifdef _MSC_VER
				strcpy_s(txt,maxText,reason);
				#else
				strcpy(txt,reason);
				#endif
				char *dest=txt+ln;
				int remain=maxText-ln;
				ln=(int)strlen(other.txt);
				if (ln<remain) {
					#ifdef _MSC_VER
					strcpy_s(dest,remain,other.txt);
					#else
					strcpy(dest,other.txt);
					#endif
				} else {
					memcpy(dest,reason,remain-1);
					dest[remain-1]=0;
				}
			} else {
				memcpy(txt,reason,maxText-1);
				txt[maxText-1]=0;
			}
		}

		const char *what() const throw() {
			return txt;
		}

	};


	//equation coefficients
	static const double n23B[5];
	static const double reg1n[34];
	static const int reg1i[34];
	static const int reg1j[34];

	static const int reg1THi[20];
	static const int reg1THj[20];
	static const double reg1THn[20];

	static const int reg1TSi[20];
	static const int reg1TSj[20];
	static const double reg1TSn[20];

	static const int reg2Ji0[9];
	static const double reg2Ni0[9];
	static const double reg2Ni0MS[9]; //for the meta-stable vapor region

	static const int reg2IiR[43];
	static const int reg2JiR[43];
	static const double reg2NiR[43];

	static const int reg2IiRMS[13];
	static const int reg2JiRMS[13];
	static const double reg2NiRMS[13];

	static const int reg2aTHi[34];
	static const int reg2aTHj[34];
	static const double reg2aTHn[34];

	static const int reg2bTHi[38];
	static const int reg2bTHj[38];
	static const double reg2bTHn[38];

	static const int reg2cTHi[23];
	static const int reg2cTHj[23];
	static const double reg2cTHn[23];

	static const double reg2aTSi[46];
	static const int reg2aTSj[46];
	static const double reg2aTSn[46];

	static const int reg2bTSi[44];
	static const int reg2bTSj[44];
	static const double reg2bTSn[44];

	static const int reg2cTSi[30];
	static const int reg2cTSj[30];
	static const double reg2cTSn[30];

	static const int reg3Ii[40];
	static const int reg3Ji[40];
	static const double reg3Ni[40];

	static const double reg4Ni[10];

	static const int reg5Ji0[6];
	static const double reg5Ni0[6];

	static const int reg5IiR[6];
	static const int reg5JiR[6];
	static const double reg5NiR[6];

	static const int T3abI[5];
	static const double T3abN[5];

	static const int T3cdI[4];
	static const double T3cdN[4];

	static const int T3ghI[5];
	static const double T3ghN[5];

	static const int T3ijI[5];
	static const double T3ijN[5];

	static const int T3jkI[5];
	static const double T3jkN[5];

	static const int T3mnI[4];
	static const double T3mnN[4];

	static const int T3opI[5];
	static const double T3opN[5];

	static const int T3quI[4];
	static const double T3quN[4];

	static const int T3rxI[4];
	static const double T3rxN[4];

	static const int T3uvI[4];
	static const double T3uvN[4];

	static const int T3wxI[5];
	static const double T3wxN[5];

	static const int V3Ia[30];
	static const int V3Ja[30];
	static const double V3Na[30];

	static const int V3Ib[32];
	static const int V3Jb[32];
	static const double V3Nb[32];

	static const int V3Ic[35];
	static const int V3Jc[35];
	static const double V3Nc[35];

	static const int V3Id[38];
	static const int V3Jd[38];
	static const double V3Nd[38];

	static const int V3Ie[29];
	static const int V3Je[29];
	static const double V3Ne[29];

	static const int V3If[42];
	static const int V3Jf[42];
	static const double V3Nf[42];

	static const int V3Ig[38];
	static const int V3Jg[38];
	static const double V3Ng[38];

	static const int V3Ih[29];
	static const int V3Jh[29];
	static const double V3Nh[29];

	static const int V3Ii[42];
	static const int V3Ji[42];
	static const double V3Ni[42];

	static const int V3Ij[29];
	static const int V3Jj[29];
	static const double V3Nj[29];

	static const int V3Ik[34];
	static const int V3Jk[34];
	static const double V3Nk[34];

	static const int V3Il[43];
	static const int V3Jl[43];
	static const double V3Nl[43];

	static const int V3Im[40];
	static const int V3Jm[40];
	static const double V3Nm[40];

	static const int V3In[39];
	static const int V3Jn[39];
	static const double V3Nn[39];

	static const int V3Io[24];
	static const int V3Jo[24];
	static const double V3No[24];

	static const int V3Ip[27];
	static const int V3Jp[27];
	static const double V3Np[27];

	static const int V3Iq[24];
	static const int V3Jq[24];
	static const double V3Nq[24];

	static const int V3Ir[27];
	static const int V3Jr[27];
	static const double V3Nr[27];

	static const int V3Is[29];
	static const int V3Js[29];
	static const double V3Ns[29];

	static const int V3It[33];
	static const int V3Jt[33];
	static const double V3Nt[33];

	static const int V3Iu[38];
	static const int V3Ju[38];
	static const double V3Nu[38];

	static const int V3Iv[39];
	static const int V3Jv[39];
	static const double V3Nv[39];

	static const int V3Iw[35];
	static const int V3Jw[35];
	static const double V3Nw[35];

	static const int V3Ix[36];
	static const int V3Jx[36];
	static const double V3Nx[36];

	static const int V3Iy[20];
	static const int V3Jy[20];
	static const double V3Ny[20];

	static const int V3Iz[23];
	static const int V3Jz[23];
	static const double V3Nz[23];

	static const double mu0h[4];
	static const double mu1h[6][7];

	static const double lambda0a[4];
	static const double lambda0b[3];
	static const double lambda0B[2];
	static const double lambda0d[4];
	static const double lambda0C[6];

	static const double h3abn[4];

	static const int t3aphi[31];
	static const int t3aphj[31];
	static const double t3aphn[31];

	static const int t3bphi[33];
	static const int t3bphj[33];
	static const double t3bphn[33];

	static const int t3apsi[33];
	static const int t3apsj[33];
	static const double t3apsn[33];

	static const int t3bpsi[28];
	static const int t3bpsj[28];
	static const double t3bpsn[28];

	__inline static double intpow(double d,int p) {
		double res;
		switch (p) {
		case 0:
			res=1;
			break;
		case 1:
			res=d;
			break;
		case -1:
			res=1.0/d;
			break;
		case -2:
			res=1.0/(d*d);
			break;
		case 2:
			res=d*d;
			break;
		default:
			res=pow(d,p);
			break;
		}
		return res;
	}

public:

	//specification of T, P state requires more info on phase
	enum STATETP {
		TPAUTO,
		TPLIQUID,
		TPVAPOR
	};

	//some constants
	static const double GASCONSTANT;
	static const double TCRIT;
	static const double PCRIT;
	static const double RHOCRIT;
	static const double TTRIP;
	static const double PTRIP;
	static const double T13;
	static const double T25;
	static const double TMIN;
	static const double TMAX;
	static const double PMAX;
	static const double P5MAX;
	static const double MOLWT;

	static const double B23TMIN;
	static const double B23TMAX;
	static const double B23PMIN;
	static const double B23PMAX;

	static const double missing;

	//state functions
	double GetQuality() {
		return X;
	}
	double GetPressure() {
		return P;
	}
	double GetTemperature() {
		return T;
	}

private:

	//state:
	double T,P;
	double X; //steam fraction, a.k.a. steam quality

	//resulting from setting state
	int region;

	//set when setting state:
	double Prel,Trel,rhoRel;
	double G,GP,GPP,GT,GTT,GTP;
	double G0,G0T,G0TT;
	double F,FR,FRR,FT,FTT,FTR;

public:

	Water() {
		//call one of the state functions next
#ifdef _DEBUG
		region=0;
#endif
		//run self test in debug mode
		// (better to do once when initializing the program)
		//CheckConsistency();
	}

private:

	//consistency test implementation

#ifdef _DEBUG
    #define Check(test,f,check) { \
		double F=f; \
		if (!_finite(F)) { \
			ASSERT(0,"not finite"); \
		} else if (_isnan(F)) { \
			ASSERT(0,"not a number"); \
		} else if (fabs((F-check)/check)>1e-5) { \
			ASSERT(0,"Failed tolerance check:" test); \
		} \
	}

public:

	void CheckConsistency() {
		//the numbers that are checked against are from the specification documents
		//double P1,D;
		//region 23 boundary:
		Check("B23P",B23P(623.15),0.165291643e2);
		//Check("B23T",B23T(0.165291643e2),623.15); causes validation, below B23 min P
		//region 1 check:
		SetReg1(3.0,300.0);
		Check("v region 1 (a)",volume(),0.100215168e-2);
		Check("h region 1 (a)",enthalpy(),0.115331273e3);
		Check("u region 1 (a)",energy(),0.112324818e3);
		Check("s region 1 (a)",entropy(),0.392294792);
		Check("cp region 1 (a)",cP(),0.417301218e1);
		Check("w region 1 (a)",speedOfSound(),0.150773921e4);
		SetReg1(80.0,300.0);
		Check("v region 1 (b)",volume(),0.971180894e-3);
		Check("h region 1 (b)",enthalpy(),0.184142828e3);
		Check("u region 1 (b)",energy(),0.106448356e3);
		Check("s region 1 (b)",entropy(),0.368563852);
		Check("cp region 1 (b)",cP(),0.401008987e1);
		Check("w region 1 (b)",speedOfSound(),0.163469054e4);
		SetReg1(3.0,500.0);
		Check("v region 1 (c)",volume(),0.120241800e-2);
		Check("h region 1 (c)",enthalpy(),0.975542239e3);
		Check("u region 1 (c)",energy(),0.971934985e3);
		Check("s region 1 (c)",entropy(),0.258041912e1);
		Check("cp region 1 (c)",cP(),0.465580682e1);
		Check("w region 1 (c)",speedOfSound(),0.124071337e4);
		//T from PH, region 1
		Check("T from PH region 1 (a)",T1fromPH(3.0,500.0),0.391798509e3);
		Check("T from PH region 1 (b)",T1fromPH(80.0,500.0),0.378108626e3);
		Check("T from PH region 1 (c)",T1fromPH(80.0,1500.0),0.611041229e3);
		//T from PS, region 1
		Check("T from PS region 1 (a)",T1fromPS(3.0,0.5),0.307842258e3);
		Check("T from PS region 1 (b)",T1fromPS(80.0,0.5),0.309979785e3);
		Check("T from PS region 1 (c)",T1fromPS(80.0,3.0),0.565899909e3);
		//region 2 check:
		SetReg2(0.0035,300.0);
		Check("v region 2 (a)",volume(),0.394913866e2);
		Check("h region 2 (a)",enthalpy(),0.254991145e4);
		Check("u region 2 (a)",energy(),0.241169160e4);
		Check("s region 2 (a)",entropy(),0.852238967e1);
		Check("cp region 2 (a)",cP(),0.191300162e1);
		Check("w region 2 (a)",speedOfSound(),0.427920172e3);
		SetReg2(0.0035,700.0);
		Check("v region 2 (b)",volume(),0.923015898e2);
		Check("h region 2 (b)",enthalpy(),0.333568375e4);
		Check("u region 2 (b)",energy(),0.301262819e4);
		Check("s region 2 (b)",entropy(),0.101749996e2);
		Check("cp region 2 (b)",cP(),0.208141274e1);
		Check("w region 2 (b)",speedOfSound(),0.644289068e3);
		SetReg2(30.0,700.0);
		Check("v region 2 (c)",volume(),0.542946619e-2);
		Check("h region 2 (c)",enthalpy(),0.263149474e4);
		Check("u region 2 (c)",energy(),0.246861076e4);
		Check("s region 2 (c)",entropy(),0.517540298e1);
		Check("cp region 2 (c)",cP(),0.103505092e2);
		Check("w region 2 (c)",speedOfSound(),0.480386523e3);
		//region 2 check, meta-stable vapor:
		SetReg2(1.0,450.0);
		Check("v region 2 (d)",volume(),0.192516540);
		Check("h region 2 (d)",enthalpy(),0.276881115e4);
		Check("u region 2 (d)",energy(),0.257629461e4);
		Check("s region 2 (d)",entropy(),0.656660377e1);
		Check("cp region 2 (d)",cP(),0.276349265e1);
		Check("w region 2 (d)",speedOfSound(),0.498408101e3);
		SetReg2(1.0,440.0);
		Check("v region 2 (e)",volume(),0.186212297);
		Check("h region 2 (e)",enthalpy(),0.274015123e4);
		Check("u region 2 (e)",energy(),0.255393894e4);
		Check("s region 2 (e)",entropy(),0.650218759e1);
		Check("cp region 2 (e)",cP(),0.298166443e1);
		Check("w region 2 (e)",speedOfSound(),0.489363295e3);
		SetReg2(1.5,450.0);
		Check("v region 2 (f)",volume(),0.121685206);
		Check("h region 2 (f)",enthalpy(),0.272134539e4);
		Check("u region 2 (f)",energy(),0.253881758e4);
		Check("s region 2 (f)",entropy(),0.629170440e1);
		Check("cp region 2 (f)",cP(),0.362795578e1);
		Check("w region 2 (f)",speedOfSound(),0.481941819e3);
		//T from PH, sub-regions of 2
		Check("T from PH region 2a (a)",T2fromPH(0.001,3000.0),0.534433241e3);
		Check("T from PH region 2a (b)",T2fromPH(3.0,3000.0),0.575373370e3);
		Check("T from PH region 2a (c)",T2fromPH(3.0,4000.0),0.101077577e4);
		Check("T from PH region 2b (a)",T2fromPH(5.0,3500.0),0.801299102e3);
		Check("T from PH region 2b (b)",T2fromPH(5.0,4000.0),0.101531583e4);
		Check("T from PH region 2b (c)",T2fromPH(25.0,3500.0),0.875279054e3);
		Check("T from PH region 2c (a)",T2fromPH(40.0,2700.0),0.743056411e3);
		Check("T from PH region 2c (b)",T2fromPH(60.0,2700.0),0.791137067e3);
		Check("T from PH region 2c (c)",T2fromPH(60.0,3200.0),0.882756860e3);
		//T from PS, sub-regions of 2
		Check("T from PS region 2a (a)",T2fromPS(0.1,7.5),0.399517097e3);
		Check("T from PS region 2a (b)",T2fromPS(0.1,8.0),0.514127081e3);
		Check("T from PS region 2a (c)",T2fromPS(2.5,8.0),0.103984917e4);
		Check("T from PS region 2b (a)",T2fromPS(8.0,6.0),0.600484040e3);
		Check("T from PS region 2b (b)",T2fromPS(8.0,7.5),0.106495556e4);
		Check("T from PS region 2b (c)",T2fromPS(90.0,6.0),0.103801126e4);
		Check("T from PS region 2c (a)",T2fromPS(20.0,5.75),0.697992849e3);
		Check("T from PS region 2c (b)",T2fromPS(80.0,5.25),0.854011484e3);
		Check("T from PS region 2c (c)",T2fromPS(80.0,5.75),0.949017998e3);
		//region 3 check
		SetReg3(500.0,650.0);
		Check("p region 3 (a)",P,0.255837018e2);
		Check("h region 3 (a)",enthalpy(),0.186343019e4);
		Check("u region 3 (a)",energy(),0.181226279e4);
		Check("s region 3 (a)",entropy(),0.405427273e1);
		Check("cp region 3 (a)",cP(),0.138935717e2);
		Check("w region 3 (a)",speedOfSound(),0.502005554e3);
		SetReg3(200.0,650.0);
		Check("p region 3 (b)",P,0.222930643e2);
		Check("h region 3 (b)",enthalpy(),0.237512401e4);
		Check("u region 3 (b)",energy(),0.226365868e4);
		Check("s region 3 (b)",entropy(),0.485438792e1);
		Check("cp region 3 (b)",cP(),0.446579342e2);
		Check("w region 3 (b)",speedOfSound(),0.383444594e3);
		SetReg3(500.0,750.0);
		Check("p region 3 (c)",P,0.783095639e2);
		Check("h region 3 (c)",enthalpy(),0.225868845e4);
		Check("u region 3 (c)",energy(),0.210206932e4);
		Check("s region 3 (c)",entropy(),0.446971906e1);
		Check("cp region 3 (c)",cP(),0.634165359e1);
		Check("w region 3 (c)",speedOfSound(),0.760696041e3);
		//check P derivative w.r.t. rho
		//D=1e-3;
		//P1=GetPressure();
		//SetReg3(500.0+D,750.0);
		//Check("d P / d rho region 3",(GetPressure()-P1)/D,Reg3dRhodP());
		//Psat
		Check("Psat (a)",Psat(300.0),0.353658941e-2);
		Check("Psat (b)",Psat(500.0),0.263889776e1);
		Check("Psat (c)",Psat(600.0),0.123443146e2);
		//Tsat
		Check("Tsat (a)",Tsat(0.1),0.372755919e3);
		Check("Tsat (b)",Tsat(1.0),0.453035632e3);
		Check("Tsat (c)",Tsat(10.0),0.584149488e3);
		//region 5 check, meta-stable vapor:
		SetReg5(0.5,1500.0);
		Check("v region 5 (a)",volume(),0.138455090e1);
		Check("h region 5 (a)",enthalpy(),0.521976855e4);
		Check("u region 5 (a)",energy(),0.452749310e4);
		Check("s region 5 (a)",entropy(),0.965408875e1);
		Check("cp region 5 (a)",cP(),0.261609445e1);
		Check("w region 5 (a)",speedOfSound(),0.917068690e3);
		SetReg5(30.0,1500.0);
		Check("v region 5 (b)",volume(),0.230761299e-1);
		Check("h region 5 (b)",enthalpy(),0.516723514e4);
		Check("u region 5 (b)",energy(),0.447495124e4);
		Check("s region 5 (b)",entropy(),0.772970133e1);
		Check("cp region 5 (b)",cP(),0.272724317e1);
		Check("w region 5 (b)",speedOfSound(),0.928548002e3);
		SetReg5(30.0,2000.0);
		Check("v region 5 (c)",volume(),0.311385219e-1);
		Check("h region 5 (c)",enthalpy(),0.657122604e4);
		Check("u region 5 (c)",energy(),0.563707038e4);
		Check("s region 5 (c)",entropy(),0.853640523e1);
		Check("cp region 5 (c)",cP(),0.288569882e1);
		Check("w region 5 (c)",speedOfSound(),0.106736948e4);
		//reverse reg 3 boundaries
		Check("T3ab",T3ab(40.0),6.930341408e2);
		Check("T3cd",T3cd(25.0),6.493659208e2);
		Check("T3ef",T3ef(40.0),7.139593992e2);
		Check("T3gh",T3gh(23.0),6.498873759e2);
		Check("T3ij",T3ij(23.0),6.515778091e2);
		Check("T3jk",T3jk(23.0),6.558338344e2);
		Check("T3mn",T3mn(22.8),6.496054133e2);
		Check("T3op",T3op(22.8),6.500106943e2);
		Check("T3qu",T3qu(22.0),6.456355027e2);
		Check("T3rx",T3rx(22.0),6.482622754e2);
		Check("T3uv",T3uv(22.3),6.477996121e2);
		Check("T3wx",T3wx(22.3),6.482049480e2);
		//inverse region 3 volume checks
		Check("V3FromPTa",V3FromPTa(50,630),1.470853100e-3);
		Check("V3FromPTa",V3FromPTa(80,670),1.503831359e-3);
		Check("V3FromPTb",V3FromPTb(50,710),2.204728587e-3);
		Check("V3FromPTb",V3FromPTb(80,750),1.973692940e-3);
		Check("V3FromPTc",V3FromPTc(20,630),1.761696406e-3);
		Check("V3FromPTc",V3FromPTc(30,650),1.819560617e-3);
		Check("V3FromPTd",V3FromPTd(26,656),2.245587720e-3);
		Check("V3FromPTd",V3FromPTd(30,670),2.506897702e-3);
		Check("V3FromPTe",V3FromPTe(26,661),2.970225962e-3);
		Check("V3FromPTe",V3FromPTe(30,675),3.004627086e-3);
		Check("V3FromPTf",V3FromPTf(26,671),5.019029401e-3);
		Check("V3FromPTf",V3FromPTf(30,690),4.656470142e-3);
		Check("V3FromPTg",V3FromPTg(23.6,649),2.163198378e-3);
		Check("V3FromPTg",V3FromPTg(24,650),2.166044161e-3);
		Check("V3FromPTh",V3FromPTh(23.6,652),2.651081407e-3);
		Check("V3FromPTh",V3FromPTh(24,654),2.967802335e-3);
		Check("V3FromPTi",V3FromPTi(23.6,653),3.273916816e-3);
		Check("V3FromPTi",V3FromPTi(24,655),3.550329864e-3);
		Check("V3FromPTj",V3FromPTj(23.5,655),4.545001142e-3);
		Check("V3FromPTj",V3FromPTj(24,660),5.100267704e-3);
		Check("V3FromPTk",V3FromPTk(23,660),6.109525997e-3);
		Check("V3FromPTk",V3FromPTk(24,670),6.427325645e-3);
		Check("V3FromPTl",V3FromPTl(22.6,646),2.117860851e-3);
		Check("V3FromPTl",V3FromPTl(23,646),2.062374674e-3);
		Check("V3FromPTm",V3FromPTm(22.6,648.6),2.533063780e-3);
		Check("V3FromPTm",V3FromPTm(22.8,649.3),2.572971781e-3);
		Check("V3FromPTn",V3FromPTn(22.6,649),2.923432711e-3);
		Check("V3FromPTn",V3FromPTn(22.8,649.7),2.913311494e-3);
		Check("V3FromPTo",V3FromPTo(22.6,649.1),3.131208996e-3);
		Check("V3FromPTo",V3FromPTo(22.8,649.9),3.221160278e-3);
		Check("V3FromPTp",V3FromPTp(22.6,649.4),3.715596186e-3);
		Check("V3FromPTp",V3FromPTp(22.8,650.2),3.664754790e-3);
		Check("V3FromPTq",V3FromPTq(21.1,640),1.970999272e-3);
		Check("V3FromPTq",V3FromPTq(21.8,643),2.043919161e-3);
		Check("V3FromPTr",V3FromPTr(21.1,644),5.251009921e-3);
		Check("V3FromPTr",V3FromPTr(21.8,648),5.256844741e-3);
		Check("V3FromPTs",V3FromPTs(19.1,635),1.932829079e-3);
		Check("V3FromPTs",V3FromPTs(20,638),1.985387227e-3);
		Check("V3FromPTt",V3FromPTt(17,626),8.483262001e-3);
		Check("V3FromPTt",V3FromPTt(20,640),6.227528101e-3);
		//simple visc part:
		Check("Visc simple (a)",viscSimple(998,298.15),889.735100);
		Check("Visc simple (b)",viscSimple(1200,298.15),1437.649467);
		Check("Visc simple (c)",viscSimple(1000,373.15),307.883622);
		Check("Visc simple (d)",viscSimple(1,433.15),14.538324);
		Check("Visc simple (e)",viscSimple(1000,433.15),217.685358);
		Check("Visc simple (f)",viscSimple(1,873.15),32.619287);
		Check("Visc simple (g)",viscSimple(100,873.15),35.802262);
		Check("Visc simple (h)",viscSimple(600,873.15),77.430195);
		Check("Visc simple (i)",viscSimple(1,1173.15),44.217245);
		Check("Visc simple (j)",viscSimple(100,1173.15),47.640433);
		Check("Visc simple (k)",viscSimple(400,1173.15),64.154608);
		//reverse region 3 h(T,P)
		Check("h3ab",h3ab(25),2.095936454e3);
		Check("T3ph 3a (a)",T3ph(20,1700),6.293083892e2);
		Check("T3ph 3a (b)",T3ph(50,2000),6.905718338e2);
		Check("T3ph 3a (c)",T3ph(100,2100),7.336163014e2);
		Check("T3ph 3b (a)",T3ph(20,2500),6.418418053e2);
		Check("T3ph 3b (b)",T3ph(50,2400),7.351848618e2);
		Check("T3ph 3b (c)",T3ph(100,2700),8.420460876e2);
		Check("T3ps 3a (a)",T3ps(20,3.8),6.282959869e2);
		Check("T3ps 3a (b)",T3ps(50,3.6),6.297158726e2);
		Check("T3ps 3a (c)",T3ps(100,4),7.056880237e2);
		Check("T3ps 3b (a)",T3ps(20,5),6.401176443e2);
		Check("T3ps 3b (b)",T3ps(50,4.5),7.163687517e2);
		Check("T3ps 3b (c)",T3ps(100,5),8.474332825e2);
	}
#else
	//do not test release mode
	void CheckConsistency() {};
#endif

	//boundary between regions 2 and 3

	static double B23P(double T) {
		ASSERT((T>=B23TMIN)&&(T<=B23TMAX),"range error");
		return (n23B[0]+(n23B[1]+n23B[2]*T)*T);
	}

	static double B23T(double P) {
		ASSERT((P>=B23PMIN)&&(P<=B23PMAX),"range error");
		return n23B[3]+sqrt((P-n23B[4])/n23B[2]);
	}

	//region 1 equations

	void SetReg1(double P,double T) {
		this->T=T;
		this->P=P;
		Prel=P/16.53;
		Trel=1386.0/T;
		X=0;//water only in region 1
		SetGamma1();
		region=1;
	}

	void SetGamma1() {
		//relative gibbs energy: master equation for region 1
		// also calculate all derivatives (1st and 2nd order)
		// make sure to call when setting state to region 1
		int i,I,J;
		double d1=7.1-Prel;
		double d2=Trel-1.222;
		double d;
		G=GP=GPP=GT=GTT=GTP=0;
		for (i=0; i<34; i++) {
			I=reg1i[i];
			J=reg1j[i];
			d=reg1n[i]*intpow(d1,I)*intpow(d2,J);
			G+=d;
			if (I) {
				GP-=d*(double)I;
				GPP+=d*double(I)*(double(I)-1.0);
			}
			if (J) {
				GT+=d*(double)J;
				GTT+=d*double(J)*(double(J)-1.0);
				if (I) {
					GTP-=d*(double)I*(double)J;
				}
			}
		}
		GP/=d1;
		GPP/=(d1*d1);
		GT/=d2;
		GTT/=(d2*d2);
		GTP/=(d1*d2);
	}

	static double T1fromPH(double P,double H) {
		//T from P and H in region 1
		int i;
		double T=0;
		double h=H/2500+1.0;
		for (i=0; i<20; i++) {
			T+=reg1THn[i]*intpow(P,reg1THi[i])*intpow(h,reg1THj[i]);
		}
		return T;
	}

	static double T1fromPS(double P, double S) {
		int i;
		double T=0;
		double s=S+2.0;
		for (i=0; i<20; i++) {
			T+=reg1TSn[i]*intpow(P,reg1TSi[i])*intpow(s,reg1TSj[i]);
		}
		return T;
	}

	//region 2 functions

	void SetReg2(double P,double T) {
		bool isMetaStableVapor;
		this->T=T;
		this->P=P;
		Prel=P;
		Trel=540.0/T;
		X=1.0;//steam only in region 2
		region=2;
		isMetaStableVapor=false;
		if (T<=TCRIT) {
			double Ps=Psat(T);
			if (P>Ps) {
				isMetaStableVapor=true;
			}
		}
		if (!isMetaStableVapor) {
			//stable
			SetGam0_2();
			SetGamR2();
		} else {
			//metastable
			SetGam0_2(true);
			SetGamR2MS();
		}
	}

	void SetGam0_2(bool metaStableVapor=false) {
		const double *reg2N=(metaStableVapor)?reg2Ni0MS:reg2Ni0;
		int i,J;
		double d;
		G0=log(Prel);
		G0T=G0TT=0;
		for (i=0; i<9; i++) {
			J=reg2Ji0[i];
			d=reg2N[i]*intpow(Trel,J);
			G0+=d;
			G0T+=d*(double)J;
			G0TT+=d*(double)J*((double)J-1.0);
		}
		G0T/=Trel;
		G0TT/=(Trel*Trel);
	}

	void SetGamR2() {
		double d1=Trel-0.5;
		int i,I,J;
		double d;
		G=GP=GPP=GT=GTT=GTP=0;
		for (i=0; i<43; i++) {
			I=reg2IiR[i];
			J=reg2JiR[i];
			d=reg2NiR[i]*intpow(Prel,I)*intpow(d1,J);
			G+=d;
			GP+=d*(double)I;
			GPP+=d*(double)I*((double)I-1.0);
			GT+=d*(double)J;
			GTT+=d*(double)J*((double)J-1.0);
			GTP+=d*(double)J*(double)I;
		}
		GP/=Prel;
		GPP/=(Prel*Prel);
		GT/=d1;
		GTT/=(d1*d1);
		GTP/=(d1*Prel);
	}

	void SetGamR2MS() {
		double d1=Trel-0.5;
		int i,I,J;
		double d;
		G=GP=GPP=GT=GTT=GTP=0;
		for (i=0; i<13; i++) {
			I=reg2IiRMS[i];
			J=reg2JiRMS[i];
			d=reg2NiRMS[i]*intpow(Prel,I)*intpow(d1,J);
			G+=d;
			GP+=d*(double)I;
			GPP+=d*(double)I*((double)I-1.0);
			GT+=d*(double)J;
			GTT+=d*(double)J*((double)J-1.0);
			GTP+=d*(double)J*(double)I;
		}
		GP/=Prel;
		GPP/=(Prel*Prel);
		GT/=d1;
		GTT/=(d1*d1);
		GTP/=(d1*Prel);
	}

	static double T2fromPH(double P,double H) {
		double h,pi;
		double T=0;
		int i;
		h=H/2000.0;
		if (P<4.0) {
			//region 2a
			h-=2.1;
			for (i=0; i<34; i++) {
				T+=reg2aTHn[i]*intpow(P,reg2aTHi[i])*intpow(h,reg2aTHj[i]);
			}
		} else {
			double P2bc=0.90584278514723e3-H*0.67955786399241+H*H*0.12809002730136e-3;
			if (P<P2bc) {
				//region 2b
				h-=2.6;
				pi=P-2.0;
				for (i=0; i<38; i++) {
					T+=reg2bTHn[i]*intpow(pi,reg2bTHi[i])*intpow(h,reg2bTHj[i]);
				}
			} else {
				//region 2c
				h-=1.8;
				pi=P+25.0;
				for (i=0; i<23; i++) {
					T+=reg2cTHn[i]*intpow(pi,reg2cTHi[i])*intpow(h,reg2cTHj[i]);
				}
			}
		}
		return T;
	}

	static double T2fromPS(double P,double S) {
		double s;
		int i;
		double T=0;
		if (P<4.0) {
			//region 2a
			s=S/2.0-2.0;
			for (i=0; i<46; i++) {
				T+=reg2aTSn[i]*pow(P,reg2aTSi[i])*intpow(s,reg2aTSj[i]);
			}
		} else if (S>=5.85) {
			//region 2b
			s=10.0-S/0.7853;
			for (i=0; i<44; i++) {
				T+=reg2bTSn[i]*intpow(P,reg2bTSi[i])*intpow(s,reg2bTSj[i]);
			}
		} else {
			//region 2c
			s=2-S/2.9251;
			for (i=0; i<30; i++) {
				T+=reg2cTSn[i]*intpow(P,reg2cTSi[i])*intpow(s,reg2cTSj[i]);
			}
		}
		return T;
	}

	//region 3 equations

	void SetReg3(double rho,double T) {
		//set the state in region 3
		// the function that uses this routine to set the state,
		// will have to also set X
		this->T=T;
		int i,I,J;
		double d;
		region=3;
		Trel=TCRIT/T;
		rhoRel=rho/RHOCRIT;
		F=reg3Ni[0]*log(rhoRel);
		FR=FRR=FT=FTT=FTR=0;
		for (i=1; i<40; i++) {
			I=reg3Ii[i];
			J=reg3Ji[i];
			d=reg3Ni[i]*intpow(rhoRel,I)*intpow(Trel,J);
			F+=d;
			FR+=d*(double)I;
			FRR+=d*(double)I*((double)I-1.0);
			FT+=d*(double)J;
			FTT+=d*(double)J*((double)J-1.0);
			FTR+=d*(double)J*(double)I;
		}
		FR+=reg3Ni[0];
		FR/=rhoRel;
		FRR-=reg3Ni[0];
		FRR/=(rhoRel*rhoRel);
		FT/=Trel;
		FTT/=(Trel*Trel);
		FTR/=(rhoRel*Trel);
		//calculate P
		P=rho*GASCONSTANT*T*rhoRel*FR*1e-3;
	}

	//backwards equation v(T,P)

	static double T3ab(double P) {
		double T=0;
		int i;
		for (i=0; i<5; i++) {
			T+=T3abN[i]*intpow(log(P),T3abI[i]);
		}
		return T;
	}

	static double T3cd(double P) {
		double T=0;
		int i;
		for (i=0; i<4; i++) {
			T+=T3cdN[i]*intpow(P,T3cdI[i]);
		}
		return T;
	}

	static double T3ef(double P) {
		return 3.727888004*(P-22.064)+647.096;
	}

	static double T3gh(double P) {
		double T=0;
		int i;
		for (i=0; i<5; i++) {
			T+=T3ghN[i]*intpow(P,T3ghI[i]);
		}
		return T;
	}

	static double T3ij(double P) {
		double T=0;
		int i;
		for (i=0; i<5; i++) {
			T+=T3ijN[i]*intpow(P,T3ijI[i]);
		}
		return T;
	}

	static double T3jk(double P) {
		double T=0;
		int i;
		for (i=0; i<5; i++) {
			T+=T3jkN[i]*intpow(P,T3jkI[i]);
		}
		return T;
	}

	static double T3mn(double P) {
		double T=0;
		int i;
		for (i=0; i<4; i++) {
			T+=T3mnN[i]*intpow(P,T3mnI[i]);
		}
		return T;
	}

	static double T3op(double P) {
		double T=0;
		int i;
		for (i=0; i<5; i++) {
			T+=T3opN[i]*intpow(log(P),T3opI[i]);
		}
		return T;
	}

	static double T3qu(double P) {
		double T=0;
		int i;
		for (i=0; i<4; i++) {
			T+=T3quN[i]*intpow(P,T3quI[i]);
		}
		return T;
	}

	static double T3rx(double P) {
		double T=0;
		int i;
		for (i=0; i<4; i++) {
			T+=T3rxN[i]*intpow(P,T3rxI[i]);
		}
		return T;
	}

	static double V3FromPT(double P,double T) {
		double V;
		if (P>40.0) {
			if (T<=T3ab(P)) {
				V=V3FromPTa(P,T);
			} else {
				V=V3FromPTb(P,T);
			}
		} else if (P>25.0) {
			if (T<=T3cd(P)) {
				V=V3FromPTc(P,T);
			} else if (T<=T3ab(P)) {
				V=V3FromPTd(P,T);
			} else if (T<=T3ef(P)) {
				V=V3FromPTe(P,T);
			} else {
				V=V3FromPTf(P,T);
			}
		} else if (P>23.5) {
			if (T<=T3cd(P)) {
				V=V3FromPTc(P,T);
			} else if (T<=T3gh(P)) {
				V=V3FromPTg(P,T);
			} else if (T<=T3ef(P)) {
				V=V3FromPTh(P,T);
			} else if (T<=T3ij(P)) {
				V=V3FromPTi(P,T);
			} else if (T<=T3jk(P)) {
				V=V3FromPTj(P,T);
			} else {
				V=V3FromPTk(P,T);
			}
		} else if (P>23.0) {
			if (T<=T3cd(P)) {
				V=V3FromPTc(P,T);
			} else if (T<=T3gh(P)) {
				V=V3FromPTl(P,T);
			} else if (T<=T3ef(P)) {
				V=V3FromPTh(P,T);
			} else if (T<=T3ij(P)) {
				V=V3FromPTi(P,T);
			} else if (T<=T3jk(P)) {
				V=V3FromPTj(P,T);
			} else {
				V=V3FromPTk(P,T);
			}
		} else if (P>22.5) {
			if (T<=T3cd(P)) {
				V=V3FromPTc(P,T);
			} else if (T<=T3gh(P)) {
				V=V3FromPTl(P,T);
			} else if (T<=T3mn(P)) {
				V=V3FromPTm(P,T);
			} else if (T<=T3ef(P)) {
				V=V3FromPTn(P,T);
			} else if (T<=T3op(P)) {
				V=V3FromPTo(P,T);
			} else if (T<=T3ij(P)) {
				V=V3FromPTp(P,T);
			} else if (T<=T3jk(P)) {
				V=V3FromPTj(P,T);
			} else {
				V=V3FromPTk(P,T);
			}
		} else if (P>Psat(643.15)) {
			if (T<=T3cd(P)) {
				V=V3FromPTc(P,T);
			} else if (T<=T3qu(P)) {
				V=V3FromPTq(P,T);
			} else if (T<=T3rx(P)) {
				V=V3FromPTNearCritical(P,T);
			} else if (T<=T3jk(P)) {
				V=V3FromPTr(P,T);
			} else {
				V=V3FromPTk(P,T);
			}
		} else if (P>20.5) {
			if (T<=T3cd(P)) {
				V=V3FromPTc(P,T);
			} else if (T<=Tsat(P)) {
				V=V3FromPTs(P,T);
			} else if (T<=T3jk(P)) {
				V=V3FromPTr(P,T);
			} else {
				V=V3FromPTk(P,T);
			}
		} else if (P>1.900881189173929e1) {
			if (T<=T3cd(P)) {
				V=V3FromPTc(P,T);
			} else if (T<=Tsat(P)) {
				V=V3FromPTs(P,T);
			} else {
				V=V3FromPTt(P,T);
			}
		} else {
			if (T<=Tsat(P)) {
				V=V3FromPTc(P,T);
			} else {
				V=V3FromPTt(P,T);
			}
		}
		return V;
	}

	//function macro for most sub regions

#define v3fun(name,NNN,III,JJJ,AAA,BBB,ARG1,ARG2,ARG3,PSTAR,TSTAR,VSTAR,NUMCOEF) \
	static double name(double P,double T) \
	{double Prel=P/PSTAR; \
		double Trel=T/TSTAR; \
		double d1=Prel-AAA; \
		double d2=Trel-BBB; \
		double V=0; \
		int i; \
		for (i=0;i<NUMCOEF;i++) V+=NNN[i]*intpow(ARG1,III[i])*intpow(ARG2,JJJ[i]); \
		return VSTAR*ARG3; \
	}

	//define functions
	v3fun(V3FromPTa,V3Na,V3Ia,V3Ja,0.085,0.817,d1,d2,V,100.0,760.0,0.0024,30);
	v3fun(V3FromPTb,V3Nb,V3Ib,V3Jb,0.28,0.779,d1,d2,V,100.0,860.0,0.0041,32);
	v3fun(V3FromPTc,V3Nc,V3Ic,V3Jc,0.259,0.903,d1,d2,V,40.0,690.0,0.0022,35);
	v3fun(V3FromPTd,V3Nd,V3Id,V3Jd,0.559,0.939,d1,d2,pow(V,4),40.0,690.0,0.0029,38);
	v3fun(V3FromPTe,V3Ne,V3Ie,V3Je,0.587,0.918,d1,d2,V,40.0,710.0,0.0032,29);
	v3fun(V3FromPTf,V3Nf,V3If,V3Jf,0.587,0.891,sqrt(d1),d2,pow(V,4),40.0,730.0,0.0064,42);
	v3fun(V3FromPTg,V3Ng,V3Ig,V3Jg,0.872,0.971,d1,d2,pow(V,4),25.0,660.0,0.0027,38);
	v3fun(V3FromPTh,V3Nh,V3Ih,V3Jh,0.898,0.983,d1,d2,pow(V,4),25.0,660.0,0.0032,29);
	v3fun(V3FromPTi,V3Ni,V3Ii,V3Ji,0.91,0.984,sqrt(d1),d2,pow(V,4),25.0,660.0,0.0041,42);
	v3fun(V3FromPTj,V3Nj,V3Ij,V3Jj,0.875,0.964,sqrt(d1),d2,pow(V,4),25.0,670.0,0.0054,29);
	v3fun(V3FromPTk,V3Nk,V3Ik,V3Jk,0.802,0.935,d1,d2,V,25.0,680.0,0.0077,34);
	v3fun(V3FromPTl,V3Nl,V3Il,V3Jl,0.908,0.989,d1,d2,pow(V,4),24.0,650.0,0.0026,43);
	v3fun(V3FromPTm,V3Nm,V3Im,V3Jm,1,0.997,d1,pow(d2,0.25),V,23.0,650.0,0.0028,40);
	v3fun(V3FromPTo,V3No,V3Io,V3Jo,0.974,0.996,sqrt(d1),d2,V,23.0,650.0,0.0034,24);
	v3fun(V3FromPTp,V3Np,V3Ip,V3Jp,0.972,0.997,sqrt(d1),d2,V,23.0,650.0,0.0041,27);
	v3fun(V3FromPTq,V3Nq,V3Iq,V3Jq,0.848,0.983,d1,d2,pow(V,4),23.0,650.0,0.0022,24);
	v3fun(V3FromPTr,V3Nr,V3Ir,V3Jr,0.874,0.982,d1,d2,V,23.0,650.0,0.0054,27);
	v3fun(V3FromPTs,V3Ns,V3Is,V3Js,0.886,0.99,d1,d2,pow(V,4),21.0,640.0,0.0022,29);
	v3fun(V3FromPTt,V3Nt,V3It,V3Jt,0.803,1.02,d1,d2,V,20.0,650.0,0.0088,33);
	v3fun(V3FromPTu,V3Nu,V3Iu,V3Ju,0.902,0.988,d1,d2,V,23.0,650.0,0.0026,38);
	v3fun(V3FromPTv,V3Nv,V3Iv,V3Jv,0.96,0.995,d1,d2,V,23.0,650.0,0.0031,39);
	v3fun(V3FromPTw,V3Nw,V3Iw,V3Jw,0.959,0.995,d1,d2,pow(V,4),23.0,650.0,0.0039,35);
	v3fun(V3FromPTx,V3Nx,V3Ix,V3Jx,0.91,0.988,d1,d2,V,23.0,650.0,0.0049,36);
	v3fun(V3FromPTy,V3Ny,V3Iy,V3Jy,0.996,0.994,d1,d2,pow(V,4),22.0,650.0,0.0031,20);
	v3fun(V3FromPTz,V3Nz,V3Iz,V3Jz,0.993,0.994,d1,d2,pow(V,4),22.0,650.0,0.0038,23);

	static double V3FromPTn(double P,double T) {
		double Prel=P/23.0;
		double Trel=T/650.0;
		double d1=Prel-0.976;
		double d2=Trel-0.997;
		double V=0;
		int i;
		for (i=0; i<39; i++) {
			V+=V3Nn[i]*intpow(d1,V3In[i])*intpow(d2,V3Jn[i]);
		}
		return 0.0031*exp(V);
	}

	static double V3FromPTNearCritical(double P,double T) {
		double V;
		if (P>22.11) {
			if (T<=T3uv(P)) {
				V=V3FromPTu(P,T);
			} else if (T<=T3ef(P)) {
				V=V3FromPTv(P,T);
			} else if (T<=T3wx(P)) {
				V=V3FromPTw(P,T);
			} else {
				V=V3FromPTx(P,T);
			}
		} else if (P>PCRIT) {
			if (T<=T3uv(P)) {
				V=V3FromPTu(P,T);
			} else if (T<=T3ef(P)) {
				V=V3FromPTy(P,T);
			} else if (T<=T3wx(P)) {
				V=V3FromPTz(P,T);
			} else {
				V=V3FromPTx(P,T);
			}
		} else {
			if (T<=Tsat(P)) {
				if (P>2.193161551e1) {
					if (T<T3uv(P)) {
						V=V3FromPTu(P,T);
					} else {
						V=V3FromPTy(P,T);
					}
				} else {
					V=V3FromPTu(P,T);
				}
			} else {
				if (P>2.190096265e1) {
					if (T<T3wx(P)) {
						V=V3FromPTz(P,T);
					} else {
						V=V3FromPTx(P,T);
					}
				} else {
					V=V3FromPTx(P,T);
				}
			}
		}
		return V;
	}

	static double T3uv(double P) {
		double T=0;
		int i;
		for (i=0; i<4; i++) {
			T+=T3uvN[i]*intpow(P,T3uvI[i]);
		}
		return T;
	}

	static double T3wx(double P) {
		double T=0;
		int i;
		for (i=0; i<5; i++) {
			T+=T3wxN[i]*intpow(log(P),T3wxI[i]);
		}
		return T;
	}

	//backwards equation T(p,h)

	static double h3ab(double P) {
		//region 3b if h>h3ab
		return h3abn[0]+P*(h3abn[1]+P*(h3abn[2]+P*h3abn[3]));
	}

	static double T3ph(double P,double H) {
		double p;
		double T;
		double h;
		int i;
		if (H<=h3ab(P)) {
			//region 3a
			p=P/100.0+0.240;
			h=H/2300.0-0.615;
			T=0;
			for (i=0; i<31; i++) {
				T+=t3aphn[i]*intpow(p,t3aphi[i])*intpow(h,t3aphj[i]);
			}
			T*=760.0;
		} else {
			//region 3b
			p=P/100.0+0.298;
			h=H/2800.0-0.720;
			T=0;
			for (i=0; i<33; i++) {
				T+=t3bphn[i]*intpow(p,t3bphi[i])*intpow(h,t3bphj[i]);
			}
			T*=860.0;
		}
		return T;
	}

	//backwards equation T(p,s)

	static double T3ps(double P,double S) {
		double p;
		double T;
		double s;
		int i;
		if (S<=4.41202148223476) {
			//region 3a
			p=P/100.0+0.240;
			s=S/4.4-0.703;
			T=0;
			for (i=0; i<33; i++) {
				T+=t3apsn[i]*intpow(p,t3apsi[i])*intpow(s,t3apsj[i]);
			}
			T*=760.0;
		} else {
			//region 3b
			p=P/100.0+0.760;
			s=S/5.3-0.818;
			T=0;
			for (i=0; i<28; i++) {
				T+=t3bpsn[i]*intpow(p,t3bpsi[i])*intpow(s,t3bpsj[i]);
			}
			T*=860.0;
		}
		return T;
	}

public:

	//region 4
	// when setting the state, make sure that T, P and X are consistent

	static double Psat(double T) {
		if (T<TMIN) {
			throw WaterException("T below minimum temperature of 0C");
		}
		if (T>TCRIT) {
			throw WaterException("T above critical temperature");
		}
		double A,B,C;
		double d;
		double Trel=T+reg4Ni[8]/(T-reg4Ni[9]);
		A=Trel*(Trel+reg4Ni[0])+reg4Ni[1];
		B=Trel*(reg4Ni[2]*Trel+reg4Ni[3])+reg4Ni[4];
		C=Trel*(reg4Ni[5]*Trel+reg4Ni[6])+reg4Ni[7];
		d=2*C/(-B+sqrt(B*B-4.0*A*C));
		d=d*d;
		return d*d;
	}

	static double Tsat(double P) {
		if (P<611.213e-6) {
			throw WaterException("P below minimum of 611.213 Pa");
		}
		if (P>22.064) {
			throw WaterException("P above critical pressure");
		}
		double beta,D,E,F,G,d;
		beta=pow(P,0.25);
		E=beta*(beta+reg4Ni[2])+reg4Ni[5];
		F=beta*(reg4Ni[0]*beta+reg4Ni[3])+reg4Ni[6];
		G=beta*(reg4Ni[1]*beta+reg4Ni[4])+reg4Ni[7];
		D=(2.0*G)/(-F-sqrt(F*F-4.0*E*G));
		d=reg4Ni[9]+D;
		return 0.5*(reg4Ni[9]+D-sqrt(d*d-4.0*(reg4Ni[8]+reg4Ni[9]*D)));
	}

private:

	//region 5 equations

	void SetReg5(double P,double T) {
		this->T=T;
		this->P=P;
		ASSERT((T>=1073.15)&&(T<=2273.15),"range error");
		ASSERT((P>=0)&&(P<=P5MAX),"range error");
		Prel=P;
		Trel=1e3/T;
		X=1.0;//steam only in region 5
		region=5;
		SetGam0_5();
		SetGamR5();
	}

	void SetGam0_5() {
		int i,J;
		double d;
		G0=log(Prel);
		G0T=G0TT=0;
		for (i=0; i<6; i++) {
			J=reg5Ji0[i];
			d=reg5Ni0[i]*intpow(Trel,J);
			G0+=d;
			G0T+=d*(double)J;
			G0TT+=d*(double)J*((double)J-1.0);
		}
		G0T/=Trel;
		G0TT/=(Trel*Trel);
	}

	void SetGamR5() {
		int i,I,J;
		double d;
		G=GP=GPP=GT=GTT=GTP=0;
		for (i=0; i<6; i++) {
			I=reg5IiR[i];
			J=reg5JiR[i];
			d=reg5NiR[i]*intpow(Prel,I)*intpow(Trel,J);
			G+=d;
			GP+=d*(double)I;
			GPP+=d*(double)I*((double)I-1.0);
			GT+=d*(double)J;
			GTT+=d*(double)J*((double)J-1.0);
			GTP+=d*(double)J*(double)I;
		}
		GP/=Prel;
		GPP/=(Prel*Prel);
		GT/=Trel;
		GTT/=(Trel*Trel);
		GTP/=(Trel*Prel);
	}

private:

	//one dimensional solver function with bounds

	void Solve(std::function<double(double)> tgtFun,double &X,double minX,double maxX,double tol) {
		//root finder; make tgtFun(X,param,target,this)=0
		// root finder is Brent's 1-dimensional root-finder
		// Algorithm
		//	G.Forsythe, M.Malcolm, C.Moler, Computer methods for mathematical
		//	computations. M., Mir, 1980, p.180 of the Russian edition
		//based on netlib implementation by oleg
		if (maxX<=minX) {
			throw WaterException("zero or negative range in solver");
		}
		double b=maxX;
		double fb=tgtFun(b);
		double a=minX;
		double fa=tgtFun(a);
		double c=a;
		double fc=fa;
		if (fa*fb>0) {
			throw WaterException("no solution in specified range");
		}
		while (true) {
			double prev_step=b-a;
			if(fabs(fc)<fabs(fb)) {
				a=b;
				b=c;
				c=a;
				fa=fb;
				fb=fc;
				fc=fa;
			}
			double tol_act=2.0*2.22045e-16*fabs(b)+tol*0.5;
			double new_step=(c-b)*0.5;
			if ((fabs(new_step)<=tol_act)||(fb==0)) {
				X=b;
				return;
			}
			if ((fabs(prev_step)>=tol_act)&&(fabs(fa)>fabs(fb))) {
				double p;
				double q;
				double cb=c-b;
				if (a==c) {
					double t1=fb/fa;
					p=cb*t1;
					q=1.0-t1;
				} else {
					double t1=fb/fc;
					double t2=fb/fa;
					q=fa/fc;
					p=t2*(cb*q*(q-t1)-(b-a)*(t1-1.0));
					q=(q-1.0)*(t1-1.0)*(t2-1.0);
				}
				if (p>0) {
					q=-q;
				} else {
					p=-p;
				}
				if ((2.0*p<(1.5*cb*q-fabs(tol_act*q)))&&(2.0*p<fabs(prev_step*q))) {
					new_step = p/q;
				}
			}
			if(fabs(new_step)<tol_act) {
				new_step=(new_step>0)?tol_act:-tol_act;
			}
			a=b;
			fa=fb;
			b+=new_step;
			fb=tgtFun(b);
			if (((fb>0)&&(fc>0))||((fb<0)&&(fc<0))) {
				c=a;
				fc=fa;
			}
		}
		throw WaterException("internal error in solver");
	}

public:

	//functions to set state

	void SetStateTPX(double T,double P,STATETP state) {
		//TP flash, with reverse region 3 function, no solver required
		// also use this function to set state before requesting vapor or liquid properties
		if (T<TMIN) {
			//invalid region
			throw WaterException("T below lower limit of 273.15 K");
		}
		if (P<0) {
			//invalid region
			throw WaterException("negative pressure");
		}
		if (P>PMAX) {
			//invalid region
			throw WaterException("P above upper limit of 100 MPa");
		}
		if (T<=T13) {
			//region 1 or 2, depending on request
			if (state==TPLIQUID) {
				//liquid region 1
				SetReg1(P,T);
			} else if (state==TPVAPOR) {
				//vapor region 2
				SetReg2(P,T);
			} else {
				ASSERT(state==TPAUTO,"unexpected state");
				//depend on Psat
				if (P>Psat(T)) {
					SetReg1(P,T);
				} else {
					SetReg2(P,T);
				}
			}
		} else if (T<=T25) {
			//region 2 or 3
			if (T>=B23TMAX) {
				SetReg2(P,T);
			} else if (P>=B23P(T)) {
				//region 3
				X=1.0; //let's assume vapor in this region, unless we decide liquid
				if (T<TCRIT) {
					//make sure we end up in the right region
					double Ps=Psat(T);
					if ((fabs(Ps-P)<1e-4)&&(state!=TPAUTO)) {
						//at the sat line
						if (state==TPLIQUID) {
							P+=1e-3;    //ensure liquid
						} else {
							P-=1e-3;    //ensure vapor
						}
					}
					//set quality
					X=(P>Ps)?0.0:1.0; //vapor when P<=Psat
				}
				double rho=1.0/V3FromPT(P,T);
				SetReg3(rho,T);
			} else {
				SetReg2(P,T);
			}
		} else if (T<=TMAX) {
			//region 5
			if (P>P5MAX) {
				throw WaterException("no valid region for given T and P");
			}
			SetReg5(P,T);
		} else {
			throw WaterException("T above upper limit of 2273.15 K");
		}
	}

	void Refine(std::function<double(double)> tgt,double guess,double tol) {
		//refine a target function with guessed solution and positive slope and positive parameter
		ASSERT(guess>0,"parameter to Refine should be positive (see limit checks)");
		double f0=tgt(guess);
		if (fabs(f0)>tol) {
			//refinement required
			if (f0<0) {
				//find upper limit
				double f1=f0;
				double guess0=guess;
				double max=guess;
				double offset=1e-3;
				for (;;) {
					guess=max;
					max+=offset;
					f1=tgt(max);
					if (fabs(f1)<tol) {
						return;
					}
					if (f1>0) {
						double X;
						Solve(tgt,X,guess,max,tol);
						return;
					}
					if (max>1.1*guess0) {
						throw WaterException("solution refinement failed");
					}
					offset*=2.0;
				}
			}
			//find lower limit
			double f1=f0;
			double guess0=guess;
			double min=guess;
			double offset=1e-3;
			for (;;) {
				guess=min;
				min-=offset;
				f1=tgt(min);
				if (fabs(f1)<tol) {
					return;
				}
				if (f1<0) {
					double X;
					Solve(tgt,X,min,guess,tol);
					return;
				}
				if (min<0.9*guess0) {
					throw WaterException("solution refinement failed");
				}
				offset*=2.0;
			}
		}
	}

	void SetStatePH(double P, double H) {
		//PH flash, using inverse functions in regions 1, 2, 3. Solver required in region 5
		double t,p,Hliq,Hvap;
		if (P<0) {
			//invalid region
			throw WaterException("negative pressure");
		}
		if (P>PMAX) {
			//invalid region
			throw WaterException("P above upper limit of 100 MPa");
		}
		//first check along phase boundary
		if (P<=PCRIT) {
			t=Tsat(P);
			SetStateTPX(t,P,TPLIQUID);
			Hliq=enthalpy();
			SetStateTPX(t,P,TPVAPOR);
			Hvap=enthalpy();
			if ((H>=Hliq)&&(H<=Hvap)) {
				//two-phase solution
				region=4;
				T=t;
				X=(H-Hliq)/(Hvap-Hliq);
				return;
			}
			//check region 1:
			t=T1fromPH(P,H);
			//check for numerical inconsistency:
			if (t<TMIN)
				if (t>273.12) {
					//check if 273.15 will work:
					SetReg1(P,273.15);
					double Hcalc=enthalpy();
					if (fabs((Hcalc-H)/Hcalc)<1e-5) {
						return;    //ok
					}
				}
			if ((t<=TCRIT)&&(t>=TMIN))
				if (t<=T13) {
					p=Psat(t);//as T may be slightly inconsistent with the vapor pressure line, keep a small correction in below check
					if (P>=p-5e-4) {
						//refine
						Refine([this,P,H](double T) {
							SetReg1(P,T);
							return enthalpy()-H;
						},t,1e-6);
						return;
					}
				}
			//check region 2:
			t=T2fromPH(P,H); //as T may be slightly inconsistent with the vapor pressure line, keep a small correction in below check
			if ((t>=TMIN)&&(t<T25)) {
				if (t<=T13) {
					p=Psat(t);
					if (P<=p+5e-4) {
						//solution is in region 2
						Refine([this,P,H](double T) {
							SetReg2(P,T);
							return enthalpy()-H;
						},t,1e-6);
						return;
					}
				} else if (t<=B23TMAX) {
					p=B23P(t); //as T may be slightly inconsistent with the B23 line, keep a small correction in below check
					if (P<=p+5e-4) {
						//solution is in region 2
						Refine([this,P,H](double T) {
							SetReg2(P,T);
							return enthalpy()-H;
						},t,1e-6);
						return;
					}
				} else {
					//solution is in region 2
					Refine([this,P,H](double T) {
						SetReg2(P,T);
						return enthalpy()-H;
					},t,1e-6);
					return;
				}
			}
			//check region 3
			t=T3ph(P,H);
			if (t>=T13) {
				if (t<=B23TMAX) {
					p=B23P(t);
					if (P>=p) {
						//solution is in region 3
						Refine([this,P,H](double T) {
							SetReg3(1.0/V3FromPT(P,T),T);
							return enthalpy()-H;
						},t,1e-6);
						//water or steam?
						if (t>TCRIT) {
							X=1.0;    //supercritical, call it steam
						} else {
							//check vs Psat
							if (P<Psat(t)) {
								X=1.0;
							} else {
								X=0.0;
							}
						}
						return;
					}
				}
			}
		} else {
			//P>=PCRIT
			//check region 2:
			t=T2fromPH(P,H);
			if ((t>=TCRIT)&&(t<=T25)) {
				p=B23P(t);
				if (P<=p) {
					//solution is in region 2
					Refine([this,P,H](double T) {
						SetReg2(P,T);
						return enthalpy()-H;
					},t,1e-6);
					return;
				}
			}
			//check region 3
			t=T3ph(P,H);
			if (t>=T13) {
				if (t<=B23TMAX) {
					p=B23P(t);
					if (P>=p) {
						//solution is in region 3
						Refine([this,P,H](double T) {
							SetReg3(1.0/V3FromPT(P,T),T);
							return enthalpy()-H;
						},t,1e-6);
						//water or steam?
						if (t>TCRIT) {
							X=1.0;    //supercritical, call it steam
						} else {
							//check vs Psat
							if (P<Psat(t)) {
								X=1.0;
							} else {
								X=0.0;
							}
						}
						return;
					}
				}
			}
		}
		//the solution appears not to be in regions 1-4
		if (P<=P5MAX) {
			//check region 5
			try {
				Solve([this,P,H](double T) -> double {
					SetReg5(P,T);
					return H-enthalpy();
				},t,T25,TMAX,1e-6);
				return;
			} catch (...) {
			}
		}
		throw WaterException("No solution found for PH specification");
	}

	void SetStateTH(double T, double H) {
		//T-H flash. Water seems a bit weird in that liquid enthalpy increases (slightly) with pressure,
		// hence, when given a liquid enthalpy, there is a corresponding two-phase solution
		if (T<TMIN) {
			throw WaterException("T below lower limit of 273.15 K");
		}
		if (T>TMAX) {
			throw WaterException("T above upper limit of 2273.15 K");
		}
		double p;
		double pmin=missing;
		double pmax=missing;
		//check phase boundary
		if (T<=TCRIT) {
			p=Psat(T);
			SetStateTPX(T,p,TPLIQUID);
			double Hliq=enthalpy();
			SetStateTPX(T,p,TPVAPOR);
			double Hvap=enthalpy();
			if ((H>=Hliq)&&(H<=Hvap)) {
				//two-phase solution
				region=4;
				P=p;
				X=(H-Hliq)/(Hvap-Hliq);
				return;
			}
			if (H>Hvap) {
				//search vapor
				pmax=p;
			} else {
				//search liquid
				pmin=p;
			}
		}
		if (pmin==missing) {
			pmin=1e-8;    //do not search zero
		}
		if (pmax==missing) {
			if (T<=T25) {
				pmax=PMAX;
			} else {
				pmax=P5MAX;
			}
		}
		//use solver
		try {
			Solve([this,T,H](double P) -> double {
					SetStateTPX(T,P,TPAUTO);
					return H-enthalpy();
				},p,pmin,pmax,1e-8);
			return;
		} catch (WaterException ex) {
			throw WaterException("No solution found for TH specification: ",ex);
		}
	}

	void SetStatePS(double P, double S) {
		//PS flash, using inverse functions in regions 1, 2, 3. Solver required in region 5
		double t,p,Sliq,Svap;
		if (P<0) {
			//invalid region
			throw WaterException("negative pressure");
		}
		if (P>PMAX) {
			//invalid region
			throw WaterException("P above upper limit of 100 MPa");
		}
		//first check along phase boundary
		if (P<=PCRIT) {
			t=Tsat(P);
			SetStateTPX(t,P,TPLIQUID);
			Sliq=entropy();
			SetStateTPX(t,P,TPVAPOR);
			Svap=entropy();
			if ((S>=Sliq)&&(S<=Svap)) {
				//two-phase solution
				region=4;
				T=t;
				X=(S-Sliq)/(Svap-Sliq);
				return;
			}
			//check region 1:
			t=T1fromPS(P,S);
			//check for numerical inconsistency:
			if (t<TMIN) {
				if (t>273.12) {
					//check if 273.15 will work:
					SetReg1(P,273.15);
					double Scalc=entropy();
					if (fabs((Scalc-S)/Scalc)<1e-5) {
						return;    //ok
					}
				}
			}
			if ((t<=TCRIT)&&(t>=TMIN)) {
				if (t<=T13) {
					p=Psat(t);
					if (P>=p) {
						//solution is in region 1
						Refine([this,P,S](double T) {
							SetReg1(P,T);
							return entropy()-S;
						},t,1e-6);
						return;
					}
				}
			}
			//check region 2:
			t=T2fromPS(P,S);
			if ((t>=TMIN)&&(t<T25)) {
				if (t<=T13) {
					p=Psat(t);
					if (P<=p) {
						//solution is in region 2
						Refine([this,P,S](double T) {
							SetReg2(P,T);
							return entropy()-S;
						},t,1e-6);
						return;
					}
				} else if (t<=B23TMAX) {
					p=B23P(t);
					if (P<=p) {
						//solution is in region 2
						Refine([this,P,S](double T) {
							SetReg2(P,T);
							return entropy()-S;
						},t,1e-6);
						return;
					}
				} else {
					//solution is in region 2
					Refine([this,P,S](double T) {
						SetReg2(P,T);
						return entropy()-S;
					},t,1e-6);
					return;
				}
			}
			//check region 3
			t=T3ps(P,S);
			if (t>=T13) {
				if (t<=B23TMAX) {
					p=B23P(t);
					if (P>=p) {
						//solution is in region 3
						Refine([this,P,S](double T) {
							SetReg3(1.0/V3FromPT(P,T),T);
							return entropy()-S;
						},t,1e-6);
						//water or steam?
						if (t>TCRIT) {
							X=1.0;    //supercritical, call it steam
						} else {
							//check vs Psat
							if (P<Psat(t)) {
								X=1.0;
							} else {
								X=0.0;
							}
						}
						return;
					}
				}
			}
		} else {
			//P>=PCRIT
			//check region 2:
			t=T2fromPS(P,S);
			if ((t>=TCRIT)&&(t<=T25)) {
				p=B23P(t);
				if (P<=p) {
					//solution is in region 2
					Refine([this,P,S](double T) {
						SetReg2(P,T);
						return entropy()-S;
					},t,1e-6);
					return;
				}
			}
			//check region 3
			t=T3ps(P,S);
			if (t>=T13) {
				if (t<=B23TMAX) {
					p=B23P(t);
					if (P>=p) {
						//solution is in region 3
						Refine([this,P,S](double T) {
							SetReg3(1.0/V3FromPT(P,T),T);
							return entropy()-S;
						},t,1e-6);
						//water or steam?
						if (t>TCRIT) {
							X=1.0;    //supercritical, call it steam
						} else {
							//check vs Psat
							if (P<Psat(t)) {
								X=1.0;
							} else {
								X=0.0;
							}
						}
						return;
					}
				}
			}
		}
		//the solution appears not to be in regions 1-4
		if (P<=P5MAX) {
			//check region 5
			try {
				Solve([this,P,S](double T) -> double {
					SetReg5(P,T);
					return S-entropy();
				},t,T25,TMAX,1e-6);
				return;
			} catch (...) {
			}
		}
		throw WaterException("No solution found for PS specification");
	}

	void SetStateTS(double T,double S) {
		//T-S flash.
		if (T<TMIN) {
			throw WaterException("T below lower limit of 273.15 K");
		}
		if (T>TMAX) {
			throw WaterException("T above upper limit of 2273.15 K");
		}
		double p;
		double pmin=missing;
		double pmax=missing;
		//check phase boundary
		if (T<=TCRIT) {
			p=Psat(T);
			SetStateTPX(T,p,TPLIQUID);
			double Sliq=entropy();
			SetStateTPX(T,p,TPVAPOR);
			double Svap=entropy();
			if ((S>=Sliq)&&(S<=Svap)) {
				//two-phase solution
				region=4;
				P=p;
				X=(S-Sliq)/(Svap-Sliq);
				return;
			}
			if (S>Svap) {
				//search vapor
				pmax=p;
			} else {
				//search liquid
				pmin=p;
			}
		}
		if (pmin==missing) {
			pmin=1e-8;    //do not search zero
		}
		if (pmax==missing) {
			if (T<=T25) {
				pmax=PMAX;
			} else {
				pmax=P5MAX;
			}
		}
		//use solver
		try {
			Solve([this,T,S](double P) -> double {
					SetStateTPX(T,P,TPAUTO);
					return S-entropy();
				},p,pmin,pmax,1e-8);
			return;
		} catch (WaterException ex) {
			throw WaterException("No solution found for TS specification: ",ex);
		}
	}

	void SetStateHVF(double H,double X) {
		//solve for vapor fraction at given enthalpy
		if (X<0) {
			throw WaterException("negative vapor fraction");
		}
		if (X>1.0) {
			throw WaterException("vapor fraction larger than unity");
		}
		//use solver
		try {
			double t;
			Solve([this,X,H](double T)->double {
					double h=0;
					double P=Water::Psat(T);
					if (X>0.0) {
						//calculate vapor
						SetStateTPX(T,P,TPVAPOR);
						h+=X*enthalpy();
					}
					if (X<1.0) {
						//calculate liquid
						SetStateTPX(T,P,TPLIQUID);
						h+=(1.0-X)*enthalpy();
					}
					return H-h;
				},t,TMIN,TCRIT,1e-6);
			//solution ok
			this->X=X;
			this->T=t;
			this->P=Psat(t);
			this->region=4;
			return;
		} catch (WaterException ex) {
			throw WaterException("No solution found for HVF specification: ",ex);
		}
	}

	void SetStateSVF(double S,double X) {
		//solve for vapor fraction at given entropy
		if (X<0) {
			throw WaterException("negative vapor fraction");
		}
		if (X>1.0) {
			throw WaterException("vapor fraction larger than unity");
		}
		//use solver
		double t;
		try {
			Solve([this,X,S](double T)->double {
				double s=0;
				double P=Water::Psat(T);
				if (X>0.0) {
					//calculate vapor
					SetStateTPX(T,P,TPVAPOR);
					s+=X*entropy();
				}
				if (X<1.0) {
					//calculate liquid
					SetStateTPX(T,P,TPLIQUID);
					s+=(1.0-X)*entropy();
				}
				return S-s;
			},t,TMIN,TCRIT,1e-6);
			//solution ok
			this->X=X;
			this->T=t;
			this->P=Psat(t);
			this->region=4;
			return;
		} catch (WaterException ex) {
			throw WaterException("No solution found for SVF specification: ",ex);
		}
	}


public:

	//physical properties, for an initialized object:

	double gibbs() { //kJ/kg
		double res;
		switch (region) {
		case 1:
			res=GASCONSTANT*T*G;
			break;
		case 5:
		case 2:
			res=GASCONSTANT*T*(G0+G);
			break;
		case 3:
			res=enthalpy()-T*entropy();
			break;
		case 4:
			ASSERT(0,"don't ask for properties at the sat line directly");
			break;
		default:
			ASSERT(0,"invalid region");
			break;
		}
		return res;
	}

	double volume() { //m3/kg
		double res;
		switch (region) {
		case 1:
			res=GASCONSTANT*T/16.53*GP;
			break;
		case 5:
		case 2:
			res=GASCONSTANT*T*(GP+1.0/Prel);
			break;
		case 3:
			res=1e3/(rhoRel*RHOCRIT);
			break;
		case 4:
			ASSERT(0,"don't ask for properties at the sat line directly");
			break;
		default:
			ASSERT(0,"invalid region");
			break;
		}
		return res*1e-3;
	}

	double energy() { //kJ/kg
		double res;
		switch (region) {
		case 1:
			res=GASCONSTANT*T*(GT*Trel-GP*Prel);
			break;
		case 5:
		case 2:
			res=GASCONSTANT*T*((GT+G0T)*Trel-GP*Prel-1.0);
			break;
		case 3:
			res=GASCONSTANT*T*Trel*FT;
			break;
		case 4:
			ASSERT(0,"don't ask for properties at the sat line directly");
			break;
		default:
			ASSERT(0,"invalid region");
			break;
		}
		return res;
	}

	double entropy() { //kJ/kg/K
		double res;
		switch (region) {
		case 1:
			res=GASCONSTANT*(Trel*GT-G);
			break;
		case 5:
		case 2:
			res=GASCONSTANT*(Trel*(GT+G0T)-G-G0);
			break;
		case 3:
			res=GASCONSTANT*(Trel*FT-F);
			break;
		case 4:
			ASSERT(0,"don't ask for properties at the sat line directly");
			break;
		default:
			ASSERT(0,"invalid region");
			break;
		}
		return res;
	}

	double enthalpy() { //kJ/kg/K
		double res;
		switch (region) {
		case 1:
			res=GASCONSTANT*T*Trel*GT;
			break;
		case 5:
		case 2:
			res=GASCONSTANT*T*Trel*(GT+G0T);
			break;
		case 3:
			res=GASCONSTANT*T*(Trel*FT+rhoRel*FR);
			break;
		case 4:
			ASSERT(0,"don't ask for properties at the sat line directly");
			break;
		default:
			ASSERT(0,"invalid region");
			break;
		}
		return res;
	}

	double cP() { //kJ/kg/K
		double res,d1;
		switch (region) {
		case 1:
			res=-GASCONSTANT*Trel*Trel*GTT;
			break;
		case 5:
		case 2:
			res=-GASCONSTANT*Trel*Trel*(GTT+G0TT);
			break;
		case 3:
			d1=rhoRel*(FR-Trel*FTR);
			res=GASCONSTANT*(d1*d1/(2.0*rhoRel*FR+rhoRel*rhoRel*FRR)-Trel*Trel*FTT);
			break;
		case 4:
			ASSERT(0,"don't ask for properties at the sat line directly");
			break;
		default:
			ASSERT(0,"invalid region");
			break;
		}
		return res;
	}

	double cV() { //kJ/kg/K
		double res,d;
		switch (region) {
		case 1:
			d=GP-Trel*GTP;
			res=GASCONSTANT*((d*d)/GPP-Trel*Trel*GTT);
			break;
		case 5:
		case 2:
			d=1.0+Prel*GP-Trel*Prel*GTP;
			res=-GASCONSTANT*(Trel*Trel*(GTT+G0TT)+(d*d)/(1.0-Prel*Prel*GPP));
			break;
		case 3:
			res=-GASCONSTANT*Trel*Trel*FTT;
			break;
		case 4:
			ASSERT(0,"don't ask for properties at the sat line directly");
			break;
		default:
			ASSERT(0,"invalid region");
			break;
		}
		return res;
	}

	double speedOfSound() { //m/s
		double res;
		double d1,d2;
		switch (region) {
		case 1:
			d2=GP-Trel*GTP;
			res=GASCONSTANT*T*GP*GP/(d2*d2/(Trel*Trel*GTT)-GPP);
			break;
		case 5:
		case 2:
			d1=Prel*GP;
			d2=1.0+d1-Trel*Prel*GTP;
			res=GASCONSTANT*T*((1.0+(2.0+d1)*d1)/((1.0-Prel*Prel*GPP)+d2*d2/(Trel*Trel*(GTT+G0TT))));
			break;
		case 3:
			d1=rhoRel*(FR-Trel*FTR);
			res=GASCONSTANT*T*(2.0*rhoRel*FR+rhoRel*rhoRel*FRR-d1*d1/(Trel*Trel*FTT));
			break;
		case 4:
			ASSERT(0,"don't ask for properties at the sat line directly");
			break;
		default:
			ASSERT(0,"invalid region");
			break;
		}
		return sqrt(1e3*res);
	}

	//viscosity: Release on the IAPWS Formulation 2008 for the Viscosity of Ordinary Water Substance

private:

	static double viscSimple(double rho,double T) {
		double mu,d,d1;
		int i,j;
		double t=T/TCRIT;
		double rt=1.0/t-1.0;
		double r=rho/RHOCRIT;
		double rr=r-1.0;
		//dilute gas part
		d=0;
		for (i=0; i<4; i++) {
			d+=mu0h[i]/intpow(t,i);
		}
		mu=100.0*sqrt(t)/d;
		//finite density contribution
		d=0;
		for (i=0; i<6; i++) {
			d1=0;
			for (j=0; j<7; j++)
				if (mu1h[i][j]) {
					d1+=mu1h[i][j]*intpow(rr,j);
				}
			d+=d1*intpow(rt,i);
		}
		mu*=exp(r*d);
		return mu;
	}

public:

	double viscosity() {
		//Pa.s
		// mind: in accordance with industrial application recommendations,
		//  critical region correction is not applied
		return viscSimple(1.0/volume(),T)*1e-6;
	}

	//thermal conductivity
	// Revised Release on the IAPWS Formulation 1985 for the Thermal Conductivity of Ordinary Water Substance
	// appendix B

	double thermalConductivity() {
		const double Tref=647.26;
		const double rhoRef=317.7;
		double rho=1.0/volume();
		double r=rho/rhoRef;
		double t=T/Tref;
		double lambda=0;
		double d,d1,d2,Q,S;
		int i;
		//ideal gas limit
		for (i=0; i<4; i++) {
			lambda+=lambda0a[i]*intpow(t,i);
		}
		lambda*=sqrt(t);
		//add
		d=r+lambda0B[1];
		lambda+=lambda0b[0]+lambda0b[1]*r+lambda0b[2]*exp(lambda0B[0]*d*d);
		//add more
		d=fabs(t-1.0)+lambda0C[3];
		d1=pow(d,-0.6);
		S=(t>=1.0)?1.0/d:lambda0C[5]*d1;
		Q=2.0+lambda0C[4]*d1;
		d=(lambda0d[0]*pow(t,-10.0)+lambda0d[1])*pow(r,9.0/5.0)*exp(lambda0C[0]*(1.0-pow(r,14.0/5.0)));
		d1=lambda0d[2]*S*pow(r,Q)*exp(Q/(1.0+Q)*(1.0-pow(r,1.0+Q)));
		d2=lambda0d[3]*exp(lambda0C[1]*pow(t,1.5)+lambda0C[2]*pow(r,-5.0));
		lambda+=d+d1+d2;
		if (lambda<0) {
			lambda=0;    //wrong phase in request at T & P?
		}
		return lambda;
	}

	//surface tension
	//International Representation of the Surface Tension of Ordinary Water Substance 1994

	static double surfaceTension(double T) {
		//N/m
		double t=1.0-T/647.096;
		if (T<273.16) {
			throw WaterException("Temperature below lower limit of 273.16 K");
		}
		if (T>=TCRIT) {
			throw WaterException("Cannot calculate surface tension for supercritical conditions");
		}
		return 235.8e-3*pow(t,1.256)*(1.0-0.625*t);
	}

};

#endif //WATER_H
