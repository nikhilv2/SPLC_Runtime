#include "iec_std_lib.h"
#include "dataStructure.h"

void updateTime()
{
	__CURRENT_TIME.tv_nsec += common_ticktime__;

	if (__CURRENT_TIME.tv_nsec >= 1000000000)
	{
		__CURRENT_TIME.tv_nsec -= 1000000000;
		__CURRENT_TIME.tv_sec += 1;
	}
}

int8_t parseHeaderFile()
{
	FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("LOCATED_VARIABLES.h", "r");
    if (fp == NULL){
        return -1;
	}

    while ((read = getline(&line, &len, fp)) != -1) {
		if (read > 0){
			char type[256];
			char name[256];
			char sectionType;
			char dataType;
			int pos1;
			int pos2;
			sscanf(line, "%s(%s,%s,%s,%s,%d,%d)", type, name, sectionType, dataType, pos1, pos2);
		}

    }

    fclose(fp);
    if (line){
        free(line);
	}

    return 0;
}


void initDS()
{
	/* Parse through the LOCATED_VARIABLES.h header file */
	boolInputs[0][0] = __IX0_0;
	boolInputs[0][1] = __IX0_1;
	boolInputs[0][0] = __QX0_0;
}