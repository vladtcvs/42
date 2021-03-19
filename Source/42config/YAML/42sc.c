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
#include "42config/YAML/yaml_functions.h"
#include "42config/YAML/42sc.h"
#include "42config/42config.h"

#include <ctype.h>
#include <libfyaml.h>


/* #ifdef __cplusplus
** namespace _42 {
** using namespace Kit;
** #endif
*/

/**********************************************************************/

void read_wheel(struct fy_node *wheel, struct WhlType *whl)
{
    printf("Load wheel %s\n", fy_node_get_path(wheel));
    fy_node_scanf(wheel, "/InitialMomentum %lf", &whl->H);
    yaml_read_vector(wheel, "/AxisOrientation", whl->A);
    UNITV(whl->A);
    PerpBasis(whl->A, whl->Uhat, whl->Vhat);
    fy_node_scanf(wheel, "/MaximalTorque %lf", &whl->Tmax);
    fy_node_scanf(wheel, "/MaximalMomentum %lf", &whl->Hmax);
    fy_node_scanf(wheel, "/RotorInertia %lf", &whl->J);
    fy_node_scanf(wheel, "/StaticImbalance %lf", &whl->Ks);
    fy_node_scanf(wheel, "/DynamicImbalance %lf", &whl->Kd);
    fy_node_scanf(wheel, "/FlexNode %ld", &whl->FlexNode);
    /* Convert from g-cm and g-cm^2 to kg-m and kg-m^2 */
    whl->Ks *= 1.0E-5;
    whl->Kd *= 1.0E-7;
}

void read_mtb(struct fy_node *mtb, struct MTBType *m)
{
    printf("Load mtb %s\n", fy_node_get_path(mtb));
    fy_node_scanf(mtb, "/Saturation %lf", &m->Mmax);
    yaml_read_vector(mtb, "AxisOrientation", m->A);
    UNITV(m->A);
    fy_node_scanf(mtb, "/FlexNode %ld", &m->FlexNode);
}

void read_thruster(struct fy_node *thr, struct ThrType *t)
{
    printf("Load thruster %s\n", fy_node_get_path(thr));
    fy_node_scanf(thr, "/ThrustForce %lf", &t->Fmax);
    fy_node_scanf(thr, "/Body %ld", &t->Body);
    yaml_read_vector(thr, "AxisOrientation", t->A);
    UNITV(t->A);
    yaml_read_vector(thr, "/Position", t->PosB);
    fy_node_scanf(thr, "/FlexNode %ld", &t->FlexNode);
}

void read_gyro(struct fy_node *gyro, struct GyroType *Gyro)
{
    printf("Load gyro %s\n", fy_node_get_path(gyro));
    double BiasTime;
    fy_node_scanf(gyro, "/SampleTime %lf", &Gyro->SampleTime);
    Gyro->MaxCounter = (long) (Gyro->SampleTime/DTSIM+0.5);
    if (Gyro->MaxCounter < 1)
    {
        Gyro->MaxCounter = 1;
        Gyro->SampleTime = DTSIM;
        printf("Info:  %s/SampleTime was smaller than DTSIM.  It has been adjusted to be DTSIM.\n", fy_node_get_path(gyro));
    }
    Gyro->SampleCounter = Gyro->MaxCounter;
    
    yaml_read_vector(gyro, "AxisOrientation", Gyro->Axis);
    UNITV(Gyro->Axis);
    
    fy_node_scanf(gyro, "/MaxRate %lf", &Gyro->MaxRate);
    Gyro->MaxRate *= D2R;

    fy_node_scanf(gyro, "/ScaleFactorError %lf", &Gyro->Scale);
    Gyro->Scale = 1.0+1.0E-6*Gyro->Scale;

    fy_node_scanf(gyro, "/Quantization %lf", &Gyro->Quant);
    Gyro->Quant *= D2R/3600.0;

    fy_node_scanf(gyro, "/AngleRandomWalk %lf", &Gyro->SigV);
    Gyro->SigV *= D2R/60.0; /* from deg/rt-hr to rad/rt-sec */

    fy_node_scanf(gyro, "/BiasStability/Value %lf", &Gyro->SigU);
    fy_node_scanf(gyro, "/BiasStability/Timespan %lf", &BiasTime);
    Gyro->SigU *= D2R/3600.0/sqrt(BiasTime*3600.0);
    
    fy_node_scanf(gyro, "/AngleNoise %lf", &Gyro->SigE);
    Gyro->SigE *= D2R/3600.0;

    fy_node_scanf(gyro, "/InitialBias %lf", &Gyro->Bias);
    Gyro->Bias *= D2R/3600.0;

    fy_node_scanf(gyro, "/FlexNode %ld", &Gyro->FlexNode);
            
    Gyro->BiasStabCoef = Gyro->SigU*sqrt(Gyro->SampleTime);
    Gyro->ARWCoef = sqrt(Gyro->SigV*Gyro->SigV/Gyro->SampleTime 
                               + Gyro->SigU*Gyro->SigU*Gyro->SampleTime/12.0);
    Gyro->AngNoiseCoef = Gyro->SigE/sqrt(Gyro->SampleTime);
    Gyro->CorrCoef = 1.0-Gyro->SampleTime/(BiasTime*3600.0);
    Gyro->Angle = 0.0;
}

