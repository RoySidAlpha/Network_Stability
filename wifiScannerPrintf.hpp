/**************************************************************
* Author: Siddhartha ROy
* date: 29th June 2021
* Version: 1.0.00
* Description: This code is used to handle printf() and log file
**************************************************************/

#ifndef WIFI_SCANNER_PRINTF_HPP_
#define WIFI_SCANNER_PRINTF_HPP_

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

void __printf__(const char* message) {
	FILE *fp;
	fp=fopen("wifi_scanner_log_file.log","a+");
	fprintf(fp , "%s" , message);
	fclose(fp);
	printf("%s",message);
}

#endif /* INCLUDE_WIFI_SCANNER_PRINTF_HPP_ */
