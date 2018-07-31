#include <iostream>
#include <libusb.h>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <cmath>
#include <string>
#include <sqlite3.h>
#include <map>
#include <sstream>
#include "cir.h"
#include "defines.h"
#include "decode_ir_code.h"
using namespace std;
map<int,uint64_t>code;
string static str_code;
string static protocol1;
string static size1;

struct database_t
{
	sqlite3 *db;
};

struct remote_t
{
	int size;	
	int protocol;
	uint16_t frequency;
	std::map<int,uint64_t>key_code;
	uint64_t code;
};

database_t *init_library(const char *db_name)
{
	database_t *db_handle = new database_t;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(db_name, &db_handle->db);

	if( rc )
	{
		cout<<"Can't open database: "<<sqlite3_errmsg(db_handle->db)<<"\n";
	} 

	else
	{
		cout<<"Database opened successfully"<<endl;
	}
	
	return db_handle;

}

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for(i=0; i<argc; i++)
	{
	}
	protocol1 = argv[1];
	size1 = argv[2];
	return 0;
} 

static int callback_map(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for(i=0; i<argc; i++)
	{
		int argv0 = stoi(argv[0]);
		uint64_t argv1;
		std::stringstream op;
		op<<std::hex<<argv[1];
		op>>argv1;
		code.insert(pair<int,uint64_t>(argv0,argv1));
	}

return 0;
} 

//returns protocol,size,frequency,map of keys and keycodes
remote_t *load_remote(int remote_id,struct database_t *db_handle)
{
	remote_t *remote = new remote_t;
	char *zErrMsg = 0;
	int rc;	
	std::string start1 = "select * from remotes where id = ";
	std::string pSQL11;
	std::stringstream sstm1;
	sstm1<<start1<<remote_id;
	pSQL11 = sstm1.str();

	const char *pSQLa = pSQL11.c_str();

		rc = sqlite3_exec(db_handle->db, pSQLa, callback, 0, &zErrMsg);
		if( rc!=SQLITE_OK )
		{
			cout<<"2SQL error: "<<sqlite3_errmsg(db_handle->db)<<"\n";
			sqlite3_free(zErrMsg);
			sqlite3_close(db_handle->db);
		}
	
	remote->protocol = stoi(protocol1);
	remote->size = stoi(size1);
	uint16_t frequency_list[16] = { 38000 , 36000 , 36000 , 36000 , 38000 , 57000 , 38000 , 40000 , 37000 , 38000 , 38000 , 38000 , 38000 , 57000 , 38000 , 38000};
	if(remote->protocol<16)
	{
		remote->frequency = frequency_list[remote->protocol];
	}	
	else
	{
		std::cout<<"frequency not found"<<std::endl;
	}

	std::string start = "select key,code from key_codes where remote_id = ";
	std::string pSQL1;
	std::stringstream sstm;
	sstm<<start<<remote_id;
	pSQL1 = sstm.str();

	const char *pSQLb = pSQL1.c_str();
	rc = sqlite3_exec(db_handle->db, pSQLb, callback_map , 0, &zErrMsg);
		if( rc!=SQLITE_OK )
		{
			cout<<"2SQL error: "<<sqlite3_errmsg(db_handle->db)<<"\n";
			sqlite3_free(zErrMsg);
			sqlite3_close(db_handle->db);
		}

	
	remote->key_code = code;
	
	return remote;
}

uint64_t get_code(int key_id ,remote_t *remote)
{
	std::map<int,uint64_t>::iterator it;
	it = remote->key_code.find(key_id);
	uint64_t code = it->second;
	remote->code = code;
	return code;
}

int get_protocol_id(remote_t *remote)
{
	int protocol_id = remote->protocol;
	return protocol_id;
}

int get_size(remote_t *remote)
{
	int size = remote->size;
	return size;
}

uint16_t get_frequency(int protocol_id)
{
	uint16_t frequency;
	uint16_t frequency_list[16] = { 38000 , 36000 , 36000 , 36000 , 38000 , 57000 , 38000 , 40000 , 37000 , 38000 , 38000 , 38000 , 38000 , 57000 , 38000 , 38000};
	if(protocol_id<16)
	{
		frequency = frequency_list[protocol_id];
	}	
	else
	{
		std::cout<<"frequency not found"<<std::endl;
	}
	return frequency;
}

