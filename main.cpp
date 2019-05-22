#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <map>
#include <iostream>
#include <dirent.h>
#include "prep.h"
#include "reader.h"
#include <string.h>
#include <iomanip>
#include "grade.h"
#include <array>
#include <random>
#include <set>
#include <cstdlib>
#include <ctime>
const std::string path_to_dir = "/Users/alexander/My_pandas/";
//const std::string path_to_dir = "tt/static/Schedules/";
const std::string path_to_rawdata = "departments_2/";
const std::string path_to_preps = "data_hashed.csv";

std::string filename;
auto rng = std::default_random_engine {};

std::map<std::string, std::vector<Lesson>> find_all_places(Grade, std::string);
std::vector<Lesson> find_free_spot_for_lesson(Group, std::string, std::map<std::string, std::vector<Lesson>>, bool doubled);
double number_of_windows(Group);
double ratings(Group, double, double, double, double, double);
double except_days(Group, int);
double time_of_study(Group, int, int, int, int);
double exeption_of_pairs(Group, std::string);

class annealing {
private:
    float temperature;
    Group min_group;
    Grade grade;
    Group current_group;
    Group new_group;
    std::string group, pair_id;
    double old_energy;
    double new_energy;
    unsigned int step_num;
    double threshold;
    double number_of_windows_weight, ratings_knowledge, ratings_skill, ratings_social, ratings_loyality, ratings_total, exept_days_weight, weight_time_of_study;
    int exept_days_int, which1, which2, days1, days2;
    std::map<std::string, std::vector<Lesson>> possible_places;
    std::vector<std::string> lesson_map_without_none;
public:
    annealing(): temperature(0), step_num(0), threshold(1) {
    }
    annealing(Grade new_grade, float init_temp, double number_of_windows_weight=1, double ratings_knowledge=0, double ratings_skill=0, double ratings_social=0,
              double ratings_loyality=0, double ratings_total=0,int exept_days_int=5, double exept_days_weight=100, int which1=0, int which2=0, int days1=0,
              int days2=0, double weight_time_of_study=0, std::string pair_id=""): temperature(init_temp), step_num(0), threshold(1), number_of_windows_weight(number_of_windows_weight),
              ratings_knowledge(ratings_knowledge), ratings_skill(ratings_skill), ratings_social(ratings_social), ratings_loyality(ratings_loyality),
              ratings_total(ratings_total), exept_days_weight(exept_days_weight), weight_time_of_study(weight_time_of_study), exept_days_int(exept_days_int),
              which1(which1), which2(which2), days1(days1), days2(days2), pair_id(pair_id)
    {
        grade.group_map = new_grade.group_map;
        grade.gradeNumber = new_grade.gradeNumber;
    }

    void decrease_temp() {
        if (temperature > 20) {
            temperature -= 0.5;
        }
        else if ((temperature <= 20) && (temperature >3))
            temperature -= 0.05;
        else
            temperature -= 0.01;
    }

    double loss(Group tempgroup){
        double loss = 0;
        //tempgroup.();
        //cout << tempgroup.lesson_map[0].prep_list[1].social;
        //double a = number_of_windows_weight * number_of_windows(tempgroup);
        //double b = ratings(tempgroup, ratings_knowledge,  ratings_skill,  ratings_social,  ratings_loyality,  ratings_total);
        double c = except_days(tempgroup, exept_days_int);
        //double d = weight_time_of_study * time_of_study(tempgroup, which1, which2, days1, days2);
        //std::cout << std::endl << std::endl << a << " " << b <<" " << c << " " << d <<std::endl;*/
        //std::cout << exept_days_weight << " " << c << std::endl;
        loss += number_of_windows_weight * number_of_windows(tempgroup)
                + ratings(tempgroup, ratings_knowledge,  ratings_skill,  ratings_social,  ratings_loyality,  ratings_total)
                + exept_days_weight * except_days(tempgroup, exept_days_int)
                + weight_time_of_study * time_of_study(tempgroup, which1, which2, days1, days2) + exeption_of_pairs(tempgroup, pair_id);
        //std::cout << loss << std::endl;
        return loss;
    }

