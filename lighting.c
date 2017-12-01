#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lighting.h"

static int readFromFile(char *fileName);
static void writeToFile(char *fileName, int gpioNum);


void turn_on_Lighting(void){
	//echo 49 > /sys/class/gpio/export
	writeToFile(GPIO_EXPORT_PATH,GPIO_NUMBER);
	writeToFile(GPIO_VALUE_PATH,0);
}

void turn_off_Lighting(void){
	//echo 49 > /sys/class/gpio/export
	writeToFile(GPIO_EXPORT_PATH,GPIO_NUMBER);
	writeToFile(GPIO_VALUE_PATH,1);
}

int get_Lighting_state(void){
	return readFromFile(GPIO_VALUE_PATH);
}

static int readFromFile(char *fileName){
	FILE *pfile = fopen(fileName, "r");
	int boo;
	if (pfile == NULL){
		printf("ERROR: Unable to open file (%s) for read\n", fileName);
		exit(-1);
	}
	
	//read integer
	fscanf(pfile, "%d", &boo);  

	//Close the file 
	fclose(pfile);
	return boo;
}


static void writeToFile(char *fileName, int value){
	FILE *pfile = fopen(fileName, "w");
	if (pfile == NULL) {
		printf("ERROR: Unable to open export file.\n");
		exit(1);
	}

	// Write to data to the file using fprintf():
	fprintf(pfile, "%d", value);

	// Close the file using fclose():
	fclose(pfile);
}

