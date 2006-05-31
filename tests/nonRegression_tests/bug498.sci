// bug498 

mode (-1)
clear

correct=%F
affich_result( min(2,uint16(1))   ==    1.  , 498.1)
affich_result( min(2,int16(1))    ==    1.  , 498.2)
affich_result( min(2,int8(1))     ==    1.  , 498.3)
affich_result( min(2,int8(10))    ==    2.  , 498.4)
affich_result( min(2,uint8(1))    ==    1.  , 498.5)
affich_result( max(uint8(10),2)   ==    10.  , 498.6)
affich_result( max(int8(10),2)    ==    10.  , 498.7)
affich_result( max(int8(10),-2)   ==    10.  , 498.8)
affich_result( max(int8(10),-2.6) ==    10.  , 498.9)
affich_result( min(int8(10),-2.6) ==  - 2.6 , 498.11)

clear

//min(2,uint16(1))   //WRONG  		ans  =    2.  
//min(2,int16(1))    //OK 			ans  =    1.  
//min(2,int8(1))     //OK 			ans  =    1.  
//min(2,int8(10))    //OK 			ans  =    2.  
//min(2,uint8(1))    //WRONG 			ans  =    2.  
//max(uint8(10),2)   //WRONG 			ans  =    2.  
//max(int8(10),2)    //WRONG 			ans  =    2.  
//max(int8(10),-2)   //WRONGER 		ans  =    2.  
//max(int8(10),-2.6) //EVEN WRONGER 	ans  =    2.  
//min(int8(10),-2.6) //OK 			ans  =  - 2.6  

