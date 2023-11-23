//指令：g++ -std=c++11 main_new.cpp algorithm.cpp algorithm_FBH.cpp HPWL.cpp allparser.cpp algorithm_new.cpp -o pl.out
// ./pl.out case12.v case12.lef case12.def case12.mlist case12.txt case12_deviation_q1.dmp case 12.pl case12_deviation_q1.pl
// accept_array[i]： i=0 (Q1/PR25) 、i=1(Q2/PR50)、 i=2(Q3/PR75)


#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <cstring>
#include <map>
#include <cmath>
#include <stdio.h>
#include <algorithm> 
#include "class.h"
using namespace std;

/*
//向中心翻
void place_macro::pin_number_in_macro_for_control(){
	int pin_in_macro_coordinate;
	
  //跑過每顆macro
  vector<Macro>::iterator miter = macrolist.begin();
  for (miter; miter != macrolist.end(); ++miter){
    //找出macro中最多pin的區域---------------------------------------------
    vector<Pin>::iterator piter = miter->pinlist.begin();
    int left_bottom=0;
    int right_bottom=0;
    int left_up=0;
    int right_up=0; //use four variable to save the pin 座標 in the macro 象限 
    for (piter; piter != miter->pinlist.end(); ++piter){
        	
    	
      	if(piter->x < miter->center_x && piter->y < miter->center_y){
  			  left_bottom++; 
   		}
   		else if(piter->x < miter->center_x && piter->y > miter->center_y){
   			left_up++;
   		}
   		else if(piter->x > miter->center_x && piter->y < miter->center_y){
   			right_bottom++;
   		}
   		else if(piter->x > miter->center_x && piter->y > miter->center_y){
   			right_up++;
   		}
   		else if(piter->x == miter->center_x && piter->y < miter->center_y){
   			left_bottom++;
   			right_bottom++;
   		}
   		else if(piter->x ==miter->center_x && piter->y > miter->center_y){
   			left_up++;
   			right_up++;
   		}
   		else if(piter->x < miter->center_x && piter->y == miter->center_y){
   			left_bottom++;
   			left_up++;
   		}
   		else if(piter->x > miter->center_x && piter->y == miter->center_y){
   			right_bottom++;
   			right_up++;
	    	}
    }
    
    //比大小
    int tempt_array[4];
    tempt_array[0]=right_up;
	  tempt_array[1]=left_up;
	  tempt_array[2]=left_bottom;
	  tempt_array[3]=right_bottom;

    int tempt = tempt_array[0];
    int biggest = 0;
    
    for( int i=1;i<=3;i++ ){
      if( tempt_array[i]>tempt ){
        tempt = tempt_array[i];
        biggest = i ;
      }
    }
    
    int flip_operation = FlipMacro( miter->orient , (biggest+1) , macro_in_chip(miter->center_x , miter->center_y) );
    for(int i=0;i<=1;i++){ //macro名字-->暫存的[方向結果][flip_operation]
        if(i==0) compare_map[miter->name][i] = update_new_orient( flip_operation , miter->orient );
        else if(i==1) compare_map[miter->name][i] = flip_operation ;
    }
    
    //miter->orient = FlipMacro( miter->orient , (biggest+1) , macro_in_chip(miter->center_x , miter->center_y) );
	}	
} 
*/
//用macrolist_fake來跑areaOPT的向中間翻版本

