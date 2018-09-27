#include <cppcms/application.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/applications_pool.h>
#include <iostream>
#include <stdlib.h>

class hello : public cppcms::application {
public:
    hello(cppcms::service &srv) :
            cppcms::application(srv)
    {

    dispatcher().assign("",&hello::welcome,this);
    mapper().assign("");

    mapper().root("/hello");
    }
   
    void welcome() {
        response().out() <<
            "<html>\n"
	    "<body>\n"
	    "<h1>Hello, World!</h1>"
	    "</body>\n"
	    "</html>\n";
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
