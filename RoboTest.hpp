/*
 * RoboTest.hpp
 *
 *  Created on: 2017Äê6ÔÂ12ÈÕ
 *      Author: JMech
 */

#ifndef ROBOTEST_HPP_
#define ROBOTEST_HPP_

#include "RoboFlow.hpp"

#include <iostream>

using namespace std;

RFlowTopSec_Header(TopSec,3);
RFlowSubSec_Header(TopSub1,2,TopSec,1,1);
RFlowSubSec_Header(TopSub2,1,TopSec,2,1);
RFlowSubSec_Header(TopSub3,2,TopSec,3,2);
RFlowSubSec_Header(Sub2Sub,2,TopSub2,1,1);

RFlowBlock_Header_O(Outputer1,float,u32,2,TopSub1,1,1)
RFlowBlock_Header_O(Outputer2,float,s32,3,TopSub1,1,2)
RFlowBlock_Header_IO(IOer1,s32,float,3,3,Sub2Sub,1,1)
RFlowBlock_Header_IO(IOer2,u32,float,2,2,Sub2Sub,1,2)
RFlowBlock_Header_IO(IOer3,float,float,5,1,TopSub3,1,1)
RFlowBlock_Header_I(Printer,float,float,1,TopSub3,-2,2)

#endif /* ROBOTEST_HPP_ */
