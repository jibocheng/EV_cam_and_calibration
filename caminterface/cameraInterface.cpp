#include "cameraInterface.h"
#include <QDebug>




cameraInterface::cameraInterface(QWidget * parent):QWidget(parent)
{
	
	m_camdisplay = new Ui_camdisplay();
	m_camdisplay->setupUi(this);
	m_rs = new ManagerRealSense();
	
	connect(m_camdisplay->detect, &QPushButton::clicked, this, &cameraInterface::cameraName);
	connect(m_camdisplay->run, &QPushButton::clicked, this, &cameraInterface::cameraRun);
	connect(m_camdisplay->stop, &QPushButton::clicked, this, &cameraInterface::cameraStop);
	connect(m_camdisplay->run_all, &QPushButton::clicked, this, &cameraInterface::runAll);
	connect(m_camdisplay->stop_all, &QPushButton::clicked, this, &cameraInterface::stopAll);
	connect(m_camdisplay->CamListWidget, &QListWidget::itemDoubleClicked, this , &cameraInterface::cameraSettings);
	
	
	
	connect(m_rs, &ManagerRealSense::image_go, this, &cameraInterface::slot_image_display);
	connect(m_rs, &ManagerRealSense::imgIdx_go, this, &cameraInterface::slot_images_display);
	connect(m_rs, &ManagerRealSense::imgrgbd_go, this, &cameraInterface::slot_image_rgbd_display);
	connect(m_rs, &ManagerRealSense::imgrgbdIdx_go, this, &cameraInterface::slot_images_rgbd_display);




	// button handling:
	connect(m_camdisplay->CamListWidget, &QListWidget::itemSelectionChanged, this, &cameraInterface::updateRunButtonstate);
	m_camdisplay->run->setDisabled(true);
	m_camdisplay->run_all->setDisabled(true);
	m_camdisplay->stop->setDisabled(true);
	m_camdisplay->stop_all->setDisabled(true);
	m_camdisplay->export_2->setDisabled(true);
}

cameraInterface::~cameraInterface()
{
	if (m_camdisplay != NULL)
	{
		delete m_camdisplay;
	}

	if (m_rs != NULL)
	{

		delete m_rs;
	}
	if (p_cameraInfo_list != NULL)
	{
		delete [] p_cameraInfo_list;
	}
	if (p_itList != NULL)
	{
		delete[] p_itList;
	}
}
// button update:
void cameraInterface::updateRunButtonstate()
{
	bool hasSelection = !m_camdisplay->CamListWidget->selectedItems().isEmpty();

	m_camdisplay->run->setEnabled(hasSelection);
}


//show camerastype on the interface:
void cameraInterface::cameraName()
{
	m_camdisplay->CamListWidget->clear();
	int total_camera = m_rs->getCamNum();
	p_itList = new Imagetype[total_camera];
	if (total_camera != 0)
	{
		m_camdisplay->CamListWidget->clear();
		p_cameraInfo_list = new camera_info[total_camera];
		std::vector<std::string> cameras = m_rs->getCamNames();
		std::vector<std::string> serials = m_rs->getSerialNum();
		for (int i = 0; i < total_camera; i++)
		{
			p_cameraInfo_list[i].cameraName = QString::fromStdString(cameras[i]);
			m_camdisplay->Devices->addItem(p_cameraInfo_list[i].cameraName);
			p_cameraInfo_list[i].serialNum = QString::fromStdString(serials[i]);
			p_cameraInfo_list[i].it = RGB;
			p_itList[i] = RGB;
			p_cameraInfo_list[i].selectedNum = i;
			m_camdisplay->CamListWidget->addItem(p_cameraInfo_list[i].cameraName + " " + p_cameraInfo_list[i].serialNum);
		}
	}


	// button handling 
	m_camdisplay->detect->setDisabled(true);
	if (m_rs->getCamNum() == 1)
	{
		//m_camdisplay->run->setEnabled(true);
		//m_camdisplay->stop->setEnabled(true);
	}
	else if (m_rs->getCamNum() > 1)
	{
		//m_camdisplay->run->setEnabled(true);
		m_camdisplay->run_all->setEnabled(true);
		//m_camdisplay->stop->setEnabled(true);		
		//m_camdisplay->stop_all->setEnabled(true);
	}

}

// camera selection:
void cameraInterface::cameraSettings()
{
	//preparion:
	m_camSet = new cameraSetup();
	connect(m_camSet, &cameraSetup::Imgtype_go, this, &cameraInterface::slot_receive_it);
	int index = m_camdisplay->CamListWidget->currentRow();
	QString name = p_cameraInfo_list[index].cameraName;
	QString serialNum = p_cameraInfo_list[index].serialNum;
	m_camSet->info_recevier(name, serialNum);
	m_camSet->setAttribute(Qt::WA_DeleteOnClose);
	m_camSet->show();
}

// image type decide:
void cameraInterface::cameraRun()
{

	m_camdisplay->display->clear();
	QListWidgetItem* selectedItem = m_camdisplay->CamListWidget->currentItem();


	if (selectedItem->isSelected())
	{
		int index = m_camdisplay->CamListWidget->currentRow();
		Imagetype it = p_cameraInfo_list[index].it;
		
		m_rs->setup(it, index);
		m_rs->run(index,it);
		
	}
	//button handling
	m_camdisplay->CamListWidget->setDisabled(true);
	m_camdisplay->stop->setEnabled(true);
	m_camdisplay->stop_all->setDisabled(true);
	m_camdisplay->export_2->setEnabled(true);
	m_camdisplay->run->setDisabled(true);
	m_camdisplay->run_all->setDisabled(true);

}

