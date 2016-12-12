//testgetnearbeacon.h
#ifndef TESTGETNEARBEACON
#define TESTGETNEARBEACON

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void shelltoC(char* thestring)
{
        FILE *fp;
        char path[1035];
        //open command for reading
        fp = popen ("./get_location", "r");
        if (fp == NULL){
                printf("failed to run command\n");
                exit(1);
        }
        //read ouput a line at a time -output it
        while (fgets(path, sizeof(path)-1, fp)!=NULL) {
                //printf("%s\n", path);
        }
        pclose(fp);

        strncpy (thestring, path, 9);
        strcpy (&thestring[9], "\0");
        return;
}
void parseID (char* beaconinfo, char* returnID)
{
	if (beaconinfo == NULL)
	{
		printf("no beaconinfo");
		return;
	}
	char* current = beaconinfo;
	strncpy (returnID, current, 2);
	strncpy (&returnID[2], current+3, 2);
	strcpy (&returnID[4], "\0");
	return;
}
void parseRSSI (char* beaconinfo, char* returnRSSI)
{
	if (beaconinfo == NULL)
	{
		printf("no beaconinfo");
		return;
	}
	char* current = beaconinfo;
	strncpy (returnRSSI, current+7, 2);
	strcpy (&returnRSSI[2], "\0");
	return;
}
int verifyBeacon (char* detectedBLE, char*routeBLE)
{
	if (!strcmp(detectedBLE, routeBLE))	//equal
		return 1;	//meaning verified true
	else
		return 0;
}

void getStrongestBeacon(char* BeaconID)
{
	char* BeaconInfo = (char*)malloc(16);
	char* beaconID = (char*)malloc(5);
	char* RSSI = (char*)malloc(3);
	char* strongbeaconID = (char*)malloc(5);
	char* strongRSSI = (char*)malloc(3);

	shelltoC(BeaconInfo);
	parseID (BeaconInfo, beaconID);
	parseRSSI(BeaconInfo, strongRSSI);
	int i;
//	FILE* fp;
	for (i=0; i<5; i++)
	{

//		 fp = popen ("./resetBlueTooth", "r");
	//        if (fp == NULL){
	  //              printf("failed to run command reset\n");
	    //            exit(1);
	      //  }
		shelltoC(BeaconInfo);
		parseID (BeaconInfo, beaconID);
		parseRSSI(BeaconInfo, RSSI);
		printf("beaconinfo:%s\n", BeaconInfo);
		printf("beaconID:%s\n", beaconID);
		printf("RSSI:%s\n", RSSI);

		if (strcmp(RSSI, strongRSSI)<0)
		{
			strcpy(strongRSSI, RSSI);
			strcpy(strongbeaconID, beaconID);
		}
		printf("strongest RSSI:%s, strong beacon:%s\n", strongRSSI, strongbeaconID);
	}
	strcpy (BeaconID, strongbeaconID);
	//printf ("final BeaconID:%s", BeaconID);
}

#endif

