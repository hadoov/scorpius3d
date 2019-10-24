/*
*********************************************************************************
*                            In the name of Almighty                            *
*                                                                               *
*          Connection.cpp : Robocup 3D Soccer Simulation Team Zigorat           *
*                     (This team was previously named Gcyrus)                   *
*                                                                               *
*  Date: 03/20/2007                                                             *
*  Author: Mahdi Hamdarsi                                                       *
*  Comments: Class definitions for TDataPorter, TConnection, TRoboCupConnection *
*            utilities for network manipulation                                 *
*                                                                               *
*********************************************************************************
*/

/*! \file Connection.cpp
<pre>
<b>File:</b>          Connection.cpp
<b>Project:</b>       Robocup Soccer Simulation Team: Zigorat
<b>Authors:</b>       Mahdi Hamdarsi
<b>Created:</b>       03/20/2007
<b>Last Revision:</b> $ID$
<b>Contents:</b>      Class definitions for TDataPorter, TConnection, TRoboCupConnection
               utilities for network manipulation
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
03/20/2007       Mahdi           Initial version created
</pre>
*/

#include "Connection.h"
//#include "Logger.h"

#include <sys/wait.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>

#define MAX_CONN_QUEUE 5 /*!< Maximum connection waiting to be listened */


using namespace std;



/********************************************************************/
/***********************  Class TDataPorter  ************************/
/********************************************************************/


/*! Constructor of TDataPorter, initializes connection mode (UDP/TCP)
    & creates sockets to send/recv data
    \param conn ConnectionT connection mode: UDP/TCP
    \param socket_fd int pre-initialized socket descriptor */
TDataPorter::TDataPorter( ConnectionT conn, int socket_fd )
{
  m_SocketFD = -1;
  setSocketFD( socket_fd, conn );
}

/*! Destructor of TDataPorter, closes available connections */
TDataPorter::~TDataPorter()
{
  if( m_SocketFD != -1 )
    close(m_SocketFD);
}

/*! This method returns connection mode: UDP/TCP
    \return ConnectionT connection mode */
ConnectionT TDataPorter::getConnectionType() const
{
  return m_ConnectionType;
}

/*! This methos returns socket descriptor
    \return socket descriptor of current socket conenction */
int TDataPorter::getSocketFD() const
{
  return m_SocketFD;
}

/*! This method sets the current socket descriptor to a pre-initialiazed
    socket holder, and sets the connection mode
    \param socket_fd pre-initialized socket descriptor
    \param conn Connection mode: TCP/UDP
    \return bool inidicating wheather class initialized or not */
bool TDataPorter::setSocketFD( int socket_fd, ConnectionT conn )
{
  if( active() )
    return false;

  m_ConnectionType = conn;
  m_SocketFD = socket_fd;
  return true;
}

/*! This method sends a message through socket connection which is made
    \param msg message or data to send
    \param iLength length of message to send
    \return int number of bytes totally sent */
int TDataPorter::sendMessage( const void * msg, int iLength ) const
{
  if( !active() )
    return -1;

  int iRet = -1;
  if( getConnectionType() == CONN_TCP )
    iRet = send( m_SocketFD, msg, iLength, 0 );
  else
  {
    cerr << "TDataPorter::sendMessage(TCP): attempted to send to a UDP packet via a TCP socket" << endl;
    return -1;
  }

  if( iRet == -1 )
    perror( "TDataPorter::sendMessage" );

  return iRet;
}

/*! This method sends a message through a UDP socket connection which is made
    \param msg message or data to send
    \param iLength length of message to send
    \param remote Socket descriptor of the data porter
    \return int number of bytes totally sent */
int TDataPorter::sendMessageUDP( const void * msg, int iLength, const sockaddr_in * remote ) const
{
  if( !active() )
    return -1;

  int iRet = -1;
  if( getConnectionType() == CONN_UDP )
    iRet = sendto( m_SocketFD, msg, iLength, 0, (sockaddr*)remote, sizeof( *remote ) );
  else
  {
    cerr << "TDataPorter::sendMessage(UDP): attempted to send to a TCP packet via a UDP socket" << endl;
    return 0;
  }

  if( iRet == -1 )
    perror( "TDataPorter::sendMessage" );

  return iRet;
}

