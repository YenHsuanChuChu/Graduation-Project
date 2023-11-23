 #include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <cstring>
#include <map>
#include <stdio.h>
#include "class.h"

using namespace std;

void place_macro::flip_by_HPWL(){
  int counter=0;
  map<double, vector<string> >:: iterator HPWLiter ;
  map<string, vector<Data> >::iterator wireiter = wiremap.begin();
  //vector<Data>::iterator dataiter=wirelist.begin();
  double a,b,c,d;
  double array[4];
  double tempt_x,tempt_y;
  int tempt_orient;
  int flip_operation=0;
  
  for(auto HPWLiter = HPWLmap.rbegin();HPWLiter!=HPWLmap.rend();++HPWLiter) //由大HPWL取到小
  {
    //cout<<"hpwl size: "<<(HPWLiter->second).size()<<endl;
    vector<string>::iterator innerit=HPWLiter->second.begin(); //innerit-->second(線名)
    for(innerit; innerit!=HPWLiter->second.end(); ++innerit)
    {
      for(wireiter=wiremap.begin();wireiter!=wiremap.end();++wireiter) //wireiter(wiremap)
      {
        if(*innerit==wireiter->first)
        {    
            //做2個vector去抓單條wire上連接到的所有pin座標，計算單科macro翻轉後對此條線的hpwl影響
            vector<Macro>::iterator miter=macrolist.begin();  //macrolist 
            for(miter; miter != macrolist.end(); ++miter)
            { 
              cout<<miter->name<<", 1  Before flipping, orient = "<<miter->orient<<"   "<<endl; 
              //******************8
              vector<Data>::iterator dataiter=wireiter->second.begin();
              for(dataiter;dataiter!=wireiter->second.end(); ++dataiter)
              {
                if((miter->name==dataiter->macro_name) && miter->fixed==0) // macrolist 與 wiremap 的wire 相同就去翻他，確定這個macro在現在看的wire上
                {    
                    
                  for(int i=0;i<=3;i++)
                  {
                      array[i] = one_HPWL(wireiter->first , miter->name , i , miter->center_x , miter->center_y);
                  }//end for(inti=0)       
                      a=array[0];
                      b=array[1];
                      c=array[2];
                      d=array[3];
                      //cout<<a<<" "<<b<<" "<<c<<" "<<d<<endl;
                      cout<<"mode 0: "<<a<<"  mode 1: "<<b<<"  mode 2: "<<c<<"  mode 3: "<<d<<endl;
                      //************************************************
                      for(int k=0; k<3; k++)
                      {
                          for(int j=0; j<3-k ; j++)
                          {
                              if( array[j] > array[j+1])
                              {   double tmp=0;
                                  tmp = array [j];
                                  array[j] = array[j+1];
                                  array[j+1] = tmp;
                              }
                          }
                      }
                      cout<<"2  Before flipping, orient = "<<miter->orient<<"   ";
                      //************************************************
                      //決定好翻轉操作並翻轉
                      
                      //***********************************try
                      if(array[0]==b)//水平翻
                          flip_operation=1;
                      else if(array[0]==c)//水平垂直都翻
                          flip_operation=3;  
                      else if(array[0]==d)//垂直翻
                          flip_operation=2;
                      else if(array[0]==a)
                          cout<<"Didn't flip!"<<endl;
                      //***********************************try
                      cout<<"!!! Before flipping, orient = "<<miter->orient<<"   ";
                      switch(flip_operation) {
                    	  case 1: //水平翻轉 
                    		    cout<<"Thus, we doing flipping by horizontal."<<endl;
                            if(miter->orient==0) miter->orient = 1;
                    		    else if(miter->orient==1) miter->orient = 0;
                    		    else if(miter->orient==2) miter->orient = 3;
                    		    else if(miter->orient==3) miter->orient = 2;    		
                    		    break;
                    	  case 2: //垂直翻轉 
                    		    cout<<"Thus, we doing flipping by vertical."<<endl;
                            if(miter->orient==0) miter->orient = 3;
                    		    else if(miter->orient==1) miter->orient = 2;
                    		    else if(miter->orient==2) miter->orient = 1;
                    		    else if(miter->orient==3) miter->orient = 0;    		
                    		    break;
                    	  case 3: //垂直和水平都翻 
                    		    cout<<"Thus, we doing flipping by both horizontal and vertical."<<endl;
                            if(miter->orient==0) miter->orient = 2;
                    		    else if(miter->orient==1) miter->orient = 3;
                    		    else if(miter->orient==2) miter->orient = 0;
                    		    else if(miter->orient==3) miter->orient = 1;	
                    		    break;
                    	  default:
                            cout<<"Thus, we don't doing flipping."<<endl;
                    		    break;
               	  }//end switch
                    
                    //*********0224**************//
                    miter->fixed=1;
                    
                    //vector<Pin>::iterator piniter=miter->pinlist.begin();
                    //for(piniter=miter->pinlist.begin(); piniter!=miter->pinlist.end(); ++piniter) cout<<"Before flipping, pin coordinate x: "<<piniter->x<<" y: "<<piniter->y<<endl;
                    //cout<<"After flipping, orient = "<<miter->orient<<endl;
                    //cout<<"The flipping orientation: "<<miter->orient<<endl;
                    update_one_macro(miter->name,flip_operation);
                    //for(piniter=miter->pinlist.begin(); piniter!=miter->pinlist.end(); ++piniter) cout<<"After flipping, pin coordinate x: "<<piniter->x<<" y: "<<piniter->y<<endl;
                }
              } 
              //*****
           }            
        }
      }
    }
  }
  //cout<<endl<<"counter: "<<counter<<endl;
}
/////////////////////////////////////////////////////////////////////////FBH_new/////////////////////////////////////////////////////////////////