void read_magnetometer(struct fy_node *mag, struct MagnetometerType *MAG)
{
    printf("Load magnetometer %s\n", fy_node_get_path(mag));
    fy_node_scanf(mag, "/SampleTime %lf", &MAG->SampleTime);
    MAG->MaxCounter = (long) (MAG->SampleTime/DTSIM+0.5);
    if (MAG->MaxCounter < 1)
    {
        MAG->MaxCounter = 1;
        MAG->SampleTime = DTSIM;
        printf("Info:  %s/SampleTime was smaller than DTSIM.  It has been adjusted to be DTSIM.\n", fy_node_get_path(mag));
    }
    MAG->SampleCounter = MAG->MaxCounter;

    yaml_read_vector(mag, "AxisOrientation", MAG->Axis);
    UNITV(MAG->Axis);

    fy_node_scanf(mag, "/Saturation %lf", &MAG->Saturation);

    fy_node_scanf(mag, "/ScaleFactorError %lf", &MAG->Scale);
    MAG->Scale = 1.0+1.0E-6*MAG->Scale;

    fy_node_scanf(mag, "/Quantization %lf", &MAG->Quant);
    fy_node_scanf(mag, "/Noise %lf", &MAG->Noise);
    fy_node_scanf(mag, "/FlexNode %ld", &MAG->FlexNode);
}
      
void read_sun_sensor(struct fy_node *css, struct CssType *CSS)
{
    printf("Load sun sensor %s\n", fy_node_get_path(css));
    fy_node_scanf(css, "/SampleTime %lf", &CSS->SampleTime);
    CSS->MaxCounter = (long) (CSS->SampleTime/DTSIM+0.5);
    if (CSS->MaxCounter < 1)
    {
        CSS->MaxCounter = 1;
        CSS->SampleTime = DTSIM;
        printf("Info:  %s/SampleTime was smaller than DTSIM.  It has been adjusted to be DTSIM.\n", fy_node_get_path(css));
    }
    CSS->SampleCounter = CSS->MaxCounter;

    fy_node_scanf(css, "Body %ld", &CSS->Body);
    yaml_read_vector(css, "AxisOrientation", CSS->Axis);
    UNITV(CSS->Axis);

    fy_node_scanf(css, "/HalfConeAngle %lf", &CSS->FovHalfAng);
    CSS->FovHalfAng *= D2R;
    CSS->CosFov = cos(CSS->FovHalfAng);

    fy_node_scanf(css, "/ScaleFactor %lf", &CSS->Scale);
    fy_node_scanf(css, "/Quantization %lf", &CSS->Quant);
    fy_node_scanf(css, "/FlexNode %ld", &CSS->FlexNode);
}

