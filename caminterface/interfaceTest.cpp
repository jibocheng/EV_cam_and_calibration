#include <iostream>
#include "cameraInterface.h"
#include <MvCameraControl.h>


int main(int argc, char* argv[])
{

	QApplication app(argc,argv);
	cameraInterface caminterface;
	caminterface.show();
	return app.exec();
}