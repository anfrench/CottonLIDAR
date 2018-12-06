#include <stdio.h>
#include <string.h>

#include <math.h>
#include <stdlib.h>
#include <time.h>

//forward decloration of functions

double deg2rad(double ldeg);
double rad2deg(double lrad);
double knots2mps(double knots); /* convert knots to meters per second */
void interpolrev(double *xvec1, double *yvec1, int numpts1, double *xvec2, double *yvec2, int numpts2, double yminleft, double ymaxright); /* interpolation function */
void latlon2utm(double *latdeg,double *londeg,short int zone, double *utme,double *utmn,double *kscale,long numpts);
double Mnaught(double mnoriginlat, double mneccentricity, double mnradiuseq);
double N(double nradiuseq,double neccentricity,double nlat);
double T(double tlat);
double C(double clat, double ceprimesq);
double A(double alat, double alon, double actrlon);
double M(double mlat, double meccentricity, double mradiuseq);
double K(double knaught,double eprimesq,double phi,double x,double N);
double X(double knaught, double radiuseq, double eccentricity, double lat, double lon, double ctrlon, double eprimesq) ;
double Y(double knaught, double lat, double eccentricity, double radiuseq, double originlat, double lon, double ctrlon, double eprimesq);


