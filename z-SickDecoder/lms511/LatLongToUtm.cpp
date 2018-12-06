#pragma once
#include "LatLongToUtm.h"
//forward decloration of functions

double knots2mps(double knots) {
	/* convert knots to meters per second */
	double speed;
	speed = knots*1852.0/3600.0;
	//speed = knots*0.5144444 ; /* 1.852 km/hr is 1 knot exactly */
	return(speed);
}
void interpolrev(double *xvec1, double *yvec1, int numpts1,
	         double *xvec2, double *yvec2, int numpts2,
                 double yminleft, double ymaxright)
{
  /* interpolate yvec1 values to yvec2 values located at xvec2 */
  /* assume xvec1 and xvec2 monotonic increasing */
  /* dont assume equal spacing of interpolates */
  /* set yvec2 values to yleft where xvec2 values are less than minimum xvec1 */
  /* set yvec2 values to yright where xvec2 values are greater than maximum xvec1 */


  long targrct;
  double cursrcx,cursrcy;
  double curtargx,curtargy;
  double minsrcx,maxsrcx;
  double xleft,xright;
  double yleft,yright;
  double delsrcx,delsrcy;
  double slope,intercept;
  long srcindex=0;

  minsrcx = xvec1[0];
  maxsrcx = xvec1[numpts1-1];
  
  
  for(targrct=0;targrct<numpts2;targrct++) {
    curtargx = xvec2[targrct];
    cursrcx = xvec1[srcindex];
    cursrcy = yvec1[srcindex];

    if(curtargx < minsrcx) {
      yvec2[targrct] = yminleft;
    } else {
      if(curtargx > maxsrcx) {
	yvec2[targrct] = ymaxright;
      } else {
	/* target x location within limits, so locate bounding source points */
	xleft = cursrcx;
	xright = xvec1[srcindex+1];
	yleft = yvec1[srcindex];
	yright = yvec1[srcindex+1];
	while(xright <= curtargx) {
	  srcindex++;
		  
	  if(srcindex < (numpts1-1)) {
	    xleft = xvec1[srcindex];
	    xright = xvec1[srcindex+1];
	    yleft = yvec1[srcindex];
	    yright = yvec1[srcindex+1];
	  } else {
	    xright = 1.000000001*curtargx;
	    yright = yvec1[numpts1-1];
	  } /* end of else */
	} /* end of while loop */
	delsrcx = xright-xleft;
	if(delsrcx <= 0.0) {
	  printf("error, delsrcx is zero or less than zero!\n");
	  exit(EXIT_FAILURE);
	}
	delsrcy = yright-yleft;
	slope = delsrcy/delsrcx;
	intercept = yleft - (slope*xleft);
	yvec2[targrct] = slope*curtargx + intercept;
      } /* end of inner else */
    } /* end of outer else */
    
  } /* end of targrct loop */

} /* end of interpolrev function */

/*convert latitude longitude values to utm */
/* must include math.h library */

/* By Andrew N. French 20 June 2003 */

#define PI 3.14159265358979

