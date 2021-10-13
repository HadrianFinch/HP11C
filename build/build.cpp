/*
    🏗️Build🏗️ with --- cl.exe /W0 /Zi /EHsc /nologo /Fe: .\build\build.exe .\build\build.cpp ---

    💻Run💻 with --- build.exe "BuildPreferencesFile" ---
*/

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
    string buildFilePath = argv[argc - 1];

    if (argc == 1)
    {
        cout << "Please pass a buildfile.";
        return 1;
    }

    string buildFolder = buildFilePath;
    {
        int lastSlash = buildFolder.find_last_of('\\');
        buildFolder = buildFolder.substr(0, lastSlash); 
    }
    
    cout << "Build file: " << buildFilePath << "\n";
    cout << "Build Directory: " << buildFolder << "\n \n";
    
    {
        string command = "cd ";
        command += buildFolder;
        system(command.c_str());
    }
    
    vector<string> includes;
    includes.push_back(buildFolder);
    vector<string> defines;
    vector<string> source;
    string resource;
    string manifest;
    string output;


    fstream fs(buildFilePath);
    if (!fs.good())
    {
        cout << "Invalid Build File.";
        return 1;
    }
    
    while (!fs.eof())
    {
        string line;
        getline(fs, line);

        if (line[line.size() - 1] == ':')
        {
            // Get the spec
            string spec = line.substr(0, line.size() - 1);
            // cout << "Specifier Found!\n";
            // cout << "spec: " << spec << "\n";

            string value;
            getline(fs, value);

            // Go through all of the values for the spec
            while ((value != "\n") and (!fs.eof()) and (value != ""))
            {
                // cout << "value: " << value << "\n";

                if (spec == "Output")
                {
                    output = value;
                }
                else if (spec == "Source")
                {
                    source.push_back(value);
                }
                else if (spec == "Resource")
                {
                    resource = value;
                }
                else if (spec == "Define")
                {
                    defines.push_back(value);
                }
                else if (spec == "Include")
                {
                    includes.push_back(value);
                }
                else if (spec == "Manifest")
                {
                    manifest = value;
                }
                
                getline(fs, value);
            }
            
            // cout << "\n";
        }
        
        // cout << line << "\n";
    }
    
    string buildCommand = "cl.exe /W0 /Zi /EHsc /nologo ";
    bool err = false;

    // Defines
    for (string& define : defines)
    {
        buildCommand += "/D";
        buildCommand += define;
        buildCommand += " ";
    }

    // Includes
    for (string& file : includes)
    {
        buildCommand += "/I";
        buildCommand += file;
        buildCommand += " ";
    }

    buildCommand += "/Fe: ";

    // Output
    if (!output.empty())
    {
        buildCommand += output;
        buildCommand += " ";
    }
    else
    {
        cout << "\nERROR! No output specified!\n";
        err = true;
    }

    // Source Files
    if (source.empty())
    {
        cout << "\nERROR! No source files specified!\n";
        err = true;
    }
    else
    {
        for (string& file : source)
        {
            buildCommand += file;
            buildCommand += " ";
        }
    }

    // Resource file
    if (!resource.empty())
    {
        string command = "rc ";
        command += resource;
        cout << "Building Resource File: " << resource << "\n";
        cout << "Resources Build Command: " << command << "\n";
        system(command.c_str());

        resource = resource.substr(0, resource.size() - 3);
        resource += ".res";
        buildCommand += " /link /SUBSYSTEM:WINDOWS ";
        buildCommand += resource;
    }

    // Check Errors
    if (!err)
    {
        cout << "Build Command: \n" << buildCommand << "\n\n";

        system(buildCommand.c_str());
    }
    else
    {
        cout << "Cannot build with errors, \nplease fix buildscript errors before continuing.\n";
        return 2;
    }

    // Manifest file
    if (!manifest.empty())
    {
        cout << "\n";

        string command = "mt.exe -manifest ";
        command += manifest;
        command += " -outputresource:";
        command += output;
        command += ";1";
        cout << "Ataching Manifest File: " << manifest << "\n";
        cout << "Manifest Attach Command: " << command << "\n";
        system(command.c_str());
    }
}
