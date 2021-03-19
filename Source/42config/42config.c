/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */


#include "42.h"
#include "42config/42config.h"
#include <ctype.h>
#include <string.h>

/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

/**********************************************************************/
long DecodeString(char *s)
{
      unsigned long i;

      for(i=0;i<strlen(s);i++) s[i] = toupper(s[i]);

      if (!strcmp(s,"FALSE")) return FALSE;
      else if (!strcmp(s,"TRUE")) return TRUE;
      else if (!strcmp(s,"OFF")) return OFF;
      else if (!strcmp(s,"ON")) return ON;
      else if (!strcmp(s,"FAST")) return FAST_TIME;
      else if (!strcmp(s,"REAL")) return REAL_TIME;
      else if (!strcmp(s,"EXTERNAL")) return EXTERNAL_TIME;
      else if (!strcmp(s,"NOS3")) return NOS3_TIME;
      else if (!strcmp(s,"SOL")) return SOL;
      else if (!strcmp(s,"MERCURY")) return MERCURY;
      else if (!strcmp(s,"VENUS")) return VENUS;
      else if (!strcmp(s,"EARTH")) return EARTH;
      else if (!strcmp(s,"MARS")) return MARS;
      else if (!strcmp(s,"JUPITER")) return JUPITER;
      else if (!strcmp(s,"SATURN")) return SATURN;
      else if (!strcmp(s,"URANUS")) return URANUS;
      else if (!strcmp(s,"NEPTUNE")) return NEPTUNE;
      else if (!strcmp(s,"PLUTO")) return PLUTO;
      else if (!strcmp(s,"LUNA")) return LUNA;
      else if (!strcmp(s,"KEP")) return INP_KEPLER;
      else if (!strcmp(s,"RV")) return INP_POSVEL;
      else if (!strcmp(s,"FILE")) return INP_FILE;
      else if (!strcmp(s,"TLE")) return INP_TLE;
      else if (!strcmp(s,"TRV")) return INP_TRV;
      else if (!strcmp(s,"PA")) return TRUE;
      else if (!strcmp(s,"AE")) return FALSE;
      else if (!strcmp(s,"POSW")) return TRUE;
      else if (!strcmp(s,"LLA")) return FALSE;
      else if (!strcmp(s,"POS")) return POSITIVE;
      else if (!strcmp(s,"NEG")) return NEGATIVE;
      else if (!strcmp(s,"CM")) return TRUE;
      else if (!strcmp(s,"ORIGIN")) return FALSE;
      else if (!strcmp(s,"CENTRAL")) return ORB_CENTRAL;
      else if (!strcmp(s,"THREE_BODY")) return ORB_THREE_BODY;
      else if (!strcmp(s,"ZERO")) return ORB_ZERO;
      else if (!strcmp(s,"FLIGHT")) return ORB_FLIGHT;
      else if (!strcmp(s,"EARTHMOON")) return EARTHMOON;
      else if (!strcmp(s,"SUNEARTH")) return SUNEARTH;
      else if (!strcmp(s,"SUNJUPITER")) return SUNJUPITER;
      else if (!strcmp(s,"MODES")) return INP_MODES;
      else if (!strcmp(s,"XYZ")) return INP_XYZ;
      else if (!strcmp(s,"SPLINE")) return INP_SPLINE;
      else if (!strcmp(s,"L1")) return LAGPT_L1;
      else if (!strcmp(s,"L2")) return LAGPT_L2;
      else if (!strcmp(s,"L3")) return LAGPT_L3;
      else if (!strcmp(s,"L4")) return LAGPT_L4;
      else if (!strcmp(s,"L5")) return LAGPT_L5;
      else if (!strcmp(s,"CW")) return DIR_CW;
      else if (!strcmp(s,"CCW")) return DIR_CCW;
      else if (!strcmp(s,"NONE")) return NONE;
      else if (!strcmp(s,"DIPOLE")) return DIPOLE;
      else if (!strcmp(s,"IGRF")) return IGRF;
      else if (!strcmp(s,"SPHERICAL")) return 1;
      else if (!strcmp(s,"GIMBAL")) return 0;
      else if (!strcmp(s,"X_AXIS")) return X_AXIS;
      else if (!strcmp(s,"Y_AXIS")) return Y_AXIS;
      else if (!strcmp(s,"Z_AXIS")) return Z_AXIS;
      else if (!strcmp(s,"POS_X")) return POS_X;
      else if (!strcmp(s,"POS_Y")) return POS_Y;
      else if (!strcmp(s,"POS_Z")) return POS_Z;
      else if (!strcmp(s,"NEG_X")) return NEG_X;
      else if (!strcmp(s,"NEG_Y")) return NEG_Y;
      else if (!strcmp(s,"NEG_Z")) return NEG_Z;
      else if (!strcmp(s,"REFORB")) return TARGET_REFORB;
      else if (!strcmp(s,"FRM")) return TARGET_FRM;
      else if (!strcmp(s,"SC")) return TARGET_SC;
      else if (!strcmp(s,"BODY")) return TARGET_BODY;

      else if (!strcmp(s,"TRACK_HOST")) return TRACK_HOST;
      else if (!strcmp(s,"TRACK_TARGET")) return TRACK_TARGET;
      else if (!strcmp(s,"FIXED_IN_HOST")) return FIXED_IN_HOST;

      else if (!strcmp(s,"DOWN")) return VIEW_DOWN;
      else if (!strcmp(s,"REAR_LEFT")) return VIEW_REAR_LEFT;
      else if (!strcmp(s,"REAR")) return VIEW_REAR;
      else if (!strcmp(s,"REAR_RIGHT")) return VIEW_REAR_RIGHT;
      else if (!strcmp(s,"LEFT")) return VIEW_LEFT;
      else if (!strcmp(s,"UP")) return VIEW_UP;
      else if (!strcmp(s,"RIGHT")) return VIEW_RIGHT;
      else if (!strcmp(s,"FRONT_LEFT")) return VIEW_FRONT_LEFT;
      else if (!strcmp(s,"FRONT")) return VIEW_FRONT;
      else if (!strcmp(s,"FRONT_RIGHT")) return VIEW_FRONT_RIGHT;

      else if (!strcmp(s,"WIREFRAME")) return FOV_WIREFRAME;
      else if (!strcmp(s,"SOLID")) return FOV_SOLID;
      else if (!strcmp(s,"VECTOR")) return FOV_VECTOR;
      else if (!strcmp(s,"PLANE")) return FOV_PLANE;

      else if (!strcmp(s,"REFPT_CM")) return REFPT_CM;
      else if (!strcmp(s,"REFPT_JOINT")) return REFPT_JOINT;

      else if (!strcmp(s,"LAGDOF_MODES")) return LAGDOF_MODES;
      else if (!strcmp(s,"LAGDOF_COWELL")) return LAGDOF_COWELL;
      else if (!strcmp(s,"LAGDOF_SPLINE")) return LAGDOF_SPLINE;

      else if (!strcmp(s,"STEADY")) return ROTDOF_STEADY;
      else if (!strcmp(s,"KIN_JOINT")) return ROTDOF_KIN_JOINT;
      else if (!strcmp(s,"DYN_JOINT")) return ROTDOF_DYN_JOINT;

      else if (!strcmp(s,"FIXED")) return ORBDOF_FIXED;
      else if (!strcmp(s,"EULER_HILL")) return ORBDOF_EULER_HILL;
      else if (!strcmp(s,"ENCKE")) return ORBDOF_ENCKE;
      else if (!strcmp(s,"COWELL")) return ORBDOF_COWELL;

      else if (!strcmp(s,"PASSIVE_FSW")) return PASSIVE_FSW;
      else if (!strcmp(s,"PROTOTYPE_FSW")) return PROTOTYPE_FSW;
      else if (!strcmp(s,"AD_HOC_FSW")) return AD_HOC_FSW;
      else if (!strcmp(s,"SPINNER_FSW")) return SPINNER_FSW;
      else if (!strcmp(s,"MOMBIAS_FSW")) return MOMBIAS_FSW;
      else if (!strcmp(s,"THREE_AXIS_FSW")) return THREE_AXIS_FSW;
      else if (!strcmp(s,"ISS_FSW")) return ISS_FSW;
      else if (!strcmp(s,"CMG_FSW")) return CMG_FSW;
      else if (!strcmp(s,"THR_FSW")) return THR_FSW;
      else if (!strcmp(s,"CFS_FSW")) return CFS_FSW;

      else if (!strcmp(s,"PHOBOS")) return PHOBOS;
      else if (!strcmp(s,"DEIMOS")) return DEIMOS;
      else if (!strcmp(s,"IO")) return IO;
      else if (!strcmp(s,"EUROPA")) return EUROPA;
      else if (!strcmp(s,"GANYMEDE")) return GANYMEDE;
      else if (!strcmp(s,"CALLISTO")) return CALLISTO;
      else if (!strcmp(s,"AMALTHEA")) return AMALTHEA;
      else if (!strcmp(s,"HIMALIA")) return HIMALIA;
      else if (!strcmp(s,"ELARA")) return ELARA;
      else if (!strcmp(s,"PASIPHAE")) return PASIPHAE;
      else if (!strcmp(s,"SINOPE")) return SINOPE;
      else if (!strcmp(s,"LYSITHEA")) return LYSITHEA;
      else if (!strcmp(s,"CARME")) return CARME;
      else if (!strcmp(s,"ANANKE")) return ANANKE;
      else if (!strcmp(s,"LEDA")) return LEDA;
      else if (!strcmp(s,"THEBE")) return THEBE;
      else if (!strcmp(s,"ADRASTEA")) return ADRASTEA;
      else if (!strcmp(s,"METIS")) return METIS;
      else if (!strcmp(s,"MIMAS")) return MIMAS;
      else if (!strcmp(s,"ENCELADUS")) return ENCELADUS;
      else if (!strcmp(s,"TETHYS")) return TETHYS;
      else if (!strcmp(s,"DIONE")) return DIONE;
      else if (!strcmp(s,"RHEA")) return RHEA;
      else if (!strcmp(s,"TITAN")) return TITAN;
      else if (!strcmp(s,"HYPERION")) return HYPERION;
      else if (!strcmp(s,"IAPETUS")) return IAPETUS;
      else if (!strcmp(s,"PHOEBE")) return PHOEBE;
      else if (!strcmp(s,"JANUS")) return JANUS;
      else if (!strcmp(s,"EPIMETHEUS")) return EPIMETHEUS;
      else if (!strcmp(s,"HELENE")) return HELENE;
      else if (!strcmp(s,"TELESTO")) return TELESTO;
      else if (!strcmp(s,"CALYPSO")) return CALYPSO;
      else if (!strcmp(s,"ATLAS")) return ATLAS;
      else if (!strcmp(s,"PROMETHEUS")) return PROMETHEUS;
      else if (!strcmp(s,"PANDORA")) return PANDORA;
      else if (!strcmp(s,"PAN")) return PAN;
      else if (!strcmp(s,"ARIEL")) return ARIEL;
      else if (!strcmp(s,"UMBRIEL")) return UMBRIEL;
      else if (!strcmp(s,"TITANIA")) return TITANIA;
      else if (!strcmp(s,"OBERON")) return OBERON;
      else if (!strcmp(s,"MIRANDA")) return MIRANDA;
      else if (!strcmp(s,"TRITON")) return TRITON;
      else if (!strcmp(s,"NERIED")) return NERIED;
      else if (!strcmp(s,"CHARON")) return CHARON;
      else if (sscanf(s,"MINORBODY_%ld",&i) == 1) return(55+i);

      else if (!strcmp(s,"SUN")) return SUN;
      else if (!strcmp(s,"PLANET")) return PLANET;
      else if (!strcmp(s,"MOON")) return MOON;
      else if (!strcmp(s,"ASTEROID")) return ASTEROID;
      else if (!strcmp(s,"COMET")) return COMET;

      else if (!strcmp(s,"SIDE")) return VIEW_SIDE;
      else if (!strcmp(s,"TOP")) return VIEW_TOP;
      else if (!strcmp(s,"TWOSIGMA")) return TWOSIGMA_ATMO;
      else if (!strcmp(s,"NOMINAL")) return NOMINAL_ATMO;
      else if (!strcmp(s,"USER")) return USER_ATMO;
      else if (!strcmp(s,"TX")) return IPC_TX;
      else if (!strcmp(s,"RX")) return IPC_RX;
      else if (!strcmp(s,"TXRX")) return IPC_TXRX;
      else if (!strcmp(s,"ACS")) return IPC_ACS;
      else if (!strcmp(s,"WRITEFILE")) return IPC_WRITEFILE;
      else if (!strcmp(s,"READFILE")) return IPC_READFILE;
      else if (!strcmp(s,"SPIRENT")) return IPC_SPIRENT;
      else if (!strcmp(s,"FFTB")) return IPC_FFTB;
      else if (!strcmp(s,"SERVER")) return IPC_SERVER;
      else if (!strcmp(s,"CLIENT")) return IPC_CLIENT;
      else if (!strcmp(s,"GMSEC_CLIENT")) return IPC_GMSEC_CLIENT;
      
      else if (!strcmp(s,"MEAN")) return EPH_MEAN;
      else if (!strcmp(s,"DE430")) return EPH_DE430;

      else if (!strcmp(s,"MAJOR")) return MAJOR_CONSTELL;
      else if (!strcmp(s,"ZODIAC")) return ZODIAC_CONSTELL;
      else if (!strcmp(s,"MINOR")) return MINOR_CONSTELL;
      else {
         printf("Bogus input %s in DecodeString (42init.c:%d)\n",s,__LINE__);
         exit(1);
      }
}

/* #ifdef __cplusplus
** }
** #endif
*/
