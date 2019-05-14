//
// Created by agavrilenko on 05.04.19.
//

#ifndef READER_DEPT_H
#define READER_DEPT_H

#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <map>
#include "group.h"
#include "reader.h"


const std::string default_group = "";

class Department {
public:
    std::string deptNumber;
    std::map<std::string, Group> groups;

    Department(std::string deptNumber = "0") :
            deptNumber(deptNumber)
    { }

    Department(std::string dep_number, DataFrame table, std::unordered_map <std::string, Professor> preps_map) {
        transpose(table.data);
        deptNumber = dep_number;
        for(size_t i = 0; i < table.header.size(); i++){
           // std::cout << table.header[i] << ":\n";
            groups[table.header[i]] = Group(table.header[i], table.data[i], preps_map);
        }
    }
    void print(std::string group_id = "", int day_id = default_day);
   // int getLessonsNumber();

};

void Department::print(std::string group_id, int day_id) {

    //print a group
    std::cout << "dept_id: " << deptNumber << std::endl;
    if (group_id != default_group) {
        groups[group_id].print(day_id);
    }
    //print all groups
    else {

        for (std::pair<std::string, Group> element : groups) {
            element.second.print(day_id);
            std::cout << std::endl;
        }
    }

}

#endif //READER_DEPT_H
