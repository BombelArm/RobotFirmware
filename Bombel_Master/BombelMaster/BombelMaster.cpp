// BombelMaster.cpp : Defines the entry point for the console application.
//


#include <iostream>

#include <stdio.h>
#include "Manipulator.h"


using namespace std;
string port = "\\\\.\\COM4";
Arm_type Bombel(port);
int main()
{
	Bombel.init_arm();
	int pos = 0;
	bool counter = 0;
	Bombel.begin_movement(0,5);
	while (1)
	{
		Bombel.update();
		Sleep(50);
		
		if (Bombel.is_movment_fin())
		{
			//std::cout<<"boom\n";
				pos++;
				Bombel.begin_movement(0, 3*sin(pos));
					
		}
		Sleep(50);
	}
	
	return 0;
}

