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
#include "42config/42config.h"
#include <ctype.h>
#include <libfyaml.h>

/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

/**********************************************************************/

/**********************************************************************/
void EchoDyn(struct SCType *S)
{
      FILE *outfile;
      char OutFileName[80];
      struct DynType *D;
      struct BodyType *B;
      struct JointType *G;
      long i,j,Ib,Ig,Nf;

      sprintf(OutFileName,"Dyn%02ld.42",S->ID);
      outfile = FileOpen(InOutPath,OutFileName,"w");

/* .. SC Structure */
      fprintf(outfile,"Dynamics Check for SC[%ld]\n\n",S->ID);
      fprintf(outfile,"Nb: %2ld   Ng: %2ld\n",S->Nb,S->Ng);
      fprintf(outfile,"Mass:  %lf\n",S->mass);
      fprintf(outfile,"cm:  %lf %lf %lf\n",S->cm[0],S->cm[1],S->cm[2]);
      fprintf(outfile,"I  :  %lf  %lf  %lf \n     %lf  %lf  %lf \n      %lf %lf  %lf\n",
         S->I[0][0],S->I[0][1],S->I[0][2],
         S->I[1][0],S->I[1][1],S->I[1][2],
         S->I[2][0],S->I[2][1],S->I[2][2]);
      fprintf(outfile,"PosR:  %lf %lf %lf\n",
         S->PosR[0],S->PosR[1],S->PosR[2]);
      fprintf(outfile,"VelR:  %lf %lf %lf\n\n",
         S->VelR[0],S->VelR[1],S->VelR[2]);

/* .. Dyn Structure */
      D = &S->Dyn;
      fprintf(outfile,"Dyn Structure\n-------------\n");
      fprintf(outfile,"Nu:  %ld   Nx:  %ld  Nf:  %ld\n",D->Nu,D->Nx,D->Nf);
      for(i=0;i<D->Nu;i++) fprintf(outfile,"u[%02ld]: %lf\n",i,D->u[i]);
      fprintf(outfile,"\n");
      for(i=0;i<D->Nx;i++) fprintf(outfile,"x[%02ld]: %lf\n",i,D->x[i]);
      fprintf(outfile,"\n");
      for(i=0;i<D->Nf;i++) fprintf(outfile,"[%02ld] uf: %lf   xf: %lf\n",
         i,D->uf[i],D->xf[i]);
      fprintf(outfile,"\n\n");

/* .. Body Structures */
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         fprintf(outfile,"Body Structure [%02ld]\n-------------------\n",Ib);
         fprintf(outfile,"Mass:  %lf\n",B->mass);
         fprintf(outfile,"cm:  %lf %lf %lf\n",B->cm[0],B->cm[1],B->cm[2]);
         fprintf(outfile,"c:   %lf %lf %lf\n",B->c[0],B->c[1],B->c[2]);
         fprintf(outfile,"I:  %lf %lf %lf\n     %lf %lf %lf\n   %lf %lf %lf\n",
            B->I[0][0],B->I[0][1],B->I[0][2],
            B->I[1][0],B->I[1][1],B->I[1][2],
            B->I[2][0],B->I[2][1],B->I[2][2]);
         fprintf(outfile,"wn:  %lf %lf %lf\n",
            B->wn[0],B->wn[1],B->wn[2]);
         fprintf(outfile,"qn:  %lf %lf %lf %lf\n",
            B->qn[0],B->qn[1],B->qn[2],B->qn[3]);
         fprintf(outfile,"vn:  %lf %lf %lf\n",
            B->vn[0],B->vn[1],B->vn[2]);
         fprintf(outfile,"pn:  %lf %lf %lf\n\n",
            B->pn[0],B->pn[1],B->pn[2]);
         fprintf(outfile,"CN:\n");
         fprintf(outfile,"%lf %lf %lf\n",
            B->CN[0][0],B->CN[0][1],B->CN[0][2]);
         fprintf(outfile,"%lf %lf %lf\n",
            B->CN[1][0],B->CN[1][1],B->CN[1][2]);
         fprintf(outfile,"%lf %lf %lf\n",
            B->CN[2][0],B->CN[2][1],B->CN[2][2]);
         fprintf(outfile,"Nf: %ld   f0: %ld\n",B->Nf,B->f0);
         fprintf(outfile,"Mf:\n");
         for(i=0;i<B->Nf;i++) {
            for(j=0;j<B->Nf;j++) fprintf(outfile,"  %lf",B->Mf[i][j]);
            fprintf(outfile,"\n");
         }
         fprintf(outfile,"\n");
         fprintf(outfile,"Kf:\n");
         for(i=0;i<B->Nf;i++) {
            for(j=0;j<B->Nf;j++) fprintf(outfile,"  %lf",B->Kf[i][j]);
            fprintf(outfile,"\n");
         }
         fprintf(outfile,"\n");
         fprintf(outfile,"Cf:\n");
         for(i=0;i<B->Nf;i++) {
            for(j=0;j<B->Nf;j++) fprintf(outfile,"  %lf",B->Cf[i][j]);
            fprintf(outfile,"\n");
         }
         fprintf(outfile,"\n");
         fprintf(outfile,"Pf:\n");
         for(i=0;i<3;i++) {
            for(j=0;j<B->Nf;j++) fprintf(outfile,"  %lf",B->Pf[i][j]);
            fprintf(outfile,"\n");
         }
         fprintf(outfile,"\n");
         fprintf(outfile,"Hf:\n");
         for(i=0;i<3;i++) {
            for(j=0;j<B->Nf;j++) fprintf(outfile,"  %lf",B->Hf[i][j]);
            fprintf(outfile,"\n");
         }
         fprintf(outfile,"\n");
      }
      fprintf(outfile,"\n");

/* .. Joint Structures */
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         fprintf(outfile,"Joint Structure [%02ld]\n---------------\n",Ig);
         fprintf(outfile,"Rotu0: %ld   Rotx0: %ld\n",G->Rotu0,G->Rotx0);
         fprintf(outfile,"Trnu0: %ld   Trnx0: %ld\n",G->Trnu0,G->Trnx0);
         fprintf(outfile,"ang:  %lf %lf %lf\n",
            G->Ang[0],G->Ang[1],G->Ang[2]);
         fprintf(outfile,"rate:  %lf %lf %lf\n",
            G->AngRate[0],G->AngRate[1],G->AngRate[2]);
         fprintf(outfile,"COI:\n");
         fprintf(outfile,"%lf %lf %lf\n",
            G->COI[0][0],G->COI[0][1],G->COI[0][2]);
         fprintf(outfile,"%lf %lf %lf\n",
            G->COI[1][0],G->COI[1][1],G->COI[1][2]);
         fprintf(outfile,"%lf %lf %lf\n",
            G->COI[2][0],G->COI[2][1],G->COI[2][2]);
         fprintf(outfile,"SpringCoef: %lf %lf %lf\n",
            G->RotSpringCoef[0],G->RotSpringCoef[1],G->RotSpringCoef[2]);
         fprintf(outfile,"DampCoef: %lf %lf %lf\n",
            G->RotDampCoef[0],G->RotDampCoef[1],G->RotDampCoef[2]);
         Nf = S->B[G->Bin].Nf;
         fprintf(outfile,"PSIi:\n");
         for(i=0;i<3;i++) {
            for(j=0;j<Nf;j++) fprintf(outfile," %lf",G->PSIi[i][j]);
            fprintf(outfile,"\n");
         }
         fprintf(outfile,"THETAi:\n");
         for(i=0;i<3;i++) {
            for(j=0;j<Nf;j++) fprintf(outfile," %lf",G->THETAi[i][j]);
            fprintf(outfile,"\n");
         }
         Nf = S->B[G->Bout].Nf;
         fprintf(outfile,"PSIo:\n");
         for(i=0;i<3;i++) {
            for(j=0;j<Nf;j++) fprintf(outfile," %lf",G->PSIo[i][j]);
            fprintf(outfile,"\n");
         }
         fprintf(outfile,"THETAo:\n");
         for(i=0;i<3;i++) {
            for(j=0;j<Nf;j++) fprintf(outfile," %lf",G->THETAo[i][j]);
            fprintf(outfile,"\n");
         }
         fprintf(outfile,"\n\n");
      }

      fclose(outfile);
}
/**********************************************************************/

/*********************************************************************/

void InitOrbit(struct OrbitType *O)
{
    size_t lenstr = strlen(O->FileName);
    if (lenstr > 4 && !strncmp(O->FileName + lenstr-4, "yaml", 4))
        InitOrbitYAML(O);
    else
        InitOrbitTxt(O);
}

/**********************************************************************/
void InitRigidDyn(struct SCType *S)
{
      long i,j,Ig,Ia,Jg,Jb,Ibody,Ib,u0,x0,c0,Nu,Nx;
      struct JointType *G;
      struct DynType *D;
      FILE *outfile;
      char filename[80];

      D = &S->Dyn;

/* .. Tree Tables */
      /* Connectivity Table */
      /* Inner body -> -1, Outer body -> +1, else 0 */
      D->ConnectTable = (long **) calloc(S->Nb,sizeof(long *));
      for(i=0;i<S->Nb;i++)
         D->ConnectTable[i] = (long *) calloc(S->Ng,sizeof(long));
      for(Ib=0;Ib<S->Nb;Ib++) {
         for(Ig=0;Ig<S->Ng;Ig++) {
            D->ConnectTable[Ib][Ig] = 0;
         }
      }
      for(Ig=0;Ig<S->Ng;Ig++) {
         D->ConnectTable[S->G[Ig].Bin][Ig] = -1;
         D->ConnectTable[S->G[Ig].Bout][Ig] =  1;
      }

      /* Joint Path Table */
      /* Joint in path -> 1, else 0 */
      D->JointPathTable = (struct JointPathTableType **)
         calloc(S->Nb,sizeof(struct JointPathTableType *));
      for(i=0;i<S->Nb;i++)
         D->JointPathTable[i] = (struct JointPathTableType *)
            calloc(S->Ng,sizeof(struct JointPathTableType));
      for(Ibody=0;Ibody<S->Nb;Ibody++) {
         Ib = Ibody;
         while(Ib > 0) {
            Ig = S->B[Ib].Gin;
            D->JointPathTable[Ibody][Ig].InPath = 1;
            Ib = S->G[Ig].Bin;
         }
      }
      /* Body Path Table */
      /* Body in path -> 1, else 0 */
      D->BodyPathTable = (struct BodyPathTableType **)
         calloc(S->Nb,sizeof(struct BodyPathTableType *));
      for(i=0;i<S->Nb;i++)
         D->BodyPathTable[i] = (struct BodyPathTableType *)
            calloc(S->Nb,sizeof(struct BodyPathTableType));
      for(Ibody=0;Ibody<S->Nb;Ibody++) {
         Ib = Ibody;
         while(Ib > 0) {
            Ig = 0;
            while(S->G[Ig].Bout != Ib) Ig++;
            D->BodyPathTable[Ibody][Ib].InPath = 1;
            Ib = S->G[Ig].Bin;
         }
         D->BodyPathTable[Ibody][0].InPath = 1;
      }
      
      /* Find ancestor joint list for each joint */
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         Ib = S->G[Ig].Bin;
         G->Nanc = 0;
         while(Ib > 0) {
            G->Nanc++;
            Jg = S->B[Ib].Gin;
            Ib = S->G[Jg].Bin;
         }
         G->Anc = (long *) calloc(G->Nanc,sizeof(long));
         Ib = S->G[Ig].Bin;
         Ia = 0;
         while(Ib > 0) {
            Jg = S->B[Ib].Gin;
            G->Anc[Ia] = Jg;
            Ib = S->G[Jg].Bin;
            Ia++;
         }
      }

      /* Determine sizes of state vectors */
      D->Nu = 6;
      D->Nx = 7;
      u0 = 3;
      x0 = 4;
      D->Nc = 0;
      c0 = 0;
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         if (G->IsSpherical) {
            G->RotDOF = 3;
            G->RotSeq = 123;
            D->Nu += 3;
            D->Nx += 4;
            G->Rotu0 = u0;
            G->Rotx0 = x0;
            u0 += 3;
            x0 += 4;
         }
         else { /* Is Gimbal */
            D->Nu += G->RotDOF;
            D->Nx += G->RotDOF;
            G->Rotu0 = u0;
            G->Rotx0 = x0;
            u0 += G->RotDOF;
            x0 += G->RotDOF;
            G->Rotc0 = c0;
            c0 += 3-G->RotDOF;
         }
         D->Nu += G->TrnDOF;
         D->Nx += G->TrnDOF;
         G->Trnu0 = u0;
         G->Trnx0 = x0;
         u0 += G->TrnDOF;
         x0 += G->TrnDOF;
         G->Trnc0 = c0;
         c0 += 3-G->TrnDOF;
      }

/* .. Allocate Workspace */
      D->PAngVel = CreateMatrix(3*S->Nb,D->Nu);
      D->IPAngVel = CreateMatrix(3*S->Nb,D->Nu);
      D->PVel = CreateMatrix(3*S->Nb,D->Nu);
      D->mPVel = CreateMatrix(3*S->Nb,D->Nu);
      D->BodyTrq = (double *) calloc(3*S->Nb,sizeof(double));
      D->BodyFrc = (double *) calloc(3*S->Nb,sizeof(double));

      D->u = (double *) calloc(D->Nu,sizeof(double));
      D->uu = (double *) calloc(D->Nu,sizeof(double));
      D->du = (double *) calloc(D->Nu,sizeof(double));
      D->udot = (double *) calloc(D->Nu,sizeof(double));
      D->x = (double *) calloc(D->Nx,sizeof(double));
      D->xx = (double *) calloc(D->Nx,sizeof(double));
      D->dx = (double *) calloc(D->Nx,sizeof(double));
      D->xdot = (double *) calloc(D->Nx,sizeof(double));
      D->h = (double *) calloc(S->Nw,sizeof(double));
      D->hh = (double *) calloc(S->Nw,sizeof(double));
      D->dh = (double *) calloc(S->Nw,sizeof(double));
      D->hdot = (double *) calloc(S->Nw,sizeof(double));

      for(i=0;i<3;i++) {
         D->PAngVel[i][i] = 1.0;
         for(j=0;j<3;j++) D->IPAngVel[i][j] = S->B[0].I[i][j];
      }
      for(Ib=0;Ib<S->Nb;Ib++) {
         for(i=0;i<3;i++) {
            D->PVel[3*Ib+i][D->Nu-3+i] = 1.0;
            D->mPVel[3*Ib+i][D->Nu-3+i] = S->B[Ib].mass;
         }
      }

      if (S->ConstraintsRequested) {
         D->Nc = 6*S->Nb - D->Nu;
         D->PAngVelc = CreateMatrix(3*S->Nb,D->Nc);
         D->PVelc = CreateMatrix(3*S->Nb,D->Nc);
         D->TotalTrq = (double *) calloc(3*S->Nb,sizeof(double));
         D->TotalFrc = (double *) calloc(3*S->Nb,sizeof(double));
         D->GenConstraintFrc = (double *) calloc(D->Nc,sizeof(double));
      }

      MapJointStatesToStateVector(S);

