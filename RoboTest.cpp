#include "RoboTest.hpp"

RFlowTopSec_Source(TopSec,3);
RFlowSubSec_Source(TopSub1,2,TopSec,1,1);
RFlowSubSec_Source(TopSub2,1,TopSec,2,1);
RFlowSubSec_Source(TopSub3,2,TopSec,3,1);
RFlowSubSec_Source(Sub2Sub,2,TopSub2,1,1);

RFlowBlock_Source_O_Start(Outputer1,float,u32,2,TopSub1,1,1)
	DataOut[0]=7;
	DataOut[1]=3;
RFlowBlock_Source_O_End(Outputer1,float,u32,2,TopSub1,1,1)

RFlowBlock_Source_O_Start(Outputer2,float,s32,3,TopSub1,1,2)
	DataOut[0]=-9;
	DataOut[1]=-4;
	DataOut[2]=-5;
RFlowBlock_Source_O_End(Outputer2,float,s32,3,TopSub1,1,2)

RFlowBlock_Source_IO_Start(IOer1,s32,float,3,3,Sub2Sub,1,1)
	DataOut[0]=(float) DataIn[1];
	DataOut[1]=(float) DataIn[0];
	DataOut[2]=(float) DataIn[2];
RFlowBlock_Source_IO_End(IOer1,s32,float,3,3,Sub2Sub,1,1)

RFlowBlock_Source_IO_Start(IOer2,u32,float,2,2,Sub2Sub,1,2)
	DataOut[0]=(float) DataIn[1];
	DataOut[1]=(float) DataIn[0];
RFlowBlock_Source_IO_End(IOer2,u32,float,2,2,Sub2Sub,1,2)

RFlowBlock_Source_IO_Start(IOer3,float,float,5,1,TopSub3,1,1)
	DataOut[0]=(DataIn[0]+DataIn[1])*DataIn[2]*(DataIn[3]-DataIn[4]);//(-4-9)*(-5)*(3-7)
RFlowBlock_Source_IO_End(IOer3,float,float,5,1,TopSub3,1,1)

RFlowBlock_Source_I_Start(Printer,float,float,1,TopSub3,1,2)
	cout<<DataIn[0]<<endl;
RFlowBlock_Source_I_End(Printer,float,float,1,TopSub3,1,2)
