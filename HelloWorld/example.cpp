#include <cppcms/application.h>
#include <cppcms/service.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/applications_pool.h>
#include <iostream>

#include "content.h"

class hello : public cppcms::application {
public:
    hello(cppcms::service &srv) :
            cppcms::application(srv)
    {

    dispatcher().assign("",&hello::welcome,this);
    //mapper().assign("");

    dispatcher().assign("/results", &hello::results, this);
    //mapper().assign("results", "/results");

    dispatcher().assign("/results/(\\w+)", &hello::results_by_user, this, 1);
    //mapper().assign("results_by_user", "/results/{1}");

    mapper().root("/myapp");
    }

    void results_by_user(std::string user) {
        response().out() << user;
    }

    void results() {
        //get top10
        //create jsons
        //send to client
        if (request().request_method() == "GET") {
            results_get();  
        }
        if (request().request_method() == "POST") {
            results_post();
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
        cppcms::json::value my_object;
        my_object["links"]["self"] = "/results";
        my_object["results"][0]["name"] = "Ivan Ivanov";
        my_object["results"][0]["score"] = 100;
        my_object["results"][0]["links"]["self"] = "/results/Ivan Ivanov";
        my_object["results"][1]["name"] = "Peter Petrov";
        my_object["results"][1]["score"] = 90;
        my_object["results"][1]["links"]["self"] = "/results/Peter Petrov";
        response().status(200);
        response().content_type("application/vnd.results.list+json");
        response().out() << my_object;
    }

    void results_post() {
        if (request().content_type() == "application/json") {
            std::pair<void*, size_t> data = request().raw_post_data();
            std::string data_string = std::string(reinterpret_cast<char const *>(data.first), data.second);
            response().out() << data_string;
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