/* .. Echo tree tables */
      sprintf(filename,"Tree%02ld.42",S->ID);
      outfile = FileOpen(InOutPath,filename,"w");
      fprintf(outfile,"SC %2ld:  Nb = %2ld  Ng = %2ld\n\n",S->ID,S->Nb,S->Ng);
      fprintf(outfile,"Connect Table:\n\n");
      fprintf(outfile,"     ");
      for(Ig=0;Ig<S->Ng;Ig++) fprintf(outfile,"  G[%02ld]",Ig);
      fprintf(outfile,"\n");
      for(Ib=0;Ib<S->Nb;Ib++) {
         fprintf(outfile,"B[%02ld]:",Ib);
         for(Ig=0;Ig<S->Ng;Ig++) {
            fprintf(outfile,"  %3ld  ",D->ConnectTable[Ib][Ig]);
         }
         fprintf(outfile,"\n");
      }
      fprintf(outfile,"\n\n\nJoint Path Table:\n\n");
      fprintf(outfile,"     ");
      for(Ig=0;Ig<S->Ng;Ig++) fprintf(outfile,"  G[%02ld]",Ig);
      fprintf(outfile,"\n");
      for(Ib=0;Ib<S->Nb;Ib++) {
         fprintf(outfile,"B[%02ld]:",Ib);
         for(Ig=0;Ig<S->Ng;Ig++) {
            fprintf(outfile,"  %3ld  ",D->JointPathTable[Ib][Ig].InPath);
         }
         fprintf(outfile,"\n");
      }
      fprintf(outfile,"\n\n\nBody Path Table:\n\n");
      fprintf(outfile,"     ");
      for(Jb=0;Jb<S->Nb;Jb++) fprintf(outfile,"  B[%02ld]",Jb);
      fprintf(outfile,"\n");
      for(Ib=0;Ib<S->Nb;Ib++) {
         fprintf(outfile,"B[%02ld]:",Ib);
         for(Jb=0;Jb<S->Nb;Jb++) {
            fprintf(outfile,"  %3ld  ",D->BodyPathTable[Ib][Jb].InPath);
         }
         fprintf(outfile,"\n");
      }

/* .. Echo State Vector */
      fprintf(outfile,"\n\n\nState Vector Map:\n\n");
      fprintf(outfile,"Body/Joint   RotDOF   TrnDOF   RotSeq   TrnSeq       u[]      x[]\n");
      fprintf(outfile,"  B[00]        wn       ---      123      ---       00-02    00-03\n");
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         Nu = G->RotDOF+G->TrnDOF;
         Nx = (G->IsSpherical ? Nu+1 : Nu);
         fprintf(outfile,"  G[%02ld]       %3ld      %3ld       %3ld      %3ld       %02ld-%02ld    %02ld-%02ld\n",
            Ig,G->RotDOF,G->TrnDOF,G->RotSeq,G->TrnSeq,G->Rotu0,G->Rotu0+Nu-1,G->Rotx0,G->Rotx0+Nx-1);
      }
      fprintf(outfile,"  B[00]       ---       vn       ---      123       %02ld-%02ld    %02ld-%02ld\n",
      D->Nu-3,D->Nu-1,D->Nx-3,D->Nx-1);

/* .. DOF/Constraint Map */
      fprintf(outfile,"\n\n\nMap all 6*Nb potential DOF Axes into DOFs or Constraints\n\n");
      fprintf(outfile,"*****************************************************************\n");
      fprintf(outfile,"Body 00:   RotSeq = 123   TrnSeq = 123\n");
      fprintf(outfile,"                                Col in   Col in       Col in\n");
      fprintf(outfile,"Axis      F/C    u[]  x[]       u%02ld.42   x%02ld.42   Constraint%02ld.42\n",S->ID,S->ID,S->ID);
      fprintf(outfile,"-----------------------------------------------------------------\n");
      fprintf(outfile,"Rot1       F      00   00         01       01           --\n");
      fprintf(outfile,"Rot2       F      01   01         02       02           --\n");
      fprintf(outfile,"Rot3       F      02   02         03       03           --\n");
      fprintf(outfile,"(Sph)      -      --   03         --       04           --\n\n");
      fprintf(outfile,"Trn1       F      %02ld   %02ld         %02ld       %02ld           --\n",D->Nu-3,D->Nx-3,D->Nu-2,D->Nx-2);
      fprintf(outfile,"Trn2       F      %02ld   %02ld         %02ld       %02ld           --\n",D->Nu-2,D->Nx-2,D->Nu-1,D->Nx-1);
      fprintf(outfile,"Trn3       F      %02ld   %02ld         %02ld       %02ld           --\n",D->Nu-1,D->Nx-1,D->Nu,D->Nx);
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         fprintf(outfile,"*****************************************************************\n");
         fprintf(outfile,"Joint %02ld:   RotSeq = %3ld   TrnSeq = %3ld\n",Ig,G->RotSeq,G->TrnSeq);
         fprintf(outfile,"                                Col in   Col in       Col in\n");
         fprintf(outfile,"Axis      F/C    u[]  x[]       u%02ld.42   x%02ld.42   Constraint%02ld.42\n",S->ID,S->ID,S->ID);
         fprintf(outfile,"-----------------------------------------------------------------\n");
         for(i=0;i<G->RotDOF;i++) {
            fprintf(outfile,"Rot%ld       F      %02ld   %02ld         %02ld       %02ld           --\n",
               i+1,G->Rotu0+i,G->Rotx0+i,G->Rotu0+i+1,G->Rotx0+i+1);
         }
         for(i=0;i<3-G->RotDOF;i++) {
            fprintf(outfile,"Rot%ld       C      --   --         --       --           %02ld\n",G->RotDOF+i+1,G->Rotc0+i+1);
         }
         if (G->IsSpherical) {
            fprintf(outfile,"(Sph)      -      --   %02ld         --       %02ld           --\n",G->Rotx0+3,G->Rotx0+4);
         }
         fprintf(outfile,"\n");
         for(i=0;i<G->TrnDOF;i++) {
            fprintf(outfile,"Trn%ld       F      %02ld   %02ld         %02ld       %02ld           --\n",
               i,G->Trnu0+i+1,G->Trnx0+i,G->Trnu0+i+1,G->Trnx0+i+1);
         }
         for(i=0;i<3-G->TrnDOF;i++) {
            fprintf(outfile,"Trn%ld       C      --   --         --       --           %02ld\n",G->TrnDOF+i+1,G->Trnc0+i+1);
         }
      }
      fclose(outfile);
}
/**********************************************************************/
void InitFlexModes(struct SCType *S)
{
      FILE *infile;
      struct DynType *D;
      struct BodyType *B;
      struct JointType *G;
      long Ib,Ig,If,Im,Jm,Ia,In;
      long Nnonzero,Iz;
      long i,j;
      double value,wf;
      char junk[80],newline;
      double ***L,****N;
      struct FlexNodeType *FN;

      D = &S->Dyn;
      D->Nf = 0;

/* .. First pass through all flex input files to allocate matrices */
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         B->Nf = 0;
         if (strcmp(B->FlexFileName,"NONE")) {
            infile = FileOpen(InOutPath,B->FlexFileName,"r");
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            /* Number of Flex Modes */
            fscanf(infile,"%ld %[^\n] %[\n]",&B->Nf,junk,&newline);
            B->f0 = D->Nf;
            D->Nf += B->Nf;
            /* Allocate matrices */
            B->xi = (double *) calloc(B->Nf,sizeof(double));
            B->eta = (double *) calloc(B->Nf,sizeof(double));
            B->Mf = CreateMatrix(B->Nf,B->Nf);
            B->Kf = CreateMatrix(B->Nf,B->Nf);
            B->Cf = CreateMatrix(B->Nf,B->Nf);
            B->Pf = CreateMatrix(3,B->Nf);
            B->Hf = CreateMatrix(3,B->Nf);
            B->CnbP = CreateMatrix(3,B->Nf);
            B->HplusQeta = CreateMatrix(3,B->Nf);
            B->Qxi = CreateMatrix(3,B->Nf);
            B->Rw = CreateMatrix(3,B->Nf);
            B->Swe = CreateMatrix(3,B->Nf);
            /* Allocate higher-order tensors */
            B->Qf = (double ***) calloc(3,sizeof(double **));
            B->Rf = (double ***) calloc(3,sizeof(double **));
            B->Sf = (double ****) calloc(3,sizeof(double ***));
            B->Sw = (double ***) calloc(3,sizeof(double **));
            for(i=0;i<3;i++) {
               B->Qf[i] = CreateMatrix(B->Nf,B->Nf);
               B->Rf[i] = CreateMatrix(B->Nf,3);
               B->Sf[i] = (double ***) calloc(B->Nf,sizeof(double **));
               for(j=0;j<B->Nf;j++) {
                  B->Sf[i][j] = CreateMatrix(B->Nf,3);
               }
               B->Sw[i] = CreateMatrix(B->Nf,B->Nf);
            }
            fclose(infile);
         }
      }
      /* For Joint Nodes */
      for(Ig=0;Ig<S->Ng;Ig++) {
         G = &S->G[Ig];
         B = &S->B[G->Bin];
         if (B->Nf > 0) {
            G->PSIi = CreateMatrix(3,B->Nf);
            G->THETAi = CreateMatrix(3,B->Nf);
         }
         B = &S->B[G->Bout];
         if (B->Nf > 0) {
            G->PSIo = CreateMatrix(3,B->Nf);
            G->THETAo = CreateMatrix(3,B->Nf);
         }
      }
      D->PAngVelf = CreateMatrix(3*S->Nb,D->Nf);
      D->IPAngVelf = CreateMatrix(3*S->Nb,D->Nf);
      D->PVelf = CreateMatrix(3*S->Nb,D->Nf);
      D->mPVelf = CreateMatrix(3*S->Nb,D->Nf);
      D->uf = (double *) calloc(D->Nf,sizeof(double));
      D->uuf = (double *) calloc(D->Nf,sizeof(double));
      D->duf = (double *) calloc(D->Nf,sizeof(double));
      D->ufdot = (double *) calloc(D->Nf,sizeof(double));
      D->xf = (double *) calloc(D->Nf,sizeof(double));
      D->xxf = (double *) calloc(D->Nf,sizeof(double));
      D->dxf = (double *) calloc(D->Nf,sizeof(double));
      D->xfdot = (double *) calloc(D->Nf,sizeof(double));
      D->FlexAcc = (double *) calloc(D->Nf,sizeof(double));
      D->FlexFrc = (double *) calloc(D->Nf,sizeof(double));
      D->FlexInertiaFrc = (double *) calloc(D->Nf,sizeof(double));
      D->Mf = CreateMatrix(D->Nf,D->Nf);
      D->PCPVelf = CreateMatrix(D->Nf,D->Nf);
      D->HplusQetaPAngVelf = CreateMatrix(D->Nf,D->Nf);

