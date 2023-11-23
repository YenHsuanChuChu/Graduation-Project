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
#include "class.h"

using namespace std;
 
void place_macro::shifting(){
    //vector <Boundary> boundary_macro;
    calculate_HPWL();
    map< double , vector<string> > :: reverse_iterator HPWLiter = HPWLmap.rbegin();
    /*map<string , vector<Data> > :: iterator wireiter = wiremap.begin();
    int wireleft=wiremap.size();
    for(wireiter=wiremap.begin(); wireiter!=wiremap.end(); ++wireiter){*/
    for(HPWLiter = HPWLmap.rbegin(); HPWLiter != HPWLmap.rend();++HPWLiter){
        for(auto wireiter = HPWLiter->second.begin(); wireiter != HPWLiter->second.end();++wireiter){
            int keepgoing = 1;
            while(keepgoing != 0){
                /*** 找boundary macro ***/
                determine_the_boundary_macro(*wireiter);
                /*** 判斷要不要重新找boundary macro ***/
                vector <Boundary> :: iterator bounditer = find_boundary_macro.begin();
                keepgoing = 0;
                for(bounditer=find_boundary_macro.begin();bounditer!=find_boundary_macro.end();++bounditer){
                    vector<Macro>::iterator miter=macrolist.begin();
                    for(miter;miter!=macrolist.end();++miter){
                        if((miter->name == bounditer->macro_name) && (miter->fixed == 0))
                            keepgoing++;
                    }                
                }
                /*** 進行shifting ***/
                for(bounditer=find_boundary_macro.begin();bounditer!=find_boundary_macro.end();++bounditer){
                    bounditer->distance = movable_distance(bounditer->macro_name,bounditer->direction);
                    update_shifting_result(bounditer->macro_name,bounditer->direction,bounditer->distance);
                }
            }
        }
    }
}

void place_macro::shifting_by_macro(){
    //vector <Boundary> boundary_macro;
    map<double, vector<string> > :: iterator disiter = dismap.begin();
    int macroleft=dismap.size();
    for(disiter=dismap.begin(); disiter!=dismap.end(); ++disiter){
        for(auto macroiter=disiter->second.begin() ; macroiter!=disiter->second.end() ; ++macroiter){
            vector<Macro>::iterator miter=macrolist.begin();
            for(miter;miter!=macrolist.end();++miter){
                if(miter->name==*macroiter){
                    string direction_x,direction_y;
                    double distance_x = 0;
                    double distance_y = 0;
                    if(miter->center_x < chip_x_center && miter->center_y < chip_y_center){
                        direction_x = "right";
                        direction_y = "up";
                    }
                    else if(miter->center_x > chip_x_center && miter->center_y < chip_y_center){
                        direction_x = "left";
                        direction_y = "up";
                    }
                    else if(miter->center_x < chip_x_center && miter->center_y > chip_y_center){
                        direction_x = "right";
                        direction_y = "down";
                    }
                    else if(miter->center_x > chip_x_center && miter->center_y > chip_y_center){
                        direction_x = "left";
                        direction_y = "down";
                    }
                    distance_x = movable_distance(miter->name,direction_x);
                    distance_y = movable_distance(miter->name,direction_y);
                    
                    if(distance_x>distance_y){
                        update_shifting_result(miter->name,direction_x,distance_x);
                        distance_y = movable_distance(miter->name,direction_y);
                        if(displacement_max-distance_x>distance_y)
                            update_shifting_result(miter->name,direction_y,distance_y);
                        else
                            update_shifting_result(miter->name,direction_y,displacement_max-distance_x);
                            
                    }
                    else{
                        update_shifting_result(miter->name,direction_y,distance_y);
                        distance_x = movable_distance(miter->name,direction_x);
                        if(displacement_max-distance_y>distance_x)
                            update_shifting_result(miter->name,direction_x,distance_x);
                        else
                            update_shifting_result(miter->name,direction_x,displacement_max-distance_y);
                    }
                    break;
                }
            }
        }

    }
    cout<<"macroleft = "<<macroleft<<endl;
}

