/*
 * RoboFlow.hpp
 */

#ifdef __JRB

#ifndef _JRB_ROOT_ROBOFLOW_HPP_
#define _JRB_ROOT_ROBOFLOW_HPP_

#include <stdlib.h>
#include <stdint.h>
#include <cmath>
#include <typeinfo>

#ifndef __SIMPLETYPE
	#define __SIMPLETYPE
	typedef uint8_t u8;
	typedef uint16_t u16;
	typedef uint32_t u32;
	typedef int8_t s8;
	typedef int16_t s16;
	typedef int32_t s32;
#endif

#define RFCON(NAME,TAIL) _##NAME##_##TAIL

////////Block Set

//Macro of Section Definition
#define RFlowTopSec_Source(NAME,SCALE)\
		void (*RFCON(NAME,FUNCBASE)[SCALE])(void);\
		s32 RFCON(NAME,ACTBASE)[SCALE]={0};\
		RFlow_Section_Base NAME((&(RFCON(NAME,FUNCBASE)[0])),&(RFCON(NAME,ACTBASE)[0]),SCALE);\

#define RFlowTopSec_Header(NAME,SCALE)\
		extern void (*RFCON(NAME,FUNCBASE)[SCALE])(void);\
		extern s32 RFCON(NAME,ACTBASE)[SCALE];\
		extern RFlow_Section_Base NAME;\

#define RFlowSubSec_Source(NAME,SCALE,HIGHSEC,COORD,FREQ)\
		void (*RFCON(NAME,FUNCBASE)[SCALE])(void);\
		s32 RFCON(NAME,ACTBASE)[SCALE]={0};\
		void RFCON(NAME,Func)(void);\
		RFlow_Section_Base NAME((&(RFCON(NAME,FUNCBASE)[0])),&(RFCON(NAME,ACTBASE)[0]),SCALE,&HIGHSEC,COORD,&RFCON(NAME,Func),FREQ);\
		void RFCON(NAME,Func)(void)\
		{\
			NAME=NAME();\
			return;\
		}\

#define RFlowSubSec_Header(NAME,SCALE,HIGHSEC,COORD,FREQ)\
		extern void (*RFCON(NAME,FUNCBASE)[SCALE])(void);\
		extern s32 RFCON(NAME,ACTBASE)[SCALE];\
		void RFCON(NAME,Func)(void);\
		extern RFlow_Section_Base NAME;\


//Class Definition of Block Set
class RFlow_Section_Base
{
public:
	void (**_FuncPtr)(void);
	s32 *_ActStat; //Positive: Loop of Function; Negative: Frequency of Function called when there is a timer;

	u32 _Clock;
	u8 _Scale;

	RFlow_Section_Base *_Sec;
	u8 _Coordinate;

public:
	explicit RFlow_Section_Base(void (**FuncAdr)(void), s32 *ActAdr,u8 InScale):
	_FuncPtr(FuncAdr),_ActStat(ActAdr),_Clock(0),_Scale(InScale),_Sec(NULL),_Coordinate(0)
	{
		return;
	}

	explicit RFlow_Section_Base(void (**FuncAdr)(void), s32 *ActAdr,u8 InScale,RFlow_Section_Base *HigherSec,u8 InCoord, void (*InFunc) (void), s32 InAct):
	_FuncPtr(FuncAdr),_ActStat(ActAdr),
	_Clock(0),_Scale(InScale),
	_Sec(HigherSec),_Coordinate(InCoord)
	{
		(*(_Sec->_FuncPtr+_Coordinate-1))=InFunc;
		(*(_Sec->_ActStat+_Coordinate-1))=InAct;
	}

	~RFlow_Section_Base(){};

	void operator()(u8 InScale)
	{
		(*(_FuncPtr+InScale-1))();
		return;
	}

	RFlow_Section_Base & operator()()	//InClock is the Timer
	{
		u8 ii;
		void (**FuncBase)(void)=_FuncPtr;
		s32 *ActBase=_ActStat;
		s32 ActStash;

		for(ii=0;ii<_Scale;ii++)
		{
			ActStash=*ActBase;
			if(ActStash>0)
			{
				while(ActStash>0)
				{
					(*FuncBase)();
					ActStash--;
				}
			}
			else if(ActStash<0)
			{
				ActStash=-ActStash;
				if((_Clock%ActStash)==0)
				{
					(*FuncBase)();
				}
			}
			FuncBase++;
			ActBase++;
		}

		_Clock++;
		return *this;
	}
};

////////Block Set

////////Block Func

