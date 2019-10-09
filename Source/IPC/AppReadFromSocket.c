#include "Ac.h"

/**********************************************************************/
void ReadFromSocket(SOCKET Socket, struct AcType *AC)
{

      struct SCType *S;
      struct OrbitType *O;
      struct DynType *D;
      long Isc,Iorb,Iw,i;
      char line[512] = "Blank";
      long RequestTimeRefresh = 0;
      long Done;
      char Msg[16384];
      char AckMsg[5] = "Ack\n";
      long Imsg,Iline;
      double DbleVal[30];
      long LongVal[30];

      long Year,doy,Month,Day,Hour,Minute;
      double Second;
      long MsgLen;

      memset(Msg,'\0',16384);
      MsgLen = 0;
      recv(Socket,Msg,16384,0);

      Done = 0;
      Imsg = 0;
      while(!Done) {
         /* Parse lines from Msg, newline-delimited */
         Iline = 0;
         memset(line,'\0',512);
         while(Msg[Imsg] != '\n') {
            line[Iline++] = Msg[Imsg++];
         }
         line[Iline++] = Msg[Imsg++];
         if (AC->EchoEnabled) printf("%s",line);

         if (sscanf(line,"TIME %ld-%ld-%ld:%ld:%lf\n",
            &Year,&doy,&Hour,&Minute,&Second) == 5)
            RequestTimeRefresh = 1;

         if (sscanf(line,"SC[%ld].AC.ParmLoadEnabled = %ld",
            &Isc,
            &LongVal[0]) == 2) {
            if (Isc == AC->ID) {
               AC->ParmLoadEnabled = LongVal[0];
            }
         }

         if (sscanf(line,"SC[%ld].AC.ParmDumpEnabled = %ld",
            &Isc,
            &LongVal[0]) == 2) {
            if (Isc == AC->ID) {
               AC->ParmDumpEnabled = LongVal[0];
            }
         }

         if (sscanf(line,"SC[%ld].AC.G[%ld].Ang = %le %le %le",
            &Isc,&i,
            &DbleVal[0],
            &DbleVal[1],
            &DbleVal[2]) == 5) {
            if (Isc == AC->ID) {
               AC->G[i].Ang[0] = DbleVal[0];
               AC->G[i].Ang[1] = DbleVal[1];
               AC->G[i].Ang[2] = DbleVal[2];
            }
         }

         if (sscanf(line,"SC[%ld].AC.Gyro[%ld].Rate = %le",
            &Isc,&i,
            &DbleVal[0]) == 3) {
            if (Isc == AC->ID) {
               AC->Gyro[i].Rate = DbleVal[0];
            }
         }

         if (sscanf(line,"SC[%ld].AC.MAG[%ld].Field = %le",
            &Isc,&i,
            &DbleVal[0]) == 3) {
            if (Isc == AC->ID) {
               AC->MAG[i].Field = DbleVal[0];
            }
         }

         if (sscanf(line,"SC[%ld].AC.CSS[%ld].Valid = %ld",
            &Isc,&i,
            &LongVal[0]) == 3) {
            if (Isc == AC->ID) {
               AC->CSS[i].Valid = LongVal[0];
            }
         }

         if (sscanf(line,"SC[%ld].AC.CSS[%ld].Illum = %le",
            &Isc,&i,
            &DbleVal[0]) == 3) {
            if (Isc == AC->ID) {
               AC->CSS[i].Illum = DbleVal[0];
            }
         }

         if (sscanf(line,"SC[%ld].AC.FSS[%ld].Valid = %ld",
            &Isc,&i,
            &LongVal[0]) == 3) {
            if (Isc == AC->ID) {
               AC->FSS[i].Valid = LongVal[0];
            }
         }

         if (sscanf(line,"SC[%ld].AC.FSS[%ld].SunAng = %le %le",
            &Isc,&i,
            &DbleVal[0],
            &DbleVal[1]) == 4) {
            if (Isc == AC->ID) {
               AC->FSS[i].SunAng[0] = DbleVal[0];
               AC->FSS[i].SunAng[1] = DbleVal[1];
            }
         }

         if (sscanf(line,"SC[%ld].AC.ST[%ld].Valid = %ld",
            &Isc,&i,
            &LongVal[0]) == 3) {
            if (Isc == AC->ID) {
               AC->ST[i].Valid = LongVal[0];
            }
         }

         if (sscanf(line,"SC[%ld].AC.ST[%ld].qn = %le %le %le %le",
            &Isc,&i,
            &DbleVal[0],
            &DbleVal[1],
            &DbleVal[2],
            &DbleVal[3]) == 6) {
            if (Isc == AC->ID) {
               AC->ST[i].qn[0] = DbleVal[0];
               AC->ST[i].qn[1] = DbleVal[1];
               AC->ST[i].qn[2] = DbleVal[2];
               AC->ST[i].qn[3] = DbleVal[3];
            }
         }

         if (sscanf(line,"SC[%ld].AC.GPS[%ld].Valid = %ld",
            &Isc,&i,
            &LongVal[0]) == 3) {
            if (Isc == AC->ID) {
               AC->GPS[i].Valid = LongVal[0];
            }
         }

         if (sscanf(line,"SC[%ld].AC.GPS[%ld].Rollover = %ld",
            &Isc,&i,
            &LongVal[0]) == 3) {
            if (Isc == AC->ID) {
               AC->GPS[i].Rollover = LongVal[0];
            }
         }

         if (sscanf(line,"SC[%ld].AC.GPS[%ld].Week = %ld",
            &Isc,&i,
            &LongVal[0]) == 3) {
            if (Isc == AC->ID) {
               AC->GPS[i].Week = LongVal[0];
            }
         }

         if (sscanf(line,"SC[%ld].AC.GPS[%ld].Sec = %le",
            &Isc,&i,
            &DbleVal[0]) == 3) {
            if (Isc == AC->ID) {
               AC->GPS[i].Sec = DbleVal[0];
            }
         }

         if (sscanf(line,"SC[%ld].AC.GPS[%ld].PosN = %le %le %le",
            &Isc,&i,
            &DbleVal[0],
            &DbleVal[1],
            &DbleVal[2]) == 5) {
            if (Isc == AC->ID) {
               AC->GPS[i].PosN[0] = DbleVal[0];
               AC->GPS[i].PosN[1] = DbleVal[1];
               AC->GPS[i].PosN[2] = DbleVal[2];
            }
         }

         if (sscanf(line,"SC[%ld].AC.GPS[%ld].VelN = %le %le %le",
            &Isc,&i,
            &DbleVal[0],
            &DbleVal[1],
            &DbleVal[2]) == 5) {
            if (Isc == AC->ID) {
               AC->GPS[i].VelN[0] = DbleVal[0];
               AC->GPS[i].VelN[1] = DbleVal[1];
               AC->GPS[i].VelN[2] = DbleVal[2];
            }
         }

         if (sscanf(line,"SC[%ld].AC.GPS[%ld].PosW = %le %le %le",
            &Isc,&i,
            &DbleVal[0],
            &DbleVal[1],
            &DbleVal[2]) == 5) {
            if (Isc == AC->ID) {
               AC->GPS[i].PosW[0] = DbleVal[0];
               AC->GPS[i].PosW[1] = DbleVal[1];
               AC->GPS[i].PosW[2] = DbleVal[2];
            }
         }

         if (sscanf(line,"SC[%ld].AC.GPS[%ld].VelW = %le %le %le",
            &Isc,&i,
            &DbleVal[0],
            &DbleVal[1],
            &DbleVal[2]) == 5) {
            if (Isc == AC->ID) {
               AC->GPS[i].VelW[0] = DbleVal[0];
               AC->GPS[i].VelW[1] = DbleVal[1];
               AC->GPS[i].VelW[2] = DbleVal[2];
            }
         }

         if (sscanf(line,"SC[%ld].AC.GPS[%ld].Lng = %le",
            &Isc,&i,
            &DbleVal[0]) == 3) {
            if (Isc == AC->ID) {
               AC->GPS[i].Lng = DbleVal[0];
            }
         }

         if (sscanf(line,"SC[%ld].AC.GPS[%ld].Lat = %le",
            &Isc,&i,
            &DbleVal[0]) == 3) {
            if (Isc == AC->ID) {
               AC->GPS[i].Lat = DbleVal[0];
            }
         }

         if (sscanf(line,"SC[%ld].AC.GPS[%ld].Alt = %le",
            &Isc,&i,
            &DbleVal[0]) == 3) {
            if (Isc == AC->ID) {
               AC->GPS[i].Alt = DbleVal[0];
            }
         }

         if (sscanf(line,"SC[%ld].AC.Accel[%ld].Acc = %le",
            &Isc,&i,
            &DbleVal[0]) == 3) {
            if (Isc == AC->ID) {
               AC->Accel[i].Acc = DbleVal[0];
            }
         }

         if (sscanf(line,"SC[%ld].AC.Whl[%ld].H = %le",
            &Isc,&i,
            &DbleVal[0]) == 3) {
            if (Isc == AC->ID) {
               AC->Whl[i].H = DbleVal[0];
            }
         }

         if (AC->ParmLoadEnabled) {
            if (sscanf(line,"SC[%ld].AC.ID = %ld",
               &Isc,
               &LongVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->ID = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.EchoEnabled = %ld",
               &Isc,
               &LongVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->EchoEnabled = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Nb = %ld",
               &Isc,
               &LongVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->Nb = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Ng = %ld",
               &Isc,
               &LongVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->Ng = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Nwhl = %ld",
               &Isc,
               &LongVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->Nwhl = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Nmtb = %ld",
               &Isc,
               &LongVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->Nmtb = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Nthr = %ld",
               &Isc,
               &LongVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->Nthr = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Ncmg = %ld",
               &Isc,
               &LongVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->Ncmg = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Ngyro = %ld",
               &Isc,
               &LongVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->Ngyro = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Nmag = %ld",
               &Isc,
               &LongVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->Nmag = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Ncss = %ld",
               &Isc,
               &LongVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->Ncss = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Nfss = %ld",
               &Isc,
               &LongVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->Nfss = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Nst = %ld",
               &Isc,
               &LongVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->Nst = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Ngps = %ld",
               &Isc,
               &LongVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->Ngps = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Nacc = %ld",
               &Isc,
               &LongVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->Nacc = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.DT = %le",
               &Isc,
               &DbleVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->DT = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.mass = %le",
               &Isc,
               &DbleVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->mass = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.cm = %le %le %le",
               &Isc,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 4) {
               if (Isc == AC->ID) {
                  AC->cm[0] = DbleVal[0];
                  AC->cm[1] = DbleVal[1];
                  AC->cm[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.MOI = %le %le %le %le %le %le %le %le %le",
               &Isc,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2],
               &DbleVal[3],
               &DbleVal[4],
               &DbleVal[5],
               &DbleVal[6],
               &DbleVal[7],
               &DbleVal[8]) == 10) {
               if (Isc == AC->ID) {
                  AC->MOI[0][0] = DbleVal[0];
                  AC->MOI[0][1] = DbleVal[1];
                  AC->MOI[0][2] = DbleVal[2];
                  AC->MOI[1][0] = DbleVal[3];
                  AC->MOI[1][1] = DbleVal[4];
                  AC->MOI[1][2] = DbleVal[5];
                  AC->MOI[2][0] = DbleVal[6];
                  AC->MOI[2][1] = DbleVal[7];
                  AC->MOI[2][2] = DbleVal[8];
               }
            }

            if (sscanf(line,"SC[%ld].AC.B[%ld].mass = %le",
               &Isc,&i,
               &DbleVal[0]) == 3) {
               if (Isc == AC->ID) {
                  AC->B[i].mass = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.B[%ld].cm = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->B[i].cm[0] = DbleVal[0];
                  AC->B[i].cm[1] = DbleVal[1];
                  AC->B[i].cm[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.B[%ld].MOI = %le %le %le %le %le %le %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2],
               &DbleVal[3],
               &DbleVal[4],
               &DbleVal[5],
               &DbleVal[6],
               &DbleVal[7],
               &DbleVal[8]) == 11) {
               if (Isc == AC->ID) {
                  AC->B[i].MOI[0][0] = DbleVal[0];
                  AC->B[i].MOI[0][1] = DbleVal[1];
                  AC->B[i].MOI[0][2] = DbleVal[2];
                  AC->B[i].MOI[1][0] = DbleVal[3];
                  AC->B[i].MOI[1][1] = DbleVal[4];
                  AC->B[i].MOI[1][2] = DbleVal[5];
                  AC->B[i].MOI[2][0] = DbleVal[6];
                  AC->B[i].MOI[2][1] = DbleVal[7];
                  AC->B[i].MOI[2][2] = DbleVal[8];
               }
            }

            if (sscanf(line,"SC[%ld].AC.G[%ld].IsUnderActiveControl = %ld",
               &Isc,&i,
               &LongVal[0]) == 3) {
               if (Isc == AC->ID) {
                  AC->G[i].IsUnderActiveControl = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.G[%ld].IsSpherical = %ld",
               &Isc,&i,
               &LongVal[0]) == 3) {
               if (Isc == AC->ID) {
                  AC->G[i].IsSpherical = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.G[%ld].RotDOF = %ld",
               &Isc,&i,
               &LongVal[0]) == 3) {
               if (Isc == AC->ID) {
                  AC->G[i].RotDOF = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.G[%ld].TrnDOF = %ld",
               &Isc,&i,
               &LongVal[0]) == 3) {
               if (Isc == AC->ID) {
                  AC->G[i].TrnDOF = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.G[%ld].RotSeq = %ld",
               &Isc,&i,
               &LongVal[0]) == 3) {
               if (Isc == AC->ID) {
                  AC->G[i].RotSeq = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.G[%ld].TrnSeq = %ld",
               &Isc,&i,
               &LongVal[0]) == 3) {
               if (Isc == AC->ID) {
                  AC->G[i].TrnSeq = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.G[%ld].CGiBi = %le %le %le %le %le %le %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2],
               &DbleVal[3],
               &DbleVal[4],
               &DbleVal[5],
               &DbleVal[6],
               &DbleVal[7],
               &DbleVal[8]) == 11) {
               if (Isc == AC->ID) {
                  AC->G[i].CGiBi[0][0] = DbleVal[0];
                  AC->G[i].CGiBi[0][1] = DbleVal[1];
                  AC->G[i].CGiBi[0][2] = DbleVal[2];
                  AC->G[i].CGiBi[1][0] = DbleVal[3];
                  AC->G[i].CGiBi[1][1] = DbleVal[4];
                  AC->G[i].CGiBi[1][2] = DbleVal[5];
                  AC->G[i].CGiBi[2][0] = DbleVal[6];
                  AC->G[i].CGiBi[2][1] = DbleVal[7];
                  AC->G[i].CGiBi[2][2] = DbleVal[8];
               }
            }

            if (sscanf(line,"SC[%ld].AC.G[%ld].CBoGo = %le %le %le %le %le %le %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2],
               &DbleVal[3],
               &DbleVal[4],
               &DbleVal[5],
               &DbleVal[6],
               &DbleVal[7],
               &DbleVal[8]) == 11) {
               if (Isc == AC->ID) {
                  AC->G[i].CBoGo[0][0] = DbleVal[0];
                  AC->G[i].CBoGo[0][1] = DbleVal[1];
                  AC->G[i].CBoGo[0][2] = DbleVal[2];
                  AC->G[i].CBoGo[1][0] = DbleVal[3];
                  AC->G[i].CBoGo[1][1] = DbleVal[4];
                  AC->G[i].CBoGo[1][2] = DbleVal[5];
                  AC->G[i].CBoGo[2][0] = DbleVal[6];
                  AC->G[i].CBoGo[2][1] = DbleVal[7];
                  AC->G[i].CBoGo[2][2] = DbleVal[8];
               }
            }

            if (sscanf(line,"SC[%ld].AC.G[%ld].AngGain = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->G[i].AngGain[0] = DbleVal[0];
                  AC->G[i].AngGain[1] = DbleVal[1];
                  AC->G[i].AngGain[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.G[%ld].AngRateGain = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->G[i].AngRateGain[0] = DbleVal[0];
                  AC->G[i].AngRateGain[1] = DbleVal[1];
                  AC->G[i].AngRateGain[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.G[%ld].PosGain = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->G[i].PosGain[0] = DbleVal[0];
                  AC->G[i].PosGain[1] = DbleVal[1];
                  AC->G[i].PosGain[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.G[%ld].PosRateGain = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->G[i].PosRateGain[0] = DbleVal[0];
                  AC->G[i].PosRateGain[1] = DbleVal[1];
                  AC->G[i].PosRateGain[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.G[%ld].MaxAngRate = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->G[i].MaxAngRate[0] = DbleVal[0];
                  AC->G[i].MaxAngRate[1] = DbleVal[1];
                  AC->G[i].MaxAngRate[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.G[%ld].MaxPosRate = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->G[i].MaxPosRate[0] = DbleVal[0];
                  AC->G[i].MaxPosRate[1] = DbleVal[1];
                  AC->G[i].MaxPosRate[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.G[%ld].MaxTrq = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->G[i].MaxTrq[0] = DbleVal[0];
                  AC->G[i].MaxTrq[1] = DbleVal[1];
                  AC->G[i].MaxTrq[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.G[%ld].MaxFrc = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->G[i].MaxFrc[0] = DbleVal[0];
                  AC->G[i].MaxFrc[1] = DbleVal[1];
                  AC->G[i].MaxFrc[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Gyro[%ld].Axis = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->Gyro[i].Axis[0] = DbleVal[0];
                  AC->Gyro[i].Axis[1] = DbleVal[1];
                  AC->Gyro[i].Axis[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.MAG[%ld].Axis = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->MAG[i].Axis[0] = DbleVal[0];
                  AC->MAG[i].Axis[1] = DbleVal[1];
                  AC->MAG[i].Axis[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.CSS[%ld].Body = %ld",
               &Isc,&i,
               &LongVal[0]) == 3) {
               if (Isc == AC->ID) {
                  AC->CSS[i].Body = LongVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.CSS[%ld].Axis = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->CSS[i].Axis[0] = DbleVal[0];
                  AC->CSS[i].Axis[1] = DbleVal[1];
                  AC->CSS[i].Axis[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.CSS[%ld].Scale = %le",
               &Isc,&i,
               &DbleVal[0]) == 3) {
               if (Isc == AC->ID) {
                  AC->CSS[i].Scale = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.FSS[%ld].qb = %le %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2],
               &DbleVal[3]) == 6) {
               if (Isc == AC->ID) {
                  AC->FSS[i].qb[0] = DbleVal[0];
                  AC->FSS[i].qb[1] = DbleVal[1];
                  AC->FSS[i].qb[2] = DbleVal[2];
                  AC->FSS[i].qb[3] = DbleVal[3];
               }
            }

            if (sscanf(line,"SC[%ld].AC.FSS[%ld].CB = %le %le %le %le %le %le %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2],
               &DbleVal[3],
               &DbleVal[4],
               &DbleVal[5],
               &DbleVal[6],
               &DbleVal[7],
               &DbleVal[8]) == 11) {
               if (Isc == AC->ID) {
                  AC->FSS[i].CB[0][0] = DbleVal[0];
                  AC->FSS[i].CB[0][1] = DbleVal[1];
                  AC->FSS[i].CB[0][2] = DbleVal[2];
                  AC->FSS[i].CB[1][0] = DbleVal[3];
                  AC->FSS[i].CB[1][1] = DbleVal[4];
                  AC->FSS[i].CB[1][2] = DbleVal[5];
                  AC->FSS[i].CB[2][0] = DbleVal[6];
                  AC->FSS[i].CB[2][1] = DbleVal[7];
                  AC->FSS[i].CB[2][2] = DbleVal[8];
               }
            }

            if (sscanf(line,"SC[%ld].AC.ST[%ld].qb = %le %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2],
               &DbleVal[3]) == 6) {
               if (Isc == AC->ID) {
                  AC->ST[i].qb[0] = DbleVal[0];
                  AC->ST[i].qb[1] = DbleVal[1];
                  AC->ST[i].qb[2] = DbleVal[2];
                  AC->ST[i].qb[3] = DbleVal[3];
               }
            }

            if (sscanf(line,"SC[%ld].AC.ST[%ld].CB = %le %le %le %le %le %le %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2],
               &DbleVal[3],
               &DbleVal[4],
               &DbleVal[5],
               &DbleVal[6],
               &DbleVal[7],
               &DbleVal[8]) == 11) {
               if (Isc == AC->ID) {
                  AC->ST[i].CB[0][0] = DbleVal[0];
                  AC->ST[i].CB[0][1] = DbleVal[1];
                  AC->ST[i].CB[0][2] = DbleVal[2];
                  AC->ST[i].CB[1][0] = DbleVal[3];
                  AC->ST[i].CB[1][1] = DbleVal[4];
                  AC->ST[i].CB[1][2] = DbleVal[5];
                  AC->ST[i].CB[2][0] = DbleVal[6];
                  AC->ST[i].CB[2][1] = DbleVal[7];
                  AC->ST[i].CB[2][2] = DbleVal[8];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Accel[%ld].PosB = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->Accel[i].PosB[0] = DbleVal[0];
                  AC->Accel[i].PosB[1] = DbleVal[1];
                  AC->Accel[i].PosB[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Accel[%ld].Axis = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->Accel[i].Axis[0] = DbleVal[0];
                  AC->Accel[i].Axis[1] = DbleVal[1];
                  AC->Accel[i].Axis[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Whl[%ld].Axis = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->Whl[i].Axis[0] = DbleVal[0];
                  AC->Whl[i].Axis[1] = DbleVal[1];
                  AC->Whl[i].Axis[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Whl[%ld].DistVec = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->Whl[i].DistVec[0] = DbleVal[0];
                  AC->Whl[i].DistVec[1] = DbleVal[1];
                  AC->Whl[i].DistVec[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Whl[%ld].J = %le",
               &Isc,&i,
               &DbleVal[0]) == 3) {
               if (Isc == AC->ID) {
                  AC->Whl[i].J = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Whl[%ld].Tmax = %le",
               &Isc,&i,
               &DbleVal[0]) == 3) {
               if (Isc == AC->ID) {
                  AC->Whl[i].Tmax = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Whl[%ld].Hmax = %le",
               &Isc,&i,
               &DbleVal[0]) == 3) {
               if (Isc == AC->ID) {
                  AC->Whl[i].Hmax = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.MTB[%ld].Axis = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->MTB[i].Axis[0] = DbleVal[0];
                  AC->MTB[i].Axis[1] = DbleVal[1];
                  AC->MTB[i].Axis[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.MTB[%ld].DistVec = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->MTB[i].DistVec[0] = DbleVal[0];
                  AC->MTB[i].DistVec[1] = DbleVal[1];
                  AC->MTB[i].DistVec[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.MTB[%ld].Mmax = %le",
               &Isc,&i,
               &DbleVal[0]) == 3) {
               if (Isc == AC->ID) {
                  AC->MTB[i].Mmax = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Thr[%ld].PosB = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->Thr[i].PosB[0] = DbleVal[0];
                  AC->Thr[i].PosB[1] = DbleVal[1];
                  AC->Thr[i].PosB[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Thr[%ld].Axis = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->Thr[i].Axis[0] = DbleVal[0];
                  AC->Thr[i].Axis[1] = DbleVal[1];
                  AC->Thr[i].Axis[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Thr[%ld].rxA = %le %le %le",
               &Isc,&i,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 5) {
               if (Isc == AC->ID) {
                  AC->Thr[i].rxA[0] = DbleVal[0];
                  AC->Thr[i].rxA[1] = DbleVal[1];
                  AC->Thr[i].rxA[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.Thr[%ld].Fmax = %le",
               &Isc,&i,
               &DbleVal[0]) == 3) {
               if (Isc == AC->ID) {
                  AC->Thr[i].Fmax = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.PrototypeCtrl.wc = %le",
               &Isc,
               &DbleVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->PrototypeCtrl.wc = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.PrototypeCtrl.amax = %le",
               &Isc,
               &DbleVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->PrototypeCtrl.amax = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.PrototypeCtrl.vmax = %le",
               &Isc,
               &DbleVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->PrototypeCtrl.vmax = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.PrototypeCtrl.Kprec = %le",
               &Isc,
               &DbleVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->PrototypeCtrl.Kprec = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.PrototypeCtrl.Knute = %le",
               &Isc,
               &DbleVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->PrototypeCtrl.Knute = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.AdHocCtrl.Kr = %le %le %le",
               &Isc,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 4) {
               if (Isc == AC->ID) {
                  AC->AdHocCtrl.Kr[0] = DbleVal[0];
                  AC->AdHocCtrl.Kr[1] = DbleVal[1];
                  AC->AdHocCtrl.Kr[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.AdHocCtrl.Kp = %le %le %le",
               &Isc,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 4) {
               if (Isc == AC->ID) {
                  AC->AdHocCtrl.Kp[0] = DbleVal[0];
                  AC->AdHocCtrl.Kp[1] = DbleVal[1];
                  AC->AdHocCtrl.Kp[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.SpinnerCtrl.Ispin = %le",
               &Isc,
               &DbleVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->SpinnerCtrl.Ispin = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.SpinnerCtrl.Itrans = %le",
               &Isc,
               &DbleVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->SpinnerCtrl.Itrans = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.SpinnerCtrl.SpinRate = %le",
               &Isc,
               &DbleVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->SpinnerCtrl.SpinRate = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.SpinnerCtrl.Knute = %le",
               &Isc,
               &DbleVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->SpinnerCtrl.Knute = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.SpinnerCtrl.Kprec = %le",
               &Isc,
               &DbleVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->SpinnerCtrl.Kprec = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.ThreeAxisCtrl.Kr = %le %le %le",
               &Isc,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 4) {
               if (Isc == AC->ID) {
                  AC->ThreeAxisCtrl.Kr[0] = DbleVal[0];
                  AC->ThreeAxisCtrl.Kr[1] = DbleVal[1];
                  AC->ThreeAxisCtrl.Kr[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.ThreeAxisCtrl.Kp = %le %le %le",
               &Isc,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 4) {
               if (Isc == AC->ID) {
                  AC->ThreeAxisCtrl.Kp[0] = DbleVal[0];
                  AC->ThreeAxisCtrl.Kp[1] = DbleVal[1];
                  AC->ThreeAxisCtrl.Kp[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.ThreeAxisCtrl.Kunl = %le",
               &Isc,
               &DbleVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->ThreeAxisCtrl.Kunl = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.IssCtrl.Kr = %le %le %le",
               &Isc,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 4) {
               if (Isc == AC->ID) {
                  AC->IssCtrl.Kr[0] = DbleVal[0];
                  AC->IssCtrl.Kr[1] = DbleVal[1];
                  AC->IssCtrl.Kr[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.IssCtrl.Kp = %le %le %le",
               &Isc,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 4) {
               if (Isc == AC->ID) {
                  AC->IssCtrl.Kp[0] = DbleVal[0];
                  AC->IssCtrl.Kp[1] = DbleVal[1];
                  AC->IssCtrl.Kp[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.IssCtrl.Tmax = %le",
               &Isc,
               &DbleVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->IssCtrl.Tmax = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.CmgCtrl.Kr = %le %le %le",
               &Isc,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 4) {
               if (Isc == AC->ID) {
                  AC->CmgCtrl.Kr[0] = DbleVal[0];
                  AC->CmgCtrl.Kr[1] = DbleVal[1];
                  AC->CmgCtrl.Kr[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.CmgCtrl.Kp = %le %le %le",
               &Isc,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 4) {
               if (Isc == AC->ID) {
                  AC->CmgCtrl.Kp[0] = DbleVal[0];
                  AC->CmgCtrl.Kp[1] = DbleVal[1];
                  AC->CmgCtrl.Kp[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.ThrCtrl.Kw = %le %le %le",
               &Isc,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 4) {
               if (Isc == AC->ID) {
                  AC->ThrCtrl.Kw[0] = DbleVal[0];
                  AC->ThrCtrl.Kw[1] = DbleVal[1];
                  AC->ThrCtrl.Kw[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.ThrCtrl.Kth = %le %le %le",
               &Isc,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 4) {
               if (Isc == AC->ID) {
                  AC->ThrCtrl.Kth[0] = DbleVal[0];
                  AC->ThrCtrl.Kth[1] = DbleVal[1];
                  AC->ThrCtrl.Kth[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.ThrCtrl.Kv = %le",
               &Isc,
               &DbleVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->ThrCtrl.Kv = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.ThrCtrl.Kp = %le",
               &Isc,
               &DbleVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->ThrCtrl.Kp = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.CfsCtrl.Kr = %le %le %le",
               &Isc,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 4) {
               if (Isc == AC->ID) {
                  AC->CfsCtrl.Kr[0] = DbleVal[0];
                  AC->CfsCtrl.Kr[1] = DbleVal[1];
                  AC->CfsCtrl.Kr[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.CfsCtrl.Kp = %le %le %le",
               &Isc,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 4) {
               if (Isc == AC->ID) {
                  AC->CfsCtrl.Kp[0] = DbleVal[0];
                  AC->CfsCtrl.Kp[1] = DbleVal[1];
                  AC->CfsCtrl.Kp[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.CfsCtrl.Kunl = %le",
               &Isc,
               &DbleVal[0]) == 2) {
               if (Isc == AC->ID) {
                  AC->CfsCtrl.Kunl = DbleVal[0];
               }
            }

            if (sscanf(line,"SC[%ld].AC.ThrSteerCtrl.Kr = %le %le %le",
               &Isc,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 4) {
               if (Isc == AC->ID) {
                  AC->ThrSteerCtrl.Kr[0] = DbleVal[0];
                  AC->ThrSteerCtrl.Kr[1] = DbleVal[1];
                  AC->ThrSteerCtrl.Kr[2] = DbleVal[2];
               }
            }

            if (sscanf(line,"SC[%ld].AC.ThrSteerCtrl.Kp = %le %le %le",
               &Isc,
               &DbleVal[0],
               &DbleVal[1],
               &DbleVal[2]) == 4) {
               if (Isc == AC->ID) {
                  AC->ThrSteerCtrl.Kp[0] = DbleVal[0];
                  AC->ThrSteerCtrl.Kp[1] = DbleVal[1];
                  AC->ThrSteerCtrl.Kp[2] = DbleVal[2];
               }
            }

         }


         if (!strncmp(line,"[EOF]",5)) {
            Done = 1;
            sprintf(line,"[EOF] reached\n");
         }
         if (Imsg > 16383) {
            Done = 1;
            printf("Imsg limit exceeded\n");
         }
      }

      /* Acknowledge receipt */
      send(Socket,AckMsg,strlen(AckMsg),0);

      if (RequestTimeRefresh) {
         /* Update AC->Time */
         DOY2MD(Year,doy,&Month,&Day);
         AC->Time = DateToAbsTime(Year,Month,Day,Hour,Minute,Second);
      }

}