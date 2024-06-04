#include "calibrationInterface.h"


int main(int argc, char* argv[])
{

	QApplication app(argc, argv);
	calibrationInterface cal_interface;
	cal_interface.show();
	return app.exec();
}