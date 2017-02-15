#include "find_dialog.h"
#include "ui_find_dialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::on_actionFindNext_triggered()
{
    QString textToFind = ui->textToFindLineEdit->text();
    if(!textToFind.isEmpty())
    {
        QTextDocument::FindFlags ff = 0;
        ff = ui->searchBackwardCheckBox->isChecked() ? ff | QTextDocument::FindBackward : ff;
        ff = ui->caseSensitiveCheckBox->isChecked() ? ff | QTextDocument::FindCaseSensitively : ff;
        ff = ui->findWholeWordsCheckBox->isChecked() ? ff | QTextDocument::FindWholeWords : ff;
        emit find(textToFind, ff);
    }
}