void latlon2utm(double *latdeg,double *londeg,short int zone,
		double *utme,double *utmn,double *kscale,long numpts) {

#if 0
printf("entering latlon2utm routine.\n");
printf("latdeg:%f londeg:%f zone:%d\n",latdeg[0],londeg[0],zone);
exit(1);
#endif

double llat;
double llon; 

long pct;
double Nval;
double eccentricity,radiuseq,knaught;
    
    double radiuspl,invflat,flat;
    double eprimesq;
    double originlatdeg,originlat;
    double lat,lon;
    double ctrlondeg,ctrlon;
   
    double xutm,yutm;
    
    radiuseq = 6378137.0;
    radiuspl = 6356752.3;
    invflat = 298.257;
    flat = 1/invflat;
    knaught = 0.9996;
    eccentricity = sqrt(1.0-((radiuspl*radiuspl)/(radiuseq*radiuseq)));
    eprimesq = (eccentricity*eccentricity)/(1.0-(eccentricity*eccentricity));
   
    originlatdeg = 0.0;
//    originlat    = (PI/180.0)*originlatdeg;
    originlat = deg2rad(originlatdeg);

    /*    ctrlondeg    = 177.0+(6.0* (double)zone);*/
    ctrlondeg = (6.0* (double)zone)-183.0;

    if(ctrlondeg>360.0) {
      ctrlondeg = ctrlondeg-360.0;
    }
    if(ctrlondeg<0.0) {
	   ctrlondeg = ctrlondeg+360.0;
    } 
    
  //  ctrlon = (PI/180.0)*ctrlondeg;
     ctrlon = deg2rad(ctrlondeg);

        printf("ctrlondeg: %lf\n",ctrlondeg);
    
    
    for(pct=0;pct<numpts;pct++) {

      if(londeg[pct] < 0.0) {
//	llon = (360.0+londeg[pct])*PI/180.0;
	llon = deg2rad(360.0+londeg[pct]);
      } else {
//	      llon = londeg[pct]*PI/180.0;
	      llon = deg2rad(londeg[pct]);
      }

//    llat = latdeg[pct]*PI/180.0;
      llat = deg2rad(latdeg[pct]);
#if 0
    printf("current lat lon values:\n");
    printf("%lf %lf\n",latdeg[pct],londeg[pct]);
    printf("llat:%f llon:%f\n",llat,llon);
#endif
    
    
    xutm =
      X(knaught,radiuseq,eccentricity,llat,llon,ctrlon,eprimesq);
    yutm = 
      Y(knaught,llat,eccentricity,radiuseq,originlat,llon,ctrlon,eprimesq);
#if 0
    printf("current xutm:%f yutm:%f\n",xutm,yutm);
    exit(1);
#endif

    utme[pct] = xutm+500000.0;
    utmn[pct] = yutm;
    Nval = N(radiuseq,eccentricity,llat);
    kscale[pct] = K(knaught,eprimesq,llat,xutm,Nval);
   

    } /*end of for loop */


} /*end of main function */

/************************************
 *                                  */
double N(double nradiuseq,double neccentricity,double nlat) {
  double term1,term2,n;
  
  term1 = neccentricity*neccentricity;
  term2 = sin(nlat)*sin(nlat);
  
  n = nradiuseq/(sqrt(1.0-(term1*term2)));
  return (n);
}

double T(double tlat) {
  double t;
  t= tan(tlat)*tan(tlat);

  return (t);
}

double C(double clat, double ceprimesq) {
  double c;
  c = ceprimesq*cos(clat)*cos(clat);
  return (c);
}

double A(double alat, double alon, double actrlon) {
  double a;
  a = cos(alat)*(alon-actrlon);
  return (a);
}

double M(double mlat, double meccentricity, double mradiuseq) {
  double m,m1,m2,m3,m4;
  m1 = 1.0-(pow(meccentricity,2.0)/4.0)-(3.0*pow(meccentricity,4.0)/64.0)-(5.0*pow(meccentricity,6.0)/256.0);
  m2 = (3.0*pow(meccentricity,2.0)/8.0)+(3.0*pow(meccentricity,4.0)/32.0)+(45.0*pow(meccentricity,6.0)/1024.0);
  m3 = (15.0*pow(meccentricity,4.0)/256.0)+(45.0*pow(meccentricity,6.0)/1024.0);
  m4 = (35.0*pow(meccentricity,6.0)/3072.0);
  m = mradiuseq*((m1*mlat)-(m2*sin(2.0*mlat))+(m3*sin(4.0*mlat))-(m4*sin(6.0*mlat)));
  return (m);
}

double Mnaught(double mnoriginlat, double mneccentricity, double mnradiuseq) {
  double mn,m1,m2,m3,m4;
  m1 = 1.0-(pow(mneccentricity,2.0)/4.0)-(3.0*pow(mneccentricity,4.0)/64.0)-(5.0*pow(mneccentricity,6.0)/256.0);
  m2 = (3.0*pow(mneccentricity,2.0)/8.0)+(3.0*pow(mneccentricity,4.0)/32.0)+(45.0*pow(mneccentricity,6.0)/1024.0);
  m3 = (15.0*pow(mneccentricity,4.0)/256.0)+(45.0*pow(mneccentricity,6.0)/1024.0);
  m4 = (35.0*pow(mneccentricity,6.0)/3072.0);
  mn = mnradiuseq*((m1*mnoriginlat)-(m2*sin(2.0*mnoriginlat))+(m3*sin(4.0*mnoriginlat))-(m4*sin(6.0*mnoriginlat)));
  return (mn);
}

