#ifndef DIALOGIMPL_H
#define DIALOGIMPL_H
//
#include <QDialog>
#include "ui_dialog.h"
//
#include "qrapidsharedownload.h"
#include "debugutils.h"
#include "qrapidshareuser.h"
class DialogImpl : public QDialog, public Ui::Dialog
{
Q_OBJECT
public:
	DialogImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
private:
	QRapidshareDownload				m_RapidShareDownload;	
private slots:
	void downloadPressed();	
	void ValueChanged(const int & value);
	void ChangeProgressName(const QString & what);
};
#endif