void place_macro::FBH_new(){
    
    map<double, vector<string> >:: iterator HPWLiter = HPWLmap.begin();
    map<string, vector<Data> >::iterator wireiter = wiremap.begin();
    vector<string>::iterator innerit=HPWLiter->second.begin(); //innerit-->second(線名)
    
    vector<string> wirename_vec; //暫存要刪除的線名
    vector<string>::iterator veciter=wirename_vec.begin();
    
    end_t = clock();
    total_t = (end_t - start_t)/CLOCKS_PER_SEC;
     
    //while(HPWLmap.size()>0){
    while(total_t<1140){
        auto HPWLiter = HPWLmap.rbegin();
        cout<<"Before FBH_new, wiremap.size = "<<wiremap.size()<<endl;
        cout<<"HPWL : "<<HPWLiter->first<<endl;
        cout<<"HPWL vector size : "<<HPWLiter->second.size()<<endl;
        for(innerit=HPWLiter->second.begin(); innerit!=HPWLiter->second.end(); ++innerit){
            FBH_new_single_wire(*innerit);
            wirename_vec.push_back(*innerit);
            //cout<<*innerit<<" ";
        }
        //cout<<endl;
        
        wiremap.clear(); //清除
        find_coordinate_construct_map(); //重抓pin資料,重建wiremap
        for(veciter=wirename_vec.begin(); veciter!=wirename_vec.end(); ++veciter){
            wiremap.erase(*veciter); //將上面處理完的線擦掉
            
            /*if(wirename_vec.size()==0) cout<<"veciter.size()==0!!!"<<endl;
            else cout<<"*veciter"<<*veciter<<" ";*/
        }
        //cout<<endl;
        cout<<"After FBH_new, wiremap.size = "<<wiremap.size()<<endl;
        HPWLmap.clear(); //清空HPWLmap
        calculate_HPWL(); //重建HPWLmap
        
        end_t = clock(); //抓做完一輪的時間點
        total_t = (end_t - start_t)/CLOCKS_PER_SEC;
    }
    cout<<"total run time after FBH : "<<total_t<<endl;
    
}


