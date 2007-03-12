/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Authors : Benjamin GAUTHIER - 24 Mar 2004
 *            Joseph BANINO
 *            Olivier JACQUES
 *            Richard GAYRAUD
 *            From Hewlett Packard Company.
 *            Guillaume Teissier from FTR&D
 */

#include "sipp.hpp"
#ifdef PCAPPLAY
#include "prepare_pcap.h"
#endif

static const char* strIntCmd(CAction::T_IntCmdType type)
{
    switch (type)
    {
        case CAction::E_INTCMD_STOPCALL:
            return "stop_call";
        case CAction::E_INTCMD_STOP_ALL:
            return "stop_gracefully";
        case CAction::E_INTCMD_STOP_NOW:
            return "stop_now";

        default:
        case CAction::E_INTCMD_INVALID:
            return "invalid";
    }
    return "invalid";
}

void CAction::afficheInfo()
{
  if (M_action == E_AT_ASSIGN_FROM_REGEXP) {
    if(M_lookingPlace == E_LP_MSG) {
      printf("Type[%d] - where[%s] - checkIt[%d] - varId[%d]",
             M_action,
             "Full Msg",
             M_checkIt,
		       M_varId);
    } else {
      printf("Type[%d] - where[%s-%s] - checkIt[%d] - varId[%d]",
             M_action,
             "Header",
             M_lookingChar,
             M_checkIt,
		       M_varId);
    }
  } else if (M_action == E_AT_EXECUTE_CMD) {
    if (M_cmdLine) {
        printf("Type[%d] - command[%-32.32s]", M_action, M_cmdLine);
    }
  } else if (M_action == E_AT_EXEC_INTCMD) {
      printf("Type[%d] - intcmd[%-32.32s]", M_action, strIntCmd(M_IntCmd));
  } else if (M_action == E_AT_LOG_TO_FILE) {
      printf("Type[%d] - message[%-32.32s]", M_action, M_message);
#ifdef PCAPPLAY
  } else if ((M_action == E_AT_PLAY_PCAP_AUDIO) || (M_action == E_AT_PLAY_PCAP_VIDEO)) {
      printf("Type[%d] - file[%s]", M_action, M_pcapArgs->file);
#endif
  }
}


CAction::T_ActionType   CAction::getActionType()   { return(M_action);       }
CAction::T_VarType      CAction::getVarType()      { return(M_varType);      }
CAction::T_LookingPlace CAction::getLookingPlace() { return(M_lookingPlace); }
CAction::T_IntCmdType   CAction::getIntCmd ()      { return(M_IntCmd);       }

bool           CAction::getCheckIt()      { return(M_checkIt);      }
bool           CAction::getCaseIndep()    { return(M_caseIndep);    }
bool           CAction::getHeadersOnly()  { return(M_headersOnly);  }
int            CAction::getOccurence()    { return(M_occurence);    }
int            CAction::getVarId()        { return(M_varId);        }
char*          CAction::getLookingChar()  { return(M_lookingChar);  }
char*          CAction::getMessage()      { return(M_message);      }
char*          CAction::getCmdLine()      { return(M_cmdLine);      }
#ifdef PCAPPLAY
pcap_pkts  *   CAction::getPcapPkts()     { return(M_pcapArgs);     }
#endif

void CAction::setActionType   (CAction::T_ActionType   P_value) 
{ M_action       = P_value; }  
void CAction::setVarType      (CAction::T_VarType      P_value) 
{ M_varType      = P_value; }  
void CAction::setLookingPlace (CAction::T_LookingPlace P_value) 
{ M_lookingPlace = P_value; }
void CAction::setCheckIt      (bool           P_value) 
{ M_checkIt      = P_value; }
void CAction::setVarId        (int            P_value) 
{ M_varId        = P_value; }
void CAction::setCaseIndep    (bool           P_value)
{ M_caseIndep    = P_value; }
void CAction::setOccurence   (int            P_value) 
{ M_occurence    = P_value; }
void CAction::setHeadersOnly  (bool           P_value)
{ M_headersOnly  = P_value; }
void CAction::setIntCmd       (T_IntCmdType P_type) 
{ M_IntCmd       = P_type;  }