void cameraInterface::cameraStop()
{
	int num = m_camdisplay->CamListWidget->currentRow();
	m_rs->stop(num);
	//m_camdisplay->CamListWidget->clear();

	//buttonhandling 
	m_camdisplay->detect->setEnabled(true);
	m_camdisplay->run->setEnabled(true);
	m_camdisplay->run_all->setEnabled(true);
	m_camdisplay->export_2->setDisabled(true);
	m_camdisplay->CamListWidget->setEnabled(true);
	m_camdisplay->stop->setDisabled(true);
	m_camdisplay->stop_all->setDisabled(true);

}

void cameraInterface::slot_receive_it(const Imagetype it_set)
{
	QListWidgetItem* selectedItem = m_camdisplay->CamListWidget->currentItem();
	if (selectedItem->isSelected())
	{
		int index = m_camdisplay->CamListWidget->currentRow();
		p_cameraInfo_list[index].it = it_set;
	}
	else
	{
		return;
	}
}

void cameraInterface::slot_image_display(const QImage& Img2dis)
{
	auto start = std::chrono::high_resolution_clock::now();
	QImage pImgDis = (Img2dis).scaled(m_camdisplay->display->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation); 
	m_camdisplay->display->setPixmap(QPixmap::fromImage(pImgDis));
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	qDebug() << "Operation took " << duration.count() << " milliseconds.\n";

}

void cameraInterface::slot_image_rgbd_display(const QImage& RGB, const QImage& D)
{
	int imgWidth = m_camdisplay->display->width() / 2;
	int imgHeight = m_camdisplay->display->height();

	if (compositeImage.size() != m_camdisplay->display->size())
	{
		compositeImage = QImage(m_camdisplay->display->size(), QImage::Format_RGB888);
		compositeImage.fill(Qt::black);
	}

	QPainter painter(&compositeImage);
	QImage scaledRGB = RGB.scaled(imgWidth, imgHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QImage scaledD = D.scaled(imgWidth, imgHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

	painter.drawImage(0, 0, scaledRGB);
	painter.drawImage(imgWidth, 0, scaledD);

	m_camdisplay->display->setPixmap(QPixmap::fromImage(compositeImage));
}

void cameraInterface::slot_images_display(const QImage& Img2dis, const int imgIdx)
{
	const int rows = m_rs->getCamNum(); 
	const int cols = 1; 

	int imgWidth = m_camdisplay->display->width() / cols;
	int imgHeight = m_camdisplay->display->height() / rows;

	if (compositeImage.size() != m_camdisplay->display->size())
	{
		compositeImage = QImage(m_camdisplay->display->size(), QImage::Format_RGB888);
		compositeImage.fill(Qt::black);
	}

	QPainter painter(&compositeImage);
	int i_cols = imgIdx * imgHeight;
	QImage scaled = Img2dis.scaled(imgWidth, imgHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	int y = imgIdx * imgHeight;
	painter.drawImage(0, y, scaled);
	m_camdisplay->display->setPixmap(QPixmap::fromImage(compositeImage));
}

void cameraInterface::slot_images_rgbd_display(const QImage& RGB, const QImage& D, const int imgIdx)
{
	const int rows = m_rs->getCamNum();
	const int cols = 2;
	int imgWidth = m_camdisplay->display->width() / cols;
	int imgHeight = m_camdisplay->display->height() / rows;

	if (compositeImage.size() != m_camdisplay->display->size())
	{
		compositeImage = QImage(m_camdisplay->display->size(), QImage::Format_RGB888);
		compositeImage.fill(Qt::black);
	}
	QPainter painter(&compositeImage);
	if (imgIdx < rows)
	{
		QImage scaledRGB = RGB.scaled(imgWidth, imgHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QImage scaledD = D.scaled(imgWidth, imgHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

		int y = imgIdx * imgHeight; 
		painter.drawImage(0, y, scaledRGB);
		painter.drawImage(imgWidth, y, scaledD); 
	}

	m_camdisplay->display->setPixmap(QPixmap::fromImage(compositeImage));
}


void cameraInterface::runAll()
{

	m_camdisplay->display->clear();
	int camNum = m_rs->getCamNum();
	for (int i = 0; i < camNum; i++)
	{
		p_itList[i] = p_cameraInfo_list[i].it;
	}
	m_rs->setupAll(p_itList, camNum);
	m_rs->run_all(p_itList, camNum);
	
	//button handling
	m_camdisplay->stop->setDisabled(true);
	m_camdisplay->stop_all->setEnabled(true);
	m_camdisplay->export_2->setEnabled(true);
	m_camdisplay->run->setDisabled(true);
	m_camdisplay->run_all->setDisabled(true);
	m_camdisplay->detect->setDisabled(true);
}

void cameraInterface::stopAll()
{
	int camNum = m_rs->getCamNum();
	m_rs->stop_all(p_itList,camNum);
	m_camdisplay->detect->setEnabled(true);
	m_camdisplay->run->setEnabled(true);
	m_camdisplay->run_all->setEnabled(true);
}


