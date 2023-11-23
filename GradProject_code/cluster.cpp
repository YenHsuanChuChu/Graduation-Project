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


#include <time.h>
#include <stdio.h>
#include <cmath>
#include "class.h"

using namespace std;

//可以透過調變cluster_standard_num改變"線上多少macro以上才算cluster"
void place_macro::bounding_box_percentage(int argc, char* argv[]){ 
    ofstream outputfile(argv[9],ios::out);
    string wire_name;
    double x_min, x_max;
    double y_min, y_max;
    double bounding_area;
    double chip_area=(chip_x_center*2)*(chip_y_center*2);
    double percentage;
    double score=0;
    int counter=0;
    int i;
    map<string , vector<Data> >:: iterator wiremap_iter ;
    for(auto wiremap_iter=wiremap.begin() ; wiremap_iter!=wiremap.end(); ++wiremap_iter){ //跑過所有的線
        //if(wiremap_iter->second.size()>= cluster_standard_num){
        if(wiremap_iter->second.size()>= 4){
            i=1;
            vector<Data>:: iterator veciter=wiremap_iter->second.begin(); //跑正在看的線的所有成員macro Data
            for(veciter;veciter!=wiremap_iter->second.end();++veciter){
                vector<Macro>::iterator miter=macrolist.begin();
                for(miter; miter!=macrolist.end(); ++miter){
                    if(veciter->macro_name==miter->name){
                        if(i==1){
                            x_min=miter->x;
                            x_max=miter->tr_x;
                            y_min=miter->y;
                            y_max=miter->tr_y;
                            i=0;
                        }
                        else{
                            if(miter->x < x_min) x_min=miter->x;
                            if(miter->tr_x > x_max) x_max=miter->tr_x;
                            if(miter->y < y_min) y_min=miter->y;
                            if(miter->tr_y > y_max) y_max=miter->tr_y;
                        }
                    }
                }
            }
            score+=percentage;
            counter++;
            bounding_area=(x_max-x_min)*(y_max-y_min);
            percentage=bounding_area/chip_area;
            score=score+percentage;
            wire_name=wiremap_iter->first;
            clustermap[wire_name]=percentage;
            outputfile<<"wire name : "<<wire_name<<"  percentage : "<<percentage<<endl;
        }
    }
    score=score/counter;
    outputfile<<"total score : "<<score;
} 

/*
//功能：存取所有cluster並將macro名字放入vector，翻轉的時候就跳過這些人
//使用時機：進行所有翻轉operation前
//可透過調整cluster_standard_num變數，調整抓取連線強度的數量
void place_macro::dont_flip_clustermacro_vector(){
    vector<string> macroname_tempt;

    map<string , vector<Data> >:: iterator wiremap_iter ;
    for(auto wiremap_iter=wiremap.begin() ; wiremap_iter!=wiremap.end(); ++wiremap_iter){ //跑過所有的線
        if(wiremap_iter->second.size()>= cluster_standard_num ){
            vector<Data>:: iterator veciter=wiremap_iter->second.begin(); //跑正在看的線的所有成員macro Data
            for(veciter ; veciter!=wiremap_iter->second.end() ; ++veciter){
                macroname_tempt.push_back(veciter->macro_name);
            }
            if(macroname_tempt.size()>1){ //確定這條線不是都連到同個macro再把這些macro name存進cluster_macros的vector
                vector<string>::iterator temptiter = macroname_tempt.begin();
                for(temptiter ; temptiter!=macroname_tempt.end() ; ++temptiter){
                    cluster_macros.push_back(*temptiter);
                }
            }       
        }
    }

}
*/

/*
int main(int argc, char* argv[]){

    place_macro a;
    a.start_t = clock();
    a.v_file( argc, argv );
    a.mlist_file( argc, argv );
    a.lef_file( argc, argv );
    a.def_file( argc, argv );
    a.txt_file( argc, argv );
    a.find_coordinate_construct_map();
    a.HPWL_result.push_back(a.calculate_HPWL());
    
    a.dmp_file(argc, argv);
    
    //////////////calculate time/////////////////
        a.end_t = clock();
        a.total_t = (a.end_t-a.start_t)/(CLOCKS_PER_SEC*60);
        cout<<"Total time: "<<a.total_t<<endl;
    
    a.pl_updating(argc, argv);
    
    a.bounding_box_percentage( argc, argv );

}*/