//用假的macrolist跑，結果也存在假的macrolist_fake裡面
void place_macro::pin_number_in_macro_for_control(){
	int pin_in_macro_coordinate;
	
  //跑過每顆macro
    vector<Macro>::iterator miter = macrolist_fake.begin();
    for (miter; miter != macrolist_fake.end(); ++miter){
        //找出macro中最多pin的區域---------------------------------------------
        vector<Pin>::iterator piter = miter->pinlist.begin();
        int left_bottom=0;
        int right_bottom=0;
        int left_up=0;
        int right_up=0; //use four variable to save the pin 座標 in the macro 象限 
        for (piter; piter != miter->pinlist.end(); ++piter){
          	
          	if(piter->x < miter->center_x && piter->y < miter->center_y){
      			  left_bottom++; 
       		}
       		else if(piter->x < miter->center_x && piter->y > miter->center_y){
       			left_up++;
       		}
       		else if(piter->x > miter->center_x && piter->y < miter->center_y){
       			right_bottom++;
       		}
       		else if(piter->x > miter->center_x && piter->y > miter->center_y){
       			right_up++;
       		}
       		else if(piter->x == miter->center_x && piter->y < miter->center_y){
       			left_bottom++;
       			right_bottom++;
       		}
       		else if(piter->x ==miter->center_x && piter->y > miter->center_y){
       			left_up++;
       			right_up++;
       		}
       		else if(piter->x < miter->center_x && piter->y == miter->center_y){
       			left_bottom++;
       			left_up++;
       		}
       		else if(piter->x > miter->center_x && piter->y == miter->center_y){
       			right_bottom++;
       			right_up++;
        	}
        }
    
        //比大小
        int tempt_array[4];
        tempt_array[0]=right_up;
 	    tempt_array[1]=left_up;
   	    tempt_array[2]=left_bottom;
    	tempt_array[3]=right_bottom;
    
        int tempt = tempt_array[0];
        int biggest = 0;
        
        for( int i=1;i<=3;i++ ){
            if( tempt_array[i]>tempt ){
                tempt = tempt_array[i];
                biggest = i ;
            }
        }
        
        if(miter->fixed==0){
            //compare_map[miter->name]=FlipMacro( miter->orient , (biggest+1) , macro_in_chip(miter->center_x , miter->center_y) );
            int flip_operation = FlipMacro( miter->orient , (biggest+1) , macro_in_chip(miter->center_x , miter->center_y) );
            miter->orient = update_new_orient(flip_operation,miter->orient);
            update_one_macro_fake(miter->name,flip_operation);
        }
	}	
} 



//areaOPT用macrolist_fake跑的，最終將結果存到compare_map裡面
void place_macro::area_Optimization(){
    string wire_name;
    
    bool already_hor, already_ver;  //暫存是否翻轉
    vector<XY> xy_tempt;
    //vector<Macro>::iterator miter=macrolist.begin();
    map<double, vector<string> >:: iterator areaiter ; 
    
    //for(miter; miter != macrolist.end(); ++miter){
    
    for(auto areaiter=areamap.rbegin() ; areaiter!=areamap.rend(); ++areaiter){
        vector<string>::iterator miter=areaiter->second.begin();
        for(miter; miter!=areaiter->second.end(); ++miter){  //miter是跑areamap後面的vector<Macro>用!!!!!
            vector<Macro>::iterator macroiter=macrolist_fake.begin();
            for(macroiter=macrolist_fake.begin(); macroiter!=macrolist_fake.end(); ++macroiter){
                if(macroiter->name == *miter && macroiter->fixed==0){
                    cout<<"Area : "<<areaiter->first<<endl;
                    cout<<"Macro Name : "<<endl;
                    cout<<macroiter->name<<endl;
                    
                    int ver_external_big = 0;
                    int hor_external_big = 0;
                    int ver_internal_big = 0;
                    int hor_internal_big = 0;
                    int ver_result[2] = {0,0};
                    int hor_result[2] = {0,0};
                    int connection = 0;
                    //cout<<"Macro : "<<miter->name<<endl;
            
        
                    macroiter->macronum=0;
                    macroiter->stdnum=0;
                    vector<Pin>::iterator piniter=macroiter->pinlist.begin();
                    hor_internal_big = judge_pin_x_orient(macroiter->pinlist, macroiter->center_x);
                    ver_internal_big = judge_pin_y_orient(macroiter->pinlist, macroiter->center_y);
                    for(piniter; piniter!=macroiter->pinlist.end(); ++piniter){
                        //cout<<"origin pin : "<<piniter->x<<"  "<<piniter->y<<endl;
                        wire_name=find_pin(macroiter->name, piniter->name); 
                        if(find_connected_pin(wire_name, piniter->x, piniter->y).size()!=0){
                          xy_tempt=find_connected_pin(wire_name, piniter->x, piniter->y);
                          vertical_external(macroiter->center_y, xy_tempt,ver_result);          
                          horizontal_external(macroiter->center_x, xy_tempt, hor_result);
                          connection++;
                          (macroiter->macronum)++;
                        }
                        else (macroiter->stdnum)++;
                                    //印pin檢查/////
                        //for(int i=0; i<xy_tempt.size(); i++){ //xy_tempt.size()=1代表那個macro pin只接到std cell
                        //    cout<<"x: "<<xy_tempt[i].x<<" y: "<<xy_tempt[i].y<<endl;
                        //}
                        //cout<<endl;
                    }
                    if(connection==0){
                      cout<<"Didn't connected to any macro !!!"<<endl<<endl<<endl; 
                      break;
                    }      
                    if(ver_result[1]>ver_result[0])
                        ver_external_big=1;
                    else if(ver_result[1]<ver_result[0])
                        ver_external_big=2;
                    else
                        ver_external_big=3;
                    if(hor_result[1]>hor_result[0])
                        hor_external_big=1;
                    else if(hor_result[1]<hor_result[0])
                        hor_external_big=2;
                    else
                        hor_external_big=3;
                    //測試
                    //cout<<"=========> Internal comparison : horizontal = "<<hor_internal_big<<" , vertical= "<<ver_internal_big<<endl;
                    //cout<<"External: Right = "<<hor_result[1]<<" ,Left = "<<hor_result[0]<<endl;
                    //cout<<"          Up    = "<<ver_result[1]<<" ,Down  = "<<ver_result[0]<<endl;
                    //cout<<"=========> External comparison : horizontal = "<<hor_external_big<<" , vertical= "<<ver_external_big<<endl;
                    //cout<<"Number of pins = "<<miter->pinlist.size()<<endl;
                    //cout<<"The origin orient is "<<miter->orient;
                    
                    cout<<"The origin orient is "<<macroiter->orient; 
                    already_hor=0;
                    already_ver=0;
                    if((hor_external_big==1&&hor_internal_big==2)||(hor_external_big==2&&hor_internal_big==1)){
                        macroiter->orient = update_new_orient(1, macroiter->orient);
                        update_one_macro_fake(macroiter->name, 1);
                        cout<<" ======> New orient after horrizontal flipping is "<<macroiter->orient;
                        already_hor=1;
                    }
                    else{
                        cout<<" ======> No horrizontal flipping!!!";
                        already_hor=0;
                    }
                    if((ver_external_big==1&&ver_internal_big==2)||(ver_external_big==2&&ver_internal_big==1)){
                        macroiter->orient = update_new_orient(2, macroiter->orient);
                        update_one_macro_fake(macroiter->name, 2);
                        cout<<" ======> New orient after vertical flipping is "<<macroiter->orient<<endl<<endl<<endl;
                        already_ver=1;
                    }
                    else{   
                        cout<<" ======> No vertical flipping!!!"<<endl;
                        already_ver=0;
                    }
                    //result_test[macroiter->name]=macroiter->orient;
                    int flip_operation=0;
                    if(already_hor==1 && already_ver==1) flip_operation=3;
                    else if(already_hor==1 && already_ver==0) flip_operation=1;
                    else if(already_hor==0 && already_ver==1) flip_operation=2;
                    else flip_operation=0;
                    compare_map[macroiter->name][0]=macroiter->orient; 
                    compare_map[macroiter->name][1]=flip_operation; 
                    cout<<"macro name : "<<macroiter->name<<"  macro orientation : "<<macroiter->orient<<endl;    
                    break;                    
                } //end if 對照macro名字
            } //end macroiter for
         } //end miter for
    } //end areaiter for
}



