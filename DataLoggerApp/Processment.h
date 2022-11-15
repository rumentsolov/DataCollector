#ifndef PROCESS_H
#define PROCESS_H

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string> 
#include <cstring>

#include "Record.h"
#include "ComInit.h"
#include "ActionLogger.h"
#include "GlobalVariables.h"


double voltageLine12AVG;       // Average voltage between line 1 and 2
double voltageLine23AVG;       // Average voltage between line 2 and 3
double voltageLine31AVG;       // Average voltage between line 3 and 1
double currentLine1AVG;        // Average current in line 1
double currentLine2AVG;        // Average current in line 2
double currentLine3AVG;        // Average current in line 3
double combinatedPowerAVG;     // Average combinated power factor for line 1+2+3
double activePowerAVG;         // Average combinated active power for line 1+2+3
double reactivePowerAVG;       // Average combinated reactive power for line 1+2+3
double apparentPowerAVG;       // Average combinated apparent power for line 1+2+3
double calculatedPowerAVG;     // Average combinated calculated power for line 1+2+3


void writeToFL(std::string totalString, const char* newFile) {
    std::fstream file;
    file.open(newFile, std::ios::out | std::ios::app);
    if (!file.is_open()) {
        std::ofstream out(newFile);
    }
    else
    {

        file << totalString << std::endl << std::endl;
        file.close();
    }
}

void operateTXTFile() {

   
   ActionSave(recordFileString);

    std::vector<Record> newRecords; // the records will be stored as objecs here in this vector for easy work

    int counter = 0; // counts the rows readed from the input file

    std::ifstream rawTxtFile;
    rawTxtFile.open(rawFile);
    std::string line;

    while (getline(rawTxtFile, line)) // its easy and better to read  and process with my own txt file 
    {
        counter++;
        if (counter > 2) { // skipping first 2 rows from file

            for (int i = 0; i < line.size(); i++) if (line[i] == ',') line[i] = ' ';
            std::stringstream sStr(line);
            std::string dateDtmp, timeDtmp;
            std::string id;                     // id inside the table controller record
            int controllerId;           // serial number of the record inside the controller
            int year;                   // year record is created
            std::string month;                  // month record is created
            std::string day;                    // day record is created
            std::string hour;                   // hour record is created
            std::string minute;                 // minute record is created
            std::string second;                 // second record is created

            double voltageLine12;       // voltage between line 1 and 2
            double voltageLine23;       // voltage between line 2 and 3
            double voltageLine31;       // voltage between line 3 and 1
            double currentLine1;        // current in line 1
            double currentLine2;        // current in line 2
            double currentLine3;        // current in line 3
            double combinatedPower;     // combinated power factor for line 1+2+3
            double activePower;         // combinated active power for line 1+2+3
            double reactivePower;       // combinated reactive power for line 1+2+3
            double apparentPower;       // combinated apparent power for line 1+2+3
            double calculatedPower = 0; // combinated calculated power for line 1+2+3

            // skipping id & controllerId
            sStr >> id >> controllerId >> dateDtmp;

            for (int i = 0; i < dateDtmp.size(); i++) if (dateDtmp[i] == '/') dateDtmp[i] = ' ';
            std::stringstream sStrDate(dateDtmp);

            sStrDate >> day >> month >> year;

            sStr >> timeDtmp;
            for (int i = 0; i < timeDtmp.size(); i++) if (timeDtmp[i] == ':') timeDtmp[i] = ' ';
            std::stringstream sStrTime(timeDtmp);

            sStrTime >> hour >> minute >> second;

            sStr.precision(2);

            sStr >> voltageLine12 >> voltageLine23 >> voltageLine31 >> currentLine1 >> currentLine2 >> currentLine3 >> combinatedPower >> activePower >> reactivePower >> apparentPower;
            //sStr >> calculatedPowtmp;


            if (currentLine1 != 0.00 & currentLine2 != 0.00 & currentLine3 != 0.00)
            {
                Record curRecord(id, controllerId, year, month, day, hour, minute, second, voltageLine12, voltageLine23, voltageLine31, currentLine1, currentLine2, currentLine3, combinatedPower, activePower, reactivePower, apparentPower, calculatedPower);

                newRecords.push_back(curRecord);
            }
        }
    }

    int correctRecords = 0;

    double line1tmp = 9999, line2tmp = 9999, line3tmp = 9999;
    std::ostringstream osTr;
    osTr << std::fixed;
    osTr << std::setprecision(2);
 
        // FINDS THE AVARAGE VALUES FROM OBJECTS

    for (int i = 0; i < newRecords.size(); i++) {

        if (
            line1tmp * startupCurrentFactor > newRecords[i].currentLine1 &&
            line2tmp * startupCurrentFactor > newRecords[i].currentLine2 &&
            line3tmp * startupCurrentFactor > newRecords[i].currentLine3
            ) 
        {
            
            voltageLine12AVG += newRecords[i].voltageLine12;
            voltageLine23AVG += newRecords[i].voltageLine23;
            voltageLine31AVG += newRecords[i].voltageLine31;

            currentLine1AVG += newRecords[i].currentLine1;
            currentLine2AVG += newRecords[i].currentLine2;
            currentLine3AVG += newRecords[i].currentLine3;

            line1tmp = newRecords[i].currentLine1;
            line2tmp = newRecords[i].currentLine2;
            line3tmp = newRecords[i].currentLine3;


            combinatedPowerAVG += newRecords[i].combinatedPower;
            activePowerAVG += newRecords[i].activePower;
            reactivePowerAVG += newRecords[i].reactivePower;
            apparentPowerAVG += newRecords[i].apparentPower;

            correctRecords++;
        }

        
    }


    voltageLine12AVG /= correctRecords;
    voltageLine23AVG /= correctRecords;
    voltageLine31AVG /= correctRecords;
    currentLine1AVG /= correctRecords;
    currentLine2AVG /= correctRecords;
    currentLine3AVG /= correctRecords;
    combinatedPowerAVG /= correctRecords;
    activePowerAVG /= correctRecords;
    reactivePowerAVG /= correctRecords;
    apparentPowerAVG /= correctRecords;

  

    osTr <<
        "AVG Voltage 1-2 :" << 
        voltageLine12AVG << 'V'<< std::endl <<
        "AVG Voltage 2-3 :" << 
        voltageLine23AVG << 'V'<< std::endl <<
        "AVG Voltage 3-1 :" << 
        voltageLine31AVG << 'V'<< std::endl <<
        "AVG Current Line 1 :" << 
        currentLine1AVG << 'A' << std::endl <<
        "AVG Current Line 2 :" <<
        currentLine2AVG << 'A' << std::endl <<
        "AVG Current Line 3 :" << 
        currentLine3AVG << 'A' << std::endl <<
        "AVG Combinated Power :" <<
        combinatedPowerAVG << 'W' << std::endl <<
        "AVG Active Power :" <<
        activePowerAVG << 'W' << std::endl <<
        "AVG Apparent Power :" <<
        apparentPowerAVG << 'W' << std::endl << std::endl;


    for (int i = 0; i < newRecords.size(); i++) {
        newRecords[i].valuesSendToStream(osTr);
    }



    writeToFL(osTr.str(), recordFile);

   /* delete[] rawFile;
    rawFile = nullptr;
    delete[] recordFile;
    recordFile = nullptr;*/

    return;
}

#endif 
