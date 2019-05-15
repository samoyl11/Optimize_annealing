#ifndef READER_LESSON_H
#define READER_LESSON_H
#include <math.h>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <iostream>
#include <map>
#include <unordered_map>
#include "reader.h"
#include "prep.h"
#include <iomanip>
class Lesson {
public:
    size_t lesson_id;
    bool exist;
    bool doubled;
    bool moveable;
    std::string subject;
    std::string room;
    std::string professor;
    std::vector <Professor> prep_list;
    std::string time;
    Lesson(size_t lesson_id = 0, bool exist = false, bool doubled = false, bool moveable = false, std::string subject = "" ,std::string room = "", std::string professor = "", std::string time = "") :
            lesson_id(lesson_id),
            doubled(doubled),
            moveable(moveable),
            exist(exist),
            subject(subject),
            room(room),
            professor(professor),
            prep_list(),
            time(time)
            { };

    Lesson(size_t i, std::string data, std::unordered_map <std::string, Professor> preps_map) {

        std::map< int, std::string > time_splits;
        time_splits[0] = "9:00-10:25";
        time_splits[1] = "10:45-12:10";
        time_splits[2] = "12:20-13:45";
        time_splits[3] = "13:55-15:20";
        time_splits[4] = "15:30-16:55";
        time_splits[5] = "17:05-18:30";
        time_splits[6] = "18:35-20:00";
        //std::cout << "data " << data << std::endl;
        std::vector <std::string> raw_data = split(data, "#");
        lesson_id = i;
        exist = false;
        if (raw_data[0] != "") {

            exist = true;
            //std::cout << raw_data[0] << " " << raw_data[1] << " " <<raw_data[2] << " " << raw_data[3] << std::endl;
            moveable = true;
            subject = raw_data[0];
            professor = raw_data[1];
            room = raw_data[2];
            time = time_splits[i%7];
            if((subject == "Компьютерные технологии") || (subject == "Общая физика: лаб.практикум") || (subject == "Доп. главы дискретного анализа") || (subject== "Современные проблемы прикладной математики") ) {
                doubled = true;
            }
            else {
                doubled = false;
            }

            if ((subject == "Доп. главы дискретного анализа") || ((subject == "Дифференциальные уравнения"))
            || (subject == "Гармонический анализ") || (subject == "Мера и интеграл Лебега") || (subject == "Ин.яз.") || (subject == "Современные проблемы прикладной математики")) {
               moveable = false;
            }

            std::vector <std::string> raw_preps = split(professor, "_");
            for(size_t i = 0; i < raw_preps.size(); i++) {

                Professor realPrep = Professor(preps_map[raw_preps[i]]);
                prep_list.push_back(realPrep);
            }
        }

    };

    void print();
    double score(double avg_score);

};

double Lesson::score(double avg_score)
{
    double professor_score = 0;
    std::vector <double> a = {0.1, 0.4, 0.3, 0.2, 0.2, 0.1, 0.1};
    double location_score = a[lesson_id%7];
    if (!prep_list.empty()) {
        for(int i = 0; i < prep_list.size(); i++){
            professor_score+= double(prep_list[i].total)/5;
        }
        professor_score = professor_score/prep_list.size();
    }
    else {
        professor_score+= double(avg_score)/5;
    }

    return roundf((professor_score + location_score) * 1000 )/ 1000;
};
void Lesson::print() {
    //std::cout  << "id: " << lesson_id << " ";
    if (exist) {
        std::cout  <<  "subject: "  << subject << " ";
        std::cout  << "room: " << room << " ";
        //std::cout  << "preps: " << professor << " ";

        for(size_t i = 0; i < prep_list.size(); i++) {

            prep_list[i].print();
        }
        std::cout << "time: " << time << " ";
        std::cout << "exist: " << exist << " ";
    }
    else {
        std::cout << "none " ;
        std::cout << "exist: " << exist << " ";
    }
    std::cout<<std::endl;



};
#endif //READER_LESSON_H
