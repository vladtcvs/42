/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2010 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */

/*    Copyright 2021 Vladislav Tsendrovskii <vtcendrovskii@gmail.com> */

#include "42.h"
#include "42config/txt/42sc.h"
#include <ctype.h>


/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

/**********************************************************************/
void ConfigSpacecraftTxt(struct SCType *S, double *PosVec, double *VelVec, long *UseCM)
{
      FILE *infile;
      char junk[120],newline,response[120];
      char response1[120],response2[120],response3[120];
      double CBL[3][3],CBF[3][3];
      long i,j,k,Ia,Ib,Ig,Iw,Im,It,Bi,Bo,Ic,Ist,Ifss;
      char RateFrame,AttFrame,AttParm;
      double wlnb[3];
      double wbn[3],CBN[3][3],qbn[4];
      double Ang1,Ang2,Ang3;
      double pIn[3],pOut[3];
      double psn[3],vsn[3],psl[3],vsl[3],pfl[3],pcmn[3],pcml[3];
      double wxr[3],wxrn[3],wxrl[3];
      double rh[3],vh[3];
      double BiasTime;
      long Seq;
      long i1,i2,i3;
      
      struct JointType *G;
      struct BodyType *B;
      struct OrbitType *O;
      struct FormationType *Fr;
      struct DynType *D;
      struct GyroType *Gyro;
      struct MagnetometerType *MAG;
      struct CssType *CSS;
      struct FssType *FSS;
      struct StarTrackerType *ST;
      struct GpsType *GPS;
      struct AccelType *Accel;
      long OldNgeom;

      infile=FileOpen(InOutPath,S->FileName,"r");
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"\"%[^\"]\" %[^\n] %[\n]",S->Label,junk,&newline);
      fscanf(infile,"%s %[^\n] %[\n]",S->SpriteFileName,junk,&newline);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      S->FswTag = DecodeString(response);
      fscanf(infile,"%lf %[^\n] %[\n]",&S->FswSampleTime,junk,&newline);
      S->FswMaxCounter = (long) (S->FswSampleTime/DTSIM+0.5);
      if (S->FswMaxCounter < 1) {
         S->FswMaxCounter = 1;
         S->FswSampleTime = DTSIM;
         printf("Info:  FswSampleTime was smaller than DTSIM.  It has been adjusted to be DTSIM.\n");
      }
      S->FswSampleCounter = S->FswMaxCounter;
      S->InitAC = 1;

/* .. Orbit Parameters */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      S->OrbDOF=DecodeString(response);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      *UseCM=DecodeString(response);
      fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
             &PosVec[0],&PosVec[1],&PosVec[2],junk,&newline);
      fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
             &VelVec[0],&VelVec[1],&VelVec[2],junk,&newline);

/* .. Initial Attitude */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      RateFrame=response[0];
      AttParm=response[1];
      AttFrame=response[2];
      fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",&wbn[0],
             &wbn[1],&wbn[2],junk,&newline);
      wbn[0] *= D2R;
      wbn[1] *= D2R;
      wbn[2] *= D2R;
      if (AttParm == 'Q') {
         fscanf(infile,"%lf %lf %lf %lf %[^\n] %[\n]",
                &qbn[0],&qbn[1],&qbn[2],&qbn[3],junk,&newline);
         fscanf(infile,"%[^\n] %[\n]",junk,&newline);
         Q2C(qbn,CBN);
      }
      else {
         fscanf(infile,"%[^\n] %[\n]",junk,&newline);
         fscanf(infile,"%lf %lf %lf %ld %[^\n] %[\n]",
                &Ang1,&Ang2,&Ang3,&Seq,junk,&newline);
         A2C(Seq,Ang1*D2R,Ang2*D2R,Ang3*D2R,CBN);
         C2Q(CBN,qbn);
      }
      if(AttFrame == 'L') {
         /* Adjust CBN */
         for(j=0;j<3;j++){
            for(k=0;k<3;k++) CBL[j][k] = CBN[j][k];
         }
         MxM(CBL,Orb[S->RefOrb].CLN,CBN);
         C2Q(CBN,qbn);
      }
      else if (AttFrame == 'F') {
         /* Adjust CBN */
         for(j=0;j<3;j++){
            for(k=0;k<3;k++) CBF[j][k] = CBN[j][k];
         }
         MxM(CBF,Frm[S->RefOrb].CN,CBN);
         C2Q(CBN,qbn);
      }
      if(RateFrame == 'L') {
         /* Add LVLH rate to wn */
         MxV(CBN,Orb[S->RefOrb].wln,wlnb);
         for(j=0;j<3;j++) wbn[j] += wlnb[j];
      }

      MxMT(CBN,Frm[S->RefOrb].CN,S->CF);

