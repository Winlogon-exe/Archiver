//
// Created by winlogon on 03.11.2024.
//

#ifndef DETROIT_REGISTER_H
#define DETROIT_REGISTER_H

#include "windows.h"
#include "QString.h"
#include "QCoreApplication.h"

class Register{

public:
    void registerAppForOpenWith();
    bool isRegisteredForOpenWith();
};

#endif //DETROIT_REGISTER_H
