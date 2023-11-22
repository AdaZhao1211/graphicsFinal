/*
 *  Lighting
 *
 *  Demonstrates basic lighting using a cube, sphere and icosahedron.
 *
 *  Key bindings:
 *  a/A        Decrease/increase ambient light
  d/D        Decrease/increase diffuse light
  s/S        Decrease/increase specular light
  e/E        Decrease/increase emitted light
  n/N        Decrease/increase shininess
  m          Toggles light movement
  []         Lower/rise light
  <>         Move light closer/further
  arrows     Change view angle
  0          Reset view angle
  ESC        Exit
  +/-        Change field of view of perspective
  F1         Toggle smooth/flat shading
 */
#include "CSCIx229.h"

int axes=1;       //  Display axes
int mode=1;       //  replace = 1, modulate = 0
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=3;     //  Size of world
// Light values
int light     =   1;  // Lighting
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int local     =   1;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  60;  // Ambient intensity (%)
int diffuse   =  50;  // Diffuse intensity (%)
int specular  =   10;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light

int ntex = 1; // texture switch
double t;

unsigned int myTexture;
int skyTexture;

/*
 *  Draw vertex in polar coordinates with normal, for Ball()
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

/*
 *  Convert hsv color to rgb
 */
void hsvToRgb(double hsv[3], GLfloat rgb[3]) {
    double c, x, m;
    double h = hsv[0], s = hsv[1], v = hsv[2];

    c = v * s;
    x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    m = v - c;

    if (h >= 0 && h < 60) {
        rgb[0] = c;
        rgb[1] = x;
        rgb[2] = 0;
    } else if (h >= 60 && h < 120) {
        rgb[0] = x;
        rgb[1] = c;
        rgb[2] = 0;
    } else if (h >= 120 && h < 180) {
        rgb[0] = 0;
        rgb[1] = c;
        rgb[2] = x;
    } else if (h >= 180 && h < 240) {
        rgb[0] = 0;
        rgb[1] = x;
        rgb[2] = c;
    } else if (h >= 240 && h < 300) {
        rgb[0] = x;
        rgb[1] = 0;
        rgb[2] = c;
    } else {
        rgb[0] = c;
        rgb[1] = 0;
        rgb[2] = x;
    }

    rgb[0] += m;
    rgb[1] += m;
    rgb[2] += m;
}

