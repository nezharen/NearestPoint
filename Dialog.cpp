#include <QtGui>
#include "Dialog.h"

Dialog::Dialog()
{
	label = new QLabel(tr("&Point number:"));
	edit = new QLineEdit;
	edit->setAlignment(Qt::AlignHCenter);
	label->setBuddy(edit);

	okButton = new QPushButton(tr("&OK"));
	okButton->setDefault(true);
	connect(okButton, SIGNAL(clicked()), this, SLOT(emitAndClose()));

	cancelButton = new QPushButton(tr("&Cancel"));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

	topLayout = new QHBoxLayout;
	topLayout->addWidget(label);
	topLayout->addWidget(edit);

	bottomLayout = new QHBoxLayout;
	bottomLayout->addWidget(okButton);
	bottomLayout->addStretch();
	bottomLayout->addWidget(cancelButton);

	mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(bottomLayout);

	setLayout(mainLayout);
	setWindowTitle(tr("Random points"));
	setFixedSize(sizeHint());
}

Dialog::~Dialog()
{
	delete label;
	delete edit;
	delete okButton;
	delete cancelButton;
	delete topLayout;
	delete bottomLayout;
	delete mainLayout;
}

void Dialog::emitAndClose()
{
	emit(randomPoints(edit->text().toInt()));
	close();
}

