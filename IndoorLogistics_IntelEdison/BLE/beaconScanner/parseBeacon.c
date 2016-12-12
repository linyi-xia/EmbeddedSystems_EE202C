//get_BeaconID.c
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
               // printf("%s\n", path);
        }
        pclose(fp);

        strncpy (thestring, path, 9);
	return ;
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
	return;
}
int main()
{
	char* BeaconInfo = (char*)malloc(16);
	shelltoC(BeaconInfo);
	printf("beaconinfo:%s", BeaconInfo);
	char* beaconID = (char*)malloc(4);
	parseID (BeaconInfo, beaconID);
	printf("beaconID:%s", beaconID);
	char* RSSI = (char*)malloc(2);
	parseRSSI(BeaconInfo, RSSI);
	printf("RSSI:%s", RSSI);
	return 0;
}


