#include <iostream>
#include <cstring>
#include <vector>
#include <ctime>
#include <sstream> // std::stringstream
#include <unistd.h> // sleep()
#include "wifiScannerPrintf.hpp"

#define __SCANNING_TIME_INTERVAL__ 15

struct wifi_profile {
	std::string SSID; //
	std::string BSSID; //
	std::string ACTIVE; //
	std::string FREQ; //
	std::string MODE; //
	std::string CHANNEL; //
	std::string RATE; //
	std::string SIGNAL; //
	std::string STRENGTH; //
	std::string SECURITY; //
};
typedef struct wifi_profile wifi_profile;

const size_t MAX_SIZE = 128;

std::string get_wifi_interface () {
	std::string cmd_to_get_interface = "iw dev | awk '$1==\"Interface\"{print $2}'";
	std::string wifi_interface = "";
	FILE *fp;
	fp = popen(cmd_to_get_interface.c_str(), "r");
	if (fp == NULL) {
		printf(RED "ERROR : " RESET "while finding the interface\n");
		exit(EXIT_FAILURE);
	}
	char buffer[MAX_SIZE];
	
	while (fgets (buffer, MAX_SIZE, fp) != NULL)
		wifi_interface.assign(buffer,strlen (buffer)-1);
	return wifi_interface;
}

std::string get_network_connectivity () {
	std::string cmd_to_get_network_connectivity = "nmcli networking connectivity check";
	std::string network_connectivity = "";
	FILE *fp;
	fp = popen(cmd_to_get_network_connectivity.c_str(), "r");
	if (fp == NULL) {
		printf(RED "ERROR : " RESET "while finding the network connectivity\n");
		exit(EXIT_FAILURE);
	}
	char buffer[MAX_SIZE];
	
	while (fgets (buffer, MAX_SIZE, fp) != NULL)
		network_connectivity.assign(buffer,strlen (buffer)-1);
	return network_connectivity;
}

void parsing (std::vector <wifi_profile> &wifi_profile_set , std::string line) {
	int8_t param_no = 0x00;
	wifi_profile individual_wifi_profile;
	std::stringstream ss(line);

	while (ss.good()) {
		std::string param_val;
		std::getline(ss, param_val, ':');
		++param_no;
		switch (param_no) {
			case 1 : individual_wifi_profile.SSID     = param_val; break;
			case 2 : 
			case 3 : 
			case 4 : 
			case 5 : 
			case 6 : param_val += ":";
			case 7 : individual_wifi_profile.BSSID     += param_val; break;
			case 8 : individual_wifi_profile.ACTIVE   = param_val; break;
			case 9 : individual_wifi_profile.FREQ     = param_val; break;
			case 10 : individual_wifi_profile.MODE     = param_val; break;
			case 11 : individual_wifi_profile.CHANNEL  = param_val; break;
			case 12 : individual_wifi_profile.RATE     = param_val; break;
			case 13 : individual_wifi_profile.SIGNAL   = param_val; break;
			case 14 : individual_wifi_profile.STRENGTH = param_val; break;
			case 15 : individual_wifi_profile.SECURITY =param_val; break;
			default : break;
		}
   	}
	wifi_profile_set.push_back(individual_wifi_profile);
}


void scan_and_get_wifi_profile_set (std::vector <wifi_profile> &wifi_profile_set) {
	std::string cmd_to_get_wifi_profiles = "nmcli device wifi rescan && nmcli -e no -t -f SSID,BSSID,ACTIVE,FREQ,MODE,CHAN,RATE,SIGNAL,BARS,SECURITY device wifi";
	FILE *fp;
	fp = popen(cmd_to_get_wifi_profiles.c_str(), "r");
	if (fp == NULL) {
		printf(RED "Unable to check wifi profile\n" RESET);
		exit(EXIT_FAILURE);
	}
	char buffer[MAX_SIZE];
	while (fgets (buffer, MAX_SIZE, fp) != NULL) {
		std::string line;
		line.assign(buffer , strlen (buffer)-1);
		parsing(wifi_profile_set, line);
	}
}



void show_wifi_profile_set (std::vector <wifi_profile> &wifi_profile_set) {
	bool is_device_connected_to_open_wifi_network = false;
	FILE *fp;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char message[300] = {0x00};
	//sprintf(timestamp, "%d.%d.%d :%d:%d:%d", tm.tm_mday , tm.tm_mon + 1 , tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
	sprintf(message,"==================================== Scanning done on : %d.%d.%d :%d:%d:%d ===========================================\n",tm.tm_mday , tm.tm_mon + 1 , tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
	std::string st = message;
	__printf__(message);
	for (int i = 0 ; i < wifi_profile_set.size () ; i++) {
		memset(message,0x00,sizeof(message));
		sprintf(message,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", \
				wifi_profile_set[i].SSID.c_str() , \
				wifi_profile_set[i].BSSID.c_str() , \
				wifi_profile_set[i].ACTIVE.c_str() , \
				wifi_profile_set[i].FREQ.c_str() , \
				wifi_profile_set[i].MODE.c_str() , \
				wifi_profile_set[i].CHANNEL.c_str() , \
				wifi_profile_set[i].RATE.c_str() , \
				wifi_profile_set[i].SIGNAL.c_str() , \
				wifi_profile_set[i].STRENGTH.c_str() , \
				wifi_profile_set[i].SECURITY.c_str());

		if (wifi_profile_set[i].SECURITY == "") {
			wifi_profile_set[i].SECURITY = "--";
			if (wifi_profile_set[i].ACTIVE == "yes") {
				is_device_connected_to_open_wifi_network = true;
				printf(RED);
			}
			else { printf(YEL); }
		}
		else if (wifi_profile_set[i].ACTIVE == "yes" and wifi_profile_set[i].SECURITY != "")
			printf(GRN);
		else { printf(RESET); }
		__printf__(message);
		printf(RESET);
	}
	if (is_device_connected_to_open_wifi_network)
		system ("canberra-gtk-play -f BOMB_SIREN-BOMB_ALARM.wav");
}


int main (int argc, char* argv[]) {
	std::string wifi_interface = get_wifi_interface();
	std::cout << "wifi_interface : " << wifi_interface << "\n";
	std::string network_connectivity = get_network_connectivity();
	std::cout << "network_connectivity : " << network_connectivity << "\n";
	std::vector <wifi_profile> wifi_profile_set;
	while (true) {
		wifi_profile_set.clear();
		scan_and_get_wifi_profile_set(wifi_profile_set);
		show_wifi_profile_set(wifi_profile_set);
		sleep(__SCANNING_TIME_INTERVAL__);
	}

	return EXIT_SUCCESS;
}

