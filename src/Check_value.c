/************************************************************
* Jianlong Chai chai.4@wright.edu June 6th 2012             *
* This function is used for calibration                     *
* The main purpose of this function is to check if          *
*  (1)The number of records in database exceed 32            *
*  (2)The step is same, but the input position is different  *
*  (3)The previous step is greater than current step, but    *
*	the previous position is less than current position     *
*  (4)The previous step is less than current step, but       *
*	the previous position is greater than current position  *
******************************************************/
#include  <stdio.h>
#include  <unistd.h>
#include  <string.h>
#include  <stdlib.h>
#include <Defs.h>
#include <ExternVars.h>

extern SLEN * getall(int *num, SLEN *bexamp);
extern int setconfig(int index,double updatevalue);
/*************************************************************************************
* checkvalue(long curr_step, double value)  										 *
* Before every calibrate setting, use this function to check the validation          *
*If the table is empty now, then insert the new data directly	return value is 1	 *
*If the table is full, then discard this new data  return value is 0                 * 
**************************************************************************************/
int checkvalue(long curr_step, double value)
{
	int i, num;
	SLEN *check;
	check = getall(&num, check);
	if(num == 0) 
	{
		return 1;
	}
	if(num >= Max_Records) 
	{ 
		//Edit 4/23/2012. If the calibrate table is full, and user want to calibrate more, then it will fresh the previous data
		setconfig(6,0);		
		return 0;
	}
	for(i=0;i<num;i++) 
	{
		if((curr_step > check[i].step) && (curr_step < check[i+1].step) && (value > check[i].length) && (value < check[i+1].length))break;
	}
	if( i < num ) 
	{	
		return 1;
	}
	else if ((i == num) && (curr_step > check[num-1].step) && (value > check[num-1].length))
		 {
			return 1;
		 }
	     else 
		 {
			return -1;
		 }
}
