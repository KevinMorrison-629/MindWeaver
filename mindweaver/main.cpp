#include "Application.h" // Assuming Application.h is in the same directory or include path

#include <iostream>
#include <stdexcept>

int main()
{
    try
    {
        MindWeaver::Application app("MindWeaver Node Editor");
        app.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Unhandled Exception: " << e.what() << std::endl;
        // You might want to display this in a platform-specific message box for release builds
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Unknown unhandled exception occurred." << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}