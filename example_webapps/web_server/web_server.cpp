#include <micro/app.hpp>

int main(int argc, char** argv){
    if(argc != 3) {
        std::cout << "Usage: test_app <web_root> <port>\n";
        exit(1);
    }

    micro::app app(argv[2]);
    app.set_pool_size(8);
    app.set_static_root(argv[1]);
    app.run();
}