#ifndef SEQUENTIALFILE_H
#define SEQUENTIALFILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include "../funciones.h"

class SequentialFile {
private:
    std::string mainFile;
    std::string auxFile;
    int auxMaxSize;

    std::vector<Record> loadRecords(const std::string &file);
    void saveRecords(const std::string &file, const std::vector<Record> &records);
    void appendRecord(const std::string &file, const Record &record);

public:
    SequentialFile(const std::string &mainFile, const std::string &auxFile);

    bool add(const Record &record);
    Record search(long dni);
    std::vector<Record> rangeSearch(long startDni, long endDni);
    void remove(long dni);
    void merge();
};

#endif // SEQUENTIALFILE_H