/* .. Second pass through all flex files to read in values */
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         if (strcmp(B->FlexFileName,"NONE")) {
            infile = FileOpen(InOutPath,B->FlexFileName,"r");
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);

            /* Initial Modal States x, u */
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            for(If=0;If<B->Nf;If++) {
               fscanf(infile,"%lf %lf %[^\n] %[\n]",
                  &B->eta[If],&B->xi[If],junk,&newline);
               D->xf[B->f0+If] = B->eta[If];
               D->uf[B->f0+If] = B->xi[If];
            }

            /* Analysis Node Locations */
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%ld %[^\n] %[\n]",
               &B->NumFlexNodes,junk,&newline);
            B->FlexNode = (struct FlexNodeType *)
               calloc(B->NumFlexNodes,sizeof(struct FlexNodeType));
            for(In=0;In<B->NumFlexNodes;In++) {
               FN = &B->FlexNode[In];
               fscanf(infile,"%ld %lf %lf %lf %[^\n] %[\n]",
                  &FN->ID,&FN->PosB[0],&FN->PosB[1],&FN->PosB[2],
                  junk,&newline);
            }
            for(In=0;In<B->NumFlexNodes;In++) {
               FN = &B->FlexNode[In];
               FN->PSI = CreateMatrix(3,B->Nf);
               FN->THETA = CreateMatrix(3,B->Nf);
               for(i=0;i<3;i++) {
                  FN->Frc[i] = 0.0;
                  FN->Trq[i] = 0.0;
               }
            }

            /**** Joint Node Mode Shapes ****/
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            /* Non-zero Translation Mode Shape (PSI) Elements */
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%ld %[^\n] %[\n]",&Nnonzero,junk,&newline);
            for(Iz=0;Iz<Nnonzero;Iz++) {
               fscanf(infile,"%ld %ld %ld %lf %[^\n] %[\n]",
                  &Im,&Ig,&Ia,&value,junk,&newline);
               if (Ig >= S->Ng) {
                  printf("Error in InitFlexModes: Joint %ld out of range\n",Ig);
                  exit(1);
               }
               if (Ia > 2) {
                  printf("Error in InitFlexModes (PSI): Axis %ld out of range\n",Ia);
                  exit(1);
               }
               if (Im >= B->Nf) {
                  printf("Error in InitFlexModes (PSI): Flex Mode %ld out of range\n",Im);
                  exit(1);
               }
               G = &S->G[Ig];
               if (Ib == G->Bin) G->PSIi[Ia][Im] = value;
               else if (Ib == G->Bout) G->PSIo[Ia][Im] = value;
               else {
                  printf("Error in InitFlexModes (PSI): Body %ld not connected to Joint %ld\n",Ib,Ig);
                  exit(1);
               }
            }
            /* Non-zero Rotation Mode Shape (THETA) Elements */
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%ld %[^\n] %[\n]",&Nnonzero,junk,&newline);
            for(Iz=0;Iz<Nnonzero;Iz++) {
               fscanf(infile,"%ld %ld %ld %lf %[^\n] %[\n]",
                  &Im,&Ig,&Ia,&value,junk,&newline);
               if (Ig >= S->Ng) {
                  printf("Error in InitFlexModes: Joint %ld out of range\n",Ig);
                  exit(1);
               }
               if (Ia > 2) {
                  printf("Error in InitFlexModes (THETA): Axis %ld out of range\n",Ia);
                  exit(1);
               }
               if (Im >= B->Nf) {
                  printf("Error in InitFlexModes (THETA): Flex Mode %ld out of range\n",Im);
                  exit(1);
               }
               G = &S->G[Ig];
               if (Ib == G->Bin) G->THETAi[Ia][Im] = value;
               else if (Ib == G->Bout) G->THETAo[Ia][Im] = value;
               else {
                  printf("Error in InitFlexModes (THETA): Body %ld not connected to Joint %ld\n",Ib,Ig);
                  exit(1);
               }
            }

            /**** Analysis Node Mode Shapes ****/
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            /* Non-zero Translation Mode Shape (PSI) Elements */
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%ld %[^\n] %[\n]",&Nnonzero,junk,&newline);
            for(Iz=0;Iz<Nnonzero;Iz++) {
               fscanf(infile,"%ld %ld %ld %lf %[^\n] %[\n]",
                  &Im,&In,&Ia,&value,junk,&newline);
               FN = &B->FlexNode[In];
               if (Ia > 2) {
                  printf("Error in InitFlexModes (PSI): Axis %ld out of range\n",Ia);
                  exit(1);
               }
               if (Im >= B->Nf) {
                  printf("Error in InitFlexModes (PSI): Flex Mode %ld out of range\n",Im);
                  exit(1);
               }
               FN->PSI[Ia][Im] = value;
            }

            /* Non-zero Rotation Mode Shape (THETA) Elements */
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%ld %[^\n] %[\n]",&Nnonzero,junk,&newline);
            for(Iz=0;Iz<Nnonzero;Iz++) {
               fscanf(infile,"%ld %ld %ld %lf %[^\n] %[\n]",
                  &Im,&In,&Ia,&value,junk,&newline);
               FN = &B->FlexNode[In];
               if (Ia > 2) {
                  printf("Error in InitFlexModes (THETA): Axis %ld out of range\n",Ia);
                  exit(1);
               }
               if (Im >= B->Nf) {
                  printf("Error in InitFlexModes (THETA): Flex Mode %ld out of range\n",Im);
                  exit(1);
               }
               FN->THETA[Ia][Im] = value;
            }

            /* Non-zero Mass Matrix Elements */
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%ld %[^\n] %[\n]",&Nnonzero,junk,&newline);
            for(Iz=0;Iz<Nnonzero;Iz++) {
               fscanf(infile,"%ld %ld %lf %[^\n] %[\n]",
                  &i,&j,&value,junk,&newline);
               if (i >= B->Nf || j >= B->Nf) {
                  printf("Error in InitFlexModes: Mass Matrix index [%ld][%ld] out of range\n",i,j);
                  exit(1);
               }
               B->Mf[i][j] = value;
            }
            /* Non-zero Stiffness Matrix Elements */
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%ld %[^\n] %[\n]",&Nnonzero,junk,&newline);
            for(Iz=0;Iz<Nnonzero;Iz++) {
               fscanf(infile,"%ld %ld %lf %[^\n] %[\n]",
                  &i,&j,&value,junk,&newline);
               if (i >= B->Nf || j >= B->Nf) {
                  printf("Error in InitFlexModes: Stiffness Matrix index [%ld][%ld] out of range\n",i,j);
                  exit(1);
               }
               B->Kf[i][j] = value;
            }
            /* Non-zero Damping Matrix Elements */
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%ld %[^\n] %[\n]",&Nnonzero,junk,&newline);
            for(Iz=0;Iz<Nnonzero;Iz++) {
               fscanf(infile,"%ld %ld %lf %[^\n] %[\n]",
                  &i,&j,&value,junk,&newline);
               if (i >= B->Nf || j >= B->Nf) {
                  printf("Error in InitFlexModes: Damping Matrix index [%ld][%ld] out of range\n",i,j);
                  exit(1);
               }
               B->Cf[i][j] = value;
            }

            /* Check modal frequencies to make sure DTSIM is small enough */
            for(i=0;i<B->Nf;i++) {
               wf = sqrt(B->Kf[i][i]/B->Mf[i][i]);
               if (Pi/wf < DTSIM) {
                  printf("Oops.  Natural frequency of Flex Mode %ld of Body %ld of SC %ld is too high to be sampled at time step of %lf.\n",i,Ib,S->ID,DTSIM);
                  printf("Suggest setting DTSIM < %lf sec\n",0.2*TwoPi/wf); /* 5 samples/cycle */
                  exit(1);
               }
            }

            /* Linear Momentum Modal Integral, Pf, 3 x Nf */
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            for(Im=0;Im<B->Nf;Im++) {
               fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
                  &B->Pf[0][Im],&B->Pf[1][Im],&B->Pf[2][Im],junk,&newline);
            }
            /* Angular Momentum Modal Integral, Hf, 3 x Nf */
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            for(Im=0;Im<B->Nf;Im++) {
               fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
                  &B->Hf[0][Im],&B->Hf[1][Im],&B->Hf[2][Im],junk,&newline);
            }
            /* Don't trust input file to make Hf, and Pf be zero */
            if (S->RefPt == REFPT_CM) {
               for(Im=0;Im<B->Nf;Im++) {
                  B->Pf[0][Im] = 0.0;
                  B->Pf[1][Im] = 0.0;
                  B->Pf[2][Im] = 0.0;
                  B->Hf[0][Im] = 0.0;
                  B->Hf[1][Im] = 0.0;
                  B->Hf[2][Im] = 0.0;
               }
            }

            /* Non-zero Elements of Linear Modal Integral, L, 3 x 3 x Nf */
            L = (double ***) calloc(3,sizeof(double **));
            for(i=0;i<3;i++) L[i] = CreateMatrix(3,B->Nf);
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%ld %[^\n] %[\n]",&Nnonzero,junk,&newline);
            for(Iz=0;Iz<Nnonzero;Iz++) {
               fscanf(infile,"%ld %ld %ld %lf %[^\n] %[\n]",
                  &i,&j,&Im,&value,junk,&newline);
               if (i >= 3 || j >= 3 || Im >= B->Nf) {
                  printf("Error in InitFlexModes: L index [%ld][%ld][%ld] out of range\n",i,j,Im);
                  exit(1);
               }
               L[i][j][Im] = value;
            }
            /* B->Rf, 3 x Nf x 3 */
            for(i=0;i<B->Nf;i++) {
               B->Rf[0][i][0] = -L[1][1][i]-L[2][2][i];
               B->Rf[1][i][1] = -L[2][2][i]-L[0][0][i];
               B->Rf[2][i][2] = -L[0][0][i]-L[1][1][i];
               B->Rf[0][i][1] = L[1][0][i];
               B->Rf[0][i][2] = L[2][0][i];
               B->Rf[1][i][0] = L[0][1][i];
               B->Rf[1][i][2] = L[2][1][i];
               B->Rf[2][i][0] = L[0][2][i];
               B->Rf[2][i][1] = L[1][2][i];
            }
            for(i=0;i<3;i++) DestroyMatrix(L[i],3);
            free(L);

            /* Non-zero Elements of Angular Modal Integral, N, 3 x 3 x Nf x Nf*/
            N = (double ****) calloc(3,sizeof(double ***));
            for(i=0;i<3;i++) {
               N[i] = (double ***) calloc(3,sizeof(double **));
               for(j=0;j<3;j++) {
                  N[i][j] = CreateMatrix(B->Nf,B->Nf);
               }
            }
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%ld %[^\n] %[\n]",&Nnonzero,junk,&newline);
            for(Iz=0;Iz<Nnonzero;Iz++) {
               fscanf(infile,"%ld %ld %ld %ld %lf %[^\n] %[\n]",
                  &i,&j,&Im,&Jm,&value,junk,&newline);
               if (i >= 3 || j >= 3 || Im >= B->Nf || Jm >= B->Nf) {
                  printf("Error in InitFlexModes: N index [%ld][%ld][%ld][%ld] out of range\n",i,j,Im,Jm);
                  exit(1);
               }
               N[i][j][Im][Jm] = value;
            }
            for(i=0;i<B->Nf;i++) {
               for(j=0;j<B->Nf;j++) {
                  /* B->Qf, 3 x Nf x Nf */
                  B->Qf[0][i][j] = N[2][1][i][j]-N[1][2][i][j];
                  B->Qf[1][i][j] = N[0][2][i][j]-N[2][0][i][j];
                  B->Qf[2][i][j] = N[1][0][i][j]-N[0][1][i][j];
                  /* B->Sf, 3 x Nf x Nf x 3 */
                  B->Sf[0][i][j][0] = -N[1][1][i][j]-N[2][2][i][j];
                  B->Sf[1][i][j][1] = -N[2][2][i][j]-N[0][0][i][j];
                  B->Sf[2][i][j][2] = -N[0][0][i][j]-N[1][1][i][j];
                  B->Sf[0][i][j][1] = N[1][0][i][j];
                  B->Sf[0][i][j][2] = N[2][0][i][j];
                  B->Sf[1][i][j][0] = N[0][1][i][j];
                  B->Sf[1][i][j][2] = N[2][1][i][j];
                  B->Sf[2][i][j][0] = N[0][2][i][j];
                  B->Sf[2][i][j][1] = N[1][2][i][j];
               }
            }
            for(i=0;i<3;i++) {
               for(j=0;j<3;j++) DestroyMatrix(N[i][j],B->Nf);
               free(N[i]);
            }
            free(N);

            fclose(infile);
         }
      }

/* .. Assemble Dyn.Mf */
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         for(i=0;i<B->Nf;i++) {
            for(j=0;j<B->Nf;j++) {
               D->Mf[B->f0+i][B->f0+j] = B->Mf[i][j];
            }
         }
      }

/* .. Find if Mf is Diagonal */
      for(Ib=0;Ib<S->Nb;Ib++) {
         B = &S->B[Ib];
         B->MfIsDiagonal = TRUE;
         for(i=0;i<B->Nf;i++) {
            for(j=0;j<B->Nf;j++) {
               if (i != j && B->Mf[i][j] != 0.0)
                  B->MfIsDiagonal = FALSE;
            }
         }
      }

/* .. Bypass flex computations if FlexActive is FALSE */
      if (!S->FlexActive) D->Nf = 0;
/* .. If FlexActive is TRUE, but no flex modes have been defined... */
      if (S->FlexActive && D->Nf == 0) {
         S->FlexActive = FALSE;
         printf("FlexActive set TRUE, but no flex modes defined.\n");
         printf("   Setting FlexActive to FALSE.\n");
      }
}

/*************************************************/

void InitSpacecraft(struct SCType *S)
{
      int i, j;
      int Iw, Im, It;
      int OldNgeom;
      double psn[3],vsn[3],psl[3],vsl[3],pfl[3],pcmn[3],pcml[3];
      double wxr[3],wxrn[3],wxrl[3];
      double rh[3],vh[3];
      struct OrbitType *O;
      struct FormationType *Fr;
      struct DynType *D;
      double PosVec[3], VelVec[3];
      long UseCM;

      size_t lenstr = strlen(S->FileName);
      if (lenstr > 4 && !strncmp(S->FileName + lenstr-4, "yaml", 4))
          InitSpacecraftYAML(S, PosVec, VelVec, &UseCM);
      else
          InitSpacecraftTxt(S, PosVec, VelVec, &UseCM);

      /* .. Initialize some Orbit and Formation variables */
      O = &Orb[S->RefOrb];
      Fr = &Frm[S->RefOrb];
      if (UseCM) {
         if (Fr->FixedInFrame == 'L') {
            MTxV(Fr->CL,PosVec,S->PosEH);
            MTxV(Fr->CL,VelVec,S->VelEH);
            if (O->Regime == ORB_ZERO) {
               for(i=0;i<3;i++) {
                  S->PosR[i] = S->PosEH[i];
                  S->VelR[i] = S->VelEH[i];
               }
            }
            else if (O->Regime == ORB_FLIGHT) {
               MTxV(O->CLN,S->PosEH,S->PosR);
               MTxV(O->CLN,S->VelEH,S->VelR);
            }
            else {
               EHRV2RelRV(O->SMA,MAGV(O->wln),
                  O->CLN,S->PosEH,S->VelEH,S->PosR,S->VelR);
            }
         }
         else {
            MTxV(Fr->CN,PosVec,S->PosR);
            MTxV(Fr->CN,VelVec,S->VelR);
            if (O->Regime == ORB_ZERO) {
               for(i=0;i<3;i++) {
                  S->PosEH[i] = S->PosR[i];
                  S->VelEH[i] = S->VelR[i];
               }
            }
            else if (O->Regime == ORB_FLIGHT) {
               MxV(O->CLN,S->PosR,S->PosEH);
               MxV(O->CLN,S->VelR,S->VelEH);
            }
            else {
               RelRV2EHRV(O->SMA,MAGV(O->wln),
                  O->CLN,S->PosR,S->VelR,S->PosEH,S->VelEH);
            }
         }
         MTxV(S->B[0].CN,S->cm,pcmn);
         for(j=0;j<3;j++) {
            psn[j] = S->PosR[j] - Fr->PosR[j] - pcmn[j];
         }
         MxV(Fr->CN,psn,S->PosF);
         VxV(S->B[0].wn,S->cm,wxr);
         MTxV(S->B[0].CN,wxr,wxrn);
         for(j=0;j<3;j++) {
            vsn[j] = S->VelR[j] - wxrn[j];
         }
         MxV(Fr->CN,vsn,S->VelF);
      }
      else {
         for(j=0;j<3;j++) {
            S->PosF[j] = PosVec[j];
            S->VelF[j] = VelVec[j];
         }
         MTxV(S->B[0].CN,S->cm,pcmn);
         VxV(S->B[0].wn,S->cm,wxr);
         MTxV(S->B[0].CN,wxr,wxrn);
         if (Fr->FixedInFrame == 'L') {
            MTxV(Fr->CL,S->PosF,psl);
            MTxV(Fr->CL,S->VelF,vsl);
            MxV(O->CLN,Fr->PosR,pfl);
            MxV(O->CLN,pcmn,pcml);
            MxV(O->CLN,wxrn,wxrl);
            for(j=0;j<3;j++) {
               S->PosEH[j] = pcml[j] + psl[j] + pfl[j];
               S->VelEH[j] = wxrl[j] + vsl[j];
            }
            if (O->Regime == ORB_ZERO) {
               for(i=0;i<3;i++) {
                  S->PosR[i] = S->PosEH[i];
                  S->VelR[i] = S->VelEH[i];
               }
            }
            else if (O->Regime == ORB_FLIGHT) {
               MTxV(O->CLN,S->PosEH,S->PosR);
               MTxV(O->CLN,S->VelEH,S->VelR);
            }
            else {
               EHRV2RelRV(O->SMA,MAGV(O->wln),O->CLN,
                  S->PosEH,S->VelEH,S->PosR,S->VelR);
            }
         }
         else {
            MTxV(Fr->CN,S->PosF,psn);
            MTxV(Fr->CN,S->VelF,vsn);
            for(j=0;j<3;j++) {
               S->PosR[j] = pcmn[j] + psn[j] + Fr->PosR[j];
               S->VelR[j] = wxrn[j] + vsn[j];
            }
            if (O->Regime == ORB_ZERO) {
            }
            else if (O->Regime == ORB_FLIGHT) {
            }
            else {
               RelRV2EHRV(O->SMA,MAGV(O->wln),O->CLN,
                  S->PosR,S->VelR,S->PosEH,S->VelEH);
            }
         }
      }
      for(j=0;j<3;j++) {
         S->PosN[j] = O->PosN[j] + S->PosR[j];
         S->VelN[j] = O->VelN[j] + S->VelR[j];
      }
      MTxV(World[O->World].CNH,S->PosN,rh);
      MTxV(World[O->World].CNH,S->VelN,vh);
      for(j=0;j<3;j++) {
         S->PosH[j] = World[O->World].PosH[j] + rh[j];
         S->PosH[j] = World[O->World].VelH[j] + vh[j];
      }

      if (O->Regime == ORB_ZERO) {
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) S->CLN[i][j] = 0.0;
            S->CLN[i][i] = 1.0;
            S->wln[i] = 0.0;
         }
      }
      else if (O->Regime == ORB_FLIGHT) {
         FindENU(S->PosN,World[O->World].w,S->CLN,S->wln);
      }
      else {
         FindCLN(S->PosN,S->VelN,S->CLN,S->wln);
      }

      

      D = &S->Dyn;
      InitRigidDyn(S);
      InitFlexModes(S);

      D->ActiveState = (double *) calloc(D->Nu+D->Nf,sizeof(double));
      D->ActiveStateIdx = (long *) calloc(D->Nu+D->Nf,sizeof(long));
      D->COEF = CreateMatrix(D->Nu+D->Nf,D->Nu+D->Nf);
      D->RHS = (double *) calloc(D->Nu+D->Nf,sizeof(double));

      MapStateVectorToBodyStates(D->u,D->x,D->uf,D->xf,S);
      MotionConstraints(S);
      SCMassProps(S);
      FindTotalAngMom(S);
      EchoDyn(S);