//Macro of Define Closed Block
#define RFlowBlock_Source_Closed_Start(NAME,TYPE1,TYPE2,SEC,FREQ,COORD) \
		void RFCON(NAME,Func)(void);\
		RFlow_Block_Base<TYPE1,TYPE2> NAME(0,&SEC,FREQ,COORD,NULL,0,NULL,0,&RFCON(NAME,Func));\
		void RFCON(NAME,Func)(void)\
		{\

#define RFlowBlock_Source_Closed_End(NAME,TYPE1,TYPE2,SEC,FREQ,COORD) \
			return;\
		};\

#define RFlowBlock_Header_Closed(NAME,TYPE1,TYPE2,SEC,FREQ,COORD) \
		void RFCON(NAME,Func)(void);\
		extern RFlow_Block_Base<TYPE1,TYPE2> NAME;\


//Macro of Define I Block
#define RFlowBlock_Source_I_Start(NAME,TYPE1,TYPE2,INNUM,SEC,FREQ,COORD) \
		TYPE1 *RFCON(NAME,InAdr)[INNUM];\
		void RFCON(NAME,Func)(void);\
		RFlow_Block_Base<TYPE1,TYPE2> NAME(1,&SEC,FREQ,COORD,&RFCON(NAME,InAdr)[0],INNUM,NULL,0,&RFCON(NAME,Func));\
		void RFCON(NAME,Func)(void)\
		{\
			TYPE1 DataIn[INNUM]={0};\
			u8 IOCounter;\
			for(IOCounter=0;IOCounter<INNUM;IOCounter++)\
			{\
				DataIn[IOCounter]=*(RFCON(NAME,InAdr)[IOCounter]);\
			};\

#define RFlowBlock_Source_I_End(NAME,TYPE1,TYPE2,INNUM,SEC,FREQ,COORD)\
			return;\
		};\

#define RFlowBlock_Header_I(NAME,TYPE1,TYPE2,INNUM,SEC,FREQ,COORD) \
		extern TYPE1 *RFCON(NAME,InAdr)[INNUM];\
		void RFCON(NAME,Func)(void);\
		extern RFlow_Block_Base<TYPE1,TYPE2> NAME;\


//Macro of Define O Block
#define RFlowBlock_Source_O_Start(NAME,TYPE1,TYPE2,OUTNUM,SEC,FREQ,COORD) \
		TYPE2 RFCON(NAME,OutVal)[OUTNUM];\
		void RFCON(NAME,Func)(void);\
		RFlow_Block_Base<TYPE1,TYPE2> NAME(2,&SEC,FREQ,COORD,NULL,0,&RFCON(NAME,OutVal)[0],OUTNUM,&RFCON(NAME,Func));\
		void RFCON(NAME,Func)(void)\
		{\
			TYPE2 DataOut[OUTNUM]={0};\
			u8 IOCounter;\

#define RFlowBlock_Source_O_End(NAME,TYPE1,TYPE2,OUTNUM,SEC,FREQ,COORD)\
			for(IOCounter=0;IOCounter<OUTNUM;IOCounter++)\
			{\
				RFCON(NAME,OutVal)[IOCounter]=DataOut[IOCounter];\
			};\
			return;\
		};\

#define RFlowBlock_Header_O(NAME,TYPE1,TYPE2,OUTNUM,SEC,FREQ,COORD) \
		extern TYPE2 RFCON(NAME,OutVal)[OUTNUM];\
		void RFCON(NAME,Func)(void);\
		extern RFlow_Block_Base<TYPE1,TYPE2> NAME;\


//Macro of Define IO Block
#define RFlowBlock_Source_IO_Start(NAME,TYPE1,TYPE2,INNUM,OUTNUM,SEC,FREQ,COORD) \
		TYPE1 *RFCON(NAME,InAdr)[INNUM];\
		TYPE2 RFCON(NAME,OutVal)[OUTNUM];\
		void RFCON(NAME,Func)(void);\
		RFlow_Block_Base<TYPE1,TYPE2> NAME(3,&SEC,FREQ,COORD,&RFCON(NAME,InAdr)[0],INNUM,&RFCON(NAME,OutVal)[0],OUTNUM,&RFCON(NAME,Func));\
		void RFCON(NAME,Func)(void)\
		{\
			TYPE1 DataIn[INNUM]={0};\
			TYPE2 DataOut[OUTNUM]={0};\
			u8 IOCounter;\
			for(IOCounter=0;IOCounter<INNUM;IOCounter++)\
			{\
				DataIn[IOCounter]=*(RFCON(NAME,InAdr)[IOCounter]);\
			};\

#define RFlowBlock_Source_IO_End(NAME,TYPE1,TYPE2,INNUM,OUTNUM,SEC,FREQ,COORD)\
			for(IOCounter=0;IOCounter<OUTNUM;IOCounter++)\
			{\
				RFCON(NAME,OutVal)[IOCounter]=DataOut[IOCounter];\
			};\
			return;\
		};\