static void hotAirBalloon(double x, double y, double z, double r)
{
   // float black[]  = {0.0,0.0,0.0,1.0};
   float red[]  = {1.0,0.0,0.0,1.0};
   // float midred[]  = {1.0,0.4,0.4,1.0};
   float white[]  = {1.0,1.0,1.0,1.0};
   // float yellow[]  = {1.0,1.0,0.0,1.0};
   float Emission[] = {0.01*emission,0.004*emission,0.004*emission,1.0};

   // glColor4fv(red);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,red);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);

   //  Save transformation
   glPushMatrix();
   glTranslated(x,y,z);

   // the top part
   for (int th = 15; th<180; th+=5){
      glBegin(GL_QUAD_STRIP);

      for (int alpha = 0; alpha <= 360; alpha +=5){

         double hsvColor[3] = {alpha, 0.6, 0.5};
         GLfloat rgbColor[3];
         hsvToRgb(hsvColor, rgbColor);
         glColor3f(rgbColor[0], rgbColor[1], rgbColor[2]);
         float localR = 1+r* 1/cosh(2.75*(th*M_PI/180 + M_PI_2));
         float higherR = 1+r* 1/cosh(2.75*((th+5)*M_PI/180 + M_PI_2));
         float highestR = 1+r* 1/cosh(2.75*((th+10)*M_PI/180 + M_PI_2));
         GLdouble localX = localR*Sin(th)*Cos(alpha);
         GLdouble localY = -localR*Cos(th);
         GLdouble localZ = localR*Sin(th)*Sin(alpha);
         GLdouble higherX = higherR*Sin(th+5)*Cos(alpha);
         GLdouble higherY = -higherR*Cos(th+5);
         GLdouble higherZ = higherR*Sin(th+5)*Sin(alpha);
         GLdouble highestX = highestR*Sin(th+10)*Cos(alpha);
         GLdouble highestY = -highestR*Cos(th+10);
         GLdouble highestZ = highestR*Sin(th+10)*Sin(alpha);

         calcNormal2V(Sin(alpha), 0, -Cos(alpha), higherX-localX, higherY-localY, higherZ-localZ);
         glVertex3d(localX, localY, localZ);
         calcNormal2V(Sin(alpha), 0, -Cos(alpha), highestX-higherX, highestY-higherY, highestZ-higherZ);
         glVertex3d(higherX, higherY, higherZ);
      }
      glEnd();
   }
   float localR = 1+r* 1/cosh(2.75*(15*M_PI/180 + M_PI_2));
   GLdouble localY = -localR*Cos(15);
   glBegin(GL_TRIANGLE_FAN);
   glColor3f(1, 1, 1);
   glVertex3d(0, localY, 0);
   for (int alpha = 0; alpha <= 360; alpha +=5){
      double hsvColor[3] = {alpha, 0.6, 0.5};
      GLfloat rgbColor[3];
      hsvToRgb(hsvColor, rgbColor);
      glColor3f(rgbColor[0], rgbColor[1], rgbColor[2]);
      glVertex3d(localR*Sin(15)*Cos(alpha), localY, localR*Sin(15)*Sin(alpha));
   }
   glEnd();
   // 4 strings
   float stringD = localR*Sin(15)*0.8;
   float stringR = localR*Sin(15)*0.05;
   for (int i = 0; i < 4; i++){
      glBegin(GL_QUAD_STRIP);
      glColor3f(1, 1, 1);
      for (int alpha = 0; alpha <= 360; alpha +=45){
         glNormal3f(Cos(alpha), 0, Sin(alpha));
         glVertex3d(stringD*Cos(i*90) + stringR*Cos(alpha), localY, stringD*Sin(i*90) + stringR*Sin(alpha));
         glVertex3d(stringD*Cos(i*90) + stringR*Cos(alpha), localY*1.2, stringD*Sin(i*90) + stringR*Sin(alpha));
      }
      glEnd();
   }

   // basket
   glBegin(GL_QUAD_STRIP);
   for (int alpha = 0; alpha <= 360; alpha +=15){
      glColor3f(1, 0.5, 0.7);
      glNormal3f(Cos(alpha), 0, Sin(alpha));
      glVertex3d(localR*Sin(15)*Cos(alpha), localY*1.2, localR*Sin(15)*Sin(alpha));
      glVertex3d(localR*Sin(15)*Cos(alpha), localY*1.4, localR*Sin(15)*Sin(alpha));
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glColor3f(1, 1, 1);
   glNormal3f(0, -1, 0); glVertex3d(0, localY*1.4, 0);
   for (int alpha = 0; alpha <= 360; alpha +=5){
      glColor3f(1, 0.5, 0.7);
      glVertex3d(localR*Sin(15)*Cos(alpha), localY*1.4, localR*Sin(15)*Sin(alpha));
   }
   glEnd();

   glPopMatrix();
}

/* 
 *  skybox
 */
static void Sky(double D)
{
   //  Textured white box dimension (-D,+D)
   glPushMatrix();
   glScaled(D,D,D);
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,skyTexture);
   glBegin(GL_QUADS);
   glTexCoord2f(0.00,0.34); glVertex3f(-1,-1,-1);
   glTexCoord2f(0.25,0.34); glVertex3f(+1,-1,-1);
   glTexCoord2f(0.25,0.666); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.00,0.666); glVertex3f(-1,+1,-1);

   glTexCoord2f(0.25,0.34); glVertex3f(+1,-1,-1);
   glTexCoord2f(0.50,0.34); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.50,0.666); glVertex3f(+1,+1,+1);
   glTexCoord2f(0.25,0.666); glVertex3f(+1,+1,-1);

   glTexCoord2f(0.50,0.34); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.75,0.34); glVertex3f(-1,-1,+1);
   glTexCoord2f(0.75,0.666); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.50,0.666); glVertex3f(+1,+1,+1);

   glTexCoord2f(0.75,0.34); glVertex3f(-1,-1,+1);
   glTexCoord2f(1.00,0.34); glVertex3f(-1,-1,-1);
   glTexCoord2f(1.00,0.666); glVertex3f(-1,+1,-1);
   glTexCoord2f(0.75,0.666); glVertex3f(-1,+1,+1);


   //  Top and bottom
   glTexCoord2f(0.25,0.667); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.5,0.667); glVertex3f(+1,+1,+1);
   glTexCoord2f(0.5,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.25,1); glVertex3f(-1,+1,-1);

   glTexCoord2f(0.25,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(0.5,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.5,0.33); glVertex3f(+1,-1,-1);
   glTexCoord2f(0.25,0.33); glVertex3f(-1,-1,-1);
   glEnd();

   //  Undo
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}