/* .. Load geometry */
      for(j=0;j<S->Nb;j++) {
         OldNgeom = Ngeom;
         Geom = LoadWingsObjFile(ModelPath,S->B[j].GeomFileName,
            &Matl,&Nmatl,Geom,&Ngeom,&S->B[j].GeomTag,
            AeroShadowsActive || SolPressShadowsActive);
         if (ContactActive && OldNgeom != Ngeom) LoadOctree(&Geom[Ngeom-1]);
      }

/* .. Initialize Bounding Box */
      memcpy(&S->BBox,&Geom[S->B[0].GeomTag].BBox,sizeof(struct BoundingBoxType));
      UpdateScBoundingBox(S);

      S->EnvTrq.First = 1;
      
      InitAC(S);
      
/* .. Loop Gain and Delays allow verification of stability margins in the time domain */
      /* Created by commands */
      S->GainAndDelayActive = FALSE;
      S->LoopGain = 1.0;
      S->LoopDelay = 0.0;
      for(Iw=0;Iw<S->Nw;Iw++) S->Whl[Iw].Delay = NULL;
      for(Im=0;Im<S->Nmtb;Im++) S->MTB[Im].Delay = NULL;
      for(It=0;It<S->Nthr;It++) S->Thr[It].Delay = NULL;


}

