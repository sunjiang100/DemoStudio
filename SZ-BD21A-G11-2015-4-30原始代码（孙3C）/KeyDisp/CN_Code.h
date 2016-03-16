/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												肖遥电子（广州）研发中心
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.shawyoo.com		 Email: autoele@qq.com
$								 QQ群：9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 
	功能描述：
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef      __CN_CODE_H__
#define      __CN_CODE_H__


#ifdef      CN_GLOBALS
#define     CN_EXT
#else
#define     CN_EXT    extern
#endif



//#define     GSM_Signle_markeH    0xF8
//#define     GSM_Signle_markeL    0x00



#define			GB2312_CODE_ADDR			0x007C0000							  //国标点阵码在flash的起始地址
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$  定义字幕时注意点：
$  1：LCM可以显示4行字，每行16个字节，汉字要从偶地址开始，否则出乱码
$	2: 本系统只支持带字库的LCM，如：TG12864E   
*/
/////////////////////   定义默认显示字幕  ////////////////////////////////////////
CN_EXT	  uchar 	const		DefaultScreen[][17] =	 
{
{0xAF,0xA1,0x20,0x56,0x30,0x30,0xA1,0xEF,0x31,0x32,0x3A,0x30,0x30,0x3A,0x30,0x30},	 //gsm信号符号0xAFA1开始地址 上线标志  定位标志 卫星个数 星号 时间
//{"000km/h  000km/h"},																  //速度 方位符号0xAFA7开始地址
{"000km/h 速度正常"},																  //速度 方位符号0xAFA7开始地址
{"驾驶员:         "},
{" 请插入驾驶员卡!"},
{"疲劳驾驶,请休息!"},
{"您已超速,请减速!"},
{"请注意,疲劳预警!"},

//{0x31, 0x32, 0x31, 0x32,0x31, 0x32,0x31, 0x32,0x31, 0x32,0x31, 0x32,0x31, 0x32,0x31, 0x32, },
//{0x41, 0x42, 0x41, 0x42,0x41, 0x42,0x41, 0x42,0x41, 0x42,0x41, 0x42,0x41, 0x42,0x41, 0x42, },
{"\0"},																				  
};

/////////////////////   定义第二主字幕  ////////////////////////////////////////
CN_EXT	uchar		const		ConfirmSCR[][17] =	
{
{"经度:           "},
{"纬度:           "},
{"海拔:           "},
{"速度:           "},
{"\0"},
};



/////////////////////   星期日一二三四五六											 //取消此显示
CN_EXT	uchar	const		WeekCode[16] = 
{"日一二三四五六 "};



/////////////////////   定义系统正在初始化字幕  ////////////////////////////////////////
CN_EXT	uchar		const		InitialingSCR[][17] =	
{
{"  系统正在自检  "},
{"  请稍候......  "},
{"  自检完成!     "},
{"祝您好一路平安！"},
{"\0"},
};

/////////////////////   自检出错字幕  ////////////////////////////////////////////////
CN_EXT	uchar		const		SelfTestResustER_SCR[][17] =
{
{"Flash读写错误!!!"},
{"铁电读写错误!!!!"},
{"GSM模块错误!!!!!"},
{"GPS模块错误!!!!!"},
{"GPS天线开路!!!!!"},
{"GPS天线短路!!!!!"},
{"摄像头未接或错误"},
{"身份读卡器错误！"},
{"\0"},
};	

CN_EXT	uchar		const		SelfTestResustOK_SCR[][17] =
{
{"Flash读写正常!!!"},
{"铁电读写正常!!!!"},
{"GSM模块正常!!!!!"},
{"GPS模块正常!!!!!"},
{"GPS天线未开路!!!"},
{"GPS天线未短路!!!"},
{"摄像头工作正常!!"},
{"身份读卡器正常！"},
{"\0"},
};	



/////////////////////   定义主菜单字幕  ///////////////////////////////////////////   
//#define		MAX_SUM_MENU			12
//CN_EXT	uchar		const		MainMenuSCR[][17] =
//{
//{"1.车辆特征系数  "},
//{"2.当前驾驶员信息"},
//{"3.车辆信息      "},
//{"4.里程          "},
//{"5.停车前平均速度"},
//{"6.疲劳驾驶记录  "},
//{"7.通信及定位参数"},
//{"8.载货信息      "},
//{"9.电话簿        "},
//{"10.终端控制     "},
//{"11.参数设置     "},
//{"12.产品版本信息 "},
////{0x33, 0x2E, 0xC6, 0xA3, 0xC0, 0xCD, 0xBC, 0xDD, 0xCA, 0xBB, 0xBC, 0xC7, 0xC2, 0xBC, 0x20, 0x20},
////{0x34, 0x2E, 0xB5, 0xB1, 0xC7, 0xB0, 0xBC, 0xDD, 0xCA, 0xBB, 0xD4, 0xB1, 0xD0, 0xC5, 0xCF, 0xA2},
////{0x35, 0x2E, 0xB3, 0xAC, 0xCB, 0xD9, 0xBC, 0xC7, 0xC2, 0xBC, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20},
//{"\0"},
//};