/*! This method recieves any data which is ready in connection to read
    \param msg pointer to write recieved bytes to
    \param iMaxLength maximum length of bytes that can be stored in msg
    \param time_out Maximum tolerable time for data to be recieved
    \return int number of bytes totally recieved */
int TDataPorter::recvMessage( void * msg, int iMaxLength, int time_out ) const
{
  time_out = 1000 ;
	if( !active() )
    return -1;

  fd_set fds;
  int iReturn = waitForData( fds, time_out );
  if( iReturn == -1 )
  {
    perror( "TDataPorter::recvMessage(select)" );
    return false;
  }
  else if( iReturn == 0 )
  {
    cerr << "TDataPorter::recvMessage: Connection Timed out" << endl;
    return false;
  }

  if( FD_ISSET( getSocketFD(), &fds ) )
  {
    if( getConnectionType() == CONN_TCP )
      iReturn = recv( getSocketFD(), msg, iMaxLength, 0);
    else
    {
      sockaddr remote;
      socklen_t from_len = sizeof( sockaddr_in );

      iReturn = recvfrom( getSocketFD(), msg, iMaxLength, 0, &remote, &from_len );
    }
  }
  else
    return false;
    
  if( iReturn == -1 )
  {
    perror( "TDataPorter::recvMessage" );
    return false;
  }
  else if( iReturn == 0 )
    return false;
  else
    ( ( (char*)msg )[ iReturn ] ) = '\0';

  return iReturn;
}

/*! This method writes a message to its peer
    \param msg message to be written for peer
    \return int number of charactors sent */
int TDataPorter::writeMessage( const char * msg ) const
{
  return writeMessage( (void*)(msg), strlen(msg) );
}

/*! This method writes a message to its peer
    \param msg message to be written for peer
    \param iLength message length
    \return int number of charactors sent */
int TDataPorter::writeMessage( const void * msg, int iLength ) const
{
  if( !active() || getConnectionType() != CONN_TCP )
    return -1;

  return ( write( getSocketFD(), msg, iLength ) );
}

/*! This method reads messages from a network connection
    \param msg buffer to write message to
    \param iLength Maximum length of buffer
    \return int number of bytes totally read */
int TDataPorter::readMessage( void * msg, int iLength ) const
{
  fd_set fds;
  FD_ZERO( &fds );
  FD_SET( getSocketFD(), &fds );
  
  ((char*)msg)[0] = '\0';

  if( select( getSocketFD() + 1, &fds, 0, 0, 0 ) > 0 )
    read( getSocketFD(), msg, iLength );
  else
    return false;

  return true;
}

/*! This method determines wheather this socket is active or not
    \return bool wheather connection is open */
bool TDataPorter::active() const
{
  return ( m_SocketFD != -1 );
}

/*! This method waits for data  to arrive on socket, waits until data
     comes or a time-out event occures
     \param fds Internal variable to synch
     \param iSecs seconds to wait until data time-out occures */
int TDataPorter::waitForData( fd_set & fds, int iSecs ) const
{
  timeval tv;
  
  tv.tv_sec = iSecs;
  tv.tv_usec = 1000000;

  FD_ZERO( &fds );
  FD_SET( this->getSocketFD(), &fds );

  return ( select( getSocketFD() + 1, &fds, NULL, NULL, &tv ) >= 0 );
}

/*! This method opens the socket for the first time & initializes
    the socket properties
    \return bool indicating update was successful */
bool TDataPorter::open()
{
  if( active() )
    return false;

  int conn = ( m_ConnectionType == CONN_TCP ? SOCK_STREAM : SOCK_DGRAM );
  int yes = 1;

  if( ( m_SocketFD = socket(PF_INET, conn, 0) ) == -1 )
  {
    perror( "TDataPorter::socket" );
    return false;
  }

  if( setsockopt( m_SocketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int) ) == -1 )
  {
    perror( "TDataPorter::setsockopt" );
    return false;
  }

  return true;
}

