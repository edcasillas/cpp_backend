#include <sqlite3.h>
#include <restbed>
#include <iostream>
#include <string>

#include "json.hpp"

using namespace std;
using namespace restbed;

void post_method_handler( const shared_ptr< Session > session )
{
    const auto request = session->get_request( );

    int content_length = request->get_header( "Content-Length", 0 );

    session->fetch( content_length, [ ]( const shared_ptr< Session > session, const Bytes & body )
    {
        fprintf( stdout, "%.*s\n", ( int ) body.size( ), body.data( ) );
        session->close( OK, "Hello, World!", { { "Content-Length", "13" } } );
    } );
}

string to_json(float number) { 
    ostringstream str_stream;
    str_stream << number; // push the result into the stream
    nlohmann::json jsonResult = {
        {"result", str_stream.str()}
    };
    return jsonResult.dump();
}

void get_method_handler(const shared_ptr<Session> session) {
   auto content = to_json(98);
   session->close(OK, content, 
        {{"Content-Length", to_string(content.size())}});
}

int main( const int, const char** )
{
    auto resource = make_shared< Resource >( );
    resource->set_path( "/resource" );
    resource->set_method_handler( "POST", post_method_handler );
    resource->set_method_handler( "GET", get_method_handler );

    auto another_resource = make_shared< Resource >( );
    another_resource->set_path( "/another_resource" );
    another_resource->set_method_handler( "GET", get_method_handler );

    auto settings = make_shared< Settings >( );
    settings->set_port( 1984 );
    settings->set_default_header( "Connection", "close" );
    settings->set_default_header("Access-Control-Allow-Origin", "*"); // Prevents CORS errors

    Service service;
    service.publish( resource );
    service.publish(another_resource);

    cout << "Service started";

    service.start( settings );

    return EXIT_SUCCESS;
}