/* .. Dynamics Flags */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      S->RotDOF=DecodeString(response);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      S->PassiveJointFrcTrqEnabled=DecodeString(response);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      S->ConstraintsRequested=DecodeString(response);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      S->RefPt=DecodeString(response);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      S->FlexActive=DecodeString(response);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      S->IncludeSecondOrderFlexTerms=DecodeString(response);
      fscanf(infile,"%lf %[^\n] %[\n]",&S->DragCoef,junk,&newline);

/* .. Body parameters */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%ld %[^\n] %[\n]",&S->Nb,junk,&newline);
      S->Ng = S->Nb-1;
      S->B = (struct BodyType *) calloc(S->Nb,sizeof(struct BodyType));
      if (S->B == NULL) {
         printf("S->B calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }
      S->G = (struct JointType *) calloc(S->Ng,sizeof(struct JointType));
      if (S->G == NULL) {
         printf("S->G calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      /* Load B[0] initial attitude */
      for(j=0;j<3;j++) {
         S->B[0].wn[j] = wbn[j];
         for(k=0;k<3;k++) S->B[0].CN[j][k] = CBN[j][k];
      }
      for(j=0;j<4;j++) S->B[0].qn[j] = qbn[j];

/* .. Body Ib */
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         fscanf(infile,"%[^\n] %[\n]",junk,&newline);
         fscanf(infile,"%lf %[^\n] %[\n]",&B->mass,junk,&newline);
         fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",&B->I[0][0],
                  &B->I[1][1],&B->I[2][2],junk,&newline);
         fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",&B->I[0][1],
                  &B->I[0][2],&B->I[1][2],junk,&newline);
         B->I[0][1] *= -1.0;
         B->I[0][2] *= -1.0;
         B->I[1][2] *= -1.0;
         B->I[1][0] = B->I[0][1];
         B->I[2][0] = B->I[0][2];
         B->I[2][1] = B->I[1][2];
         fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",&B->cm[0],
                  &B->cm[1],&B->cm[2],junk,&newline);
         fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",&B->Hgyro[0],
                  &B->Hgyro[1],&B->Hgyro[2],junk,&newline);
         fscanf(infile,"%s %[^\n] %[\n]",B->GeomFileName,junk,&newline);
         fscanf(infile,"%s %[^\n] %[\n]",B->FlexFileName,junk,&newline);
         if (S->RefPt == REFPT_JOINT)
            for(i=0;i<3;i++) B->c[i] = B->mass*B->cm[i];
         else
            for(i=0;i<3;i++) B->c[i] = 0.0;
      }