/*! This method re-initializes the socket connection
    \return bool indicating wheather update was successful */
bool TDataPorter::reOpen()
{
  closeAll();
  open();
  
  return true;
}

/*! This method closes socket connections and clears internal fields
    \return bool inidicating wheather socket closed normally or not*/
bool TDataPorter::closeAll()
{
  if( !active() )
    return false;

  close( m_SocketFD );
  m_SocketFD = -1;
  return true;
}



/********************************************************************/
/***********************  Class TConnection  ************************/
/********************************************************************/

/*! This is the constructor for TConnection class which encapsulates
    connection routines under linux API, like listen(), accept() and
    others; Initializes with connection mode: TCP/UDP
    \param conn Connection mode to use: TCP/IP & UDP/IP are supported
    \param strHost hostname to connect to
    \param iPort Port to connect to on remote host
*/
TConnection::TConnection( ConnectionT conn, const char *strHost, int iPort )
{
  m_SelfAddress.sin_family      = AF_INET;        // host byte order
  m_SelfAddress.sin_port        = htons( 0 );     // short, network byte order
  m_SelfAddress.sin_addr.s_addr = INADDR_ANY;     // automatically fill with my IP
  memset( &(m_SelfAddress.sin_zero), '\0', 8 );   // zero the rest of the struct

  m_SocketFD = new TDataPorter( conn );

  if( *strHost && iPort != -1 )
    openConnection( strHost, iPort );
}

/*! This is the destructor of TConnection which closes available
     connections and frees up all memory used */
TConnection::~TConnection()
{
  closeConnection();
  
  delete m_SocketFD;
};

/*! This method returns wheather connection is made or not
    \return bool wheather connection is active */
bool TConnection::connected() const
{
  return m_SocketFD->active();
}

/*! This method returns connection mode, ie. TCP/UDP
    \return bool indicating connection mode */
ConnectionT TConnection::getConnectionType() const
{
  return m_SocketFD->getConnectionType();
}

/*! This method returns local port assigned to connection
    \return int current local port being used */
int TConnection::getLocalPort( ) const
{
  return m_SelfAddress.sin_port;
}

/*! This method sets local port to use
    \param iPort port to use for connections
    \return bool wheather update was successful */
bool TConnection::setLocalPort( int iPort )
{
  if( connected() )
    return false;

  m_SelfAddress.sin_port = iPort;
  return true;
}

/*! This method returns socket descriptor
    \return socket descriptor */
int TConnection::getSocketFD() const
{
  return m_SocketFD->getSocketFD();
}

/*! This method makes a network connection to a server
    listening for connection
    \param str hostname to connect to
    \param iPort port to connect on remote machine
    \return bool wheather connection is made or not */
bool TConnection::openConnection( const char *str, int iPort )
{
  struct hostent *he;
  if( ( he = gethostbyname( str ) ) == NULL )
  {  // get the host info
    herror( "TConnection::gethostbyname" );
    return false;
  }

  m_SocketFD->reOpen();

  m_Remote.sin_family = AF_INET;    // host byte order
  m_Remote.sin_port   = htons(iPort); // short, network byte order
  m_Remote.sin_addr   = *((struct in_addr *)he->h_addr);
  memset( &(m_Remote.sin_zero), '\0', 8 ); // zero the rest of the struct
  if( getConnectionType() == CONN_TCP &&
         connect( m_SocketFD->getSocketFD(), (struct sockaddr *)&m_Remote, sizeof(struct sockaddr)) == -1 )
  {
    perror("TConnection::openConnection");
    return false;
  }

  return true;
}

/*! This method starts listening on port specified
    for any connection to be made
    \param iPort port to start listening on 
    \return bool wheather server started listening or not */
