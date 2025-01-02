#ifndef GETDATA_H
#define GETDATA_H

#define MAX_PATIENTS 5001
#define MAX_FIELDS 20

void BrowseFilePatients(char file_name[], char* PatientsData[MAX_PATIENTS][MAX_FIELDS]);
void BrowseFileLivestyle(char file_name[], char* PatientsData[MAX_PATIENTS][MAX_FIELDS]);

#endif