/*********************************************************************/
void LoadTdrs(void)
{
      FILE *infile;
      char junk[120],newline;
      char response[120];
      long i;

/* .. Initialize TDRS */
      infile = FileOpen(InOutPath,"Inp_TDRS.txt","r");
/* .. 42 TDRS Configuration File */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);

      for(i=0;i<10;i++) {
         fscanf(infile,"%s \"%[^\"]\" %[^\n] %[\n]",response,
            Tdrs[i].Designation,junk,&newline);
         Tdrs[i].Exists = DecodeString(response);
      }

      fclose(infile);
}
/*********************************************************************/
void LoadSun(void)
{
      /* Rumor is, Sun's magfield is highly variable, poorly modeled */
      /* by simple dipole.                                           */
      double DipoleAxis[3] = {0.0,0.0,1.0};
      double SunColor[3] = {1.0,1.0,0.9};
      unsigned char Glyph[14] = {0xc0,0xc0,0x00,0x00,0x18,0x66,0x42,
                                 0x99,0x99,0x42,0x66,0x18,0x00,0x00};
      long i,j;
      struct WorldType *W;

      W = &World[SOL];

      /* Relationships */
      W->Exists = TRUE;
      W->Type = SUN;
      W->Parent = 0;

      W->Nsat = 9;
      W->Sat = (long *) calloc(W->Nsat,sizeof(long));
      if (W->Sat == NULL) {
         printf("W->Sat calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }
      for(i=0;i<W->Nsat;i++) W->Sat[i] = MERCURY+i;

      /* Physical Properties */
      W->mu=1.32715E20;
      W->rad=6.98E8;
      W->w=2.69E-6;
      W->RadOfInfluence = 2.0E13;  /* Beyond Pluto's Orbit */
      W->DipoleMoment = 0.0;
      for(j=0;j<3;j++) {
         W->DipoleAxis[j] = DipoleAxis[j];
         W->DipoleOffset[j] = 0.0;
      }
      W->RingInner = 0.0;
      W->RingOuter = 0.0;

      /* Ephemeris */
      W->eph.World = 0;
      W->eph.mu = W->mu;
      W->eph.SMA=0.0;
      W->eph.ecc=0.0;
      W->eph.inc=0.0;
      W->eph.RAAN=0.0;
      W->eph.ArgP=0.0;
      W->eph.tp=0.0;
      W->eph.anom=0.0;
      W->eph.alpha=0.0;
      W->eph.SLR=0.0;
      W->eph.rmin=0.0;

      /* Graphical Properties */
      W->Atmo.Exists = FALSE;
      W->HasRing = FALSE;
      strcpy(W->Name,"Sun");
      strcpy(W->MapFileName,"NONE");
      strcpy(W->GeomFileName,"NONE");
      strcpy(W->ColTexFileName,"NONE");
      strcpy(W->BumpTexFileName,"NONE");
      for(j=0;j<3;j++) {
         W->Color[j] = (float) SunColor[j];
      }
      W->Color[3] = 1.0;
      for(j=0;j<14;j++) W->Glyph[j] = Glyph[j];

      /* State Variables */
      for(i=0;i<3;i++) {
         W->eph.PosN[i]=0.0;
         W->eph.VelN[i]=0.0;
         for(j=0;j<3;j++) W->CNH[i][j] = 0.0;
         W->CNH[i][i] = 1.0;
         W->qnh[i] = 0.0;
      }
      W->qnh[3] = 1.0;
}
/*********************************************************************/
void LoadPlanets(void)
{

      struct OrbitType *Eph;
      double Zaxis[3] = {0.0,0.0,1.0};
      double GMST;

      char PlanetName[10][20] ={"Sun","Mercury","Venus","Earth","Mars",
                                "Jupiter","Saturn","Uranus","Neptune",
                                "Pluto"};
      char MapFileName[10][20] = {"NONE","Rockball","Venus.ppm","Earth.ppm","Mars.ppm",
         "Jupiter.ppm","Saturn.ppm","Uranus.ppm","Neptune.ppm","Iceball"};
      double Mu[10] = {1.32715E20,2.18E13,3.2485E14,3.98604E14,
                       4.293E13,1.2761E17,3.792E16,5.788E15,6.8E15,
                       3.2E14};
      double J2[10] = {0.0,0.0,0.0,1.08263E-3,1.96045E-3,0.0,0.0,0.0,0.0,0.0};
      double Rad[10] = {6.98E8,2.42E6,6.1E6,6.378145E6,3.41E6,
                        7.14E7,6.04E7,2.35E7,2.23E7,7.0E6};
      double W[10] = {2.69E-6,1.23E-6,2.94E-7,7.292115E-5,7.0882E-5,
                      1.7659E-4,1.6728E-4,1.631E-4,1.105E-4,0.0};

      double PoleRA[10] = {0.0,281.008,272.758,0.0,317.683,268.057,40.587,257.313,299.333,133.046};
      double PoleDec[10] = {90.0,61.45,67.16,0.0,52.8865,64.496,83.537,-15.175,42.95,-6.145};
      double CNJ[3][3];
      double PriMerAngJ2000[10] = {0.0,329.71,160.26,190.16,176.868,284.95,38.90,203.81,253.18,236.77};
      /* Magnetic Field Dipole Strength, Wb-m */
      double DipoleMoment[10] = {0.0,0.0,0.0,7.943E15,0.0,
                                 0.0,0.0,0.0,0.0,0.0};
      /* Magnetic Field Dipole Axis Unit Vector */
      double DipoleAxis[10][3] = {{0.0,0.0,1.0},
                                  {0.0,0.0,1.0},
                                  {0.0,0.0,1.0},
                                  {-6.53286E-2,0.186549,-0.980271},
                                  {0.0,0.0,1.0},
                                  {0.0,0.0,1.0},
                                  {0.0,0.0,1.0},
                                  {0.0,0.0,1.0},
                                  {0.0,0.0,1.0},
                                  {0.0,0.0,1.0}};
      /* Magnetic Field Dipole Offset from Center, m */
      double DipoleOffset[10][3] = {{0.0,0.0,0.0},
                                    {0.0,0.0,0.0},
                                    {0.0,0.0,0.0},
                                    {-3.74461E5,2.44108E5,-1.58291E5},
                                    {0.0,0.0,0.0},
                                    {0.0,0.0,0.0},
                                    {0.0,0.0,0.0},
                                    {0.0,0.0,0.0},
                                    {0.0,0.0,0.0},
                                    {0.0,0.0,0.0}};
      float Color[11][3] = {{1.0f    , 1.0f     , 0.9f},      /* Sun */
                           {0.400318f, 0.347338f, 0.253973f}, /* Mercury */
                           {0.716824f, 0.676952f, 0.623907f}, /* Venus */
                           {0.212f   , 0.293502f, 0.522072f}, /* Earth */
                           {0.687493f, 0.454481f, 0.365368f}, /* Mars */
                           {0.793131f, 0.627618f, 0.477430f}, /* Jupiter */
                           {0.705187f, 0.677713f, 0.620916f}, /* Saturn */
                           {0.486074f, 0.584573f, 0.769742f}, /* Uranus */
                           {0.187558f, 0.243884f, 0.413025f}, /* Neptune */
                           {0.268063f, 0.268183f, 0.268204f}, /* Pluto */
                           {0.440417f, 0.441343f, 0.441084f}};/* Luna */
      unsigned char Glyph[11][14] = {
{0xc0,0xc0,0x00,0x00,0x18,0x66,0x42,0x99,0x99,0x42,0x66,0x18,0x00,0x00}, /* Sun */
{0xc0,0xc0,0x00,0x10,0x7c,0x10,0x38,0x44,0x82,0x82,0x44,0x38,0x44,0x82}, /* Mercury */
{0xc0,0xc0,0x00,0x10,0x10,0x7c,0x10,0x38,0x44,0x82,0x82,0x44,0x38,0x00}, /* Venus */
{0xc0,0xc0,0x00,0x00,0x38,0x54,0x92,0xfe,0x92,0x54,0x38,0x00,0x00,0x00}, /* Earth */
{0xc0,0xc0,0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x44,0x3c,0x05,0x01,0x07}, /* Mars */
{0xc0,0xc0,0x00,0x04,0x04,0x7f,0x44,0x24,0x14,0x14,0x24,0xc2,0x00,0x00}, /* Jupiter */
{0xc0,0xc0,0x00,0x0c,0x08,0x44,0x42,0x42,0x64,0x58,0x40,0x40,0xe0,0x00}, /* Saturn */
{0xc0,0xc0,0x00,0x38,0x44,0x82,0x92,0x82,0x44,0x38,0x10,0x54,0x38,0x10}, /* Uranus */
{0xc0,0xc0,0x00,0x10,0xfe,0x10,0x38,0x54,0x92,0x92,0xd6,0x92,0x00,0x00}, /* Neptune */
{0xc0,0xc0,0x00,0x00,0xf8,0x80,0x80,0xf0,0x88,0x88,0xf0,0x00,0x00,0x00}, /* Pluto */
{0xc0,0xc0,0x00,0x00,0x18,0x70,0x60,0xe0,0xe0,0x60,0x70,0x18,0x00,0x00}};/* Luna */
      long HasAtmo[11] = {0,0,0,1,1,0,0,0,0,0,0};
      long i,j;

      for(i=MERCURY;i<=PLUTO;i++){
         strcpy(World[i].Name,PlanetName[i]);
         strcpy(World[i].MapFileName,MapFileName[i]);
         strcpy(World[i].ColTexFileName,"NONE");
         strcpy(World[i].BumpTexFileName,"NONE");
         World[i].mu=Mu[i];
         World[i].J2 = J2[i];
         World[i].rad=Rad[i];
         World[i].w=W[i];
         World[i].PriMerAngJ2000 = PriMerAngJ2000[i]*D2R;
         World[i].Parent = SOL;
         World[i].eph.World = SOL;
         World[i].eph.mu = World[SOL].mu;
         World[i].DipoleMoment = DipoleMoment[i];
         for(j=0;j<3;j++) {
            World[i].DipoleAxis[j] = DipoleAxis[i][j];
            World[i].DipoleOffset[j] = DipoleOffset[i][j];
            World[i].Color[j] = Color[i][j];
         }
         World[i].Color[3] = 1.0;
         for(j=0;j<14;j++) World[i].Glyph[j] = Glyph[i][j];
         World[i].Atmo.Exists = HasAtmo[i];
      }

      World[EARTH].Atmo.GasColor[0] = 0.17523;
      World[EARTH].Atmo.GasColor[1] = 0.40785;
      World[EARTH].Atmo.GasColor[2] = 1.0;
      World[EARTH].Atmo.DustColor[0] = 1.0;
      World[EARTH].Atmo.DustColor[1] = 1.0;
      World[EARTH].Atmo.DustColor[2] = 1.0;
      World[EARTH].Atmo.RayScat[0] = 5.8E-6;
      World[EARTH].Atmo.RayScat[1] = 13.5E-6;
      World[EARTH].Atmo.RayScat[2] = 33.1E-6;
      World[EARTH].Atmo.RayScaleHt = 8000.0;
      World[EARTH].Atmo.MieScat = 4.0E-6;
      World[EARTH].Atmo.MieScaleHt = 1200.0;
      World[EARTH].Atmo.MieG = 0.76;
      World[EARTH].Atmo.MaxHt = 8.0*World[EARTH].Atmo.RayScaleHt;
      World[EARTH].Atmo.rad = World[EARTH].rad + World[EARTH].Atmo.MaxHt;

      World[MARS].Atmo.GasColor[0] = 0.70588;
      World[MARS].Atmo.GasColor[1] = 0.50196;
      World[MARS].Atmo.GasColor[2] = 0.19608;
      World[MARS].Atmo.DustColor[0] = 0.5;
      World[MARS].Atmo.DustColor[1] = 0.5;
      World[MARS].Atmo.DustColor[2] = 0.5;
      World[MARS].Atmo.RayScat[0] = 19.918E-6;
      World[MARS].Atmo.RayScat[1] = 13.57E-6;
      World[MARS].Atmo.RayScat[2] = 5.75E-6;
      World[MARS].Atmo.RayScaleHt = 11000.0;
      World[MARS].Atmo.MieScat = 0.0E-6;
      World[MARS].Atmo.MieScaleHt = 11000.0;
      World[MARS].Atmo.MieG = 0.76;
      World[MARS].Atmo.MaxHt = 8.0*World[MARS].Atmo.RayScaleHt;
      World[MARS].Atmo.rad = World[MARS].rad + World[MARS].Atmo.MaxHt;

/* .. Load planetary orbit elements for date of interest */
      for(i=MERCURY;i<=PLUTO;i++){
         PlanetEphemerides(i,TT.JulDay,World[i].eph.mu,
                                &World[i].eph.SMA,
                                &World[i].eph.ecc,
                                &World[i].eph.inc,
                                &World[i].eph.RAAN,
                                &World[i].eph.ArgP,
                                &World[i].eph.tp,
                                &World[i].eph.anom,
                                &World[i].eph.SLR,
                                &World[i].eph.alpha,
                                &World[i].eph.rmin,
                                &World[i].eph.MeanMotion,
                                &World[i].eph.Period);
      }

      /* Planetocentric Inertial Reference Frames */
      A2C(123,-23.4392911*D2R,0.0,0.0,World[EARTH].CNH);
      C2Q(World[EARTH].CNH,World[EARTH].qnh);
      for(i=MERCURY;i<=PLUTO;i++) {
         if (i != EARTH) {
            A2C(312,(PoleRA[i]+90.0)*D2R,(90.0-PoleDec[i])*D2R,0.0,CNJ);
            MxM(CNJ,World[EARTH].CNH,World[i].CNH);
            C2Q(World[i].CNH,World[i].qnh);
         }
      }

/* .. Saturn's Rings */
      World[SATURN].HasRing = 1;
      World[SATURN].RingInner = 67258.0E3;
      World[SATURN].RingOuter = 181328.0E3;

      for(i=MERCURY;i<=PLUTO;i++) {
         World[i].RadOfInfluence =
            RadiusOfInfluence(World[i].eph.mu,World[i].mu,World[i].eph.SMA);
         World[i].Type = PLANET;
      }

      for(i=MERCURY;i<=PLUTO;i++){
         if(World[i].Exists){
            Eph = &World[i].eph;
            Eph2RV(Eph->mu,Eph->SLR,Eph->ecc,Eph->inc,Eph->RAAN,Eph->ArgP,
                   DynTime-Eph->tp,Eph->PosN,Eph->VelN,&Eph->anom);
            for(j=0;j<3;j++) World[i].PosH[j] = Eph->PosN[j];
            World[i].PriMerAng = fmod(World[i].PriMerAngJ2000+World[i].w*DynTime,TwoPi);
            SimpRot(Zaxis,World[i].PriMerAng,World[i].CWN);
            C2Q(World[i].CWN,World[i].qwn);
         }
      }
/* .. Earth rotation is a special case */
      GMST = JD2GMST(TT.JulDay);
      World[EARTH].PriMerAng = TwoPi*GMST;
      SimpRot(Zaxis,World[EARTH].PriMerAng,World[EARTH].CWN);
      C2Q(World[EARTH].CWN,World[EARTH].qwn);

      strcpy(World[EARTH].BumpTexFileName,"EarthBump.ppm");
}
/*********************************************************************/
void LoadMoonOfEarth(void)
{
#define Nm 1

      char Name[Nm][40] = {"Luna"};
      char MapFileName[Nm][40] = {"Luna.ppm"};
      float Color[4] = {0.440417f, 0.441343f, 0.441084f,1.0f};
      double mu[Nm] = {4.903E12};
      double J2[Nm] = {2.027E-4};
      double rad[Nm] = {1.738E6};
      double w[Nm] = {2.66E-6};
      double SMA[Nm] = {384400000.0};
      double ecc[Nm] = {0.0549};
      double inc[Nm] = {0.0};
      double RAAN[Nm] = {0.0};
      double omg[Nm] = {0.0};
      long EpochYear[Nm] = {2000};
      long EpochMon[Nm] = {1};
      long EpochDay[Nm] = {1};
      long EpochHour[Nm] = {12};
      double MeanAnom[Nm] = {0.0};
      double Epoch;
      unsigned char Glyph[14] = {0xc0,0xc0,0x00,0x00,0x18,0x70,0x60,
                                 0xe0,0xe0,0x60,0x70,0x18,0x00,0x00};

      long Ip = EARTH;
      long Iw,Im;
      long i;
      struct WorldType *M,*P;
      struct OrbitType *E;
      double CNJ[3][3];

      P = &World[Ip];
      P->Nsat = 1;
      P->Sat = (long *) calloc(Nm,sizeof(long));
      if (P->Sat == NULL) {
         printf("Earth P->Sat calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      for(Im=0;Im<Nm;Im++) {
         Iw = LUNA+Im;
         M = &World[Iw];
         E = &M->eph;
         P->Sat[Im] = Iw;

         M->Exists = TRUE;
         M->Parent = EARTH;
         strcpy(M->Name,Name[Im]);
         strcpy(M->MapFileName,MapFileName[Im]);
         strcpy(M->ColTexFileName,"LunaCol.ppm");
         strcpy(M->BumpTexFileName,"LunaBump.ppm");
         for(i=0;i<4;i++) M->Color[i] = Color[i];
         for(i=0;i<14;i++) M->Glyph[i] = Glyph[i];
         M->mu = mu[Im];
         M->J2 = J2[Im];
         M->rad = rad[Im];
         M->w = w[Im];
         M->PriMerAng = 0.0;
         E->Exists = TRUE;
         E->Regime = ORB_CENTRAL;
         E->World = Ip;
         E->mu = P->mu;
         E->SMA = SMA[Im];
         E->ecc = ecc[Im];
         E->inc = inc[Im];
         E->RAAN = RAAN[Im];
         E->ArgP = omg[Im];

         Epoch = DateToTime(EpochYear[Im],EpochMon[Im],EpochDay[Im],EpochHour[Im],0,0.0);
         E->MeanMotion = sqrt(E->mu/(E->SMA*E->SMA*E->SMA));
         E->Period = TwoPi/E->MeanMotion;
         E->tp = Epoch - MeanAnom[Im]*D2R/E->MeanMotion;
         while ((E->tp-DynTime0) < -E->Period) E->tp += E->Period;
         while ((E->tp-DynTime0) >  E->Period) E->tp -= E->Period;

         E->alpha = 1.0/E->SMA;
         E->SLR = E->SMA*(1.0-E->ecc*E->ecc);
         E->rmin = E->SMA*(1.0-E->ecc);

         E->anom = TrueAnomaly(E->mu,E->SLR,E->ecc,DynTime-E->tp);
         M->RadOfInfluence = RadiusOfInfluence(P->mu,M->mu,E->SMA);

         LunaInertialFrame(TT.JulDay,CNJ);
         MxM(CNJ,World[EARTH].CNH,M->CNH);
         C2Q(M->CNH,M->qnh);
         M->PriMerAng = LunaPriMerAng(TT.JulDay);
         M->Type = MOON;
      }
#undef Nm
}
/**********************************************************************/
/*  See JPL web pages MoonEphems and MoonParms in Development folder  */
void LoadMoonsOfMars(void)
{
#define Nm 2

      char Name[Nm][40] = {"Phobos","Deimos"};
      char MapFileName[Nm][40] = {"Rockball","Rockball"};
      double mu[Nm] = {7.158E5,9.8E4};
      double rad[Nm] = {11.1E3,6.2E3};
      double w[Nm] = {0.0,0.0};
      double SMA[Nm] = {9380.0E3,23460.0E3};
      double ecc[Nm] = {0.0151,0.0002};
      double inc[Nm] = {1.075,1.793};
      double RAAN[Nm] = {164.931,339.600};
      double omg[Nm] = {150.247,290.496};
      long EpochYear[Nm] = {1950,1950};
      long EpochMon[Nm] = {1,1};
      long EpochDay[Nm] = {1,1};
      double MeanAnom[Nm] = {92.474,296.230};
      double Epoch;

      long Ip = MARS;
      long Im,Iw;
      long i,j;
      struct WorldType *M,*P;
      struct OrbitType *E;

      P = &World[Ip];
      P->Nsat = Nm;
      P->Sat = (long *) calloc(Nm,sizeof(long));
      if (P->Sat == NULL) {
         printf("Mars P->Sat calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      for(Im=0;Im<Nm;Im++) {
         Iw = PHOBOS + Im;
         M = &World[Iw];
         E = &M->eph;
         P->Sat[Im] = Iw;

         M->Exists = TRUE;
         M->Parent = MARS;
         strcpy(M->Name,Name[Im]);
         strcpy(M->MapFileName,MapFileName[Im]);
         strcpy(M->ColTexFileName,"NONE");
         strcpy(M->BumpTexFileName,"NONE");
         M->mu = mu[Im];
         M->rad = rad[Im];
         M->w = w[Im];
         M->PriMerAng = 0.0;
         E->Exists = TRUE;
         E->Regime = ORB_CENTRAL;
         E->World = Ip;
         E->mu = World[Ip].mu;
         E->SMA = SMA[Im];
         E->ecc = ecc[Im];
         E->inc = inc[Im]*D2R;
         E->RAAN = RAAN[Im]*D2R;
         E->ArgP = omg[Im];

         Epoch = DateToTime(EpochYear[Im],EpochMon[Im],EpochDay[Im],0,0,0.0);
         E->MeanMotion = sqrt(E->mu/(E->SMA*E->SMA*E->SMA));
         E->Period = TwoPi/E->MeanMotion;
         E->tp = Epoch - MeanAnom[Im]*D2R/E->MeanMotion;
         while ((E->tp-DynTime0) < -E->Period) E->tp += E->Period;
         while ((E->tp-DynTime0) >  E->Period) E->tp -= E->Period;

         E->alpha = 1.0/E->SMA;
         E->SLR = E->SMA*(1.0-E->ecc*E->ecc);
         E->rmin = E->SMA*(1.0-E->ecc);

         E->anom = TrueAnomaly(E->mu,E->SLR,E->ecc,DynTime-E->tp);
         M->RadOfInfluence = RadiusOfInfluence(P->mu,M->mu,E->SMA);

         /* CNH assumed to be same as parent planet */
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) M->CNH[i][j] = P->CNH[i][j];
         }
         C2Q(M->CNH,M->qnh);
         for(i=0;i<4;i++) M->Color[i] = 1.0;
         M->Type = MOON;
      }
      strcpy(World[PHOBOS].GeomFileName,"Phobos.obj");
      Geom = LoadWingsObjFile(ModelPath,World[PHOBOS].GeomFileName,
         &Matl,&Nmatl,Geom,&Ngeom,&World[PHOBOS].GeomTag,FALSE);


#undef Nm
}
/**********************************************************************/
void LoadMoonsOfJupiter(void)
{
#define Nm 16

      char Name[Nm][40] = {"Io","Europa","Ganymede","Callisto","Amalthea",
         "Himalia","Elara","Pasiphae","Sinope","Lysithea","Carme",
         "Ananke","Leda","Thebe","Adrastea","Metis"};
      char MapFileName[Nm][40] = {"NONE","NONE","NONE","NONE",
         "NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE",
         "NONE","NONE","NONE","NONE"};
      double mu[Nm] = {5.959E9,3202.739E9,9887.834E9,7179.289E9,1.38E8,
         4.5E8,5.8E7,2.0E7,5.0E6,4.2E6,8.8E6,
         2.0E6,7.3E5,1.0E8,5.0E5,8.0E6};
      double rad[Nm] = {1821.6E3,1560.8E3,2631.2E3,2410.3E3,83.45E3,
         85.0E3,43.0E3,30.0E3,19.0E3,18.0E3,23.0E3,
         14.0E3,10.0E3,49.3E3,8.2E3,21.5E3};
      double w[Nm] = {0.0,0.0,0.0,0.0,
         0.0,0.0,0.0,0.0,0.0,
         0.0,0.0,0.0,0.0,};
      double SMA[Nm] = {4.2180E8,6.7110E8,1.07040E9,1.8827E9,1.814E8,
         1.1461E10,1.1741E10,2.3624E10,2.3939E10,1.1717E10,2.3404E10,
         2.1276E10,1.1165E10,2.219E8,1.29E8,1.28E8};
      double ecc[Nm] = {0.0041,0.0094,0.0013,0.0074,0.0032,
         0.1623,0.2174,0.4090,0.2495,0.1124,0.2533,
         0.2435,0.1636,0.0176,0.0018,0.0012};
      double inc[Nm] = {0.036,0.466,0.177,0.192,0.380,
         27.496,26.627,151.431,158.109,28.302,164.907,
         148.889,27.457,1.08,0.054,0.019};
      double RAAN[Nm] = {43.977,219.106,63.552,298.848,108.946,
         57.245,109.373,312.990,303.081,5.528,113.738,
         7.615,217.137,235.694,228.378,146.912};
      double omg[Nm] = {84.129,88.97,192.417,52.643,155.873,
         331.995,143.591,170.45,346.394,49.486,28.199,
         100.619,272.349,234.269,328.047,297.177};
      long EpochYear[Nm] = {1997,1997,1997,1997,1997,
         2000,2000,2000,2000,2000,2000,2000,2000,
         1997,1997,1997};
      long EpochMon[Nm] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
      long EpochDay[Nm] = {16,16,16,16,16,
         1,1,1,1,1,1,1,1,
         16,16,16};
      double MeanAnom[Nm] = {342.021,171.016,317.54,181.408,185.194,
         68.721,332.962,280.193,168.397,329.121,234.027,248.793,228.076,
         135.956,135.673,276.047};
      double Epoch;

      long Ip = JUPITER;
      long Im,Iw;
      long i,j;
      struct WorldType *M,*P;
      struct OrbitType *E;

      P = &World[Ip];
      P->Nsat = Nm;
      P->Sat = (long *) calloc(Nm,sizeof(long));
      if (P->Sat == NULL) {
         printf("Jupiter P->Sat calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      for(Im=0;Im<Nm;Im++) {
         Iw = IO + Im;
         M = &World[Iw];
         E = &M->eph;
         P->Sat[Im] = Iw;

         M->Exists = TRUE;
         M->Parent = JUPITER;
         strcpy(M->Name,Name[Im]);
         strcpy(M->MapFileName,MapFileName[Im]);
         strcpy(M->ColTexFileName,"NONE");
         strcpy(M->BumpTexFileName,"NONE");
         M->mu = mu[Im];
         M->rad = rad[Im];
         M->w = w[Im];
         M->PriMerAng = 0.0;
         E->Exists = TRUE;
         E->Regime = ORB_CENTRAL;
         E->World = Ip;
         E->mu = World[Ip].mu;
         E->SMA = SMA[Im];
         E->ecc = ecc[Im];
         E->inc = inc[Im]*D2R;
         E->RAAN = RAAN[Im]*D2R;
         E->ArgP = omg[Im]*D2R;

         Epoch = DateToTime(EpochYear[Im],EpochMon[Im],EpochDay[Im],0,0,0.0);
         E->MeanMotion = sqrt(E->mu/(E->SMA*E->SMA*E->SMA));
         E->Period = TwoPi/E->MeanMotion;
         E->tp = Epoch - MeanAnom[Im]*D2R/E->MeanMotion;
         while ((E->tp-DynTime0) < -E->Period) E->tp += E->Period;
         while ((E->tp-DynTime0) >  E->Period) E->tp -= E->Period;

         E->alpha = 1.0/E->SMA;
         E->SLR = E->SMA*(1.0-E->ecc*E->ecc);
         E->rmin = E->SMA*(1.0-E->ecc);

         E->anom = TrueAnomaly(E->mu,E->SLR,E->ecc,DynTime-E->tp);
         M->RadOfInfluence = RadiusOfInfluence(P->mu,M->mu,E->SMA);

         /* CNH assumed to be same as parent planet */
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) M->CNH[i][j] = P->CNH[i][j];
         }
         C2Q(M->CNH,M->qnh);
         for(i=0;i<4;i++) M->Color[i] = 1.0;
         M->Type = MOON;
      }
#undef Nm
}
/**********************************************************************/
void LoadMoonsOfSaturn(void)
{
#define Nm 18

      char Name[Nm][40] = {"Mimas","Enceladus","Tethys","Dione","Rhea",
         "Titan","Hyperion","Iapetus","Phoebe","Janus","Epimetheus",
         "Helene","Telesto","Calypso","Atlas","Prometheus","Pandora","Pan"};
      char MapFileName[Nm][40] = {"NONE","Iceball","NONE","NONE",
         "NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE",
         "NONE","NONE","NONE","NONE","NONE","NONE"};
      double mu[Nm] = {2.53E9,7.21E9,4.121E10,7.3113E10,1.5407E11,
         8.97819E12,3.7E8,1.205E11,5.531E8,1.266E8,3.51E7,
         1.7E6,4.8E5,2.4E5,1.4E5,1.246E7,9.95E6,3.3E5};
      double rad[Nm] = {198.8E3,252.3E3,536.3E3,562.5E3,764.5E3,
         2575.5E3,133.0E3,734.5E3,106.6E3,90.4E3,58.3E3,
         16.0E3,12.0E3,9.5E3,10.E3,46.8e3,40.6E3,12.8E3};
      double w[Nm] = {0.0,0.0,0.0,0.0,0.0,0.0,
         0.0,0.0,0.0,0.0,0.0,0.0,
         0.0,0.0,0.0,0.0,0.0,0.0};
      double SMA[Nm] = {1.8554E8,2.3804E8,2.9467E8,3.7742E8,5.2707E8,
         1.22187E9,1.50088E9,3.56084E9,1.294778E10,1.5146E8,1.5141E8,
         3.7742E8,2.9471E8,2.9471E8,1.3767E8,1.3938E8,1.4172E8,1.3358E8};
      double ecc[Nm] = {0.0196,0.0047,0.0001,0.0022,0.001,
         0.0288,0.0274,0.0283,0.1635,0.0068,0.0098,
         0.0071,0.0002,0.0005,0.0012,0.0022,0.0042,0.0};
      double inc[Nm] = {1.572,0.009,1.091,0.028,0.331,
         0.28,0.63,7.489,175.986,0.163,0.351,
         0.213,1.18,1.499,0.003,0.008,0.05,0.001};
      double RAAN[Nm] = {153.152,93.204,330.882,168.909,311.531,
         24.502,264.022,75.831,241.57,46.899,85.244,
         40.039,300.256,25.327,0.5,259.504,327.215,40.557};
      double omg[Nm] = {14.352,211.923,262.845,168.82,256.609,
         185.671,324.183,275.921,345.582,241.778,312.63,
         292.056,341.795,234.788,331.521,164.389,83.461,139.318};
      long EpochYear[Nm] = {2004,2004,2004,2004,2004,
         2004,2004,2004,2004,2004,2004,
         2004,2004,2004,2004,2004,2004,2004};
      long EpochMon[Nm] = {1,1,1,1,1,
         1,1,1,1,1,1,1,1,1,1,1,1,1};
      long EpochDay[Nm] = {1,1,1,1,1,
         1,1,1,1,1,1,1,1,1,1,1,1,1};
      double MeanAnom[Nm] = {255.312,197.047,189.003,65.99,311.551,
         15.154,295.906,356.029,287.593,242.754,308.322,
         134.07,200.143,101.961,157.738,242.224,202.697,246.065};
      double Epoch;

      long Ip = SATURN;
      long Im,Iw;
      long i,j;
      struct WorldType *M,*P;
      struct OrbitType *E;

      P = &World[Ip];
      P->Nsat = Nm;
      P->Sat = (long *) calloc(Nm,sizeof(long));
      if (P->Sat == NULL) {
         printf("Saturn P->Sat calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      for(Im=0;Im<Nm;Im++) {
         Iw = MIMAS + Im;
         M = &World[Iw];
         E = &M->eph;
         P->Sat[Im] = Iw;

         M->Exists = TRUE;
         M->Parent = SATURN;
         strcpy(M->Name,Name[Im]);
         strcpy(M->MapFileName,MapFileName[Im]);
         strcpy(M->ColTexFileName,"NONE");
         strcpy(M->BumpTexFileName,"NONE");
         M->mu = mu[Im];
         M->rad = rad[Im];
         M->w = w[Im];
         M->PriMerAng = 0.0;
         E->Exists = TRUE;
         E->Regime = ORB_CENTRAL;
         E->World = Ip;
         E->mu = World[Ip].mu;
         E->SMA = SMA[Im];
         E->ecc = ecc[Im];
         E->inc = inc[Im];
         E->RAAN = RAAN[Im];
         E->ArgP = omg[Im];

         Epoch = DateToTime(EpochYear[Im],EpochMon[Im],EpochDay[Im],0,0,0.0);
         E->MeanMotion = sqrt(E->mu/(E->SMA*E->SMA*E->SMA));
         E->Period = TwoPi/E->MeanMotion;
         E->tp = Epoch - MeanAnom[Im]*D2R/E->MeanMotion;
         while ((E->tp-DynTime0) < -E->Period) E->tp += E->Period;
         while ((E->tp-DynTime0) >  E->Period) E->tp -= E->Period;

         E->alpha = 1.0/E->SMA;
         E->SLR = E->SMA*(1.0-E->ecc*E->ecc);
         E->rmin = E->SMA*(1.0-E->ecc);

         E->anom = TrueAnomaly(E->mu,E->SLR,E->ecc,DynTime-E->tp);
         M->RadOfInfluence = RadiusOfInfluence(P->mu,M->mu,E->SMA);

         /* CNH assumed to be same as parent planet */
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) M->CNH[i][j] = P->CNH[i][j];
         }
         C2Q(M->CNH,M->qnh);
         for(i=0;i<4;i++) M->Color[i] = 1.0;
         M->Type = MOON;
      }
#undef Nm
}
/**********************************************************************/
void LoadMoonsOfUranus(void)
{
#define Nm 5

      char Name[Nm][40] = {"Ariel","Umbriel","Titania","Oberon","Miranda"};
      char MapFileName[Nm][40] = {"NONE","NONE","NONE","NONE","NONE"};
      double mu[Nm] = {90.3E9,78.2E9,235.3E9,201.1E9,4.4E9};
      double rad[Nm] = {578.9E3,584.7E3,788.9E3,761.4E3,235.8E3};
      double w[Nm] = {0.0,0.0,0.0,0.0,0.0};
      double SMA[Nm] = {1.909E8,2.66E8,4.363E8,5.835E8,1.299E8};
      double ecc[Nm] = {0.0012,0.0039,0.0011,0.0014,0.0013};
      double inc[Nm] = {0.041,0.128,0.079,0.068,4.338};
      double RAAN[Nm] = {22.394,33.485,99.771,279.771,326.438};
      double omg[Nm] = {115.349,84.709,284.4,104.4,68.312};
      long EpochYear[Nm] = {1980,1980,1980,1980,1980};
      long EpochMon[Nm] = {1,1,1,1,1};
      long EpochDay[Nm] = {1,1,1,1,1};
      double MeanAnom[Nm] = {39.481,12.469,24.614,283.088,311.33};
      double Epoch;

      long Ip = URANUS;
      long Im,Iw;
      long i,j;
      struct WorldType *M,*P;
      struct OrbitType *E;

      P = &World[Ip];
      P->Nsat = Nm;
      P->Sat = (long *) calloc(Nm,sizeof(long));
      if (P->Sat == NULL) {
         printf("Uranus P->Sat calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      for(Im=0;Im<Nm;Im++) {
         Iw = ARIEL + Im;
         M = &World[Iw];
         E = &M->eph;
         P->Sat[Im] = Iw;

         M->Exists = TRUE;
         M->Parent = URANUS;
         strcpy(M->Name,Name[Im]);
         strcpy(M->MapFileName,MapFileName[Im]);
         strcpy(M->ColTexFileName,"NONE");
         strcpy(M->BumpTexFileName,"NONE");
         M->mu = mu[Im];
         M->rad = rad[Im];
         M->w = w[Im];
         M->PriMerAng = 0.0;
         E->Exists = TRUE;
         E->Regime = ORB_CENTRAL;
         E->World = Ip;
         E->mu = World[Ip].mu;
         E->SMA = SMA[Im];
         E->ecc = ecc[Im];
         E->inc = inc[Im];
         E->RAAN = RAAN[Im];
         E->ArgP = omg[Im];

         Epoch = DateToTime(EpochYear[Im],EpochMon[Im],EpochDay[Im],0,0,0.0);
         E->MeanMotion = sqrt(E->mu/(E->SMA*E->SMA*E->SMA));
         E->Period = TwoPi/E->MeanMotion;
         E->tp = Epoch - MeanAnom[Im]*D2R/E->MeanMotion;
         while ((E->tp-DynTime0) < -E->Period) E->tp += E->Period;
         while ((E->tp-DynTime0) >  E->Period) E->tp -= E->Period;

         E->alpha = 1.0/E->SMA;
         E->SLR = E->SMA*(1.0-E->ecc*E->ecc);
         E->rmin = E->SMA*(1.0-E->ecc);

         E->anom = TrueAnomaly(E->mu,E->SLR,E->ecc,DynTime-E->tp);
         M->RadOfInfluence = RadiusOfInfluence(P->mu,M->mu,E->SMA);

         /* CNH assumed to be same as parent planet */
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) M->CNH[i][j] = P->CNH[i][j];
         }
         C2Q(M->CNH,M->qnh);
         for(i=0;i<4;i++) M->Color[i] = 1.0;
         M->Type = MOON;
      }