void read_fine_sun_sensor(struct fy_node *fss, struct FssType *FSS)
{
    printf("Load fine sun sensor %s\n", fy_node_get_path(fss));
    double Ang1, Ang2, Ang3;
    long Seq;
    int i;
    fy_node_scanf(fss, "/SampleTime %lf", &FSS->SampleTime);
    FSS->MaxCounter = (long) (FSS->SampleTime/DTSIM+0.5);
    if (FSS->MaxCounter < 1)
    {
        FSS->MaxCounter = 1;
        FSS->SampleTime = DTSIM;
        printf("Info:  %s.SampleTime was smaller than DTSIM.  It has been adjusted to be DTSIM.\n", fy_node_get_path(fss));
    }
    FSS->SampleCounter = FSS->MaxCounter;

    yaml_read_angles(fss, "Mounting/Angles", &Ang1,&Ang2,&Ang3);
    fy_node_scanf(fss, "/Mounting/Sequence %ld", &Seq);
    A2C(Seq, Ang1*D2R, Ang2*D2R, Ang3*D2R, FSS->CB);
    C2Q(FSS->CB, FSS->qb);
    
    fy_node_scanf(fss, "/FOV/X %lf /FOV/Y %lf", &FSS->FovHalfAng[0], &FSS->FovHalfAng[1]);
    for (i = 0; i < 2; i++)
    {
        FSS->FovHalfAng[i] *= 0.5*D2R;
    }

    fy_node_scanf(fss, "/NoiseEquivalentAngle %lf", &FSS->NEA);
    FSS->NEA *= D2R;

    fy_node_scanf(fss, "/Quantization %lf", &FSS->Quant);
    FSS->Quant *= D2R;
    fy_node_scanf(fss, "/FlexNode %ld", &FSS->FlexNode);
}

void read_star_tracker(struct fy_node *st, struct StarTrackerType *ST)
{
    printf("Load star tracker %s\n", fy_node_get_path(st));
    double Ang1, Ang2, Ang3;
    long Seq;
    int i;
    
    fy_node_scanf(st, "/SampleTime %lf", &ST->SampleTime);        
    ST->MaxCounter = (long) (ST->SampleTime/DTSIM+0.5);
    if (ST->MaxCounter < 1)
    {
        ST->MaxCounter = 1;
        ST->SampleTime = DTSIM;
        printf("Info:  %s.SampleTime was smaller than DTSIM.  It has been adjusted to be DTSIM.\n", fy_node_get_path(st));
    }
    ST->SampleCounter = ST->MaxCounter;

    yaml_read_angles(st, "Mounting/Angles", &Ang1, &Ang2, &Ang3);
    fy_node_scanf(st, "/Mounting/Sequence %ld", &Seq);
    A2C(Seq, Ang1*D2R, Ang2*D2R, Ang3*D2R, ST->CB);
    C2Q(ST->CB, ST->qb);

    fy_node_scanf(st, "/FOV/X %lf /FOV/Y %lf", &ST->FovHalfAng[0], &ST->FovHalfAng[1]);
    for(i=0;i<2;i++)
    {
        ST->FovHalfAng[i] *= 0.5*D2R;
        ST->CosFov[i] = cos(ST->FovHalfAng[i]);
    }
    
    fy_node_scanf(st, "/Exclusion/Sun %lf /Exclusion/Earth %lf /Exclusion/Moon %lf", &ST->SunExclAng,&ST->EarthExclAng,&ST->MoonExclAng);
    ST->SunExclAng *= D2R;
    ST->EarthExclAng *= D2R;
    ST->MoonExclAng *= D2R;
    ST->CosSunExclAng = cos(ST->SunExclAng);
    ST->CosEarthExclAng = cos(ST->EarthExclAng);
    ST->CosMoonExclAng = cos(ST->MoonExclAng);

    yaml_read_vector(st, "NoiseEquivalentAngle", ST->NEA);        
    for(i = 0; i < 3; i++)
        ST->NEA[i] *= D2R/3600.0;

    fy_node_scanf(st, "/FlexNode %ld", &ST->FlexNode);
}