void place_macro::determine_the_boundary_macro(string wire_name){
  find_boundary_macro.clear();
  find_coordinate_construct_map_onewire(wire_name);
  map<string , vector<Data> > :: iterator wireiter = onewiremap.begin();
  string tempt_right_macro,tempt_left_macro,tempt_up_macro,tempt_down_macro;
  double tempt_left_pin_x,tempt_right_pin_x,tempt_up_pin_y,tempt_down_pin_y;
  
  Boundary b;
  
  for(wireiter=onewiremap.begin(); wireiter!=onewiremap.end(); ++wireiter)//從wiremap裡面找吃到的線
  {
    if(wireiter->first == wire_name && (wireiter->second).size()!=1)//比對到後去讀data
    { 
      vector<Data> :: iterator dataiter = (wireiter -> second).begin();
      tempt_right_macro = dataiter->macro_name, tempt_left_macro = dataiter->macro_name , tempt_up_macro = dataiter->macro_name, tempt_down_macro = dataiter->macro_name;
      tempt_left_pin_x = dataiter->pin_x , tempt_right_pin_x = dataiter->pin_x , tempt_up_pin_y=dataiter->pin_y , tempt_down_pin_y=dataiter->pin_y;
      
      vector<Macro>::iterator miter=macrolist.begin();
      
      
      for(dataiter=((wireiter -> second).begin());dataiter != (wireiter->second).end();++dataiter)//跑data裡的每個pin座標
      {
        //cout<<"Macro name : "<<dataiter->macro_name <<endl;
        for(miter=macrolist.begin(); miter!=macrolist.end(); ++miter)
          if(dataiter->macro_name == miter->name){
              //cout<<"rectangle('Position',[ "<<miter->x<<" "<<miter->y<<" "<< miter->width << " " << miter->height<<" ])"<<endl; 
          } 
        if(dataiter->pin_x > tempt_right_pin_x)
        {
          tempt_right_macro = dataiter->macro_name;//找出這條線上最右邊的macro並將macro存起來  
          tempt_right_pin_x = dataiter->pin_x;     //存他的x座標
        } 
        if(dataiter->pin_x < tempt_left_pin_x)
        {
          tempt_left_macro = dataiter->macro_name;//找出這條線上最左邊的macro並將macro存起來  
          tempt_left_pin_x = dataiter->pin_x; //存他的x座標
        }
        
        if(dataiter->pin_y > tempt_up_pin_y)
        {
          tempt_up_macro = dataiter->macro_name;
          tempt_up_pin_y = dataiter->pin_y;
        }
        if(dataiter->pin_y < tempt_down_pin_y)
        {
          tempt_down_macro = dataiter->macro_name;
          tempt_down_pin_y = dataiter->pin_y;
        }
      }//end for(dataiter)
      cout<<endl;
      //half_of_bounding_box_x = (tempt_right_pin_x - tempt_left_pin_x)/2;
      //half_of_bounding_box_y = (tempt_up_pin_y - tempt_down_pin_y)/2;
      half_of_bounding_box_x = (tempt_right_pin_x - tempt_left_pin_x);
      half_of_bounding_box_y = (tempt_up_pin_y - tempt_down_pin_y);
      /*
      cout<<"tempt_right_macro : "<<tempt_right_macro<<endl;
      cout<<"tempt_left_macro : "<<tempt_left_macro<<endl;
      cout<<"tempt_up_macro : "<<tempt_up_macro<<endl;
      cout<<"tempt_down_macro : "<<tempt_down_macro<<endl;
      */
      for(dataiter = (wireiter->second).begin();dataiter!=(wireiter->second).end();++dataiter)//把最上的macro存回vector
      {
        if(tempt_up_pin_y==dataiter->pin_y)
        {
          b.direction="down";
          b.macro_name=dataiter->macro_name;
          find_boundary_macro.push_back(b);
        }        
      }
      for(dataiter = (wireiter->second).begin();dataiter!=(wireiter->second).end();++dataiter)//把最下的macro存回vector
      {
        if(tempt_down_pin_y==dataiter->pin_y)
        {
          b.direction="up";
          b.macro_name=dataiter->macro_name;
          find_boundary_macro.push_back(b);
        }        
      }
      for(dataiter = (wireiter->second).begin();dataiter!=(wireiter->second).end();++dataiter)//把最左的macro存回vector
      {
        if(tempt_right_pin_x==dataiter->pin_x)
        {
          b.direction="left";
          b.macro_name=dataiter->macro_name;
          find_boundary_macro.push_back(b);
        }        
      }
      for(dataiter = (wireiter->second).begin();dataiter!=(wireiter->second).end();++dataiter)//把最右的macro存回vector
      {
        if(tempt_left_pin_x==dataiter->pin_x)
        {
          b.direction="right";
          b.macro_name=dataiter->macro_name;
          find_boundary_macro.push_back(b);
        }        
      }  
      
     }//end if(wireiter)
   }//end for(wireiter)   
}


