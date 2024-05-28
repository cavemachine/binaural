#include "dialog.h"
#include <QStringList>
#include <QValidator>
#include <QDebug>
#include "./ui_dialog.h"

Dialog::Dialog(QWidget *parent) : QDialog(parent)
{
    ui = new Ui::Dialog;
    ui->setupUi(this);
    connect(&exe_process, SIGNAL(started()), SLOT(iniciou()));
    connect(&exe_process, SIGNAL(errorOccurred(QProcess::ProcessError)), SLOT(deu_erro()));
    UpdateLabelBinaural();

}

Dialog::~Dialog() {
    delete ui;
}

bool Dialog::ValidateAll() {
    QDoubleValidator *val = new QDoubleValidator(0,20000,2) ;
    //qSetRealNumberPrecision(1);
    //val.decimals();
    //val.setRange(0, 20000);
    ui->lnFREQ_a->setValidator(val);
    ui->lnFREQ_b->setValidator(val);

    if ((ui->lnFREQ_a->hasAcceptableInput() == true) && (ui->lnFREQ_b->hasAcceptableInput() == true)){
        ui->statusBox->clear();
        return true;
    } else {
        ui->statusBox->setPlainText("Invalid input.");
        ui->labelBinaural->setText("Binaural Frequency (Hz): ---");
        return false;
    }
}

QString Dialog::getBinaural() {
    float binaural = ui->lnFREQ_a->text().toFloat() - ui->lnFREQ_b->text().toFloat();
    return QString::number(abs(binaural), 'f', 2);
}

void Dialog::UpdateLabelBinaural() {
    if (ValidateAll()) {
        QString ss = "Binaural Frequency (Hz): ";
        ss.append(getBinaural());
        ui->labelBinaural->setText(ss);
    }
}


void Dialog::on_btnPlay_clicked()
{
    if (ValidateAll() == false) {
       // ui->caixaTexto->setPlainText("Invalid frequency input.");
        return;
    }

    QStringList all_args;
    all_args << "-autoexit" \
             << "-nodisp" \
             << "-f" \
             << "lavfi" \
             << "-i" \
             << "sine=f=$FREQ_A[0];sine=f=$FREQ_B[1];[0][1]amerge=inputs=2"; //,atrim=end=2

    all_args.replaceInStrings("$FREQ_A",ui->lnFREQ_a->text());
    all_args.replaceInStrings("$FREQ_B",ui->lnFREQ_b->text());
    exe_process.start("ffplay", all_args);

    //exe_process.waitForFinished();
    //exe_process.closeWriteChannel();

    QByteArray result = exe_process.readAll();
    QString msg = "Playing... \nLeft channel: $LEFT \nRight channel: $RIGHT\n\nBinaural frequency: $BINAURAL";
    msg.replace("$LEFT",ui->lnFREQ_a->text());
    msg.replace("$RIGHT",ui->lnFREQ_b->text());
    msg.replace("$BINAURAL", getBinaural());
    ui->statusBox->setPlainText(msg);
}

void Dialog::iniciou(){
    qDebug() << "Playing...";
}

void Dialog::deu_erro(){
    qDebug() << "Error";
    qDebug() << exe_process.error();
}

void Dialog::on_btnStop_clicked() {
    exe_process.terminate();
    ui->statusBox->setPlainText("Stopped");
}

void Dialog::on_lnFREQ_a_textEdited(const QString &arg1) {
    UpdateLabelBinaural();
}

void Dialog::on_lnFREQ_b_textEdited(const QString &arg1) {
    UpdateLabelBinaural();
}

