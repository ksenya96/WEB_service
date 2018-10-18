#pragma once

#include <string>

class Record {
    private:
        int position;
        std::string name;
        int score;
    public:
        Record();
        Record(std::string name, int score);
        int getPosition();
        std::string getName();
        int getScore();
        void setPosition(int pos);
        void setName(std::string name);
        void setScore(int score);
        bool operator<(const Record& other) const;
};
