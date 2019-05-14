#ifndef READER_GROUP_H
#define READER_GROUP_H
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <random>
#include <set>
#include <ctime>
#include "day.h"

const int default_day = 777;
//double score_preps(DataFrame preps);
class Group {
public:
    std::string groupNumber;
    std::vector <Day> days ;
    std::vector <Lesson> lesson_map;
    std::set <std::string> lesson_set;

    Group(std::string groupNumber = "") :
            groupNumber(groupNumber)
            { };

    Group(std::string groupNumber, std::vector<std::string> data, std::unordered_map <std::string, Professor> preps_map) :
            groupNumber(groupNumber)
    {
        for(size_t i = 0; i < data.size(); i++) {

            //size_t key = i;

            lesson_map.push_back(Lesson(i, data[i],preps_map));
//            Lesson new1 = Lesson(i, data[i],preps_map);
//            new1.print();

            //Lesson one_lesson(i, data[i],preps_map);
            std::vector <std::string> raw_data = split(data[i], "#");
            lesson_set.insert(raw_data[0]);
//            std::vector<std::string>::const_iterator first = data.begin() + i ;
//            std::vector<std::string>::const_iterator last = data.begin() + i + 7;
//            std::vector<std::string> newVec(first, last);
//
//            Day one_day(i/7, newVec, preps_map);
//            days.push_back(one_day);
        }
    };

    void print(int day_id = default_day);
    void shuffle();
    //double score(DataFrame preps);
    void swapLessons(Lesson lesson1, Lesson lesson2);
    int getLessonsNumber();

};

//void Group::swapLessons(Lesson lesson1, Lesson lesson2) {
//    Lesson extra_lesson = lesson1;
//    for (auto it = s.begin(); it != s.end(); it++) {
//        cout << *it << endl;
//    }
//}

/*double Group::score(DataFrame preps) {
    double avg_score = score_preps(preps);
    double Loss = 0;
    int lesson_number = 0;

    for (int i = 0; i < lesson_map.size(); i++){
        if (lesson_map[i].exist) {
            Loss+= lesson_map[i].score(avg_score);
            lesson_number++;
        }
    }
    return Loss;
}*/
int myrandom (int i) { return std::rand()%i;}
void Group::shuffle() {


    srand(time(0));
//    std::map<size_t,Lesson> m = lesson_map;
//    std::vector<size_t> v;
//
//       for(auto i: m)
//    {
//        v.push_back(i.first);
//    }
    //random_shuffle(lesson_map.begin(), lesson_map.end(),myrandom);
//    std::vector<size_t>::iterator it=v.begin();
//
//    for(auto& i:m)
//    {
//        Lesson ts=i.second;
//        i.second=m[*it];
//        m[*it]=ts;
//        it++;
//    }
//    lesson_map = m;

}

void Group::print(int day_id) {

    std::cout << "group_id:" << groupNumber << std::endl;

    for (int i = 0; i < 42; i++) {
        if(i % 7 == 0)
            std::cout << "Day: " << i / 7 << std::endl
                         ;
        for (auto lesson: lesson_map) {
            if(lesson.lesson_id == i) {
                lesson.print();
            }

        }
    }

}



#endif //READER_GROUP_H
