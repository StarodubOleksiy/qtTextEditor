#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QCloseEvent>
#include <QFileInfo>
#include <QTextCursor>
#include <QTextBlock>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    new_file(true), file_changed(false),
    findDialog(new FindDialog),
    replaceDialog( new ReplaceDialog )
{
    ui->setupUi(this);


    // Стосовно іконки файлу програми -- див. http://qt-project.org/doc/qt-5/appicon.html
    setWindowIcon( QIcon(":/icons/app_icon.png") );

    app_name =  windowTitle( );

    filesMask = tr(
    "Text Files (*.txt);;"
    "C++ Files (*.cpp *.h)"
    );

    SetupStatusbar();

    connect(findDialog.data(), SIGNAL(find(const QString &, QTextDocument::FindFlags)),
            this, SLOT(onFind(const QString &,QTextDocument::FindFlags))); // Qt::UniqueConnection

    connect(replaceDialog.data(), SIGNAL(find(const QString &, QTextDocument::FindFlags)),
            this, SLOT(onFindForReplace(const QString &,QTextDocument::FindFlags)) );

    connect(replaceDialog.data(), SIGNAL(replace(const QString &, const QString &, QTextDocument::FindFlags)),
            this, SLOT(onReplace(const QString &,const QString &, QTextDocument::FindFlags)) );

    connect(replaceDialog.data(), SIGNAL(replaceAll(const QString &, const QString &, QTextDocument::FindFlags)),
            this, SLOT(onReplaceAll(const QString &,const QString &, QTextDocument::FindFlags)) );

}

void MainWindow::SetupStatusbar()
{
    //Status bar fields
    sbFilename = new QLabel("");
    statusBar()->addWidget(sbFilename);

    sbFileSymbols = new QLabel("     ");
    sbFileSymbols->setMinimumSize( sbFileSymbols->sizeHint() );
    sbFileSymbols->setAlignment(Qt::AlignCenter);
    sbFileSymbols->setToolTip(tr("Symbols in file"));
    statusBar()->addPermanentWidget(sbFileSymbols);

    statusBar()->addPermanentWidget( new QLabel(tr("C:")) );
    sbFileCol = new QLabel(tr("     "));
    sbFileCol->setMinimumSize( sbFileCol->sizeHint() );
    sbFileCol->setAlignment(Qt::AlignCenter);
    sbFileCol->setToolTip(tr("Column"));
    statusBar()->addPermanentWidget(sbFileCol);

    statusBar()->addPermanentWidget( new QLabel(tr("R:")) );
    sbFileRow = new QLabel(tr("     "));
    sbFileRow->setMinimumSize( sbFileRow->sizeHint() );
    sbFileRow->setAlignment(Qt::AlignCenter);
    sbFileRow->setToolTip(tr("Row"));
    statusBar()->addPermanentWidget(sbFileRow);

    sbFileChanged = new QLabel(tr("      "));
    sbFileChanged->setMinimumSize( sbFileChanged->sizeHint() );
    sbFileChanged->setAlignment(Qt::AlignCenter);
    sbFileChanged->setToolTip(tr("Is file changed?"));
    statusBar()->addPermanentWidget(sbFileChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//========================================================================
//=======Save and load handling===========================================
//========================================================================

int MainWindow::askToSaveChanged(const QString& title)
{
    QMessageBox quitMsg;
    quitMsg.setWindowTitle(app_name);
    quitMsg.setText(title);
    quitMsg.setStandardButtons(QMessageBox::Yes | QMessageBox::Save | QMessageBox::Cancel);
    quitMsg.setDefaultButton(QMessageBox::Cancel);
    return quitMsg.exec();
}

void MainWindow::on_actionQuit_triggered()
{
    if(file_changed)
    {
        switch( askToSaveChanged(tr("Current changes are not saved. \nDo you really want to quit?")) )
        {
        case QMessageBox::Yes:
            QApplication::instance()->quit();
            break;
        case QMessageBox::Save:
            emit ui->actionSave->triggered();
            break;
        }
    }else
    {
        QApplication::instance()->quit();
    }
}

void MainWindow::closeEvent(QCloseEvent *bar)
{
    emit ui->actionQuit->triggered();
    // If we are there --- user chose to ignore exit button.
    bar->ignore();
}


void MainWindow::on_actionOpen_triggered()
{
    if(file_changed)
    {
        switch( askToSaveChanged(tr("Current changes are not saved."
                                    "\nDo you really want to throw out them?")) )
        {
        case QMessageBox::Cancel:
            return;
            break;
        case QMessageBox::Save:
            emit ui->actionSave->triggered();
            break;
        }
    }

    setFileName(
       QFileDialog::getOpenFileName(this, tr("Open File"), QString(), filesMask)
    );

    if (!curFileName.isEmpty()) {

        QFile file(curFileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), QString(tr("Could not open file %1").arg(curFileName)));
            return;
        }
        QTextStream data(&file);
        ui->textEdit->setText(data.readAll());
        setFileNewStatus(false);
        setFileChangedStatus(false);
        file.close();
    }

}

void MainWindow::on_actionSave_as_triggered()
{
    QString newFileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(), filesMask);
    if (!newFileName.isEmpty()) {
        saveToFile(newFileName);
        setFileName(newFileName);
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (!curFileName.isEmpty() && !new_file ) {
        saveToFile(curFileName);
    }else
    {
        emit ui->actionSave_as->triggered();
    }
}

