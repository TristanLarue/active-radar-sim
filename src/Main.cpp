#include <exception>
#include <iostream>

int RunVizualisation(int argc, char** argv);

int main(int argc, char** argv) {
    try {
        return RunVizualisation(argc, argv);
    } catch(const std::exception& error) {
        std::cerr << "Failed to start visualisation: " << error.what() << '\n';
    } catch(...) {
        std::cerr << "Failed to start visualisation due to an unknown error.\n";
    }

    return 1;
}