void read_gps(struct fy_node *gps, struct GpsType *GPS)
{
    printf("Load GPS %s\n", fy_node_get_path(gps));
    fy_node_scanf(gps, "/SampleTime %lf", &GPS->SampleTime);
    GPS->MaxCounter = (long) (GPS->SampleTime/DTSIM+0.5);
    if (GPS->MaxCounter < 1)
    {
        GPS->MaxCounter = 1;
        GPS->SampleTime = DTSIM;
        printf("Info:  %s/SampleTime was smaller than DTSIM.  It has been adjusted to be DTSIM.\n", fy_node_get_path(gps));
    }
    GPS->SampleCounter = GPS->MaxCounter;

    fy_node_scanf(gps, "/PositionNoise %lf", &GPS->PosNoise);
    fy_node_scanf(gps, "/VelocityNoise %lf", &GPS->VelNoise);
    fy_node_scanf(gps, "/TimeNoise %lf", &GPS->TimeNoise);
    fy_node_scanf(gps, "/FlexNode %ld", &GPS->FlexNode);
}

void read_accelerometer(struct fy_node *acc, struct AccelType *Accel)
{
    printf("Load accelerometer %s\n", fy_node_get_path(acc));
    double BiasTime;
    fy_node_scanf(acc, "/SampleTime %lf", &Accel->SampleTime);
    Accel->MaxCounter = (long) (Accel->SampleTime/DTSIM+0.5);
    if (Accel->MaxCounter < 1)
    {
        Accel->MaxCounter = 1;
        Accel->SampleTime = DTSIM;
        printf("Info:  %s/SampleTime was smaller than DTSIM.  It has been adjusted to be DTSIM.\n", fy_node_get_path(acc));
    }
    Accel->SampleCounter = Accel->MaxCounter;

    yaml_read_vector(acc, "Position", Accel->PosB);
    yaml_read_vector(acc, "AxisOrientation", Accel->Axis);
    UNITV(Accel->Axis);

    fy_node_scanf(acc, "/MaxAcceleration %lf", &Accel->MaxAcc);
    fy_node_scanf(acc, "/ScaleFactorError %lf", &Accel->Scale);
    Accel->Scale = 1.0+1.0E-6*Accel->Scale;
    
    fy_node_scanf(acc, "/Quantization %lf", &Accel->Quant);
    
    fy_node_scanf(acc, "/DV_RandomWalk %lf", &Accel->SigV);
    Accel->SigV /= 60.0; /* from m/s/rt-hr to m/s/rt-sec */

    fy_node_scanf(acc, "/BiasStability/Value %lf", &Accel->SigU);
    fy_node_scanf(acc, "/BiasStability/Timespan %lf", &BiasTime);
    Accel->SigU /= sqrt(BiasTime*3600.0);

    fy_node_scanf(acc, "/DV_Noise %lf", &Accel->SigE);
    fy_node_scanf(acc, "/InitialBias %lf", &Accel->Bias);

    fy_node_scanf(acc, "/FlexNode %ld", &Accel->FlexNode);

    Accel->BiasStabCoef = Accel->SigU*sqrt(Accel->SampleTime);
    Accel->DVRWCoef = sqrt(Accel->SigV*Accel->SigV/Accel->SampleTime 
                               + Accel->SigU*Accel->SigU*Accel->SampleTime/12.0);
    Accel->DVNoiseCoef = Accel->SigE/sqrt(Accel->SampleTime);
    Accel->CorrCoef = 1.0-Accel->SampleTime/(BiasTime*3600.0);
    Accel->DV = 0.0;
}

