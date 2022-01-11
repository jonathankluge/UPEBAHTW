#include<stdio.h>
#include<stdbool.h>
#include<iostream>
#include<math.h>
#include<fstream>
#include<string>
#include <cstdlib>




using namespace std;
using std::ofstream;

bool bang_bang_control(float voltage, float current, bool state)
{
	bool ret = false;

	float u_cond = 102; //Spannung am Kondensator wenn voll aufgeladen
	float i_cond = 70;  //Strommittelwert
	float delta_i = 30; //Hysterese
	bool IGBT = 0;

	if (voltage >= u_cond)
	{
		ret = false;  // IGBT aus bzw. zu start zurück

	}
	else 
	{
		if (current < i_cond - delta_i)		//Strom ist kleiner als i_min
		{  
			IGBT = 1;
			ret = IGBT;
		}

		else if (current < i_cond + delta_i && state == 1)		//Strom ist kleiner als i_max und IGBT ist an
		{      
			IGBT = 1;
			ret = IGBT;   
		}

		else if(current < i_cond + delta_i && state == 0)		//Strom ist kleiner als i_max und IGBT ist aus
		{
			IGBT = 0;
			ret = IGBT;
		}

		else													//Wenn Strom > als i_max 
		{
			IGBT = 0;
			ret = IGBT;
		}
	}
	return ret;
}

float strom(float t, bool state)		
{
	int Anstieg = 66666;			 //Anstieg in A/s
	int vorz = 0;
	float n = 0.0f;					//Startwert

	if (state == 0)
	{
		vorz = -1;
		n = 100;
	}
	else
	{
		vorz = 1;
		n = 40;
	}

	return vorz * Anstieg * t + n ;	// Strom bei Zeitpunkt t

}



int main() {

	float voltage = 0.0f;			//Spannungsstartwert
	float current = 0.0f;			//Stromstartwert
	bool state = false;				//Anfangszustand
	float c = 88;					//Kapazität eines Kondensators
	float u_target = 102.0f;		//maximale Ladespannung
	float q = 0.0f;					//Ladung
	float timer = 0.0f;				//interner Zeitgeber der zurückgesetzt wird beim Umschalten von state
	float t = 0.0f;					//gesamte Zeit
	float d_t = 100E-6;				//Zeit fuer aktuellen Stromwert
	float string[4] = { t, current, voltage, state };
	int i ;


	ofstream outfile;
	outfile.open("Outfile.txt");

	

	while (voltage < u_target) 
	{
	   	t = t + d_t;
		q = q + current * d_t; 
		voltage = q / c; 
		
		if (outfile.is_open())
		{
			outfile <<"	" << state <<"	" << current <<"	" << t <<"	"<< voltage << endl;
		}
		else
		{
			cout << "failed to create outfile" << endl;

		}
		

		if (current >= 100.0000001 || current <= 39.999999)
		{
			timer = 0.0f;					//interner Zeitgeber für Berechnung des aktuellen Stromwertes 
		}
		else
		{
			timer = timer + d_t;
		}

		current = strom(timer, state); 
		state = bang_bang_control(voltage, current, state);

		cout<<" Zustand:"<<state<<" Strom:"<<current<<" Zeit gesamt: "<<t<<" Spannung:"<<voltage<< endl;
	}
	outfile.close();
	

	return 0;

}
