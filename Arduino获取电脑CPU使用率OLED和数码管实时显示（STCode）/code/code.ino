#include <Wire.h> 
#include <Adafruit_GFX.h>        //OLED头文件
#include <Adafruit_SSD1306.h>
#include "TM1637.h"              //数码管头文件
#define CLK 2                    //数码管CLK接口
#define DIO 3                    //数码管DIO接口
TM1637 tm1637(CLK,DIO);         //创建一个数码管对象
#define OLED_RESET     4
Adafruit_SSD1306 display(128, 64, &Wire,OLED_RESET);

int RX_Count = 0;
char CPU_Data[10] = {0};
char RX_Buffer[200] = {0};
char *Head, *Tail;
int USE_CPU;
void setup()
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.setTextColor(WHITE);//开像素点发光
  display.clearDisplay();//清屏
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
}

void loop()
{
  while (Serial.available())
  {
    RX_Buffer[RX_Count++] = Serial.read();
    if (RX_Count >= 200)
    {
      RX_Count = 0;
      memset(RX_Buffer, 0, 200);
    }
  }
  if ((Head = strchr(RX_Buffer, 'S')) != NULL)
  {
    if ((Tail = strchr(RX_Buffer, 'E')) != NULL)
    {
      if (Tail > Head)
      {
        RX_Count = 0;
        memset(CPU_Data, 0, 10);
        memcpy(CPU_Data, Head + 2, (Tail - Head - 2));
        memset(RX_Buffer, 0, 200);
        USE_CPU = atoi(CPU_Data);       //数据类型转换
        //Serial.println(CPU_Data);
        //Serial.println(USE_CPU);   
      }
    }
  }
  display.setTextSize(4); //设置字体大小  
  display.setCursor(20, 5);//设置显示位置
  display.println(USE_CPU);
  display.setCursor(80, 5);
  display.println("%");
  display.display();      // 开显示
  display.clearDisplay(); //清屏
  
  //数码管显示（这里用6位数码管代替4位数码管）
  tm1637.display(3,USE_CPU/100);    //在数码管的第四个位置显示百位
  tm1637.display(4,USE_CPU%100/10); //在数码管的第五个位置显示十位
  tm1637.display(5,USE_CPU%10);     //在数码管的第六个位置显示个位
}