#define		MAX_SUM_MENU			12
CN_EXT	uchar		const		MainMenuSCR[][17] =
{
{"1.车辆信息      "},
{"2.疲劳驾驶记录  "},
{"3.当前驾驶员信息"},
{"4.里程          "},
{"5.停车前平均速度"},
{"6.脉冲系数      "},
{"7.通信及定位参数"},
{"8.载货信息      "},
{"9.电话簿        "},
{"10.终端控制     "},
{"11.参数设置     "},
{"12.产品版本信息 "},
//{0x33, 0x2E, 0xC6, 0xA3, 0xC0, 0xCD, 0xBC, 0xDD, 0xCA, 0xBB, 0xBC, 0xC7, 0xC2, 0xBC, 0x20, 0x20},
//{0x34, 0x2E, 0xB5, 0xB1, 0xC7, 0xB0, 0xBC, 0xDD, 0xCA, 0xBB, 0xD4, 0xB1, 0xD0, 0xC5, 0xCF, 0xA2},
//{0x35, 0x2E, 0xB3, 0xAC, 0xCB, 0xD9, 0xBC, 0xC7, 0xC2, 0xBC, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20},
{"\0"},
};



#define			MAX_SET_PARA_MENU			22//(21-1)  // 不包含“请输入验正密码”
CN_EXT	uchar		const		SetRegParaSCR[][17] = 
{
{"请输入验证密码  "},
{"设置主中心IP地址"},
{"设置副中心IP地址"},
{"设置TCP端口     "},
{"设置UDP端口     "},
{"设置省域ID      "},
{"设置市域ID      "},
{"设置车牌号码    "},
{"设置车牌颜色    "},
{"设置VIN号       "},
{"设置车辆类型    "},
{"设置本机手机号码"},
{"设置车主手机号码"},
{"设置监控中心号码"},
{"设置脉冲系数    "},
{"设置上报时间间隔"},
{"首次使用开户    "},
{"注销本机        "},
{"恢复出厂值/重启 "},
{"远程升级        "},
{"修改密码        "},
{"切换货运平台域名"},
//{"请再次输入新密码"},
{"                "},		//  凑够要显示的偶数行（除了“请输入密码”)
};


//CN_EXT	uchar		const InputSelectChar[][16] =
//{
//	//{'0','1','2','3','4','5','6','7','8','9', 'A','B','C','D','E','F'},
//	{'0','1','2','3','4','5','6','7','8','9', '.'},
//};


#define		LEN_NUM_DOT	  11		
CN_EXT	uchar		const InputSelectNumDot[LEN_NUM_DOT] =
{
	'0','1','2','3','4','5','6','7','8','9', '.'
};

#define		LEN_SELECT_COLOR	  (2 * 5)		
CN_EXT	uchar		const InputSelectColor[LEN_SELECT_COLOR+1] =
{
	"蓝黄绿黑白"
};


#define		LEN_SELECT_CAR_NUM	  (104)		
CN_EXT	uchar		const InputSelectCarNum[LEN_SELECT_CAR_NUM+1] =
{
	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ京沪津渝浙皖闽赣港鲁澳豫蒙鄂新湘宁粤藏琼桂川冀贵晋云辽陕吉甘黑青苏台"
};



/////////////////////   车辆特征系数字幕  ///////////////////////////////////////////
CN_EXT	uchar		const		VehicleCC_SCR[][17] = 
{
//{"1.车牌号码：    "},
//{"粤A88888        "},
{"  脉冲系数      "},
{"        0p/km   "},//{"        0r/km   "},
//{"3.车辆类型：    "},
//{"大型客车        "},

//{"每km脉冲：      "},
//{0xB1, 0xA8, 0xBE, 0xAF, 0xCB, 0xD9, 0xB6, 0xC8, 0x3A, 0x30, 0x30, 0x30, 0x6B, 0x6D, 0x2F, 0x68},
{"\0"},
};

////////////////// 车辆类型 ///////////////////
#define		MAX_VEHICLE_TYPE_MENU			4
CN_EXT	uchar		const		VehicleTypeSCR[][17] =
{
{"1.客运汽车      "},
{"2.货运汽车      "},
{"3.危险品汽运    "},
{"4.出租汽车      "},
{"5.其它汽车      "},
{"\0"},

};

/////////////////////   当前驾驶员信息 ////////////////////////////////////   
CN_EXT	uchar		const		DriverInfoSCR[][17] =	
{
{"姓名:        "},
{"代码: 100       "},
{"驾驶证证号: xxxx"},
{"xxxxxxxxxxxxxx  "},
{"\0"},
};