//原先跑stdcell的function改的
void place_macro::pin_connection_in_macro(){
    double accept_array[3];
    calculate_deviation_deviation(accept_array);
    ////////////////////////
    cout<<"After return : ";
    for(int i=0; i<3; i++)cout<<accept_array[i]<<" ";
    cout<<endl;
    ////////////////////////


	int pin_in_macro_coordinate;
    //跑過每顆macro
    vector<Macro>::iterator miter = macrolist.begin();
    for (miter; miter != macrolist.end(); ++miter){
       if(miter->fixed==0){
           //找出macro中最多pin的區域---------------------------------------------
           vector<Pin>::iterator piter = miter->pinlist.begin();
           int left_bottom=0;
           int right_bottom=0;
           int left_up=0;
           int right_up=0; //use four variable to save the pin 座標 in the macro 象限 
           for (piter; piter != miter->pinlist.end(); ++piter){	
             	if(piter->x < miter->center_x && piter->y < miter->center_y){
         			left_bottom += connectmap[ return_wirename(miter->name, piter->name) ]; 
          		}
          		else if(piter->x < miter->center_x && piter->y > miter->center_y){
          			left_up += connectmap[ return_wirename(miter->name, piter->name) ]; 
          		}
          		else if(piter->x > miter->center_x && piter->y < miter->center_y){
          			right_bottom += connectmap[ return_wirename(miter->name, piter->name) ];
          		}
          		else if(piter->x > miter->center_x && piter->y > miter->center_y){
          			right_up += connectmap[ return_wirename(miter->name, piter->name) ];
          		}
          		else if(piter->x == miter->center_x && piter->y < miter->center_y){
          			left_bottom += connectmap[ return_wirename(miter->name, piter->name) ];
          			right_bottom += connectmap[ return_wirename(miter->name, piter->name) ];
          		}
          		else if(piter->x ==miter->center_x && piter->y > miter->center_y){
          			left_up += connectmap[ return_wirename(miter->name, piter->name) ];
          			right_up += connectmap[ return_wirename(miter->name, piter->name) ]; 
          		}
          		else if(piter->x < miter->center_x && piter->y == miter->center_y){
          			left_bottom += connectmap[ return_wirename(miter->name, piter->name) ]; 
          			left_up += connectmap[ return_wirename(miter->name, piter->name) ];
          		}
          		else if(piter->x > miter->center_x && piter->y == miter->center_y){
          			right_bottom += connectmap[ return_wirename(miter->name, piter->name) ];
          			right_up += connectmap[ return_wirename(miter->name, piter->name) ]; 
       	    }
           }
           
           
           
           //比大小
           int tempt_array[4];
           double difference;  //save deviation / average
           
           tempt_array[0]=right_up;
           tempt_array[1]=left_up;
           tempt_array[2]=left_bottom;
           tempt_array[3]=right_bottom;
           difference=deviation(tempt_array); 
           
           int flip_operation;
           //0327 test accept_array[2]->0.85
           if(difference>=accept_array[1]){ //做std cell翻轉
               for(int i=0;i<=3;i++ )cout<<tempt_array[i]<<"  ";
               cout<<endl;
               int tempt = tempt_array[0];
               int biggest = 0;
           
               for( int i=1;i<=3;i++ ){
                   if( tempt_array[i]>tempt ){
                       tempt = tempt_array[i];
                       biggest = i ;
                   }
               }
               cout<<"macro name : "<<miter->name<<endl;
               cout<<"!!!   Flip Way : pin weight"<<endl;
               cout<<"before flipping : "<<miter->orient<<endl;
               cout<<"macro location : "<<macro_in_chip(miter->center_x, miter->center_y)<<endl;
               flip_operation = FlipMacro( miter->orient , (biggest+1) , macro_in_chip(miter->center_x , miter->center_y) );
               miter->orient = update_new_orient( flip_operation , miter->orient );
               update_one_macro(miter->name, flip_operation);
               cout<<"after flipping : "<<miter->orient<<endl<<endl;       
           }
           else{ //做向中間翻轉
           cout<<"macro name : "<<miter->name<<endl;
               cout<<"!!!   Flip Way : pin toward to chip center + areaOPT"<<endl;
               cout<<"before flipping : "<<miter->orient<<endl;
               miter->orient=compare_map[miter->name][0];
               update_one_macro(miter->name, compare_map[miter->name][1]);  
               cout<<"after flipping : "<<miter->orient<<endl<<endl;       
           }
        }
	}	
} 

