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


void place_macro::pin_number_in_macro(){
	int pin_in_macro_coordinate;
	
  //�]�L�C��macro
  vector<Macro>::iterator miter = macrolist.begin();
  for (miter; miter != macrolist.end(); ++miter){
    //��Xmacro���̦hpin���ϰ�---------------------------------------------
    vector<Pin>::iterator piter = miter->pinlist.begin();
    int left_bottom=0;
    int right_bottom=0;
    int left_up=0;
    int right_up=0; //use four variable to save the pin �y�� in the macro �H�� 
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
    
    //��j�p
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
    miter->orient = FlipMacro( miter->orient , (biggest+1) , macro_in_chip(miter->center_x , miter->center_y) );
    
	}	
}  

string place_macro::return_wirename(string macro_name, string pin_name){
    vector<Data>::iterator wireiter=wirelist.begin();
    for(wireiter; wireiter!=wirelist.end(); ++wireiter){
        if(wireiter->macro_name==macro_name && wireiter->pin_name==pin_name) {return wireiter->wire_name;  break;}
    }
}



//��Xmacro�bchip�W����m
int place_macro::macro_in_chip(double center_x,double center_y){
	
	  int macro_in_chip_coordinate; //judge macro in chip ���H�� �k�W=1,���W=2,���U=3,�k�U=4 
	  Macro M;
	
	  if(center_x > chip_x_center && center_y > chip_y_center){
		  macro_in_chip_coordinate=1;
	  }
	  else if(center_x < chip_x_center && center_y > chip_y_center){
	  	macro_in_chip_coordinate=2;
	  }
	  else if(center_x < chip_x_center && center_y < chip_y_center){
	  	macro_in_chip_coordinate=3;
	  }
	  else if(center_x > chip_x_center && center_y < chip_y_center){
	  	macro_in_chip_coordinate=4;
	  }
	  else if(center_x < chip_x_center && center_y == chip_y_center){
	  	macro_in_chip_coordinate=5;
	  }
	  else if(center_x > chip_x_center && center_y == chip_y_center){
	  	macro_in_chip_coordinate=6;
	  }
	  else if(center_x == chip_x_center && center_y > chip_y_center){
	  	macro_in_chip_coordinate=7;
	  }
	  else if(center_x == chip_x_center && center_y < chip_y_center){
	  	macro_in_chip_coordinate=8;
	  }
	  else if(center_x == chip_x_center && center_y == chip_y_center){
		  macro_in_chip_coordinate=9;
	  }
	  return macro_in_chip_coordinate;
}

//½��macro
int place_macro::FlipMacro(int macro_direction , int maximum_pin_region , int macro_pos){
	  int flip_operation; // 1: horizontal 2:vertical 3:both 
	  //�M�w���ƻ�½�� 
    switch(macro_pos) {  
        case 1: //macro�b�Ĥ@�H��
            //cout<<"Macro is at right_up of chip"<<endl;
            if(maximum_pin_region==1)
            	flip_operation = 3;
			      else if(maximum_pin_region==2)
				      flip_operation = 2;
			      else if(maximum_pin_region==4)
				      flip_operation = 1;
            break; 
        case 2: //macro�b�ĤG�H�� 
            //cout<<"Macro is at left_up of chip"<<endl;
            if(maximum_pin_region==1)
            	flip_operation = 2;
			      else if(maximum_pin_region==2)
				      flip_operation = 3;
			      else if(maximum_pin_region==3)
				      flip_operation = 1;
            break; 
        case 3: //macro�b�ĤT�H�� 
            //cout<<"Macro is at left_bottom of chip"<<endl;
			      if(maximum_pin_region==2)
				      flip_operation = 1;
			      else if(maximum_pin_region==3)
				      flip_operation = 3;
			      else if(maximum_pin_region==4)
				      flip_operation = 2;
            break; 
        case 4: //macro�b�ĥ|�H�� 
            //cout<<"Macro is at right_bottom of chip"<<endl;
            if(maximum_pin_region==1)
            	flip_operation = 1;
			      else if(maximum_pin_region==3)
				      flip_operation = 2;
			      else if(maximum_pin_region==4)
				      flip_operation = 3;
				    break; 
        case 5: //macro�bchip����x�b�W�a��
            //cout<<"Macro is at -x of chip"<<endl; 
            if(maximum_pin_region==2||maximum_pin_region==3)
            	flip_operation = 1;
            break;   
		    case 6: //macro�bchip����x�b�W�a�k
            //cout<<"Macro is at +x of chip"<<endl; 
            if(maximum_pin_region==1||maximum_pin_region==4)
            	flip_operation = 1; 	
            break;   
		    case 7: //macro�bchip����y�b�W�a�W
            //cout<<"Macro is at +y of chip"<<endl; 
            if(maximum_pin_region==1||maximum_pin_region==2)
            	flip_operation = 2; 	
            break;   
        case 8: //macro�bchip����y�b�W�a�U 
            //cout<<"Macro is at -y of chip"<<endl;
            if(maximum_pin_region==3||maximum_pin_region==4)
            	flip_operation = 2;
            break;   
        case 9: //macro�bchip������ 
            //cout<<"Macro is at the center of chip"<<endl;
            break;
        default: 
            //cout << "Macro_flipping failed." << endl; 
            break;
    } 
    
    return flip_operation;
}

