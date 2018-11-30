#include <cppcms/application.h>
#include <cppcms/service.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/applications_pool.h>
#include <iostream>
#include <cppcms/http_response.h>
#include <cppcms/http_request.h>
#include <cppcms/url_mapper.h>
#include <algorithm>

//#include "content.h"
#include "Record.h"
#include "access_file_data.h"

class hello : public cppcms::application {
public:
    hello(cppcms::service &srv) :
            cppcms::application(srv)
    {

    dispatcher().assign("",&hello::welcome,this);
    mapper().assign("");

    dispatcher().assign("/results", &hello::results, this);
    mapper().assign("results", "/results");

    dispatcher().assign("/results/(\\w+)", &hello::results_by_user, this, 1);
    mapper().assign("results_by_user", "/results/{1}");

    mapper().root("/myapp");
    }

    void results_by_user(std::string user) {
        if (request().request_method() == "GET") {
            std::vector<Record> user_results = getResultsByUserName(user);
            if (user_results.size() > 0) {
                cppcms::json::value results_json;
                results_json["links"]["self"] = "/results/" + user;
                results_json["player"]["name"] = user;
                for (unsigned int i = 0; i < user_results.size(); i++) {
                    results_json["player"]["results"][i]["number"] = user_results[i].getPosition();
                    results_json["player"]["results"][i]["score"] = user_results[i].getScore();
                }
                response().status(200);
                response().content_type("application/vnd.results.player+json");
                response().out() << results_json;
            }
            else {
                response().status(404);
            }
        }
        else {
                response().status(405);
        }
    }

    void results() {
        if (request().request_method() == "GET") {
            results_get();  
        }
        else if (request().request_method() == "POST") {
            results_post();
        }
        else {
            response().status(405);
        }       
    }

    void welcome() {
        response().out() <<
            "<h1>Welcome to service with game results</h1>\n"
            "<a href='"<< url("/results") <<"'>Top 10</a><br>\n"
            "";
    }
private:
    void results_get() {
        cppcms::json::value results_json = getJsonForGetResults();
        response().status(200);
        response().content_type("application/vnd.results.list+json");
        response().out() << results_json;
    }

    cppcms::json::value getJsonForGetResults() {
        cppcms::json::value results_json;
        results_json["links"]["self"] = "/results";
        std::vector<Record> results_list = findAll();
        //std::sort(results_list.begin(), results_list.end());
        int n = results_list.size();

        for (int i = 0; i < n; i++) {
            results_json["results"][i]["name"] = results_list[i].getName();
            results_json["results"][i]["score"] = results_list[i].getScore();
            results_json["results"][i]["links"]["self"] = "/results/" + results_list[i].getName();
        }
        return results_json;
    }

    void results_post() {
        if (request().content_type() == "application/vnd.results.player+json") {
            std::pair<void*, size_t> data = request().raw_post_data();
            if (data.second > 0) {
                std::string data_string = std::string(reinterpret_cast<char const *>(data.first), data.second);
                std::istringstream ss(data_string);
                cppcms::json::value input;
                if (!input.load(ss, true)) {
                    response().status(400, "Invalid JSON");
                }
                else {
                    try {
                        std::string res = addRecord(input);
                        if (res.compare("") == 0) {
                            response().status(400, "Invalid input");
                        }
                        else {
                            response().status(201);
                            response().location(res);                           
                        }
                    }
                    catch(cppcms::json::bad_value_cast const &e) {
                        response().status(400, "Invalid input");
                    }
                }
                
            }
            else {
                response().status(400, "No data in POST");
            }
        }
        else {
            response().status(400, "Invalid Content Type");
        }
    }

};

int main(int argc,char ** argv)
{
    try {
        cppcms::service srv(argc,argv);
        srv.applications_pool().mount(cppcms::applications_factory<hello>());
        srv.run();
    }
    catch(std::exception const &e) {
        std::cerr << e.what() << std::endl;
 }
}