/* .. Joint Parameters */
      /* Read Spacecraft input file */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      if (S->Ng == 0) {  /* Read and discard template */
         for(i=0;i<18;i++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      }
      else {
         for(Ig=0;Ig<S->Ng;Ig++) {
            G = &S->G[Ig];
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%ld %ld %[^\n] %[\n]",
                   &G->Bin,&G->Bout,junk,&newline);
            if (G->Bin > G->Bout) {
               printf("Yo!  SC[%ld].G[%ld] inner body index (%ld) is greater than outer body index (%ld)\n",S->ID,Ig,G->Bin,G->Bout);
               printf("You must define inner bodies before outer bodies!\n");
               exit(1);
            }
            Bi=G->Bin;
            Bo=G->Bout;
            S->B[Bo].Gin = Ig;

            fscanf(infile,"%ld %ld %s %[^\n] %[\n]",
               &G->RotDOF,&G->RotSeq,response,junk,&newline);
            G->IsSpherical = DecodeString(response);
            if (G->RotSeq < 100) {
               printf("Invalid RotSeq %ld for SC[%ld].G[%ld].  All three axes required.\n",
                  G->RotSeq,S->ID,Ig);
               exit(1);
            }
            i3 = G->RotSeq % 10;         /* Pick off third digit */
            i2 = (G->RotSeq % 100)/10;   /* Extract second digit */
            i1 = G->RotSeq/100;          /* Pick off first digit */
            if (i1 == i2 || i1 == i3 || i2 == i3) {
               printf("Invalid RotSeq %ld for SC[%ld].G[%ld].  Repeated indices are not allowed.\n",
                  G->RotSeq,S->ID,Ig);
               exit(1);
            }

            fscanf(infile,"%ld %ld %[^\n] %[\n]",
               &G->TrnDOF,&G->TrnSeq,junk,&newline);
            if (G->TrnSeq < 100) {
               printf("Invalid TrnSeq %ld for SC[%ld].G[%ld].  All three axes required.\n",
                  G->TrnSeq,S->ID,Ig);
               exit(1);
            }
            i3 = G->TrnSeq % 10;         /* Pick off third digit */
            i2 = (G->TrnSeq % 100)/10;   /* Extract second digit */
            i1 = G->TrnSeq/100;          /* Pick off first digit */
            if (i1 == i2 || i1 == i3 || i2 == i3) {
               printf("Invalid TrnSeq %ld for SC[%ld].G[%ld].  Repeated indices are not allowed.\n",
                  G->TrnSeq,S->ID,Ig);
               exit(1);
            }
            fscanf(infile,"%s %s %s %[^\n] %[\n]",
               response1,response2,response3,junk,&newline);
            G->RotLocked[0] = DecodeString(response1);
            G->RotLocked[1] = DecodeString(response2);
            G->RotLocked[2] = DecodeString(response3);
            fscanf(infile,"%s %s %s %[^\n] %[\n]",
               response1,response2,response3,junk,&newline);
            G->TrnLocked[0] = DecodeString(response1);
            G->TrnLocked[1] = DecodeString(response2);
            G->TrnLocked[2] = DecodeString(response3);

            /* Load in initial angles and angular rates */
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
                   &G->Ang[0],&G->Ang[1],&G->Ang[2],junk,&newline);
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
                   &G->AngRate[0],&G->AngRate[1],&G->AngRate[2],junk,&newline);
            for(k=0;k<3;k++) {
               G->Ang[k] *= D2R;
               G->AngRate[k] *= D2R;
            }
            /* Load in initial displacements and rates */
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
                  &G->Pos[0],&G->Pos[1],&G->Pos[2],junk,&newline);
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
                  &G->PosRate[0],&G->PosRate[1],&G->PosRate[2],junk,&newline);

            fscanf(infile,"%lf %lf %lf %ld %[^\n] %[\n]",
                   &Ang1,&Ang2,&Ang3,&Seq,junk,&newline);
            A2C(Seq,Ang1*D2R,Ang2*D2R,Ang3*D2R,G->CGiBi);
            fscanf(infile,"%lf %lf %lf %ld %[^\n] %[\n]",
                  &Ang1,&Ang2,&Ang3,&Seq,junk,&newline);
            A2C(Seq,Ang1*D2R,Ang2*D2R,Ang3*D2R,G->CBoGo);

            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
                   &pIn[0],&pIn[1],&pIn[2],junk,&newline);
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
                   &pOut[0],&pOut[1],&pOut[2],junk,&newline);

            if (S->RefPt == REFPT_JOINT) {
               for(j=0;j<3;j++) {
                  G->RigidRin[j] = pIn[j];
                  G->RigidRout[j] = pOut[j];
               }
            }
            else {
               for(j=0;j<3;j++) {
                  G->RigidRin[j] = pIn[j] - S->B[Bi].cm[j];
                  G->RigidRout[j] = pOut[j] - S->B[Bo].cm[j];
               }
            }
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
                   &G->RotSpringCoef[0],
                   &G->RotSpringCoef[1],
                   &G->RotSpringCoef[2],
                   junk,&newline);
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
                   &G->RotDampCoef[0],
                   &G->RotDampCoef[1],
                   &G->RotDampCoef[2],
                   junk,&newline);
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
                   &G->TrnSpringCoef[0],
                   &G->TrnSpringCoef[1],
                   &G->TrnSpringCoef[2],
                   junk,&newline);
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
                   &G->TrnDampCoef[0],
                   &G->TrnDampCoef[1],
                   &G->TrnDampCoef[2],
                   junk,&newline);
         }
      }