int place_macro::update_new_orient(int flip_operation, int macro_direction){
    switch(flip_operation){
    	  case 1: //����½�� 
    		    //cout<<"Thus, we doing flipping by horrizontal."<<endl;
            if(macro_direction==0) macro_direction = 1;
    		    else if(macro_direction==1) macro_direction = 0;
    		    else if(macro_direction==2) macro_direction = 3;
    		    else if(macro_direction==3) macro_direction = 2;    		
    		    break;
    	  case 2: //����½�� 
    		    //cout<<"Thus, we doing flipping by vertical."<<endl;
            if(macro_direction==0) macro_direction = 3;
    		    else if(macro_direction==1) macro_direction = 2;
    		    else if(macro_direction==2) macro_direction = 1;
    		    else if(macro_direction==3) macro_direction = 0;    		
    		    break;
    	  case 3: //�����M������½ 
    		    //cout<<"Thus, we doing flipping by both horrizontal and vertical."<<endl;
            if(macro_direction==0) macro_direction = 2;
    		    else if(macro_direction==1) macro_direction = 3;
    		    else if(macro_direction==2) macro_direction = 0;
    		    else if(macro_direction==3) macro_direction = 1;	
    		    break;
    	  default:
            //cout<<"Thus, we don't do flipping."<<endl;
    		    break;
	  }
    return macro_direction;
}


void place_macro::update_macrolist_pinlist(){
    vector<Macro>::iterator miter=macrolist.begin();
    for(miter; miter != macrolist.end(); ++miter){
        //miter->orient==0 ����½��
        if(miter->orient==1){
            vector<Pin>::iterator piter = miter->pinlist.begin();
            for (piter; piter != miter->pinlist.end(); ++piter){
                (piter->x)=2*(miter->center_x)-(piter->x);
            }
        }            
        else if(miter->orient==2){
            vector<Pin>::iterator piter = miter->pinlist.begin();
            for (piter; piter != miter->pinlist.end(); ++piter){
                (piter->x)=2*(miter->center_x)-(piter->x);
                (piter->y)=2*(miter->center_y)-(piter->y);
            }       
        }        
        else if(miter->orient==3){
            vector<Pin>::iterator piter = miter->pinlist.begin();
            for (piter; piter != miter->pinlist.end(); ++piter){
                (piter->y)=2*(miter->center_y)-(piter->y);
            }       
        }
        else ;
    }
}

void place_macro::update_one_macro(string macro_name , int operation){
    vector<Macro>::iterator miter=macrolist.begin();
    vector<Pin>::iterator piter = miter->pinlist.begin();
    for(miter=macrolist.begin(); miter!=macrolist.end(); ++miter){
        if(miter->name == macro_name){
            if(operation==1){
                for (piter = miter->pinlist.begin() ; piter != miter->pinlist.end() ; ++piter){
                    (piter->x)=2*(miter->center_x)-(piter->x);
                }
            }            
            else if(operation==3){
                for (piter = miter->pinlist.begin() ; piter != miter->pinlist.end() ; ++piter){
                    (piter->x)=2*(miter->center_x)-(piter->x);
                    (piter->y)=2*(miter->center_y)-(piter->y);
                }       
            }        
            else if(operation==2){
                for (piter = miter->pinlist.begin() ; piter != miter->pinlist.end() ; ++piter){
                    (piter->y)=2*(miter->center_y)-(piter->y);
                }       
            }
            else ;       
        }
    }
}

