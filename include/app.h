#pragma once
#ifndef APP_H
#define APP_H

#include <iostream>
#include "filehandler.h"

class App{
private:
    std::string filePath;
    FileHandler FH;
public:
    App();
    bool run();
    void printStart();
    std::string askForPasswd() const noexcept;
};

#endif //APP_H