double place_macro::movable_distance(string macro_name, string direction){
    double distance=0;
    //cout<<fixed<<setprecision(0);
    double DTC = 0;
    double radius = pow( pow( chip_x_center , 2 ) + pow( chip_y_center , 2 ) , 0.5 );
    vector<Macro>::iterator miter=macrolist.begin();
    for(miter;miter!=macrolist.end();++miter){
        if(miter->name==macro_name){
            DTC = (pow( pow( (miter->center_x - chip_x_center) , 2 ) + pow( (miter->center_y - chip_y_center) , 2 ) , 0.5 )) / radius;
        }
    }
    if(direction=="left"){ 
        vector<Macro>::iterator miter=macrolist.begin();
        auto object_iter=miter;
        auto target_iter=miter;
        
        /**************** 鎖定object_iter *****************/
        right_xmap.clear();
        for(auto miter=macrolist.begin(); miter!=macrolist.end(); ++miter){
            right_xmap[(miter->x+miter->width)].push_back(miter->name);
            if(miter->name==macro_name)
                object_iter=miter; //令指定macro的macrolist iter=object_iter
        } 
        
        
        
        map<double, vector<string> >::iterator right_xiter=right_xmap.begin(); //第一層
        vector<string>::iterator same_x_miter=right_xiter->second.begin(); //第二層
        
        /********************* 輸出測試 *************************/ 
        /*
        cout<<"object_macro : "<<object_iter->name<<"\n==> (left_x , lower_y) = ("<<object_iter->x<<" , "<<object_iter->y<<")\n";
        cout<<"    (width , height) = ("<<object_iter->width<<" , "<<object_iter->height<<")\n";
        cout<<"    (right_x , upper_y) = ("<<object_iter->x+object_iter->width<<" , "<<object_iter->y+object_iter->height<<")\n"<<endl<<endl;  
        for(auto right_xiter=right_xmap.begin(); right_xiter!=right_xmap.end();++right_xiter){
            cout<<"right_xiter = "<<right_xiter->first<<"\nmacro: ";
            for(auto same_x_miter=right_xiter->second.begin(); same_x_miter!=right_xiter->second.end();++same_x_miter)
                cout<<*same_x_miter<<"    ";
            cout<<endl<<endl;
        }*/
        
        /*************** 使 mapiter 與 miter 相同 ***************/
        
        int quit=0;
        for(right_xiter=right_xmap.begin(); right_xiter!=right_xmap.end();++right_xiter){
            for(same_x_miter=right_xiter->second.begin(); same_x_miter!=right_xiter->second.end();++same_x_miter){
                if(macro_name == *same_x_miter){ //鎖定指定macro
                    quit=1;
                    break;
                }
            }//end same_x_iter
            if(quit==1)
                break;
        }//end xiter
        
        /*************** 鎖定target_iter並判斷distance ***************/
                
        while(distance < half_of_bounding_box_x){
            int overlap=0;
            if(right_xiter==right_xmap.begin())
                break;
            else
                right_xiter--;     
            distance = object_iter->x-right_xiter->first;
            if(macro_name=="A9_B24_C34_D11_F1_D2_E1_H1_F1_o1272089"){
                cout<<direction<<endl;
                cout<<"\n"<<object_iter->name<<" : "<<endl;
                cout<<"distance = object_iter->x - right_xiter->first = "<<distance<<" = "<<object_iter->x<<" - "<<right_xiter->first<<endl;
            }
            else{
                //cout<<"\n"<<object_iter->name<<" : "<<endl;
                //cout<<"distance = object_iter->x - right_xiter->first = "<<distance<<" = "<<object_iter->x<<" - "<<right_xiter->first<<endl;
            }
            if(distance < 0)
                continue;
            else if(distance>=displacement_max){
                distance = displacement_max;
                break;
            }
            else if(distance>=half_of_bounding_box_x){
                distance = half_of_bounding_box_x;
                break;
            }
            else{       
                for(same_x_miter=right_xiter->second.begin();same_x_miter!=right_xiter->second.end();++same_x_miter){
                    for(miter=macrolist.begin(); miter!=macrolist.end(); ++miter)
                        if(miter->name == *same_x_miter){
                            target_iter=miter; //令目標macro的macrolist iter=target_iter
                            overlap = overlap + y_overlap_detection(object_iter,target_iter);
                            break;
                        }
                }
                if(overlap==0)
                    continue;
                else
                    break;
            }//end else
        }//end for offset 
              
        if(distance < 0)
            distance=0;
            
    }//end if direction = left 
    
    
    
    
    
    else if(direction=="right"){
        vector<Macro>::iterator miter=macrolist.begin();
        auto object_iter=miter;
        auto target_iter=miter;
        
        /**************** 鎖定object_iter *****************/
        left_xmap.clear();
        for(auto miter=macrolist.begin(); miter!=macrolist.end(); ++miter){
            left_xmap[miter->x].push_back(miter->name);
            if(miter->name==macro_name)
                object_iter=miter; //令指定macro的macrolist iter=object_iter
        } 
        
        map<double, vector<string> >::reverse_iterator last_left_xiter=left_xmap.rbegin(); //最後一個xiter
        
        map<double, vector<string> >::iterator left_xiter=left_xmap.begin(); //第一層
        vector<string>::iterator same_x_miter=left_xiter->second.begin(); //第二層
        
        /*************** 輸出測試 ***************/
        /*
        cout<<"object_macro : "<<object_iter->name<<"\n==> (left_x , lower_y) = ("<<object_iter->x<<" , "<<object_iter->y<<")\n";
        cout<<"    (width , height) = ("<<object_iter->width<<" , "<<object_iter->height<<")\n";
        cout<<"    (right_x , upper_y) = ("<<object_iter->x+object_iter->width<<" , "<<object_iter->y+object_iter->height<<")\n"<<endl<<endl;    
        for(auto left_xiter=left_xmap.begin(); left_xiter!=left_xmap.end();++left_xiter){
            cout<<"left_xiter = "<<left_xiter->first<<"\nmacro: ";
            for(auto same_x_miter=left_xiter->second.begin(); same_x_miter!=left_xiter->second.end();++same_x_miter)
                cout<<*same_x_miter<<"    ";
            cout<<endl<<endl;
        }
        */
        /*************** 使 mapiter 與 miter 相同 ***************/
        int quit=0;
        for(left_xiter=left_xmap.begin(); left_xiter!=left_xmap.end();++left_xiter){
            for(same_x_miter=left_xiter->second.begin(); same_x_miter!=left_xiter->second.end();++same_x_miter){
                if(macro_name == *same_x_miter){ //鎖定指定macro
                    quit=1;
                    break;
                }
            }//end same_x_iter
            if(quit==1)
                break;
        }//end xiter
        
        /*************** 鎖定target_iter並判斷distance ***************/
           
        while(distance < half_of_bounding_box_x){       
            int overlap=0;
            if(left_xiter->first == last_left_xiter->first)
                break;
            else
                left_xiter++;
            distance = left_xiter->first-(object_iter->x+object_iter->width);
            if(macro_name=="A9_B24_C34_D11_F1_D2_E1_H1_F1_o1272089"){
                cout<<direction<<endl;
                cout<<"\n"<<object_iter->name<<" : "<<endl;
                cout<<"distance = left_xiter->first - (object_iter->x+object_iter->width) = "<<distance<<" = "<<left_xiter->first<<" - "<<object_iter->x+object_iter->width<<endl; 
            }
            /*else{            
                cout<<"\n"<<object_iter->name<<" : "<<endl;
                cout<<"distance = left_xiter->first - (object_iter->x+object_iter->width) = "<<distance<<" = "<<left_xiter->first<<" - "<<object_iter->x+object_iter->width<<endl;
            }*/
            if(distance < 0)
                continue;
            else if(distance>=(displacement_max)){
                distance = displacement_max;
                break;
            }
            else if(distance>=half_of_bounding_box_x){
                distance = half_of_bounding_box_x;
                break;
            }
            else{       
                for(same_x_miter=left_xiter->second.begin();same_x_miter!=left_xiter->second.end();++same_x_miter){
                    for(miter=macrolist.begin(); miter!=macrolist.end(); ++miter)
                        if(miter->name == *same_x_miter){
                            target_iter=miter; //令目標macro的macrolist iter=target_iter
                            overlap = overlap + y_overlap_detection(object_iter,target_iter);
                            break;
                        }
                }
                if(overlap==0)
                    continue;
                else
                    break;
            }//end else
        }//end for offset
        
        if(distance < 0)
            distance=0;
            
    }//end if direction = right
    
    
    
    
    
    else if(direction=="up"){
        vector<Macro>::iterator miter=macrolist.begin();
        auto object_iter=miter;
        auto target_iter=miter;
        
        /**************** 鎖定object_iter *****************/
        lower_ymap.clear();
        for(auto miter=macrolist.begin(); miter!=macrolist.end(); ++miter){
            lower_ymap[miter->y].push_back(miter->name);
            if(miter->name==macro_name)
                object_iter=miter; //令指定macro的macrolist iter=object_iter
        } 
        
        map<double, vector<string> >::reverse_iterator last_lower_yiter=lower_ymap.rbegin(); //最後一個yiter
        
        map<double, vector<string> >::iterator lower_yiter=lower_ymap.begin(); //第一層
        vector<string>::iterator same_y_miter=lower_yiter->second.begin(); //第二層
        
        /********************* 輸出測試 *************************/ 
        
        /*cout<<"object_macro : "<<object_iter->name<<"\n==> (left_x , lower_y) = ("<<object_iter->x<<" , "<<object_iter->y<<")\n";
        cout<<"    (width , height) = ("<<object_iter->width<<" , "<<object_iter->height<<")\n";
        cout<<"    (right_x , upper_y) = ("<<object_iter->x+object_iter->width<<" , "<<object_iter->y+object_iter->height<<")\n"<<endl<<endl;    
        for(auto lower_yiter=lower_ymap.begin(); lower_yiter!=lower_ymap.end();++lower_yiter){
            cout<<"lower_yiter = "<<lower_yiter->first<<"\nmacro: ";
            for(auto same_y_miter=lower_yiter->second.begin(); same_y_miter!=lower_yiter->second.end();++same_y_miter)
                cout<<*same_y_miter<<"    ";
            cout<<endl<<endl;
        }*/
        
        /*************** 使 mapiter 與 miter 相同 ***************/

        int quit=0;
        for(lower_yiter=lower_ymap.begin(); lower_yiter!=lower_ymap.end();++lower_yiter){
            for(same_y_miter=lower_yiter->second.begin(); same_y_miter!=lower_yiter->second.end();++same_y_miter){
                if(macro_name == *same_y_miter){ //鎖定指定macro
                    quit=1;
                    break;
                }
            }//end same_y_iter
            if(quit==1)
                break;
        }//end yiter
           
        /*************** 鎖定target_iter並判斷distance ***************/
        
        while(distance < half_of_bounding_box_y){       
            int overlap=0;
            if(lower_yiter->first == last_lower_yiter->first)
                break;
            else
                lower_yiter++;
            distance = lower_yiter->first-(object_iter->y+object_iter->height);            
            if(macro_name=="A9_B24_C34_D11_F1_D2_E1_H1_F1_o1272089"){
                cout<<direction<<endl;
                cout<<"\n"<<object_iter->name<<" : "<<endl;
                cout<<"distance = lower_yiter->first-(object_iter->y+object_iter->height) = "<<distance<<" = "<<lower_yiter->first<<" - "<<object_iter->y+object_iter->height<<endl;
            }
            else{
                //cout<<"\n"<<object_iter->name<<" : "<<endl;
                //cout<<"distance = lower_yiter->first-(object_iter->y+object_iter->height) = "<<distance<<" = "<<lower_yiter->first<<" - "<<object_iter->y+object_iter->height<<endl;
            }
            if(distance < 0)
                continue;
            else if(distance>=(displacement_max)){
                distance = displacement_max;
                break;
            }
            else if(distance>=half_of_bounding_box_y){
                distance = half_of_bounding_box_y;
                break;
            }
            else{       
                for(same_y_miter=lower_yiter->second.begin();same_y_miter!=lower_yiter->second.end();++same_y_miter){
                    for(miter=macrolist.begin(); miter!=macrolist.end(); ++miter)
                        if(miter->name == *same_y_miter){
                            target_iter=miter; //令目標macro的macrolist iter=target_iter
                            overlap = overlap + x_overlap_detection(object_iter,target_iter);
                            break;
                        }
                }
                if(overlap==0)
                    continue;
                else
                    break;
            }//end else
        }//end for offset
        
        if(distance < 0)
            distance=0;
            
    }//end if direction = up 
    
    
    
    
    
    else if(direction=="down"){
        vector<Macro>::iterator miter=macrolist.begin();
        auto object_iter=miter;
        auto target_iter=miter;
        
        /**************** 鎖定object_iter *****************/
        upper_ymap.clear();
        for(auto miter=macrolist.begin(); miter!=macrolist.end(); ++miter){
            upper_ymap[miter->y+miter->height].push_back(miter->name);
            if(miter->name==macro_name)
                object_iter=miter; //令指定macro的macrolist iter=object_iter
        } 
        
        map<double, vector<string> >::iterator upper_yiter=upper_ymap.begin(); //第一層
        vector<string>::iterator same_y_miter=upper_yiter->second.begin(); //第二層
        
        /********************* 輸出測試 *************************/ 
       /* 
        cout<<"object_macro : "<<object_iter->name<<"\n==> (left_x , lower_y) = ("<<object_iter->x<<" , "<<object_iter->y<<")\n";
        cout<<"    (width , height) = ("<<object_iter->width<<" , "<<object_iter->height<<")\n";
        cout<<"    (right_x , upper_y) = ("<<object_iter->x+object_iter->width<<" , "<<object_iter->y+object_iter->height<<")\n"<<endl<<endl;    
        for(auto upper_yiter=upper_ymap.begin(); upper_yiter!=upper_ymap.end();++upper_yiter){
            cout<<"upper_yiter = "<<upper_yiter->first<<"\nmacro: ";
            for(auto same_y_miter=upper_yiter->second.begin(); same_y_miter!=upper_yiter->second.end();++same_y_miter)
                cout<<*same_y_miter<<"    ";
            cout<<endl<<endl;
        }*/
        
        /*************** 使 mapiter 與 miter 相同 ***************/
        

        int quit=0;
        for(upper_yiter=upper_ymap.begin(); upper_yiter!=upper_ymap.end();++upper_yiter){
            for(same_y_miter=upper_yiter->second.begin(); same_y_miter!=upper_yiter->second.end();++same_y_miter){
                if(macro_name == *same_y_miter){ //鎖定指定macro
                    quit=1;
                    break;
                }
            }//end same_x_iter
            if(quit==1)
                break;
        }//end xiter
        
        /*************** 鎖定target_iter並判斷distance ***************/
 
           
        while(distance < half_of_bounding_box_y){       
            int overlap=0;
            if(upper_yiter==upper_ymap.begin())
                break;
            else
                upper_yiter--;
            distance = object_iter->y-upper_yiter->first;
            
            if(macro_name=="A9_B24_C34_D11_F1_D2_E1_H1_F1_o1272089"){
                cout<<direction<<endl;
                cout<<"\n"<<object_iter->name<<" : "<<endl;
                cout<<"distance = object_iter->y - upper_yiter->first = "<<distance<<" = "<<object_iter->y<<" - "<<upper_yiter->first<<endl;
            }
            else{
                //cout<<"\n"<<object_iter->name<<" : "<<endl;
                //cout<<"distance = object_iter->y - upper_yiter->first = "<<distance<<" = "<<object_iter->y<<" - "<<upper_yiter->first<<endl;
            }
            if(distance < 0)
                continue;
            else if(distance>=(displacement_max)){
                distance = displacement_max;
                break;
            }
            else if(distance>=half_of_bounding_box_y){
                distance = half_of_bounding_box_y;
                break;
            }
            else{       
                for(same_y_miter=upper_yiter->second.begin();same_y_miter!=upper_yiter->second.end();++same_y_miter){
                    for(miter=macrolist.begin(); miter!=macrolist.end(); ++miter)
                        if(miter->name == *same_y_miter){
                            target_iter=miter; //令目標macro的macrolist iter=target_iter
                            overlap = overlap + x_overlap_detection(object_iter,target_iter);
                            break;
                        }
                }
                if(overlap==0)
                    continue;
                else
                    break;
            }//end else
        }//end for offset
        
        if(distance < 0)
            distance=0;
            
    }//end if direction = down
    
    //distance = maximum_adjustion(macro_name,distance);
    return distance;
}