void place_macro::update_one_macro_fake(string macro_name , int operation){
    vector<Macro>::iterator miter=macrolist_fake.begin();
    vector<Pin>::iterator piter = miter->pinlist.begin();
    for(miter=macrolist_fake.begin(); miter!=macrolist_fake.end(); ++miter){
        if(miter->name == macro_name){
            if(operation==1){
                for (piter = miter->pinlist.begin() ; piter != miter->pinlist.end() ; ++piter){
                    (piter->x)=2*(miter->center_x)-(piter->x);
                }
            }            
            else if(operation==3){
                for (piter = miter->pinlist.begin() ; piter != miter->pinlist.end() ; ++piter){
                    (piter->x)=2*(miter->center_x)-(piter->x);
                    (piter->y)=2*(miter->center_y)-(piter->y);
                }       
            }        
            else if(operation==2){
                for (piter = miter->pinlist.begin() ; piter != miter->pinlist.end() ; ++piter){
                    (piter->y)=2*(miter->center_y)-(piter->y);
                }       
            }
            else ;       
        }
    }
}

//macro ���n�Ƨ�
void place_macro::Area_Arrangement(){
    double area_tempt;
    double area_total;
    area_total = (chip_x_center * 2) * (chip_y_center * 2);
    vector<Macro>::iterator miter=macrolist.begin();
    for(miter; miter!=macrolist.end(); ++miter){
        area_tempt=(miter->width)*(miter->height);
        areamap[area_tempt].push_back(miter->name);
        miter->area=area_tempt / area_total;
    }
    
}

//�Z���Ƨ�
void place_macro::Distance_Arrangement(){
    double dis_tempt = 0 ;
    double radius = 0; //�������ߨ촹����ɪ��̻��Z��
    radius = pow( pow( chip_x_center , 2 ) + pow( chip_y_center , 2 ) , 0.5 );
    vector<Macro>::iterator miter=macrolist.begin();
    for(miter; miter!=macrolist.end(); ++miter){
        dis_tempt= pow( pow( (miter->center_x - chip_x_center) , 2 ) + pow( (miter->center_y - chip_y_center) , 2 ) , 0.5 ); //�p��macro���ߩMchip���߶��Z
        dismap[dis_tempt].push_back(miter->name); //�إ�distance map
        miter->distance = dis_tempt / radius; //��Z�����macrolist
    }
}

void place_macro::Percentage_Arrangement(){
    double percentage_tempt = 0 ;
    vector<Macro>::iterator miter=macrolist.begin();
    for(miter; miter!=macrolist.end(); ++miter){
        percentage_tempt= (miter->macronum)/(miter->pinlist.size()) ; //�p��pin����~��macro�����macro����v
        percentagemap[percentage_tempt].push_back(miter->name); //�إ�percentage map
        miter->percentage = percentage_tempt; //���v���macrolist
    }
}

//area_weight�G���n�v�� / distance_weight�G�Z���v�� �A �v����p��
void place_macro::Weight_Arrangement( float area_weight , float distance_weight , float percentage_weight){
    double weight_tempt = 0 ;
    vector<Macro>::iterator miter=macrolist.begin();
    for(miter; miter!=macrolist.end(); ++miter){
        weight_tempt = area_weight*miter->area + distance_weight*miter->distance + percentage_weight*miter->percentage ; 
        weightmap[weight_tempt].push_back(miter->name); //�إ�weight map
    }
}