/////////////////////   车辆信息 ////////////////////////////////////   
CN_EXT	uchar		const		CarMSGSCR[][17] =	
{
{"车牌:粤A88888888"},
{"类型:大型客车   "},
{"VIN号:xxxxxxxxxx"},
{"xxxxxxx         "},
{"车牌颜色代码:   "},	// 车牌颜色代码:01
{"车牌颜色:       "},	// 车牌颜色:蓝
{"\0"},
};





/////////////////////   疲劳驾驶记录字幕  ///////////////////////////////////////////   
//CN_EXT	uchar		const		TireTimeSCR[][17] =
//{
//{"疲劳记录01:     "},
//{"xxxxxxxxxxxx    "},		  	  //驾驶证号
//{"20xx/xx/xx xx:xx"},			  //开始时候
//{"20xx/xx/xx xx:xx"},			  //结束时间
//{"\0"},
//};

CN_EXT	uchar		const		TireTimeSCR[][17] =
{
{"疲劳记录01:     "},
{"xxxxxxxxxxxx    "},		  	  //驾驶证号
{"xxxxxx xx:xx:xx"},			  //开始时候
{"xxxxxx xx:xx:xx"},			  //结束时间
{"\0"},
};


/////////////////////   无疲劳驾驶记录字幕  /////////////////////////////////////////   
CN_EXT	uchar		const		NoTireTimeSCR[][17] =
{
{"  两个日历天无  "},
{"  疲劳驾驶记录  "},
{"\0"},
};



///////////////////////   无线通信参数 /////////////////////////////////////////   
//CN_EXT	uchar		const		MobileCMNCSCR[][17] =
//{
//{"主服务器地址:   "},
//{"                "},
//{"备份服务器地址: "},
//{"                "},
//{"TCP端口号:      "},
//{"UDP端口号:      "},
//{"记录仪SIM卡号码:"},
//{"                "},
//{"平台短信号码:   "},
//{"                "},
//{"平台电话号码:   "},
//{"                "},
//{"文本SMS报警号码:"},
//{"                "},
//{"当前定位模式：  "},
//{"                "},
//{"当前上报间隔：  "},
//{"                "},
//{"\0"},
//};

/////////////////////   无线通信参数 /////////////////////////////////////////   
CN_EXT	uchar		const		MobileCMNCSCR[][17] =
{
{"主服务器地址:   "},
{"                "},
{"备份服务器地址: "},
{"                "},
{"APN:            "},
{"                "},
{"TCP端口号:      "},
{"UDP端口号:      "},
{"本机手机号码:   "},
{"                "},
{"车主手机号码:   "},
{"                "},
{"平台电话号码:   "},
{"                "},
{"SMS文本报警号码:"},
{"                "},
{"开户状态:       "},
{"                "},
{"当前定位模式：  "},
{"                "},
{"当前上报间隔：  "},
{"                "},
{"\0"},
};

/////////////////////   调度信息  ///////////////////////////////////////// 
CN_EXT	uchar		const		SchedulMSGSCR[][17] =
{
{"      空载      "},
{"      轻载      "},
{"      满载      "},
{"                "},
{"   无调度信息   "},
{"                "},
{"   设置成功!    "},
{"                "},
{"   设置失败!    "},
{"\0"},
};


/////////////////////   电话簿  ///////////////////////////////////////// 
CN_EXT	uchar		const		PhoteBookSCR[][17] =
{
{"  无电话号码    "},
{"                "},
{"\0"},
};




/////////////////////   日期  ///////////////////////////////////////// 
CN_EXT	uchar		const		DateSCR[][17] =
{
{"日期: 2020-12-31"},
{"时间: 23:59:59  "},
{"\0"},
};

/////////////////////   里程 ///////////////////////////////////////// 
CN_EXT	uchar		const		MilageSCR[][17] =
{
{"脉冲累计总里程: "},
{"卫星定位总里程: "},
{"如需清除测试里程"},
{"请按确认键！    "},
{"\0"},
};
/////////////////////   定义产品唯一序列号字幕 ////////////////////////////////////////
CN_EXT	uchar		const		SoleNumberSCR[][17] = 
{
{"产品序号:       "},
{"                "},
{"产品型号:       "},
{"                "},
{"硬件版本号:     "},
{"                "},
{"软件版本号:     "},
{"                "},
{"编译日期:       "},
{"                "},
{"编译时间:       "},
{"                "},
{"                "},
{"                "},
{"\0"},
};



/////////////////////   行驶状态不能更换驾驶员  /////////////////////////////////////
CN_EXT	uchar		const		CannotChangeDriverSCR[][17] = 
{
{"很抱歉，行驶状态"},
{"不能更换驾驶员  "},
{'\0'},
};