void CAction::setSubVarId (int    P_value) {
   if ( M_nbSubVarId < M_maxNbSubVarId ) {
     M_subVarId[M_nbSubVarId] = P_value; 
     M_nbSubVarId++;
   }
}

int  CAction::getSubVarId(int P_index) { 
    return(M_subVarId[P_index]);        
}

int*  CAction::getSubVarId() { 
    return(M_subVarId);        
}

void CAction::setNbSubVarId (int            P_value) {
   M_maxNbSubVarId        = P_value; 
   if(M_subVarId != NULL) {
     delete [] M_subVarId;
     M_subVarId      = NULL;
   }
   M_subVarId = new int[M_maxNbSubVarId] ;
   M_nbSubVarId = 0 ;
}
int  CAction::getNbSubVarId () {
    return(M_nbSubVarId);        
}


void CAction::setLookingChar  (char*          P_value)
{
  if(M_lookingChar != NULL)
  {
    delete [] M_lookingChar;
    M_lookingChar = NULL;
  }

  if(P_value != NULL)
  { 
    M_lookingChar = new char[strlen(P_value)+1];
    strcpy(M_lookingChar, P_value);
  }
}

void CAction::setMessage  (char*          P_value)
{
  if(M_message != NULL)
  {
    delete [] M_message;
    M_message = NULL;
  }

  if(P_value != NULL)
  { 
    M_message = new char[strlen(P_value)+1];
    strcpy(M_message, P_value);
  }
}

void CAction::setCmdLine  (char*          P_value)
{
  if(M_cmdLine != NULL)
  {
    delete [] M_cmdLine;
    M_cmdLine = NULL;
  }

  if(P_value != NULL)
  { 
    M_cmdLine = new char[strlen(P_value)+1];
    strcpy(M_cmdLine, P_value);
  }
}

#ifdef PCAPPLAY
void CAction::setPcapArgs (pcap_pkts  *  P_value)
{
  if(M_pcapArgs != NULL)
  {
    free(M_pcapArgs);
    M_pcapArgs = NULL;
  }

  if(P_value != NULL)
  { 
    M_pcapArgs = (pcap_pkts *)malloc(sizeof(*M_pcapArgs));
    memcpy(M_pcapArgs, P_value, sizeof(*M_pcapArgs));
  }
}

void CAction::setPcapArgs (char*        P_value)
{
  if(M_pcapArgs != NULL)
  {
    free(M_pcapArgs);
    M_pcapArgs = NULL;
  }

  if(P_value != NULL)
  { 
    M_pcapArgs = (pcap_pkts *) malloc(sizeof(*M_pcapArgs));
    if (parse_play_args(P_value, M_pcapArgs) == -1)
    {
      ERROR("Play pcap error");
    }
    if (access(M_pcapArgs->file, F_OK)) {
      ERROR_P1("Cannot read file %s\n", M_pcapArgs->file);
    }
  }
}
#endif

void CAction::setAction(CAction P_action)
{
  int L_i;
  setActionType   ( P_action.getActionType()   );
  setVarType      ( P_action.getVarType()      );
  setLookingPlace ( P_action.getLookingPlace() );
  setVarId        ( P_action.getVarId()        );

  setNbSubVarId   ( P_action.getNbSubVarId()   );
  for (L_i = 0; L_i < P_action.getNbSubVarId() ; L_i++ ) {
    setSubVarId (P_action.getSubVarId(L_i));
  }

  setLookingChar  ( P_action.getLookingChar()  );
  setCheckIt      ( P_action.getCheckIt()      );
  setCaseIndep    ( P_action.getCaseIndep()    ); 
  setOccurence    ( P_action.getOccurence()   );
  setHeadersOnly  ( P_action.getHeadersOnly()  );
  setMessage      ( P_action.M_message         );
  setCmdLine      ( P_action.M_cmdLine         );
  setIntCmd       ( P_action.M_IntCmd          );
#ifdef PCAPPLAY
  setPcapArgs     ( P_action.M_pcapArgs        );
#endif
}

