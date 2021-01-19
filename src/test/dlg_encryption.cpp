#include "dlg_encryption.h"
#include "ui_dlg_encryption.h"

#include <QString>
#include <QStringList>
#include <QListWidget>
#include <QObject>
#include <QCryptographicHash>

dlg_encryption::dlg_encryption(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::dlg_encryption)
{
	ui->setupUi(this);
}

dlg_encryption::~dlg_encryption()
{
	delete ui;
}

void dlg_encryption::on_pbMD4Encrypt_clicked()
{
	QString qstring1 = ui->cbQString1->currentText();

	//QCryptographicHash::Md4        0		        Generate an MD4 hash sum
	//QCryptographicHash::Md5        1		        Generate an MD5 hash sum
	//QCryptographicHash::Sha1       2		        Generate an SHA - 1 hash sum
	//QCryptographicHash::Sha224     3		        Generate an SHA - 224 hash sum(SHA - 2).Introduced in Qt 5.0
	//QCryptographicHash::Sha256     4		        Generate an SHA - 256 hash sum(SHA - 2).Introduced in Qt 5.0
	//QCryptographicHash::Sha384     5		        Generate an SHA - 384 hash sum(SHA - 2).Introduced in Qt 5.0
	//QCryptographicHash::Sha512     6		        Generate an SHA - 512 hash sum(SHA - 2).Introduced in Qt 5.0
	//QCryptographicHash::Sha3_224   RealSha3_224     Generate an SHA3 - 224 hash sum.Introduced in Qt 5.1
	//QCryptographicHash::Sha3_256   RealSha3_256     Generate an SHA3 - 256 hash sum.Introduced in Qt 5.1
	//QCryptographicHash::Sha3_384   RealSha3_384     Generate an SHA3 - 384 hash sum.Introduced in Qt 5.1
	//QCryptographicHash::Sha3_512   RealSha3_512     Generate an SHA3 - 512 hash sum.Introduced in Qt 5.1
	//QCryptographicHash::Keccak_224 7                Generate a Keccak - 224 hash sum.Introduced in Qt 5.9.2
	//QCryptographicHash::Keccak_256 8                Generate a Keccak - 256 hash sum.Introduced in Qt 5.9.2
	//QCryptographicHash::Keccak_384 9                Generate a Keccak - 384 hash sum.Introduced in Qt 5.9.2
	//QCryptographicHash::Keccak_512 10               Generate a Keccak - 512 hash sum.Introduced in Qt 5.9.2

	QByteArray input = qstring1.toLatin1();
	QByteArray resultBA = QCryptographicHash::hash(input, QCryptographicHash::Md4);
	QString result = resultBA.toHex();

	ui->leMD4EncryptResult->setText(result);
}

void dlg_encryption::on_pbMD4Decrypt_clicked()
{
	//MD4可以解密，待补充
}