void place_macro::FBH_new_single_wire(string wire_name){ //innerit指到的wire
    vector<Data>data_tempt;
    vector<Pin>small_pinlist;
    vector<XY>xy;
    string wire_tempt;
    int macro_orientation;
    int flip_operation;
    double HPWL_result[4]={0, 0, 0, 0};
    double HPWL_register[4]={0, 0, 0, 0};
    
    data_tempt=find_wire_data(wire_name); //找出那條線後面的所有關聯macro資料
    vector<Data>::iterator dataiter=data_tempt.begin();
    cout<<"wire name: "<<wire_name<<endl;
    for(dataiter; dataiter!=data_tempt.end(); ++dataiter){
        double HPWL_result[4]={0, 0, 0, 0};
        cout<<dataiter->macro_name<<" : "<<endl;
        //cout<<dataiter->pin_name<<" : "<<endl;
        int quit=0;
        vector<Macro>::iterator miter=macrolist.begin();
        for(miter; miter!=macrolist.end(); ++miter)
            if(dataiter->macro_name==miter->name && miter->fixed==1)
                quit=1;
        if(quit==1)
            continue;
        small_pinlist=single_macro_pinlist(dataiter->macro_name); //找出單顆macro上面所有的pin資料
        vector<Pin>::iterator piniter=small_pinlist.begin();
        for(piniter; piniter!=small_pinlist.end(); ++piniter){ //跑過所有的pin
            wire_tempt=find_pin(dataiter->macro_name, piniter->name); //找出單一pin連接到的線
            xy=find_connected_pin(wire_tempt, piniter->x , piniter->y ); //透過pin連接到的線名，找出有單條線上的所有pin位置
            cout<<"Pin: "<<piniter->name<<endl;
            cout<<"The number of pins on the wire "<<wire_tempt<<" = "<<xy.size()<<endl;
            for(int i=0; i<4; i++){
                one_HPWL_FBH(i, dataiter->macro_name, xy, piniter->x, piniter->y, HPWL_result); //依序儲存四種方式翻轉後的結果
            }
        }
            //HPWL_register=HPWL_result;
            for(int i=0; i<4; i++){
                HPWL_register[i]=HPWL_result[i]; //暫存比較用的資料
                cout<<"HPWL_register["<<i<<"]= "<<HPWL_register[i]<<endl;
            }
            bubble_sort(HPWL_result);
            cout<<"HPWL_result[0] = "<<HPWL_result[0]<<endl;
            //找出最佳翻轉方向
            if(HPWL_result[0]==HPWL_register[0])flip_operation=0;
            else if(HPWL_result[0]==HPWL_register[1])flip_operation=1;
            else if(HPWL_result[0]==HPWL_register[2])flip_operation=3;
            else if(HPWL_result[0]==HPWL_register[3])flip_operation=2;
            
            //更新該macro資料(方向,fixed)
            
            for(miter=macrolist.begin(); miter!=macrolist.end(); ++miter){
                if(dataiter->macro_name==miter->name && miter->fixed==0) {
                    cout<<"Old orientation after FBH = "<<miter->orient<<endl;
                    cout<<"Flip_operation = "<<flip_operation<<endl;
                    miter->orient = update_new_orient(flip_operation, miter->orient);
                    update_one_macro(dataiter->macro_name,flip_operation); //更新macro方向
                    miter->fixed = 1;
                    cout<<"New orientation after FBH = "<<miter->orient<<endl<<endl<<endl;
                }
            }  
            
            //計算單條線的HPWL的函式
            //回傳HPWL並加總所有線長
            //跑完4種方向，並決定flip_operation
            //更新pinlist        
        
    }
}

//找到這條線相關聯的資料
vector<Data> place_macro::find_wire_data(string wire_name){
    map<string, vector<Data> >::iterator mapiter=wiremap.begin();
    for(mapiter; mapiter!=wiremap.end(); ++mapiter){
        if(wire_name==mapiter->first) return mapiter->second;
    }
} 