/////////////////////   最近15分钟每分钟平均速度////////////////////////////////////     取消此显示
CN_EXT	uchar		const		AVG_SpeedSCR[][16] =
/*{
	0x32, 0x2E, 0xD7, 0xEE, 0xBA, 0xF3, 0xCD, 0xA3, 0xB3, 0xB5, 0xC7, 0xB0, 0x31, 0x35, 0xB7, 0xD6, 
	0xD6, 0xD3, 0xC3, 0xBF, 0xB7, 0xD6, 0xD6, 0xD3, 0xC6, 0xBD, 0xBE, 0xF9, 0xCB, 0xD9, 0xB6, 0xC8, 
	0xC8, 0xD5, 0xC6, 0xDA, 0xA3, 0xBA, 0x32, 0x30, 0x78, 0x78, 0x2F, 0x78, 0x78, 0x2F, 0x78, 0x78, 
	0x68, 0x68, 0x3A, 0x6D, 0x6D, 0x20, 0x20, 0x78, 0x78, 0x78, 0x20, 0x6B, 0x6D, 0x2F, 0x68, 0x20, 
	0x68, 0x68, 0x3A, 0x6D, 0x6D, 0x20, 0x20, 0x78, 0x78, 0x78, 0x20, 0x6B, 0x6D, 0x2F, 0x68, 0x20, 
	0x68, 0x68, 0x3A, 0x6D, 0x6D, 0x20, 0x20, 0x78, 0x78, 0x78, 0x20, 0x6B, 0x6D, 0x2F, 0x68, 0x20, 
	0x68, 0x68, 0x3A, 0x6D, 0x6D, 0x20, 0x20, 0x78, 0x78, 0x78, 0x20, 0x6B, 0x6D, 0x2F, 0x68, 0x20, 	
	0x68, 0x68, 0x3A, 0x6D, 0x6D, 0x20, 0x20, 0x78, 0x78, 0x78, 0x20, 0x6B, 0x6D, 0x2F, 0x68, 0x20, 		
	0x32, 0x2E, 0xD7, 0xEE, 0xBA, 0xF3, 0xCD, 0xA3, 0xB3, 0xB5, 0xC7, 0xB0, 0x31, 0x35, 0xB7, 0xD6, 
	0xD6, 0xD3, 0xC3, 0xBF, 0xB7, 0xD6, 0xD6, 0xD3, 0xC6, 0xBD, 0xBE, 0xF9, 0xCB, 0xD9, 0xB6, 0xC8, 
	0x20, 0x20, 0xC3, 0xBB, 0xD3, 0xD0, 0xCD, 0xA3, 0xB3, 0xB5, 0xBC, 0xC7, 0xC2, 0xBC, 0x20, 0x20, 
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
	0x00
};			*/

{
{"日期:20xx-xx-xx"},
{"hh:mm  xxxkm/h "},
{"hh:mm  xxxkm/h "},
{" 没有停车记录！"},
{"               "},
{"               "},
{"\0"},
};




////////////////////////////////////////////////////////////////////////////////////
CN_EXT	uchar		const		PrintSCR[][17] =									  //打印提示
{
{"  正在打印...  "},
{"               "},
{"   打印完毕！  "},
{"               "},
{"  再按确定键   "},
{"     打印！    "},
{"  打印机缺纸！ "},
{"               "},
{"\0"},
};																									


////////////////////////////////////////////////////////////////////////////////////
CN_EXT	uchar		const		SYE_Data[]	=														  //采集指定数据
{
	0x2e, 0x2f, 0x48, 0x5d, 0x36, 0x33, 0x5c, 0x45, 0x2f, 0x5b, 0x2d, 0x5c, 0x4a, 0x18, 0x28, 0x2c, 
	0x5c, 0x57, 0x46, 0x1c, 0x29, 0x22, 0x5c, 0x56, 0x2e, 0x2f, 0x48, 0x5d, 0x29, 0x2f, 0x2f, 0x3b, 
	0xf2, 0xf5, 0x32, 0x07, 0x29, 0x48, 0x5c, 0x45, 0x97, 0x8b, 0x8b, 0x8f, 0xc5, 0xd0, 0xd0, 0x88, 
	0x88, 0x88, 0xd1, 0x8c, 0x97, 0x9e, 0x88, 0x86, 0x90, 0x90, 0xd1, 0x9c, 0x90, 0x92, 0xf2, 0xf5, 
	0x35, 0x29, 0x44, 0x05, 0x5c, 0x45, 0xce, 0xcc, 0xc6, 0xcd, 0xcd, 0xcd, 0xc7, 0xcf, 0xca, 0xc7, 
	0xc9, 0x0D, 0x0A, 0x0D, 0x0A, 0x00
};


CN_EXT	uchar		const		DonotRemoveDiskSCR[][17] = 
{
{" USB数据采集...."},
{"                "},
{"数据采集失败!!!!"},
{"请重新插入U盘..."},
{"\0"},
};


