//
// Created by winlogon on 06.08.2024.
//

#include "logic/MainFormLogic.h"

MainFormLogic::MainFormLogic() {
    InitializeMapButton();
}

void MainFormLogic::ButtonState(QObject* sender, ButtonsState state) {
    buttonStateMap[sender] = state;
}

//сигнал из логики должен идти только если он обновляет UI, то есть если логика не требует полей из ui то выполняется здесь
void MainFormLogic::InitializeMapButton() {
    logicMap[ARCHIVE_File]  = [this]()  { emit ArchiveFileButton(); };
    logicMap[EXTRACT_File]  = [this]()  { };
    logicMap[Path_LineEdit] = [this]()  { emit UpdateFileSystem();};
}

void MainFormLogic::ProcessState(QObject *sender) {
    ButtonsState state = buttonStateMap[sender];
    auto it = logicMap.find(state);

    if (it != logicMap.end())
    {
        it->second();
    }
}

void MainFormLogic::ArchiveFile(const QString &path) {

}

void MainFormLogic::CompressFile(const QString &filePath, const QString &outputFilePath) {

}

void MainFormLogic::CompressFolder(const QString &folderPath, const QString &outputFolderPath) {

}

void MainFormLogic::CompressData(const QByteArray &data, QByteArray &compressedData) {

}

void MainFormLogic::CreateZipArchive(const QString &zipFilePath, const QString &fileName,
                                     const std::vector<unsigned char> &data) {


}