void print_remote(int remote_id,struct remote_t *remote)
{
	std::cout<<"The remote ID is "<<remote_id<<std::endl;

	string name[16] = {"NEC" , "RC5" , "RC6" , "RCMM" , "XMP1" , "RC5_57" , "NEC_SHORT" , "SONY" , "PANASONIC" , "JVC" , "RC5_38" , "SHARP" , "RCA_38" , "RCA" , "MITSUBISHI" , "KONKA" };
	std::cout<<"The protocol is "<<name[remote->protocol]<<std::endl;

	std::cout<<"The frequency is "<<remote->frequency<<std::endl;

	std::cout<<"The size of the code sent is "<<remote->size<<std::endl;

	
 	map <int,uint64_t> :: iterator itr;
        for (itr =remote->key_code.begin(); itr != remote->key_code.end(); ++itr)
    	{
		uint64_t sec = itr->second;
		stringstream ss;
		ss<<hex<<sec;
		string hex_code = ss.str(); 
        	cout  <<  '\t' << itr->first
        	      <<  '\t' <<"0x"<<hex_code<< '\n';
    	}

   	std::cout<<"\n";
}

void print_key_and_code(int key_id,struct remote_t *remote)
{
	std::cout<<"The key is "<<key_id<<std::endl;
	string key_func[42] = {"KEY_0","KEY_1","KEY_2","KEY_3","KEY_4","KEY_5","KEY_6","KEY_7","KEY_8","KEY_9","KEY_VOL_UP ","KEY_VOL_DOWN ","KEY_VOL_MUTE ","KEY_CH_UP ","KEY_CH_DOWN ","KEY_POWER ","KEY_SELECT ","KEY_BACK ","KEY_EXIT ","KEY_MENU ","KEY_INFO ","KEY_UP_BUTTON ","KEY_DOWN_BUTTON ","KEY_LEFT_BUTTON ","KEY_RIGHT_BUTTON ","KEY_OK ","KEY_RED ","KEY_GREEN ","KEY_YELLOW ","KEY_BLUE ","KEY_PLAY ","KEY_REWIND ","KEY_FAST_FORWARD ","KEY_RECORD ","KEY_PAUSE ","KEY_STOP ","KEY_SOURCE ","KEY_OPTIONS ","KEY_LANGUAGE ","KEY_GUIDE ","KEY_PREVIOUS ","KEY_NEXT "};
	std::cout<<"The function is "<<key_func[key_id]<<std::endl;
	stringstream ss;
	ss<<hex<<remote->code;
	string hex_code = ss.str();
	std::cout<<"The code sent is 0x"<<hex_code<<std::endl;
	std::cout<<"\n";

}

uint16_t generate_buffer(uint16_t *data,remote_t *remote)
{
	data = new uint16_t;
	uint16_t written = 0;
	
	uint16_t offset[16] = {generate_nec(remote->code, remote->size, data) , generate_rc5(remote->code,remote->size,data) , generate_rc6(remote->code,remote->size,data) , generate_rcmm(remote->code,remote->size,data) , generate_xmp1(remote->code,remote->size,data) , generate_rc5_57(remote->code,remote->size,data) , generate_nec_short(remote->code,remote->size,data) , generate_sony(remote->code,remote->size,data) , generate_pana(remote->code,remote->size,data) ,  generate_jvc(remote->code,remote->size,data) , generate_rc5_38(remote->code,remote->size,data) , generate_sharp(remote->code,remote->size,data) , generate_rca_38(remote->code,remote->size,data) , generate_rca_57(remote->code,remote->size,data) ,  generate_mitsubishi(remote->code,remote->size,data) , generate_konka(remote->code,remote->size,data) };

	if(remote->protocol<16)
	{
		
		written += offset[remote->protocol];
	}	
	else
	{
		std::cout<<"protocol not defined"<<std::endl;
	}

	return written;
}