/* .. Wheel parameters */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%ld %[^\n] %[\n]",&S->Nw,junk,&newline);
      S->Whl = (struct WhlType *) calloc(S->Nw,sizeof(struct WhlType));
      if (S->Nw == 0) {
         for(i=0;i<8;i++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      }
      else {
         for(Iw=0;Iw<S->Nw;Iw++) {
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&S->Whl[Iw].H,junk,&newline);
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
                &S->Whl[Iw].A[0],&S->Whl[Iw].A[1],
                &S->Whl[Iw].A[2],junk,&newline);
            UNITV(S->Whl[Iw].A);
            PerpBasis(S->Whl[Iw].A,S->Whl[Iw].Uhat,S->Whl[Iw].Vhat);
            fscanf(infile,"%lf %lf %[^\n] %[\n]",
                &S->Whl[Iw].Tmax,&S->Whl[Iw].Hmax,junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&S->Whl[Iw].J,junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&S->Whl[Iw].Ks,junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&S->Whl[Iw].Kd,junk,&newline);
            fscanf(infile,"%ld %[^\n] %[\n]",&S->Whl[Iw].FlexNode,junk,&newline);
            /* Convert from g-cm and g-cm^2 to kg-m and kg-m^2 */
            S->Whl[Iw].Ks *= 1.0E-5;
            S->Whl[Iw].Kd *= 1.0E-7;
         }
      }

/* .. MTB parameters */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%ld %[^\n] %[\n]",&S->Nmtb,junk,&newline);
      S->MTB = (struct MTBType *) calloc(S->Nmtb,sizeof(struct MTBType));
      if (S->Nmtb == 0) {
         for(i=0;i<4;i++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      }
      else {
         for(Im=0;Im<S->Nmtb;Im++) {
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%lf  %[^\n] %[\n]",
               &S->MTB[Im].Mmax,junk,&newline);
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
               &S->MTB[Im].A[0],&S->MTB[Im].A[1],&S->MTB[Im].A[2],
                   junk,&newline);
            UNITV(S->MTB[Im].A);
            fscanf(infile,"%ld %[^\n] %[\n]",&S->MTB[Im].FlexNode,junk,&newline);
         }
      }

/* .. Thruster parameters */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%ld %[^\n] %[\n]",&S->Nthr,junk,&newline);
      S->Thr = (struct ThrType *) calloc(S->Nthr,sizeof(struct ThrType));
      if (S->Nthr == 0) {
         for(i=0;i<5;i++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      }
      else {
         for(It=0;It<S->Nthr;It++) {
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&S->Thr[It].Fmax,
                   junk,&newline);
            fscanf(infile,"%ld %lf %lf %lf %[^\n] %[\n]",
                   &S->Thr[It].Body,
                   &S->Thr[It].A[0],
                   &S->Thr[It].A[1],
                   &S->Thr[It].A[2],junk,&newline);
            UNITV(S->Thr[It].A);
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
                   &S->Thr[It].PosB[0],
                   &S->Thr[It].PosB[1],
                   &S->Thr[It].PosB[2],junk,&newline);
            fscanf(infile,"%ld %[^\n] %[\n]",&S->Thr[It].FlexNode,junk,&newline);
         }
      }

