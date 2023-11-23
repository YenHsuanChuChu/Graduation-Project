#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <cstring>
#include <map>
#include "class.h"

//to-do-list
//1.write a function to check if one wire can connect to multiple iopad
//2.calculate HPWL for single wire
//3.put the wire name in order in a vector form

using namespace std;

void place_macro::find_coordinate_construct_map(){
    wiremap.clear();
    string macro_tempt;
    string wire_tempt;
    string pin_tempt;
    string macro_find;
    string pin_find;
    double x_find;
    double y_find;
    Data data_tempt;
    vector<Data>::iterator find_iter = wirelist.begin();
    for(find_iter; find_iter!=wirelist.end(); ++ find_iter){
        macro_tempt=find_iter->macro_name;
        wire_tempt=find_iter->wire_name;
        pin_tempt=find_iter->pin_name;
        vector<Macro>::iterator miter = macrolist.begin();
        for (miter; miter != macrolist.end(); ++miter){
            macro_find=miter->name;
            if(macro_find == macro_tempt){
                vector<Pin>::iterator piter = miter->pinlist.begin();
                for (piter; piter != miter->pinlist.end(); ++piter){
                    pin_find=piter->name;
                    if(pin_tempt == pin_find){
                        find_iter->pin_x=piter->x;
                        find_iter->pin_y=piter->y;
                        data_tempt.macro_name=macro_tempt;
                        data_tempt.wire_name=wire_tempt;
                        data_tempt.pin_name=pin_tempt;
                        data_tempt.pin_x=find_iter->pin_x;
                        data_tempt.pin_y=find_iter->pin_y;
                        wiremap[wire_tempt].push_back(data_tempt);
                    }
                }
            }
        }
    }
    return;
}
void place_macro::find_coordinate_construct_map_onewire(string wirename){
    onewiremap.clear();
    string macro_tempt;
    string wire_tempt;
    string pin_tempt;
    string macro_find;
    string pin_find;
    double x_find;
    double y_find;
    Data data_tempt;
    vector<Data>::iterator find_iter = wirelist.begin();
    for(find_iter; find_iter!=wirelist.end(); ++ find_iter){
        if(find_iter->wire_name ==wirename){
            macro_tempt=find_iter->macro_name;
            wire_tempt=find_iter->wire_name;
            pin_tempt=find_iter->pin_name;
            vector<Macro>::iterator miter = macrolist.begin();
            for (miter; miter != macrolist.end(); ++miter){
                macro_find=miter->name;
                if(macro_find == macro_tempt){
                    vector<Pin>::iterator piter = miter->pinlist.begin();
                    for (piter; piter != miter->pinlist.end(); ++piter){
                        pin_find=piter->name;
                        if(pin_tempt == pin_find){
                            find_iter->pin_x=piter->x;
                            find_iter->pin_y=piter->y;
                            data_tempt.macro_name=macro_tempt;
                            data_tempt.wire_name=wire_tempt;
                            data_tempt.pin_name=pin_tempt;
                            data_tempt.pin_x=find_iter->pin_x;
                            data_tempt.pin_y=find_iter->pin_y;
                            onewiremap[wire_tempt].push_back(data_tempt);
                        }
                    }
                }
            }
        }
    }
    return;
}

int place_macro::find_pin_iopad(string wire_name){
    for(int i=0; i<IOpadlist.size(); i++){
        if(IOpadlist[i].netname==wire_name){
        cout<<"CCCCCC"<<endl;
        return i;
        }
    }
    return 1000000000;
}

void place_macro::iopad_wire_wiremap(){
    int result;
    string wire_name;
    Data data_tempt;
    map<string, vector<Data> >::iterator mapiter=wiremap.begin();
    for(mapiter; mapiter!=wiremap.end(); ++mapiter){
        wire_name=mapiter->first;
        result=find_pin_iopad(wire_name);
        if(result==1000000000);
        else{
            //cout<<"BBBBBB"<<endl;
            data_tempt.macro_name=IOpadlist[result].name;
            data_tempt.wire_name=IOpadlist[result].netname;
            data_tempt.pin_name="IOpad";
            data_tempt.pin_x=IOpadlist[result].x;
            data_tempt.pin_y=IOpadlist[result].y;
            wiremap[wire_name].push_back(data_tempt);
        }
    }

}


