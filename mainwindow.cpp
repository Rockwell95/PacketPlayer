#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QIntValidator>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // ===== Create a string for a regular expression =====
  QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
  /* Create a regular expression with a string
   * as a repeating element
   */
  QRegularExpression ipRegex("^" + ipRange + "\\." + ipRange + "\\." + ipRange +
                             "\\." + ipRange + "$");
  /* Create a validation regular expression
   * using a regular expression
   */
  QRegularExpressionValidator *ipValidator =
      new QRegularExpressionValidator(ipRegex, this);
  /* Set Validator on QLineEdit */
  ui->ipAddress->setValidator(ipValidator);

  // ===== Set port number validator =====
  ui->portNumber->setValidator(new QIntValidator(0, 65535, this));

  QIcon::setThemeName("Adwaita");
  ui->playButton->setIcon(QIcon::fromTheme("media-playback-start-symbolic"));
  ui->stopButton->setIcon(QIcon::fromTheme("media-playback-stop-symbolic"));
  this->setWindowTitle(tr("Packet Player"));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_fileDialogButton_clicked() {
  QString filename = QFileDialog::getOpenFileName(
      this, tr("Find Files"), QDir::homePath(), tr("Packet Captures (*.pcap)"));

  if (filename.isEmpty())
    return;

  ui->filePath->clear();
  ui->filePath->setText(filename);
}
