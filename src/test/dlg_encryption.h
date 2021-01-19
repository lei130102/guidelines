#ifndef DLG_ENCRYPTION_H
#define DLG_ENCRYPTION_H

#include <QDialog>

namespace Ui {
	class dlg_encryption;
}

class dlg_encryption : public QDialog
{
	Q_OBJECT

public:
	explicit dlg_encryption(QWidget* parent = 0);
	~dlg_encryption();

private slots:
	void on_pbMD4Encrypt_clicked();
	void on_pbMD4Decrypt_clicked();

private:
	Ui::dlg_encryption* ui;
};


#endif //DLG_ENCRYPTION_H