uint16_t generate_buffer_with_raw_data(uint16_t *data,uint64_t code,int size, int protocol_id)
{
	uint16_t written = 0;
	
	uint16_t offset[16] = {generate_nec(code, size, data) , generate_rc5(code,size,data) , generate_rc6(code,size,data) , generate_rcmm(code,size,data) , generate_xmp1(code,size,data) , generate_rc5_57(code,size,data) , generate_nec_short(code,size,data) , generate_sony(code,size,data) , generate_pana(code,size,data) ,  generate_jvc(code,size,data) , generate_rc5_38(code,size,data) , generate_sharp(code,size,data) , generate_rca_38(code,size,data) , generate_rca_57(code,size,data) ,  generate_mitsubishi(code,size,data) , generate_konka(code,size,data) };

	if(protocol_id<16)
	{
		
		written += offset[protocol_id];
	}	
	else
	{
		std::cout<<"protocol not defined"<<std::endl;
	}

	return written;
}

uint16_t generate_test_pattern(uint64_t code, uint32_t size, uint16_t *buffer)
{
    uint16_t offset = 0;

    for(int i = 0; i < 5; ++i) {
        buffer[offset++] = 5;
    }

    return offset;
}

void rewrite_pattern(uint32_t *target, uint16_t *buffer, uint16_t size) {
    for(size_t i = 0; i < size; ++i) {
        target[i] = buffer[i];
    }
}

struct device_state_t {
    uint8_t state;
    uint16_t last_operation_status;
    uint16_t frequency;
    uint16_t buffer_size;
    uint16_t buffer_target_size;
    uint16_t current_position;

    void show() {
        switch(state) {
            case STATE_IDLE:
                cout<<"State: Idle, Last Transaction result: "<<get_last_transaction_status(last_operation_status)<<endl;
                break;
            case 255:
                break;
            default:
                cout<<"State: "<<get_state_name(state)<<", Last Op: "<<get_last_transaction_status(last_operation_status)<<", Frequency: "<<frequency<<", Buffer Size: "
                    <<buffer_size<<", Target Size: "<<buffer_target_size<<", Position: "<<current_position<<endl;
                break;
        }
    }
private:
    const char* get_state_name(uint8_t state) {
        switch(state) {
            case STATE_IDLE:
                return "Idle";
            case STATE_CONFIGURED:
                return "Configured";
            case STATE_BUFFERING:
                return "Buffering";
            case STATE_BUFFERING_EMITTING:
                return "BufferringEmitting";
            case STATE_EMITTING:
                return "Emitting";
            default:
                return "Error";
        }
    }

    const char* get_last_transaction_status(uint16_t last_operation_status) {
        switch(last_operation_status) {
            case CODE_OK:
                return "CODE_OK";
            case CODE_START_UP:
                return "CODE_START_UP";
            case CODE_SUCCESS:
                return "CODE_SUCCESS";
            case CODE_BAD_FREQUENCY:
                return "CODE_BAD_FREQUENCY";
            case CODE_TOO_BIG_SIGNAL:
                return "CODE_TOO_BIG_SIGNAL";
            case CODE_BAD_SIZE:
                return "CODE_BAD_SIZE";
            case CODE_ILLEGAL:
                return "CODE_ILLEGAL";
            case CODE_UNDERFLOW:
                return "CODE_UNDERFLOW";
            case CODE_ERROR:
                return "CODE_ERROR";
        }
    }
};

#if (defined __GNUC__ && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__))
#define NO_TRANSFORM
#endif

uint16_t read_uint16(const uint8_t *source) {
#ifdef NO_TRANSFORM
    uint16_t result = 0;
    result |= *(source + 1);
    result <<= 8;
    result |= *source;
    return result;
#else
    uint16_t result = 0;
    result |= *source;
    source++;
    result <<= 8;
    result |= *source;
    return result;
#endif
}

void write_uint16(uint8_t* dest, uint16_t value) {
#ifdef NO_TRANSFORM
    uint8_t* source = (uint8_t*)&value;
    for(size_t i = 0; i < sizeof(uint16_t); ++i) {
        *dest++ = *(source + i);
    }    
#else
    uint8_t* source = (uint8_t*)&value;
    size_t size = sizeof(uint16_t) - 1;
    for(size_t i = 0; i < sizeof(uint16_t); ++i) {
        *dest++ = *(source + size - i);
    }
#endif
}

