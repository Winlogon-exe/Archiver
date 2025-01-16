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
    void addArchiveOptionToContextMenu();

private:
    const wchar_t* subParamOpen = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\Zipster.Open";
    const wchar_t* subParamAdd = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\Zipster.Add";
    const wchar_t* subParamExtend = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\Zipster.Extend";

    const wchar_t* defaultValueOpen = L"Открыть в Zipster";
    const wchar_t* defaultValueAdd = L"Добавить в Архив";
    const wchar_t* defaultValueExtend = L"Разархивировать в текущем месте";

    const wchar_t* commandKeyOpen = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\Zipster.Open\\command";
    const wchar_t* commandKeyAdd = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\Zipster.Add\\command";
    const wchar_t* commandKeyExtend = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\Zipster.Extend\\command";

private:
    std::wstring exePath = QCoreApplication::applicationFilePath().toStdWString();
    QString command = QString("\"%1\" \"%2\"").arg(exePath).arg("%1");
    std::wstring wCommand = command.toStdWString();
};

#endif //DETROIT_REGISTER_H