#undef Nm
}
/**********************************************************************/
void LoadMoonsOfNeptune(void)
{
#define Nm 2

      char Name[Nm][40] = {"Triton","Nereid"};
      char MapFileName[Nm][40] = {"NONE","NONE"};
      double mu[Nm] = {1427.9E9,2.06E9};
      double rad[Nm] = {1353.4E3,170.0E3};
      double w[Nm] = {0.0,0.0};
      double SMA[Nm] = {3.548E8,5.5134E9};
      double ecc[Nm] = {0.0,0.7512};
      double inc[Nm] = {156.834,7.232};
      double RAAN[Nm] = {172.431,334.762};
      double omg[Nm] = {344.046,280.83};
      long EpochYear[Nm] = {1989,1989};
      long EpochMon[Nm] = {8,8};
      long EpochDay[Nm] = {25,25};
      double MeanAnom[Nm] = {264.775,359.341};
      double Epoch;

      long Ip = NEPTUNE;
      long Im,Iw;
      long i,j;
      struct WorldType *M,*P;
      struct OrbitType *E;

      P = &World[Ip];
      P->Nsat = Nm;
      P->Sat = (long *) calloc(Nm,sizeof(long));
      if (P->Sat == NULL) {
         printf("Neptune P->Sat calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      for(Im=0;Im<Nm;Im++) {
         Iw = TRITON + Im;
         M = &World[Iw];
         E = &M->eph;
         P->Sat[Im] = Iw;

         M->Exists = TRUE;
         M->Parent = NEPTUNE;
         strcpy(M->Name,Name[Im]);
         strcpy(M->MapFileName,MapFileName[Im]);
         strcpy(M->ColTexFileName,"NONE");
         strcpy(M->BumpTexFileName,"NONE");
         M->mu = mu[Im];
         M->rad = rad[Im];
         M->w = w[Im];
         M->PriMerAng = 0.0;
         E->Exists = TRUE;
         E->Regime = ORB_CENTRAL;
         E->World = Ip;
         E->mu = World[Ip].mu;
         E->SMA = SMA[Im];
         E->ecc = ecc[Im];
         E->inc = inc[Im];
         E->RAAN = RAAN[Im];
         E->ArgP = omg[Im];

         Epoch = DateToTime(EpochYear[Im],EpochMon[Im],EpochDay[Im],0,0,0.0);
         E->MeanMotion = sqrt(E->mu/(E->SMA*E->SMA*E->SMA));
         E->Period = TwoPi/E->MeanMotion;
         E->tp = Epoch - MeanAnom[Im]*D2R/E->MeanMotion;
         while ((E->tp-DynTime0) < -E->Period) E->tp += E->Period;
         while ((E->tp-DynTime0) >  E->Period) E->tp -= E->Period;

         E->alpha = 1.0/E->SMA;
         E->SLR = E->SMA*(1.0-E->ecc*E->ecc);
         E->rmin = E->SMA*(1.0-E->ecc);

         E->anom = TrueAnomaly(E->mu,E->SLR,E->ecc,DynTime-E->tp);
         M->RadOfInfluence = RadiusOfInfluence(P->mu,M->mu,E->SMA);

         /* CNH assumed to be same as parent planet */
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) M->CNH[i][j] = P->CNH[i][j];
         }
         C2Q(M->CNH,M->qnh);
         for(i=0;i<4;i++) M->Color[i] = 1.0;
         M->Type = MOON;
      }
