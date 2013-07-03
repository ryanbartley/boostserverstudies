#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try {
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query("216.171.112.36", "daytime");
        
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::resolver::iterator end;
        
        tcp::socket socket(io_service);
        boost::system::error_code error = boost::asio::error::host_not_found;
        
        std::cout << "I'm going into the while loop" << std::endl;
        
        while (error && endpoint_iterator != end) {
            socket.close();
            socket.connect(*endpoint_iterator, error);
        }
        
        std::cout << "I'm out of the while loop" << std::endl;
        
        if (error) {
            throw boost::system::system_error(error);
        }
        
        std::cout << "I'm past the error" << std::endl;
        
        for (; ; ) {
            boost::array<char, 128> buf;
            boost::system::error_code error;
            
            size_t len = socket.read_some(boost::asio::buffer(buf), error);
            
            if (error == boost::asio::error::eof) {
                std::cout << "I have an error" << std::endl;
                break;
            } else if (error) {
                throw boost::system::system_error(error);
            }
            std::cout << "I should be writing something" << std::endl;
            std::cout.write(buf.data(), len);
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    
    return 0;
}
