//
// Created by winlogon on 03.11.2024.
//

#include <QDir>
#include "logic/Register.h"

void Register::registerAppForOpenWith()
{
    HKEY hKey;
    const char* fileType = ".zip";
    const char* customFileType = "ZipsterFile"; // Уникальное имя для типа файла

    // Создаем ключ
    if (RegOpenKeyExA(HKEY_CLASSES_ROOT, fileType, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegSetValueA(hKey, NULL, REG_SZ, customFileType, strlen(customFileType));
        RegCloseKey(hKey);
    }

    // Создаем папку (ключ)
    QString appKeyPath = QString("Applications\\zipster.exe\\shell\\open\\command");

    if (RegCreateKeyExA(HKEY_CLASSES_ROOT, appKeyPath.toUtf8().constData(), 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS)
    {
        RegSetValueA(hKey, NULL, REG_SZ, command.toUtf8().constData(), command.size());
        RegCloseKey(hKey);
    }

    const char* openWithProgIdsKey = ".zip\\OpenWithProgids";
    if (RegOpenKeyExA(HKEY_CLASSES_ROOT, openWithProgIdsKey, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
    {
        RegSetValueA(hKey, customFileType, REG_NONE, NULL, 0);
        RegCloseKey(hKey);
    }
}

void Register::addArchiveOptionToContextMenu()
{
    HKEY key;
    const wchar_t* subKey = L"*\\shell\\Zipster";
    const wchar_t* commandsValue = L"Zipster.Open;Zipster.Add;Zipster.Extend";

    if (RegCreateKeyEx(HKEY_CLASSES_ROOT, subKey, 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &key, NULL) == ERROR_SUCCESS)
    {
        RegSetValueEx(key, L"SubCommands", 0, REG_SZ, (const BYTE*)commandsValue, (lstrlenW(commandsValue) + 1) * sizeof(wchar_t));
        RegSetValueEx(key, L"Icon", 0, REG_SZ, (const BYTE*)wIconPath.c_str(), (lstrlenW(wIconPath.c_str()) + 1) * sizeof(wchar_t));
        RegCloseKey(key);
    }

    HKEY hkeyLocal;
    if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, subParamOpen, 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hkeyLocal, NULL) == ERROR_SUCCESS)
    {
        RegSetValueEx(hkeyLocal, NULL, 0, REG_SZ, (const BYTE*)defaultValueOpen, (lstrlenW(defaultValueOpen) + 1) * sizeof(wchar_t));
        RegSetValueEx(hkeyLocal, L"Icon", 0, REG_SZ, (const BYTE*)wIconPath.c_str(), (lstrlenW(wIconPath.c_str()) + 1) * sizeof(wchar_t));

        //command
        RegCreateKeyEx(HKEY_LOCAL_MACHINE, commandKeyOpen, 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hkeyLocal, NULL);
        RegSetValueEx(hkeyLocal, NULL, 0, REG_SZ, (const BYTE*)wCommand.c_str(), (lstrlenW(wCommand.c_str()) + 1) * sizeof(wchar_t));
    }

    if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, subParamAdd, 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hkeyLocal, NULL) == ERROR_SUCCESS)
    {
        RegSetValueEx(hkeyLocal, NULL, 0, REG_SZ, (const BYTE*)defaultValueAdd, (lstrlenW(defaultValueAdd) + 1) * sizeof(wchar_t));
        RegSetValueEx(hkeyLocal, L"Icon", 0, REG_SZ, (const BYTE*)wIconPath.c_str(), (lstrlenW(wIconPath.c_str()) + 1) * sizeof(wchar_t));

        //command
        RegCreateKeyEx(HKEY_LOCAL_MACHINE, commandKeyAdd, 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hkeyLocal, NULL);
        RegSetValueEx(hkeyLocal, NULL, 0, REG_SZ, (const BYTE*)wCommand.c_str(), (lstrlenW(wCommand.c_str()) + 1) * sizeof(wchar_t));
    }

    if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, subParamExtend, 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hkeyLocal, NULL) == ERROR_SUCCESS)
    {
        RegSetValueEx(hkeyLocal, NULL, 0, REG_SZ, (const BYTE*)defaultValueExtend, (lstrlenW(defaultValueExtend) + 1) * sizeof(wchar_t));
        RegSetValueEx(hkeyLocal, L"Icon", 0, REG_SZ, (const BYTE*)wIconPath.c_str(), (lstrlenW(wIconPath.c_str()) + 1) * sizeof(wchar_t));

        //command
        RegCreateKeyEx(HKEY_LOCAL_MACHINE, commandKeyExtend, 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hkeyLocal, NULL);
        RegSetValueEx(hkeyLocal, NULL, 0, REG_SZ, (const BYTE*)wCommand.c_str(), (lstrlenW(wCommand.c_str()) + 1) * sizeof(wchar_t));
    }

    RegCloseKey(hkeyLocal);
}

bool Register::isRegisteredForOpenWith() {
    QString appKey = QString("Applications\\zipster.exe\\shell\\open\\command");

    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CLASSES_ROOT, appKey.toUtf8().constData(), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return true;
    }

    return false;
}
