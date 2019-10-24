#include "WorldModel.h"
#include "Player.h"
#include "Connection.h"
#include <time.h>

#include <fstream>
#include <sstream>
#define MAX_TEAM_NAME_LENGTH	50
// ******************** Global Values ****************************

TRoboCupConnection * C;
WorldModel	WM;
Player		player;
char     strTeamName[MAX_TEAM_NAME_LENGTH] = "Scorpius";
int      iPort                             = 3100;
char     strHost[128]                      = "127.0.0.1";
//char     strFormations[128]                = "formations.conf";
int      iNum                               = 1;
char     strRsg[128]		     = 	"(scene rsg/agent/nao/nao.rsg)";
char		*str;
string   strRsgPrefix  = "(scene rsg/agent/" ;
string   strRsgPostfix = ".rsg)";
// ******************* Local main functions ******************** 
void PrintGreeting ( )
{
	cout<<"\n";
	cout<<"\n";
	cout<<"   ####   ####   ##   ####   ####  ## ##  ##  ####   \n";
	cout<<"  ##     ##    ##  ## ## ##  ## ## ## ##  ## ##      \n";
 	cout<<"   ####  ##    ##  ## ####   ####  ## ##  ##  ####   \n";
	cout<<"      ## ##    ##  ## ## ##  ##    ## ##  ##     ##  \n";
	cout<<"   ####   ####   ##   ##  ## ##    ##  ####   ####   \n";
	cout<<"  __________________H U M A N O I D_________________\n";
	cout<<" Copyright (C) 2008 Scorpius Soccer Simulation Team\n";
	cout<<"\n";
	cout<<"\n";
}

int parseFirstInt( char** strMsg )
{
  int  iRes  = 0;
  bool bIsMin= false;
  char *str  = *strMsg;

  while( *str != '\0' && !isdigit(*str) && *str!='-')
    str++;                             // walk to first non digit or minus sign


  if( *str != '\0' && *str=='-')       // if it was a minus sign, remember
  {
    bIsMin=true;
    str++;
  }

  while( *str != '\0' && *str<='9' && *str>='0' )   // for all digits
  {
      iRes=iRes*10+(int)(*str-'0');                 // multiply old res with 10
      str++;                                        // and add new value
  }
  *strMsg = str;
  return (bIsMin) ? -iRes : iRes;
}
void printOptions( )
{
  cout << "\n\nScorpius Base Command options:\n\n"                                         << endl <<

   " r(sg) file            - choosing proper rsg (e.g. soccerbot056 )"  << endl <<
     //" f(ormations) file     - file with formation info"               << endl <<
   " he(lp)                - print this information"                 << endl <<
   " h(ost) hostname       - host to connect with"                   << endl <<
   " t(eam Name)           - team Name"				     << endl <<			
   " n(umber) int          - player number in formation"             << endl <<
   " p(ort)                - port number to connect with\n\n"            << endl;
   
}
void ReadOptions ( int argc , char* argv[] )
{
	string tmp;
	for( int i = 1 ; i < argc ; i = i + 2  )
  {
    // help is only option that does not have to have an argument
    if( i + 1 >= argc && strncmp( argv[i], "-help", 3 ) != 0 )
    {
      cout << "Need argument for option: " << argv[i] << endl;
      exit( 0 );
    }
    // read a command option
    if( argv[i][0] == '-' && strlen( argv[i] ) > 1)
    {
      switch( argv[i][1] )
      {
        case '?':                                   // print help
          printOptions( );
          exit(0);
          break;
        case 'r':                                   // rsg file 
          	strcpy(strRsg , argv[i+1]);
		tmp = strRsgPrefix+string(strRsg)+strRsgPostfix ;
		strcpy(strRsg , tmp.c_str());
          break;
        
        //case 'f':                                   // formations file
         // strcpy( strFormations, argv[i+1] );
          //break;
		
        case 'h':                                   // host server or help
          if( strlen( argv [i]) > 2 && argv[i][2] == 'e' )
          {
            printOptions( );
            exit(0);
          }
          else
            strcpy( strHost, argv[i+1] );
          break;
        
        case 'n':                                   // number in formation int
          str = &argv[i+1][0];
          iNum = parseFirstInt( &str );
          break;
        
        case 'p':                                   // port
          str = &argv[i+1][0];
          iPort = parseFirstInt( &str );
          break;
        
        case 't':                                   // teamname name
          strcpy( strTeamName, argv[i+1] );
          break;
        default:
          cerr << "(main) Unknown command option: " << argv[i] << endl;
      }
    }
  }
}

void Run ( )
{
	bool mInit = false ;
	C->sendCommand ( strRsg );	// Initializing the Robot
	cout << "Connected to the server Successfully" << endl ;
	char msg[1024*16];
	WM.setPlayerNumber ( iNum );
	player.setWorldModel ( &WM );
	clock_t t1,t2;
	while ( C->getMessage(msg,1024*16) )
	{
		if (!mInit)
    		{
        		mInit = true;
				stringstream ss;
				ss << "(init (unum " << WM.getPlayerNumber();
				ss<<")(teamname "<<strTeamName<<"))";
				C->sendCommand ( ss.str().c_str() );
    				WM.setTeamName(strTeamName);
		}

		else if ( WM.update (string(msg)) )
		{	
			t1 = clock();
			player.resetComponents();
			player.think();
			C->sendCommand ( player.getCommand().c_str() );
			//for (long int k=0 ; k<1000000; k++)	
			t2 = clock();
			
			double t3 = (double)((t2-t1)/CLOCKS_PER_SEC);
			//cout <<"Loop time: "<<t3<<endl;
		}else
			{
				cout << "waiting for new message...\n" ;
				//if (sleep()) cout <<"sleep true\n" ;
			}
		
	}
}

int main (int argc , char* argv[] )
{
	ReadOptions ( argc , argv );
//	PrintGreeting ();
	C = new TRoboCupConnection( strHost, iPort );
	Run ( );
	return 0;
}
