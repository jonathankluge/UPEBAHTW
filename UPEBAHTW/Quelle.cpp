#include <stdio.h>
#include <stdbool.h>
#include<iostream>

using namespace std;

bool bang_bang_control(float voltage, float current, bool state)
{
	bool ret = false;

	float u_cond = 102; //Spannung am Kondensator wenn voll aufgeladen
	float i_cond = 70;  //Strommittelwert
	float delta_i = 30; //Hysterese
	bool IGBT = 0;

	if (voltage >= u_cond) {
		ret = false;  // IGBT aus bzw. zu start zurück

	}
	else {
		if (current < i_cond - delta_i) {  //Strom ist größer als i_max

			IGBT = 1;
			ret = IGBT;
		}
		else if (current < i_cond + delta_i && state == 1) {     //Strom ist kleiner als i_min

			IGBT = 1;
			ret = IGBT;   //IGBT an 
		}

		else {
			IGBT = 0;
			ret = IGBT;
		}
	}
	return ret;
}

float strom(int on_off, float t)		//on_off als Vorzeichen für anstieg bzw. Abfall des Stromes
{
	float current = 0.0f;
	int Anstieg = 12; //Anstieg in A/s

	return on_off* Anstieg * t;

}



int main() {

	float voltage = 12.0f;
	float current = 0.0f;
	bool state = false;
	float c = 50;
	float u_target = 102.0f;
	float q = 0.0f;
	float i = 0.0f;

	
	

	while (voltage < u_target) 
	{

		for (i = 0; i < 100; i++)
		{
			state = bang_bang_control(current, voltage, state);
			current = strom(-1, i); 
		    // runs every 100µs
			//state = bang_bang_control(voltage, current, state);
			cout<<"Zustand"<< state;
		}
			


	}
	return 0;

}