    void run(std::string in_group) {
        group = in_group;
        possible_places = find_all_places(grade, group);
        std::cout << "Possible places found" << std::endl;

        for (auto x: grade.group_map[group].lesson_map) {
            if(x.subject != "")
                lesson_map_without_none.push_back(x.subject);
        }

        std::shuffle(std::begin(lesson_map_without_none), std::end(lesson_map_without_none), rng); //shuffle lessons
        double score = 1000;
        //Group tempgroup;
        //Group best;
        //double min = 10000;
        /*for(int i = 0; i < 1000; i++) {
            tempgroup = generate_group();
            if (tempgroup.lesson_map.size() != 0)
                score = loss(tempgroup, number_of_windows_weight, ratings_knowledge, ratings_skill, ratings_social, ratings_loyality, ratings_total, exept_days_int, exept_days_weight, which1, which2, days1, days2, weight_time_of_study);
                if (score < min) {
                    best = tempgroup;
                    min = score;
                }
            std::cout << "SCORE: " << score << std::endl;*/
            //tempgroup.();
        //}

        //best.();
        //std::cout << "BEST SCORE = " << min << std::endl;
        old_energy = loss(grade.group_map[group]);
        std::cout << "INIT_OLD_ENERGY" << old_energy << std::endl;
        current_group = grade.group_map[group];
        //min_group = group;
        while(temperature > 0) {
            if (step_num % 100 == 0) {
                print();
            }
            /*std::cout << "############################### CURRENT GROUP ###############################" << std::endl;
            current_group.print();
            std::cout << "############################### CURRENT GROUP END ###############################" << std::endl << std::endl;
            std::cout << "############################### CURRENT ENERGY (LOSS OF CURRENT GROUP) ###############################        " << loss(current_group) << std::endl;
            std::cout << "############################### CURRENT ENERGY            (OLD ENERGY) ###############################        " << old_energy << std::endl;*/
            step();
        }
        double a = loss(current_group);
        //std::cout << "FINAL ENERGY " << a << std::endl;
        //std::cout << "FINISHED STEPS" << std::endl;
        //std::cout << "WRITING TO "<< filename << std::endl << std::endl;
        current_group.to_csv(filename);
    }

    void step() {
        step_num += 1;
        if (step_num == 1) {
            decrease_temp();
            return;
        }
        new_group = generate_group();
        if(new_group.lesson_map.size() != 0) {
            new_energy = loss(new_group);
            //std::cout << "OLD: " << old_energy << " NEW: " << new_energy<<std::endl;
            if (new_energy > old_energy) {
                threshold = exp(-1.0 * (new_energy - old_energy) / temperature);
                float p = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                //std::cout << "THRESHOLD : " << threshold << " P: " << p << std::endl;
                //cout <<std::endl << "new energy > old energy, probability: " << p <<std::endl;
                if (p < threshold) {
                    //std::cout << "HERE" << std::endl;
                    current_group = new_group;
                    old_energy = new_energy;
                    //cout << "p = " << p << ", threshold = " << threshold <<std::endl;
                }
            }
            else {
                old_energy = new_energy;
                current_group = new_group;
            }
        }
        decrease_temp();
    }

    void print() {
        std::cout << "STEP: " << step_num <<" ENERGY: " << old_energy << " TEMPERATURE: " << temperature << std::endl;
    }

    Group generate_group();
};