/* .. Gyro Parameters */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%ld %[^\n] %[\n]",&S->Ngyro,junk,&newline);
      S->Gyro = (struct GyroType *) calloc(S->Ngyro,sizeof(struct GyroType));
      if (S->Ngyro == 0) {
         for(i=0;i<11;i++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      }
      else {
         for(Ig=0;Ig<S->Ngyro;Ig++) {
            Gyro = &S->Gyro[Ig];
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&Gyro->SampleTime,junk,&newline);
            Gyro->MaxCounter = (long) (Gyro->SampleTime/DTSIM+0.5);
            if (Gyro->MaxCounter < 1) {
               Gyro->MaxCounter = 1;
               Gyro->SampleTime = DTSIM;
               printf("Info:  Gyro[%ld].SampleTime was smaller than DTSIM.  It has been adjusted to be DTSIM.\n",Ig);
            }
            Gyro->SampleCounter = Gyro->MaxCounter;
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
               &Gyro->Axis[0],&Gyro->Axis[1],&Gyro->Axis[2],junk,&newline);
            UNITV(Gyro->Axis);
            fscanf(infile,"%lf %[^\n] %[\n]",&Gyro->MaxRate,junk,&newline);
            Gyro->MaxRate *= D2R;
            fscanf(infile,"%lf %[^\n] %[\n]",&Gyro->Scale,junk,&newline);
            Gyro->Scale = 1.0+1.0E-6*Gyro->Scale;
            fscanf(infile,"%lf %[^\n] %[\n]",&Gyro->Quant,junk,&newline);
            Gyro->Quant *= D2R/3600.0;
            fscanf(infile,"%lf %[^\n] %[\n]",&Gyro->SigV,junk,&newline);
            Gyro->SigV *= D2R/60.0; /* from deg/rt-hr to rad/rt-sec */
            fscanf(infile,"%lf %lf %[^\n] %[\n]",&Gyro->SigU,&BiasTime,junk,&newline);
            Gyro->SigU *= D2R/3600.0/sqrt(BiasTime*3600.0);
            fscanf(infile,"%lf %[^\n] %[\n]",&Gyro->SigE,junk,&newline);
            Gyro->SigE *= D2R/3600.0;
            fscanf(infile,"%lf %[^\n] %[\n]",&Gyro->Bias,junk,&newline);
            Gyro->Bias *= D2R/3600.0;
            fscanf(infile,"%ld %[^\n] %[\n]",&Gyro->FlexNode,junk,&newline);
            
            Gyro->BiasStabCoef = Gyro->SigU*sqrt(Gyro->SampleTime);
            Gyro->ARWCoef = sqrt(Gyro->SigV*Gyro->SigV/Gyro->SampleTime 
                               + Gyro->SigU*Gyro->SigU*Gyro->SampleTime/12.0);
            Gyro->AngNoiseCoef = Gyro->SigE/sqrt(Gyro->SampleTime);
            Gyro->CorrCoef = 1.0-Gyro->SampleTime/(BiasTime*3600.0);
            Gyro->Angle = 0.0;
         }
      }
      
/* .. Magnetometer Parameters */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%ld %[^\n] %[\n]",&S->Nmag,junk,&newline);
      S->MAG = (struct MagnetometerType *) calloc(S->Nmag,sizeof(struct MagnetometerType));
      if (S->Nmag == 0) {
         for(i=0;i<8;i++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      }
      else {
         for(Im=0;Im<S->Nmag;Im++) {
            MAG = &S->MAG[Im];
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&MAG->SampleTime,junk,&newline);
            MAG->MaxCounter = (long) (MAG->SampleTime/DTSIM+0.5);
            if (MAG->MaxCounter < 1) {
               MAG->MaxCounter = 1;
               MAG->SampleTime = DTSIM;
               printf("Info:  MAG[%ld].SampleTime was smaller than DTSIM.  It has been adjusted to be DTSIM.\n",Im);
            }
            MAG->SampleCounter = MAG->MaxCounter;
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
               &MAG->Axis[0],&MAG->Axis[1],&MAG->Axis[2],junk,&newline);
            UNITV(MAG->Axis);
            fscanf(infile,"%lf %[^\n] %[\n]",&MAG->Saturation,junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&MAG->Scale,junk,&newline);
            MAG->Scale = 1.0+1.0E-6*MAG->Scale;
            fscanf(infile,"%lf %[^\n] %[\n]",&MAG->Quant,junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&MAG->Noise,junk,&newline);
            fscanf(infile,"%ld %[^\n] %[\n]",&MAG->FlexNode,junk,&newline);
         }
      }
      
/* .. Coarse Sun Sensors */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%ld %[^\n] %[\n]",&S->Ncss,junk,&newline);
      S->CSS = (struct CssType *) calloc(S->Ncss,sizeof(struct CssType));
      if (S->Ncss == 0) {
         for(i=0;i<7;i++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      }
      else {
         for(Ic=0;Ic<S->Ncss;Ic++) {
            CSS = &S->CSS[Ic];
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&CSS->SampleTime,junk,&newline);
            CSS->MaxCounter = (long) (CSS->SampleTime/DTSIM+0.5);
            if (CSS->MaxCounter < 1) {
               CSS->MaxCounter = 1;
               CSS->SampleTime = DTSIM;
               printf("Info:  CSS[%ld].SampleTime was smaller than DTSIM.  It has been adjusted to be DTSIM.\n",Ig);
            }
            CSS->SampleCounter = CSS->MaxCounter;
            fscanf(infile,"%ld  %lf %lf %lf %[^\n] %[\n]",
               &CSS->Body,&CSS->Axis[0],&CSS->Axis[1],&CSS->Axis[2],junk,&newline);
            UNITV(CSS->Axis);
            fscanf(infile,"%lf %[^\n] %[\n]",&CSS->FovHalfAng,junk,&newline);
            CSS->FovHalfAng *= D2R;
            CSS->CosFov = cos(CSS->FovHalfAng);
            fscanf(infile,"%lf %[^\n] %[\n]",&CSS->Scale,junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&CSS->Quant,junk,&newline);
            fscanf(infile,"%ld %[^\n] %[\n]",&CSS->FlexNode,junk,&newline);
         }
      }

