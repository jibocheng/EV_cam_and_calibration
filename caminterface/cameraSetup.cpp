#include "cameraSetup.h"


cameraSetup::cameraSetup()
{
	p_setup = new Ui_camerasetup();
	p_setup->setupUi(this);
	p_setup->camEditor->setDisabled(true);
	p_setup->serialEditor->setDisabled(true);
	p_setup->RGB_check->setEnabled(true);
	p_setup->DEPTH_check->setEnabled(true);
	p_setup->POINTCLOUD_check->setEnabled(false);
	connect(p_setup->ok, &QPushButton::clicked, this, &cameraSetup::onOkClicked);

}

cameraSetup::~cameraSetup()
{
	//if (p_setup != NULL)
	//{
	//	delete p_setup;
	//}
}

void cameraSetup::info_recevier(const QString camName, const QString serialNum)
{
	p_setup->camEditor->setText(camName);
	p_setup->serialEditor->setText(serialNum);

}



void cameraSetup::onOkClicked()
{
	if (p_setup->RGB_check->isChecked() && !p_setup->DEPTH_check->isChecked())
	{
		it = RGB;
		emit Imgtype_go(it);
	}
	else if (p_setup->DEPTH_check->isChecked() && !p_setup->RGB_check->isChecked())
	{
		it = DEPTH;
		emit Imgtype_go(it);
	}
	else if (p_setup->POINTCLOUD_check->isChecked())
	{
		it = POINTCLOUD;
		emit Imgtype_go(it);
	}
	else if (p_setup->RGB_check->isChecked() && p_setup->DEPTH_check->isChecked())
	{
		it = RGBD;
		emit Imgtype_go(it);
	}
	else
	{
		it = RGB;
		emit Imgtype_go(it);
	}
	
	close();
}