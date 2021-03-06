#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

/// Write the header to the output stream. The header is common among all glueVars files.
/// @param glueVars The output stream to write to.
void generateHeader(ostream& glueVars)
{
	glueVars << 	"\
\r\n\
#include \"iec_std_lib.h\"\r\n\
\r\n\
TIME __CURRENT_TIME;\r\n\
extern unsigned long long common_ticktime__;\r\n\
\r\n\
//Internal buffers for I/O and memory. These buffers are defined in the\r\n\
//auto-generated glueVars.cpp file\r\n\
#define BUFFER_SIZE		1024\r\n\
\r\n\
//Booleans\r\n\
IEC_BOOL *bool_input[BUFFER_SIZE][8];\r\n\
IEC_BOOL *bool_output[BUFFER_SIZE][8];\r\n\
\r\n\
//Bytes\r\n\
IEC_BYTE *byte_input[BUFFER_SIZE];\r\n\
IEC_BYTE *byte_output[BUFFER_SIZE];\r\n\
\r\n\
//Analog I/O\r\n\
IEC_UINT *int_input[BUFFER_SIZE];\r\n\
IEC_UINT *int_output[BUFFER_SIZE];\r\n\
\r\n\
//Memory\r\n\
IEC_UINT *int_memory[BUFFER_SIZE];\r\n\
IEC_DINT *dint_memory[BUFFER_SIZE];\r\n\
IEC_LINT *lint_memory[BUFFER_SIZE];\r\n\
\r\n\
//Special Functions\r\n\
IEC_LINT *special_functions[BUFFER_SIZE];\r\n\
\r\n\
\r\n\
#define __LOCATED_VAR(type, name, ...) type __##name;\r\n\
#include \"LOCATED_VARIABLES.h\"\r\n\
#undef __LOCATED_VAR\r\n\
#define __LOCATED_VAR(type, name, ...) type* name = &__##name;\r\n\
#include \"LOCATED_VARIABLES.h\"\r\n\
#undef __LOCATED_VAR\r\n\
\r\n\
void glueVars()\r\n\
{\r\n";
}

int parseIecVars(istream& locatedVars, char *varName, char *varType)
{
	string line;
	char buffer[1024];

	if (getline(locatedVars, line))
	{
		int i = 0, j = 0;
		strncpy(buffer, line.c_str(), 1024);
		for (i = 0; buffer[i] != '('; i++);
		i++;

		while (buffer[i] != ',')
		{
			varType[j] = buffer[i];
			i++; j++;
			varType[j] = '\0';
		}
		i++; j=0;

		while (buffer[i] != ',')
		{
			varName[j] = buffer[i];
			i++; j++;
			varName[j] = '\0';
		}

		return 1;
	}
	else
	{
		return 0;
	}
}

void findPositions(char *varName, int *pos1, int *pos2)
{
	int i=4, j=0;
	char tempBuffer[100];

	while (varName[i] != '_' && varName[i] != '\0')
	{
		tempBuffer[j] = varName[i];
		i++; j++;
		tempBuffer[j] = '\0';
	}
	*pos1 = atoi(tempBuffer);

	if (varName[i] == '\0')
	{
		*pos2 = 0;
		return;
	}

	j = 0; i++;

	while (varName[i] != '\0')
	{
		tempBuffer[j] = varName[i];
		i++; j++;
		tempBuffer[j] = '\0';
	}
	*pos2 = atoi(tempBuffer);
}

