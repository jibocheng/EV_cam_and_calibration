//#include "MvCameraControl.h"
//#include <iostream>
//
//int main() {
//    // 初始化SDK
//    MV_CC_DEVICE_INFO_LIST stDeviceList;
//    memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
//
//    // 枚举设备
//    int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);
//    if (MV_OK != nRet) {
//        std::cerr << "枚举设备失败！错误代码：" << nRet << std::endl;
//        return -1;
//    }
//
//    // 检查是否找到设备
//    if (stDeviceList.nDeviceNum == 0) {
//        std::cerr << "没有找到设备！" << std::endl;
//        return -1;
//    }
//
//    // 创建句柄
//    void* handle = nullptr;
//    nRet = MV_CC_CreateHandle(&handle, stDeviceList.pDeviceInfo[0]);
//    if (MV_OK != nRet) {
//        std::cerr << "创建句柄失败！错误代码：" << nRet << std::endl;
//        return -1;
//    }
//
//    // 打开设备
//    nRet = MV_CC_OpenDevice(handle);
//    if (MV_OK != nRet) {
//        std::cerr << "打开设备失败！错误代码：" << nRet << std::endl;
//        MV_CC_DestroyHandle(handle);
//        return -1;
//    }
//
//    // 设置像素格式为Mono8
//    nRet = MV_CC_SetEnumValue(handle, "PixelFormat", PixelType_Gvsp_Mono10);
//    if (MV_OK != nRet) {
//        std::cerr << "设置像素格式失败！错误代码：" << nRet << std::endl;
//    }
//    else {
//        std::cout << "像素格式设置为Mono8成功！" << std::endl;
//    }
//
//    // 关闭设备
//    MV_CC_CloseDevice(handle);
//
//    // 销毁句柄
//    MV_CC_DestroyHandle(handle);
//
//    return 0;
//}
