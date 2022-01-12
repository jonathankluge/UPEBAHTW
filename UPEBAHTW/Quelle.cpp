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
	float i_cond = 70;  //Strommittelwert-->Sollte durch Nutzer einstellbar sein
	float delta_i = 30; //Hysterese -->Sollte ebenfalls von Nutzer einstellbar sein
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
	int Anstieg = 66666;			 //66666Anstieg in A/s
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

	float voltage = 0.0f;			//Spannungwer-->Wird über Spannungsmessung übergeben
	float current = 0.0f;			//Stromstartwer-->Wird über Stromsmessung übergeben
	bool state = false;				//Anfangszustand--> Übergabe sollte hier ebenfalls durchgeführt werden
	float c = 88;					//Kapazität eines Kondensators
	float u_target = 0.1f;		    //maximale Ladespannung, für testzewcke auf 0.1V gestetzt--> in realer Anwendung auf 102V setzen
	float q = 0.0f;					//Ladung
	float timer = 0.0f;				//interner Zeitgeber der zurückgesetzt wird beim Umschalten von state
	float t = 0.0f;					//gesamte Zeit
	float d_t = 100E-5;				//Zeit fuer aktuellen Stromwert
	float string[4] = { t, current, voltage, state };



	ofstream outfile;
	outfile.open("Outfile.csv");

	if (current > 500 || voltage<0 ) { //Plausibilitätsprüfung
		
		state = 0;
		cout << "caution Current to high! " << endl;
		cout << " Check Polarity of voltage measurement" << endl;


	}
	else
	{

	while (voltage < u_target) 
	{
	   	t = t + d_t;
		q = q + current * d_t; 
		voltage = q / c; 
		
		if (outfile.is_open())				// Abfrage, falss csv Datei nicht geöffnet werden kann
		{
			outfile << state <<";" << current <<";" << t <<";"<< voltage << endl;
		}
		else
		{
			cout << "failed to create outfile" << endl;

		}
		

		if (current >= 100.0000001 || current <= 39.999999) 
		{
			timer = 0.0f;					//interner Zeitgeber für Berechnung des aktuellen Stromwertes, wird zurückgesetzt
											// wenn Stromfunktion ihren minimal bzw. Maximalwert erreicht
		}
		else
		{
			timer = timer + d_t;				
		}

		current = strom(timer, state);		// berechnung des Stromwertes anhand der aktuellen zeit und des IGBT zustandes
		state = bang_bang_control(voltage, current, state); //Zustandskontrolle anhand der Aktuellen Werte, des Stromes, der Spannung 
															//und des vorherigen Zustandes
		cout<<" Zeit gesamt:"<<t<< "Zustand:"<< state << " Strom:" << current  << " Spannung:" << voltage << endl; //Ausgabe von Strom, Spannung, Zustand, und Zeit
	}
	outfile.close();
	
	std::string filename = "/Users/Jonathan/source/repos/UPEBA/UPEBAHTW/Graph_Plot.py"; //Aufruf der Python Datei
	std::string command = "python";
	command += filename;
	system(command.c_str());

	}	
	return 0;

}
