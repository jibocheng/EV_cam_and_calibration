#pragma once
#include "ui_cameraSetup.h"
#include "sharedate.h"

class cameraSetup : public QWidget
{
	Q_OBJECT;
public:
	cameraSetup();
	~cameraSetup();
	void info_recevier(const QString camName, const QString serialNum);


signals:
	void Imgtype_go(const Imagetype it);
private slots:
	void onOkClicked();
	

private:
	Ui_camerasetup* p_setup = NULL;
	Imagetype it;

};