Group annealing::generate_group() {
    srand(time(0));
    int i = rand() % lesson_map_without_none.size();
    int j = rand() % lesson_map_without_none.size();

    //std::cout << "I J: " << i << " " << j << std::endl;


    std::string temp = lesson_map_without_none[i];
    lesson_map_without_none[i] = lesson_map_without_none[j];
    lesson_map_without_none[j] = temp;

    //std::cout << "SWAPPED TWO IN LESSON_MAP: " << std::endl;

    /*
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(lesson_map_without_none), std::end(lesson_map_without_none), rng); //shuffle lessons
    */

    //START FILLING TIMETABLE
    Group new_group;
    new_group.lesson_map = std::vector<Lesson> (42);
    std::vector<Lesson> new_lesson;
    std::multiset<std::string> tempset;
    for(auto lesson: lesson_map_without_none) {
        if(possible_places[lesson][0].doubled == false){
            //std::cout << "DOUBLED " << possible_places[lesson][0].doubled << " FOR " << lesson << std::endl;
            new_lesson = find_free_spot_for_lesson(new_group, lesson, possible_places, false);
        //std::cout << "NEW LESSON: " << std::endl;
            if (new_lesson[0].subject != "") {
                new_group.lesson_map[new_lesson[0].lesson_id] = new_lesson[0];
            }
            else {
                return Group();
            }
        }
        else {
            //std::cout << "DOUBLED " << possible_places[lesson][0].doubled << " FOR " << lesson << std::endl;
            if(tempset.find(lesson) != tempset.end()) {
                //std::cout << "FOUND " << lesson << " IN SET" << std::endl;
                tempset.erase(tempset.find(lesson));
            }
            else {
                new_lesson = find_free_spot_for_lesson(new_group, lesson, possible_places, true);
                tempset.insert(lesson);
                //std::cout << "PUT " << lesson << " IN SET" << std::endl;
                if (new_lesson[0].subject != "") {
                    new_group.lesson_map[new_lesson[0].lesson_id] = new_lesson[0];
                    new_group.lesson_map[new_lesson[1].lesson_id] = new_lesson[1];
                }
            }
        }
    }
    std::vector<int> prohibited;
    for (auto lesson: new_group.lesson_map) {
        if (lesson.subject != "")
            prohibited.push_back(lesson.lesson_id);
    }
    for (int i = 0; i < 42; i++) {
        bool flag = true;
        for(auto j: prohibited) {
            if (i == j)
                flag = false;
        }
        if (flag == true)
            new_group.lesson_map[i]= Lesson(i);
    }
    //std::cout << "RETURNING: " << std::endl;
    return new_group;
}



int main(int agrc, char ** argv)
{
    filename = argv[1];
    std::string group = argv[2];


    DataFrame preps(",");
    std::unordered_map <std::string, Professor> preps_map;

    std::cout << "Loading preps ..." << std::endl;
    preps.read_csv(path_to_dir + path_to_preps);
    std::cout << preps.data.size() << std::endl;

       // std::cout << "avg_score: " << avg_score << std::endl;

        //load preps(check data_hashed)
    for (size_t i = 0; i< preps.data.size(); i++) {

        std::string hash = preps.data[i][9];
        preps_map[hash] = Professor(preps.data[i]);

    }

    Grade new_grade(path_to_dir, path_to_rawdata, preps_map);
    //std::cout << new_grade.group_map["776"].lesson_map.size();
    //new_grade.print();
    //std::map<std::string, std::vector<Lesson>> possible_places = find_all_places(new_grade, "778");
    /*for (auto it = new_grade.group_map[0].lesson_set.begin(); it != new_grade.group_map[0].lesson_set.end(); ++it) {
        std::cout<<*it<<std::endl;
    }*/
//    std::cout<<possible_places.begin()->first << std::endl;
//    std::cout<<"wtf"<<std::endl;
    //std::cout << possible_places.begin()++->second[0];



    /*for(const auto & it: possible_places) {
        std::cout<<"≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈"<<it.first << "≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈"<<std::endl << std::endl;
        for(const auto itt: it.second) {
            std::cout<<itt.subject << "   " << itt.lesson_id / 7 <<  "   " << itt.time << "   " << itt.professor << std::endl;
        }
        std::cout<<std::endl<<std::endl;
    }*/

    annealing optimizer(new_grade, 150, std::stod(argv[3]), std::stod(argv[4]), std::stod(argv[5]), std::stod(argv[6]), std::stod(argv[7]), std::stod(argv[8]),
            std::stoi(argv[9]), 0.01, std::stoi(argv[10]), std::stoi(argv[11]), std::stoi(argv[12]), std::stoi(argv[13]), std::stod(argv[14]), argv[15]);

    optimizer.run(group);
    //std::cout << optimizer.loss(new_grade.group_map["777"]) << std::endl;
    //optimizer.run("778");

    return 0;
}

// FIND ALL POSSIBLE PLACES FOR ALL LESSONS
std::map<std::string, std::vector<Lesson>> find_all_places(Grade grade, std::string group_id) {
    std::map<std::string, std::vector<Lesson>> return_map;
    for(auto it=grade.group_map[group_id].lesson_set.begin(); it != grade.group_map[group_id].lesson_set.end(); ++it) {
        if (*it == "")
            continue;
        //all subjects of current group
        std::vector<Lesson> current;
        for(auto group: grade.group_map) {
            //all groups in grade
            for(auto lesson: group.second.lesson_map) {
                if(lesson.subject == *it) {
                    bool flag = true;
                    for (auto in_subjects: current) {
                        if((lesson.time == in_subjects.time) && (lesson.professor == in_subjects.professor))
                            flag = false;
                    }
                    if (flag == true)
                        current.push_back(lesson);
                }
            }


        }
        return_map.insert(std::pair<std::string, std::vector<Lesson>>(*it, current));
    }
    return return_map;
}