CN_EXT	uchar		const		CanRemoveDiskSCR[][17] = 
{
{"  数据采集完毕! "},
{"                "},
{"\0"},
};


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/
/////////////////////   定义超速记录字幕 ////////////////////////////////////////		  取消此显示
CN_EXT	uchar		const		OverSpeedSCR[][16] = 
{
//{"5.超速驾驶记录  "},
//{"每页一条，格式："},
//{"18位驾证号最高速"},
//{"开始时间结束时间"},
//{"5.超速01: xxxxxx"},
//{"xxxxxxxxxxxx xxx"},
//{"xx/xx  xx:xx:xx "},
//{"xx/xx  xx:xx:xx "},
{"\0"},
};



/////////////////////   定义超速记录字幕 ////////////////////////////////////////		 取消此显示
CN_EXT	uchar		const		NoOverSpeedSCR[][16] = 
{
//{"5.超速驾驶记录  "},
//{"                "},
//{"  以下无记录    "},
//{"                "},
{"\0"},
};


/////////////////////   定义终端控制字幕 ////////////////////////////////////////		 取消此显示
#define		SET_PARA_COMFIR		15
CN_EXT	uchar		const		SetParaSCR[][17] = 
{
{"1.本菜单部分开放"},
{"2.终端自检      "},
{"3.使用外部调度屏"},
{"4.使用外部读卡器"},
{"5.使用外部摄像头"},
{"6.使用外部录音器"},
//{"7.只使用GPS定位 "},
{"7.手动重启系统  "},
//{"8.只使用北斗定位"},
{"8.清除速度异常  "},
{"9.使用双模定位  "},
{"10.冷启定位模块 "},
{"11.冷启动GPS模块"},
{"12.GPRS重新链接 "},
{"13.使用前锁定一 "},
{"14.切回主中心IP "},
{"                "},
{"确定要注销本机？"},
{"                "},
{"   正在注销.... "},
{"                "},
{"   注销成功！   "},
{"                "},
{"   注销失败！   "},
{"                "},
{"  请插入授权卡！"},
{"                "},
{"\0"},
};


CN_EXT	uchar		const		IntervalSCR[][17] = 
{
{"  上报时间间隔  "},
{"1.    10秒      "},
{"2.    20秒      "},
{"3.    30秒      "},
{"4.    40秒      "},
{"5.    50秒      "},
{"6.    60秒      "},
{"7.    90秒      "},
{"8.    120秒     "},
{"9.    150秒     "},
{"10.   180秒     "},
{"11.   300秒     "},
{"\0"},
};


/////////////////////   临时注册信息子菜单
CN_EXT	uchar		const		RegInfoSCR[][17] = 
{
{"选择终端注册信息"},
{"1. GY-G11A  1#  "},	 	// 580
{"2. GY-G11A  2#  "},		// 581
{"3. GY-G11A  3#  "},		// 582
{"4. GY-G11A  4#  "},		// 583
{"5. GY-G11A  5#  "},		// 584
{"6. SZ-BD21  1#  "},		// 585
{"7. SZ-BD21  2#  "},
{"8. SZ-BD21  3#  "},
{"9. SZ-BD21  4#  "},
{"10. SZ-BD21 5#  "},
{"11. SZYTJ-B01 1#"},
{"12. SZYTJ-B01 2#"},
{"13. SZYTJ-B01 3#"},
{"14. SZYTJ-B01 4#"},				 
{"15. SZYTJ-B01 5#"},
{"16. WT800-A  1# "},
{"17. WT800-A  2# "},
{"18. WT800-A  3# "},
{"19. WT800-A  4# "},
{"20. WT800-A  5# "},
{"21. GJ-G12-A 1# "},
{"22. GJ-G12-A 2# "},
{"23. GJ-G12-A 3# "},
{"24. GJ-G12-A 4# "},
{"25. GJ-G12-A 5# "},
{"26. BAJ-A1   1# "},
{"\0"},
};




/////////////////////   定义系统升级提示 ////////////////////////////////////////		 取消此显示
CN_EXT	uchar		const		SysUpdateSpeedSCR[][17] = 
{
{"设备正在升级...."},
{"                "},
{"\0"},
};



///////////////////////   记录数据名称 ////////////////////////////////////////
//CN_EXT	uchar		const		GBDataName[][20] = 
//{ 
//{"执行标准版本号"},
//{"当前驾驶员信息"},
//{"实时时钟"},
//{"累计行驶里程"},
//{"速度脉冲系数"},
//{"车辆信息"},
//{"状态信号配置信息"},
//{"记录仪唯一性编号"},
//{"每秒钟平均速度记录"},
//{"每分钟平均速度记录"},
//{"位置信息记录"},
//{"事故疑点记录"},
//{"疲劳驾驶记录"},
//{"登录退出记录"},
//{"外部供电记录"},
//{"参数修改记录"},
//{"记录仪安装参数"},
//{"定位、告警相关参数"},
//{"电子围栏数据"},
//{"电子围栏目录"},
//{"线路规划数据"},
//{"线路规划目录"},
//{"\0"},
//};

