#include "42.h"
#include "42config/YAML/42sim.h"
#include <libfyaml.h>

void InitSimYAML(const char *path, const char *filename, long *MinorBodiesExist)
{
    FILE *infile;
    char response[120],response1[120],response2[120];
    long Iorb,Isc,i,j,Ip,Im,Iw,Nm;

    /* .. Read from file Inp_Sim.txt */
    infile=FileOpen(path, filename, "r");
    struct fy_document *fyd = fy_document_build_from_fp(NULL, infile);
    if (fyd == NULL)
    {
        printf("Can not open Sim description file: %s\n", filename);
        exit(1);
    }
    struct fy_node *root = fy_document_root(fyd);

    struct fy_node *sim = fy_node_by_path(root, "/Simulation", -1, 0);
    fy_node_scanf(sim, "/Time_Mode %s", response);
    TimeMode = DecodeString(response);

/* .. Duration, Step size */
    fy_node_scanf(sim, "/Duration %lf", &STOPTIME);
    fy_node_scanf(sim, "/Step %lf", &DTSIM);

/* .. File output interval */
    fy_node_scanf(sim, "/File_Output_Interval %lf", &DTOUT);

/* .. Graphics Front End? */
    fy_node_scanf(sim, "/GUI %s", response);
    GLEnable = DecodeString(response);

/* .. Cmd Script File Name */
    fy_node_scanf(sim, "/Script %s",CmdFileName);

/* .. Reference Orbits */
    struct fy_node *orbits = fy_node_by_path(root, "/Orbits", -1, 0);
    Norb = fy_node_sequence_item_count(orbits);
    Orb = (struct OrbitType *) calloc(Norb,sizeof(struct OrbitType));
    if (Orb == NULL)
    {
        printf("Orb calloc returned null pointer.  Bailing out!\n");
        exit(1);
    }
    Frm = (struct FormationType *) calloc(Norb,sizeof(struct FormationType));
    if (Frm == NULL)
    {
        printf("Frm calloc returned null pointer.  Bailing out!\n");
        exit(1);
    }

    for(Iorb = 0; Iorb < Norb; Iorb++)
    {
        struct fy_node *orbit = fy_node_sequence_get_by_index(orbits, Iorb);
        fy_node_scanf(orbit, "/Enabled %s", response);
        Orb[Iorb].Exists = DecodeString(response);
        fy_node_scanf(orbit,"/Filename %s", Orb[Iorb].FileName);
        Orb[Iorb].Tag = Iorb;
    }

/* .. Spacecraft */
    struct fy_node *scs = fy_node_by_path(root, "/Spacecrafts", -1, 0);
    Nsc = fy_node_sequence_item_count(scs);
    SC = (struct SCType *) calloc(Nsc,sizeof(struct SCType));
    if (SC == NULL)
    {
        printf("SC calloc returned null pointer.  Bailing out!\n");
        exit(1);
    }

    for (Isc = 0; Isc < Nsc; Isc++)
    {
        struct fy_node *sc = fy_node_sequence_get_by_index(scs, Isc);
        fy_node_scanf(sc, "/Enabled %s", response);
        SC[Isc].Exists = DecodeString(response);

        fy_node_scanf(sc, "/Orbit %ld", &SC[Isc].RefOrb);
        fy_node_scanf(sc, "/Filename %s", SC[Isc].FileName);
        SC[Isc].ID = Isc;
        if ((SC[Isc].Exists && !Orb[SC[Isc].RefOrb].Exists) || (SC[Isc].RefOrb > Norb))
        {
            printf("Yo!  SC[%ld] is assigned to non-existent Orb[%ld]\n",
               Isc,SC[Isc].RefOrb);
            exit(1);
        }
    }
/* .. Environment */
/* .. Date and time (UTC) */
    struct fy_node *env = fy_node_by_path(root, "/Environment", -1, 0);

    struct fy_node *dt = fy_node_by_path(env, "/Datetime", -1, 0);
    fy_node_scanf(dt, "/Year %ld /Month %ld /Day %ld", &UTC.Year, &UTC.Month, &UTC.Day);
    fy_node_scanf(dt, "/Hr %ld /Min %ld /Sec %lf", &UTC.Hour, &UTC.Minute, &UTC.Second);
    fy_node_scanf(dt, "/Leap %lf",&LeapSec);

/* .. Choices for Modeling Solar Activity */
    struct fy_node *ap = fy_node_by_path(env, "/Atmosphere", -1, 0);

    fy_node_scanf(ap, "/F10.7 %s", response);
    AtmoOption=DecodeString(response);

    if (AtmoOption == USER_ATMO)
    {
        fy_node_scanf(ap, "/User_F10.7/Flux %lf",             &Flux10p7);
        fy_node_scanf(ap, "/User_F10.7/GeomagneticIndex %lf", &GeomagIndex);
    }

/* .. Magnetic Field Model */
    struct fy_node *mag = fy_node_by_path(env, "/Magnetic_Field", -1, 0);

    fy_node_scanf(mag, "/Model %s", response);
    MagModel.Type = DecodeString(response);

    if (MagModel.Type == IGRF)
    {
        fy_node_scanf(mag, "/IGRF/N %ld /IGRF/M %ld", &MagModel.N, &MagModel.M);
    }

    struct fy_node *grav = fy_node_by_path(env, "/Gravity_Model", -1, 0);
/* .. Earth Gravity Model */
    fy_node_scanf(grav, "/EARTH/N %ld /MARS/M %ld", &EarthGravModel.N, &EarthGravModel.M);

/* .. Mars Gravity Model */
    fy_node_scanf(grav, "/MARS/N %ld /MARS/M %ld", &MarsGravModel.N, &MarsGravModel.M);

/* .. Luna Gravity Model */
    fy_node_scanf(grav, "/LUNA/N %ld /LUNA/M %ld", &LunaGravModel.N, &LunaGravModel.M);

/* .. Toggle on/off various environmental effects */
    fy_node_scanf(env, "/Aerodynamics/Forces %s", response);
    AeroActive=DecodeString(response);
    fy_node_scanf(env, "/Aerodynamics/Shadows %s", response);
    AeroShadowsActive=DecodeString(response);

    fy_node_scanf(env, "/Gravity_Gradient/Forces %s", response);
    GGActive=DecodeString(response);

    fy_node_scanf(env, "/Solar_Pressure/Forces %s", response);
    SolPressActive=DecodeString(response);
    fy_node_scanf(env, "/Solar_Pressure/Shadows %s", response);
    SolPressShadowsActive=DecodeString(response);

    fy_node_scanf(env, "/Gravity_Perturbations %s", response);
    GravPertActive=DecodeString(response);

    fy_node_scanf(env, "/Passive_Joint_Forces %s", response);
    JointTrqActive=DecodeString(response);

    fy_node_scanf(env, "/Thruster_Plume_Forces %s", response);
    ThrusterPlumesActive=DecodeString(response);

    fy_node_scanf(env, "/RWA_Imbalance_Forces %s", response);
    RwaImbalanceActive=DecodeString(response);

    fy_node_scanf(env, "/Contact_Forces %s", response);
    ContactActive=DecodeString(response);

    fy_node_scanf(env, "/CFD_Slosh_Forces %s", response);
    SloshActive=DecodeString(response);

    fy_node_scanf(env, "/Log_Enviromental_Forces %s", response);
    ComputeEnvTrq=DecodeString(response);

/* .. Celestial Bodies */
    struct fy_node *cel = fy_node_by_path(root, "/Celestial_Bodies", -1, 0);

    fy_node_scanf(cel, "/Ephemeride_Option %s", response);
    EphemOption = DecodeString(response);

    fy_node_scanf(cel, "/Minor_Bodies %s", response);
    *MinorBodiesExist=DecodeString(response);

    struct fy_node *planets = fy_node_by_path(cel, "/Planets", -1, 0);
    int Nbplanets = fy_node_mapping_item_count(planets);

    for (i = 0; i < Nbplanets; i++)
    {
        struct fy_node_pair *pl = fy_node_mapping_get_by_index(planets, i);
        struct fy_node *name = fy_node_pair_key(pl);
        struct fy_node *en = fy_node_pair_value(pl);

        size_t plen;
        const char *pname = fy_node_get_scalar(name, &plen);
        strncpy(response, pname, plen);
        response[plen] = 0;
        long planet_id = DecodeString(response);

        const char *pex = fy_node_get_scalar(en, &plen);
        strncpy(response, pex, plen);
        response[plen] = 0;
        World[planet_id].Exists=DecodeString(response);
    }
      

/* .. Lagrange Point Systems */
    struct fy_node *lss = fy_node_by_path(root, "/Lagrange_Point_Systems", -1, 0);
    int Nblss = fy_node_mapping_item_count(lss);

    for (i = 0; i < Nblss; i++)
    {
        struct fy_node_pair *pair = fy_node_mapping_get_by_index(lss, i);
        struct fy_node *name = fy_node_pair_key(pair);
        struct fy_node *en = fy_node_pair_value(pair);

        size_t plen;
        const char *ls_name = fy_node_get_scalar(name, &plen);
        strncpy(response, ls_name, plen);
        response[plen] = 0;
        int ls_id;
        if (!strcmp(response, "SUN-EARTH"))
        {
            ls_id = SUNEARTH;
        }
        else if (!strcmp(response, "EARTH-LUNA"))
        {
            ls_id = EARTHMOON;
        }
        else if (!strcmp(response, "SUN-JUPITER"))
        {
            ls_id = SUNJUPITER;
        }
        else
        {
            printf("Unknown largange point system %s\n", ls_name);
            exit(1);
        }
        const char *pex = fy_node_get_scalar(en, &plen);
        strncpy(response, pex, plen);
        response[plen] = 0;
        LagSys[ls_id].Exists = DecodeString(response);
    }

/* .. Ground Stations */
    struct fy_node *gss = fy_node_by_path(env, "/Ground_Stations", -1, 0);
    Ngnd = fy_node_sequence_item_count(gss);
    GroundStation = (struct GroundStationType *) calloc(Ngnd,sizeof(struct GroundStationType));
    for (i = 0; i < Ngnd; i++)
    {
        struct fy_node *gs = fy_node_sequence_get_by_index(gss, i);

        fy_node_scanf(gs, "/Enabled %s", response);
        GroundStation[i].Exists = DecodeString(response);

        fy_node_scanf(gs, "/Body %s", response);
        GroundStation[i].World = DecodeString(response);

        fy_node_scanf(gs, "/Label %s", GroundStation[i].Label);

        fy_node_scanf(gs, "/Lon %lf", &GroundStation[i].lng);
        fy_node_scanf(gs, "/Lat %lf", &GroundStation[i].lat);
    }

    fy_document_destroy(fyd);
    fclose(infile);
}