void InitSpacecraftYAML(struct SCType *S, double *PosVec, double *VelVec, long *UseCM)
{
    double CBL[3][3],CBF[3][3];
    long i,j,k,Ib,Ig,Bi,Bo;
    double wlnb[3];
    double wbn[3],CBN[3][3],qbn[4];
    double Ang1,Ang2,Ang3;
    double pIn[3],pOut[3];
    long Seq;
    long i1,i2,i3;
    struct JointType *G;
    struct BodyType *B;

    char value[256+1];

    FILE *infile=FileOpen(InOutPath, S->FileName, "r");
    struct fy_document *fyd = fy_document_build_from_fp(NULL, infile);
    if (fyd == NULL)
    {
        printf("Can not open SC description file: %s\n", S->FileName);
        exit(1);
    }
    struct fy_node *root = fy_document_root(fyd);

/*  .. Basic ship parameters */
    struct fy_node *ship = fy_node_by_path(root, "/Ship", -1, 0);
    fy_node_scanf(ship, "/Label %s", S->Label);
    fy_node_scanf(ship, "/Sprite %s", S->SpriteFileName);
    fy_node_scanf(ship, "/FSW/FSW %256s", value);
    S->FswTag = DecodeString(value);
    fy_node_scanf(ship, "/FSW/SampleTime %lf", &S->FswSampleTime);

    S->FswMaxCounter = (long) (S->FswSampleTime/DTSIM+0.5);
    if (S->FswMaxCounter < 1)
    {
        S->FswMaxCounter = 1;
        S->FswSampleTime = DTSIM;
        printf("Info:  FswSampleTime was smaller than DTSIM.  It has been adjusted to be DTSIM.\n");
    }
    S->FswSampleCounter = S->FswMaxCounter;
    S->InitAC = 1;

/* .. Orbit Parameters */
    struct fy_node *orbit = fy_node_by_path(root, "/Orbit_Parameters", -1, 0);
    fy_node_scanf(orbit, "/Type %256s", value);
    S->OrbDOF = DecodeString(value);

    fy_node_scanf(orbit, "/Position_Point %256s", value);
    *UseCM = DecodeString(value);

    yaml_read_vector(orbit, "Position", PosVec);
    yaml_read_vector(orbit, "Velocity", VelVec);

/* .. Initial Attitude */
    struct fy_node *att = fy_node_by_path(root, "/Initial_Attitude", -1, 0);

    yaml_read_vector(att, "Angular_Vel/Vel", wbn);
    wbn[0] *= D2R;
    wbn[1] *= D2R;
    wbn[2] *= D2R;
    fy_node_scanf(att, "/Angular_Vel/Frame %256s", value);
    if (!strcmp(value, "LVLH"))
    {
        /* Add LVLH rate to wn */
        MxV(CBN, Orb[S->RefOrb].wln, wlnb);
        for (j = 0; j < 3; j++)
            wbn[j] += wlnb[j];
    }

    fy_node_scanf(att, "/Orientation/Type %256s", value);
    if (!strcmp(value, "Angles"))
    {
        yaml_read_angles(att, "Orientation/Angles", &Ang1, &Ang2, &Ang3);
        fy_node_scanf(att, "/Orientation/Sequence %ld", &Seq);
        A2C(Seq, Ang1*D2R, Ang2*D2R, Ang3*D2R, CBN);
        C2Q(CBN, qbn);
    }
    else
    {
        yaml_read_quaternion(att, "Quaternion", qbn);
        Q2C(qbn, CBN);
    }

    fy_node_scanf(att, "/Orientation/Frame %256s", value);
    if (!strcmp(value, "LVLH"))
    {
        /* Adjust CBN */
        for (j = 0; j < 3; j++)
            for(k = 0;k < 3;k++)
                CBL[j][k] = CBN[j][k];
        
        MxM(CBL, Orb[S->RefOrb].CLN, CBN);
        C2Q(CBN, qbn);
    }
    else if (!strcmp(value, "Formation"))
    {
        /* Adjust CBN */
        for(j = 0; j < 3; j++)
            for(k = 0; k < 3; k++)
                CBF[j][k] = CBN[j][k];
        
        MxM(CBF, Frm[S->RefOrb].CN, CBN);
        C2Q(CBN, qbn);
    }
    MxMT(CBN,Frm[S->RefOrb].CN,S->CF);

/* .. Dynamics Flags */
    struct fy_node *flags = fy_node_by_path(root, "/Flags", -1, 0);
    fy_node_scanf(flags, "/Rotation %256s", value);
    S->RotDOF=DecodeString(value);
    fy_node_scanf(flags, "/PassiveForces %256s", value);
    S->PassiveJointFrcTrqEnabled=DecodeString(value);
    fy_node_scanf(flags, "/ConstraintForces %256s", value);
    S->ConstraintsRequested=DecodeString(value);
    fy_node_scanf(flags, "/MassPropsRef %256s", value);
    S->RefPt=DecodeString(value);
    fy_node_scanf(flags, "/Flex %256s", value);
    S->FlexActive=DecodeString(value);
    fy_node_scanf(flags, "/Flex2ndOrder %256s", value);
    S->IncludeSecondOrderFlexTerms=DecodeString(value);
    fy_node_scanf(flags, "/DragCoefficient %lf", &S->DragCoef);

/* .. Body parameters */
    struct fy_node *bodies = fy_node_by_path(root, "/Bodies", -1, 0);

    S->Nb = fy_node_sequence_item_count(bodies);
    S->Ng = S->Nb - 1;
    S->B = (struct BodyType *) calloc(S->Nb,sizeof(struct BodyType));
    if (S->B == NULL)
    {
        printf("S->B calloc returned null pointer.  Bailing out!\n");
        exit(1);
    }
    S->G = (struct JointType *) calloc(S->Ng,sizeof(struct JointType));
    if (S->G == NULL)
    {
        printf("S->G calloc returned null pointer.  Bailing out!\n");
        exit(1);
    }

    /* Load B[0] initial attitude */
    for(j=0; j<3; j++)
    {
        S->B[0].wn[j] = wbn[j];
        for (k = 0; k < 3; k++)
            S->B[0].CN[j][k] = CBN[j][k];
    }
    for (j = 0; j < 4; j++)
        S->B[0].qn[j] = qbn[j];

/* .. Body Ib */
    for (Ib = 0; Ib < S->Nb; Ib++)
    {
        struct fy_node *body = fy_node_sequence_get_by_index(bodies, Ib);
        printf("body path = %s\n", fy_node_get_path(body));
        B = &S->B[Ib];
        fy_node_scanf(body, "/Mass %lf", &B->mass);
        yaml_read_3_vals(body, "MomentsOfInertia", &B->I[0][0], &B->I[1][1], &B->I[2][2]);
        yaml_read_3_vals(body, "ProductsOfInertia", &B->I[0][1], &B->I[0][2], &B->I[1][2]);

        B->I[0][1] *= -1.0;
        B->I[0][2] *= -1.0;
        B->I[1][2] *= -1.0;
        B->I[1][0] = B->I[0][1];
        B->I[2][0] = B->I[0][2];
        B->I[2][1] = B->I[1][2];

        yaml_read_vector(body, "CenterMassPosition", B->cm);
        yaml_read_vector(body, "EmbeddedMomentum", B->Hgyro);

        fy_node_scanf(body, "/ModelFile %s", B->GeomFileName);
        fy_node_scanf(body, "/FlexFile %s", B->FlexFileName);
        if (S->RefPt == REFPT_JOINT)
        {
            for(i = 0; i < 3; i++)
                B->c[i] = B->mass*B->cm[i];
        }
        else
        {
            for(i = 0; i < 3; i++)
                B->c[i] = 0.0;
        }
    }

/* .. Joint Parameters */
    /* Read Spacecraft input file */
    struct fy_node *joints = fy_node_by_path(root, "/Joints", -1, 0);
    for (Ig = 0; Ig < S->Ng; Ig++)
    {
        struct fy_node *joint = fy_node_sequence_get_by_index(joints, Ig);
        G = &S->G[Ig];

        fy_node_scanf(joint, "/InnerBody %ld", &G->Bin);
        fy_node_scanf(joint, "/OuterBody %ld", &G->Bout);
        if (G->Bin > G->Bout)
        {
            printf("Yo!  SC[%ld].G[%ld] inner body index (%ld) is greater than outer body index (%ld)\n", S->ID, Ig, G->Bin, G->Bout);
            printf("You must define inner bodies before outer bodies!\n");
            exit(1);
        }
        Bi=G->Bin;
        Bo=G->Bout;
        S->B[Bo].Gin = Ig;

        yaml_read_vector(joint, "InnerBodyVector", pIn);
        yaml_read_vector(joint, "OuterBodyVector", pOut);

        // Rotations
        struct fy_node *rot = fy_node_by_path(joint, "Rotation", -1, 0);

        fy_node_scanf(rot, "/DOF %ld", &G->RotDOF);
        fy_node_scanf(rot, "/Sequence %ld", &G->RotSeq);
        fy_node_scanf(rot, "/Type %256s", value);
        G->IsSpherical = DecodeString(value);
        if (G->RotSeq < 100)
        {
            printf("Invalid RotSeq %ld for SC[%ld].G[%ld].  All three axes required.\n",
                   G->RotSeq, S->ID, Ig);
            exit(1);
        }
        i3 = G->RotSeq % 10;         /* Pick off third digit */
        i2 = (G->RotSeq % 100)/10;   /* Extract second digit */
        i1 = G->RotSeq/100;          /* Pick off first digit */
        if (i1 == i2 || i1 == i3 || i2 == i3)
        {
            printf("Invalid RotSeq %ld for SC[%ld].G[%ld].  Repeated indices are not allowed.\n",
                   G->RotSeq, S->ID, Ig);
            exit(1);
        }

        fy_node_scanf(rot, "/Locked/0 %s", value);
        G->RotLocked[0] = DecodeString(value);
        fy_node_scanf(rot, "/Locked/1 %s", value);
        G->RotLocked[1] = DecodeString(value);
        fy_node_scanf(rot, "/Locked/2 %s", value);
        G->RotLocked[2] = DecodeString(value);

        yaml_read_vector(rot, "PassiveSpringCoefficients", G->RotSpringCoef);
        yaml_read_vector(rot, "PassiveDamplingCoefficients", G->RotDampCoef);
        
        yaml_read_vector(rot, "Angles", G->Ang);
        yaml_read_vector(rot, "Vel", G->AngRate);
        
        for (k = 0; k < 3; k++)
        {
            G->Ang[k] *= D2R;
            G->AngRate[k] *= D2R;
        }

        // Translations
        struct fy_node *trn = fy_node_by_path(joint, "Translation", -1, 0);

        fy_node_scanf(trn, "/DOF %ld", &G->TrnDOF);
        fy_node_scanf(trn, "/Sequence %ld", &G->TrnSeq);

        if (G->TrnSeq < 100)
        {
            printf("Invalid TrnSeq %ld for SC[%ld].G[%ld].  All three axes required.\n",
                    G->TrnSeq,S->ID,Ig);
            exit(1);
        }
        i3 = G->TrnSeq % 10;         /* Pick off third digit */
        i2 = (G->TrnSeq % 100)/10;   /* Extract second digit */
        i1 = G->TrnSeq/100;          /* Pick off first digit */
        if (i1 == i2 || i1 == i3 || i2 == i3)
        {
            printf("Invalid TrnSeq %ld for SC[%ld].G[%ld].  Repeated indices are not allowed.\n",
                   G->TrnSeq,S->ID,Ig);
            exit(1);
        }
        
        fy_node_scanf(trn, "/Locked/0 %s", value);
        G->TrnLocked[0] = DecodeString(value);
        fy_node_scanf(trn, "/Locked/1 %s", value);
        G->TrnLocked[1] = DecodeString(value);
        fy_node_scanf(trn, "/Locked/2 %s", value);
        G->TrnLocked[2] = DecodeString(value);

        yaml_read_vector(trn, "PassiveSpringCoefficients", G->TrnSpringCoef);
        yaml_read_vector(trn, "PassiveDamplingCoefficients", G->TrnDampCoef);

        yaml_read_vector(trn, "Displacement", G->Pos);
        yaml_read_vector(trn, "Vel", G->PosRate);

        // Bi Gi and Go Bo
        yaml_read_angles(joint, "Bi_2_Gi/Angles", &Ang1, &Ang2, &Ang3);
        fy_node_scanf(joint, "/Bi_2_Gi/Sequence %ld", &Seq);
        A2C(Seq, Ang1*D2R, Ang2*D2R, Ang3*D2R, G->CGiBi);

        yaml_read_angles(joint, "Go_2_Bo/Angles", &Ang1, &Ang2, &Ang3);
        fy_node_scanf(joint, "/Go_2_Bo/Sequence %ld", &Seq);
        A2C(Seq, Ang1*D2R, Ang2*D2R, Ang3*D2R, G->CBoGo);

        if (S->RefPt == REFPT_JOINT)
        {
            for (j = 0; j < 3; j++)
            {
                G->RigidRin[j] = pIn[j];
                G->RigidRout[j] = pOut[j];
            }
        }
        else
        {
            for (j = 0; j < 3; j++)
            {
                G->RigidRin[j] = pIn[j] - S->B[Bi].cm[j];
                G->RigidRout[j] = pOut[j] - S->B[Bo].cm[j];
            }
        }
    }

#define LOAD_COMPONENT(name, type, vals_field, num_field, read_node)                \
    do                                                                              \
    {                                                                               \
        char str[256+1];                                                          \
        struct fy_node *nodes = fy_node_by_path(root, name, -1, 0);               \
        int Nf = fy_node_sequence_item_count(nodes);                                \
        int index, id;                                                              \
        num_field = 0;                                                              \
                                                                                    \
        for (index = 0; index < Nf; index++)                                        \
        {                                                                           \
            struct fy_node *node = fy_node_sequence_get_by_index(nodes, index);     \
            fy_node_scanf(node, "/Enabled %256s", str);                           \
            long en = DecodeString(str);                                          \
            if (en)                                                                 \
                num_field++;                                                        \
        }                                                                           \
        vals_field = (type *) calloc(num_field, sizeof(type));                      \
        for (id = 0, index = 0; index < num_field; index++)                         \
        {                                                                           \
            struct fy_node *node = fy_node_sequence_get_by_index(nodes, index);     \
            fy_node_scanf(node, "/Enabled %256s", str);                           \
            long en = DecodeString(str);                                          \
            if (en == TRUE)                                                         \
                read_node(node, &((vals_field)[id++]));                             \
        }                                                                           \
    } while (0)
    

    LOAD_COMPONENT("Wheels", struct WhlType, S->Whl, S->Nw, read_wheel);
    LOAD_COMPONENT("MTBs", struct MTBType, S->MTB, S->Nmtb, read_mtb);
    LOAD_COMPONENT("Thrusters", struct ThrType, S->Thr, S->Nthr, read_thruster);
    LOAD_COMPONENT("Gyros", struct GyroType, S->Gyro, S->Ngyro, read_gyro);
    LOAD_COMPONENT("Magnetometers", struct MagnetometerType, S->MAG, S->Nmag, read_magnetometer);
    LOAD_COMPONENT("SunSensors", struct CssType, S->CSS, S->Ncss, read_sun_sensor);
    LOAD_COMPONENT("FineSunSensors", struct FssType, S->FSS, S->Nfss, read_fine_sun_sensor);
    LOAD_COMPONENT("StarTrackers", struct StarTrackerType, S->ST, S->Nst, read_star_tracker);
    LOAD_COMPONENT("GPSs", struct GpsType, S->GPS, S->Ngps, read_gps);
    LOAD_COMPONENT("Accelerometers", struct AccelType, S->Accel, S->Nacc, read_accelerometer);

#undef LOAD_COMPONENT

    fy_document_destroy(fyd);
    fclose(infile);
}


/* #ifdef __cplusplus
** }
** #endif
*/