void glueVar(ostream& glueVars, char *varName, char *varType)
{
	cout << "varName: " << varName << "\tvarType: " << varType << endl;
	int pos1, pos2;

	findPositions(varName, &pos1, &pos2);

	if (pos2 >= 8)
	{
		cout << "***Invalid addressing on located variable" << varName << "***" << endl;
	}

	if (varName[2] == 'I')
	{
		//INPUT
		switch (varName[3])
		{
			case 'X':
				glueVars << "\tbool_input[" << pos1 << "][" << pos2 << "] = " << varName << ";\r\n";
				break;
			case 'B':
				glueVars << "\tbyte_input[" << pos1 << "] = " << varName << ";\r\n";
				break;
			case 'W':
				glueVars << "\tint_input[" << pos1 << "] = " << varName << ";\r\n";
				break;
		}
	}
	else if (varName[2] == 'Q')
	{
		//OUTPUT
		switch (varName[3])
		{
			case 'X':
				glueVars << "\tbool_output[" << pos1 << "][" << pos2 << "] = " << varName << ";\r\n";
				break;
           	case 'B':
				glueVars << "\tbyte_output[" << pos1 << "] = " << varName << ";\r\n";
				break;
			case 'W':
				glueVars << "\tint_output[" << pos1 << "] = " << varName << ";\r\n";
				break;
		}
	}
	else if (varName[2] == 'M')
	{
		//MEMORY
		switch (varName[3])
		{
			case 'W':
				glueVars << "\tint_memory[" << pos1 << "] = " << varName << ";\r\n";
				break;
			case 'D':
				glueVars << "\tdint_memory[" << pos1 << "] = (IEC_DINT *)" << varName << ";\r\n";
				break;
			case 'L':
				if (pos1 > 1023)
					glueVars << "\tspecial_functions[" << (pos1-1024) << "] = (IEC_LINT *)" << varName << ";\r\n";
				else
					glueVars << "\tlint_memory[" << pos1 << "] = (IEC_LINT *)" << varName << ";\r\n";
				break;
		}
	}
}

void generateBottom(ostream& glueVars)
{
	glueVars << "}\r\n\
\r\n\
void updateTime()\r\n\
{\r\n\
	__CURRENT_TIME.tv_nsec += common_ticktime__;\r\n\
\r\n\
	if (__CURRENT_TIME.tv_nsec >= 1000000000)\r\n\
	{\r\n\
		__CURRENT_TIME.tv_nsec -= 1000000000;\r\n\
		__CURRENT_TIME.tv_sec += 1;\r\n\
	}\r\n\
}";
}

void generateBody(istream& locatedVars, ostream& glueVars) {
    // Start the generation process.
    char iecVar_name[100];
    char iecVar_type[100];

    while (parseIecVars(locatedVars, iecVar_name, iecVar_type))
    {
        glueVar(glueVars, iecVar_name, iecVar_type);
    }
}

/// This is our main function. We define it with a different name and then
/// call it from the main function so that we can mock it for the purpose
/// of testing.
int mainImpl(int argc, char *argv[])
{
	// Parse the command line arguments - if they exist. Show the help if there are too many arguments
    // or if the first argument is for help.
    bool show_help = argc >= 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0);
    if (show_help || (argc != 1 && argc != 3)) {
		cout << "Usage " << endl << endl;
		cout << "  glue_generator [options] <path-to-located-variables.h> <path-to-glue-vars.cpp>" << endl << endl;
		cout << "Reads the LOCATED_VARIABLES.h file generated by the MATIEC compiler and produces" << endl;
		cout << "glueVars.cpp for the OpenPLC runtime. If not specified, paths are relative to" << endl;
		cout << "the current directory." << endl << endl;
		cout << "Options" << endl;
		cout << "  --help,-h   = Print usage information and exit." << endl;
		return 0;
	}

	// If we have 3 arguments, then the user provided input and output paths
	string input_file_name("LOCATED_VARIABLES.h");
	string output_file_name("glueVars.cpp");
	if (argc == 3) {
		input_file_name = argv[1];
		output_file_name = argv[2];
	}

	// Try to open the files for reading and writing.
	ifstream locatedVars(input_file_name, ios::in);
	if (!locatedVars.is_open()) {
        cout << "Error opening located variables file at " << input_file_name << endl;
		return 1;
	}
	ofstream glueVars(output_file_name, ios::trunc);
	if (!glueVars.is_open()) {
		cout << "Error opening glue variables file at " << output_file_name << endl;
		return 2;
	}

    generateHeader(glueVars);
    generateBody(locatedVars, glueVars);
	generateBottom(glueVars);

	return 0;
}

// For testing, we need to allow omitting the main function defined here.
#ifndef OPLCGLUE_OMIT_MAIN
int main(int argc, char *argv[]) {
    return mainImpl(argc, argv);
}
#endif // OPLCGLUE_OMIT_MAIN
