#include <restbed>
#include <iostream>
#include <string>

#include "json.hpp"

using namespace std;
using namespace restbed;

string to_json(float number) { 
    ostringstream str_stream;
    str_stream << number; // push the result into the stream
    nlohmann::json jsonResult = {
        {"result", str_stream.str()},
        {"data", "12345"}
    };
    return jsonResult.dump();
}

void post_method_handler( const shared_ptr< Session > session ){
    const auto request = session->get_request( );
    int content_length = request->get_header( "Content-Length", 0 );

    session->fetch( content_length, [ ]( const shared_ptr< Session > session, const Bytes & body ) {
        // body can be read here
        session->close( OK, "POSTED!", { { "Content-Length", "7" } } );
    } );
}

void close_session_with_result(const shared_ptr<Session> session, const string content) {
    session->close(OK, content, 
        {{"Content-Length", to_string(content.size())}});
}

void get_method_handler(const shared_ptr<Session> session) {
    close_session_with_result(session, to_json(98));
}

string get_circles_data_json() { 
    nlohmann::json jsonResult = {
        {"default-circle-color", "brown"},
        {"last-circle-color", "green"},
        {"greeting", "Hello from the backend!"}
    };
    return jsonResult.dump();
}

void get_circles_data(const shared_ptr<Session> session) {
    close_session_with_result(session, get_circles_data_json());
}

int main( const int, const char** ) {
    auto resource = make_shared<Resource>( );
    resource->set_path( "/resource" );
    resource->set_method_handler( "POST", post_method_handler );
    resource->set_method_handler( "GET", get_method_handler );

    auto another_resource = make_shared<Resource>( );
    another_resource->set_path( "/another_resource" );
    another_resource->set_method_handler( "GET", get_method_handler );

    auto circles_resource = make_shared<Resource>();
    circles_resource->set_path("/circles");
    circles_resource->set_method_handler("GET", get_circles_data);

    auto settings = make_shared< Settings >( );
    settings->set_port( 1984 );
    settings->set_default_header( "Connection", "close" );
    settings->set_default_header("Access-Control-Allow-Origin", "*"); // Prevents CORS errors

    Service service;
    service.publish( resource );
    service.publish(another_resource);
    service.publish(circles_resource);

    cout << "Service started";

    service.start( settings );

    return EXIT_SUCCESS;
}