/*
//originalOPT
void place_macro::Optimization(){
    string wire_name;
    vector<XY> xy_tempt;
    vector<Macro>::iterator miter=macrolist.begin();
    
    for(auto miter=macrolist.rbegin(); miter != macrolist.rend(); ++miter){
       cout<<"Macro Name : "<<endl;
       cout<<miter->name<<endl;
       
       int ver_external_big = 0;
       int hor_external_big = 0;
       int ver_internal_big = 0;
       int hor_internal_big = 0;
       int ver_result[2] = {0,0};
       int hor_result[2] = {0,0};
       int connection = 0;

       miter->macronum=0;
       miter->stdnum=0;
       vector<Pin>::iterator piniter=miter->pinlist.begin();
       hor_internal_big = judge_pin_x_orient(miter->pinlist, miter->center_x);
       ver_internal_big = judge_pin_y_orient(miter->pinlist, miter->center_y);
       int count=0;
       for(piniter; piniter!=miter->pinlist.end(); ++piniter){
       
           if(count<20) cout<<"origin pin : "<<piniter->x<<"  "<<piniter->y<<endl;
           count++;
           
           wire_name=find_pin(miter->name, piniter->name); 
           if(find_connected_pin(wire_name, piniter->x, piniter->y).size()!=0){
             xy_tempt=find_connected_pin(wire_name, piniter->x, piniter->y);
             vertical_external(miter->center_y, xy_tempt,ver_result);          
             horizontal_external(miter->center_x, xy_tempt, hor_result);
             connection++;
             (miter->macronum)++;
           }
           else (miter->stdnum)++;
       }
       if(connection==0){
         cout<<"Didn't connected to any macro !!!"<<endl<<endl<<endl; 
         continue;
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
       cout<<"The origin orient is "<<miter->orient;
       
       if((hor_external_big==1&&hor_internal_big==2)||(hor_external_big==2&&hor_internal_big==1)){
           miter->orient = update_new_orient(1, miter->orient);
           update_one_macro(miter->name , 1);
           cout<<" ======> New orient after horrizontal flipping is "<<miter->orient;
       }
       else
           cout<<" ======> No horrizontal flipping!!!";
       if((ver_external_big==1&&ver_internal_big==2)||(ver_external_big==2&&ver_internal_big==1)){
           miter->orient = update_new_orient(2, miter->orient);
           update_one_macro(miter->name , 2);
           cout<<" ======> New orient after vertical flipping is "<<miter->orient<<endl<<endl<<endl;
       }
       else   
           cout<<" ======> No vertical flipping!!!"<<endl<<endl<<endl;    
       
       
       piniter=miter->pinlist.begin();
       count=0;
       for(piniter; piniter!=miter->pinlist.end(); ++piniter){
       
           if(count<20) cout<<"after pin : "<<piniter->x<<"  "<<piniter->y<<endl;
           else break;
           count++;
           
       }
       cout<<endl<<endl;
    } //end miter for
}

*/


//areaOPT 
void place_macro::Optimization(){
    string wire_name;
    vector<XY> xy_tempt;
    //vector<Macro>::iterator miter=macrolist.begin();
    map<double, vector<string> >:: iterator areaiter ;
    
    //for(miter; miter != macrolist.end(); ++miter){
    
    for(auto areaiter=areamap.rbegin() ; areaiter!=areamap.rend(); ++areaiter){
        vector<string>::iterator miter=areaiter->second.begin();
        for(miter; miter!=areaiter->second.end(); ++miter){  //miter�O�]areamap�᭱��vector<Macro>��!!!!!
            vector<Macro>::iterator macroiter=macrolist.begin();
            for(macroiter=macrolist.begin(); macroiter!=macrolist.end(); ++macroiter){
                if(macroiter->name==*miter){
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
                                    //�Lpin�ˬd/////
                        //for(int i=0; i<xy_tempt.size(); i++){ //xy_tempt.size()=1�N����macro pin�u����std cell
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
                    //����
                    //cout<<"=========> Internal comparison : horizontal = "<<hor_internal_big<<" , vertical= "<<ver_internal_big<<endl;
                    //cout<<"External: Right = "<<hor_result[1]<<" ,Left = "<<hor_result[0]<<endl;
                    //cout<<"          Up    = "<<ver_result[1]<<" ,Down  = "<<ver_result[0]<<endl;
                    //cout<<"=========> External comparison : horizontal = "<<hor_external_big<<" , vertical= "<<ver_external_big<<endl;
                    //cout<<"Number of pins = "<<miter->pinlist.size()<<endl;
                    //cout<<"The origin orient is "<<miter->orient;
                    
                    cout<<"The origin orient is "<<macroiter->orient;
                    
                    if((hor_external_big==1&&hor_internal_big==2)||(hor_external_big==2&&hor_internal_big==1)){
                        macroiter->orient = update_new_orient(1, macroiter->orient);
                        update_one_macro(macroiter->name, 1);
                        cout<<" ======> New orient after horrizontal flipping is "<<macroiter->orient;
                    }
                    else
                        cout<<" ======> No horrizontal flipping!!!";
                    if((ver_external_big==1&&ver_internal_big==2)||(ver_external_big==2&&ver_internal_big==1)){
                        macroiter->orient = update_new_orient(2, macroiter->orient);
                        update_one_macro(macroiter->name, 2);
                        cout<<" ======> New orient after vertical flipping is "<<macroiter->orient<<endl<<endl<<endl;
                    }
                    else   
                        cout<<" ======> No vertical flipping!!!"<<endl;
                    result_test[macroiter->name]=macroiter->orient;     
                    break;                    
                } //end if ���macro�W�r
            } //end macroiter for
         } //end miter for
    } //end areaiter for
}

