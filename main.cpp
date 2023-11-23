#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <cstring>
#include <map>

#include <time.h>
#include <stdio.h>

#include "class.h"

using namespace std;


int main(int argc, char* argv[])
{  
    
    place_macro a;
    
    a.start_t = clock();
    
    a.v_file( argc, argv );
    
    /*
    map<string, vector<int> >::iterator mapiter=a.connectmap.begin();
    int counter_macro;
    int counter_std;
    for(mapiter; mapiter!=a.connectmap.end(); ++mapiter){
        counter_macro=0;
        counter_std=0;
        cout<<"wire name : "<<mapiter->first<<endl;
        cout<<"all connect :";
        vector<int>::iterator veciter=mapiter->second.begin();
        for(veciter; veciter!=mapiter->second.end(); ++veciter){
            cout<<*veciter<<"  ";
            if(*veciter==0)counter_std++;
            else counter_macro++;
        }
        cout<<endl<<"connect to macro : "<<counter_macro<<endl;
        cout<<"connect to standard cell : "<<counter_std<<endl;
        cout<<endl<<endl;
    }
    */
    a.mlist_file( argc, argv );
    a.lef_file( argc, argv );
    a.def_file( argc, argv );
    a.txt_file( argc, argv );
    
    vector<Macro> :: iterator miter = a.macrolist.begin();
    vector<Pin> :: iterator piter = miter->pinlist.begin();
    int i=0;
    for(miter; miter!= a.macrolist.end(); miter++){
      for(piter=miter->pinlist.begin() ; piter != miter->pinlist.end();piter++ ){
      
        i++;
      }    
    }
    cout<< " macro number: "<<i;
    /*cout<<a.die_x<<"  "<<a.die_y<<endl;
    cout<<a.chip_x_center<<"  "<<a.chip_y_center<<endl;
    vector<Macro>::iterator it1=a.macrolist.begin();
    for(it1; it1!=a.macrolist.end(); ++it1){
      cout<<"macro "<<fixed<<it1->x<<" "<<fixed<<it1->y<<" "<<fixed<<it1->width<<" "<<fixed<<it1->height<<endl;
      vector<Pin>::iterator it2=it1->pinlist.begin();
      for(it2; it2!=it1->pinlist.end(); ++it2)
        cout<<it2->x<<" "<<it2->y<<endl;
    }*/
  
    a.find_coordinate_construct_map();
  
    //**********************for checking the wirelist content*************************
    /*vector<Data>::iterator wireiter = a.wirelist.begin();
    for(wireiter; wireiter != a.wirelist.end(); ++wireiter)
        cout<<setw(30)<<wireiter->wire_name<<setw(40)<<wireiter->macro_name<<setw(5)<<wireiter->pin_name<<setw(15)<<setprecision(2)<<fixed<<wireiter->pin_x<<setw(15)<<setprecision(2)<<fixed<<wireiter->pin_y<<endl;*/

    //a.iopad_wire_wiremap();
    
    /*
    vector<IOpad>::iterator paditer = a.IOpadlist.begin();
	  for (paditer; paditer != a.IOpadlist.end(); ++paditer)
	    cout << paditer->name << " " << paditer->netname << " " << paditer->x << " " << paditer->y << " " << paditer->type << endl;
    */
    /*
    
    map<string, vector<Data> >::iterator mapiter=a.wiremap.begin();
    for(mapiter; mapiter!=a.wiremap.end(); ++mapiter){
    cout<<mapiter->first<<endl;
    vector<Data>::iterator initer=mapiter->second.begin();
    for(initer; initer!=mapiter->second.end(); ++initer)
     cout<<initer->macro_name<<"  "<<initer->pin_name<<"  "<<fixed<<setprecision(2)<<initer->pin_x<<"  "<<setprecision(2)<<fixed<<initer->pin_y<<endl;
    }
    
    */
    
   
    //cout<<"The result of origin HPWL : "<<fixed<<a.calculate_HPWL()<<endl;
    //a.calculate_HPWL();
    //cout<<endl<<"wiremap size: "<<a.wiremap.size()<<endl;
    //cout<<"HPWLmap size: "<<a.HPWLmap.size()<<endl;
	  /*
    vector<Data>::iterator wireiter = a.wirelist.begin();
    for(wireiter; wireiter != a.wirelist.end(); ++wireiter)
        cout<<setw(30)<<wireiter->wire_name<<setw(40)<<wireiter->macro_name<<setw(5)<<wireiter->pin_name<<setw(15)<<setprecision(2)<<fixed<<wireiter->pin_x<<setw(15)<<setprecision(2)<<fixed<<wireiter->pin_y<<endl;
    */
        
    /*  
	  vector<IOpad>::iterator paditer = a.IOpadlist.begin();
	  for (paditer; paditer != a.IOpadlist.end(); ++paditer)
		cout << paditer->name << " " << paditer->netname << " " << paditer->x << " " << paditer->y << " " << paditer->type << endl;
    */
  
    /*
    int num;
    cout<<"Initial HPWL: "<<fixed<<a.calculate_HPWL()<<endl;
    cout<<"Enter the order of macro(from 1):";
    cin>>num;
	  vector<Macro>::iterator miter = a.macrolist.begin()+num-1;
	  for (miter; miter != a.macrolist.end(); ++miter){
	    cout << miter->name << " " << miter->type << " Fixed:" << miter->fixed  << ", Orientation: " << miter->orient <<", Width = "<<miter->width<<setprecision(2)<<fixed<<", Height = "<<miter->height<<setprecision(2)<<fixed<<endl;
	    cout<<"Bottom left (x,y) = ("<<miter->x<<setprecision(2)<<fixed<<" , "<<miter->y<<setprecision(2)<<fixed<<"), Center (x,y) = ("<<miter->center_x<<setprecision(2)<<fixed<<" , "<<miter->center_y<<setprecision(2)<<fixed<<")"<<endl;
		  vector<Pin>::iterator piter = miter->pinlist.begin();
		  for (piter; piter != miter->pinlist.end(); ++piter)
		    cout<<piter->name<<" "<<" X:"<<setprecision(2)<<fixed<<(piter->x-miter->x)/a.UDM<<" * 2000 + "<<setprecision(0)<<fixed<<miter->x<<" = "<<piter->x<<"   Y:"<<setprecision(2)<<fixed<<(piter->y-miter->y)/a.UDM<<" * 2000 + "<<setprecision(0)<<fixed<<miter->y<<" = "<<piter->y<<endl;
    }
    */

    //cout<<endl<<"test"<<endl<<endl;
    

    
    
    //map<double, vector<Macro> >::iterator mapiter=a.areamap.begin();
    /*
    //檢查areamap
    for(mapiter; mapiter!=a.areamap.end(); ++mapiter){
        //cout<<"Area : "<<mapiter->first<<endl;
        //cout<<"Macro Name : "<<endl;
        vector<Macro>::iterator innerit=mapiter->second.begin();
        for(innerit; innerit!=mapiter->second.end(); ++innerit){
            cout<<*innerit<<"  ";
        }
        vector<Macro>::iterator innerit=mapiter->second.begin();
        for(innerit; innerit!=mapiter->second.end(); ++innerit){
            cout<<"Area : "<<mapiter->first<<endl;
            cout<<"Macro Name : "<<endl;
            cout<<innerit->name<<endl;
            vector<Pin>::iterator piniter=innerit->pinlist.begin();
            for(piniter; piniter!=innerit->pinlist.end(); ++piniter){
                //cout<<"pin name : "<<piniter->name;
                //cout<<"    pin x : "<<piniter->x<<"  pin y : "<<piniter->y<<endl;
            }
        }
        cout<<endl;
    }
    */
    /////////////////algorithm///////////////////
    //跑Algorithm1 + Algorithm 2(FBH_old，不要fixed) + OPT(考慮area)
    
    //a.pin_number_in_macro();
    //a.pin_connection_in_macro();
    //a.update_macrolist_pinlist();
    
    //a.flip_by_HPWL();
    //a.Distance_Arrangement();
    //a.Area_Arrangement();
    //a.Optimization();
    //a.flip_by_HPWL();
    //a.print_result();
    //a.FBH_new();
    

    /*a.Distance_Arrangement();
    map<double, vector<string> > :: iterator disiter = a.dismap.begin();
    for(disiter=a.dismap.begin(); disiter!=a.dismap.end(); ++disiter){
        cout<<"Distance = "<<disiter->first<<endl;
        for(auto macroiter=disiter->second.begin() ; macroiter!=disiter->second.end() ; ++macroiter)
            cout<<"macro : "<< *macroiter<<"  ";
        cout<<endl<<endl;
    }
    a.shifting_by_macro();*/
    
    a.shifting();
    
    /*vector<Macro>::iterator miter = a.macrolist.begin();
	  for (miter; miter != a.macrolist.end(); ++miter)
        miter->fixed=0;
        
    a.shifting();
    
	  for (miter = a.macrolist.begin(); miter != a.macrolist.end(); ++miter)
        miter->fixed=0;
        
    a.shifting();
    
	  for (miter = a.macrolist.begin(); miter != a.macrolist.end(); ++miter)
        miter->fixed=0;
       
    a.shifting();*/
    
    
    /*map<string, vector<Data> >::iterator wireiter=a.wiremap.begin();

    for(int i=0;i<=5;i++)
    {
      cout<<wireiter->first<<endl;
      a.determine_the_boundary_macro(wireiter->first);
      cout<<"\n"<<"\n";
      wireiter++;
    }
    
    
    cout<<wireiter->first<<endl;
    a.determine_the_boundary_macro(wireiter->first);*/
    /////////////////////////////////////////////
    
    
    //list the HPWL from the longest one to the shorest 
    /*    
    for (auto it = a.HPWLmap.rbegin(); it != a.HPWLmap.rend(); it++) {
      cout << "HPWL = " << (*it).first << ", wire name = " << (*it).second << "\n";
     }
    */

    //a.wiremap.erase(a.wiremap.begin(), a.wiremap.end());
    
    //a.wiremap.clear();
    //a.HPWLmap.clear();
    //a.find_coordinate_construct_map();
    /*
    cout<<endl<<"map size: "<<a.wiremap.size()<<endl;
    cout<<"The result of HPWL after flipping : "<<fixed<<a.calculate_HPWL()<<endl;/*
    cout<<endl<<"HPWLmap size: "<<a.HPWLmap.size()<<endl;
    */
    
    
    
    /*
    map<string, vector<Data>>::iterator mapiter=a.wiremap.begin();
    for(mapiter; mapiter!=a.wiremap.end(); ++mapiter){
      cout<<mapiter->first<<endl;
      vector<Data>::iterator initer=mapiter->second.begin();
      for(initer; initer!=mapiter->second.end(); ++initer)
        cout<<initer->macro_name<<"  "<<initer->pin_name<<"  "<<fixed<<setprecision(2)<<initer->pin_x<<"  "<<setprecision(2)<<fixed<<initer->pin_y<<endl;
    }*/
    
    /*a.find_coordinate_construct_map();
    cout<<"\nHPWL = "<<fixed<<setprecision(0)<<a.calculate_HPWL()<<endl;*/
    
    a.draw_rectangle( argc, argv );

    //******************** output *********************/
    a.dmp_file(argc, argv);
    
        a.end_t = clock();
        a.total_t = (a.end_t-a.start_t)/CLOCKS_PER_SEC; 
        cout<<"Total time: "<<a.total_t<<endl;
    
    a.pl_updating(argc, argv);
    //還沒摳權重函式
    /*
    a.pin_number_in_macro();
    a.Weight_Arrangement( 0.33 , 0.33 , 0.33);
    a.Optimization(); //改成seanonly內的版本
    a.pl_updating(argc, argv , 1);
    */
    
     
    
    return 0;
}