//FIND A FREE SPOT FOR A LESSON
std::vector<Lesson> find_free_spot_for_lesson(Group group, std::string lesson_name, std::map<std::string, std::vector<Lesson>> possible_places, bool doubled) {
    //BUSY IDS
    std::vector<int> busy_ids;
    std::vector<Lesson> new_lessons;
    for (auto lesson:group.lesson_map) {
        if (lesson.subject != "")
            busy_ids.push_back(lesson.lesson_id);
    }

    for(const auto & it: possible_places) {
        if(it.first == lesson_name) {
            std::vector<Lesson> all_lessons_with_this_name = it.second;
            std::shuffle(std::begin(all_lessons_with_this_name), std::end(all_lessons_with_this_name), rng); //shuffle lessons
            if(doubled == false) {
                for (auto lesson:all_lessons_with_this_name) {
                    bool flag = true;
                    for (auto id: busy_ids) {
                       if (lesson.lesson_id == id)
                           flag = false;
                   }
                   if (flag == true) {
                       new_lessons.push_back(lesson);
                        //std::cout<<lesson_name << std::endl;
                        //lesson.print();
                        //std::cout<<lesson.lesson_id<<std::endl;
                        return new_lessons;
                   }

                }
            }
            else {
                for(auto lesson_1: all_lessons_with_this_name) {
                    for(auto lesson_2: all_lessons_with_this_name) {
                        bool flag = true;
                        if (((lesson_2.lesson_id - lesson_1.lesson_id) == 1) && (lesson_1.professor == lesson_2.professor)) {

                            for(auto id: busy_ids) {
                                if ((lesson_1.lesson_id == id) || (lesson_2.lesson_id == id))
                                    flag = false;
                            }
                            if (flag == true) {
                                new_lessons.push_back(lesson_1);
                                new_lessons.push_back(lesson_2);
                                return new_lessons;
                            }
                        }


                    }
                }
            }
        }
    }
    new_lessons.push_back(Lesson());
    return new_lessons;
}

double number_of_windows(Group tempgroup) {

    std::vector<int> windows(6);
    std::vector<int> total(6);
    std::vector<double> part(6);
    std::vector<int> first = {0,0,0,0,0,0};
    std::vector<int> last = {6, 6, 6, 6, 6, 6};

    int s = 0;
    int t = 0;
    for (int i = 0; i < 6; i++) { //это номер дня
        for (int j = 7 * i; j < 7 * i + 7; j++) { // номер пары в этот день
            if(t == 0){
                if(tempgroup.lesson_map[j].subject == ""){
                    first[i]++;
                }
                else {
                    t = 1;
                }
            }
        }
        for (int j = 7 * i + 6; j > 7 * i - 1; j--) { // номер пары в этот день

            if(s == 0){
                if(tempgroup.lesson_map[j].subject == ""){
                    last[i]--;
                }
                else {
                    s = 1;
                }
            }
        }
        total[i] = last[i] - first[i] + 1;
        //cout << first[i] << ";" << last[i] <<std::endl;

        s = 0;
        t = 0;
    }
    //до этого момента считали общее количество пар.

    /*for (int i = 0; i < 6; i++) { //это номер дня
        for (int j = 7 * i; j < 7 * i + 7; j++) { // номер пары в этот день
            if(tempgroup.lesson_map[j].subject == "" && j >= first[i]  + 7 * i && j < last[j] + 7 * i){ //то есть что если индекс лежит между последней и первой парой в этот день
                windows[i]++;
               std::cout << i << "===" << first[i]  + 7 * i << "====" << last[j] + 7 * i <<std::endl;
            }
        }
    }*/
    for(int j = 0; j < 42; j++){
        t = j/7;
        if((tempgroup.lesson_map[j].subject == "") && (7 * t + first[t] < j) && (7 * t + last[t] > j)){ //то есть что если индекс лежит между последней и первой парой в этот день
            windows[t]++;
        }
        //cout<< "номер первой пары =" << 7 * t + first[t] << ";    номер последней пары =" << 7 * t + last [t]<< ";    " << j  << "кол-во окон равно:" << windows[t] <<std::endl;
    }

    int sum_win = 0;
    int sum_tot = 0;
    for (int i = 0; i < 6; i++) {
        if(total[i] == -7){
            total[i] = 0;
            windows[i] = 0;//это убираем выходной
        }
        sum_tot += total[i];
        sum_win += windows[i];

    }
    //cout << sum_tot << ";   " << sum_win <<std::endl;
    //cout << (double)sum_win/(double)sum_tot;
    return (double)sum_win/(double)sum_tot;


}

