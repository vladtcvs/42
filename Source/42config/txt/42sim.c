#include "42.h"
#include <stdio.h>

void InitSimTxt(const char *path, const char *filename, long *MinorBodiesExist)
{
      FILE *infile;
      char junk[120],newline;
      char response[120],response1[120],response2[120];
      long Iorb,Isc,i,j,Ip,Im,Iw,Nm;

    /* .. Read from file Inp_Sim.txt */
      infile=FileOpen(path, filename, "r");

      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
/* .. Time Mode */
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      TimeMode = DecodeString(response);
/* .. Duration, Step size */
      fscanf(infile,"%lf %lf %[^\n] %[\n]",&STOPTIME,&DTSIM,junk,&newline);
/* .. File output interval */
      fscanf(infile,"%lf %[^\n] %[\n]",&DTOUT,junk,&newline);
/* .. Graphics Front End? */
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      GLEnable = DecodeString(response);
/* .. Cmd Script File Name */
      fscanf(infile,"%s %[^\n] %[\n]",CmdFileName,junk,&newline);

/* .. Reference Orbits */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%ld %[^\n] %[\n]",&Norb,junk,&newline);
      Orb = (struct OrbitType *) calloc(Norb,sizeof(struct OrbitType));
      if (Orb == NULL) {
         printf("Orb calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }
      Frm = (struct FormationType *) calloc(Norb,sizeof(struct FormationType));
      if (Frm == NULL) {
         printf("Frm calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }
      for(Iorb=0;Iorb<Norb;Iorb++){
         fscanf(infile,"%s %s %[^\n] %[\n]",response,Orb[Iorb].FileName,
                junk,&newline);
         Orb[Iorb].Exists = DecodeString(response);
         Orb[Iorb].Tag = Iorb;
      }

/* .. Spacecraft */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%ld %[^\n] %[\n]",&Nsc,junk,&newline);
      SC = (struct SCType *) calloc(Nsc,sizeof(struct SCType));
      if (SC == NULL) {
         printf("SC calloc returned null pointer.  Bailing out!\n");
         exit(1);
      }
      for(Isc=0;Isc<Nsc;Isc++){
         fscanf(infile,"%s  %ld %s %[^\n] %[\n]",response,
            &SC[Isc].RefOrb,SC[Isc].FileName,junk,&newline);
         SC[Isc].Exists=DecodeString(response);
         SC[Isc].ID = Isc;
         if ((SC[Isc].Exists && !Orb[SC[Isc].RefOrb].Exists) || (SC[Isc].RefOrb > Norb)) {
            printf("Yo!  SC[%ld] is assigned to non-existent Orb[%ld]\n",
               Isc,SC[Isc].RefOrb);
            exit(1);
         }
      }
/* .. Environment */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
/* .. Date and time (UTC) */
      fscanf(infile,"%ld %ld %ld %[^\n] %[\n]",&UTC.Month,&UTC.Day,&UTC.Year,
             junk,&newline);
      fscanf(infile,"%ld %ld %lf %[^\n] %[\n]",&UTC.Hour,&UTC.Minute,&UTC.Second,
             junk,&newline);
      fscanf(infile,"%lf %[^\n] %[\n]",&LeapSec,junk,&newline);
/* .. Choices for Modeling Solar Activity */
      fscanf(infile,"%s  %[^\n] %[\n]",response,junk,&newline);
      AtmoOption=DecodeString(response);
      fscanf(infile,"%lf %[^\n] %[\n]",&Flux10p7,junk,&newline);
      fscanf(infile,"%lf %[^\n] %[\n]",&GeomagIndex,junk,&newline);
/* .. Magnetic Field Model */
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         MagModel.Type = DecodeString(response);
      fscanf(infile,"%ld %ld %[^\n] %[\n]",&MagModel.N,&MagModel.M,
             junk,&newline);
/* .. Earth Gravity Model */
      fscanf(infile,"%ld %ld %[^\n] %[\n]",&EarthGravModel.N,
             &EarthGravModel.M,junk,&newline);
/* .. Mars Gravity Model */
      fscanf(infile,"%ld %ld %[^\n] %[\n]",&MarsGravModel.N,
             &MarsGravModel.M,junk,&newline);
/* .. Luna Gravity Model */
      fscanf(infile,"%ld %ld %[^\n] %[\n]",&LunaGravModel.N,
             &LunaGravModel.M,junk,&newline);
/* .. Toggle on/off various environmental effects */
      fscanf(infile,"%s  %s %[^\n] %[\n]",response1,response2,junk,&newline);
      AeroActive=DecodeString(response1);
      AeroShadowsActive = DecodeString(response2);
      fscanf(infile,"%s  %[^\n] %[\n]",response,junk,&newline);
      GGActive=DecodeString(response);
      fscanf(infile,"%s %s %[^\n] %[\n]",response1,response2,junk,&newline);
      SolPressActive=DecodeString(response1);
      SolPressShadowsActive=DecodeString(response2);
      fscanf(infile,"%s  %[^\n] %[\n]",response,junk,&newline);
      GravPertActive=DecodeString(response);
      fscanf(infile,"%s  %[^\n] %[\n]",response,junk,&newline);
      JointTrqActive=DecodeString(response);
      fscanf(infile,"%s  %[^\n] %[\n]",response,junk,&newline);
      ThrusterPlumesActive=DecodeString(response);
      fscanf(infile,"%s  %[^\n] %[\n]",response,junk,&newline);
      RwaImbalanceActive=DecodeString(response);
      fscanf(infile,"%s  %[^\n] %[\n]",response,junk,&newline);
      ContactActive=DecodeString(response);
      fscanf(infile,"%s  %[^\n] %[\n]",response,junk,&newline);
      SloshActive=DecodeString(response);
      fscanf(infile,"%s  %[^\n] %[\n]",response,junk,&newline);
      ComputeEnvTrq=DecodeString(response);
/* .. Celestial Bodies */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      EphemOption = DecodeString(response);
      for(i=MERCURY;i<=PLUTO;i++){
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         World[i].Exists=DecodeString(response);
      }
      fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
      *MinorBodiesExist=DecodeString(response);

/* .. Lagrange Point Systems */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      for(i=0;i<3;i++){
         fscanf(infile,"%s %[^\n] %[\n]",response,junk,&newline);
         LagSys[i].Exists=DecodeString(response);
      }

/* .. Ground Stations */
      fscanf(infile,"%[^\n] %[\n]",junk,&newline);
      fscanf(infile,"%ld %[^\n] %[\n]",&Ngnd,junk,&newline);
      GroundStation = (struct GroundStationType *) calloc(Ngnd,sizeof(struct GroundStationType));
      for(i=0;i<Ngnd;i++) {
         fscanf(infile,"%s %s %lf %lf \"%[^\"]\" %[^\n] %[\n]",
            response1,response2,&GroundStation[i].lng,&GroundStation[i].lat,
            GroundStation[i].Label,junk,&newline);
         GroundStation[i].Exists = DecodeString(response1);
         GroundStation[i].World = DecodeString(response2);
      }

      fclose(infile);
}