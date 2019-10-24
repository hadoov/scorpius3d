/*
*********************************************************************************
*                            In the name of Almighty                            *
*                                                                               *
*           Connection.h : Robocup 3D Soccer Simulation Team Zigorat            *
*                     (This team was previously named Gcyrus)                   *
*                                                                               *
*  Date: 03/20/2007                                                             *
*  Author: Mahdi Hamdarsi                                                       *
*  Comments: Class decleration for TDataPorter, TConnection, TRoboCupConnection *
*            utilities for network manipulation                                 *
*                                                                               *
*********************************************************************************
*/

/*! \file Connection.h
<pre>
<b>File:</b>          Connection.h
<b>Project:</b>       Robocup Soccer Simulation Team: Zigorat
<b>Authors:</b>       Mahdi Hamdarsi
<b>Created:</b>       03/20/2007
<b>Last Revision:</b> $ID$
<b>Contents:</b>      Class declerations for TDataPorter, TConnection, TRoboCupConnection
               utilities for network manipulation
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
03/20/2007       Mahdi           Initial version created
</pre>
*/

#ifndef CONNECTION
#define CONNECTION

#include <arpa/inet.h>
#include <string.h>

using namespace std;

/*! ConnectionT is an enumeration of currently supported connection types */
enum ConnectionT
{
  CONN_TCP,
  CONN_UDP,
  CONN_ILLEGAL
};



/***************************************************************************/
/***************************  Class TDataPorter  ***************************/
/***************************************************************************/

/*! TDataPorter is class tha encapsulates socket connection operations, 
    like opening, reusing, sending & recieving data across sockets */
class TDataPorter
{
  protected:
    int         m_SocketFD;          /*!< Socket file desriptor of the connection */
    ConnectionT m_ConnectionType;    /*!< Type of the connection, ie. TCP/UDP */

  public:
      TDataPorter                    ( ConnectionT conn, int socket_fd = -1 );
    ~ TDataPorter                    (                                      );
    
    ConnectionT getConnectionType    (                                      ) const;

    int         getSocketFD          (                                      ) const;
    bool        setSocketFD          ( int socket_fd, ConnectionT conn      );

    int         sendMessage          ( const void * msg, int iLength        ) const;
    int         sendMessageUDP       ( const void * msg, int iLength,
                                          const sockaddr_in * remote        ) const;
    int         recvMessage          ( void * msg, int iMaxLength, int to   ) const;
    int         writeMessage         ( const char * msg                     ) const;
    int         writeMessage         ( const void * msg, int iLength        ) const;
    int         readMessage          ( void * msg, int iLength              ) const;
    
    bool        active               (                                      ) const;
    int         waitForData          ( fd_set &fds, int iSecs = 1           ) const;
    
    bool        open                 (                                      );
    bool        reOpen               (                                      );
    bool        closeAll             (                                      );
};



/***************************************************************************/
/***************************  Class TConnection  ***************************/
/***************************************************************************/

/*! TConnection is a class that encapsulates connection handling routines,
    like starting listening, accepting connections, sending & receiving 
    data, and making connection to existing listening servers, currently
    supports to modes of communication: TCP/IP & UDP/IP */
class TConnection
{
protected:
  sockaddr_in   m_SelfAddress;       /*!< Self socket information */
  sockaddr_in   m_Remote;            /*!< Remote connection which is established */
  TDataPorter * m_SocketFD;          /*!< Socket descriptor of this data porter */

public:
  bool          connected            (                                      ) const;
  ConnectionT   getConnectionType    (                                      ) const;
  int           getLocalPort         (                                      ) const;
  bool          setLocalPort         ( int iPort                            );
  
  int           getSocketFD          (                                      ) const;

  bool          openConnection       ( const char *str, int iPort           );
  bool          startServer          ( int port                             );
  bool          waitForConnection    (                                      );
  bool          closeConnection      (                                      );

  bool          waitForData          ( int time_out = 1                     ) const;
  bool          sendMessage          ( const char * strMsg                  ) const;
  bool          sendData             ( const void * data, int iLength       ) const;
  bool          writeMessage         ( const char * msg                     ) const;
  bool          writeMessage         ( const void * msg, int iLength        ) const;

  bool          recvMessage          ( char * strMsg, int iMaxLen,
                                       int time_out, bool close_empty=false );
  bool          readMessage          ( void * msg, int iMaxLength           ) const;

    TConnection                      ( ConnectionT conn, 
                                       const char * strHost = "",
                                       int iPort = -1                       );
  ~ TConnection                      (                                      );
};

/********************************************************************/
/*******************  Class TRoboCupConnection  *********************/
/********************************************************************/

/*! TRoboCupConnection is a class that encapsulates spark simulation server 
    handling methods. like recieving and sending data to simulation server. */
class TRoboCupConnection : public TConnection
{
public:
    TRoboCupConnection               ( const char *strHost = "", 
                                                           int iPort = -1   );
  ~ TRoboCupConnection               (                                      );

  bool sendCommand                   ( const char * msg                     ) const;
  bool getMessage                    ( char * msg, int iMaxLen              ) const;
};              

#endif // CONNECTION
  
