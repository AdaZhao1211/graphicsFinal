// Ada Zhao
#include "CSCIx229.h"

/*
helper functions to help with calculations
*/


#define LEN 8192  //  Maximum length of text string

/*
calculate the normal for a, given b and c are on the same plane
*/
void calcNormal(double ax, double ay, double az, 
               double bx, double by, double bz, 
               double cx, double cy, double cz)
{
   //  Planar vector 0
   float dx0 = bx-ax;
   float dy0 = by-ay;
   float dz0 = bz-az;
   //  Planar vector 1
   float dx1 = cx-ax;
   float dy1 = cy-ay;
   float dz1 = cz-az;
   //  Normal = d1 * d0
   float Nx = dy1*dz0 - dy0*dz1;
   float Ny = dz1*dx0 - dz0*dx1;
   float Nz = dx1*dy0 - dx0*dy1;

   glNormal3f(Nx,Ny,Nz);
}


// get a coordinates with top left as the origin in pixel
// convert it into opengl bottom left coordinates in (0, 1)
void calcTextCord( double x, double y)
{
   glTexCoord2f(x/512,1-y/512);
}

void calcNormal2V(double X0, double Y0, double Z0, double X1, double Y1, double Z1){
   double X2 = Y0*Z1-Y1*Z0;
   double Y2 = Z0*X1-Z1*X0;
   double Z2 = X0*Y1-X1*Y0;
   glNormal3f(X2,Y2,Z2);


}



