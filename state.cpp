#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
using namespace std;

typedef enum mode
{
	idle,busy
}mode;

class phone
{
	double phno;
	mode status;
	double oncall;
	double destin;
	public:
	phone()
	{
		phno = 0;
		status = idle;
		oncall = 0;
	}
	void set_phno(int num);
	friend int make_call(phone *ph);
	friend phone* check_number(phone *P,double num);
	friend int check_status(phone* p);
	void set_status(phone *D,mode smode,mode dmode);
	friend int disconnect_call(phone *PH);
	int disconnect(phone *PH);
};

int make_call(phone *);
phone* check_number(phone *,double);
int check_status(phone *);
void ringing();
int call_wait();

void phone :: set_phno(int num)
{
	phno += num;
}

void phone :: set_status(phone *D,mode smode,mode dmode)
{
	status = smode;
	D->status = dmode;
}

int phone :: disconnect(phone *PH)
{
	status = idle;
	phone *d = check_number(PH,destin);
	d->status = idle;
	destin = 0;
	d->destin = 0;
	cout<<"\n\t\t\t\t\t\t### Call has been Disconnected Successfully! ###\n";
	return 1;
}

int main(void)
{
	system("clear");
	phone PH[10];
	int ch,i,j;
	char choice;
	int source;
	cout<<"\n\t\t\t\t\t\t... Initialising Machine! ...\n";
	for(i=0;i<10;i++)
		PH[i].set_phno(100+i);
	cout<<"\t\t\t\t\t\t... Machine Initialized! ...\n";
	cout<<"\t\t\t\t\t\t*** CALLING NUMBERS AVAILABLE : 100 TO 109 ***\n";
	do{
		cout<<"\n\t\t\t\t\t\tOptions Available : \n";
		cout<<"\t\t\t\t\t\t1. Make a call\n";
		cout<<"\t\t\t\t\t\t2. Disconnect the call\n";
		cout<<"Enter Choice : ";
		cin>>ch;
		switch(ch)
		{
			case 1:
				make_call(PH);break;
			case 2:
				disconnect_call(PH);break;
			default : cout<<"\t\t\t\tInvalid Entry !\n";
		}
		cout<<"\nDo you want to Continue [Y/N] : ";
		cin>>choice;
	}while('Y' == choice || 'y' == choice);
	return 0;
}

int make_call(phone *PH)
{
	char ch;
	double source,destin;
	int i;
	phone *S,*D;
	cout<<"\nEnter Source Number : ";
	cin>>source;
	S = check_number(PH,source);
	if(NULL == S)
		return -1;
	if(0 == check_status(S))
	{
		cout<<"\n\t\t\t\t\t\t### The Number you are trying to reach is currently Busy on another Call! ###\n";
		cout<<"Do you want to disconnect it [Y/N] : ";
		cin>>ch;
		if('Y' == ch || 'y' == ch)
			S->disconnect(PH);
		else
			return -1;
	}ch = 0;
	cout<<"Enter Destination Number : ";
	cin>>destin;
	D = check_number(PH,destin);
	if(NULL == D)
		return -1;
	if(0 == check_status(D))
	{
		cout<<"\n\t\t\t\t\t\t### The Number you are trying to reach is currently Busy on another Call! ###\n";
		return -1;
	}
	cout<<"\t\t\t\t\t\t *** Call is in Progress ... ***\n";
//	cout<<"\n\nAnswer Call [Y/N] : ";
//	cin>>ch;
//	if('Y' == ch || 'y' == ch)
	if(1 == call_wait())
	{
		S->destin = destin;
		D->destin = source;
		S->set_status(D,busy,busy);
		cout<<"\t\t\t\t\t\t*** Call has been Established! ... ***\n";
	}
	else
		cout<<"\t\t\t\t\t\t### Call has been Declined! ###\n";
	sleep(3);
	fflush(stdout);
	system("clear");
}

phone* check_number(phone *PH,double num)
{
	int i;
	for(i=0;i<10;i++)
		if(num == PH[i].phno)
			return &(PH[i]);
	if(10<=i){
		cout<<"\n\t\t\t\t\t\t### The Number you have dialed does Not Exist! ###\n";
		return NULL;
	}
}

int check_status(phone *p)
{
	if(busy == p->status)
		return 0;
	else
		return 1;
}

int disconnect_call(phone *PH)
{
	double num;
	cout<<"\nEnter Number of which you Want to Disconnect Call : ";
	cin>>num;
	phone *S = check_number(PH,num);
	if(NULL == S)
		return -1;
	if(1 == check_status(S))
	{
		cout<<"\n\t\t\t\t\t\t### Cannot Disconnect Call ###\n\t\t\t\t\t\t### Phone already is in IDLE State ! ###\n";
		return -1;
	}
	S->disconnect(PH);
}

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
		printf("%c",c);
            cout<<c<<endl;
            read( fileno( stdin ), &c, 1 );
		if('Y' == c || 'y' == c)
			return 1;
		else
			return 0;
        }
        else if( res < 0 )
        {	
            cerr<<"select error";
            return 0;
        }
        else
        {
            cout<<"Timeout...\n";
		return 0;
		
        }
    }
	fflush(stdout);

    tcsetattr( fileno( stdin ), TCSANOW, &oldSettings );
}

void ringing()
{
//	system("clear");
	cout<<"\n\t\t\t\t\t\t\tRinging . ";
	for(int j=0;j<10;j++)
		cout<<". ";
	cout<<"\n\t\t\t\t\t\t\tAnswer Call [Y/N] : ";
	fflush(stdout);
}