uint32_t read_uint32(const uint8_t *source) {
#ifdef NO_TRANSFORM
    uint32_t result = 0;
    uint8_t *dest = (uint8_t*)&result;
    for(size_t i = 0; i < sizeof(uint32_t); ++i) {
        *dest = *source;
        dest++;
        source++;
    }
    return result;
#else
    uint32_t result = 0;
    uint8_t *dest = (uint8_t*)&result + sizeof(uint32_t) - 1;
    for(size_t i = 0; i < sizeof(uint32_t); ++i) {
        *dest = *source;
        dest--;
        source++;
    }

    return result;
#endif
}

void write_uint32(uint8_t* dest, uint32_t value) {
#ifdef NO_TRANSFORM
    uint8_t* source = (uint8_t*)&value;
    for(size_t i = 0; i < sizeof(uint32_t); ++i) {
        *dest++ = *(source + i);
    }
#else
    uint8_t* source = (uint8_t*)&value;
    size_t size = sizeof(uint32_t) - 1;
    for(size_t i = 0; i < sizeof(uint32_t); ++i) {
        *dest++ = *(source + size - i);
    }
#endif
}

uint64_t read_uint64(const uint8_t *source) {
#ifdef NO_TRANSFORM
    uint64_t result = 0;
    uint8_t *dest = (uint8_t*)&result;
    for(size_t i = 0; i < sizeof(uint64_t); ++i) {
        *dest = *source;
        dest++;
        source++;
    }

    return result;
#else
    uint64_t result = 0;
    uint8_t *dest = (uint8_t*)&result + sizeof(uint64_t) - 1;
    for(size_t i = 0; i < sizeof(uint64_t); ++i) {
        *dest = *source;
        dest--;
        source++;
    }

    return result;
#endif
}

void write_uint64(uint8_t* dest, uint64_t value) {
#ifdef NO_TRANSFORM
    uint8_t* source = (uint8_t*)&value;
    for(size_t i = 0; i < sizeof(uint64_t); ++i) {
        *dest++ = *(source + i);
    }
#else
    uint8_t* source = (uint8_t*)&value;
    size_t size = sizeof(uint64_t) - 1;
    for(size_t i = 0; i < sizeof(uint64_t); ++i) {
        *dest++ = *(source + size - i);
    }
#endif
}

#if (defined __GNUC__ && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__))
#undef NO_TRANSFORM
#endif

device_state_t read_device_state(libusb_device_handle *dev_handle) {
    device_state_t ret_val;

    uint8_t data[BUFFER_FRAME_SIZE];
    int result;
    int actual;

    //Prepare a get config request.
    data[0] = OP_GET_CONFIG;
    result = libusb_interrupt_transfer(dev_handle, (2 | LIBUSB_ENDPOINT_OUT), data, 1, &actual, 0);
    if(result < 0) {
        cout<<"Request for device state failed"<<endl;
        ret_val.state = 255;
        return ret_val;
    }

    //read and wait for the result to come back.
    result = libusb_interrupt_transfer(dev_handle, (1 | LIBUSB_ENDPOINT_IN), data, BUFFER_FRAME_SIZE, &actual, 0);
    if(result < 0) {
        cout<<"Cannot read the device state response"<<endl;
        ret_val.state = 255;
        return ret_val;
    }

    int data_size = data[2];

    if(data[0] != OP_GET_CONFIG || data[1] != RESULT_CODE_SUCCESS || data_size == 0) {
        cout<<"Device reported an error while reading configuration"<<endl;
        cout<<uint32_t(data[0])<<", "<<uint32_t(data[1])<<", "<<actual<<endl;
        ret_val.state = 255;
        return ret_val;
    }

    //Decode the state.
    ret_val.state = data[3];
    if(ret_val.state == STATE_IDLE) {
        ret_val.last_operation_status = data[4];
    }
    else if(ret_val.state > STATE_IDLE && ret_val.state <= STATE_EMITTING) {
        ret_val.last_operation_status = data[4];
        ret_val.frequency = read_uint16(data + 5);
        ret_val.buffer_size = read_uint16(data + 7);
        ret_val.buffer_target_size = read_uint16(data + 9);
        ret_val.current_position = read_uint16(data + 11);
    }
    else {
        cout<<"Unknown device state reported: "<<uint32_t(ret_val.state)<<endl;
        cout<<uint32_t(data[0])<<", "<<uint32_t(data[1])<<", "<<uint32_t(data[2])<<", "<<uint32_t(data[3])<<endl;
        ret_val.state = 255;
    }

    return ret_val;
}

