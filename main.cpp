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
#include "dept.h"
#include <string.h>
#include "dept.h"
#include <iomanip>
#include "grade.h"
#include <array>
#include <random>
#include <cstdlib>
#include <ctime>

const std::string path_to_dir = "/Users/alexander/My_pandas/";
const std::string path_to_rawdata = "departments_2/";
const std::string path_to_preps = "data_hashed.csv";

std::map<std::string, std::vector<Lesson>> find_all_places(Grade, int);
Lesson find_free_spot_for_lesson(Group, std::string, std::map<std::string, std::vector<Lesson>>);

class annealing {
private:
    float temperature;
    Grade grade;
    int group;
    double old_energy;
    double new_energy;
    unsigned int step_num;
    double threshold;
    std::map<std::string, std::vector<Lesson>> possible_places;
    std::vector<std::string> lesson_map_without_none;
public:
    annealing(): temperature(0), step_num(0), threshold(1) {}
    annealing(Grade new_grade, float init_temp): temperature(init_temp), step_num(0), threshold(1) {
        auto rng = std::default_random_engine {};
        std::cout << typeid (rng).name();
        grade.group_map = new_grade.group_map;
        grade.gradeNumber = new_grade.gradeNumber;
    }

    void decrease_temp() {
        if (temperature > 20) {
            temperature -= 0.001;
        }
        else if ((temperature <= 20) && (temperature >3))
            temperature -= 0.0001;
        else
            temperature -= 0.00003;
    }

    double count_energy() {
        double total_energy = 0;
        // TODO
        return total_energy;
    }

    void run(int in_group) {
        group = in_group;
        possible_places = find_all_places(grade, group);
        std::cout << "Possible places found" << std::endl;

        for (auto x: grade.group_map[group].lesson_map) {
            if(x.subject != "")
                lesson_map_without_none.push_back(x.subject);
        }
        auto rng = std::default_random_engine {};
        std::shuffle(std::begin(lesson_map_without_none), std::end(lesson_map_without_none), rng); //shuffle lessons

        std::cout << "All lessons: " << std::endl;
        for (auto lesson: lesson_map_without_none)
            std::cout<<lesson << std::endl;

        std::cout << "Going to generate" << std::endl;
        Group tempgroup = generate_group();

        tempgroup.print();
        std::cout << std::endl << std::endl << std::endl;
        tempgroup = generate_group();
        tempgroup.print();
        /*while(temperature > 0) {
            step();
            if (step_num % 10000 == 0) {
                print();
            }
        }*/
    }

    /*void step() {
        step_num += 1;
        if (step_num == 1) {
            old_energy = count_energy();
            decrease_temp();
            return;
        }

        //cout << "----------------------------------------------------------------------" << endl;
        //cout << "i: " << i << ", j: " << j << endl;
        //cout << "----------------------------------------------------------------------" << endl;
        new_energy = count_energy();
        if (new_energy > old_energy) {
            threshold = exp(-1.0 * (new_energy - old_energy) / temperature);
            float p = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            //cout << endl << "new energy > old energy, probability: " << p << endl;
            if (p > threshold) {
                reverse(i, j);
                //cout << "p = " << p << ", threshold = " << threshold << endl;
            }
            else
                old_energy = new_energy;
        }
        else
            old_energy = new_energy;
        decrease_temp();
    }*/

    void print() {
        std::cout << "STEP: " << step_num <<" ENERGY: " << old_energy << " TEMPERATURE: " << temperature << "PROBA: " << threshold << std::endl;
    }

    Group generate_group();
};

Group annealing::generate_group() {
    srand(time(0));
    int i = rand() % lesson_map_without_none.size();
    int j = rand() % lesson_map_without_none.size();

    std::cout << "I J: " << i << " " << j << std::endl;


    std::string temp = lesson_map_without_none[i];
    lesson_map_without_none[i] = lesson_map_without_none[j];
    lesson_map_without_none[j] = temp;


    /*
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(lesson_map_without_none), std::end(lesson_map_without_none), rng); //shuffle lessons
    */

    //START FILLING TIMETABLE
    Group new_group;
    Lesson new_lesson;
    for(auto lesson: lesson_map_without_none) {
        new_lesson = find_free_spot_for_lesson(new_group, lesson, possible_places);
        if (new_lesson.subject != "") {
            new_group.lesson_map.push_back(new_lesson);
        }
        else {
            return Group();
        }
    }
    return new_group;
}



int main()
{
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

    std::map<std::string, std::vector<Lesson>> possible_places = find_all_places(new_grade, 7);
    /*for (auto it = new_grade.group_map[0].lesson_set.begin(); it != new_grade.group_map[0].lesson_set.end(); ++it) {
        std::cout<<*it<<std::endl;
    }*/
//    std::cout<<possible_places.begin()->first << std::endl;
//    std::cout<<"wtf"<<std::endl;
    //std::cout << possible_places.begin()++->second[0];
    for(const auto & it: possible_places) {
        std::cout<<"≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈"<<it.first << "≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈"<<std::endl << std::endl;
        for(const auto itt: it.second) {
            std::cout<<itt.subject << "   " << itt.lesson_id / 7 <<  "   " << itt.time << "   " << itt.professor << std::endl;
        }
        std::cout<<std::endl<<std::endl;
    }

    annealing optimizer(new_grade, 10);

    optimizer.run(7);

    return 0;
}

// FIND ALL POSSIBLE PLACES FOR ALL LESSONS
std::map<std::string, std::vector<Lesson>> find_all_places(Grade grade, int group_id) {
    std::map<std::string, std::vector<Lesson>> return_map;
    for(auto it=grade.group_map[group_id].lesson_set.begin(); it != grade.group_map[group_id].lesson_set.end(); ++it) {
        if (*it == "")
            continue;
        //all subjects of current group
        std::vector<Lesson> current;
        for(auto group: grade.group_map) {
            //all groups in grade
            for(auto lesson: group.lesson_map) {
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
Lesson find_free_spot_for_lesson(Group group, std::string lesson_name, std::map<std::string, std::vector<Lesson>> possible_places) {
    //BUSY IDS
    std::vector<int> busy_ids;
    for (auto lesson:group.lesson_map) {
        if (lesson.subject != "")
            busy_ids.push_back(lesson.lesson_id);
    }

    for(const auto & it: possible_places) {
        if(it.first == lesson_name) {
            auto rng = std::default_random_engine {};
            std::vector<Lesson> all_lessons_with_this_name = it.second;
            std::shuffle(std::begin(all_lessons_with_this_name), std::end(all_lessons_with_this_name), rng); //shuffle lessons
            for (auto lesson:all_lessons_with_this_name) {
                bool flag = true;
                for (auto id: busy_ids) {
                    if (lesson.lesson_id == id)
                        flag = false;
                }
                if (flag == true) {
                    //std::cout<<lesson_name << std::endl;
                    //lesson.print();
                    //std::cout<<lesson.lesson_id<<std::endl;
                    return lesson;
                }
            }

        }

    }
    return Lesson();
}