bool TConnection::startServer( int iPort )
{
  if( connected() )
    return false;

  m_SocketFD->reOpen();
  
  m_SelfAddress.sin_port = htons( iPort );     // short, network byte order
  if( bind( m_SocketFD->getSocketFD(), (struct sockaddr *)&m_SelfAddress, sizeof(struct sockaddr) ) == -1 )
  {
    perror("TConnection::bind");
    return false;
  }

  if( getConnectionType() == CONN_TCP && listen( m_SocketFD->getSocketFD(), MAX_CONN_QUEUE ) == -1 )
  {
    perror("TConnection::listen");
    return false;
  }

  return true;
}

/*! This method waits until a connection is made and a
    client asked for a connection
    \return bool if listening was successful */
bool TConnection::waitForConnection()
{
  if( getConnectionType() != CONN_TCP )
    return false;

  int new_fd;
  sockaddr_in their_addr;
  socklen_t sin_size = sizeof(struct sockaddr_in);

  if( ( new_fd = accept( m_SocketFD->getSocketFD(), (struct sockaddr *)&their_addr, &sin_size ) ) == -1 )
  {
    perror("TConnection::accept");
    return false;
  }

  cout << "server: got connection from " << inet_ntoa(their_addr.sin_addr) << endl;
  if( !fork() )
  {                                                                // this is the child process
    m_SocketFD->closeAll();                                        // child doesn’t need the listener
    TDataPorter new_socket( CONN_TCP, new_fd );
    new_socket.sendMessage( "Hello, world!\n", 14 );
    new_socket.closeAll();

    return false;//exit(0);
  }

  close(new_fd); // parent doesn’t need this
  return true;
}

/*! This method closes connections on client side,
    or stops listening with servers
    \return bool wheather any connection was closed */
bool TConnection::closeConnection()
{
  if(!connected())
    return false;

  m_SocketFD->closeAll();
  return true;
}

/*! This method waits for data to be recieved to the socket
    \param time_out time in seconds for socket to determine
        packets are availabe
    \return bool indicating socket status */
bool TConnection::waitForData( int time_out ) const
{
	time_out = 1000;
  fd_set fds;

  return ( m_SocketFD->waitForData( fds, time_out ) >= 0 );
}

/*! This method sends a message via a network socket connection made
    \param strMsg message to send via network connection
    \return bool wheather message was sent */
bool TConnection::sendMessage( const char * strMsg ) const
{
  return ( sendData( (void*)strMsg, strlen(strMsg) ) );
}

/*! This method sends a message via a network socket connection made
    \param data message to send via network connection
    \param iLength Length of the message
    \return bool wheather message was sent */
bool TConnection::sendData( const void * data, int iLength ) const
{
  if( !connected() )
    return false;

  if( m_SocketFD->getConnectionType() == CONN_TCP )
    return ( m_SocketFD->sendMessage( data, iLength ) > 0 ? true : false );
  else
    return ( m_SocketFD->sendMessageUDP( data, iLength, &m_Remote ) > 0 ? true : false );
}

/*! This method writes a message to its connected peer
    \param msg Message to be written for peer
    \return bool indicating wheather message was written or not */
bool TConnection::writeMessage( const char * msg ) const
{
  return writeMessage( (void*)msg, strlen(msg) );
}

/*! This method writes a message to its connected peer
    \param msg Message to be written for peer
    \param iLength Message length
    \return bool indicating wheather message was written or not */
bool TConnection::writeMessage( const void * msg, int iLength ) const
{
  return ( m_SocketFD->writeMessage( msg, iLength ) );
}

/*! This method checks for data to recieve on the connection and
    returns recieved data
    \param strMsg place for recieved data to be written on
    \param iMaxLen maximum length of place to store data on
    \param time_out maximum t ime to wait for socket to
    \param close_empty Close the connection if no data recieved
    \return bool wheather any data was recieved */
