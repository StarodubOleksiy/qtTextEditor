#include "replace_dialog.h"
#include "ui_replace_dialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}


void ReplaceDialog::on_actionFindNext_triggered()
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

void ReplaceDialog::on_actionReplace_triggered()
{
    if(!ui->textToFindLineEdit->text().isEmpty())
    {
        QTextDocument::FindFlags ff = 0;
        ff = ui->searchBackwardCheckBox->isChecked() ? ff | QTextDocument::FindBackward : ff;
        ff = ui->caseSensitiveCheckBox->isChecked() ? ff | QTextDocument::FindCaseSensitively : ff;
        ff = ui->findWholeWordsCheckBox->isChecked() ? ff | QTextDocument::FindWholeWords : ff;

        emit replace(ui->textToFindLineEdit->text(), ui->replaceToLineEdit->text(), ff);
    }
}

void ReplaceDialog::on_actionReplaceAll_triggered()
{
    if(!ui->textToFindLineEdit->text().isEmpty())
    {
        QTextDocument::FindFlags ff = 0;
        // ! Тільки вперед шукаємо --- від початку до кінця
        // ff = ui->searchBackwardCheckBox->isChecked() ? ff | QTextDocument::FindBackward : ff;
        ff = ui->caseSensitiveCheckBox->isChecked() ? ff | QTextDocument::FindCaseSensitively : ff;
        ff = ui->findWholeWordsCheckBox->isChecked() ? ff | QTextDocument::FindWholeWords : ff;

        emit replaceAll(ui->textToFindLineEdit->text(), ui->replaceToLineEdit->text(), ff);
    }
}
