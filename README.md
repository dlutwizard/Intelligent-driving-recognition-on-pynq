# Intelligent-driving-recognition-on-pynqz2
## 在PYNQ开发板上搭建卷积神经网络实现道路交通标志识别  
本项目为2024年AMD赛道自主选题赛道作品介绍  
  
### 项目简介   
项目主要实现方式为在ZYNQ的PL端部署了基于HLS开发的卷积加速器和池化加速器，再在PS端利用Python编程实现对加速器的调用，完成交通标志的识别。模型在Tensorflow下采用德国交通标志数据集（GTSRB）进行训练，最终精度达到97%左右。
### 文件目录
1. HLS： 包含了卷积加速器和池化加速器的的C语言代码。因为全连接层可以看作一种特殊卷积层并借助卷积加速器进行加速，因此未包含单独的全连接层加速器。
2. python for train： 包含了训练所用的Python代码和最终模型的参数文件。所采用的数据集链接为：http://benchmark.ini.rub.de/?section=gtsrb&subsection=dataset 。
3. bd:在vivado中生成的block design
4. jupyter_notebooks: 此文件夹内包含上传到PYNQ开发板上的Python代码文件和模型参数bin文件等。文件上传到开发板后可以在Jupyter Notebook中运行‘Traffic-Signs-Recognition.ipynb’文件进行测试，也可以直接运行‘Traffic-signs-recognition.py’进行测试。
### 运行环境
* 开发板： PYNQ-Z2 （ZYNQ XC7Z020-1CLG400C）
* 固件版本： V2.5
* Vivado版本： 2023.2
* Vivado HLS版本： 2023.3
* TensorFlow版本； 1.14
* 训练环境： pytroch+CUDA 12.7+python3.11
### 性能参数
* 识别种类： 43种(可基于数据集继续添加)
* 识别精度： 97% （在GTSRB下）
* 识别速度： 0.7s左右 （识别速度较慢，还需改进）