/* .. Fine Sun Sensors */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%ld %[^\n] %[\n]",&S->Nfss,junk,&newline);
      S->FSS = (struct FssType *) calloc(S->Nfss,sizeof(struct FssType));
      if (S->Nfss == 0) {
         for(i=0;i<7;i++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      }
      else {
         for(Ifss=0;Ifss<S->Nfss;Ifss++) {
            FSS = &S->FSS[Ifss];
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&FSS->SampleTime,junk,&newline);
            FSS->MaxCounter = (long) (FSS->SampleTime/DTSIM+0.5);
            if (FSS->MaxCounter < 1) {
               FSS->MaxCounter = 1;
               FSS->SampleTime = DTSIM;
               printf("Info:  FSS[%ld].SampleTime was smaller than DTSIM.  It has been adjusted to be DTSIM.\n",Ig);
            }
            FSS->SampleCounter = FSS->MaxCounter;
            fscanf(infile,"%lf %lf %lf %ld %[^\n] %[\n]",
               &Ang1,&Ang2,&Ang3,&Seq,junk,&newline);
            A2C(Seq,Ang1*D2R,Ang2*D2R,Ang3*D2R,FSS->CB);
            C2Q(FSS->CB,FSS->qb);
            fscanf(infile,"%lf %lf %[^\n] %[\n]",
               &FSS->FovHalfAng[0],&FSS->FovHalfAng[1],junk,&newline);
            for(i=0;i<2;i++) {
               FSS->FovHalfAng[i] *= 0.5*D2R;
            }
            fscanf(infile,"%lf %[^\n] %[\n]",&FSS->NEA,junk,&newline);
            FSS->NEA *= D2R;
            fscanf(infile,"%lf %[^\n] %[\n]",&FSS->Quant,junk,&newline);
            FSS->Quant *= D2R;
            fscanf(infile,"%ld %[^\n] %[\n]",&FSS->FlexNode,junk,&newline);
         }
      }
      
/* .. Star Trackers */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%ld %[^\n] %[\n]",&S->Nst,junk,&newline);
      S->ST = (struct StarTrackerType *) calloc(S->Nst,sizeof(struct StarTrackerType));
      if (S->Nst == 0) {
         for(i=0;i<7;i++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      }
      else {
         for(Ist=0;Ist<S->Nst;Ist++) {
            ST = &S->ST[Ist];
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&ST->SampleTime,junk,&newline);
            ST->MaxCounter = (long) (ST->SampleTime/DTSIM+0.5);
            if (ST->MaxCounter < 1) {
               ST->MaxCounter = 1;
               ST->SampleTime = DTSIM;
               printf("Info:  ST[%ld].SampleTime was smaller than DTSIM.  It has been adjusted to be DTSIM.\n",Ig);
            }
            ST->SampleCounter = ST->MaxCounter;
            fscanf(infile,"%lf %lf %lf %ld %[^\n] %[\n]",
               &Ang1,&Ang2,&Ang3,&Seq,junk,&newline);
            A2C(Seq,Ang1*D2R,Ang2*D2R,Ang3*D2R,ST->CB);
            C2Q(ST->CB,ST->qb);
            fscanf(infile,"%lf %lf %[^\n] %[\n]",
               &ST->FovHalfAng[0],&ST->FovHalfAng[1],junk,&newline);
            for(i=0;i<2;i++) {
               ST->FovHalfAng[i] *= 0.5*D2R;
               ST->CosFov[i] = cos(ST->FovHalfAng[i]);
            }
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
               &ST->SunExclAng,&ST->EarthExclAng,&ST->MoonExclAng,junk,&newline);
            ST->SunExclAng *= D2R;
            ST->EarthExclAng *= D2R;
            ST->MoonExclAng *= D2R;
            ST->CosSunExclAng = cos(ST->SunExclAng);
            ST->CosEarthExclAng = cos(ST->EarthExclAng);
            ST->CosMoonExclAng = cos(ST->MoonExclAng);
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
               &ST->NEA[0],&ST->NEA[1],&ST->NEA[2],junk,&newline);
            for(i=0;i<3;i++) ST->NEA[i] *= D2R/3600.0;
            fscanf(infile,"%ld %[^\n] %[\n]",
               &ST->FlexNode,junk,&newline);
         }
      }
      