double ratings(Group tempgroup, double ratings_knowledge, double ratings_skill, double ratings_social, double ratings_loyality, double ratings_total){
    int s = 0;
    double summary_rate = 0;
   /*for (auto lesson: tempgroup.lesson_map) {
        std::cout << lesson.exist << " ";
    }
    std::cout << std::endl;*/
    for (int i = 0; i < 42; i++) {
        //std::cout << "ama here1111"<<std::endl;
        //std::cout << "exist = " << tempgroup.lesson_map[i].exist << std::endl;
        if(tempgroup.lesson_map[i].exist){

            if(tempgroup.lesson_map[i].prep_list[0].total > 0){
                summary_rate += ratings_skill * tempgroup.lesson_map[i].prep_list[0].skill +
                        ratings_social * tempgroup.lesson_map[i].prep_list[0].social +
                        ratings_loyality * tempgroup.lesson_map[i].prep_list[0].loyality +
                        ratings_knowledge * tempgroup.lesson_map[i].prep_list[0].knowledge+
                        ratings_total * tempgroup.lesson_map[i].prep_list[0].total;
                //std::cout << tempgroup.lesson_map[i].prep_list[0].skill<<std::endl;
                s++;
                //cout << "Имя: " << tempgroup.lesson_map[i].prep_list[0].name << ":     "  << tempgroup.lesson_map[i].prep_list[0].skill << "  "<<
                      // tempgroup.lesson_map[i].prep_list[0].social << "  " <<
                      //          tempgroup.lesson_map[i].prep_list[0].loyality << "   "
                        //           << tempgroup.lesson_map[i].prep_list[0].knowledge << "   "
                         //          << tempgroup.lesson_map[i].prep_list[0].total <<std::endl;
            }
        }
    }
    //
    //std::cout << "суммарный рейтинг равен: "<< summary_rate << " s = " << s  << "    "<<  summary_rate/25/s << std::endl;
    return summary_rate/25/s;
}

double except_days(Group tempgroup, int input){
    //cout << input <<std::endl;
    double output = 0;
    int s6 = input % 10;
    input = input / 10;
    //cout << s6 << "  " <<  input <<std::endl;
    int s5 = input % 10;
    input = input / 10;
    //cout << s5 << "  " << input <<std::endl;

    int s4 = input % 10;
    input = input / 10;
    //cout << s4 << "  " << input <<std::endl;

    int s3 = input % 10;
    input = input / 10;
    //cout <<  s3 << "  " << input <<std::endl;

    int s2 = input % 10;
    input = input / 10;
    //cout << s2 << "  "  << input <<std::endl;

    int s1 = input % 10;
    //cout << "s1 = " << s1 << " input =    " <<  input <<std::endl;
    //tempgroup.print();
    for (int i = 0; i < 42; i++) {
        //std::cout << tempgroup.lesson_map[i].exist << std::endl;
        if(tempgroup.lesson_map[i].exist == 1){
            //std::cout << i / 7 + 1 << "  " << s1 << s2 << s3 << s4 << s5 << s6  << std::endl;
            if((i / 7 + 1 == s1 or i / 7 + 1 == s2  or i / 7 + 1 == s3  or i / 7 + 1 == s4 or i / 7 + 1 == s5 or i / 7 + 1 == s6)){
                //\cout << i <<std::endl;
                output += 100;
            }
        }

    }
    return output;

}

