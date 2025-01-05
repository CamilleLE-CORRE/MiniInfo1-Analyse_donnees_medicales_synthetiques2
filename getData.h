#ifndef GETDATA_H
#define GETDATA_H

int max_patients = 5001;
int max_fields = 13;

void BrowseFilePatients(char file_name[], char* PatientsData[max_patients][max_fields]);
void BrowseFileLivestyle(char file_name[], char* PatientsData[max_patients][max_fields]);

#endif