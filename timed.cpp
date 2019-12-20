#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
using namespace std;

void ringing();
int call_wait();

int call_wait()
{
    struct termios oldSettings, newSettings;

    tcgetattr( fileno( stdin ), &oldSettings );
    newSettings = oldSettings;
    newSettings.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr( fileno( stdin ), TCSANOW, &newSettings );    

    while ( 1 )
    {
        fd_set set;
        struct timeval tv;

        tv.tv_sec = 10;
        tv.tv_usec = 0;
	ringing();
        FD_ZERO( &set );
        FD_SET( fileno( stdin ), &set );
        int res = select( fileno( stdin )+1, &set, NULL, NULL, &tv );

        if( res > 0 )
        {
            char c;
            cout<<c<<endl;
            read( fileno( stdin ), &c, 1 );
		if('Y' == c || 'y' == c)
			return 1;
		else
			return 0;
        }
        else if( res < 0 )
        {	
            perror( "select error" );
            break;
        }
        else
        {
            cout<<"Timeout...\n";
		return 0;
		
        }
    }

    tcsetattr( fileno( stdin ), TCSANOW, &oldSettings );
}

void ringing()
{
	cout<<"\n\t\t\t\t\t\t\tRinging . ";
	for(int j=0;j<10;j++)
		cout<<". ";
	cout<<"\n\t\t\t\t\t\t\tAnswer Call [Y/N] : ";
	system("clear");
	fflush(stdout);
}

int main(void){
	cout<<call_wait();
	return 0;
}