double time_of_study(Group tempgroup, int which1, int which2, int input1, int input2){ //1 - когда начинать, 2 - когда заканчивать
    std::vector<int> total = {0, 0, 0, 0, 0, 0};
   std::vector<int> first = {0,0,0,0,0,0};
   std::vector<int> last = {6, 6, 6, 6, 6, 6};
    double output = 0;


    int s = 0;
    int t = 0;
    for (int i = 0; i < 6; i++) { //это номер дня
        for (int j = 7 * i; j < 7 * i + 7; j++) { // номер пары в этот день
            if(t == 0){
                if(tempgroup.lesson_map[j].subject == ""){
                    first[i]++;
                }
                else {
                    t = 1;
                }
            }
        }
        for (int j = 7 * i + 6; j > 7 * i - 1; j--) { // номер пары в этот день

            if(s == 0){
                if(tempgroup.lesson_map[j].subject == ""){
                    last[i]--;
                }
                else {
                    s = 1;
                }
            }
        }
        total[i] = last[i] - first[i] + 1;
        //cout << first[i] << "  " << last[i] << "  " << total[i] << std::endl;
        s = 0;
        t = 0;
    }

    int s6 = input1 % 10;
    input1 = input1 / 10;
    //cout << s6 << "  " <<  input <<std::endl;
    int s5 = input1 % 10;
    input1 = input1 / 10;
    //cout << s5 << "  " << input <<std::endl;

    int s4 = input1 % 10;
    input1 = input1 / 10;
    //cout << s4 << "  " << input <<std::endl;

    int s3 = input1 % 10;
    input1 = input1 / 10;
    //cout <<  s3 << "  " << input <<std::endl;

    int s2 = input1 % 10;
    input1 = input1 / 10;
    //cout << s2 << "  "  << input <<std::endl;

    int s1 = input1 % 10;
    //cout << "s1 = " << s1 << " inpu

    if(which1 == 1){// раньше начинать
        for(int i = 0; i < 6; i++){
            if((i + 1 == s1 or i + 1 == s2  or i + 1 == s3  or i + 1 == s4 or i + 1 == s5 or i + 1 == s6) && first[i] != 7){
                //cout << "   " << i << "   "<< first[i] << "o   " << output;
                //cout << first[i] <<std::endl;
                output += (double)first[i]/7;
                //cout << (double)first[i]/7;
            }
        }
    }
    if(which1 == -1){//позже начинать
        for(int i = 0; i < 6; i++){
            if((i + 1 == s1 or i + 1 == s2  or i + 1 == s3  or i + 1 == s4 or i + 1 == s5 or i + 1 == s6) && first[i] != 7){
                output += (1 - (double)first[i]/7)/6;

            }
        }
    }

    int t6 = input2 % 10;
    input2 = input2 / 10;
    //cout << s6 << "  " <<  input <<std::endl;
    int t5 = input2 % 10;
    input2 = input2 / 10;
    //cout << s5 << "  " << input <<std::endl;

    int t4 = input2 % 10;
    input2 = input2 / 10;
    //cout << s4 << "  " << input <<std::endl;

    int t3 = input2 % 10;
    input2 = input2 / 10;
    //cout <<  s3 << "  " << input <<std::endl;

    int t2 = input2 % 10;
    input2 = input2 / 10;
    //cout << s2 << "  "  << input <<std::endl;

    int t1 = input2 % 10;
    //cout << "s1 = " << s1 << " inpu

    if(which2 == 1){// раньше заканчивать
        for(int i = 0; i < 6; i++){
            if((i + 1 == t1 or i + 1 == t2  or i + 1 == t3  or i + 1 == t4 or i + 1 == t5 or i + 1 == t6) && first[i] != 7 ){
                output += (double)last[i]/42;
            }
        }
    }
    if(which2 == -1){//позже заканчивать
        for(int i = 0; i < 6; i++){
            if((i + 1 == t1 or i + 1 == t2  or i + 1 == t3  or i + 1 == t4 or i + 1 == t5 or i + 1 == t6) && first[i] != 7){
                output += 1 - (double)last[i]/6;
            }
        }
    }

    return output;
    //здесь у нас уже есть массив с количеством пар по дням, а также с тем, когда пары начинаются
}

double exeption_of_pairs(Group tempgroup, std::string pair_id){
    double loss = 0;
    //const char *s_numbers = "1343245";
    size_t len = pair_id.size();
    int days[len/2];
    int pairs[len/2];
    int s = 0;
    for(size_t i = 0; i < len; i += 2){
        days[s] = pair_id[i] - 48;
        pairs[s] = pair_id[i + 1] - 48;
        s++;
    }
    int exepts[len/2];
    for(size_t i = 0; i < len/2; i++){
        exepts[i] = 7 * days[i] + pairs[i];
    }
    for(size_t i = 0; i < len/2; i++){
        if(tempgroup.lesson_map[exepts[i]].exist == 1){
            loss += 100;
        }
    }

    //cout << sum <<std::endl;

    return loss;
}