vector<Pin> place_macro::single_macro_pinlist(string macro_name){
    vector<Pin>small_pinlist;
    vector<Macro>::iterator miter=macrolist.begin();
    for(miter; miter!=macrolist.end(); ++miter){
        if(macro_name==miter->name) small_pinlist=miter->pinlist;
    }
    return small_pinlist;
}


void place_macro::one_HPWL_FBH(int i, string macro_name, vector<XY>xy, double x, double y, double HPWL_tempt[4]){
    //cout<<"mode : "<<i<<endl;
    //cout<<"macro name : "<<macro_name<<endl;
    double tempt_x, tempt_y;
    double center_x, center_y;
    vector<Macro>::iterator miter=macrolist.begin();
    for(miter; miter!=macrolist.end(); ++miter){
        if(miter->name==macro_name){
            center_x=miter->center_x;
            center_y=miter->center_y;
        }
    }
    //cout<<"x : "<<x<<"  : "<<y<<endl;
    //cout<<"center_x : "<<center_x<<"  center_y : "<<center_y<<endl;
    switch(i){
        case 0:
            cout<<"Orient = 0"<<endl;
            tempt_x=x;
            tempt_y=y; 
            HPWL_tempt[0]=HPWL_tempt[0]+real_HPWL(xy, tempt_x, tempt_y);  
            //cout<<"tempt x : "<<tempt_x<<"  tempt y : "<<tempt_y<<endl;
        break;
        case 1:
            cout<<"Orient = 1"<<endl;
            tempt_x=2*center_x-(x);
            tempt_y=y;
            HPWL_tempt[1]=HPWL_tempt[1]+real_HPWL(xy, tempt_x, tempt_y);  
            //cout<<"tempt x : "<<tempt_x<<"  tempt y : "<<tempt_y<<endl;
        break;
        case 2:
            cout<<"Orient = 2"<<endl;
            tempt_x=2*center_x-(x);
            tempt_y=2*center_y-(y);
            HPWL_tempt[2]=HPWL_tempt[2]+real_HPWL(xy, tempt_x, tempt_y);  
            //cout<<"tempt x : "<<tempt_x<<"  tempt y : "<<tempt_y<<endl;
        break;
        case 3:
            cout<<"Orient = 3"<<endl;
            tempt_x=x;
            tempt_y=2*center_y-(y); 
            HPWL_tempt[3]=HPWL_tempt[3]+real_HPWL(xy, tempt_x, tempt_y);  
            //cout<<"tempt x : "<<tempt_x<<"  tempt y : "<<tempt_y<<endl;     
        break; 
    }
}


double place_macro::real_HPWL(vector<XY>xy, double x, double y){
    double max=x, min=x;
    double diff_x, diff_y;
    vector<XY>::iterator it=xy.begin();
    cout<<fixed<<"(x,y) of pin on the macro = ("<<x<<" , "<<y<<")"<<endl;
    for(it;it!=xy.end();++it){
      cout<<"(x,y) of pins on the wire = ("<<it->x<<" , "<<it->y<<")\n";
    }
    for(it=xy.begin(); it!=xy.end(); ++it){
        if(it->x>max) max=it->x;
        else if(it->x<min) min=it->x;
    }
    cout<<"====> max: "<<max<<" min: "<<min<<"\n";
    diff_x=max-min;
    
    max=y, min=y;
    for(it=xy.begin(); it!=xy.end(); ++it){
        if(it->y>max) max=it->y;
        else if(it->y<min) min=it->y;
    }
    diff_y=max-min;
    
    return diff_x+diff_y;
}

void place_macro::bubble_sort(double array[4]){
    double tmp=0;
    for(int k=0; k<3; k++){
        for(int j=0; j<3-k ; j++){
            if( array[j] > array[j+1]){   
                tmp=0;
                tmp = array [j];
                array[j] = array[j+1];
                array[j+1] = tmp;
            }
        }
    }
}


