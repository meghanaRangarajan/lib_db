#define     DEVICE_VERSION              1


/* Ops related definition */

#define     OP_GET_CONFIG               1

#define     OP_SET_CONFIG               2

#define     OP_START_BUFFERING          3

#define     OP_END_BUFFERING            4


#define     OP_DFU_START                42


#define     OP_RESET                    120


#define     OP_GET_DEVICE_VERSION       127



#define     RESULT_CODE_SUCCESS         0

#define     RESULT_CODE_FAILURE         255


/* Burst device related definition */


#define     STATE_IDLE                  0

#define     STATE_CONFIGURED            1

#define     STATE_BUFFERING             2

#define     STATE_BUFFERING_EMITTING    3

#define     STATE_EMITTING              4


//Extended code indicating the state of the last operation on Consumer IR.

#define     CODE_OK                     0

#define     CODE_START_UP               1

#define     CODE_SUCCESS                3

#define     CODE_BAD_FREQUENCY          4

#define     CODE_TOO_BIG_SIGNAL         5

#define     CODE_BAD_SIZE               6

#define     CODE_ILLEGAL                7


#define     CODE_RESET                  250


#define     CODE_UNDERFLOW              254

#define     CODE_ERROR                  255


#define     BUFFER_FRAME_SIZE           64
