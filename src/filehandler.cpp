#include "filehandler.h"

const std::string FileHandler::extension = ".enc";

void FileHandler::getAppDataDir(){
#if defined(_WIN32)
    // Get the user's app data directory on Windows
    char* appDataDirChar = nullptr;
    size_t len = 0;
    _dupenv_s(&appDataDirChar, &len, "APPDATA");
    if (appDataDirChar != nullptr) {
        this->appDataDir = appDataDirChar;
        free(appDataDirChar);
    }
#elif defined(__APPLE__)
    // Get the user's home directory on macOS
    char* homeDirChar = nullptr;
    size_t len = 0;
    _dupenv_s(&homeDirChar, &len, "HOME");
    if (homeDirChar != nullptr) {
        std::string homeDir = homeDirChar;
        free(homeDirChar);
        this->appDataDir = homeDir + "/Library/Application Support/Pman";
    }
#elif defined(__unix__)
    // Get the user's home directory on Unix-based systems
    char* homeDirChar = std::getenv("HOME");
    if (homeDirChar != nullptr) {
        std::string homeDir = homeDirChar;
        this->appDataDir = homeDir + "/.pman";
    }
#else
    #error Unsupported platform
#endif
}


void FileHandler::createAppDataDir(){
    this->getAppDataDir();
    std::string appDataDir = this->appDataDir;
    if (!appDataDir.empty()) {
        struct stat info;
        if (stat(appDataDir.c_str(), &info) != 0 || !(info.st_mode & S_IFDIR)) {
            // The directory doesn't exist, so create it
            mkdir(appDataDir.c_str(), 0700);
        }
    }
}

void FileHandler::createAppDataFile(){
    std::filesystem::path appdatapath = this->getAppDataFilePath();
    if(!std::filesystem::exists(appdatapath)){
        std::ofstream(appdatapath.c_str());
    }
}

std::filesystem::path FileHandler::getAppDataFilePath() const noexcept{
    return std::filesystem::path(this->appDataDir) / std::filesystem::path(this->appDataName);
}

void FileHandler::resetAppData() const noexcept{
    //WORK
}

FileHandler::FileHandler(){
    this->createAppDataDir();
    this->createAppDataFile();

}

bool FileHandler::setAppSetting(std::string setting_name, std::string setting_value) const noexcept{
    /*
    APP SETTINGS
    filePath -> Path to the current encryption file
    
    */
    std::fstream file(this->getAppDataFilePath().c_str());
    std::stringstream file_content;     //stores the data of the file
    std::string line;
    int line_number = -1;
    while (std::getline(file, line)){
        line_number++;
        std::istringstream iss(line);
        std::string setting, value;
        if (!(iss >> setting >> value)){
            std::cout << "The AppDataFile is not in the right format" << std::endl; // error occured, not in right format
            std::cout << "Do you wanna reset the appData (y/n): ";
            std::string tmp;
            std::cin >> tmp;
            if(tmp == "y"){
                this->resetAppData();
            }
            return false;
        } 
        std::cout << setting << "|" << value << std::endl;  //DEBUGONLY
        if(!(setting == setting_name)){
            //setting was not set previously, we have to save it, delete if it is the setting we wanna change
            file_content << line << std::endl;
        }
    }
    //add the new setting to the file content stream
    file_content << setting_name << " " << setting_value << std::endl;

    std::ofstream write_file;
    write_file.open(this->getAppDataFilePath().c_str(), std::ofstream::out | std::ofstream::trunc);
    write_file << file_content.str();       //writes the file again with the read content and the added setting
    return true;
}

bool FileHandler::setFilePath(std::string path) noexcept{
    std::filesystem::path fp{path};
    bool exist = std::filesystem::exists(fp);
    if(exist){
        this->setAppSetting("filePath", path);
    }
    return exist;
}

std::string FileHandler::getFilePath() const noexcept{
    if(this->fileName.empty()){
        return std::string{};
    }
    return (std::filesystem::path(this->appDataDir) / (std::filesystem::path(this->fileName)));
}
