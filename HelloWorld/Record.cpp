#include <string>

#include "Record.h"

Record::Record() {
    this->position = 0;
    this->name = "";
    this->score = 0;
}

Record::Record(std::string name, int score) {
    this->position = 0;
    this->name = name;
    this->score = score;
}

int Record::getPosition() {
    return position;
}

std::string Record::getName() {
    return name;
}

int Record::getScore() {
    return score;
}

void Record::setPosition(int pos) {
    position = pos;
}

void Record::setName(std::string name) {
    this->name = name;
}

void Record::setScore(int score) {
    this->score = score;
}

bool Record::operator<(const Record& other) const {
    return (score > other.score);
}