/*
vector<IOpad>::iterator paditer = a.IOpadlist.begin();
	for (paditer; paditer != a.IOpadlist.end(); ++paditer)
		cout << paditer->name << " " << paditer->netname << " " << paditer->x << " " << paditer->y << " " << paditer->type << endl;
*/
int place_macro::find_pin_iopad_x(string wirename){
    int enable=0;
    //vector<IOpad>::iterator paditer = IOpadlist.begin();
    /*for (paditer; paditer != IOpadlist.end(); ++paditer){
        if(paditer->netname==wirename){
            //cout<<paditer->netname<<"  "<<wirename<<endl;
            return paditer;
        }
        enable=1;
    }*/
    for(int i=0; i<IOpadlist.size(); i++){
        if(IOpadlist[i].netname==wirename){
            //cout<<paditer->netname<<"  "<<wirename<<endl;
            return i;
        }
        enable=1;       
    }
    //if(enable==0)return 0;
}
/*
double place_macro::find_pin_iopad_y(string wirename){
    int enable=0;
    vector<IOpad>::iterator paditer = IOpadlist.begin();
    for (paditer; paditer != IOpadlist.end(); ++paditer){
        if(paditer->netname==wirename)return paditer->y;
        enable=1;
    }
    if(enable==0)return 0;
}*/


double place_macro::calculate_HPWL(){
    HPWLmap.clear();
    wiremap.clear();
    find_coordinate_construct_map();
    double tempt_length=0;
    double x_largest=0;
    double y_largest=0;
    double x_smallest=0; 
    double y_smallest=0;
    double HPWL=0;
    int index;
    int counter=0;
    map<string, vector<Data> >::iterator mapiter=wiremap.begin();
    string wire_tempt;
    double x, y;
    double io_x=0, io_y=0;
     for(mapiter; mapiter!=wiremap.end(); ++mapiter){
       wire_tempt=mapiter->first;
       index=find_pin_iopad_x(wire_tempt); 
       io_x=IOpadlist[index].x;
       io_y=IOpadlist[index].y;
       counter++;
       //y=find_pin_iopad_y(wire_tempt);
       //if(x!=0)io_x=x;
       //if(y!=0)io_y=y;
       
       //cout<<wire_tempt<<endl;
       vector<Data>::iterator initer=mapiter->second.begin();
       //x_largest=x;
       //y_largest=y;
       //x_smallest=x;
       //y_smallest=y;
       for(initer; initer!=mapiter->second.end(); ++initer){
           if(initer==mapiter->second.begin()){
               x_largest=initer->pin_x;
               y_largest=initer->pin_y;
               x_smallest=initer->pin_x;
               y_smallest=initer->pin_y;
               /*if(io_x!=0 && io_y!=0){
                   if(io_x > x_largest) x_largest=io_x;
                   if(io_y > y_largest) y_largest=io_y;
                   if(io_x < x_smallest) x_smallest=io_x;
                   if(io_y < y_smallest) y_smallest=io_y;             
               }*/
           }
           
           if(initer->pin_x > x_largest) x_largest=initer->pin_x;
           if(initer->pin_y > y_largest) y_largest=initer->pin_y;
           if(initer->pin_x < x_smallest) x_smallest=initer->pin_x;
           if(initer->pin_y < y_smallest) y_smallest=initer->pin_y;
       }
       
       //cout<<"x_largest: "<<x_largest<<"  y_largest: "<<fixed<<y_largest<<"  ";
       //cout<<"x_smallest: "<<x_smallest<<"  y_smallest: "<<fixed<<y_smallest<<endl;
       //cout<<"x_largest - x_smallest: "<<fixed<<x_largest-x_smallest<<"  ";
       //cout<<"y_largest - y_smallest: "<<fixed<<y_largest-y_smallest<<endl;
       
       //calculate HPWL for single wire (negelect the situation that there exist same HPWL in different wire) 
       HPWL = (x_largest-x_smallest) + (y_largest-y_smallest);
       //HPWLmap.insert({ HPWL , wire_tempt });
       //HPWLmap_tempt.insert({ wire_tempt, HPWL });
       //calculate total HPWL
       
       /*if(HPWLmap.find(HPWL)!=HPWLmap.end()){
         
       }*/
       if(HPWL!=0)HPWLmap[HPWL].push_back(wire_tempt);
       tempt_length = tempt_length + HPWL;
       //cout<<"current length: "<<fixed<<tempt_length<<endl;
     }
     
     //bubble sort
     
     /*
     map< double, vector<string> >::iterator it=HPWLmap.begin();
     for(it; it!=HPWLmap.end(); ++it){
         //cout<<it->first<<" ";
         vector<string>::iterator innerit=it->second.begin();
         //if(it->second.size()>1) cout<<"vector size: "<<it->second.size()<<endl;
         for(innerit; innerit!=it->second.end(); ++innerit){
             //cout<<"wire length: "<<it->first<<"  wire name: "<<*innerit<<endl;
         }
     }
     */
     
     return tempt_length;
}