void read_and_print_device_state(libusb_device_handle *dev_handle) {
    device_state_t device_state = read_device_state(dev_handle);
    device_state.show();
}

void wait_for_state(libusb_device_handle *dev_handle, int loop_limit, int state) {
    for(int i = 0; i < loop_limit; ++i) {
        device_state_t device_state = read_device_state(dev_handle);
        if(device_state.state == state) {
            break;
        }
    }
}


void wait_for_state_printing(libusb_device_handle *dev_handle, int loop_limit, int state) {
    for(int i = 0; i < loop_limit; ++i) {
        device_state_t device_state = read_device_state(dev_handle);
        device_state.show();
        if(device_state.state == state) {
            break;
        }
    }
}

bool configure_device(libusb_device_handle *dev_handle, uint16_t frequency, uint16_t buffer_size) {
    uint8_t data[BUFFER_FRAME_SIZE];

    //First configure the device.
    data[0] = OP_SET_CONFIG;
    write_uint16(data + 1, frequency);
    write_uint16(data + 3, buffer_size);

    int result;
    int actual;
    result = libusb_interrupt_transfer(dev_handle, (2 | LIBUSB_ENDPOINT_OUT), data, 5, &actual, 0);
    if(result < 0) {
        cout<<"Unable to configure the device"<<endl;
        return false;
    }

    //read and wait for the result to come back.
    result = libusb_interrupt_transfer(dev_handle, (1 | LIBUSB_ENDPOINT_IN), data, BUFFER_FRAME_SIZE, &actual, 0);
    if(result < 0) {
        cout<<"Cannot read configure operation response"<<endl;
        return false;
    }

    if(data[0] != OP_SET_CONFIG || data[1] != RESULT_CODE_SUCCESS) {
        cout<<"Device configuration failed with an error"<<endl;
        return false;
    }

    wait_for_state(dev_handle, 10, STATE_CONFIGURED);

    return true;
}

bool start_buffering(libusb_device_handle *dev_handle) {
    uint8_t data[BUFFER_FRAME_SIZE];

    data[0] = OP_START_BUFFERING;
    int result;
    int actual;
    result = libusb_interrupt_transfer(dev_handle, (2 | LIBUSB_ENDPOINT_OUT), data, 5, &actual, 0);
    if(result < 0) {
        cout<<"Unable to start buffering on the device"<<endl;
        return false;
    }

    //read and wait for the result to come back.
    result = libusb_interrupt_transfer(dev_handle, (1 | LIBUSB_ENDPOINT_IN), data, BUFFER_FRAME_SIZE, &actual, 0);
    if(result < 0) {
        cout<<"Cannot read start buffering operation response"<<endl;
        return false;
    }

    if(data[0] != OP_START_BUFFERING || data[1] != RESULT_CODE_SUCCESS) {
        cout<<"Start buffering failed with an error"<<endl;
        return false;
    }

    wait_for_state(dev_handle, 10, STATE_BUFFERING);

    return true;
}

