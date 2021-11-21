#include <iostream>
#include "engine.h"


int main(int argc, char* argv[]) {
    Engine engine = Engine(argc, argv);
    while (!engine.quit) {
        engine.update();
        engine.render();
    }
    return 0;
    
}