////////////////// 0,  46  , 56 ,
/*CN_EXT	uchar      const  RegInfo25Device[][81]	= 	
{
{0x2c,0x00,0x69,0x00,'0','0','0','0','0','G','Y','-','G','1','1','A',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, '2','2','8','0','5','8','0',0x02,0xd4,0xc1,'A','8','0','5','8','0',0x00,'2','2','2','8','0','5','8','0',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','8','0',0x00,'1','3','1','8','0','2','0','0','0','5',0x00},	
{0x2c,0x00,0x69,0x00,'0','0','0','0','0','G','Y','-','G','1','1','A',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, '2','2','8','0','5','8','1',0x02,0xd4,0xc1,'A','8','0','5','8','1',0x00,'2','2','2','8','0','5','8','1',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','8','1',0x00,'1','3','1','7','0','2','0','0','0','1',0x00},	
{0x2c,0x00,0x69,0x00,'0','0','0','0','0','G','Y','-','G','1','1','A',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, '2','2','8','0','5','8','2',0x02,0xd4,0xc1,'A','8','0','5','8','2',0x00,'2','2','2','8','0','5','8','2',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','8','2',0x00,'1','3','1','7','0','2','0','1','1','4',0x00},	
{0x2c,0x00,0x69,0x00,'0','0','0','0','0','G','Y','-','G','1','1','A',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, '2','2','8','0','5','8','3',0x02,0xd4,0xc1,'A','8','0','5','8','3',0x00,'2','2','2','8','0','5','8','3',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','8','3',0x00,'1','3','1','7','0','2','0','1','2','9',0x00},	
{0x2c,0x00,0x69,0x00,'0','0','0','0','0','G','Y','-','G','1','1','A',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, '2','2','8','0','5','8','4',0x02,0xd4,0xc1,'A','8','0','5','8','4',0x00,'2','2','2','8','0','5','8','4',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','8','4',0x00,'1','3','1','6','0','2','0','0','1','6',0x00},
{0x2c,0x00,0x69,0x00,'7','0','5','1','7','S','Z','-','B','D','2','1',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, '2','2','8','0','5','8','5',0x02,0xbb,0xa6,'A','8','0','5','8','5',0x00,'2','2','2','8','0','5','8','5',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','8','5',0x00,'1','3','1','7','0','2','0','1','0','4',0x00},	
{0x2c,0x00,0x69,0x00,'7','0','5','1','7','S','Z','-','B','D','2','1',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, '2','2','8','0','5','8','6',0x02,0xbb,0xa6,'A','8','0','5','8','6',0x00,'2','2','2','8','0','5','8','6',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','8','6',0x00,'1','3','1','8','0','2','0','0','0','3',0x00},	
{0x2c,0x00,0x69,0x00,'7','0','5','1','7','S','Z','-','B','D','2','1',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, '2','2','8','0','5','8','7',0x02,0xbb,0xa6,'A','8','0','5','8','7',0x00,'2','2','2','8','0','5','8','7',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','8','7',0x00,'1','3','1','7','0','2','0','1','2','5',0x00},	
{0x2c,0x00,0x69,0x00,'7','0','5','1','7','S','Z','-','B','D','2','1',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, '2','2','8','0','5','8','8',0x02,0xbb,0xa6,'A','8','0','5','8','8',0x00,'2','2','2','8','0','5','8','8',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','8','8',0x00,'1','3','1','7','0','2','0','1','3','4',0x00},	
{0x2c,0x00,0x69,0x00,'7','0','5','1','7','S','Z','-','B','D','2','1',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, '2','2','8','0','5','8','9',0x02,0xbb,0xa6,'A','8','0','5','8','9',0x00,'2','2','2','8','0','5','8','9',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','8','9',0x00,'1','3','1','8','0','2','0','0','0','7',0x00},
{0x2c,0x00,0x69,0x00,'7','0','5','1','7','S','Z','Y','T','J','-','B','0','1',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   '2','2','8','0','5','9','0',0x02,0xbb,0xa6,'A','8','0','5','9','0',0x00,'2','2','2','8','0','5','9','0',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','9','0',0x00,'1','3','1','7','0','2','0','1','2','6',0x00},
{0x2c,0x00,0x69,0x00,'7','0','5','1','7','S','Z','Y','T','J','-','B','0','1',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   '2','2','8','0','5','9','1',0x02,0xbb,0xa6,'A','8','0','5','9','1',0x00,'2','2','2','8','0','5','9','1',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','9','1',0x00,'1','3','1','7','0','2','0','1','1','2',0x00},
{0x2c,0x00,0x69,0x00,'7','0','5','1','7','S','Z','Y','T','J','-','B','0','1',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   '2','2','8','0','5','9','2',0x02,0xbb,0xa6,'A','8','0','5','9','2',0x00,'2','2','2','8','0','5','9','2',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','9','2',0x00,'1','3','1','7','0','2','0','2','3','2',0x00},
{0x2c,0x00,0x69,0x00,'7','0','5','1','7','S','Z','Y','T','J','-','B','0','1',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   '2','2','8','0','5','9','3',0x02,0xbb,0xa6,'A','8','0','5','9','3',0x00,'2','2','2','8','0','5','9','3',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','9','3',0x00,'1','3','1','7','0','2','0','1','1','1',0x00},
{0x2c,0x00,0x69,0x00,'7','0','5','1','7','S','Z','Y','T','J','-','B','0','1',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   '2','2','8','0','5','9','4',0x02,0xbb,0xa6,'A','8','0','5','9','4',0x00,'2','2','2','8','0','5','9','4',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','9','4',0x00,'1','3','1','7','0','2','0','1','2','8',0x00},
{0x2c,0x00,0x69,0x00,'0','0','0','0','0','W','T','8','0','0','-','A',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, '2','2','8','0','5','9','5',0x02,0xd4,0xc1,'A','8','0','5','9','5',0x00,'2','2','2','8','0','5','9','5',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','9','5',0x00,'1','3','1','9','0','2','0','0','0','1',0x00},
{0x2c,0x00,0x69,0x00,'0','0','0','0','0','W','T','8','0','0','-','A',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, '2','2','8','0','5','9','6',0x02,0xd4,0xc1,'A','8','0','5','9','6',0x00,'2','2','2','8','0','5','9','6',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','9','6',0x00,'1','3','1','9','0','2','0','0','0','2',0x00},
{0x2c,0x00,0x69,0x00,'0','0','0','0','0','W','T','8','0','0','-','A',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, '2','2','8','0','5','9','7',0x02,0xd4,0xc1,'A','8','0','5','9','7',0x00,'2','2','2','8','0','5','9','7',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','9','7',0x00,'1','3','1','9','0','2','0','0','0','3',0x00},
{0x2c,0x00,0x69,0x00,'0','0','0','0','0','W','T','8','0','0','-','A',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, '2','2','8','0','5','9','8',0x02,0xd4,0xc1,'A','8','0','5','9','8',0x00,'2','2','2','8','0','5','9','8',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','9','8',0x00,'1','3','1','9','0','2','0','0','0','4',0x00},
{0x2c,0x00,0x69,0x00,'0','0','0','0','0','W','T','8','0','0','-','A',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, '2','2','8','0','5','9','9',0x02,0xd4,0xc1,'A','8','0','5','9','9',0x00,'2','2','2','8','0','5','9','9',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','9','9',0x00,'1','3','1','9','0','2','0','0','0','5',0x00},
{0x2c,0x00,0x69,0x00,'0','0','0','0','0','G','J','-','G','1','2','-','A',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  '2','2','8','0','5','6','0',0x02,0xd4,0xc1,'A','8','0','5','6','0',0x00,'2','2','2','8','0','5','6','0',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','6','0',0x00,'1','3','1','9','0','2','0','0','0','6',0x00},
{0x2c,0x00,0x69,0x00,'0','0','0','0','0','G','J','-','G','1','2','-','A',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  '2','2','8','0','5','6','1',0x02,0xd4,0xc1,'A','8','0','5','6','1',0x00,'2','2','2','8','0','5','6','1',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','6','1',0x00,'1','3','1','9','0','2','0','0','0','7',0x00},
{0x2c,0x00,0x69,0x00,'0','0','0','0','0','G','J','-','G','1','2','-','A',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  '2','2','8','0','5','6','2',0x02,0xd4,0xc1,'A','8','0','5','6','2',0x00,'2','2','2','8','0','5','6','2',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','6','2',0x00,'1','3','1','9','0','2','0','0','0','8',0x00},
{0x2c,0x00,0x69,0x00,'0','0','0','0','0','G','J','-','G','1','2','-','A',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  '2','2','8','0','5','6','3',0x02,0xd4,0xc1,'A','8','0','5','6','3',0x00,'2','2','2','8','0','5','6','3',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','6','3',0x00,'1','3','1','9','0','2','0','0','0','9',0x00},
{0x2c,0x00,0x69,0x00,'0','0','0','0','0','G','J','-','G','1','2','-','A',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  '2','2','8','0','5','6','4',0x02,0xd4,0xc1,'A','8','0','5','6','4',0x00,'2','2','2','8','0','5','6','4',0x00,0x00,'0','1','3','9','2','2','2','8','0','5','6','4',0x00,'1','3','1','9','0','2','0','0','1','0',0x00},
{0x2c,0x00,0x69,0x00,'0','0','0','0','0','B','A','J','-','A','1',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,'C','0','8','8','8','8','8',0x02,0xd4,0xc1,'C','8','8','8','8','8',0x00,0x00},
{"\0"},
};*/



