//
// Created by agavrilenko on 05.04.19.
//

#ifndef READER_DAY_H
#define READER_DAY_H
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <iostream>
#include "lesson.h"


class Day
{
public:

    std::vector <Lesson>  lessons ;
    std::string name = "";

    Day(std::string name = "friday") :
            name(name)
            { };

    Day(int day_id, std::vector <std::string> data, std::unordered_map <std::string, Professor> preps_map)
    {

        std::map< int, std::string > week_days;
        week_days[0] = "Понедельник";
        week_days[1] = "Вторник";
        week_days[2] = "Среда";
        week_days[3] = "Четверг";
        week_days[4] = "Пятница";
        week_days[5] = "Суббота";
        week_days[6] = "Воскресение";

        name = week_days[day_id];

        for(size_t i = 0; i < data.size(); i++) {
            Lesson one_lesson(i, data[i],preps_map);
            lessons.push_back(one_lesson);
        }
    };

    void print();
    int getLessonsNumber();

};

int Day::getLessonsNumber() {
    int number = 0;
    for (size_t i = 0; i < lessons.size(); i++) {
        if(lessons[i].exist == true) {
            number++;
        }
    }
    return number;
}
void Day::print() {
    std::cout << "day: " << name << std::endl;
    for (size_t i = 0; i < lessons.size(); i++) {
        lessons[i].print();
        std::cout << std::endl;
    }
};

#endif //READER_DAY_H
