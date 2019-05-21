//
// Created by agavrilenko on 06.04.19.
//

#ifndef READER_PREP_H
#define READER_PREP_H
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <iostream>
#include <map>


class Professor {
public:
    int prep_id;
    std::string name;
    std::string surname;
    std::string middle_name;
    std::string link;
    double knowledge;
    double skill;
    double social;
    double loyality;
    double total;

    Professor(int prep_id = 1, std::string name = "", std::string surname = "", std::string middle_name = "",
        std::string link= "",
        int knowledge = 0,
        int skill= 0,
        int social= 0,
        int loyality= 0,
        int total= 0) :
            prep_id(prep_id),
    name(name), surname(surname), middle_name(middle_name), link(link),
    knowledge(knowledge), skill(skill),social(social),loyality (loyality),total(total)
            { }

    Professor(std::vector<std::string> data) {
        prep_id = std::stoi(data[0]);
        name = data[1];
        surname = data[2];
        middle_name = data[3];
        knowledge = std::stoi(data[4]);
        skill = std::stoi(data[5]);
        social = std::stoi(data[6]);
        loyality = std::stoi(data[7]);
        total = std::stoi(data[8]);
        link = "";

    }

    void print(bool with_marks = false);


};

void Professor::print (bool with_marks){

    std::cout << "prof: ";
    std::cout << surname << " ";
    std::cout << name << " ";
    std::cout << middle_name << " ";
    if (with_marks) {
        std::cout << knowledge << " ";
        std::cout << skill << " ";
        std::cout << social << " ";
        std::cout << loyality << " ";
        std::cout << total << " ";
    }

}

#endif //READER_PREP_H
