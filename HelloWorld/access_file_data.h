#pragma once

#include <vector>


#include "Record.h"

std::vector<Record> findAll();
std::vector<Record> getResultsByUserName(std::string name);
std::string addRecord(cppcms::json::value input);