bool buffer_data(libusb_device_handle *dev_handle, uint8_t *buffer, uint16_t size) {
    int result = 0;
    int actual = 0;

    auto begin = std::chrono::high_resolution_clock::now();

    /*result = libusb_bulk_transfer(dev_handle, (4 | LIBUSB_ENDPOINT_OUT), buffer, size, &actual, 0);
    if(result < 0) {
        cout<<"Error Sending buffer to the device."<<endl;
        return false;
    }*/

    int start = 0;
    while(start < size) {
        int limit = min(BUFFER_FRAME_SIZE, size - start);
        
        result = libusb_bulk_transfer(dev_handle, (4 | LIBUSB_ENDPOINT_OUT), buffer + start, limit, &actual, 0);
        if(result < 0) {
            cout<<"Error Sending buffer to the device."<<endl;
            return false;
        }

        start += actual;

        read_and_print_device_state(dev_handle);
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count();

    cout<<"Time Elapsed: "<<duration<<" microseconds"<<endl;

    read_and_print_device_state(dev_handle);

    return true;
}

bool finish_buffering(libusb_device_handle *dev_handle) {
    uint8_t data[BUFFER_FRAME_SIZE];

    data[0] = OP_END_BUFFERING;
    int result;
    int actual;
    result = libusb_interrupt_transfer(dev_handle, (2 | LIBUSB_ENDPOINT_OUT), data, 5, &actual, 0);
    if(result < 0) {
        cout<<"Unable to end buffering on the device"<<endl;
        return false;
    }

    //read and wait for the result to come back.
    result = libusb_interrupt_transfer(dev_handle, (1 | LIBUSB_ENDPOINT_IN), data, BUFFER_FRAME_SIZE, &actual, 0);
    if(result < 0) {
        cout<<"Cannot read end buffering operation response"<<endl;
        return false;
    }

    if(data[0] != OP_END_BUFFERING || data[1] != RESULT_CODE_SUCCESS) {
        cout<<"End buffering failed with an error"<<endl;
        return false;
    }

    wait_for_state(dev_handle, 10, STATE_EMITTING);

    return true;
}

void release_device(libusb_context *ctx, libusb_device_handle *dev_handle) {
    int r = libusb_release_interface(dev_handle, 0); //release the claimed interface
    if(r!=0) {
        cout<<"Cannot Release Interface"<<endl;
        return;
    }
    cout<<"Released Interface"<<endl;

    libusb_close(dev_handle); //close the device we opened
    libusb_exit(ctx); //needs to be called to end the
}

int send_buffer(uint16_t buffer_size, uint16_t *data ,remote_t *remote = new remote_t)
{
	union 
	{
     	uint32_t target[8192];
     	uint8_t buffer[8192 * sizeof(uint32_t)];
	};

	libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
	libusb_device_handle *dev_handle; //a device handle
	libusb_context *ctx = NULL; //a libusb session
	int r; //for return values
	ssize_t cnt;

	 //holding number of devices in list
    r = libusb_init(&ctx); //initialize the library for the session we just declared
    if(r < 0) {
        cout<<"Init Error "<<r<<endl; //there was an error
        return 1;
    }
    libusb_set_debug(ctx, 3); //set verbosity level to 3, as suggested in the documentation

    cnt = libusb_get_device_list(ctx, &devs); //get the list of devices
    if(cnt < 0) {
        cout<<"Get Device Error"<<endl; //there was an error
        return 1;
    }
    cout<<cnt<<" Devices in list."<<endl;

    dev_handle = libusb_open_device_with_vid_pid(ctx, 10007, 61); //these are vendorID and productID I found for my usb device
    if(dev_handle == NULL)
        cout<<"Cannot open device"<<endl;
    else
        cout<<"Device Opened"<<endl;
    libusb_free_device_list(devs, 1); //free the list, unref the devices in it

    if(libusb_kernel_driver_active(dev_handle, 0) == 1) { //find out if kernel driver is attached
        cout<<"Kernel Driver Active"<<endl;
        if(libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it
            cout<<"Kernel Driver Detached!"<<endl;
    }
    r = libusb_claim_interface(dev_handle, 0); //claim interface 0 (the first) of device (mine had jsut 1)
    if(r < 0) {
        cout<<"Cannot Claim Interface"<<endl;
        return 1;
    }
    cout<<"Claimed Interface"<<endl;
	std::cout<<"\n";

    rewrite_pattern(target, data, buffer_size);

    //Multiply to get the size in bytes.
    buffer_size*= sizeof(uint32_t);

    read_and_print_device_state(dev_handle);
    if(!configure_device(dev_handle, remote->frequency, buffer_size)) {
        read_and_print_device_state(dev_handle);
        release_device(ctx, dev_handle);
        return -1;
    }
    read_and_print_device_state(dev_handle);

    if(!start_buffering(dev_handle)) {
        read_and_print_device_state(dev_handle);
        release_device(ctx, dev_handle);
        return -1;
    }
    read_and_print_device_state(dev_handle);

    if(!buffer_data(dev_handle, buffer, buffer_size)) {
        read_and_print_device_state(dev_handle);
        release_device(ctx, dev_handle);
        return -1;
    }

    if(!finish_buffering(dev_handle)) {
        read_and_print_device_state(dev_handle);
        release_device(ctx, dev_handle);
        return -1;
    }
    read_and_print_device_state(dev_handle);
	std::cout<<"\n";

    wait_for_state(dev_handle, 10000, STATE_IDLE);

	read_and_print_device_state(dev_handle);
	std::cout<<"\n";

    release_device(ctx, dev_handle); 
	std::cout<<"\n";

    return 1;

} 

int send_raw_code(uint16_t buffer_size , uint16_t *data, uint16_t frequency)
{
	union 
	{
     	uint32_t target[8192];
     	uint8_t buffer[8192 * sizeof(uint32_t)];
	};

	libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
	libusb_device_handle *dev_handle; //a device handle
	libusb_context *ctx = NULL; //a libusb session
	int r; //for return values
	ssize_t cnt;

	 //holding number of devices in list
    r = libusb_init(&ctx); //initialize the library for the session we just declared
    if(r < 0) {
        cout<<"Init Error "<<r<<endl; //there was an error
        return 1;
    }
    libusb_set_debug(ctx, 3); //set verbosity level to 3, as suggested in the documentation

    cnt = libusb_get_device_list(ctx, &devs); //get the list of devices
    if(cnt < 0) {
        cout<<"Get Device Error"<<endl; //there was an error
        return 1;
    }
    cout<<cnt<<" Devices in list."<<endl;

    dev_handle = libusb_open_device_with_vid_pid(ctx, 10007, 61); //these are vendorID and productID I found for my usb device
    if(dev_handle == NULL)
        cout<<"Cannot open device"<<endl;
    else
        cout<<"Device Opened"<<endl;
    libusb_free_device_list(devs, 1); //free the list, unref the devices in it

    if(libusb_kernel_driver_active(dev_handle, 0) == 1) { //find out if kernel driver is attached
        cout<<"Kernel Driver Active"<<endl;
        if(libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it
            cout<<"Kernel Driver Detached!"<<endl;
    }
    r = libusb_claim_interface(dev_handle, 0); //claim interface 0 (the first) of device (mine had jsut 1)
    if(r < 0) {
        cout<<"Cannot Claim Interface"<<endl;
        return 0;
    }
    cout<<"Claimed Interface"<<endl;
	std::cout<<"\n";

    rewrite_pattern(target, data, buffer_size);

    //Multiply to get the size in bytes.
    buffer_size *= sizeof(uint32_t);

    read_and_print_device_state(dev_handle);
    if(!configure_device(dev_handle, frequency, buffer_size)) {
        read_and_print_device_state(dev_handle);
        release_device(ctx, dev_handle);
        return -1;
    }
    read_and_print_device_state(dev_handle);

    if(!start_buffering(dev_handle)) {
        read_and_print_device_state(dev_handle);
        release_device(ctx, dev_handle);
        return -1;
    }
    read_and_print_device_state(dev_handle); 

    if(!buffer_data(dev_handle, buffer, buffer_size)) {
        read_and_print_device_state(dev_handle);
        release_device(ctx, dev_handle);
        return -1;
    }

    if(!finish_buffering(dev_handle)) {
        read_and_print_device_state(dev_handle);
        release_device(ctx, dev_handle);
        return -1;
    }
    read_and_print_device_state(dev_handle);
	std::cout<<"\n";

    wait_for_state(dev_handle, 10000, STATE_IDLE);

	read_and_print_device_state(dev_handle);
	std::cout<<"\n";

    release_device(ctx, dev_handle); 
	std::cout<<"\n";

    return 1;

}

int send_code(int key_id,remote_t *remote)
{
	uint16_t *data;
	uint16_t written;
	get_code(key_id,remote);
	written = generate_buffer(data,remote);
	int rc = send_buffer(written, data, remote);
	if(rc == 1)
	{
		return 1;
	}
	return 0;
}

int close_remote(remote_t *remote)
{
	code.clear(); 
	remote->size = 0;
	remote->protocol = 0;
	remote->frequency = 0;
	remote->code = 0;	
	return 1;
}

int close_library(struct database_t *db_handle)
{
	sqlite3_close(db_handle->db);
	std::cout<<"\n";
	return 1;
}