/*


//distanceOPT

void place_macro::Optimization(){
    string wire_name;
    vector<XY> xy_tempt;
    //map<string, string> result_test;
    //vector<Macro>::iterator miter=macrolist.begin();
    map<double, vector<string> >:: iterator disiter ;
    
    //for(miter; miter != macrolist.end(); ++miter){
    
    for(auto disiter=dismap.rbegin() ; disiter!=dismap.rend(); ++disiter){
        vector<string>::iterator miter=disiter->second.begin();
        for(miter; miter!=disiter->second.end(); ++miter){  //miter�O�]areamap�᭱��vector<Macro>��!!!!!
            vector<Macro>::iterator macroiter=macrolist.begin();
            for(macroiter=macrolist.begin(); macroiter!=macrolist.end(); ++macroiter){
                if(macroiter->name==*miter){
                    cout<<"Area : "<<disiter->first<<endl;
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
                                    //�Lpin�ˬd/////
                        //for(int i=0; i<xy_tempt.size(); i++){ //xy_tempt.size()=1�N����macro pin�u����std cell
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
                    //����
                    //cout<<"=========> Internal comparison : horizontal = "<<hor_internal_big<<" , vertical= "<<ver_internal_big<<endl;
                    //cout<<"External: Right = "<<hor_result[1]<<" ,Left = "<<hor_result[0]<<endl;
                    //cout<<"          Up    = "<<ver_result[1]<<" ,Down  = "<<ver_result[0]<<endl;
                    //cout<<"=========> External comparison : horizontal = "<<hor_external_big<<" , vertical= "<<ver_external_big<<endl;
                    //cout<<"Number of pins = "<<miter->pinlist.size()<<endl;
                    //cout<<"The origin orient is "<<miter->orient;
                    
                    cout<<"The origin orient is "<<macroiter->orient;
                    
                    if((hor_external_big==1&&hor_internal_big==2)||(hor_external_big==2&&hor_internal_big==1)){
                        macroiter->orient = update_new_orient(1, macroiter->orient);
                        update_one_macro(macroiter->name, 1);
                        cout<<" ======> New orient after horrizontal flipping is "<<macroiter->orient;
                    }
                    else
                        cout<<" ======> No horrizontal flipping!!!";
                    if((ver_external_big==1&&ver_internal_big==2)||(ver_external_big==2&&ver_internal_big==1)){
                        macroiter->orient = update_new_orient(2, macroiter->orient);
                        update_one_macro(macroiter->name, 2);
                        cout<<" ======> New orient after vertical flipping is "<<macroiter->orient<<endl<<endl<<endl;
                    }
                    else   
                        cout<<" ======> No vertical flipping!!!"<<endl;
                    result_test[macroiter->name]=macroiter->orient;     
                    break;    
                               
                } //end if ���macro�W�r
            } //end macroiter for
         } //end miter for
    } //end areaiter for
}
*/