#undef Nm
}
/**********************************************************************/
void LoadMoonsOfPluto(void)
{
#define Nm 1

      char Name[Nm][40] = {"Charon"};
      char MapFileName[Nm][40] = {"Iceball"};
      double mu[Nm] = {108.0E9};
      double rad[Nm] = {593.0E3};
      double w[Nm] = {0.0};
      double SMA[Nm] = {1.7536E7};
      double ecc[Nm] = {0.0022};
      double inc[Nm] = {0.001};
      double RAAN[Nm] = {85.187};
      double omg[Nm] = {71.255};
      long EpochYear[Nm] = {2000};
      long EpochMon[Nm] = {1};
      long EpochDay[Nm] = {1};
      long EpochHour[Nm] = {12};
      double MeanAnom[Nm] = {147.848};
      double Epoch;

      long Ip = PLUTO;
      long Iw,Im;
      long i,j;
      struct WorldType *M,*P;
      struct OrbitType *E;

      P = &World[Ip];
      P->Nsat = 1;
      P->Sat = (long *) calloc(Nm,sizeof(long));
      if (P->Sat == NULL) {
         printf("Pluto P->Sat calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }

      for(Im=0;Im<Nm;Im++) {
         Iw = CHARON+Im;
         M = &World[Iw];
         E = &M->eph;
         P->Sat[Im] = Iw;

         M->Exists = TRUE;
         M->Parent = PLUTO;
         strcpy(M->Name,Name[Im]);
         strcpy(M->MapFileName,MapFileName[Im]);
         strcpy(M->ColTexFileName,"NONE");
         strcpy(M->BumpTexFileName,"NONE");
         M->mu = mu[Im];
         M->rad = rad[Im];
         M->w = w[Im];
         M->PriMerAng = 0.0;
         E->Exists = TRUE;
         E->Regime = ORB_CENTRAL;
         E->World = Ip;
         E->mu = P->mu;
         E->SMA = SMA[Im];
         E->ecc = ecc[Im];
         E->inc = inc[Im];
         E->RAAN = RAAN[Im];
         E->ArgP = omg[Im];

         Epoch = DateToTime(EpochYear[Im],EpochMon[Im],EpochDay[Im],EpochHour[Im],0,0.0);
         E->MeanMotion = sqrt(E->mu/(E->SMA*E->SMA*E->SMA));
         E->Period = TwoPi/E->MeanMotion;
         E->tp = Epoch - MeanAnom[Im]*D2R/E->MeanMotion;
         while ((E->tp-DynTime0) < -E->Period) E->tp += E->Period;
         while ((E->tp-DynTime0) >  E->Period) E->tp -= E->Period;

         E->alpha = 1.0/E->SMA;
         E->SLR = E->SMA*(1.0-E->ecc*E->ecc);
         E->rmin = E->SMA*(1.0-E->ecc);

         E->anom = TrueAnomaly(E->mu,E->SLR,E->ecc,DynTime-E->tp);
         M->RadOfInfluence = RadiusOfInfluence(P->mu,M->mu,E->SMA);

         /* CNH assumed to be same as parent planet */
         for(i=0;i<3;i++) {
            for(j=0;j<3;j++) M->CNH[i][j] = P->CNH[i][j];
         }
         C2Q(M->CNH,M->qnh);
         for(i=0;i<4;i++) M->Color[i] = 1.0;
         M->Type = MOON;
      }
#undef Nm
}
/**********************************************************************/
void LoadMinorBodies(void)
{
      FILE *infile;
      struct WorldType *W;
      struct OrbitType *E;
      char junk[120],newline,response[120];
      long Ib,i;
      long EpochYear,EpochMon,EpochDay,EpochHour;
      double CNJ[3][3],PoleRA,PoleDec,Epoch;
      double ZAxis[3] = {0.0,0.0,1.0};

      infile = FileOpen(ModelPath,"MinorBodies.txt","r");
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%ld %[^\n] %[\n]",&Nmb,junk,&newline);
      if (Nmb > 10) {
         printf("Only 10 minor bodies are supported.  Adjust NWORLD to suit.\n");
         exit(1);
      }
      for(Ib=0;Ib<Nmb;Ib++) {
         W = &World[55+Ib];
         E = &W->eph;
         fscanf(infile,"%[^\n] %[\n]",junk,&newline);
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         W->Exists=DecodeString(response);
         fscanf(infile,"\"%[^\"]\" %[^\n] %[\n]",W->Name,junk,&newline);
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         W->Type=DecodeString(response);
         fscanf(infile,"%s %[^\n] %[\n]",W->MapFileName,junk,&newline);
         fscanf(infile,"%s %[^\n] %[\n]",W->GeomFileName,junk,&newline);
         fscanf(infile,"%s %[^\n] %[\n]",W->ColTexFileName,junk,&newline);
         fscanf(infile,"%s %[^\n] %[\n]",W->BumpTexFileName,junk,&newline);
         fscanf(infile,"%lf %[^\n] %[\n]",&W->mu,junk,&newline);
         fscanf(infile,"%lf %[^\n] %[\n]",&W->rad,junk,&newline);
         fscanf(infile,"%lf %[^\n] %[\n]",&W->w,junk,&newline);
         fscanf(infile,"%lf %lf %[^\n] %[\n]",&PoleRA,&PoleDec,junk,&newline);
         A2C(312,(PoleRA+90.0)*D2R,(90.0-PoleDec)*D2R,0.0,CNJ);
         MxM(CNJ,World[EARTH].CNH,W->CNH);
         C2Q(W->CNH,W->qnh);
         E->Exists = TRUE;
         E->Regime = ORB_CENTRAL;
         E->World = SOL;
         E->mu = World[SOL].mu;
         fscanf(infile,"%lf %[^\n] %[\n]",&E->SMA,junk,&newline);
         fscanf(infile,"%lf %[^\n] %[\n]",&E->ecc,junk,&newline);
         fscanf(infile,"%lf %[^\n] %[\n]",&E->inc,junk,&newline);
         fscanf(infile,"%lf %[^\n] %[\n]",&E->RAAN,junk,&newline);
         fscanf(infile,"%lf %[^\n] %[\n]",&E->ArgP,junk,&newline);
         fscanf(infile,"%ld %ld %ld %ld %[^\n] %[\n]",
            &EpochYear,&EpochMon,&EpochDay,&EpochHour,junk,&newline);
         fscanf(infile,"%lf %[^\n] %[\n]",&E->anom,junk,&newline);
         Epoch = DateToTime(EpochYear,EpochMon,EpochDay,EpochHour,0,0.0);
         E->MeanMotion = sqrt(E->mu/(E->SMA*E->SMA*E->SMA));
         E->Period = TwoPi/E->MeanMotion;
         E->alpha = 1.0/E->SMA;
         E->SLR = E->SMA*(1.0-E->ecc*E->ecc);
         E->rmin = E->SMA*(1.0-E->ecc);
         E->tp = Epoch - TimeSincePeriapsis(E->mu,E->SLR,E->ecc,E->anom);
         while ((E->tp-DynTime0) < -E->Period) E->tp += E->Period;
         while ((E->tp-DynTime0) >  E->Period) E->tp -= E->Period;

         Geom = LoadWingsObjFile(ModelPath,W->GeomFileName,
            &Matl,&Nmatl,Geom,&Ngeom,&W->GeomTag,TRUE);
         W->Density = W->mu/(6.67408E-11*PolyhedronVolume(&Geom[W->GeomTag]));

         W->Parent = SOL;
         W->Nsat = 0;
         W->RadOfInfluence = 100.0E3; /* Being generous */
         W->DipoleMoment = 0.0;
         W->DipoleAxis[2] = 1.0;
         W->Atmo.Exists = FALSE;
         W->HasRing = FALSE;
         for(i=0;i<3;i++) W->Color[i] = 0.5;
         W->Color[3] = 1.0;

         Eph2RV(E->mu,E->SLR,E->ecc,E->inc,E->RAAN,E->ArgP,
                DynTime-E->tp,E->PosN,E->VelN,&E->anom);
         for(i=0;i<3;i++) {
            W->PosH[i] = E->PosN[i];
            W->VelH[i] = E->VelN[i];
         }
         W->PriMerAng = fmod(W->w*DynTime,TwoPi);
         SimpRot(ZAxis,W->PriMerAng,W->CWN);
         C2Q(W->CWN,W->qwn);

      }
      fclose(infile);
}
/**********************************************************************/
void LoadRegions(void)
{
      FILE *infile;
      long Ir;
      char Exists[20],WorldID[20],IsPosW[120],junk[120],newline;
      struct WorldType *W;
      struct RegionType *R;
      double MagR;

      infile = FileOpen(InOutPath,"Inp_Region.txt","rt");

      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%ld %[^\n] %[\n]",&Nrgn,junk,&newline);
      Rgn = (struct RegionType *) calloc(Nrgn,sizeof(struct RegionType));
      for(Ir=0;Ir<Nrgn;Ir++) {
         R = &Rgn[Ir];
         fscanf(infile,"%[^\n] %[\n]",junk,&newline);
         fscanf(infile,"%s %[^\n] %[\n]",Exists,junk,&newline);
         R->Exists = DecodeString(Exists);
         fscanf(infile,"\"%[^\"]\" %[^\n] %[\n]",
            R->Name,junk,&newline);
         fscanf(infile,"%s %[^\n] %[\n]",WorldID,junk,&newline);
         R->World = DecodeString(WorldID);
         if (R->World<0 || R->World > NWORLD) {
            printf("Region's World is out of range in LoadRegions.  Bailing out.\n");
            exit(1);
         }
         W = &World[R->World];
         fscanf(infile,"%s %[^\n] %[\n]",IsPosW,junk,&newline);
         if (DecodeString(IsPosW)) {
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
               &R->PosW[0],&R->PosW[1],&R->PosW[2],junk,&newline);
            R->Lng = atan2(R->PosW[1],R->PosW[0]);
            MagR = MAGV(R->PosW);
            R->Lat = asin(R->PosW[2]/MagR);
            R->Alt = MagR-W->rad;
            A2C(312,R->Lng+HalfPi,HalfPi-R->Lat,0.0,R->CW);
            /* for(i=0;i<3;i++) R->CRW[i][i] = 1.0; */
            MTxV(W->CWN,R->PosW,R->PosN);
            MxM(R->CW,W->CWN,R->CN);
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
         }
         else {
            fscanf(infile,"%[^\n] %[\n]",junk,&newline);
            fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
               &R->Lng,&R->Lat,&R->Alt,junk,&newline);
            R->Lng *= D2R;
            R->Lat *= D2R;
            MagR = W->rad+R->Alt;
            R->PosW[0] = MagR*cos(R->Lng)*cos(R->Lat);
            R->PosW[1] = MagR*sin(R->Lng)*cos(R->Lat);
            R->PosW[2] = MagR*sin(R->Lat);
            A2C(312,R->Lng+HalfPi,HalfPi-R->Lat,0.0,R->CW);
            MTxV(W->CWN,R->PosW,R->PosN);
            MxM(R->CW,W->CWN,R->CN);
         }
         R->VelN[0] = -W->w*R->PosN[1];
         R->VelN[1] = W->w*R->PosN[0];
         R->VelN[2] = 0.0;
         R->wn[0] = 0.0;
         R->wn[1] = W->w*cos(R->Lat);
         R->wn[2] = W->w*sin(R->Lat);
         fscanf(infile,"%lf %lf %lf %[^\n] %[\n]",
            &R->ElastCoef,&R->DampCoef,&R->FricCoef,junk,&newline);
         fscanf(infile,"%s %[^\n] %[\n]",R->GeomFileName,junk,&newline);
         Geom = LoadWingsObjFile(ModelPath,R->GeomFileName,
            &Matl,&Nmatl,Geom,&Ngeom,&R->GeomTag,TRUE);
      }
      fclose(infile);
}
/**********************************************************************/
void InitLagrangePoints(void)
{
      long i,j;
      char LagsysName[3][20]={"Earth-Luna","Sun-Earth","Sun-Jupiter"};
      struct LagrangeSystemType *LS;
      struct WorldType *W1,*W2;

      LagSys[EARTHMOON].Body1 = EARTH;
      LagSys[EARTHMOON].Body2 = LUNA;
      LagSys[SUNEARTH].Body1 = SOL;
      LagSys[SUNEARTH].Body2 = EARTH;
      LagSys[SUNJUPITER].Body1 = SOL;
      LagSys[SUNJUPITER].Body2 = JUPITER;

      for(i=0;i<3;i++) {
         LS = &LagSys[i];
         W1 = &World[LS->Body1];
         W2 = &World[LS->Body2];
         strcpy(LS->Name,LagsysName[i]);
         if (LS->Exists) {
            if (!(W1->Exists && W2->Exists)) {
               printf("Lagrange System %s depends on worlds that don't exist.  Check Inp_Sim.txt\n",LS->Name);
               exit(1);
            }
            LS->mu1 = W1->mu;
            LS->mu2 = W2->mu;
            LS->rho = LS->mu2/(LS->mu1+LS->mu2);
            LS->SLR = W2->eph.SLR;
            LS->SMA = W2->eph.SMA;
            LS->ecc = W2->eph.ecc;
            LS->inc = W2->eph.inc;
            LS->RAAN = W2->eph.RAAN;
            LS->ArgP = W2->eph.ArgP;
            LS->tp = W2->eph.tp;
            LS->MeanRate = sqrt(LS->mu1/LS->SMA)/LS->SMA;
            LS->Period = TwoPi/LS->MeanRate;

            FindLagPtParms(LS);
            for(j=0;j<5;j++) {
               FindLagPtPosVel(DynTime,LS,j,
                  LS->LP[j].PosN,LS->LP[j].VelN,LS->CLN);
            }
         }
      }
}
/******************************************************************************/
long LoadDE430(char DE430Path[80],double JD)
{
      FILE *infile;
      double Block[1020];
      long BlockNum,NumEntries;
      long FoundBlock;
      char line[512];
      double JD1,JD2;
      long i,n,Ic,Iw;
      long Nseg,Start,N;
      struct Cheb3DType *Cheb;
      struct OrbitType *Eph;
      struct WorldType *W;
      double u,dudJD,T[20],U[20],P,dPdu;
      double rh[3],vh[3];
      double EarthMoonBaryPosH[3],EarthMoonBaryVelH[3];
      double EMRAT = 81.30056907419062; /* Earth-Moon mass ratio */
      double ZAxis[3] = {0.0,0.0,1.0}; 
      double PosJ[3],VelJ[3];
      double qJ2000H[4] = {-0.203123038887,  0.0,  0.0,  0.979153221449};

/* .. Select input file */
      if (JD < 2433264.5) {
         printf("JD earlier than DE430 input files.  Falling back to lower-precision planetary ephemerides.\n");
         return(1);
      }
      else if (JD < 2469808.5) {
         infile = FileOpen(DE430Path,"ascp1950.430","rt");
      }
      else if (JD < 2506352.5) {
         infile = FileOpen(DE430Path,"ascp2050.430","rt");
      }
      else if (JD < 2542864.5) {
         infile = FileOpen(DE430Path,"ascp2150.430","rt");
      }
      else {
         printf("JD later than available DE430 input files.  Falling back to lower-precision planetary ephemerides.\n");
         return(1);
      }

/* .. Search for block */
      FoundBlock = 0;
      while(!FoundBlock) {
         fgets(line,511,infile);
         if (sscanf(line,"%ld %ld",&BlockNum,&NumEntries) == 2) {
         fgets(line,511,infile);
            if (sscanf(line,"%lf %lf %lf",&Block[0],&Block[1],&Block[2]) == 3) {
               if (JD >= Block[0] && JD < Block[1]) {
                  FoundBlock = 1;
                  JD1 = Block[0];
                  JD2 = Block[1];
               }
            }
         }
      }
      
/* .. Load block */ 
      for(i=1;i<340;i++) {
         fgets(line,511,infile);
         sscanf(line,"%lf %lf %lf",&Block[3*i],&Block[3*i+1],&Block[3*i+2]);
      }
      fclose(infile);
            
/* .. Distribute to Worlds [Starting Entry (1-based), Order, Number of Segments] */
      /* Mercury [3 14 4] */
      Iw = MERCURY;
      Nseg = 4;
      Start = 3-1;
      N = 14;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Venus [171 10 2] */
      Iw = VENUS;
      Nseg = 2;
      Start = 171-1;
      N = 10;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Earth-Moon barycenter [231 13 2] */
      Iw = EARTH;
      Nseg = 2;
      Start = 231-1;
      N = 13;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Mars [309 11 1] */
      Iw = MARS;
      Nseg = 1;
      Start = 309-1;
      N = 11;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Jupiter [342 8 1] */
      Iw = JUPITER;
      Nseg = 1;
      Start = 342-1;
      N = 8;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Saturn [366 7 1] */
      Iw = SATURN;
      Nseg = 1;
      Start = 366-1;
      N = 7;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Uranus [387 6 1] */
      Iw = URANUS;
      Nseg = 1;
      Start = 387-1;
      N = 6;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Neptune [405 6 1] */
      Iw = NEPTUNE;
      Nseg = 1;
      Start = 405-1;
      N = 6;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Pluto [423 6 1] */
      Iw = PLUTO;
      Nseg = 1;
      Start = 423-1;
      N = 6;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Moon (geocentric) [441 13 8] */
      Iw = LUNA;
      Nseg = 8;
      Start = 441-1;
      N = 13;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }
      /* Sun [753 11 2] */
      Iw = SOL;
      Nseg = 2;
      Start = 753-1;
      N = 11;
      World[Iw].eph.Ncheb = Nseg;
      World[Iw].eph.Cheb = (struct Cheb3DType *) calloc(Nseg,sizeof(struct Cheb3DType));
      for(Ic=0;Ic<Nseg;Ic++) {
         Cheb = &World[Iw].eph.Cheb[Ic];
         Cheb->JD1 = JD1 + ((double) Ic)*(JD2-JD1)/((double) Nseg);
         Cheb->JD2 = JD2 - ((double) (Nseg-1-Ic))*(JD2-JD1)/((double) Nseg);
         Cheb->N = N;
         for(n=0;n<N;n++) {
            for(i=0;i<3;i++) {
               Cheb->Coef[i][n] = Block[Start + N*3*Ic + N*i+n];
            }
         }
      }