#define RFlowBlock_Header_IO(NAME,TYPE1,TYPE2,INNUM,OUTNUM,SEC,FREQ,COORD) \
		extern TYPE1 *RFCON(NAME,InAdr)[INNUM];\
		extern TYPE2 RFCON(NAME,OutVal)[OUTNUM];\
		void RFCON(NAME,Func)(void);\
		extern RFlow_Block_Base<TYPE1,TYPE2> NAME;\


//Class Definition of Function Block
template <class Type1, class Type2>
class RFlow_Block_Base
{
public:
	//Input of the Block
	Type1 **_InletAdr;
	u8 _InletNum;

	//Output of the Block
	Type2 *_OutletAdr;
	u8 _OutletNum;

	u8 _Type;//0: Closed; 1: Inlet Only; 2: Outlet Only 3:Intermediate Type(with both input and output)

	RFlow_Section_Base *_Sec;
	u8 _Coordinate;//Priority Coordinate of the Block

public:
	//Construct a I/O Block
	explicit RFlow_Block_Base(u8 InType, RFlow_Section_Base *InSec, s32 InAct, u8 InCoord, Type1 **IAdr, u8 INum, Type2 *OAdr, u8 ONum, void (*InFunc)(void)):
	_InletAdr(NULL),_InletNum(0),
	_OutletAdr(NULL),_OutletNum(0),
	_Type(InType),
	_Sec(InSec),
	_Coordinate(InCoord)
	{
		switch(_Type)
		{
			case 0:
				break;
			case 1:
				if(INum>0)
				{
					_InletAdr=IAdr;
					_InletNum=INum;
				}
				else
				{
					_Type=0;
				}
				break;
			case 2:
				if(ONum>0)
				{
					_OutletAdr=OAdr;
					_OutletNum=ONum;
				}
				else
				{
					_Type=0;
				}
				break;
			case 3:
				if(INum*ONum>0)
				{
					_InletAdr=IAdr;
					_InletNum=INum;
					_OutletAdr=OAdr;
					_OutletNum=ONum;
				}
				else
				{
					_Type=0;
				}
				break;
			default:
				_Type=0;
				break;
		}

		(*(_Sec->_FuncPtr+_Coordinate-1))=InFunc;
		(*(_Sec->_ActStat+_Coordinate-1))=InAct;
		return;
	};

	//Destructer
	~RFlow_Block_Base(){};

	void operator()(void)
	{
		(*_Sec)(_Coordinate);
		return;
	}

	Type1 InVal (u8 AskNum)
	{
		return **(_InletAdr+AskNum-1);
	}

	Type2 OutVal(u8 AskNum)
	{
		return *(_OutletAdr+AskNum-1);
	}

	Type2 *operator()(u8 AskNum)
	{
		return (_OutletAdr+AskNum-1);
	}

	RFlow_Block_Base<Type1,Type2> & operator()(u8 LinkNum, Type1 *PreOutAdr)
	{
		Type1 **InAdr=_InletAdr+LinkNum-1;
		*InAdr=PreOutAdr;
		return *this;
	}

	RFlow_Block_Base<Type1,Type2> & ShareIn(RFlow_Block_Base<Type1,Type2> ExpBlk)
	{
		this->_InletAdr=ExpBlk._InletAdr;
		this->_InletNum=ExpBlk._InletNum;

		this->_Type+=1;

		return *this;
	}

	RFlow_Block_Base<Type1,Type2> & ShareOut(RFlow_Block_Base<Type1,Type2> ExpBlk)
	{
		this->_OutletAdr=ExpBlk._OutletAdr;
		this->_OutletNum=ExpBlk._OutletNum;

		this->_Type+=2;

		return *this;
	}

	RFlow_Block_Base<Type1,Type2> & Follow(Type1 *PreOutAdr,u8 InStart,u8 OutStart,u8 Num)
	{
		u8 ii;
		for(ii=0;ii<Num;ii++)
		{
			*(_InletAdr+InStart+ii-1)=(PreOutAdr+OutStart+ii-1);
		}
		return *this;
	}
};

////////Block Func

#endif /* ROOT_ROBOFLOW_HPP_ */

#endif