void place_macro::print_result(){
    map<string, int>::iterator mapiter=result_test.begin();
    for(mapiter; mapiter!=result_test.end(); ++mapiter){
        cout<<"macro name : "<<mapiter->first<<"  macro orient : "<<mapiter->second<<endl;
    }
}

string place_macro::find_pin(string macro_name, string pin_name){ 
    vector<Data>::iterator dataiter=wirelist.begin();
    for(dataiter; dataiter!=wirelist.end(); ++dataiter){
        if(dataiter->macro_name==macro_name && dataiter->pin_name==pin_name) return dataiter->wire_name;
    }
}

vector<XY> place_macro::find_connected_pin(string wire_name, double x_in, double y_in){
    XY tempt_xy;
    vector<XY> vec;
    vec.clear();
    string tempt_macro_name;
    map<string, vector<Data> >::iterator mapiter=wiremap.begin();

    for(mapiter; mapiter!=wiremap.end(); ++mapiter){
        
        if(mapiter->first==wire_name ){
            //��Xx_in�My_in�Ӧۭ���macro(�ץ��u���s�b�P��macro�����D)
            
            vector<Data>::iterator veciter=mapiter->second.begin();
            for(veciter=mapiter->second.begin(); veciter!=mapiter->second.end(); ++veciter){
                if(veciter->pin_x==x_in && veciter->pin_y==y_in){
                    tempt_macro_name = veciter->macro_name;
                }
            }
            
            
            //�N���F����pin���y�г��s��vector�^��
            //vector<Data>::iterator veciter=mapiter->second.begin();
            for(veciter=mapiter->second.begin(); veciter!=mapiter->second.end(); ++veciter){
                tempt_xy.x=veciter->pin_x;
                tempt_xy.y=veciter->pin_y;
                if((x_in!=tempt_xy.x && y_in!=tempt_xy.y) || (veciter->macro_name!=tempt_macro_name))vec.push_back(tempt_xy);
                //if(x_in!=tempt_xy.x && y_in!=tempt_xy.y)vec.push_back(tempt_xy);
            }
        }
    }
    return vec;
}

int place_macro::vertical_external(double center_y, vector<XY>xy, int ver_result[2]){
    for(int i=0; i<xy.size(); i++){
        if(xy[i].y>center_y) ver_result[1]++;
        else if(xy[i].y<center_y) ver_result[0]++;
        else ;
    }
}

int place_macro::horizontal_external(double center_x, vector<XY>xy,int hor_result[2]){
    for(int i=0; i<xy.size(); i++){
        if(xy[i].x>center_x) hor_result[1]++;
        else if(xy[i].x<center_x) hor_result[0]++;
        else ;
    }
}

int place_macro::judge_pin_x_orient(vector <Pin> & pinlist,double center_x){
  vector<Pin>::iterator piter=pinlist.begin();
  int left=0;
  int right=0;
  for (piter; piter != pinlist.end(); ++piter){	
      if(piter->x < center_x ) left++; 
      else if(piter->x > center_x )	right++;   		
  }
  //cout<<"Internal: Right = "<<right<<" ,Left = "<<left<<endl;
  if(left<right) 
      return 1;
  else if(left>right) 
      return 2;
  else 
      return 3;
}


int place_macro::judge_pin_y_orient(vector <Pin> & pinlist,double center_y){
  vector<Pin>::iterator piter=pinlist.begin();
  int up=0;
  int down=0;
  for (piter; piter != pinlist.end(); ++piter){	
      if(piter->y < center_y ) down++; 
      else if(piter->y > center_y )	up++;   		
  }
  //cout<<"          Up    = "<<up<<" ,Down = "<<down<<endl;
  if(down<up)
      return 1;
  else if(down>up) 
      return 2;
  else 
      return 3;
}

//�Ѱ�macro��fixed���A(���Fprototype�N�Ofiexed���H�H�~)
//�u�n���ܥ����ܼ� unfixed�Cunfixed=0(�w�])�Bunfixed=1�N�����Ѱ��T�w�C
void place_macro::unfixed_macro(bool unfixed){
    vector<Macro>::iterator miter=macrolist.begin();
    for(miter=macrolist.begin(); miter!=macrolist.end(); ++miter){
        if(miter->preplaced==0) miter->fixed = 0; 
    }
}
