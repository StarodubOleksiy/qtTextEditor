#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMessageBox>

#include "find_dialog.h"
#include "replace_dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();
    void on_actionOpen_triggered();
    void on_actionTextChanged_triggered();
    void on_actionSave_as_triggered();
    void on_actionSave_triggered();
    void on_actionNew_triggered();
    void on_actionCursorMoved_triggered();
    void on_actionAbout_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionCut_triggered();
    void on_actionDelete_triggered();
    void on_actionFind_triggered();
    void on_actionReplace_triggered();

    // Names, which starts from on_... are interpreted by moc
    // http://www.qtforum.org/article/20685/connectslotsbyname.html
    // *"The parser iterates over the list of slot names looking
    // * for the following pattern: on_objectName_signal, where
    // * on_objectName_signal is the name of the slot, objectName is the
    // * object name and signal is the signal. For example, if you have
    // * a slot named, on_doneButton_clicked(), the parser looks for an
    // * object named doneButton, a signal named clicked and then connects
    // * the on_doneButton_clicked() slot to QButton’s signal clicked().
    // *
    // * If you follow this naming convention, you do not need to call the
    // * connect() method, nor do you need to connect the signal via the
    // * Qt UI editor in VisualStudio. connectSlotsByName will automatically
    // * make the connection for you.
    // *
    // * So, in order to get rid of the “No matching signal for…” warnings, we need
    // * to either follow this naming convention, or make sure none of our slot
    // * names begin with “on_”. "
    void onFind(const QString &str, QTextDocument::FindFlags ff);
    // The only difference from previous --- QMessageBox parent widget
    void onFindForReplace(const QString &str, QTextDocument::FindFlags ff);
    void onReplace(const QString &strFind, const QString &strReplace, QTextDocument::FindFlags ff);
    void onReplaceAll(const QString &strFind, const QString &strReplace, QTextDocument::FindFlags ff);


private:
    QString app_name;

    // На випадок, якщо користувач натиснув хрестик-закриття вікна
    void closeEvent(QCloseEvent *bar);

    void saveToFile(const QString& fileName);
    int askToSaveChanged(const QString& title);

    void setFileChangedStatus(bool fc=true);
    void setFileNewStatus(bool fc=true);
    void setFileName(const QString& fileName);

    void SetupStatusbar();

    Ui::MainWindow *ui;

    bool new_file;
    bool file_changed;

    QLabel* sbFilename, *sbFileSymbols, *sbFileCol, *sbFileRow, *sbFileChanged;

    QString curFileName;
    QString filesMask;

    QScopedPointer<FindDialog> findDialog;
    // Зараз діалог пошуку існує весь час роботи програми.
    // Інші підходи див. тут: http://stackoverflow.com/questions/7069282/qt-modeless-dialog-destruction
    // Один із підходів -- QWeakPointer +  Qt::WA_DeleteOnClose атрибут
    QScopedPointer<ReplaceDialog> replaceDialog;

};

#endif // MAINWINDOW_H

