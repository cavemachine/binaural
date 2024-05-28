#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

//--------------------------------

class Dialog : public QDialog {
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
    bool ValidateAll();
    QString getBinaural();
    void UpdateLabelBinaural();
    QProcess exe_process;

private slots:
    void on_btnPlay_clicked();
    void iniciou();
    void deu_erro();

    void on_btnStop_clicked();

    void on_lnFREQ_a_textEdited(const QString &arg1);

    void on_lnFREQ_b_textEdited(const QString &arg1);

private:
    Ui::Dialog *ui;

};

//--------------------------
#endif // DIALOG_H
