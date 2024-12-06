//
// Created by winlogon on 03.11.2024.
//

#include "logic/Register.h"

void Register::registerAppForOpenWith()
{
    QString appPath = QCoreApplication::applicationFilePath();
    appPath = appPath.replace("/", "\\");

    QString command = QString("\"%1\" \"%2\"").arg(appPath).arg("%1");

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

    if (RegCreateKeyExA(HKEY_CLASSES_ROOT, appKeyPath.toUtf8().constData(), 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        RegSetValueA(hKey, NULL, REG_SZ, command.toUtf8().constData(), command.size());
        RegCloseKey(hKey);
    }

    // Добавляем  тип файла в контекстное меню "Открыть с помощью" для файлов .zip
    const char* openWithProgIdsKey = ".zip\\OpenWithProgids";
    if (RegOpenKeyExA(HKEY_CLASSES_ROOT, openWithProgIdsKey, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegSetValueA(hKey, customFileType, REG_NONE, NULL, 0);
        RegCloseKey(hKey);
    }
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
