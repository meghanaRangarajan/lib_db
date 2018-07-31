#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <string>
#include "cir.h"
using namespace std;

int main()
{
	std::cout<<"_____________________________________________Session started____________________________________________"<<std::endl;
	remote_handle *remote;
	db_handle *db;
	int remote_id;
	int ans_db = 1; 
	while(ans_db == 1)
	{
		std::string name;
	    std::cout<<"Enter the name of the database : "<<"\n";
	   	std::cin>>name;
	    const char *db_name = name.c_str();
		db = init_library(db_name);
		
		std::cout<<"\n";
		int ans_remote = 1;
		while(ans_remote == 1)
		{
			std::cout<<"Enter the remote id : ";
			std::cin>>remote_id;
			remote = load_remote(remote_id,db);
			print_remote(remote_id,remote);
			
			int ans_code = 1; 
			while(ans_code == 1)
			{
				std::cout<<"\n";
				int key_id;
				std::cout<<"Enter the key id : ";
				std::cin>>key_id;

				get_code(key_id,remote);
				print_key_and_code(key_id,remote);

				int rc = send_buffer(remote);
				if (rc == 1)
				{
					std::cout<<"Code sent successfully"<<std::endl;
				}
				else
				{
					std::cout<<"Error has occurred while sending the code"<<std::endl;
				}
				std::cout<<"\n";
				
				std::cout<<"Do you want to send another code? 1/0 : ";
				std::cin>>ans_code;
				
			}
			int cl = close_remote(remote);
			if(cl == 1)
			{
				std::cout<<"Remote closed"<<std::endl;
			}
			std::cout<<"\n";
			std::cout<<"Do you want to select another remote? 1/0 : ";  
			std::cin>>ans_remote;
		}
		int cll = close_library(db);
		if(cll == 1)
		{
			std::cout<<"Database is closed"<<"\n";
		}	
		std::cout<<"\n";
		std::cout<<"Do you want to open another database? 1/0 : ";  
		std::cin>>ans_db;
		std::cout<<"\n";
	}

	std::cout<<"_____________________________________________Session ended_____________________________________________"<<std::endl;
	return 1;


}