void MainWindow::saveToFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Error"), QString(tr("Could not write to file %1").arg(fileName)));
         emit ui->actionSave_as->triggered();
    } else {
        QTextStream stream(&file);
        stream << ui->textEdit->toPlainText();
        stream.flush();

        setFileNewStatus(false);
        setFileChangedStatus(false);

        file.close();
    }
}

void MainWindow::on_actionNew_triggered()
{
    //ToDo: ask once more if save cancelled.
    if(file_changed)
    {
        switch( askToSaveChanged(tr("Current changes are not saved."
                                    "\nDo you really want to throw out them?")) )
        {
        case QMessageBox::Cancel:
            return;
            break;
        case QMessageBox::Save:
            emit ui->actionSave->triggered();
            break;
        }
    }
    ui->textEdit->clear ();
    setFileNewStatus(true);
    setFileChangedStatus(false);
    setFileName(tr(""));
}

//========================================================================
//=======Current state information management=============================
//========================================================================


void MainWindow::on_actionTextChanged_triggered()
{
    setFileChangedStatus(true);
    sbFileSymbols->setNum( ui->textEdit->document()->characterCount()-1 );
}

void MainWindow::setFileChangedStatus(bool fc)
{
    if(fc)
    {
        file_changed = true;
        this->setWindowTitle(app_name + " (*)");
        sbFileChanged->setText("X");
    }else
    {
        file_changed = false;
        this->setWindowTitle(app_name);
        sbFileChanged->clear();
    }
}

void MainWindow::setFileNewStatus(bool fc)
{
    // На відміну від setFileChangedStatus, інших дій тут немає,
    // тому if буде зайвим
    new_file=fc;
}

void MainWindow::setFileName(const QString& fileName)
{
    curFileName = fileName;
    QFileInfo curFI(curFileName);
    sbFilename->setText( curFI.fileName() );
    sbFilename->setToolTip( curFI.canonicalFilePath() );
}

void MainWindow::on_actionCursorMoved_triggered()
{
    QTextCursor curCursor = ui->textEdit->textCursor () ;
    sbFileCol->setNum( curCursor.positionInBlock()+1 );
    sbFileRow->setNum( curCursor.block().firstLineNumber()+1 );
}

//========================================================================
//=======Auxiliary actions================================================
//========================================================================

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox aboutBox;
    aboutBox.setIcon(QMessageBox::Information);
    aboutBox.setWindowTitle(app_name);
    aboutBox.setText(tr("Simple text editor."));
    aboutBox.setInformativeText(tr("Developed for Qt course. \nAuthor e-mail: indrekis@gmail.com"));
    aboutBox.setStandardButtons(QMessageBox::Ok);
    aboutBox.exec();

}

//========================================================================
//=======Editing tools section============================================
//========================================================================

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionDelete_triggered()
{
    QTextCursor curCursor = ui->textEdit->textCursor ();
    curCursor.removeSelectedText();
}


void MainWindow::on_actionFind_triggered()
{
    findDialog->show();
    findDialog->activateWindow();
    // Then we call show() and activateWindow() to ensure that the window is visible
    // and active. A call to show() alone is sufficient to make a hidden window visible
    // and active, but the Find dialog may be invoked when its window is already
    // visible, in which case show() does nothing and activateWindow() is necessary to
    // make the window active.

}

void MainWindow::onFind(const QString &str, QTextDocument::FindFlags ff)
{
    if( ! ui->textEdit->find(str,ff) )
    {
         QMessageBox::warning(findDialog.data(), tr("Warning"), QString(tr("String \"%1\" not found.").arg(str)));
         // Зауважте, що предок --- findDialog а не this, інакше QMessageBox ховатиме findDialog
    }
}

void MainWindow::onFindForReplace(const QString &str, QTextDocument::FindFlags ff)
{
    if( ! ui->textEdit->find(str,ff) )
    {
         QMessageBox::warning(replaceDialog.data(), tr("Warning"), QString(tr("String \"%1\" not found.").arg(str)));
    }
}

void MainWindow::onReplace(const QString &strFind, const QString &strReplace, QTextDocument::FindFlags ff)
{
    if( ! ui->textEdit->textCursor().hasSelection() )
    {
        emit onFindForReplace(strFind, ff);
    }else
    {
        ui->textEdit->textCursor().insertText( strReplace );
        emit onFindForReplace(strFind, ff);
    }
}

void MainWindow::onReplaceAll(const QString &strFind, const QString &strReplace, QTextDocument::FindFlags ff)
{
    int currentPos = ui->textEdit->textCursor().position();

    // From the beginning
    ui->textEdit->moveCursor(QTextCursor::Start);
    int replacedN=0;
    int replaced_before_cursor=0;
    while ( ui->textEdit->find(strFind,ff) )
    {
        ui->textEdit->textCursor().insertText( strReplace );
        ++replacedN;
        if( ui->textEdit->textCursor().position() <=
                currentPos + (replaced_before_cursor+1) * ( strReplace.size() - strFind.size() )
           )
            ++replaced_before_cursor;
    }
    QMessageBox::information(replaceDialog.data(), tr("Information"), \
            QString(tr("String \"%1\" replaced %2 times.").arg(strFind)).arg(replacedN));
    // textCursor() повертає копію курсора!
    QTextCursor cc = ui->textEdit->textCursor();
    // Коректуємо на зміну довжини тексту
    currentPos += replaced_before_cursor*( strReplace.size() - strFind.size() );
    cc.setPosition( currentPos );
    ui->textEdit->setTextCursor( cc );
}

void MainWindow::on_actionReplace_triggered()
{
    replaceDialog->show();
    replaceDialog->activateWindow();
}

