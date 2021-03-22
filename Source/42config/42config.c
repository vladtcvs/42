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
#include <strings.h>

/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

/**********************************************************************/
long DecodeString(const char *s)
{
      long i;
      if (!strcasecmp(s,"FALSE")) return FALSE;
      else if (!strcasecmp(s,"TRUE")) return TRUE;
      else if (!strcasecmp(s,"OFF")) return OFF;
      else if (!strcasecmp(s,"ON")) return ON;
      else if (!strcasecmp(s,"FAST")) return FAST_TIME;
      else if (!strcasecmp(s,"REAL")) return REAL_TIME;
      else if (!strcasecmp(s,"EXTERNAL")) return EXTERNAL_TIME;
      else if (!strcasecmp(s,"NOS3")) return NOS3_TIME;
      else if (!strcasecmp(s,"SOL")) return SOL;
      else if (!strcasecmp(s,"MERCURY")) return MERCURY;
      else if (!strcasecmp(s,"VENUS")) return VENUS;
      else if (!strcasecmp(s,"EARTH")) return EARTH;
      else if (!strcasecmp(s,"MARS")) return MARS;
      else if (!strcasecmp(s,"JUPITER")) return JUPITER;
      else if (!strcasecmp(s,"SATURN")) return SATURN;
      else if (!strcasecmp(s,"URANUS")) return URANUS;
      else if (!strcasecmp(s,"NEPTUNE")) return NEPTUNE;
      else if (!strcasecmp(s,"PLUTO")) return PLUTO;
      else if (!strcasecmp(s,"LUNA")) return LUNA;
      else if (!strcasecmp(s,"KEP")) return INP_KEPLER;
      else if (!strcasecmp(s,"RV")) return INP_POSVEL;
      else if (!strcasecmp(s,"FILE")) return INP_FILE;
      else if (!strcasecmp(s,"TLE")) return INP_TLE;
      else if (!strcasecmp(s,"TRV")) return INP_TRV;
      else if (!strcasecmp(s,"PA")) return TRUE;
      else if (!strcasecmp(s,"AE")) return FALSE;
      else if (!strcasecmp(s,"POSW")) return TRUE;
      else if (!strcasecmp(s,"LLA")) return FALSE;
      else if (!strcasecmp(s,"POS")) return POSITIVE;
      else if (!strcasecmp(s,"NEG")) return NEGATIVE;
      else if (!strcasecmp(s,"CM")) return TRUE;
      else if (!strcasecmp(s,"ORIGIN")) return FALSE;
      else if (!strcasecmp(s,"CENTRAL")) return ORB_CENTRAL;
      else if (!strcasecmp(s,"THREE_BODY")) return ORB_THREE_BODY;
      else if (!strcasecmp(s,"ZERO")) return ORB_ZERO;
      else if (!strcasecmp(s,"FLIGHT")) return ORB_FLIGHT;
      else if (!strcasecmp(s,"EARTHMOON")) return EARTHMOON;
      else if (!strcasecmp(s,"SUNEARTH")) return SUNEARTH;
      else if (!strcasecmp(s,"SUNJUPITER")) return SUNJUPITER;
      else if (!strcasecmp(s,"MODES")) return INP_MODES;
      else if (!strcasecmp(s,"XYZ")) return INP_XYZ;
      else if (!strcasecmp(s,"SPLINE")) return INP_SPLINE;
      else if (!strcasecmp(s,"L1")) return LAGPT_L1;
      else if (!strcasecmp(s,"L2")) return LAGPT_L2;
      else if (!strcasecmp(s,"L3")) return LAGPT_L3;
      else if (!strcasecmp(s,"L4")) return LAGPT_L4;
      else if (!strcasecmp(s,"L5")) return LAGPT_L5;
      else if (!strcasecmp(s,"CW")) return DIR_CW;
      else if (!strcasecmp(s,"CCW")) return DIR_CCW;
      else if (!strcasecmp(s,"NONE")) return NONE;
      else if (!strcasecmp(s,"DIPOLE")) return DIPOLE;
      else if (!strcasecmp(s,"IGRF")) return IGRF;
      else if (!strcasecmp(s,"SPHERICAL")) return 1;
      else if (!strcasecmp(s,"GIMBAL")) return 0;
      else if (!strcasecmp(s,"X_AXIS")) return X_AXIS;
      else if (!strcasecmp(s,"Y_AXIS")) return Y_AXIS;
      else if (!strcasecmp(s,"Z_AXIS")) return Z_AXIS;
      else if (!strcasecmp(s,"POS_X")) return POS_X;
      else if (!strcasecmp(s,"POS_Y")) return POS_Y;
      else if (!strcasecmp(s,"POS_Z")) return POS_Z;
      else if (!strcasecmp(s,"NEG_X")) return NEG_X;
      else if (!strcasecmp(s,"NEG_Y")) return NEG_Y;
      else if (!strcasecmp(s,"NEG_Z")) return NEG_Z;
      else if (!strcasecmp(s,"REFORB")) return TARGET_REFORB;
      else if (!strcasecmp(s,"FRM")) return TARGET_FRM;
      else if (!strcasecmp(s,"SC")) return TARGET_SC;
      else if (!strcasecmp(s,"BODY")) return TARGET_BODY;

      else if (!strcasecmp(s,"TRACK_HOST")) return TRACK_HOST;
      else if (!strcasecmp(s,"TRACK_TARGET")) return TRACK_TARGET;
      else if (!strcasecmp(s,"FIXED_IN_HOST")) return FIXED_IN_HOST;

      else if (!strcasecmp(s,"DOWN")) return VIEW_DOWN;
      else if (!strcasecmp(s,"REAR_LEFT")) return VIEW_REAR_LEFT;
      else if (!strcasecmp(s,"REAR")) return VIEW_REAR;
      else if (!strcasecmp(s,"REAR_RIGHT")) return VIEW_REAR_RIGHT;
      else if (!strcasecmp(s,"LEFT")) return VIEW_LEFT;
      else if (!strcasecmp(s,"UP")) return VIEW_UP;
      else if (!strcasecmp(s,"RIGHT")) return VIEW_RIGHT;
      else if (!strcasecmp(s,"FRONT_LEFT")) return VIEW_FRONT_LEFT;
      else if (!strcasecmp(s,"FRONT")) return VIEW_FRONT;
      else if (!strcasecmp(s,"FRONT_RIGHT")) return VIEW_FRONT_RIGHT;

      else if (!strcasecmp(s,"WIREFRAME")) return FOV_WIREFRAME;
      else if (!strcasecmp(s,"SOLID")) return FOV_SOLID;
      else if (!strcasecmp(s,"VECTOR")) return FOV_VECTOR;
      else if (!strcasecmp(s,"PLANE")) return FOV_PLANE;

      else if (!strcasecmp(s,"REFPT_CM")) return REFPT_CM;
      else if (!strcasecmp(s,"REFPT_JOINT")) return REFPT_JOINT;

      else if (!strcasecmp(s,"LAGDOF_MODES")) return LAGDOF_MODES;
      else if (!strcasecmp(s,"LAGDOF_COWELL")) return LAGDOF_COWELL;
      else if (!strcasecmp(s,"LAGDOF_SPLINE")) return LAGDOF_SPLINE;

      else if (!strcasecmp(s,"STEADY")) return ROTDOF_STEADY;
      else if (!strcasecmp(s,"KIN_JOINT")) return ROTDOF_KIN_JOINT;
      else if (!strcasecmp(s,"DYN_JOINT")) return ROTDOF_DYN_JOINT;

      else if (!strcasecmp(s,"FIXED")) return ORBDOF_FIXED;
      else if (!strcasecmp(s,"EULER_HILL")) return ORBDOF_EULER_HILL;
      else if (!strcasecmp(s,"ENCKE")) return ORBDOF_ENCKE;
      else if (!strcasecmp(s,"COWELL")) return ORBDOF_COWELL;

      else if (!strcasecmp(s,"PASSIVE_FSW")) return PASSIVE_FSW;
      else if (!strcasecmp(s,"PROTOTYPE_FSW")) return PROTOTYPE_FSW;
      else if (!strcasecmp(s,"AD_HOC_FSW")) return AD_HOC_FSW;
      else if (!strcasecmp(s,"SPINNER_FSW")) return SPINNER_FSW;
      else if (!strcasecmp(s,"MOMBIAS_FSW")) return MOMBIAS_FSW;
      else if (!strcasecmp(s,"THREE_AXIS_FSW")) return THREE_AXIS_FSW;
      else if (!strcasecmp(s,"ISS_FSW")) return ISS_FSW;
      else if (!strcasecmp(s,"CMG_FSW")) return CMG_FSW;
      else if (!strcasecmp(s,"THR_FSW")) return THR_FSW;
      else if (!strcasecmp(s,"CFS_FSW")) return CFS_FSW;

      else if (!strcasecmp(s,"PHOBOS")) return PHOBOS;
      else if (!strcasecmp(s,"DEIMOS")) return DEIMOS;
      else if (!strcasecmp(s,"IO")) return IO;
      else if (!strcasecmp(s,"EUROPA")) return EUROPA;
      else if (!strcasecmp(s,"GANYMEDE")) return GANYMEDE;
      else if (!strcasecmp(s,"CALLISTO")) return CALLISTO;
      else if (!strcasecmp(s,"AMALTHEA")) return AMALTHEA;
      else if (!strcasecmp(s,"HIMALIA")) return HIMALIA;
      else if (!strcasecmp(s,"ELARA")) return ELARA;
      else if (!strcasecmp(s,"PASIPHAE")) return PASIPHAE;
      else if (!strcasecmp(s,"SINOPE")) return SINOPE;
      else if (!strcasecmp(s,"LYSITHEA")) return LYSITHEA;
      else if (!strcasecmp(s,"CARME")) return CARME;
      else if (!strcasecmp(s,"ANANKE")) return ANANKE;
      else if (!strcasecmp(s,"LEDA")) return LEDA;
      else if (!strcasecmp(s,"THEBE")) return THEBE;
      else if (!strcasecmp(s,"ADRASTEA")) return ADRASTEA;
      else if (!strcasecmp(s,"METIS")) return METIS;
      else if (!strcasecmp(s,"MIMAS")) return MIMAS;
      else if (!strcasecmp(s,"ENCELADUS")) return ENCELADUS;
      else if (!strcasecmp(s,"TETHYS")) return TETHYS;
      else if (!strcasecmp(s,"DIONE")) return DIONE;
      else if (!strcasecmp(s,"RHEA")) return RHEA;
      else if (!strcasecmp(s,"TITAN")) return TITAN;
      else if (!strcasecmp(s,"HYPERION")) return HYPERION;
      else if (!strcasecmp(s,"IAPETUS")) return IAPETUS;
      else if (!strcasecmp(s,"PHOEBE")) return PHOEBE;
      else if (!strcasecmp(s,"JANUS")) return JANUS;
      else if (!strcasecmp(s,"EPIMETHEUS")) return EPIMETHEUS;
      else if (!strcasecmp(s,"HELENE")) return HELENE;
      else if (!strcasecmp(s,"TELESTO")) return TELESTO;
      else if (!strcasecmp(s,"CALYPSO")) return CALYPSO;
      else if (!strcasecmp(s,"ATLAS")) return ATLAS;
      else if (!strcasecmp(s,"PROMETHEUS")) return PROMETHEUS;
      else if (!strcasecmp(s,"PANDORA")) return PANDORA;
      else if (!strcasecmp(s,"PAN")) return PAN;
      else if (!strcasecmp(s,"ARIEL")) return ARIEL;
      else if (!strcasecmp(s,"UMBRIEL")) return UMBRIEL;
      else if (!strcasecmp(s,"TITANIA")) return TITANIA;
      else if (!strcasecmp(s,"OBERON")) return OBERON;
      else if (!strcasecmp(s,"MIRANDA")) return MIRANDA;
      else if (!strcasecmp(s,"TRITON")) return TRITON;
      else if (!strcasecmp(s,"NERIED")) return NERIED;
      else if (!strcasecmp(s,"CHARON")) return CHARON;
      else if (sscanf(s,"MINORBODY_%ld",&i) == 1) return(55+i);

      else if (!strcasecmp(s,"SUN")) return SUN;
      else if (!strcasecmp(s,"PLANET")) return PLANET;
      else if (!strcasecmp(s,"MOON")) return MOON;
      else if (!strcasecmp(s,"ASTEROID")) return ASTEROID;
      else if (!strcasecmp(s,"COMET")) return COMET;

      else if (!strcasecmp(s,"SIDE")) return VIEW_SIDE;
      else if (!strcasecmp(s,"TOP")) return VIEW_TOP;
      else if (!strcasecmp(s,"TWOSIGMA")) return TWOSIGMA_ATMO;
      else if (!strcasecmp(s,"NOMINAL")) return NOMINAL_ATMO;
      else if (!strcasecmp(s,"USER")) return USER_ATMO;
      else if (!strcasecmp(s,"TX")) return IPC_TX;
      else if (!strcasecmp(s,"RX")) return IPC_RX;
      else if (!strcasecmp(s,"TXRX")) return IPC_TXRX;
      else if (!strcasecmp(s,"ACS")) return IPC_ACS;
      else if (!strcasecmp(s,"WRITEFILE")) return IPC_WRITEFILE;
      else if (!strcasecmp(s,"READFILE")) return IPC_READFILE;
      else if (!strcasecmp(s,"SPIRENT")) return IPC_SPIRENT;
      else if (!strcasecmp(s,"FFTB")) return IPC_FFTB;
      else if (!strcasecmp(s,"SERVER")) return IPC_SERVER;
      else if (!strcasecmp(s,"CLIENT")) return IPC_CLIENT;
      else if (!strcasecmp(s,"GMSEC_CLIENT")) return IPC_GMSEC_CLIENT;
      
      else if (!strcasecmp(s,"MEAN")) return EPH_MEAN;
      else if (!strcasecmp(s,"DE430")) return EPH_DE430;

      else if (!strcasecmp(s,"MAJOR")) return MAJOR_CONSTELL;
      else if (!strcasecmp(s,"ZODIAC")) return ZODIAC_CONSTELL;
      else if (!strcasecmp(s,"MINOR")) return MINOR_CONSTELL;
      else {
         printf("Bogus input %s in DecodeString (42init.c:%d)\n",s,__LINE__);
         exit(1);
      }
}

