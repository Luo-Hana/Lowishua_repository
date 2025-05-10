# 项目介绍
  本项目实现了基于Arduino开发板的蓝牙和指纹识别开锁系统。

# 使用说明

必要的Arduino库文件：

    LiquidCrystal_I2C.h
  
    SoftwareSerial.h 
  
    Wire.h
  
    Adafruit_Fingerprint.h
  
必要开发板：
  
    Arduino UNO或其他兼容的开发板
  
引脚位置：
  
    蓝牙串口通信引脚 RX 0 TX 1
    pin8作为机械锁的控制串口
    指纹模块软串口通信引脚 RX 2 TX 3
    超声波模块引脚 TrigPin=7 EchoPin=6
    
 参数：
 
      finger.confidence 系统函数库内设定的置信值 可调


  
