void Fpssystm::PostRead(int indx, int fail, int warn, ListMcm* list_ptr)
{
  SysBase<LowfWrite, LowfRead>::PostRead(indx, fail, warn, list_ptr);
  if (fail <= 0) { 
    AnsStatus = Rx->DecodeBuffer(list_ptr->Ans); // DECODEBUFFER
    DecodeList(list_ptr);
  }
  else DataPlus.Length = 0;
}

void Fpssystm::DecodeList(ListMcm* list_ptr)
{
  int i;

  FpsStatus = list_ptr->Ans.Msg[9];
  DecodeStatus(FpsStatus);
  EncCount = (list_ptr->Ans.Msg[10] + (list_ptr->Ans.Msg[11]<<8))*2;
  Rpm = (list_ptr->Ans.Msg[12]*5*60.0)/(2048.0) * 300.0;
  if ((int)list_ptr->Ans.Msg[5] == 2) {
    DataPlus.Length = list_ptr->Ans.Msg[13] + (list_ptr->Ans.Msg[14]<<8);
    for (i=0;i<DataPlus.Length;i++) DataPlus.Msg[i] = list_ptr->Ans.Msg[13+i];
  }
  else DataPlus.Length = 0;
  TimeStamp();     cout << "FPS list answered\n";
}

int LowRead::DecodeBuffer(const MSG CmdRx)
{
 // int i;
  char CmdStatus[100];
 // cout << " I M in LowMcmread decode buffer\n " ;
/*  for (i=0 ;i < CmdRx.Length ;i++)
  {
    printf ( " Data at Lowread Decode buffer is %x\n",CmdRx.Msg[i]);
  } */
 switch (CmdRx.Msg[4]) {
    case 0 : sprintf(CmdStatus, "MCM %d: Command OK", CmdRx.Msg[0]); break;
    case 1 : sprintf(CmdStatus, "MCM %d: TimeOut", CmdRx.Msg[0]); break;
    case 2 : sprintf(CmdStatus, "MCM %d: Command Check Sum Error", CmdRx.Msg[0]); break;
    case 4 : sprintf(CmdStatus, "MCM %d: Command unknown", CmdRx.Msg[0]); break;
    case 8 : sprintf(CmdStatus, "MCM %d: Packet too large", CmdRx.Msg[0]); break;
    case 16 : sprintf(CmdStatus, "MCM %d: Answer Message truncated", CmdRx.Msg[0]); break;
  }
  if (CmdRx.Msg[4]) *fdrec << "\t\t\t" << CmdStatus << endl;
  return CmdRx.Msg[4];
}

void Fpssystm::DecodeStatus(const char stat)
{
  char cur = stat;

  switch(cur) {
    case 0x42: *FileRec->fd << "280 Limit Hit "; break;
    case 0x43: *FileRec->fd << "-10 Limit Hit ";
  }
  cur &= 0xfe;
  switch(cur) {
    case 0x00: *FileRec->fd << "Feed Calibrated & Idle "; break;
    case 0x40: *FileRec->fd << "Feed Un-Calibrated "; break;
    case 0x08: *FileRec->fd << "Busy - Preset Run "; break;
    case 0x10: *FileRec->fd << "Busy - Free Run "; break;
    case 0x4a: *FileRec->fd << "Busy - Passworded Run "; break;
    case 0x0c0: *FileRec->fd << "Busy - In Calibration "; break;
    case 0x28: *FileRec->fd << "Jammed - Preset Run "; break;
    case 0x30: *FileRec->fd << "Jammed - Free Run "; break;
    case 0x0e0: *FileRec->fd << "Jammed - In Calibration "; break;
    case 0x62: *FileRec->fd << "Jammed - Passworded Run "; break;
    case 0x20: *FileRec->fd << "Jammed "; break;
    case 0x0ff: *FileRec->fd << "Ua0 Offset Progressing ";
  }
  *FileRec->fd << flush;
}

/******************************************************************************
 RxPop class inline functions
******************************************************************************/ 
inline void RxPop::TOpop(char *str)
{
  int i=0;
  while (ToOnl->Msg[ToOnl->Length] != ',')
    str[i++] = ToOnl->Msg[ToOnl->Length++];
  str[i] = '\0';
  TOpopcoma();      
}

inline void RxPop::TOpop(int *param, int len)
{
  *param = ToOnl->Msg[ToOnl->Length++];
  if (len == 2) *param |= (ToOnl->Msg[ToOnl->Length++]<<8);
}

inline void RxPop::TOpop(int *param) {TOpop(param, 1);}

inline void RxPop::TOpop(unsigned char *param) {*param = ToOnl->Msg[ToOnl->Length++];}

inline void RxPop::TOpopcoma() {ToOnl->Length++;}

inline void RxPop::TOpop(float *param)
{
  char str[20];
  TOpop(str);
  *param = atof(str);
}