long LoadTRVfromFile(const char *Path, const char *TrvFileName,
                     const char *ElemLabel, double Time, struct OrbitType *O)
{
      FILE *infile;
      char line[80],response1[80],response2[80];
      char Label[25];
      long i,Nchar;
      long Success = 0;
      double EpochJD,R[3],V[3];
      long EpochYear,EpochMonth,EpochDay,EpochHour,EpochMinute;
      double EpochSecond;

      infile=FileOpen(Path,TrvFileName,"r");

      Nchar = strlen(ElemLabel);
      /* Pad label to 24 characters to assure unique match */
      while(!feof(infile) && !Success) {
         fgets(line,79,infile);
         if (sscanf(line,"\"%[^\"]\"",Label) == 1) {
            if (!strncmp(Label,ElemLabel,Nchar)) {
               Success = 1;
               fscanf(infile,"%s %s %ld-%ld-%ld %ld:%ld:%lf\n",
                  response1,response2,
                  &EpochYear,&EpochMonth,&EpochDay,
                  &EpochHour,&EpochMinute,&EpochSecond);
               fscanf(infile,"%lf %lf %lf\n",&R[0],&R[1],&R[2]);
               fscanf(infile,"%lf %lf %lf\n",&V[0],&V[1],&V[2]);
            }
         }
      }
      fclose(infile);

      if (Success) {
         /* Epoch is in UTC */
         EpochJD = DateToJD(EpochYear,EpochMonth,EpochDay,
            EpochHour,EpochMinute,EpochSecond);
         O->Epoch = JDToTime(EpochJD);
         O->Epoch += DynTime-CivilTime;
         O->Regime = DecodeString(response1);
         if (O->Regime == ORB_CENTRAL) {
            O->World = DecodeString(response2);
            O->mu = World[O->World].mu;
            RV2Eph(O->Epoch,O->mu,R,V,&O->SMA,&O->ecc,&O->inc,&O->RAAN,
               &O->ArgP,&O->anom,&O->tp,&O->SLR,&O->alpha,&O->rmin,
               &O->MeanMotion,&O->Period);
            Eph2RV(O->mu,O->SLR,O->ecc,O->inc,O->RAAN,O->ArgP,Time-O->Epoch,
               O->PosN,O->VelN,&O->anom);
         }
         else {
            O->Sys = DecodeString(response2);
            O->Body1 = LagSys[O->Sys].Body1;
            O->Body2 = LagSys[O->Sys].Body2;
            O->mu1 = World[O->Body1].mu;
            O->mu2 = World[O->Body2].mu;
            O->World = O->Body1;
            O->mu = O->mu1;
            for(i=0;i<3;i++) {
               O->PosN[i] = R[i];
               O->VelN[i] = V[i];
            }
            /* RV2LagModes(O->Epoch,&LagSys[O->Sys],O); */
            R2StableLagMode(O->Epoch,&LagSys[O->Sys],O);
            LagModes2RV(Time,&LagSys[O->Sys],O,O->PosN,O->VelN);
         }
      }

      return(Success);
}

/* #ifdef __cplusplus
** }
** #endif
*/
