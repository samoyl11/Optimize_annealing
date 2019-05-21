//
// Created by agavrilenko on 05.04.19.
//

#ifndef READER_READER_H
#define READER_READER_H

#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
//#include <boost/algorithm/string.hpp>
#include <iostream>

std::vector<std::string> split (std::string line, std::string delim);

void transpose(std::vector<std::vector<std::string> > &b)
{
    if (b.size() == 0)
        return;

    std::vector<std::vector<std::string> > trans_vec(b[0].size(), std::vector<std::string>());

    for (int i = 0; i < b.size(); i++)
    {
        for (int j = 0; j < b[i].size(); j++)
        {
            trans_vec[j].push_back(b[i][j]);
        }
    }

    b = trans_vec;    // <--- reassign here
}

class DataFrame
{
public:

    std::string fileName;
    std::string delimeter;
    std::vector<std::vector<std::string> > data;

    std::vector<std::string> header;


    DataFrame(std::string filename = "", std::string delm = ",") :
            fileName(filename), delimeter(delm)
    { }

    // Function to fetch data from a CSV File
    void read_csv(std::string fileName = "", std::string delimiter = ",");
    void print(int rows_number = 5);

};

void DataFrame::print(int rows_number)
{

    std::cout<< "Printig data:\n";
    for (size_t i = 0; i < rows_number; i++) {
        for (size_t j = 0; j < data[i].size(); j++) {
            std::cout << data[i][j] << " ";
        }
        std::cout << std::endl;
    }

};

void DataFrame::read_csv(std::string fileName, std::string delimiter)
{

    std::ifstream file(fileName);
  //  std::string delimiter = ";";
    std::string line;


    if(getline(file, line)) {
//        std::cout << "Get header:\n";
//        std::cout << line << "\n";
        header = split(line, delimiter);
    }
    // Iterate through each line and split the content using delimeter
    while (getline(file, line))
    {

        std::vector <std::string> row = split(line, delimiter);
        data.push_back(row);
    }
    // Close the File

    file.close();
}

std::vector<std::string> split (std::string line, std::string delim)
{

    size_t pos = 0;
    std::string token;
    std::vector <std::string> row;
    while ((pos = line.find(delim)) != std::string::npos) {

        token = line.substr(0, pos);
        line.erase(0, pos + delim.length());
        row.push_back(token);
    }
    row.push_back(line);
    return row;
}

#endif //READER_READER_H