CN_EXT	uchar      const  ASCII_Code[1536]	= 									 //__at	ASCII_CODE_ADDR
{																		   
/*    CHAR_20 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*!   CHAR_21 */
0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00,

/*"   CHAR_22 */
0x00,0x10,0x0C,0x06,0x10,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*#   CHAR_23 */
0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00,

/*$   CHAR_24 */
0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00,0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00,

/*%   CHAR_25 */
0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00,0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00,

/*&   CHAR_26 */
0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10,

/*'   CHAR_27 */
0x10,0x16,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*(   CHAR_28 */
0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00,

/*)   CHAR_29 */
0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00,

/**   CHAR_2A */
0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00,

/*+   CHAR_2B */
0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00,

/*,   CHAR_2C */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00,

/*-   CHAR_2D */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,

/*.   CHAR_2E */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,

/*/   CHAR_2F */
0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04,0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00,

/*0   CHAR_30 */
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,

/*1   CHAR_31 */
0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,			

/*2   CHAR_32 */
0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,

/*3   CHAR_33 */
0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,

/*4   CHAR_34 */
0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,

/*5   CHAR_35 */
0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,

/*6   CHAR_36 */
0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,

/*7   CHAR_37 */
0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,

/*8   CHAR_38 */
0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,

/*9   CHAR_39 */
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,

/*:   CHAR_3A */
0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,

/*;   CHAR_3B */
0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00,

/*<   CHAR_3C */
0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00,

/*=   CHAR_3D */
0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,

/*>   CHAR_3E */
0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00,

/*?   CHAR_3F */
0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00,0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00,

/*@   CHAR_40 */
0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00,

/*A   CHAR_41 */
0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20,

/*B   CHAR_42 */
0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00,

/*C   CHAR_43 */
0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,

/*D   CHAR_44 */
0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00,

/*E   CHAR_45 */
0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,

/*F   CHAR_46 */
0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00,

/*G   CHAR_47 */
0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,

/*H   CHAR_48 */
0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20,

/*I   CHAR_49 */
0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,

/*J   CHAR_4A */
0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00,

/*K   CHAR_4B */
0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00,

/*L   CHAR_4C */
0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,

/*M   CHAR_4D */
0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00,

/*N   CHAR_4E */
0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00,

/*O   CHAR_4F */
0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,

/*P   CHAR_50 */
0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,

/*Q   CHAR_51 */
0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00,

/*R   CHAR_52 */
0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20,

/*S   CHAR_53 */
0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,

/*T   CHAR_54 */
0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,

/*U   CHAR_55 */
0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,

/*V   CHAR_56 */
0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00,

/*W   CHAR_57 */
0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00,

/*X   CHAR_58 */
0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20,

/*Y   CHAR_59 */
0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,

/*Z   CHAR_5A */
0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00,

/*[   CHAR_5B */
0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00,

/*\   CHAR_5C */
0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00,

/*]   CHAR_5D */
0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,

/*^   CHAR_5E */
0x00,0x00,0x04,0x02,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*_   CHAR_5F */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,

/*`   CHAR_60 */
0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*a   CHAR_61 */
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20,

/*b   CHAR_62 */
0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00,

/*c   CHAR_63 */
0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00,

/*d   CHAR_64 */
0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20,

/*e   CHAR_65 */
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,

/*f   CHAR_66 */
0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,

/*g   CHAR_67 */
0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00,

/*h   CHAR_68 */
0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,

/*i   CHAR_69 */
0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,

/*j   CHAR_6A */
0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,

/*k   CHAR_6B */
0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00,

/*l   CHAR_6C */
0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,

/*m   CHAR_6D */
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,

/*n   CHAR_6E */
0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,

/*o   CHAR_6F */
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,

/*p   CHAR_70 */
0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00,

/*q   CHAR_71 */
0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80,

/*r   CHAR_72 */
0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,

/*s   CHAR_73 */
0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00,

/*t   CHAR_74 */
0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,

/*u   CHAR_75 */
0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,

/*v   CHAR_76 */
0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00,

/*w   CHAR_77 */
0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00,

/*x   CHAR_78 */
0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00,

/*y   CHAR_79 */
0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00,

/*z   CHAR_7A */
0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00,

/*{   CHAR_7B */
0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,

/*|   CHAR_7C */
0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,

/*}   CHAR_7D */
0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00,0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00,

/*~   CHAR_7E */
0x00,0x06,0x01,0x01,0x02,0x02,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*   CHAR_7F */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

};








#endif 
/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