bool TConnection::recvMessage( char * strMsg, int iMaxLen, int time_out, bool close_empty )
{
	time_out = 1000;
  if( !connected() )
    return false;

  int iBytesRecieved = m_SocketFD->recvMessage( strMsg, iMaxLen - 1, time_out );
  if( iBytesRecieved == -1 )
    return false;
  else if( iBytesRecieved == 0 )
  {
    if( close_empty )
    {
      cerr << "Nothing recieved, probably a dead connection, disconnecting" << endl;
      closeConnection();
    }
    return false;
  }
  else
    return true;
}

/*! This method reads a message from the connection. When there is
    no message available, it does not wait for one.
    \param msg string in which message is stored
    \param iMaxLength Maximum length of the message
    \return bool indicating any message were read or not */
bool TConnection::readMessage( void * msg, int iMaxLength ) const
{
  return ( m_SocketFD->readMessage( msg, iMaxLength ) > 0 );
}




/********************************************************************/
/*******************  Class TRoboCupConnection  *********************/
/********************************************************************/

/*! TRoboCupConnection constructor to create & initialize connection
    to simulation server */
TRoboCupConnection::TRoboCupConnection( const char * strHost, int iPort ) : TConnection( CONN_TCP, strHost, iPort )
{
}

/*! TRoboCupConnection destructor, only closes connections and frees
    up memory */
TRoboCupConnection::~TRoboCupConnection()
{
  closeConnection();
}

/*! This method is the only part needed for sending commands to
    simulation server, when a connection is ready
    \param msg message to send to the server
    \return bool wheather command was sent or not */

bool TRoboCupConnection::sendCommand( const char * msg ) const
{
 //static ofstream fout ("/root/Desktop/Connection_Test_Log.txt");
  if( ! connected() )
    return false;
  // prefix the message with it's payload length
 
  unsigned int len = htonl( strlen(msg) ) ;
  string prefix( (const char*)&len, sizeof(unsigned int) ); 
  string str = prefix + msg;
//  fout << "length: "<<len <<"\n"<< str <<endl ;
//cout << str <<endl ;
//	fout << "\n ************-=-=********** \n" ;
 // CommLogger << "Client -> Serever: \"" << str << "\"" << endl;
  return ( write( getSocketFD(), str.data(), str.size() ) >= 0 );
}

/*! This method is the only routine needed to get the simulation
    server messages
    \param msg place to write message recieved to
    \param iMaxLen maximum buffer capacity
    \return bool indicating wheather any message is recieved */
bool TRoboCupConnection::getMessage( char * msg, int iMaxLen ) const
{
//cout <<"nnnnnnnnooooooooooooooooooooooooooooooonnnnnnnnnnn" <<endl;
  static char buffer[16 * 1024];
  unsigned int bytesRead = read( m_SocketFD->getSocketFD(), buffer, sizeof(buffer));
    
  if( bytesRead < sizeof(unsigned int) )
    return false;

  // msg is prefixed with it's total length
  unsigned int msgLen = ntohl(*(unsigned int*)buffer);
  if( msgLen > (unsigned)iMaxLen )
    return false;

  // read remaining message segments
  unsigned int msgRead = bytesRead - sizeof(unsigned int);
  char *offset = buffer + bytesRead;
  //clock_t t1,t2;
  //t1 = clock();
  while( msgRead < msgLen )
  {
    fd_set fds;
    FD_ZERO( &fds );
    FD_SET( m_SocketFD->getSocketFD(), &fds );

    if( select(m_SocketFD->getSocketFD() + 1, &fds, 0, 0, 0) > 0 )
    {
      msgRead += read(m_SocketFD->getSocketFD(), offset, sizeof(buffer) - msgRead);
      offset += msgRead;
    }
	
  }
 // cout <<"sleep: " << sleep (1)<<endl;
  //t2 = clock();
  //cout << (float)((t1-t2)/CLOCKS_PER_SEC)<<endl;

  // zero terminate received data
  (*offset) = 0;
  strcpy( (char*)msg, string(buffer+sizeof(unsigned int) ).data() );
 // CommLogger << "Server -> Client: \"" << msg << "\"" << "\n";

  return true;
}