double X(double knaught, double radiuseq, double eccentricity, double lat, double lon, double ctrlon, double eprimesq) {
  double x;
  x = knaught*N(radiuseq,eccentricity,lat)*(A(lat,lon,ctrlon)+(1-T(lat)+C(lat,eprimesq))*pow(A(lat,lon,ctrlon),3.0)/6.0+(5.0-18.0*T(lat)+pow(T(lat),2.0)+72.0*C(lat,eprimesq)-58.0*eprimesq)*pow(A(lat,lon,ctrlon),5.0)/120.0);

#if 0
  printf("N:%f\n",N(radiuseq,eccentricity,lat));
  printf("A:%f\n",A(lat,lon,ctrlon));
  printf("T:%f\n",T(lat));
  printf("C:%f\n",C(lat,eprimesq));
  printf("eprimesq:%f\n",eprimesq);
  printf("eccentricity:%f\n",eccentricity);
  printf("ctrlon:%f\n",ctrlon);
  printf("x:%f\n",x);
  exit(1);
#endif
  return (x);
}

double Y(double knaught, double lat, double eccentricity, double radiuseq, double originlat, double lon, double ctrlon, double eprimesq) {
  double y;
  y = knaught*(M(lat,eccentricity,radiuseq)-Mnaught(originlat,eccentricity,radiuseq)+N(radiuseq,eccentricity,lat)*tan(lat)*((pow(A(lat,lon,ctrlon),2.0)/2.0)+(5.0-T(lat)+9.0*C(lat,eprimesq)+4.0*pow(C(lat,eprimesq),2.0))*(pow(A(lat,lon,ctrlon),4.0)/24.0)+(61.0-58.0*T(lat)+pow(T(lat),2.0)+600.0*C(lat,eprimesq)-330.0*eprimesq)*pow(A(lat,lon,ctrlon),6.0)/720.0));
return (y);
}

double K(double knaught,double eprimesq,double phi,double x,double N)
{
  double k;

  k=knaught*(1.0+(((1.0+eprimesq*cos(phi)*cos(phi))*x*x)/(2.0*knaught*knaught*N*N)));
  return(k);
}

double deg2rad(double ldeg) {
	/* convert degrees to radians */
	double lrad;
	lrad = ldeg*PI/180.0;
	return(lrad);
}

double rad2deg(double lrad) {
	/* convert radians to degrees */
	double ldeg;
	ldeg = lrad*180.0/PI;
	return(ldeg);
}


//} /*end of main function */
#if 0

char ID[NUMCHARSGPS];
	char timestr[10];
	char validity[2];
	char latstr[14];
	char Nstr[2];
	char lonstr[15];
	char Wstr[2];
	double SpeedKts;
	double Heading;
	char ddmmyystr[7];
	double MagVariation;
	/* usually would have char Estr[2] here, but input is non-standard */
	char checksum[10];
	int SysYear;
	int SysMonth;
	int SysDay;
	int SysHour;
	int SysMinute;
	int SysSeconds;
	int SysmSeconds;

typedef struct{
	char ID[NUMCHARSGPS];
	char timestr[10];
	char latstr[14];
	char Nstr[2];
	char lonstr[15];
	char Wstr[2];
	int FixQuality;
	int NumSat;
	float HDOP;
	double Altm;
	char Mstr[2];
	double HgtGeoid;
	char M2str[2];
	double UpdateTime;
	char checksum[8];
	int SysYear;
	int SysMonth;
	int SysDay;
	int SysHour;
	int SysMinute;
	int SysSeconds;
	int SysmSeconds;
} GGAstruc;
#endif
