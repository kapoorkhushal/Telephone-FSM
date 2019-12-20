#include<iostream>
#include<unistd.h>
using namespace std;

int main()
{
	for(int i=0;i<10;i++)
	{
		cout<<"Ringing . ";
		for(int j=0;j<i;j++)
			cout<<". ";
		sleep(1);
		system("clear");
		fflush(stdout);

	}return 0;
}