/* .. Initialize Planetary Pos/Vel */
      for(Iw=SOL;Iw<=LUNA;Iw++) {
         W = &World[Iw];
         Eph = &W->eph;
         /* Determine segment */
         Ic=0;
         while(TT.JulDay > Eph->Cheb[Ic].JD2) Ic++;
         /* Apply Chebyshev polynomials */
         Cheb = &Eph->Cheb[Ic];
         dudJD = 2.0/(Cheb->JD2-Cheb->JD1);
         u = (TT.JulDay-Cheb->JD1)*dudJD - 1.0;
         ChebyPolys(u,Cheb->N,T,U);
         for(i=0;i<3;i++) {
            ChebyInterp(T,U,Cheb->Coef[i],Cheb->N,&P,&dPdu);
            PosJ[i] = 1000.0*P;
            VelJ[i] = 1000.0*dPdu*dudJD/86400.0;
         }
         QTxV(qJ2000H,PosJ,Eph->PosN);
         QTxV(qJ2000H,VelJ,Eph->VelN);
      }
      /* Adjust for barycenters */
      /* Move planets from barycentric to Sun-centered */
      for(Iw=MERCURY;Iw<=PLUTO;Iw++) {
         W = &World[Iw];
         for(i=0;i<3;i++) {
            W->eph.PosN[i] -= World[SOL].eph.PosN[i];
            W->eph.VelN[i] -= World[SOL].eph.VelN[i];
            W->PosH[i] = W->eph.PosN[i];
            W->VelH[i] = W->eph.VelN[i];
         }
         W->PriMerAng = fmod(W->w*DynTime,TwoPi);
         SimpRot(ZAxis,W->PriMerAng,W->CWN);
      }
      /* Move Sun to origin */
      for(i=0;i<3;i++) {
         World[SOL].PosH[i] = 0.0;
         World[SOL].VelH[i] = 0.0;
         World[SOL].eph.PosN[i] = 0.0;
         World[SOL].eph.VelN[i] = 0.0;
      }
      /* Adjust Earth from Earth-Moon barycenter */
      for(i=0;i<3;i++) {
         EarthMoonBaryPosH[i] = World[EARTH].eph.PosN[i];
         EarthMoonBaryVelH[i] = World[EARTH].eph.VelN[i];
         World[EARTH].eph.PosN[i] -= World[LUNA].eph.PosN[i]/EMRAT;
         World[EARTH].eph.VelN[i] -= World[LUNA].eph.VelN[i]/EMRAT;
         World[EARTH].PosH[i] = World[EARTH].eph.PosN[i];
         World[EARTH].VelH[i] = World[EARTH].eph.VelN[i];
      }
      /* Move Moon from barycentric to Earth-centered */
      for(i=0;i<3;i++) {
         rh[i] = World[LUNA].eph.PosN[i]*(1.0+1.0/EMRAT);
         vh[i] = World[LUNA].eph.VelN[i]*(1.0+1.0/EMRAT);
         World[LUNA].PosH[i] = World[EARTH].PosH[i] + rh[i];
         World[LUNA].VelH[i] = World[EARTH].VelH[i] + vh[i];
      }
      /* Rotate Moon into ECI */
      MxV(World[EARTH].CNH,rh,World[LUNA].eph.PosN);
      MxV(World[EARTH].CNH,vh,World[LUNA].eph.VelN);
      World[LUNA].PriMerAng = LunaPriMerAng(TT.JulDay);
      SimpRot(ZAxis,World[LUNA].PriMerAng,World[LUNA].CWN);
      
      for(Iw=MERCURY;Iw<=LUNA;Iw++) {
         Eph = &World[Iw].eph;
         RV2Eph(DynTime,Eph->mu,Eph->PosN,Eph->VelN,
                &Eph->SMA,&Eph->ecc,&Eph->inc,
                &Eph->RAAN,&Eph->ArgP,&Eph->anom,
                &Eph->tp,&Eph->SLR,&Eph->alpha,&Eph->rmin,
                &Eph->MeanMotion,&Eph->Period);
      }

      return(0);
}
/**********************************************************************/
void LoadConstellations(void) {

      FILE *infile;
      char junk[120],newline,response[120];
      double RA,Dec;
      long i,j;
      struct ConstellationType *C;

      infile=FileOpen(ModelPath,"Constellations.txt","r");

      for (i=0;i<89;i++) {
         C = &Constell[i];
         fscanf(infile,"%s %s %ld %ld\n",C->Tag,response,&C->Nstars,&C->Nlines);
         C->Class = DecodeString(response);

         C->StarVec = CreateMatrix(C->Nstars,3);

         C->Star1 = (long *) calloc(C->Nlines,sizeof(long));
         C->Star2 = (long *) calloc(C->Nlines,sizeof(long));
         
         for (j=0; j<C->Nstars; j++) {
            fscanf(infile,"%lf %lf %[^\n] %[\n]",&RA,&Dec,junk,&newline);
            RA *= D2R;
            Dec *= D2R;
            C->StarVec[j][0] = cos(RA)*cos(Dec);
            C->StarVec[j][1] = sin(RA)*cos(Dec);
            C->StarVec[j][2] = sin(Dec);
         }

         for (j=0; j<C->Nlines; j++) {
            fscanf(infile,"%ld %ld %[^\n] %[\n]",&C->Star1[j],&C->Star2[j],junk,&newline);                  
         }
      }
      
      fclose(infile);
}
/**********************************************************************/
void LoadSchatten(void)
{
      FILE *infile;
      char junk[120],newline;
      long i,fileyear,filemonth;
      
      infile = FileOpen(ModelPath,"SolFlx0908_Schatten.txt","rt");

      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      for(i=0;i<410;i++) {
         fscanf(infile,"%ld %ld %lf %lf %lf %lf,%[^\n] %[\n]",
            &fileyear,&filemonth,
            &SchattenTable[1][i],&SchattenTable[2][i],
            &SchattenTable[3][i],&SchattenTable[4][i],junk,&newline);
         SchattenTable[0][i] = DateToJD(fileyear,filemonth,01,12,00,00);
      }
      fclose(infile);
}
/**********************************************************************/
void InitSim(int argc, char **argv)
{
      struct OrbitType *Eph;
      char junk[120],newline;
      double r1[3],rh[3],vh[3];
      double Zaxis[3] = {0.0,0.0,1.0};
      long Iorb,Isc,i,j,Ip,Im,Iw,Nm;
      long MinorBodiesExist;
      long JunkTag;
      double CGJ2000[3][3] =
         {{-0.054873956175539,-0.873437182224835,-0.483835031431981},
          { 0.494110775064704,-0.444828614979805, 0.746981957785302},
          {-0.867665382947348,-0.198076649977489, 0.455985113757595}};
      double qJ2000h[4] = {-0.203123038887,  0.0,  0.0,  0.979153221449};
      double CJ2000H[3][3];

      Pi = 4.0*atan(1.0);
      TwoPi = 2.0*Pi;
      HalfPi = 0.5*Pi;
      SqrtTwo = sqrt(2.0);
      SqrtHalf = sqrt(0.5);
      R2D = 180.0/Pi;
      D2R = Pi/180.0;
      GoldenRatio = (1.0+sqrt(5.0))/2.0;

      sprintf(InOutPath,"./InOut/");
      sprintf(ModelPath,"./Model/");
      if (argc > 1) sprintf(InOutPath,"./%s/",argv[1]);
      if (argc > 2) sprintf(ModelPath,"./%s/",argv[2]);

      const char *fname = "Inp_Sim.yaml";

      size_t lenstr = strlen(fname);
      if (lenstr > 4 && !strncmp(fname + lenstr-4, "yaml", 4))
          InitSimYAML(InOutPath, fname, &MinorBodiesExist);
      else
          InitSimTxt(InOutPath, fname, &MinorBodiesExist);

/* .. Load Materials */
      Nmatl = 0;
      Matl = AddMtlLib(ModelPath,"42.mtl",Matl,&Nmatl);
      ScaleSpecDiffFrac(Matl,Nmatl);

      /* Known bug: First Geom loaded in gets corrupted.
      Kludge fix: Load a sacrificial geom first.  */
      Geom = LoadWingsObjFile(ModelPath,"Point.obj",
            &Matl,&Nmatl,Geom,&Ngeom,&JunkTag,FALSE);

/* .. Time */
      if (TimeMode == EXTERNAL_TIME) {
         printf("Initializing with External Time\n");
         RealSystemTime(&UTC.Year,&UTC.doy,&UTC.Month,&UTC.Day,
            &UTC.Hour,&UTC.Minute,&UTC.Second,DTSIM);
      }         
      CivilTime = DateToTime(UTC.Year,UTC.Month,UTC.Day,
         UTC.Hour,UTC.Minute,UTC.Second);
      AtomicTime = CivilTime + LeapSec;
      DynTime0 = AtomicTime + 32.184;
      GpsTime = AtomicTime - 19.0;
      DynTime = DynTime0;

      TT.JulDay = TimeToJD(DynTime);
      TimeToDate(DynTime,&TT.Year,&TT.Month,&TT.Day,
         &TT.Hour,&TT.Minute,&TT.Second,DTSIM);
      TT.doy = MD2DOY(TT.Year,TT.Month,TT.Day);
      
      UTC.JulDay = TimeToJD(CivilTime);
      UTC.doy = MD2DOY(UTC.Year,UTC.Month,UTC.Day);
      
      JDToGpsTime(TT.JulDay,&GpsRollover,&GpsWeek,&GpsSecond);

/* .. Load Sun and Planets */
      LoadSun();
      LoadPlanets();
      /* JPL planetary ephems */
      if (EphemOption == EPH_DE430) LoadDE430(ModelPath,TT.JulDay);

/* .. Load Moons */
      if (World[EARTH].Exists) LoadMoonOfEarth();
      if (World[MARS].Exists) LoadMoonsOfMars();
      if (World[JUPITER].Exists) LoadMoonsOfJupiter();
      if (World[SATURN].Exists) LoadMoonsOfSaturn();
      if (World[URANUS].Exists) LoadMoonsOfUranus();
      if (World[NEPTUNE].Exists) LoadMoonsOfNeptune();
      if (World[PLUTO].Exists) LoadMoonsOfPluto();

/* .. Asteroids and Comets */
      if (MinorBodiesExist) LoadMinorBodies();
      else Nmb = 0;

/* .. Regions */
      LoadRegions();

/* .. Galactic Frame */
      Q2C(qJ2000h,CJ2000H);
      MxM(CGJ2000,CJ2000H,CGH);

/* .. Ground Station Locations */
      for(i=0;i<Ngnd;i++) {
         if (GroundStation[i].Exists && !World[GroundStation[i].World].Exists) {
            printf("Ground Station[%ld].World doesn't exist.\n",i);
         }
         GroundStation[i].PosW[0] = World[GroundStation[i].World].rad*
            cos(GroundStation[i].lng*D2R)*cos(GroundStation[i].lat*D2R);
         GroundStation[i].PosW[1] = World[GroundStation[i].World].rad*
            sin(GroundStation[i].lng*D2R)*cos(GroundStation[i].lat*D2R);
         GroundStation[i].PosW[2] = World[GroundStation[i].World].rad*
            sin(GroundStation[i].lat*D2R);
      }

/* .. Locate Luna */
      if (World[LUNA].Exists){
         Eph = &World[LUNA].eph;
         /* Meeus computes Luna Position in geocentric ecliptic */
         LunaPosition(TT.JulDay,rh);
         LunaPosition(TT.JulDay+0.01,r1);
         for(j=0;j<3;j++) vh[j] = (r1[j]-rh[j])/(864.0);
         /* Convert to Earth's N frame */
         MxV(World[EARTH].CNH,rh,Eph->PosN);
         MxV(World[EARTH].CNH,vh,Eph->VelN);
         /* Find Luna's osculating elements */
         RV2Eph(DynTime,Eph->mu,Eph->PosN,Eph->VelN,
            &Eph->SMA,&Eph->ecc,&Eph->inc,&Eph->RAAN,
            &Eph->ArgP,&Eph->anom,&Eph->tp,&Eph->SLR,&Eph->alpha,
            &Eph->rmin,&Eph->MeanMotion,&Eph->Period);
         World[LUNA].PriMerAng =
            atan2(Eph->PosN[1],Eph->PosN[0])+Pi;
         SimpRot(Zaxis,World[LUNA].PriMerAng,World[LUNA].CWN);
         C2Q(World[LUNA].CWN,World[LUNA].qwn);
         for(j=0;j<3;j++) {
            World[LUNA].PosH[j] = rh[j] + World[EARTH].PosH[j];
            World[LUNA].VelH[j] = vh[j] + World[EARTH].VelH[j];
         }
      }

/* .. Other planets' moons */
      for(Ip=MARS;Ip<=PLUTO;Ip++) {
         if (World[Ip].Exists) {
            Nm = World[Ip].Nsat;
            for(Im=0;Im<Nm;Im++) {
               Iw = World[Ip].Sat[Im];
               Eph = &World[Iw].eph;
               Eph2RV(Eph->mu, Eph->SLR,Eph->ecc,Eph->inc,Eph->RAAN,
                      Eph->ArgP,DynTime-Eph->tp,Eph->PosN,Eph->VelN,&Eph->anom);
               World[Iw].PriMerAng = fmod(World[i].w*DynTime,TwoPi);
               SimpRot(Zaxis,World[Iw].PriMerAng,World[Iw].CWN);
               C2Q(World[Iw].CWN,World[Iw].qwn);
               MTxV(World[Ip].CNH,Eph->PosN,rh);
               MTxV(World[Ip].CNH,Eph->VelN,vh);
               for(i=0;i<3;i++) {
                  World[Iw].PosH[i] = rh[i] + World[Ip].PosH[i];
                  World[Iw].VelH[i] = vh[i] + World[Ip].VelH[i];
               }
            }
         }
      }

/* .. Note that some moons are so dominated by their planet that   */
/*    they don't really have a sphere of influence!                */
      /*
      for(Iw=1;Iw<NWORLD;Iw++) {
         if (0.5*World[Iw].RadOfInfluence < World[Iw].rad)
            printf("World %s is bigger than its inner sphere of influence\n",World[Iw].Name);
         if (2.0*World[Iw].RadOfInfluence < World[Iw].rad)
            printf("World %s is bigger than its outer sphere of influence\n",World[Iw].Name);
      }
      */

      InitLagrangePoints();
      for(Iorb=0;Iorb<Norb;Iorb++) {
         if (Orb[Iorb].Exists) InitOrbit(&Orb[Iorb]);
      }
      OrbitMotion(DynTime);
      for (Isc=0;Isc<Nsc;Isc++) {
         if (SC[Isc].Exists) {
            InitSpacecraft(&SC[Isc]);
         }
      }

      LoadTdrs();
      
      RNG = CreateRandomProcess(1);

      LoadConstellations();
      
      LoadSchatten();
}

/* #ifdef __cplusplus
** }
** #endif
*/
