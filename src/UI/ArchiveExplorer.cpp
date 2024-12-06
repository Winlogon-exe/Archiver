//
// Created by winlogon on 19.08.2024.
//
#include "ui/ArchiveExplorer.h"

ArchiveExplorer::ArchiveExplorer(QWidget *parent): QWidget(parent)
{}

void ArchiveExplorer::openArchiveExplorer(const QString &file)
{
    InitUI();
}

void ArchiveExplorer::InitUI()
{
    // Устанавливаем минимальный размер окна
    this->adjustSize();
    this->setMinimumSize(800, 600);

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
}

void ArchiveExplorer::setupConnect()
{
}

ArchiveExplorer::~ArchiveExplorer()
{

}