/* .. GPS Sensors */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%ld %[^\n] %[\n]",&S->Ngps,junk,&newline);
      S->GPS = (struct GpsType *) calloc(S->Ngps,sizeof(struct GpsType));
      if (S->Ngps == 0) {
         for(i=0;i<6;i++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      }
      else {
         for(Ig=0;Ig<S->Ngps;Ig++) {
            GPS = &S->GPS[Ig];
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&GPS->SampleTime,junk,&newline);
            GPS->MaxCounter = (long) (GPS->SampleTime/DTSIM+0.5);
            if (GPS->MaxCounter < 1) {
               GPS->MaxCounter = 1;
               GPS->SampleTime = DTSIM;
               printf("Info:  GPS[%ld].SampleTime was smaller than DTSIM.  It has been adjusted to be DTSIM.\n",Ig);
            }
            GPS->SampleCounter = GPS->MaxCounter;
            fscanf(infile,"%lf %[^\n] %[\n]",&GPS->PosNoise,junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&GPS->VelNoise,junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&GPS->TimeNoise,junk,&newline);
            fscanf(infile,"%ld %[^\n] %[\n]",&GPS->FlexNode,junk,&newline);
         }
      }
      
/* .. Accelerometers */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%ld %[^\n] %[\n]",&S->Nacc,junk,&newline);
      S->Accel = (struct AccelType *) calloc(S->Nacc,sizeof(struct AccelType));
      if (S->Nacc == 0) {
         for(i=0;i<11;i++) fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      }
      else {
         for(Ia=0;Ia<S->Nacc;Ia++) {
            Accel = &S->Accel[Ia];
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&Accel->SampleTime,junk,&newline);
            Accel->MaxCounter = (long) (Accel->SampleTime/DTSIM+0.5);
            if (Accel->MaxCounter < 1) {
               Accel->MaxCounter = 1;
               Accel->SampleTime = DTSIM;
              
               printf("Info:  Accel[%ld].SampleTime was smaller than DTSIM.  It has been adjusted to be DTSIM.\n",Ia);
            }
            Accel->SampleCounter = Accel->MaxCounter;
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
               &Accel->PosB[0],&Accel->PosB[1],&Accel->PosB[2],junk,&newline);
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
               &Accel->Axis[0],&Accel->Axis[1],&Accel->Axis[2],junk,&newline);
            UNITV(Accel->Axis);
            fscanf(infile,"%lf %[^\n] %[\n]",&Accel->MaxAcc,junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&Accel->Scale,junk,&newline);
            Accel->Scale = 1.0+1.0E-6*Accel->Scale;
            fscanf(infile,"%lf %[^\n] %[\n]",&Accel->Quant,junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&Accel->SigV,junk,&newline);
            Accel->SigV /= 60.0; /* from m/s/rt-hr to m/s/rt-sec */
            fscanf(infile,"%lf %lf %[^\n] %[\n]",&Accel->SigU,&BiasTime,junk,&newline);
            Accel->SigU /= sqrt(BiasTime*3600.0);
            fscanf(infile,"%lf %[^\n] %[\n]",&Accel->SigE,junk,&newline);
            fscanf(infile,"%lf %[^\n] %[\n]",&Accel->Bias,junk,&newline);
            fscanf(infile,"%ld %[^\n] %[\n]",&Accel->FlexNode,junk,&newline);
            Accel->BiasStabCoef = Accel->SigU*sqrt(Accel->SampleTime);
            Accel->DVRWCoef = sqrt(Accel->SigV*Accel->SigV/Accel->SampleTime 
                               + Accel->SigU*Accel->SigU*Accel->SampleTime/12.0);
            Accel->DVNoiseCoef = Accel->SigE/sqrt(Accel->SampleTime);
            Accel->CorrCoef = 1.0-Accel->SampleTime/(BiasTime*3600.0);
            Accel->DV = 0.0;
         }
      }
      fclose(infile);
}


/* #ifdef __cplusplus
** }
** #endif
*/
