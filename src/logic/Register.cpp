//
// Created by winlogon on 03.11.2024.
//

#include "logic/Register.h"

void Register::registerAppForOpenWith()
{
    QString appPath = QCoreApplication::applicationFilePath();
    QString command = QString("\"%1\" \"%1\"").arg(appPath);

    HKEY hKey;
    const char* fileType = ".zip";
    const char* customFileType = "MyArchiverFile"; // Уникальное имя для типа файла

    //  ассоциацию расширения файла .zip с типом MyArchiverFile
    if (RegOpenKeyExA(HKEY_CLASSES_ROOT, fileType, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegSetValueA(hKey, NULL, REG_SZ, customFileType, strlen(customFileType));
        RegCloseKey(hKey);
    }

    QString appKey = QString("Applications\\%1\\shell\\open\\command").arg(appPath);
    if (RegCreateKeyExA(HKEY_CLASSES_ROOT, appKey.toUtf8().constData(), 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        RegSetValueA(hKey, NULL, REG_SZ, command.toUtf8().constData(), command.size());
        RegCloseKey(hKey);
    }
}

bool Register::isRegisteredForOpenWith() {
    QString appPath = QCoreApplication::applicationFilePath();
    QString appKey = QString("Applications\\%1\\shell\\open\\command").arg(appPath);

    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CLASSES_ROOT, appKey.toUtf8().constData(), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return false;
    }

    return true;
}
