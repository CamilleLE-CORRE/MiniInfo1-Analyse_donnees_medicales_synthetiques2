#ifndef GETDATA_H
#define GETDATA_H

// #define MAX_PATIENTS 5001
// #define MAX_FIELDS 13
int max_patients = 5001;
int max_fields = 13;

void BrowseFilePatients(char file_name[], char* PatientsData[5001][13]);
void BrowseFileLivestyle(char file_name[], char* PatientsData[5001][13]);

#endif