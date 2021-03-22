#include "42.h"
#include <stdio.h>
#include "42config/txt/42orbit.h"
#include "42config/42config.h"

void InitOrbitTxt(struct OrbitType *O)
{
      FILE *infile;
      char junk[120],newline,response[120];
      double Alt1,Alt2,MaxAnom;
      double mu,rad;
      double p[3],Ang1,Ang2,Ang3;
      char FrmExpressedIn;
      long Ir,i,j,k,Seq;
      struct FormationType *F;
      double AmpXY1,PhiXY1,SenseXY1;
      double AmpXY2,PhiXY2,SenseXY2;
      double AmpZ,PhiZ;
      struct RegionType *R;
      long InputType,UsePA,ElementType;
      char ElementLabel[40];
      char ElementFileName[40];
      long Success;
      long NodeYear,NodeMonth,NodeDay,NodeHour,NodeMin;
      double NodeSec;

      infile = FileOpen(InOutPath,O->FileName,"r");

/* .. Orbit Parameters */
      O->Epoch = DynTime;
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      O->Regime = DecodeString(response);

      if (O->Regime == ORB_ZERO) {
         fscanf(infile,"%[^\n] %[\n]",junk,&newline);
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         O->World = DecodeString(response);
         if (!World[O->World].Exists) {
            printf("Oops.  Orbit %ld depends on a World that doesn't exist.\n",O->Tag);
            exit(1);
         }
         O->mu = World[O->World].mu;
         for(j=0;j<3;j++) {
            O->PosN[j] = 0.0;
            O->VelN[j] = 0.0;
            for(k=0;k<3;k++) O->CLN[j][k] = 0.0;
            O->CLN[j][j] = 1.0;
            O->wln[j] = 0.0;
         }
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         O->PolyhedronGravityEnabled = DecodeString(response);
         /* Skip FLIGHT, CENTRAL, THREE_BODY sections */
         for(j=0;j<35;j++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      }
      else if (O->Regime == ORB_FLIGHT) {
         /* Skip ZERO section */
         for(j=0;j<3;j++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
         fscanf(infile,"%[^\n] %[\n]",junk,&newline);
         fscanf(infile,"%ld %[^\n] %[\n]",&Ir,junk,&newline);
         if (!Rgn[Ir].Exists) {
            printf("Oops.  Orbit %ld depends on a Region that doesn't exist.\n",O->Tag);
            exit(1);
         }
         O->Region = Ir;
         R = &Rgn[Ir];
         O->World = R->World;
         O->mu = World[O->World].mu;
         for(j=0;j<3;j++) {
            O->PosN[j] = R->PosN[j];
            O->VelN[j] = R->VelN[j];
            for(k=0;k<3;k++) O->CLN[j][k] = R->CN[j][k];
            O->wln[0] = 0.0;
            O->wln[1] = 0.0;
            O->wln[2] = World[O->World].w;
         }
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         O->PolyhedronGravityEnabled = DecodeString(response);

         /* Skip CENTRAL and THREE_BODY sections */
         for(j=0;j<32;j++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      }
      else if (O->Regime == ORB_CENTRAL) {
         /* Skip ZERO and FLIGHT sections */
         for(j=0;j<6;j++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);

         fscanf(infile,"%[^\n] %[\n]",junk,&newline);
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         O->World = DecodeString(response);
         if (!World[O->World].Exists) {
            printf("Oops.  Orbit %ld depends on a World that doesn't exist.\n",O->Tag);
            exit(1);
         }
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         O->J2DriftEnabled = DecodeString(response);
         mu = World[O->World].mu;
         rad = World[O->World].rad;
         O->mu=mu;
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         InputType = DecodeString(response);
         if (InputType == INP_KEPLER) {
            fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
            UsePA = DecodeString(response);
            if (UsePA) {
               fscanf(infile,"%lf %lf %[^\n] %[\n]",&Alt1,&Alt2,
                      junk,&newline);
               if (Alt2 < Alt1) {
                  printf("Apoapsis below Periapsis for Orbit %ld\n",O->Tag);
                  exit(1);
               }
               fscanf(infile,"%[^\n] %[\n]",junk,&newline);
               O->SMA=rad+0.5*(Alt1+Alt2)*1.0E3;
               O->ecc=1.0E3*fabs(Alt1-Alt2)/(2.0*O->SMA);
               O->SLR = O->SMA*(1.0-O->ecc*O->ecc);
               O->alpha = 1.0/O->SMA;
               O->rmin = rad+Alt1*1.0E3;
            }
            else {
               fscanf(infile,"%[^\n] %[\n]",junk,&newline);
               fscanf(infile,"%lf %lf %[^\n] %[\n]",
                      &Alt1,&O->ecc,junk,&newline);
               O->rmin = rad+Alt1*1.0E3;
               O->SLR = O->rmin*(1.0+O->ecc);
               O->alpha = (1.0-O->ecc)/O->rmin;
               if (O->alpha != 0.0) O->SMA = 1.0/O->alpha;
            }
            fscanf(infile,"%lf %[^\n] %[\n]",&O->inc,junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&O->RAAN,junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&O->ArgP,junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&O->anom,junk,&newline);
            O->inc *= D2R;
            O->RAAN *= D2R;
            O->ArgP *= D2R;
            O->anom *= D2R;

            /* Some anomalies are unreachable for hyperbolic trajectories */
            if (O->ecc > 1.0) {
               MaxAnom = Pi - acos(1.0/O->ecc);
               if (fabs(O->anom) > MaxAnom){
                  printf("True Anomaly out of range for Orbit %ld\n",O->Tag);
                  exit(1);
               }
            }

            if (O->J2DriftEnabled) {
               FindJ2DriftParms(O->mu,World[O->World].J2,World[O->World].rad,O);
               O->RAAN0 = O->RAAN;
               O->ArgP0 = O->ArgP;
               O->tp = O->Epoch - TimeSincePeriapsis(O->MuPlusJ2,O->SLR,O->ecc,O->anom);
               Eph2RV(O->MuPlusJ2,O->SLR,O->ecc,O->inc,
                      O->RAAN,O->ArgP,O->Epoch-O->tp,
                      O->PosN,O->VelN,&O->anom);
            }
            else {
               O->MuPlusJ2 = O->mu;
               O->RAANdot = 0.0;
               O->ArgPdot = 0.0;
               O->J2Fr0 = 0.0;
               O->J2Fh1 = 0.0;
               O->MeanMotion = sqrt(O->mu/(O->SMA*O->SMA*O->SMA));
               O->Period = TwoPi/O->MeanMotion;
               O->tp = O->Epoch - TimeSincePeriapsis(O->mu,O->SLR,O->ecc,O->anom);
               Eph2RV(O->mu,O->SLR,O->ecc,O->inc,
                      O->RAAN,O->ArgP,O->Epoch-O->tp,
                      O->PosN,O->VelN,&O->anom);
            }

            /* Skip RV and FILE */
            for(j=0;j<4;j++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
         }
         else if (InputType == INP_POSVEL) {
            /* Skip KEPLER section */
            for(j=0;j<7;j++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
               &O->PosN[0],&O->PosN[1],&O->PosN[2],
               junk,&newline);
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
                  &O->VelN[0],&O->VelN[1],&O->VelN[2],
                  junk,&newline);
            for(j=0;j<3;j++){
               O->PosN[j] *= 1.0E3;
               O->VelN[j] *= 1.0E3;
            }            
            if (O->J2DriftEnabled) {
               FindJ2DriftParms(O->mu,World[O->World].J2,World[O->World].rad,O);
               RV2Eph(O->Epoch,O->MuPlusJ2,O->PosN,O->VelN,
                  &O->SMA,&O->ecc,&O->inc,&O->RAAN,
                  &O->ArgP,&O->anom,&O->tp,
                  &O->SLR,&O->alpha,&O->rmin,
                  &O->MeanMotion,&O->Period);
               O->RAAN0 = O->RAAN;
               O->ArgP0 = O->ArgP;
            }
            else {
               O->MuPlusJ2 = O->mu;
               O->RAANdot = 0.0;
               O->ArgPdot = 0.0;
               O->J2Fr0 = 0.0;
               O->J2Fh1 = 0.0;
               RV2Eph(O->Epoch,O->mu,O->PosN,O->VelN,
                  &O->SMA,&O->ecc,&O->inc,&O->RAAN,
                  &O->ArgP,&O->anom,&O->tp,
                  &O->SLR,&O->alpha,&O->rmin,
                  &O->MeanMotion,&O->Period);
            }
            /* Skip FILE section */
            for(j=0;j<2;j++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
         }
         else if (InputType == INP_FILE) {
            for(j=0;j<9;j++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%s \"%[^\"]\" %[^\n] %[\n]",
               response,ElementLabel,junk,&newline);
            ElementType = DecodeString(response);
            fscanf(infile,"\"%[^\"]\" %[^\n] %[\n]",ElementFileName,junk,&newline);
            if (ElementType == INP_TLE) {
               Success = LoadTleFromFile(InOutPath,ElementFileName,
                  ElementLabel,TT.JulDay,O->mu,O);
               if (!Success) {
                  printf("Error loading TLE %s from file %s.\n",
                     ElementLabel,ElementFileName);
                  exit(1);
               }
            }
            else if (ElementType == INP_TRV) {
               Success = LoadTRVfromFile(InOutPath, ElementFileName,
                  ElementLabel, CivilTime, O);
               if (!Success) {
                  printf("Error loading TRV %s from file %s.\n",
                     ElementLabel,ElementFileName);
                  exit(1);
               }
            }
            else {
               printf("Oops.  Unknown ElementType in InitOrbit.\n");
               exit(1);
            }
            if (O->J2DriftEnabled) {
               FindJ2DriftParms(O->mu,World[O->World].J2,World[O->World].rad,O);
               O->RAAN0 = O->RAAN + O->RAANdot*(DynTime-O->Epoch);
               O->ArgP0 = O->ArgP + O->ArgPdot*(DynTime-O->Epoch);
               O->tp = O->Epoch - TimeSincePeriapsis(O->MuPlusJ2,O->SLR,O->ecc,O->anom);
               Eph2RV(O->MuPlusJ2,O->SLR,O->ecc,O->inc,
                      O->RAAN,O->ArgP,O->Epoch-O->tp,
                      O->PosN,O->VelN,&O->anom);
            }
            else {
               O->MuPlusJ2 = O->mu;
               O->RAANdot = 0.0;
               O->ArgPdot = 0.0;
               O->J2Fr0 = 0.0;
               O->J2Fh1 = 0.0;
               O->MeanMotion = sqrt(O->mu/(O->SMA*O->SMA*O->SMA));
               O->Period = TwoPi/O->MeanMotion;
               O->tp = O->Epoch - TimeSincePeriapsis(O->mu,O->SLR,O->ecc,O->anom);
               Eph2RV(O->mu,O->SLR,O->ecc,O->inc,
                      O->RAAN,O->ArgP,O->Epoch-O->tp,
                      O->PosN,O->VelN,&O->anom);
            }
         }
         else {
            printf("Oops.  Unknown InputType in InitOrbit.\n");
            exit(1);
         }
         FindCLN(O->PosN,O->VelN,O->CLN,O->wln);

         /* Skip THREE_BODY section */
         for(j=0;j<17;j++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);

      }
      else if (O->Regime == ORB_THREE_BODY) {
         /* Skip ZERO, FLIGHT, and CENTRAL sections */
         for(j=0;j<21;j++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);

         fscanf(infile,"%[^\n] %[\n]",junk,&newline);
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         O->Sys = DecodeString(response);
         if (!LagSys[O->Sys].Exists) {
            printf("Oops.  Orbit %ld depends on a Lagrange System that doesn't exist.\n",O->Tag);
            exit(1);
         }
         O->Body1 = LagSys[O->Sys].Body1;
         O->Body2 = LagSys[O->Sys].Body2;
         O->mu1 =  LagSys[O->Sys].mu1;
         O->mu2 =  LagSys[O->Sys].mu2;
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         O->LagDOF = DecodeString(response);
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         InputType = DecodeString(response);
         if (InputType == INP_MODES) { /* Initial modes given */
            fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
            O->LP = DecodeString(response);
            fscanf(infile,"%lf %[^\n] %[\n]",
                    &AmpXY1,junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",
                    &PhiXY1,junk,&newline);
            fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
            if (DecodeString(response) == DIR_CCW) SenseXY1 = 1.0;
            else SenseXY1 = -1.0;
            fscanf(infile,"%lf %[^\n] %[\n]",
                    &AmpXY2,junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",
                    &PhiXY2,junk,&newline);
            fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
            if (DecodeString(response) == DIR_CCW) SenseXY2 = 1.0;
            else SenseXY2 = -1.0;
            fscanf(infile,"%lf %[^\n] %[\n]",
                    &AmpZ,junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",
                    &PhiZ,junk,&newline);
            AmpXY1  *= 1000.0;
            AmpXY2  *= 1000.0;
            AmpZ    *= 1000.0;
            PhiXY1 *= D2R;
            PhiXY2 *= D2R;
            PhiZ   *= D2R;
            AmpPhase2LagModes(0.0,AmpXY1,PhiXY1,SenseXY1,
               AmpXY2,PhiXY2,SenseXY2,AmpZ,PhiZ,
               &LagSys[O->Sys],O);
            /* Find r,v from modal description */
            LagModes2RV(DynTime,&LagSys[O->Sys],O,O->PosN,O->VelN);
            /* Skip XYZ and FILE sections */
            for(j=0;j<4;j++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
         }
         else if (InputType == INP_XYZ) {
            /* Skip MODES section */
            for(j=0;j<9;j++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
                    &O->x,&O->y,&O->z,junk,&newline);
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
                    &O->xdot,&O->ydot,&O->zdot,junk,&newline);
            XYZ2LagModes(0.0,&LagSys[O->Sys],O);
            LagModes2RV(DynTime,&LagSys[O->Sys],O,O->PosN,O->VelN);
            /* Skip FILE section */
            for(j=0;j<2;j++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
         }
         else if (InputType == INP_FILE) {
            fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
            O->LP = DecodeString(response);
            /* Skip MODES and XYZ sections */
            for(j=0;j<10;j++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%s \"%[^\"]\" %[^\n] %[\n]",
               response,ElementLabel,junk,&newline);
            ElementType = DecodeString(response);
            fscanf(infile,"\"%[^\"]\" %[^\n] %[\n]",ElementFileName,junk,&newline);
            if (ElementType == INP_TRV) {
               Success = LoadTRVfromFile(InOutPath, ElementFileName,
                  ElementLabel, CivilTime, O);
               if (!Success) {
                  printf("Error loading TRV %s from file %s.\n",
                     ElementLabel,ElementFileName);
                  exit(1);
               }
            }
            else if (ElementType == INP_SPLINE) {
               O->SplineFile = FileOpen(InOutPath,ElementFileName,"rt");
               for(i=0;i<4;i++) {
                  fscanf(O->SplineFile," %ld:%ld:%ld:%ld:%ld:%lf %lf %lf %lf %lf %lf %lf %[\n]",
                     &NodeYear,&NodeMonth,&NodeDay,&NodeHour,&NodeMin,&NodeSec,
                     &O->NodePos[i][0],&O->NodePos[i][1],&O->NodePos[i][2],
                     &O->NodeVel[i][0],&O->NodeVel[i][1],&O->NodeVel[i][2],
                     &newline);
                  O->NodeDynTime[i] = DateToTime(NodeYear,NodeMonth,NodeDay,
                     NodeHour,NodeMin,NodeSec);
                  for(j=0;j<3;j++) {
                     O->NodePos[i][j] *= 1000.0;
                     O->NodeVel[i][j] *= 1000.0;
                  }
                  if (DynTime < O->NodeDynTime[1]) {
                     printf("Oops.  Spline file beginning is in the future.\n");
                     exit(1);
                  }
               }
               SplineToPosVel(O);
            }
            else {
               printf("Oops.  Unknown ElementType in InitOrbit.\n");
               exit(1);
            }
         }
         else {
            printf("Oops.  Unknown Input Type in InitOrbit.\n");
            exit(1);
         }
         O->World = O->Body1;
         O->mu = O->mu1;
         O->SMA = MAGV(O->PosN); /* For sake of EH */
         FindCLN(O->PosN,O->VelN,O->CLN,O->wln);
         O->MeanMotion = LagSys[O->Sys].MeanRate;
         O->Period = TwoPi/O->MeanMotion;
      }
      else {
         printf("Bogus Orbit Regime in file %s\n",O->FileName);
         exit(1);
      }

/* .. Formation Frame Parameters */
      F = &Frm[O->Tag];
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      F->FixedInFrame = response[0];
      fscanf(infile,"%lf %lf %lf %ld %[^\n] %[\n]",
             &Ang1,&Ang2,&Ang3,&Seq,junk,&newline);
      A2C(Seq,Ang1*D2R,Ang2*D2R,Ang3*D2R,F->CN);
      if(F->FixedInFrame == 'L') {
         /* Adjust CFN */
         for(j=0;j<3;j++){
            for(k=0;k<3;k++) F->CL[j][k] = F->CN[j][k];
         }
         MxM(F->CL,O->CLN,F->CN);
      }
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      FrmExpressedIn = response[0];
      fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
             &F->PosR[0],&F->PosR[1],&F->PosR[2],junk,&newline);
      if (FrmExpressedIn == 'L') {
         for(j=0;j<3;j++) p[j] = F->PosR[j];
         MTxV(O->CLN,p,F->PosR);
      }

      fclose(infile);

}
