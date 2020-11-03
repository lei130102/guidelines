#ifndef DLG_QDIALOG_DERIVED_CLASS_H
#define DLG_QDIALOG_DERIVED_CLASS_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class dlg_qdialog_derived_class;
}

class dlg_qdialog_derived_class : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_qdialog_derived_class(QWidget *parent = 0);
    ~dlg_qdialog_derived_class();

private:
    QString get_qstrCaption() const;
    QString get_qstrDir() const;
    QString get_qstrFilter() const;
    QColor get_qcolorInitial() const;
    QString get_qstrTitle() const;
    QFont get_qfontInitial() const;
    QString get_qstrLabel() const;
    QString get_qstrText() const;
    int get_iValue() const;
    int get_iMin() const;
    int get_iMax() const;
    int get_iStep() const;
    double get_dValue() const;
    double get_dMin() const;
    double get_dMax() const;
    int get_iDecimals() const;
    QStringList get_qstringlistItems() const;
    int get_iCurrent() const;
    bool get_bEditable() const;
    int get_iButton0() const;
    int get_iButton1() const;
    int get_iButton2() const;
    QString get_qstrButton0Text() const;
    QString get_qstrButton1Text() const;
    QString get_qstrButton2Text() const;
    int get_iDefaultButtonNumber() const;
    int get_iEscapeButtonNumber() const;
    QMessageBox::StandardButtons get_standardbuttonButtons() const;
    QMessageBox::StandardButton get_standardbuttonDefaultButton() const;

private slots:
    void on_pbGetOpenFileName_clicked();
    void on_pbGetOpenFileNames_clicked();
    void on_pbGetExistingDirectory_clicked();
    void on_pbGetSaveFileName_clicked();
    void on_pbGetColor_clicked();
    void on_pbGetFont_clicked();
    void on_pbGetText_clicked();
    void on_pbGetInt_clicked();
    void on_pbGetDouble_clicked();
    void on_pbGetItem_clicked();
    void on_pbInformation_clicked();
    void on_pbInformationQString_clicked();
    void on_pbInformationStandardButton_clicked();
    void on_pbAbout_clicked();
    void on_pbAboutQt_clicked();

private:
    Ui::dlg_qdialog_derived_class *ui;
};

#endif // DLG_QDIALOG_DERIVED_CLASS_H
