#ifndef DIALOG_H
#define DIALOG_H
#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

class Dialog : public QDialog
{
	Q_OBJECT
public:
	Dialog();
	~Dialog();
signals:
	void randomPoints(const int &x);
protected slots:
	void emitAndClose();
private:
	QLabel *label;
	QLineEdit *edit;
	QPushButton *okButton, *cancelButton;
	QHBoxLayout *topLayout, *bottomLayout;
	QVBoxLayout *mainLayout;
};

#endif