CAction::CAction()
{
  M_action       = E_AT_NO_ACTION;
  M_varId        = 0;

  M_nbSubVarId    = 0;
  M_maxNbSubVarId = 0;
  M_subVarId      = NULL;

  M_varType      = E_VT_UNDEFINED;
  M_checkIt      = false;
  M_lookingPlace = E_LP_MSG;
  M_lookingChar  = NULL;
  M_caseIndep    = false;
  M_occurence    = 1;
  M_headersOnly  = true;   
  M_message      = NULL;
  M_cmdLine      = NULL;
  M_IntCmd       = E_INTCMD_INVALID;
#ifdef PCAPPLAY
  M_pcapArgs     = NULL;
#endif
}

CAction::CAction(const CAction &P_action)
{
  int L_i ;

  M_lookingChar = NULL;
  M_message     = NULL;
  M_cmdLine     = NULL;
  M_subVarId    = NULL;
  M_IntCmd      = E_INTCMD_INVALID;
#ifdef PCAPPLAY
  M_pcapArgs    = NULL;
#endif

  // M_maxNbSubVarId = P_action.M_maxNbSubVarId ;
  setNbSubVarId   ( P_action.M_nbSubVarId );
  for (L_i = 0; L_i < M_maxNbSubVarId ; L_i++ ) {
    setSubVarId (P_action.M_subVarId[L_i]);
  }

  setActionType   ( P_action.M_action       );
  setVarType      ( P_action.M_varType      );
  setLookingPlace ( P_action.M_lookingPlace );
  setVarId        ( P_action.M_varId        );
  setLookingChar  ( P_action.M_lookingChar  );
  setCheckIt      ( P_action.M_checkIt      );
  setCaseIndep    ( P_action.M_caseIndep    );
  setOccurence    ( P_action.M_occurence   );
  setHeadersOnly  ( P_action.M_headersOnly  );   
  setMessage      ( P_action.M_message      );
  setCmdLine      ( P_action.M_cmdLine      );
  setIntCmd       ( P_action.M_IntCmd       );
#ifdef PCAPPLAY
  setPcapArgs     ( P_action.M_pcapArgs     );
#endif
}  

CAction::~CAction()
{
  if(M_lookingChar != NULL)
  {
    delete [] M_lookingChar;
    M_lookingChar = NULL;
  }
  if(M_message != NULL)
  {
    delete [] M_message;
    M_message = NULL;
  }
  if(M_cmdLine != NULL)
  {
    delete [] M_cmdLine;
    M_cmdLine = NULL;
  }
  if(M_subVarId != NULL)
  {
    delete [] M_subVarId;
    M_subVarId      = NULL;
  }
#ifdef PCAPPLAY
  if (M_pcapArgs != NULL) {
    free(M_pcapArgs);
    M_pcapArgs = NULL;
  }
#endif
}

/****************************** CActions class ************************/

void CActions::afficheInfo()
{
  printf("Action Max Size = [%d] - Action Current Setted[%d]\n", M_nbAction, M_currentSettedAction);
  for(int i=0; i<M_currentSettedAction; i++)
  {
    printf("actionlist[%d] : \n", i);
    M_actionList[i].afficheInfo();
  }
}

void CActions::reset()
{
  M_currentSettedAction = 0;
}

int CActions::getMaxSize()
{
  return(M_nbAction);
}

int CActions::getUsedAction()
{
  return(M_currentSettedAction);
}

void CActions::setAction(CAction P_action)
{
  if(M_currentSettedAction < M_nbAction)
  {
    M_actionList[M_currentSettedAction].setAction(P_action);
    M_currentSettedAction++;
  }
}

CAction* CActions::getAction(int i)
{
  if(i < M_currentSettedAction)
  {
    return(&M_actionList[i]);
  }
  else
    return(NULL);
}


// constructor and destructor

CActions::CActions(const CActions &P_actions)
{
    M_currentSettedAction = 0;
    M_nbAction = P_actions.M_currentSettedAction;
    M_actionList = new CAction[M_nbAction];
  
    for(int i=0; i<M_nbAction; i++)
	 {
      setAction(P_actions.M_actionList[i]);
	 }
}
  

CActions::CActions(int P_nbAction)
{
  M_nbAction = P_nbAction;
  M_currentSettedAction = 0;
  M_actionList = new CAction[P_nbAction];
}


CActions::~CActions()
{
  delete [] M_actionList;
  M_actionList = NULL;
}
