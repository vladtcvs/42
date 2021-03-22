#include <stdio.h>
#include <libfyaml.h>
#include "42.h"
#include "42config/YAML/yaml_functions.h"
#include "42config/txt/42orbit.h"
#include "42config/42config.h"

void ConfigOrbitZero(struct OrbitType *O)
{
    int j, k;
    if (!World[O->World].Exists)
    {
        printf("Oops.  Orbit %ld depends on a World that doesn't exist.\n",O->Tag);
        exit(1);
    }
    O->mu = World[O->World].mu;
    for (j = 0; j < 3; j++)
    {
        O->PosN[j] = 0.0;
        O->VelN[j] = 0.0;
        for (k = 0; k < 3; k++)
            O->CLN[j][k] = 0.0;

        O->CLN[j][j] = 1.0;
        O->wln[j] = 0.0;
    }
}

void ConfigOrbitFlight(struct OrbitType *O)
{
    int j, k;
    struct RegionType *R;
    R = &Rgn[O->Region];
    O->World = R->World;
    O->mu = World[O->World].mu;
    for (j = 0; j < 3; j++)
    {
        O->PosN[j] = R->PosN[j];
        O->VelN[j] = R->VelN[j];
        for (k = 0; k < 3; k++)
            O->CLN[j][k] = R->CN[j][k];

        O->wln[0] = 0.0;
        O->wln[1] = 0.0;
        O->wln[2] = World[O->World].w;
    }
}

