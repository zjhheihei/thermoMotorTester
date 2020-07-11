#include "macrotypedef.h"
#include "math.h"

/*
	Read,Write Local address function:
  	int ReadLocal( const char *type, int addr, int nRegs, void *buf, int flag  );
	int WriteLocal( const char *type, int addr, int nRegs, void *buf , int flag );

	Parameter:     type     is the string of "LW","LB" etc;
								address is the Operation address ;
 								nRegs    is the length of read or write ;
								buf        is the buffer which store the reading or writing data;
 								flag       is 0,then codetype is BIN,is 1  then codetype is BCD;
	return value : 1  ,Operation success
 								0,  Operation fail.

 	eg: read the value of local lw200 and write it to the lw202,with the codetype BIN,
		The code is :

    	short buf[2] = {0};
		ReadLocal("LW", 200, 2, (void*)buf, 0);
		WriteLocal("LW", 202, 2, (void*)buf, 0);
*/
//200ms循环
//----------------------------------------------------------------
typedef enum

{   

    mgm_select           = 0x00,//电压、产品选择

    mgm_measure          = 0x01,//测量界面

    mgm_parameter_s      = 0x02,//参数设定

    mgm_em_stop          = 0x03,//急停

}modbus_gui_menu_def;

//-----------------------------------------------------------------------------

//测量子菜单

typedef enum

{

    mgs_ms_idle         = 0x00,

    mgs_ms_loading      = 0x01,//加负载

    mgs_ms_measuring    = 0x02,//测量中

    mgs_ms_unloading    = 0x03,

    mgs_ms_complete     = 0x04,

}modbus_gui_s_measure_def;

//-----------------------------------------------------------------------------

typedef struct

{

    modbus_gui_menu_def  m_gui_menu;

    unsigned short          m_gui_bits;

    union

    {

        unsigned short   m_gui_sm;

        modbus_gui_s_measure_def  m_gui_s_measure;

    };



}modbus_gui_def;
//---------------------------------------------------------------
typedef union
{
    unsigned short buff[100];
    struct
    {
        modbus_gui_def  modbus_gui;
		unsigned short backup_key_ss;
		
        short test_s;
        float test_f;
    };
}sc_parameter_def;

sc_parameter_def  sc_parameter;
//-----------------------------------------------------------------
#define win_ix_rg_site      "LW"
#define win_ix_rg_addr    20
#define WIN_SELECT       10
#define WIN_MEASURE     0
//++++++++++++++++++++++++++++++++++++++
#define  sdt_bit0     0x0001

#define  sdt_bit1     0x0002

#define  sdt_bit2     0x0004

#define  sdt_bit3     0x0008

#define  sdt_bit4     0x0010

#define  sdt_bit5     0x0020

#define  sdt_bit6     0x0040

#define  sdt_bit7     0x0080

#define  sdt_bit8     0x0100

#define  sdt_bit9     0x0200

#define  sdt_bit10    0x0400

#define  sdt_bit11    0x0800

#define  sdt_bit12    0x1000

#define  sdt_bit13    0x2000

#define  sdt_bit14    0x4000

#define  sdt_bit15    0x8000
//-----------------------------------------------------------------
#define regAddr_m_version        0x0000

#define regAddr_m_gui_menu       0x0001//gui 主菜单

#define regAddr_m_gui_sm         0x0002//gui 子菜单

#define regAddr_m_gui_bits       0x0003//gui 位标识

//-----------------------------------------------------------------------------

#define regAddr_m_states_bits    0x0004

#define bits_mSBits_gzero        sdt_bit0//光栅零点寻找完毕

#define bits_mSBits_selected     sdt_bit1//已完成电压、型号选择

#define bits_mSBits_doorOpen     sdt_bit2//门被打开

#define bits_mSBits_emergercy    sdt_bit3

//-----------------------------------------------------------------------------

#define regAddr_m_event_bits      0x0005   

