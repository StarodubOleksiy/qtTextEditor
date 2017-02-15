#ifndef REPLACE_DIALOG_H
#define REPLACE_DIALOG_H

#include <QDialog>
#include <QTextDocument>
#include <QTextEdit>

namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = 0);
    ~ReplaceDialog();

signals:
    void find(const QString &str, QTextDocument::FindFlags ff);
    void replace(const QString &strFind, const QString &strReplace, QTextDocument::FindFlags ff);
    void replaceAll(const QString &strFind, const QString &strReplace, QTextDocument::FindFlags ff);


private slots:
    void on_actionFindNext_triggered();

    void on_actionReplace_triggered();

    void on_actionReplaceAll_triggered();

private:
    Ui::ReplaceDialog *ui;
};

#endif // REPLACE_DIALOG_H
