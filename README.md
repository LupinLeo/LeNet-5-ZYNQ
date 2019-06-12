# LeNet-5 implemented in ZYNQ

 &nbsp;
## Introduction  
![](https://github.com/LupinLeo/LeNet-5-in-zynq/blob/master/image/LeNetmodule.bmp)
本项目是基于Xlinx的ZYNQ7000系列，实现了卷积神经网络（CNN）中经典的LeNet-5网络。本项目的初衷是在ZYNQ上用C++拉通一个小卷积网络，所以性能不高，对于MNIST的32x32输入，只能10帧。（高性能参数可配置的大型卷积网络（VGG16），还是需要用verilog实现，且板子有DDR4最佳）

* 卷积层和池化层的编写使用了C++在Vivado HLS工具上实现
* 全连接层则在ARM中实现（减小FPGA资源使用）
* 卷积和池化层是固定大小参数。如果改变需要重新更改代码
* 网络的模型和参数是基于caffe实例中的mnist
* 输入数据也是MNIST里的images和labels
* 所有代码经过板上验证，我使用的是ZYNQ7100，ZYNQ7000系列应该没问题
* 使用了窗口缓冲和行缓冲的优化

## Envoirment  
* OS: Windows 7
* Vivado 2018.2
* Vivado HLS
* Xlinx SDK

## Usage
* data中的bin文件是根据deploy.prototxt和lenet_iter_10000.caffemodel产生，可用作HLS部分测试用。dat文件源自bin文件，应和MNIST_data里的文件存放入板子的SD卡内
* HLS_src内是网络中卷积和池化的代码，需要根据自己的板子建立HLS工程，用此文件夹内代码生成IP核。test内是HLS测试代码。
* SDK_src内是ARM部分的代码，在Vivado中根据自己的板子，用生成的IP核搭建系统并生成bit流文件后，lauch SDK创建裸机工程使用
### Notice
* SDK里需要设置栈的大小，不然由于数组过大，栈空间不够而报错
* SDK内需要在bsp setting内导入xilffs，才能从sd卡内读文件
## System Model


![](https://github.com/LupinLeo/LeNet-5-in-zynq/blob/master/image/LeNet-5%E7%B3%BB%E7%BB%9F.bmp)