void place_macro::calculate_deviation_deviation(double return_array[3]){
    int pin_in_macro_coordinate;
    double deviation_sum=0;
    //跑過每顆macro
    vector<Macro>::iterator miter = macrolist.begin();
    for (miter; miter != macrolist.end(); ++miter){
        if(miter->fixed==0){
            //找出macro中最多pin的區域---------------------------------------------
            vector<Pin>::iterator piter = miter->pinlist.begin();
            int left_bottom=0;
            int right_bottom=0;
            int left_up=0;
            int right_up=0; //use four variable to save the pin 座標 in the macro 象限 
            for (piter; piter != miter->pinlist.end(); ++piter){	
              	if(piter->x < miter->center_x && piter->y < miter->center_y){
          			left_bottom += connectmap[ return_wirename(miter->name, piter->name) ]; 
           		}
           		else if(piter->x < miter->center_x && piter->y > miter->center_y){
           			left_up += connectmap[ return_wirename(miter->name, piter->name) ]; 
           		}
           		else if(piter->x > miter->center_x && piter->y < miter->center_y){
           			right_bottom += connectmap[ return_wirename(miter->name, piter->name) ];
           		}
           		else if(piter->x > miter->center_x && piter->y > miter->center_y){
           			right_up += connectmap[ return_wirename(miter->name, piter->name) ];
           		}
           		else if(piter->x == miter->center_x && piter->y < miter->center_y){
           			left_bottom += connectmap[ return_wirename(miter->name, piter->name) ];
           			right_bottom += connectmap[ return_wirename(miter->name, piter->name) ];
           		}
           		else if(piter->x ==miter->center_x && piter->y > miter->center_y){
           			left_up += connectmap[ return_wirename(miter->name, piter->name) ];
           			right_up += connectmap[ return_wirename(miter->name, piter->name) ]; 
           		}
           		else if(piter->x < miter->center_x && piter->y == miter->center_y){
           			left_bottom += connectmap[ return_wirename(miter->name, piter->name) ]; 
           			left_up += connectmap[ return_wirename(miter->name, piter->name) ];
           		}
           		else if(piter->x > miter->center_x && piter->y == miter->center_y){
           			right_bottom += connectmap[ return_wirename(miter->name, piter->name) ];
           			right_up += connectmap[ return_wirename(miter->name, piter->name) ]; 
        	    }
            }
            
            //比大小
            int tempt_array[4];
            double difference;  //save deviation / average
            tempt_array[0]=right_up;
            tempt_array[1]=left_up;
            tempt_array[2]=left_bottom;
        	tempt_array[3]=right_bottom;
            difference=deviation(tempt_array);
            cout<<"macro name : "<<miter->name<<"  deviation : "<<difference<<endl;
            //vector<double>deviation_vec;
            deviation_vec.push_back(difference);
        }
    }
    bubbleSort(deviation_vec);
    cout<<"deviation_vec begin : "<<endl;
    for(int i=0; i<deviation_vec.size(); i++)cout<<deviation_vec[i]<<endl;
    cout<<"deviation_vec end : "<<endl;
    cout<<"Before return : "<<endl;
    int tempt=0;
    for(int i=0; i<3; i++){
        tempt=deviation_vec.size()*0.25*(i+1);
        return_array[i]=deviation_vec[tempt];
        cout<<return_array[i]<<"  ";
    }
    //return_array[2]=deviation_vec.size()*0.85;
    cout<<endl;
    //return return_array;
}

