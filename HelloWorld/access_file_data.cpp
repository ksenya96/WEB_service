#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <cppcms/json.h>
#include <regex>


#include "Record.h"
#include "access_file_data.h"

const char * file_name = "data.txt"; 

std::vector<Record> findAll() {
    std::vector<Record> results;
    std::ifstream fin;
    fin.open(file_name);

    if (!fin) {
        std::cerr << "Unable to open file" << std::endl;
        return results;
    }

    std::string name;
    int score;
    while (fin >> name && fin >> score) {
        results.push_back(Record(name, score));
    }
    fin.close();
    return results;
}

std::vector<Record> getResultsByUserName(std::string name) {
    std::vector<Record> records_list = findAll();
    std::sort(records_list.begin(), records_list.end());
    std::vector<Record> res;
    for (unsigned int i = 0; i < records_list.size(); i++) {
        if (records_list[i].getName().compare(name) == 0) {
            records_list[i].setPosition(i + 1);
            res.push_back(records_list[i]);
        }
    }
    return res;
}

std::string addRecord(cppcms::json::value input) {
    std::string name = input["player"]["name"].str();
    int score = input["player"]["results"][0]["score"].number();
    std::regex e("(\\w+)");
    if (std::regex_match(name, e) && score >= 0) {
        std::fstream fs;
        fs.open(file_name, std::fstream::out | std::fstream::app);
        if (!fs) {
            std::cerr << "Unable to open file" << std::endl;
            return "";
        }
        fs << name << " " << score << std::endl;
        fs.close();
        return "/results/" + name;

    }
    else {
        return "";
    }
}