void place_macro::update_shifting_result(string macro_name,string direction,double distance){
    vector<Macro>::iterator miter=macrolist.begin();
    for(miter;miter!=macrolist.end();++miter){
        if(miter->name==macro_name){
            if(miter->fixed==1)
                break;
            else if(direction=="up")
            {
                miter->y = miter->y + distance;
                miter->center_y = miter->center_y + distance;
                vector<Pin> :: iterator piter=miter -> pinlist.begin();
                for(piter;piter!=miter->pinlist.end();++piter)
                {
                  piter->y = piter->y +distance;
                }          
            }
            else if(direction=="down")
            {
                miter->y = miter->y - distance;
                miter->center_y = miter->center_y - distance;
                vector<Pin> :: iterator piter=miter -> pinlist.begin();
                for(piter;piter!=miter->pinlist.end();++piter)
                {
                  piter->y = piter->y - distance;
                }    
            }
            else if(direction=="left")
            {
                miter->x = miter->x - distance;
                miter->center_x = miter->center_x - distance;
                vector<Pin> :: iterator piter=miter -> pinlist.begin();
                for(piter;piter!=miter->pinlist.end();++piter)
                {
                  piter->x = piter->x - distance;
                }  
            }
            else if(direction=="right")
            {
                miter->x = miter->x + distance;
                miter->center_x = miter->center_x + distance;
                vector<Pin> :: iterator piter=miter -> pinlist.begin();
                for(piter;piter!=miter->pinlist.end();++piter)
                {
                  piter->x = piter->x + distance;
                }  
            }
            miter->fixed = 1;
            break;
        }       
    }
}
double place_macro::maximum_adjustion(string macro_name,double distance){ 
    double DTC = 0;
    double radius = pow( pow( chip_x_center , 2 ) + pow( chip_y_center , 2 ) , 0.5 );
    vector<Macro>::iterator miter=macrolist.begin();
    for(miter;miter!=macrolist.end();++miter){
        if(miter->name==macro_name){
            DTC = (pow( pow( (miter->center_x - chip_x_center) , 2 ) + pow( (miter->center_y - chip_y_center) , 2 ) , 0.5 )) / radius;
        }
    }
    
    if(DTC<=0.2){
        if(distance <= 10000)
            return distance*0.5;
        else if (distance > 10000 && distance<=100000)
            return distance*0.7;
        else if (distance > 100000 && distance<=1000000)
            return distance*0.7;
        else 
            return distance*0.5;
    }
    else if (DTC>0.2&&DTC<=0.4){
        if(distance <= 10000)
            return distance*0.5;
        else if (distance > 10000 && distance<=100000)
            return distance*0.7;
        else if (distance > 100000 && distance<=1000000)
            return distance*0.7;
        else 
            return distance*0.5;
    }
    else if (DTC>0.4&&DTC<=0.6){
        if(distance <= 10000)
            return distance*0.5;
        else if (distance > 10000 && distance<=100000)
            return distance*0.7;
        else if (distance > 100000 && distance<=1000000)
            return distance*0.7;
        else 
            return distance*0.5;
    }
    else if (DTC>0.6&&DTC<=0.8){
        if(distance <= 10000)
            return distance*0.5;
        else if (distance > 10000 && distance<=100000)
            return distance*0.7;
        else if (distance > 100000 && distance<=1000000)
            return distance*0.7;
        else 
            return distance*0.5;
    }
    else{
        if(distance <= 10000)
            return distance*0.5;
        else if (distance > 10000 && distance<=100000)
            return distance*0.7;
        else if (distance > 100000 && distance<=1000000)
            return distance*0.7;
        else 
            return distance*0.5;
    }
}

int place_macro::x_overlap_detection(vector<Macro>::iterator object_iter, vector<Macro>::iterator target_iter){
    if((object_iter->x > (target_iter->x+target_iter->width)) || (target_iter->x > (object_iter->x+object_iter->width)))
        return 0;
    else
        return 1;
}
int place_macro::y_overlap_detection(vector<Macro>::iterator object_iter, vector<Macro>::iterator target_iter){
    if((object_iter->y > (target_iter->y+target_iter->height)) || (target_iter->y > (object_iter->y+object_iter->height)))
        return 0;
    else
        return 1;
}


