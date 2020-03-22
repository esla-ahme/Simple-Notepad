#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    connect(ui->textEdit,SIGNAL(textChanged()),this,SLOT(notSaved()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::notSaved(){MainWindow::savedFlag = false;}


void MainWindow::on_actionNew_triggered()
{
    if(savedFlag){
    currentFile.clear();
    ui->textEdit->setText(QString());

    } else
    {
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
          case QMessageBox::Save:
              MainWindow::on_actionSave_triggered();
              savedFlag = true;
              MainWindow::on_actionNew_triggered();
              break;
          case QMessageBox::Discard:
              savedFlag = true;
              MainWindow::on_actionNew_triggered();
              break;
          case QMessageBox::Cancel:
              // Cancel was clicked
              break;
          default:
              // should never be reached
              break;
        }

    }
}
void MainWindow::on_actionOpen_triggered()
{
     if(savedFlag){
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");

       // An object for reading and writing files
       QFile file(fileName);

       // Store the currentFile name
       currentFile = fileName;

       // Try to open the file as a read only file if possible or display a
       // warning dialog box
       if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
           QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
           return;
       }

       // Set the title for the window to the file name
       setWindowTitle(fileName);

       // Interface for reading text
       QTextStream in(&file);

       // Copy text in the string
       QString text = in.readAll();

       // Put the text in the textEdit widget
       ui->textEdit->setText(text);

       // Close the file
       file.close();
       } else {
         QMessageBox msgBox;
         msgBox.setText("The document has been modified.");
         msgBox.setInformativeText("Do you want to save your changes?");
         msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
         msgBox.setDefaultButton(QMessageBox::Save);
         int ret = msgBox.exec();
         switch (ret) {
           case QMessageBox::Save:
               MainWindow::on_actionSave_triggered();
               savedFlag = true;
               MainWindow::on_actionOpen_triggered();
               break;
           case QMessageBox::Discard:
               savedFlag = true;
               MainWindow::on_actionOpen_triggered();
               break;
           case QMessageBox::Cancel:
               // Cancel was clicked
               break;
           default:
               // should never be reached
               break;
         }

       }

}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
 
    QFile file(fileName);
 
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
 
    currentFile = fileName;
 
    setWindowTitle(fileName);
 
    QTextStream out(&file);
 
    QString text = ui->textEdit->toPlainText();
 
    out << text;
 
    file.close();
    savedFlag =true;
    QMessageBox *mbox = new  QMessageBox;

    mbox->setWindowTitle(tr("saved"));
    mbox->setText("File has been saved successfully");
    mbox->show();
    QTimer::singleShot(5000, mbox, SLOT(hide()));
}



void MainWindow::on_actionExit_triggered()
{
    if(savedFlag){
    QApplication::quit();
    } else {

        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
          case QMessageBox::Save:
              MainWindow::on_actionSave_triggered();
              savedFlag = true;
              MainWindow::on_actionExit_triggered();
              break;
          case QMessageBox::Discard:
              savedFlag = true;
              MainWindow::on_actionExit_triggered();
              break;
          case QMessageBox::Cancel:
              // Cancel was clicked
              break;
          default:
              // should never be reached
              break;
        }


    }
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("Printer name");
    QPrintDialog pDialog(&printer,this);
    if (pDialog.exec() == QDialog::Rejected) {
       QMessageBox::warning(this,"Warning","Cannot Access Printer.");
       return;
    }
    ui->textEdit->print(&printer);
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
    savedFlag =false;
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
    savedFlag =false;
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
    savedFlag =false;
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
    savedFlag =false;
}