//功能：將被認定有cluster的macro存進vector(cluster_macros)裡，這些macro在翻轉時應該被跳過
//會被跳過者：
//    1.面積夠大，翻轉造成的影響夠大    macro_area/chip_area >= 0.0185778
//    2.連線關係夠強    if(cciter->second>=4) 
//    3.續2.，連線關係夠強且距離很近 if(percentage<=0.2) [或是可以改成直接取用兩者的連線距離？]
//可以調的變數:
//    macro_area/chip_area >= 0.0185778 
//    if(cciter->second>=4) 
//    if(percentage<=0.2) cluster_macros.(cciter->first);
void place_macro::find_cluster(){
    //Macro macro_tempt;
    double bounding_box_area;
    double macro_area;
    double chip_area=(chip_x_center*2)*(chip_y_center*2);
    double chip_distance = pow( pow(chip_x_center*2,2)+pow(chip_y_center*2,2) , 0.5 );
    string wire_tempt;
    string macro_tempt;
    vector<string> need_cal; //需要計算boundind box 的macro
    vector<string> need_compare;  //需要比較的macro -> 面積要夠大
    double target_center_x,target_center_y;
    double ccmacro_center_x,ccmacro_center_y; //147,148
    
    vector<Macro>::iterator miter=macrolist.begin();
    for(miter=macrolist.begin(); miter!=macrolist.end(); ++miter){
        macro_area=(miter->width)*(miter->height);
        if(macro_area/chip_area >= 0 ){
            need_compare.push_back(miter->name);   
        }
    } //end find need_compare
    cout<<endl;
    cout<<"!!! size of need_compare : "<<need_compare.size()<<endl;
    cout<<"!!! size of wiremap : "<<wiremap.size()<<endl;
    //找出所有不想被翻轉的cluste macro並存入vector(cluster_macros)
    vector<string>::iterator niter=need_compare.begin();
    for(niter=need_compare.begin(); niter!=need_compare.end(); ++niter){
        cout<<"The target macro now is "<< *niter <<endl; 
        ccmap.clear();
        vector<Macro>::iterator miter=macrolist.begin();
        for(miter=macrolist.begin(); miter!=macrolist.end(); ++miter){
            if(miter->name == *niter){
                //cout<<"yooooooo111111"<<endl;
                vector<Pin>::iterator piter=miter->pinlist.begin();
                for(piter=miter->pinlist.begin(); piter!=miter->pinlist.end(); ++piter){
                    wire_tempt=find_pin(miter->name, piter->name);
                    //cout<<"wire tempt : "<<wire_tempt<<"piter->name : "<<piter->name<<endl;
                    map<string, vector<Data> >::iterator mapiter=wiremap.begin(); 
                    for(mapiter=wiremap.begin(); mapiter!=wiremap.end(); ++mapiter){
                        //cout<<"yooooooo22222222"<<endl;             
                        if(mapiter->first == wire_tempt){
                            //if(mapiter->second.size()!=1)cout<<"wire tempt : "<<wire_tempt<<"  wiremap vector size : "<<mapiter->second.size()<<endl;
                            vector<Data>::iterator veciter=mapiter->second.begin();
                            for(veciter=mapiter->second.begin(); veciter!=mapiter->second.end(); ++veciter){
                                macro_tempt=veciter->macro_name; 
                                //cout<<"yooooooo3333333333333b"<<endl;
                                if(macro_tempt != *niter){
                                    //cout<<"macro tempt : "<<macro_tempt<<"  target macro : "<<*niter<<endl;
                                    if(ccmap.find(macro_tempt)!=ccmap.end()) ccmap[macro_tempt]++;
                                    else ccmap[macro_tempt]=1;
                                }
                            }
                        }
                    }
                }
            }
        }
        double x_coordinate[4];
        double y_coordinate[4];
        double x_real, y_real, tr_x_real, tr_y_real;
        map<string, int>::iterator cciter=ccmap.begin();
        
        for(cciter=ccmap.begin(); cciter!=ccmap.end(); ++cciter){ //跑一次連線關係夠強的macro們
            cout<<"    The macro of ccmap is : "<<cciter->first<<endl;
            if(cciter->second>=4) {
                cout<<"This macro has strong connection!"<<endl;
                vector<Macro>::iterator miter=macrolist.begin();
                for(miter; miter!=macrolist.end(); ++miter){
                    if(miter->name == cciter->first){ //抓出連線關係夠強macro座標
                        x_coordinate[0]=miter->x;
                        y_coordinate[0]=miter->y;
                        x_coordinate[1]=miter->tr_x;
                        y_coordinate[1]=miter->tr_y;
                        ccmacro_center_x = miter->center_x;
                        ccmacro_center_y = miter->center_y;
                    }
                    if(miter->name == *niter){ //抓出目標macro座標
                        x_coordinate[2]=miter->x;
                        y_coordinate[2]=miter->y;
                        x_coordinate[3]=miter->tr_x;
                        y_coordinate[3]=miter->tr_y;
                        target_center_x = miter->center_x;
                        target_center_y = miter->center_y;
                    }
                }
                
                //判斷兩macro夠近；用bounding box嘗試
                /*
                //排x最大最小
                for(int i=0 ; i<4 ; i++ ){
                    for(int j=i+1 ; j<4 ; j++ ){
                        if( x_coordinate[i] > x_coordinate[j] ){
                            //變數交換
                            int tmp = x_coordinate[i];
                            x_coordinate[i] = x_coordinate[j];
                            x_coordinate[j] = tmp;
                        }
                    }
                }
                x_real = x_coordinate[0];
                tr_x_real = x_coordinate[3];
                //排y最大最小
                for(int i=0 ; i<4 ; i++ ){
                    for(int j=i+1 ; j<4 ; j++ ){
                        if( y_coordinate[i] > y_coordinate[j] ){
                            //變數交換
                            int tmp = y_coordinate[i];
                            y_coordinate[i] = y_coordinate[j];
                            y_coordinate[j] = tmp;
                        }
                    }
                }    
                y_real = y_coordinate[0];
                tr_y_real = y_coordinate[3];
                
                
                //算bounding box
                bounding_box_area = (tr_x_real-x_real) * (tr_y_real-y_real) ;
                double percentage = bounding_box_area/chip_area;
                cout<<"bounding box area : "<<bounding_box_area<<"  chip area : "<<chip_area<<"  percentage : "<<percentage<<endl;
                if(percentage <= 0.001){  //把這對連線關係強又"夠近"者存入clutser_macros
                    cluster_macros.push_back(*niter);
                    cluster_macros.push_back(cciter->first);  
                    cout<<" "<<cciter->first;  
                } //終於確定有cluster
                */
                 
                //判斷兩macro夠近；用macro中心間距離嘗試
                double distance = pow( pow(target_center_x-ccmacro_center_x,2)+pow(target_center_y-ccmacro_center_y,2) , 0.5 );
                double percentage = distance/chip_distance;
                cout<<"macro distance : "<<distance<<"  chip distance : "<<chip_distance<<"  percentage : "<<percentage<<endl;
                if(percentage <= 0.01){  //把這對連線關係強又"夠近"者存入clutser_macros
                    cluster_macros.push_back(*niter);
                    cluster_macros.push_back(cciter->first);  
                    cout<<" "<<cciter->first;  
                } //終於確定有cluster
            }
        }
        cout<<endl;
    }
    
    //刪除vector(cluster_macros)中的重複元素
    remove(cluster_macros);
    
    //檢查
    cout<<"cluster_macros = "<<endl;
    vector<string>:: iterator vecciter;
    for(vecciter=cluster_macros.begin(); vecciter!=cluster_macros.end(); ++vecciter){
        cout<<*vecciter<<endl;
    }
    cout<<endl<<endl<<endl;
}

void place_macro::remove(vector<string> &v) {
    auto end = v.end();
    for (auto it = v.begin(); it != end; ++it) {
        end = std::remove(it + 1, end, *it);
    }
 
    v.erase(end, v.end());
}


//畫出目前macro結果
void place_macro::draw_rectangle_connection(int argc, char* argv[]){ 
    ofstream outputfile(argv[6],ios::out);

    vector<Macro>::iterator miter=macrolist.begin();
    for(miter; miter!=macrolist.end(); ++miter){
        vector<string>:: iterator veciter=cluster_macros.begin();
        for(veciter;veciter!=cluster_macros.end();++veciter){
            if(miter->name==*veciter){
                outputfile<<"rectangle('Position',[ ";
                outputfile<<miter->x<<" "<<miter->y<<" ";
                outputfile<<miter->width<<" "<<miter->height<<" ], 'EdgeColor','r' )"<<endl;
            }
        }
    }
    outputfile<<"rectangle('Position',[ 0 0 "<<chip_x_center*2<<" "<<chip_y_center*2<<" ])"<<endl; //chip外框
    outputfile<<"axis equal tight";  //讓x y軸的比例相同
} 
