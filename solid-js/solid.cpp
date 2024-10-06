#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>


bool createDirectory(const std::string &name) {

    if (name.empty())
        throw std::invalid_argument("Directory path cannot be empty");

    // 777 is the permission for the directory: can read, write, and execute by owner, group, and others.
    if (mkdir(name.c_str(), 0777) == -1) {

        throw std::runtime_error(  (errno == EEXIST) ?
                  name + " directory already exists" :
         "Error creating the " + name + " directory" );
    }
    return true;
}


bool createFile(const std::string &name, const std::string &content) {

    if (name.empty())
        throw std::invalid_argument("File name cannot be empty");

    std::ofstream file(name);

    if (!file.is_open())
        throw std::runtime_error("Error creating file");


    file << content;
    file.close();
    return true;
}


int main(int argc, char *argv[]) {

    try {
        int opt;
        std::string component_name;

        while((opt = getopt(argc, argv, "c:")) !=-1) {
            switch(opt) {
                case 'c':
                    component_name = optarg;
                    break;

                default:
                    throw std::invalid_argument("Usage: " + std::string(argv[0]) + " -c <component_name>");
            }
        }

        if (component_name.empty())
            throw std::invalid_argument("option -c is required");

        const std::string directory_path = component_name;
        const std::string css_path       = directory_path + '/' + component_name + ".css";
        const std::string tsx_path       = directory_path + '/' + component_name + ".tsx";

        const std::string css_content = "\n";
        const std::string tsx_content = (
            "import './" + component_name + ".css';\n"
            "\n"
            "export default function " + component_name + "() {\n"
            "    return \n"
            "}\n"
        );

        createDirectory(directory_path);
        createFile(css_path, css_content);
        createFile(tsx_path, tsx_content);

        std::cout << "Component " << component_name << " created successfully" << std::endl;

        return EXIT_SUCCESS;
    }
    catch (const std::exception &error) {
        std::cerr << error.what() << std::endl;
        return EXIT_FAILURE;
    }
}