//-----------------------------------------------------------------------------
#define bits_mEBits_start         sdt_bit0//开始测量
#define bits_mEBits_stop          sdt_bit1//停止测量
#define bits_mEBits_load          sdt_bit2//加载
#define bits_mEBits_unload        sdt_bit3
#define bits_mEBits_selectCMT     sdt_bit4//选择完毕
#define bits_mEBits_heat          sdt_bit5//加热
#define bits_mEBits_unheat        sdt_bit6//不加热
//-----------------------------------------------------------------------------
#define regAddr_m_msrSecond        0x0006
#define regAddr_m_msrGt_um_0       0x0007
#define regAddr_m_msrGt_um_1       0x0008
#define regAddr_m_msrGt_max_0      0x0009
#define regAddr_m_msrGt_max_1      0x000A
#define regAddr_m_grating_pds_0    0x000B//物理位置,read
#define regAddr_m_grating_pds_1    0x000C
#define regAddr_m_logic_zero_0     0x000D//逻辑零点,rw
#define regAddr_m_logic_zero_1     0x000E
#define regAddr_m_select_voltage   0x000F//电压等级
#define regAddr_m_select_product   0x0010//产品类型
#define regAddr_m_second_3_5T      0x0011//3.5行程时间
//++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++
void get_remote_gui(void)
{
    
}
//++++++++++++++++++++++++++++++++++++++
void read_sc_parameter(void)
{
   // 
    sc_parameter.test_f +=0.001;
  // WriteLocal("LW",300,100,&sc_parameter.buff[0],0);
}
//++++++++++++++++++++++++++++++++++++++
int MacroEntry()
{

    ReadLocal("LW",1000,100,&sc_parameter.buff[0],0);
    sc_parameter.modbus_gui.m_gui_menu = (modbus_gui_menu_def)modbus_r[regAddr_m_gui_menu];
    sc_parameter.modbus_gui.m_gui_sm =  (modbus_gui_menu_def)modbus_r[regAddr_m_gui_sm];
    
    
    unsigned short win_menu;
    ReadLocal(win_ix_rg_site,win_ix_rg_addr,1,&win_menu,0);
    WriteLocal("LW",30,1,&sc_parameter.modbus_gui.m_gui_menu ,0);
	
	sc_parameter.modbus_gui.m_gui_menu = mgm_measure;
    switch(sc_parameter.modbus_gui.m_gui_menu)
    {
         case mgm_select:
         {
             if(WIN_SELECT != win_menu)
             {
                 win_menu  = WIN_SELECT;
                 WriteLocal(win_ix_rg_site,win_ix_rg_addr,1,&win_menu,0);
             }
             #define key_sel_ok_site     "LB"
             #define key_sel_ok_addr      100
             unsigned short key_select;
             
			 ReadLocal(key_sel_ok_site,key_sel_ok_addr,1,&key_select,0);
             if(key_select)
             {
                  win_menu = WIN_MEASURE;
                  WriteLocal(win_ix_rg_site,win_ix_rg_addr,1,&win_menu,0);
                  modbus_w[regAddr_m_event_bits] = bits_mEBits_selectCMT;
                  key_select = 0;
                  WriteLocal(key_sel_ok_site,key_sel_ok_addr,1,&key_select,0);
             }
             break;
         }
         case mgm_measure:
         {
			 #define key_start_stop_site   "LB"
			 #define key_start_stop_addr    0
			 unsigned short key_ss;
			 ReadLocal(key_start_stop_site,key_start_stop_addr,1,&key_ss,0);
			 if(sc_parameter.backup_key_ss != key_ss)
		     {
				sc_parameter.backup_key_ss = key_ss;
				if(key_ss)  //start
				{
				    modbus_w[regAddr_m_event_bits] = bits_mEBits_start;
					
				}
			    else
				{
				    modbus_w[regAddr_m_event_bits] = bits_mEBits_stop;
				}				 
			 }
			 switch(sc_parameter.modbus_gui.m_gui_s_measure)
			 {
				case mgs_ms_idle:
				{
					break;
				}
				case mgs_ms_loading:
				{
					break;
				}
				case mgs_ms_measuring:
				{
					break;
				}
				case mgs_ms_unloading:
				{
					break;
				}
				case mgs_ms_complete:
				{
					break;
				}
				default:
				{
					break;
				}
			 }

             break;
         }
         case mgm_parameter_s:
         {
             break;
         }
         case mgm_em_stop:
         {
             break;
         }
	     default:
		 {
			 break;
		 }
    }

    WriteLocal("LW",1000,100,&sc_parameter.buff[0],0);
	return 0;
}
 