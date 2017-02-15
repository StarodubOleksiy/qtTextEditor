#ifndef FIND_DIALOG_H
#define FIND_DIALOG_H

#include <QDialog>
#include <QTextDocument>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = 0);
    ~FindDialog();

signals:
    void find(const QString &str, QTextDocument::FindFlags ff);

private slots:
    void on_actionFindNext_triggered();

private:
    Ui::FindDialog *ui;
};

#endif // FIND_DIALOG_H
