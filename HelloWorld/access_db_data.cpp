#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <cppcms/json.h>
#include <regex>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/read_concern.hpp>


#include "Record.h"
#include "access_file_data.h"

const char * file_name = "data.txt"; 

mongocxx::instance inst{};
mongocxx::client conn{mongocxx::uri{}};
mongocxx::read_concern::level local = mongocxx::read_concern::level::k_local;
mongocxx::write_concern::level wc_level = mongocxx::write_concern::level::k_majority;;
mongocxx::read_concern rc{};
mongocxx::write_concern wc{};

auto collection = conn["test"]["result"];

std::vector<Record> findAll() {
    std::vector<Record> results;   
    rc.acknowledge_level(local);
    conn.read_concern(rc);

    auto order = bsoncxx::builder::stream::document{} << "score" << -1 << bsoncxx::builder::stream::finalize;
    auto opts = mongocxx::options::find{};
    opts.sort(order.view());
    opts.limit(10);
    auto cursor = collection.find({}, opts);

    for (auto&& doc : cursor) {
//        std::cout << bsoncxx::to_json(doc) << std::endl;
        std::string name = doc.find("name")->get_utf8().value.to_string();
        int score = (int)(doc.find("score")->get_double().value);
        results.push_back(Record(name, score));
    }
    return results;
}

std::vector<Record> getResultsByUserName(std::string name) {
    rc.acknowledge_level(local);
    conn.read_concern(rc);
    auto cursor = collection.find(bsoncxx::builder::stream::document{} << "name" 
	<< name 
	<< bsoncxx::builder::stream::finalize);

    std::vector<Record> records_list;
    for (auto doc : cursor) {
        std::string name = doc.find("name")->get_utf8().value.to_string();
        int score = (int)(doc.find("score")->get_double().value);
        records_list.push_back(Record(name, score));
    }
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
    wc.acknowledge_level(wc_level);
    wc.journal(true);
    conn.write_concern(wc);
    std::string name = input["player"]["name"].str();
    int score = input["player"]["results"][0]["score"].number();
    std::regex e("(\\w+)");
    if (std::regex_match(name, e) && score >= 0) {
        bsoncxx::builder::stream::document document{};
        document << "name" << name;
        document << "score" << score;
        collection.insert_one(document.view());
        collection.delete_many(bsoncxx::builder::stream::document{} << "name" 
<< name << bsoncxx::builder::stream::finalize);
        return "/results/" + name;

    }
    else {
        return "";
    }
}