double place_macro::deviation(int array[4]){
    double average=0;
    double total=0;
    double dev=0;
    for(int i=0; i<4; i++){ average=array[i]/4 + average; }
    for(int i=0; i<4; i++){
        total=total+pow(array[i]-average, 2);   
    }
    dev=0.5*sqrt(total);
    return dev;
}

// bubble sort vector
void place_macro::bubbleSort(vector<double>& intVec) {
    size_t size = intVec.size();
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size - 1; ++j) {
            if (intVec[j] > intVec[j + 1]) {
                double tmp = intVec[j];

                intVec[j] = intVec[j + 1];
                intVec[j + 1] = tmp;
            }
        }
    }
}

void place_macro::update_macrolist_fake(){
    Macro data;
    Pin data_pin;
    int counter=0;
    int counter2=0;
    vector<Macro>::iterator miter=macrolist.begin();
    for(miter; miter!=macrolist.end(); ++miter){
        counter++;
        data.pinlist.clear();
        data.name=miter->name;
        data.orient=miter->orient;
        data.center_x=miter->x + 0.5* miter->width;
        data.center_y=miter->y + 0.5* miter->height;
        data.x=miter->x;
        data.y=miter->y;
        counter2=0;
        vector<Pin>::iterator piter=miter->pinlist.begin();
        for(piter; piter!=miter->pinlist.end(); ++piter){
            
            data_pin.name=piter->name;
            data_pin.x=piter->x;
            data_pin.y=piter->y;
            if(counter<5 && counter2<3){
                cout<<"pin name : "<<data_pin.name<<"  pin x : "<<data_pin.x<<"  pin y : "<<data_pin.y<<endl;
                counter2++;
            }
            data.pinlist.push_back(data_pin);
        }
        
        vector<Pin>::iterator piter_new=miter->pinlist.begin();
        for(piter_new; piter_new!=miter->pinlist.begin()+3; ++piter_new){
            cout<<"pin name : "<<piter_new->name<<"  pin x : "<<piter_new->x<<"  pin y : "<<piter_new->y<<endl;
        }
        
        macrolist_fake.push_back(data);
    }
}


void place_macro::fixed_big_macros(){
    vector<Macro>::iterator miter=macrolist.begin();
    for(miter=macrolist.begin(); miter!=macrolist.end(); ++miter){
        //固定大macro
        if(miter->type=="block_4449x4239_2703")
            miter->fixed=1;
        else{
            //固定cluster macros
            //vector<string>::iterator cmiter=cluster_macros.begin();
            //for(cmiter=cluster_macros.begin(); cmiter!=cluster_macros.end(); ++cmiter)
                //if(miter->name==*cmiter) miter->fixed=1;
        }
    }
    
    miter=macrolist_fake.begin();
    for(miter=macrolist_fake.begin(); miter!=macrolist_fake.end(); ++miter){
        if(miter->type=="block_4449x4239_2703")    
            miter->fixed=1;
        else{
            //固定cluster macros
            //vector<string>::iterator cmiter=cluster_macros.begin();
            //for(cmiter=cluster_macros.begin(); cmiter!=cluster_macros.end(); ++cmiter)
                //if(miter->name==*cmiter) miter->fixed=1;
        }
    }

}