/*
double place_macro::one_HPWL(string wire_name){
    double tempt_length;
    double tempt_xmax=0, tempt_xmin=0;
    double tempt_ymax=0, tempt_ymin=0;
    
    map<string, vector<Data> >::iterator mapiter=wiremap.begin();
    //map<string, vector<Data> >::iterator mapiter=wiremap.begin();
    for(mapiter; mapiter!=wiremap.end(); ++mapiter){
        if(mapiter->first == wire_name){
            vector<Data>::iterator it=mapiter->second.begin();
            for(it; it!=mapiter->second.end(); ++it){
                if(it==mapiter->second.begin()){
                    tempt_xmax=it->pin_x;
                    tempt_xmin=it->pin_x;
                    tempt_ymax=it->pin_y;
                    tempt_ymax=it->pin_y;
                }
            }
            if(it->pin_x > tempt_xmax) tempt_xmax=it->pin_x;
            if(it->pin_y > tempt_ymax) tempt_ymax=it->pin_y;
            if(it->pin_x < tempt_xmin) tempt_xmin=it->pin_x;
            if(it->pin_y < tempt_ymin) tempt_ymin=it->pin_y;        
        }
        tempt_length=(tempt_xmax-tempt_xmin) + (tempt_ymax-tempt_ymin);
    }
    return tempt_length;
}
*/

double place_macro::one_HPWL(string wire_name, string macro_name, int i, double center_x, double center_y){
    //直接在這邊計算所有的翻轉結果可能導致的線長變化->不去更新macrolist裡面pinlist的數值
    //但需要從flip_by_hpwl傳遞x,y center的數值
    double tempt_x, tempt_y; //暫存x, y的數值(翻轉後的)
    double value;
    vector<double>x_cor;
    vector<double>y_cor;
    x_cor.clear();  //清除上次計算裡面的殘值
    y_cor.clear();
    map<string, vector<Data> >::iterator mapiter=wiremap.begin();
    
    //for (auto mapiter=wiremap.find(wire_name); mapiter!=wiremap.end(); ++mapiter) { //找到那條線
    for (mapiter=wiremap.begin(); mapiter!=wiremap.end(); ++mapiter) {
      if(mapiter->first==wire_name){
      
        vector<Data>::iterator it=mapiter->second.begin();
        //cout<<"i = "<<i<<endl;
        for(it=mapiter->second.begin(); it!=mapiter->second.end(); ++it){
            if(macro_name==it->macro_name){  //如果在wiremap 那條線上的macro與嘗試翻轉的相同
                switch(i){
                    case 0:
                        tempt_x=it->pin_x;
                        tempt_y=it->pin_y;
                    break;
                    case 1:
                        tempt_x=2*center_x-(it->pin_x);
                        tempt_y=it->pin_y;
                    break;
                    case 2:
                        tempt_x=2*center_x-(it->pin_x);
                        tempt_y=2*center_y-(it->pin_y);
                    break;
                    case 3:
                        tempt_x=it->pin_x;
                        tempt_y=2*center_y-(it->pin_y);      
                    break;
                }   
            }
            else{ //非嘗試翻轉的macro
                tempt_x=it->pin_x;
                tempt_y=it->pin_y;
            }
            x_cor.push_back(tempt_x);
            y_cor.push_back(tempt_y);
         }
       
      }//end if
    } 
    //cout<<x_cor.size()<<endl;
    value=count(x_cor)+count(y_cor);
    //value1=count(y_cor);
    //cout<<count(x_cor)+count(y_cor)<<endl;
    
    /*for (int i = 0; i < x_cor.size(); i++) {
       cout << "pin = " <<x_cor.at(i) << ' ' << y_cor.at(i) <<endl;
    }*/
    //cout<<"This HPWL= "<<value<<endl;
    return value;
}

double place_macro::count(vector<double>& input){
    double max, min; //暫存最大值與最小值
    double value;
    vector<double>::iterator it=input.begin(); 
    for(it=input.begin(); it!=input.end(); ++it){
        if(it==input.begin()){
            max=*it;
            min=*it;
        }
        else if(*it>max) max=*it;
        else if(*it<min) min=*it;
    }
    value=max-min;
    return value;
}
