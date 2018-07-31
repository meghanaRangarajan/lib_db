#include<stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
//protocol ids
const int protocol_nec = 0;
const int protocol_rc5 = 1 ;
const int protocol_rc6 = 2 ;
const int protocol_rcmm = 3 ;
const int protocol_xmp1 = 4 ;
const int protocol_rc5_57 = 5 ;
const int protocol_nec_short = 6;
const int protocol_sony = 7 ;
const int protocol_panasonic = 8 ;
const int protocol_jvc = 9 ;
const int protocol_rc5_38 = 10 ;
const int protocol_sharp = 11 ;
const int protocol_rca_38 = 12 ;
const int protocol_rca = 13 ;
const int protocol_mitsubishi = 14 ;
const int protocol_konka = 15 ;
const int protocol_xiomi = 16 ;
const int protocol_mce = 17 ;

//key ids
const int key_0 = 0;
const int key_1 = 1;
const int key_2 = 2;
const int key_3 = 3;
const int key_4 = 4;
const int key_5 = 5;
const int key_6 = 6;
const int key_7 = 7;
const int key_8 = 8;
const int key_9 = 9;
const int key_vol_up = 10;
const int key_vol_down = 11;
const int key_vol_mute = 12;
const int key_ch_up = 13;
const int key_ch_down = 14;
const int key_power = 15;
const int key_select = 16;
const int key_back = 17;
const int key_exit = 18;
const int key_menu = 19;
const int key_info = 20;
const int key_up_button = 21;
const int key_down_button = 22;
const int key_left_button = 23;
const int key_right_button = 24;
const int key_ok = 25;
const int key_red = 26;
const int key_green = 27;
const int key_yellow = 28;
const int key_blue = 29;
const int key_play = 30;
const int key_rewind = 31;
const int key_fast_forward = 32;
const int key_record = 33;
const int key_pause = 34;
const int key_stop = 35;
const int key_source = 36;
const int key_options = 37;
const int key_language = 38;
const int key_guide = 39;
const int key_previous = 40;
const int key_next = 41;

struct remote_t;
typedef remote_t remote_handle;

struct database_t;
typedef database_t db_handle;

db_handle *init_library(const char *db_name);

remote_handle *load_remote(int remote_id,db_handle *db);

uint64_t get_code(int key_id ,remote_t *remote);
int get_protocol_id(remote_t *remote);
int get_size(remote_t *remote);
uint16_t get_frequency(int protocol_id);

uint16_t generate_buffer(uint16_t *data,remote_t *remote);
int send_buffer(uint16_t buffer_size , uint16_t *data ,remote_handle *remote);

uint16_t generate_buffer_with_raw_data(uint16_t *data,uint64_t code,int size, int protocol_id);
int send_raw_code(uint16_t buffer_size , uint16_t *data , uint16_t frequency);

int send_code(int key_id,remote_t *remote);

void print_remote(int remote_id,struct remote_t *remote);

void print_key_and_code(int key_id,struct remote_t *remote);

int close_remote(remote_handle *remote);

int close_library(db_handle *db);
 
#ifdef __cplusplus 
}
#endif