static void water(double x,double y,double z,double s){

   double step = s/40;
   for(double i = x-s/2; i < x+s/2 ; i+=step){
      glBegin(GL_QUAD_STRIP);
      for(double j = z-s/2; j <= z+s/2 ; j+=step){
         float perlinY = pnoise3d(i*0.2, j*0.2, t, 0.1, 5, 12124);
         glVertex3d(i, y+perlinY, j);
         float perlinY1 = pnoise3d((i+step)*0.2, j*0.2, t, 0.1, 5, 12124);
         glVertex3d(i+step, y+perlinY1, j);
      }
      glEnd();
   }

}




/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball with yellow specular
   float yellow[]   = {1.0,1.0,0.0,1.0};
   float Emission[] = {0.0,0.0,0.01*emission,1.0};
   glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (int ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}


/*
 *  Draw a car
 *     at (x,y,z)
 *     with w in width, l in length and h in height
 *     rotated th about the y axis
 */

void drawCar(double x,double y,double z,
            double w,double l,double h,
            double th)
{
   // float black[]  = {0.0,0.0,0.0,1.0};
   float red[]  = {1.0,0.0,0.0,1.0};
   // float midred[]  = {1.0,0.4,0.4,1.0};
   float white[]  = {1.0,1.0,1.0,1.0};
   // float yellow[]  = {1.0,1.0,0.0,1.0};
   float Emission[] = {0.01*emission,0.004*emission,0.004*emission,1.0};

   // glColor4fv(red);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,red);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);

   //  Draw the Car
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotatef(th,0,1,0);

   glBegin(GL_QUADS);

   // wrap around the car
   glColor3f(1, 0, 0);
   calcNormal(-l/2, -h/2, w/2, -l/2, -h/2, -w/2, -5*l/12, 0, w/2);
   calcTextCord(260, 117); glVertex3f(-l/2, -h/2, -w/2);
   calcTextCord(501, 117); glVertex3f(-l/2, -h/2, w/2);
   calcTextCord(491, 54); glVertex3f(-5*l/12, 0, w/2);
   calcTextCord(270, 54); glVertex3f(-5*l/12, 0, -w/2);

   calcNormal(-5*l/12, 0, w/2, -5*l/12, 0, -w/2, -l/4, 0, w/2);
   calcTextCord(297, 351); glVertex3f(-5*l/12, 0, -w/2);
   calcTextCord(297, 234); glVertex3f(-5*l/12, 0, w/2);
   calcTextCord(244, 234); glVertex3f(-l/4, 0, w/2);
   calcTextCord(244, 351); glVertex3f(-l/4, 0, -w/2);

   // （glass color）
   glColor3f(1, 1, 1);
   calcNormal(-l/4, 0, w/2, -l/4, 0, -w/2, -l/6, h/2, w/2);
   calcTextCord(323, 44); glVertex3f(-l/4, 0, -w/2);
   calcTextCord(439, 44); glVertex3f(-l/4, 0, w/2);
   calcTextCord(439, 13); glVertex3f(-l/6, h/2, w/2);
   calcTextCord(323, 13); glVertex3f(-l/6, h/2, -w/2);

   glColor3f(1, 0, 0);
   calcNormal(-l/6, h/2, w/2, -l/6, h/2, -w/2, l/6, h/2, w/2);
   calcTextCord(176, 323); glVertex3f(-l/6, h/2, -w/2);
   calcTextCord(176, 256); glVertex3f(-l/6, h/2, w/2);
   calcTextCord(114, 256); glVertex3f(l/6, h/2, w/2);
   calcTextCord(114, 323); glVertex3f(l/6, h/2, -w/2);

   calcNormal(l/6, h/2, w/2, l/6, h/2, -w/2, l/4, 0, w/2);
   calcTextCord(178, 15); glVertex3f(l/6, h/2, -w/2);
   calcTextCord(69, 15); glVertex3f(l/6, h/2, w/2);
   calcTextCord(69, 31); glVertex3f(l/4, 0, w/2);
   calcTextCord(178, 31); glVertex3f(l/4, 0, -w/2);


   calcNormal(l/4, 0, w/2, l/4, 0, -w/2, 5*l/12, 0, w/2);
   calcTextCord(41, 323); glVertex3f(l/4, 0, -w/2);
   calcTextCord(41, 252); glVertex3f(l/4, 0, w/2);
   calcTextCord(22, 252); glVertex3f(5*l/12, 0, w/2);
   calcTextCord(22, 323); glVertex3f(5*l/12, 0, -w/2);

   calcNormal(5*l/12, 0, w/2, 5*l/12, 0, -w/2, l/2, -h/2, w/2);
   calcTextCord(230, 46); glVertex3f(5*l/12, 0, -w/2);
   calcTextCord(14, 46); glVertex3f(5*l/12, 0, w/2);
   calcTextCord(14, 119); glVertex3f(l/2, -h/2, w/2);
   calcTextCord(230, 119); glVertex3f(l/2, -h/2, -w/2);

   calcNormal(l/2, -h/2, w/2, l/2, -h/2, -w/2, -l/2, -h/2, w/2);
   calcTextCord(448, 149); glVertex3f(l/2, -h/2, -w/2);
   calcTextCord(326, 149); glVertex3f(l/2, -h/2, w/2);
   calcTextCord(326, 404); glVertex3f(-l/2, -h/2, w/2);
   calcTextCord(448, 404); glVertex3f(-l/2, -h/2, -w/2);
   glEnd();


   // side
   float tempW = w/2;
   for (int i=0; i<2;i++){

      glNormal3f(0, 0, tempW);

      glBegin(GL_QUAD_STRIP);
      glColor3f(1, 0.4, 0.4);

      calcTextCord(357, 456); glVertex3f(-l/2, -h/2, tempW);
      calcTextCord(357, 424); glVertex3f(-5*l/12, 0, tempW);
      
      calcTextCord(267, 456); glVertex3f(-l/4, -h/2, tempW);
      calcTextCord(267, 409); glVertex3f(-l/4, 0, tempW);

      calcTextCord(210, 456); glVertex3f(-l/6, -h/2, tempW);
      calcTextCord(210, 383); glVertex3f(-l/6, h/2, tempW);

      calcTextCord(131, 456); glVertex3f(l/6, -h/2, tempW);
      calcTextCord(131, 383); glVertex3f(l/6, h/2, tempW);

      calcTextCord(85, 456); glVertex3f(l/4, -h/2, tempW);
      calcTextCord(85, 398); glVertex3f(l/4, 0, tempW);

      calcTextCord(3, 456); glVertex3f(l/2, -h/2, tempW);
      calcTextCord(18, 396); glVertex3f(5*l/12, 0, tempW);
      
      glEnd();

      // draw the other side
      tempW *= -1;
   }
   //  wheels
   glColor3f(1,1,0);
   // the x and z position of the side face of all wheels
   float const wheelCenter [8][2] = 
   {
    {l/3, 7*w/12},
    {l/3, 5*w/12},
    {l/3, -5*w/12},
    {l/3, -7*w/12},
    {-l/3, 7*w/12},
    {-l/3, 5*w/12},
    {-l/3, -5*w/12},
    {-l/3, -7*w/12}
   };
   float r = 3*l/24;
   // wheel hub
   for (int i = 0; i < 8; i++){
      glBegin(GL_TRIANGLE_FAN);
      glNormal3f(0, 0, 1-i%2*2);
      calcTextCord(292, 459); glVertex3d(wheelCenter[i][0], -h/2, wheelCenter[i][1]);
      for (int th=0;th<=360;th+=45){
         calcTextCord(292 + 28*Cos(th), 459+ 28*Sin(th)); 
         glVertex3d(wheelCenter[i][0]+Cos(th)*r, -h/2+Sin(th)*r,wheelCenter[i][1]);
      }
      glEnd();
   }
   
   // tire
   for (int i = 0; i < 8; i+=2){
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=45){
         glNormal3f(Cos(th), Sin(th), 0);
         calcTextCord(512, 125+48*th/45);
         glVertex3d(wheelCenter[i][0]+Cos(th)*r, -h/2+Sin(th)*r,wheelCenter[i][1]);
         calcTextCord(470, 125+48*th/45);
         glVertex3d(wheelCenter[i+1][0]+Cos(th)*r, -h/2+Sin(th)*r,wheelCenter[i+1][1]);
      }
      glNormal3f(1, 0, 0);
      glVertex3d(wheelCenter[i][0]+Cos(0)*r, -h/2+Sin(0)*r,wheelCenter[i][1]);
      glVertex3d(wheelCenter[i+1][0]+Cos(0)*r, -h/2+Sin(0)*r,wheelCenter[i+1][1]);
      glEnd();
   }
   //  Undo transformations
   glPopMatrix();

}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   double Ex = -2*dim*Sin(th)*Cos(ph);
   double Ey = +2*dim        *Sin(ph);
   double Ez = +2*dim*Cos(th)*Cos(ph);
   gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);



   //  Flat or smooth shading
   glShadeModel(GL_SMOOTH);

   // 4 parameters. homogeneous
   //  Translate intensity to color vectors
   float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
   float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
   float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
   //  Light position
   float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
   //  Draw light position as ball (still no lighting here)
   glColor3f(1,1,1);
   ball(Position[0],Position[1],Position[2] , 0.1);

   //  OpenGL should normalize normal vectors
   glEnable(GL_NORMALIZE);
   //  Enable lighting
   glEnable(GL_LIGHTING);
   //  Location of viewer for specular calculations
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
   //  glColor sets ambient and diffuse color materials
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);
   //  Enable light 0
   glEnable(GL_LIGHT0);
   //  Set ambient, diffuse, specular components and position of light 0
   glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
   glLightfv(GL_LIGHT0,GL_POSITION,Position);

   if (ntex==0)
      glDisable(GL_TEXTURE_2D);
   else
   {
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , mode?GL_REPLACE:GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D,myTexture);
   }

   //  Draw individual objects
   drawCar(0,-4,0, 1, 2, 1, 0);
   Sky(3.5*dim);

   glDisable(GL_TEXTURE_2D);

   ball(-4,0,0 , 0.5);
   hotAirBalloon(0,2,0,40);
   // water(0,0,0,10);
   
   

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glDisable(GL_TEXTURE_2D);

   glColor3f(1,1,1);
   if (axes)
   {
      const double len=2.0;  //  Length of axes
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }

   //  Display parameters
   glWindowPos2i(5,45);
   Print("Texture(T)=%s Mode(M)= %s",ntex?"On":"Off", mode?"Replace":"Modulate", distance,ylight);
   glWindowPos2i(5,25);
   Print("Ambient(A)=%d  Diffuse(D)=%d Specular(S)=%d Emission(E)=%d Shininess(N)=%.0f",ambient,diffuse,specular,emission,shiny);

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 2;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 2;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph -= 2;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph += 2;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(fov,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0; 
   //  Toggle light movement
   else if (ch == 'f' || ch == 'F')
      move = 1-move;
   // change modulate / replace mode of texture
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  turn on / off texture
   else if (ch == 't' || ch == 'T')
      ntex = 1 - ntex;
   else if (ch == '<')
      distance += 1;
   else if (ch == '>'){
      distance -= 1;
   }
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(fov,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, RES*width,RES*height);
   //  Set projection
   Project(fov,asp,dim);
}


/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,400);
   glutCreateWindow("Ada Zhao Final Project");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   myTexture = LoadTexBMP("Car.bmp");
   skyTexture = LoadTexBMP("daylight.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