void InitOrbitYAML(struct OrbitType *O)
{
    FILE *infile;
    char junk[120],newline,response[120];
    double Alt1,Alt2,MaxAnom;
    
    double p[3],Ang1,Ang2,Ang3;
    char FrmExpressedIn;
    long Ir,i,j,k,Seq;
    struct FormationType *F;
    struct RegionType *R;
    long InputType,UsePA,ElementType;
    char ElementLabel[40];
    char ElementFileName[40];
    long Success;
    long NodeYear,NodeMonth,NodeDay,NodeHour,NodeMin;
    double NodeSec;

    infile = FileOpen(InOutPath,O->FileName,"r");

    struct fy_document *fyd = fy_document_build_from_fp(NULL, infile);
    if (fyd == NULL)
    {
        printf("Can not open orbit description file: %s\n", O->FileName);
        exit(1);
    }
    struct fy_node *root = fy_document_root(fyd);

    struct fy_node *orbit = fy_node_by_path(root, "/Orbit", -1, 0);

/* .. Orbit Parameters */
    O->Epoch = DynTime;
    fy_node_scanf(orbit, "/Type %s", response);
    O->Regime = DecodeString(response);

    if (O->Regime == ORB_ZERO)
    {
        struct fy_node *zero = fy_node_by_path(orbit, "/Zero_Orbit_Parameters", -1, 0);
        fy_node_scanf(zero, "/World %s", response);
        O->World = DecodeString(response);
        fy_node_scanf(zero, "/UsePolyhedronGravity %s", response);
        O->PolyhedronGravityEnabled = DecodeString(response);

        ConfigOrbitZero(O);
    }
    else if (O->Regime == ORB_FLIGHT)
    {
        struct fy_node *flight = fy_node_by_path(orbit, "/Flight_Orbit_Parameters", -1, 0);
        fy_node_scanf(flight, "/RegionNumber %ld", &Ir);
        if (!Rgn[Ir].Exists)
        {
            printf("Oops.  Orbit %ld depends on a Region that doesn't exist.\n",O->Tag);
            exit(1);
        }
        O->Region = Ir;
        fy_node_scanf(flight, "/UsePolyhedronGravity %s", response);
        O->PolyhedronGravityEnabled = DecodeString(response);
        
        ConfigOrbitFlight(O);
    }
    else if (O->Regime == ORB_CENTRAL)
    {
        struct fy_node *central = fy_node_by_path(orbit, "/Central_Orbit_Parameters", -1, 0);
        double mu, rad;

        fy_node_scanf(central, "/Central_Body %s", response);
        O->World = DecodeString(response);
        if (!World[O->World].Exists)
        {
            printf("Oops.  Orbit %ld depends on a World that doesn't exist.\n",O->Tag);
            exit(1);
        }

        fy_node_scanf(central, "/J2_Secular_Orbit_Drift %s", response);
        O->J2DriftEnabled = DecodeString(response);

        mu = World[O->World].mu;
        rad = World[O->World].rad;
        O->mu=mu;

        fy_node_scanf(central, "/Orbit_Parameters_Type %s", response);
        InputType = DecodeString(response);

        if (InputType == INP_KEPLER)
        {
            struct fy_node *kep = fy_node_by_path(central, "/Keplerian_Orbit_Parameters", -1, 0);
            fy_node_scanf(kep, "/Type %s", response);
            //UsePA = DecodeString(response);

            if (!strcmp(response, "PA"))
            {
                fy_node_scanf(kep, "/PA/Periapsis %lf", &Alt1);
                fy_node_scanf(kep, "/PA/Apoapsis %lf", &Alt2);

                if (Alt2 < Alt1)
                {
                    printf("Apoapsis below Periapsis for Orbit %ld\n",O->Tag);
                    exit(1);
                }
               
                O->SMA=rad+0.5*(Alt1+Alt2)*1.0E3;
                O->ecc=1.0E3*fabs(Alt1-Alt2)/(2.0*O->SMA);
                O->SLR = O->SMA*(1.0-O->ecc*O->ecc);
                O->alpha = 1.0/O->SMA;
                O->rmin = rad+Alt1*1.0E3;
            }
            else
            {
                fy_node_scanf(kep, "/AE/Periapsis %lf", &Alt1);
                fy_node_scanf(kep, "/AE/Eccentricity %lf", &O->ecc);
                O->rmin = rad+Alt1*1.0E3;
                O->SLR = O->rmin*(1.0+O->ecc);
                O->alpha = (1.0-O->ecc)/O->rmin;
                if (O->alpha != 0.0)
                    O->SMA = 1.0/O->alpha;
            }
            fy_node_scanf(kep, "/Inclination %lf",              &O->inc);
            fy_node_scanf(kep, "/Right_Ascension %lf",          &O->RAAN);
            fy_node_scanf(kep, "/Argument_of_Periapsis %lf",    &O->ArgP);
            fy_node_scanf(kep, "/True_Anomaly %lf",             &O->anom);
            O->inc *= D2R;
            O->RAAN *= D2R;
            O->ArgP *= D2R;
            O->anom *= D2R;

            /* Some anomalies are unreachable for hyperbolic trajectories */
            if (O->ecc > 1.0)
            {
                MaxAnom = Pi - acos(1.0/O->ecc);
                if (fabs(O->anom) > MaxAnom)
                {
                    printf("True Anomaly out of range for Orbit %ld\n",O->Tag);
                    exit(1);
                }
            }

            if (O->J2DriftEnabled)
            {
                FindJ2DriftParms(O->mu,World[O->World].J2,World[O->World].rad,O);
                O->RAAN0 = O->RAAN;
                O->ArgP0 = O->ArgP;
                O->tp = O->Epoch - TimeSincePeriapsis(O->MuPlusJ2,O->SLR,O->ecc,O->anom);
                Eph2RV(O->MuPlusJ2,O->SLR,O->ecc,O->inc,
                       O->RAAN,O->ArgP,O->Epoch-O->tp,
                       O->PosN,O->VelN,&O->anom);
            }
            else
            {
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
        else if (InputType == INP_POSVEL)
        {
            struct fy_node *posvel = fy_node_by_path(root, "/Position_Velocity_Orbit_Parameters", -1, 0);
            
            yaml_read_vector(posvel, "Position", O->PosN);
            yaml_read_vector(posvel, "Velocity", O->VelN);
            
            for(j=0;j<3;j++)
            {
               O->PosN[j] *= 1.0E3;
               O->VelN[j] *= 1.0E3;
            }            
            if (O->J2DriftEnabled)
            {
                FindJ2DriftParms(O->mu,World[O->World].J2,World[O->World].rad,O);
                RV2Eph(O->Epoch,O->MuPlusJ2,O->PosN,O->VelN,
                       &O->SMA,&O->ecc,&O->inc,&O->RAAN,
                       &O->ArgP,&O->anom,&O->tp,
                       &O->SLR,&O->alpha,&O->rmin,
                       &O->MeanMotion,&O->Period);
                O->RAAN0 = O->RAAN;
                O->ArgP0 = O->ArgP;
            }
            else
            {
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
        }
        else if (InputType == INP_FILE)
        {
            struct fy_node *file = fy_node_by_path(central, "/File_Orbit_Parameters", -1, 0);
            fy_node_scanf(file, "/Label %s", ElementLabel);
            fy_node_scanf(file, "/Filename %s", ElementFileName);
            fy_node_scanf(file, "/Format %s", response);
            ElementType = DecodeString(response);
            
            if (ElementType == INP_TLE)
            {
                Success = LoadTleFromFile(InOutPath,ElementFileName,
                                          ElementLabel,TT.JulDay,O->mu,O);
                if (!Success)
                {
                    printf("Error loading TLE %s from file %s.\n",
                           ElementLabel,ElementFileName);
                    exit(1);
                }
            }
            else if (ElementType == INP_TRV)
            {
                Success = LoadTRVfromFile(InOutPath, ElementFileName,
                                          ElementLabel, CivilTime, O);
                if (!Success)
                {
                    printf("Error loading TRV %s from file %s.\n",
                            ElementLabel,ElementFileName);
                    exit(1);
                }
            }
            else
            {
                printf("Oops.  Unknown ElementType in InitOrbit.\n");
                exit(1);
            }
            
            if (O->J2DriftEnabled)
            {
                FindJ2DriftParms(O->mu,World[O->World].J2,World[O->World].rad,O);
                O->RAAN0 = O->RAAN + O->RAANdot*(DynTime-O->Epoch);
                O->ArgP0 = O->ArgP + O->ArgPdot*(DynTime-O->Epoch);
                O->tp = O->Epoch - TimeSincePeriapsis(O->MuPlusJ2,O->SLR,O->ecc,O->anom);
                Eph2RV(O->MuPlusJ2,O->SLR,O->ecc,O->inc,
                        O->RAAN,O->ArgP,O->Epoch-O->tp,
                        O->PosN,O->VelN,&O->anom);
            }
            else
            {
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
        else
        {
            printf("Oops.  Unknown InputType in InitOrbit.\n");
            exit(1);
        }
        FindCLN(O->PosN,O->VelN,O->CLN,O->wln);
    }
    else if (O->Regime == ORB_THREE_BODY)
    {
        struct fy_node *thbs = fy_node_by_path(orbit, "/Three_Body_Orbit_Parameters", -1, 0);
        fy_node_scanf(thbs, "/Lagrange_System %s", response);
        O->Sys = DecodeString(response);
        if (!LagSys[O->Sys].Exists)
        {
            printf("Oops.  Orbit %ld depends on a Lagrange System that doesn't exist.\n",O->Tag);
            exit(1);
        }

        O->Body1 = LagSys[O->Sys].Body1;
        O->Body2 = LagSys[O->Sys].Body2;
        O->mu1 =  LagSys[O->Sys].mu1;
        O->mu2 =  LagSys[O->Sys].mu2;

        fy_node_scanf(thbs, "/Propagation %s", response);
        O->LagDOF = DecodeString(response);
        fy_node_scanf(thbs, "/Init %s", response);
        InputType = DecodeString(response);
        if (InputType == INP_MODES)
        {
            double AmpXY1 = 0, PhiXY1 = 0, SenseXY1 = 0;
            double AmpXY2 = 0, PhiXY2 = 0, SenseXY2 = 0;
            double AmpZ, PhiZ;
    
            struct fy_node *modes = fy_node_by_path(thbs, "/Initial_Modes", -1, 0);

            /* Initial modes given */
            fy_node_scanf(modes, "/Libration_Point %s", response);
            O->LP = DecodeString(response);

            fy_node_scanf(modes, "/XY_Semi_major_axis %lf", &AmpXY1);
            fy_node_scanf(modes, "/XY_Phase %lf", &PhiXY1);
            fy_node_scanf(modes, "/Sense %s", response);
            if (DecodeString(response) == DIR_CCW)
                SenseXY1 = 1.0;
            else
                SenseXY1 = -1.0;

            if (O->LP == LAGPT_L4 || O->LP == LAGPT_L5)
            {
                fy_node_scanf(modes, "/Second_XY_Semi_major_axis %lf", &AmpXY2);
                fy_node_scanf(modes, "/Second_XY_Phase %lf", &PhiXY2);
                fy_node_scanf(modes, "/Sense %s", response);
                if (DecodeString(response) == DIR_CCW)
                    SenseXY2 = 1.0;
                else
                    SenseXY2 = -1.0;
            }

            fy_node_scanf(modes, "/Z_Semi_axis %lf", &AmpZ);
            fy_node_scanf(modes, "/Z_Phase %lf", &PhiZ);
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
        else if (InputType == INP_XYZ)
        {
            struct fy_node *xyz = fy_node_by_path(thbs, "/Initial_XYZ", -1, 0);
            yaml_read_3_vals(xyz, "Position", &O->x, &O->y, &O->z);
            yaml_read_3_vals(xyz, "Position", &O->xdot, &O->ydot, &O->zdot);

            XYZ2LagModes(0.0,&LagSys[O->Sys],O);
            LagModes2RV(DynTime,&LagSys[O->Sys],O,O->PosN,O->VelN);
         }
        else if (InputType == INP_FILE)
        {
            struct fy_node *file = fy_node_by_path(thbs, "/Initial_File", -1, 0);
            
            fy_node_scanf(file, "/Libration_Point %s", response);
            O->LP = DecodeString(response);

            fy_node_scanf(file, "/Label %s", ElementLabel);
            fy_node_scanf(file, "/Filename %s", ElementFileName);
            fy_node_scanf(file, "/Format %s", response);
            ElementType = DecodeString(response);

            if (ElementType == INP_TRV)
            {
                Success = LoadTRVfromFile(InOutPath, ElementFileName,
                                          ElementLabel, CivilTime, O);
                if (!Success)
                {
                    printf("Error loading TRV %s from file %s.\n",
                            ElementLabel, ElementFileName);
                    exit(1);
                }
            }
            else if (ElementType == INP_SPLINE)
            {
                O->SplineFile = FileOpen(InOutPath,ElementFileName,"rt");
                for(i=0;i<4;i++)
                {
                  fscanf(O->SplineFile," %ld:%ld:%ld:%ld:%ld:%lf %lf %lf %lf %lf %lf %lf %[\n]",
                        &NodeYear,&NodeMonth,&NodeDay,&NodeHour,&NodeMin,&NodeSec,
                        &O->NodePos[i][0],&O->NodePos[i][1],&O->NodePos[i][2],
                        &O->NodeVel[i][0],&O->NodeVel[i][1],&O->NodeVel[i][2],
                        &newline);
                    O->NodeDynTime[i] = DateToTime(NodeYear,NodeMonth,NodeDay,
                                                   NodeHour,NodeMin,NodeSec);
                    for (j = 0; j < 3; j++)
                    {
                        O->NodePos[i][j] *= 1000.0;
                        O->NodeVel[i][j] *= 1000.0;
                    }
                    if (DynTime < O->NodeDynTime[1])
                    {
                        printf("Oops.  Spline file beginning is in the future.\n");
                        exit(1);
                    }
                }
                SplineToPosVel(O);
            }
            else
            {
                printf("Oops.  Unknown ElementType in InitOrbit.\n");
                exit(1);
            }
        }
        else
        {
            printf("Oops.  Unknown Input Type in InitOrbit.\n");
            exit(1);
        }
        O->World = O->Body1;
        O->mu = O->mu1;
        O->SMA = MAGV(O->PosN); /* For sake of EH */
        FindCLN(O->PosN, O->VelN, O->CLN, O->wln);
        O->MeanMotion = LagSys[O->Sys].MeanRate;
        O->Period = TwoPi/O->MeanMotion;
    }
    else
    {
        printf("Bogus Orbit Regime in file %s\n",O->FileName);
        exit(1);
    }

    struct fy_node *frm = fy_node_by_path(root, "/Formation", -1, 0);
/* .. Formation Frame Parameters */
    F = &Frm[O->Tag];
    fy_node_scanf(frm, "/Orientation/Frame %s", response);
    if (!strcmp(response, "Inertial"))
        F->FixedInFrame = 'N';
    else
        F->FixedInFrame = 'L';

    yaml_read_angles(frm, "Orientation/Angles", &Ang1,&Ang2,&Ang3);
    fy_node_scanf(frm, "Orientation/Sequence %ld", &Seq);

    A2C(Seq, Ang1*D2R, Ang2*D2R, Ang3*D2R, F->CN);
    if(F->FixedInFrame == 'L')
    {
        /* Adjust CFN */
        for(j=0;j<3;j++)
        {
            for(k=0;k<3;k++) F->CL[j][k] = F->CN[j][k];
        }
        MxM(F->CL,O->CLN,F->CN);
    }

    fy_node_scanf(frm, "/Position/Frame %s", response);
    if (!strcmp(response, "Inertial"))
        FrmExpressedIn = 'N';
    else
        FrmExpressedIn = 'L';

    yaml_read_vector(frm, "Position/Position", F->PosR);
    if (FrmExpressedIn == 'L')
    {
        for (j = 0; j < 3; j++)
            p[j] = F->PosR[j];
        MTxV(O->CLN,p,F->PosR);
    }

    fy_document_destroy(fyd);
    fclose(infile);
}
