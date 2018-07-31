#include "decode_ir_code.h"
#include <cmath>


uint16_t generate_nec(uint64_t code, uint32_t size, uint16_t *buffer)
{
	uint16_t offset = 0;

	//First add the header.
	buffer[offset++] = NEC_HEADER_MARK;
	buffer[offset++] = NEC_HEADER_SPACE;

	uint64_t mask = 0x1;
	int i;
	for(i = size - 1; i >= 0; --i)//to detect the size of the code
	{
		if((code & (mask << i)) == 0)//logical 0
		{
			buffer[offset++] = NEC_MARK;
			buffer[offset++] = NEC_LOW_SPACE;
		}
		else//logical 1
		{
			buffer[offset++] = NEC_MARK;
			buffer[offset++] = NEC_HIGH_SPACE;
		}
	}
	buffer[offset++] = NEC_MARK;//end of message

	return offset;
}

uint16_t generate_nec_short(uint64_t code, uint32_t size, uint16_t *buffer)
{
	uint16_t offset = 0;

	//First add the header.(lead burst)
	buffer[offset++] = NEC_SHORT_HEADER_MARK;
	buffer[offset++] = NEC_SHORT_HEADER_SPACE;

	uint64_t mask = 0x1;
	int i;
	for(i = size - 1; i >= 0; --i) // to check the number of bits in the code to be sent
	{
		if((code & (mask << i)) == 0)// to send the MSB first
		{
			buffer[offset++] = NEC_SHORT_MARK;// when the and operarion is 0 then that bit is 0
			buffer[offset++] = NEC_SHORT_LOW_SPACE;//(the bit sent is 0)
		}
		else
		{
			buffer[offset++] = NEC_SHORT_MARK;// whe the and operation is not zero the that bit is 1
			buffer[offset++] = NEC_SHORT_HIGH_SPACE;//(the bit sent is 1)
		}
	}
	buffer[offset++] = NEC_SHORT_MARK;//represents the end of message

	return offset;
}

uint16_t generate_rc5(uint64_t code, uint32_t size, uint16_t *buffer)
{
	uint16_t offset = 0;//buffer starts with a mark i.e. buffer[0] is always a mark
	//every buffer[even] is mark and every buffer[odd] is a space

	//Mark Space Mark at the start.(header pulse,two logic 1s)
	buffer[offset++] = RC5_SLICE;//0,1
	buffer[offset++] = RC5_SLICE;//1,2
	buffer[offset++] = RC5_SLICE;//2,3

	uint64_t mask = 0x1;
	int i;
	for(i = size - 1; i >= 0; --i)//to detect the size of the code to be sent
	{
		if((code & (mask << i)) == 0)//for logical 0
		{
			//Space and mark.
			if(offset % 2 == 0)//if the previous bit is different
			{
				//currently at mark. So we can extend the space just before this.
				buffer[offset - 1] += RC5_SLICE; 
				buffer[offset++] = RC5_SLICE;
			}
			else//if the previous bit is thee same as this one
			{
				buffer[offset++] = RC5_SLICE;
				buffer[offset++] = RC5_SLICE;
			}
		}
		else// for logical 1
		{
			//mark and space
			if(offset % 2 == 1)//if the previous bit is different
			{
				//currently at space. So we can extend the mark just before this.
				buffer[offset - 1] += RC5_SLICE;
				buffer[offset++] = RC5_SLICE;			
			}
			else//if the previous bit is the same as this one
			{
				buffer[offset++] = RC5_SLICE;
				buffer[offset++] = RC5_SLICE;
			}
		}
	}

	return offset;
}

uint16_t generate_rc5_38(uint64_t code, uint32_t size, uint16_t *buffer)
{
	uint16_t offset = 0;

	//Mark Space Mark at the start.
	buffer[offset++] = RC5_38_SLICE;
	buffer[offset++] = RC5_38_SLICE;
	buffer[offset++] = RC5_38_SLICE;

	uint64_t mask = 0x1;
	int i;
	for(i = size - 1; i >= 0; --i)
	{
		if((code & (mask << i)) == 0)
		{
			//Space and mark.
			if(offset % 2 == 0)
			{
				//currently at mark. So we can extend the space just before this.
				buffer[offset - 1] += RC5_38_SLICE;
				buffer[offset++] = RC5_38_SLICE;
			}
			else
			{
				buffer[offset++] = RC5_38_SLICE;
				buffer[offset++] = RC5_38_SLICE;
			}
		}
		else
		{
			//mark and space
			if(offset % 2 == 1)
			{
				//currently at space. So we can extend the mark just before this.
				buffer[offset - 1] += RC5_38_SLICE;
				buffer[offset++] = RC5_38_SLICE;
			}
			else
			{
				buffer[offset++] = RC5_38_SLICE;
				buffer[offset++] = RC5_38_SLICE;
			}
		}
	}

	return offset;
}

uint16_t generate_rc5_57(uint64_t code, uint32_t size, uint16_t *buffer)
{
	uint16_t offset = 0;

	//Mark Space Mark at the start.
	buffer[offset++] = RC5_57_SLICE;
	buffer[offset++] = RC5_57_SLICE;
	buffer[offset++] = RC5_57_SLICE;

	uint64_t mask = 0x1;
	int i;
	for(i = size - 1; i >= 0; --i)
	{
		if((code & (mask << i)) == 0)
		{
			//Space and mark.
			if(offset % 2 == 0)
			{
				//currently at mark. So we can extend the space just before this.
				buffer[offset - 1] += RC5_57_SLICE;
				buffer[offset++] = RC5_57_SLICE;
			}
			else
			{
				buffer[offset++] = RC5_57_SLICE;
				buffer[offset++] = RC5_57_SLICE;
			}
		}
		else
		{
			//mark and space
			if(offset % 2 == 1)
			{
				//currently at space. So we can extend the mark just before this.
				buffer[offset - 1] += RC5_57_SLICE;
				buffer[offset++] = RC5_57_SLICE;
			}
			else
			{
				buffer[offset++] = RC5_57_SLICE;
				buffer[offset++] = RC5_57_SLICE;
			}
		}
	}

	return offset;
}

uint16_t generate_rc6(uint64_t code, uint32_t size, uint16_t *buffer)
{//the size should be 4+addr+command,and its sent msb first
	//the fourth bit is the trailer bit, it has time twice the normal time 
	uint16_t offset = 0;//buffer[0] is a mark
	//buffer[even]is a mark,buffer[odd] is a space

	//Mark Space Mark at the start.
	buffer[offset++] = RC6_HEADER_MARK;//0,1(m)
	buffer[offset++] = RC6_HEADER_SPACE;//1,2(s)
	buffer[offset++] = RC6_SLICE;//2,3 (M) this is the start bit(always 1)
	buffer[offset++] = RC6_SLICE;//3,4(S)

	uint64_t mask = 0x1;
	int trail = size - 4;//there are four bits in the header field.first three are the mode bits and the last one is trailer bit
	int i;
	for(i = size - 1; i >= 0; --i)
	{
		int timing = (i == trail)? RC6_SLICE + RC6_SLICE: RC6_SLICE;//the time of the trailer bit is twice the normal bit
		if((code & (mask << i)) == 0)//logical 0
		{
			//Space and mark.
			if(offset % 2 == 0) //even offset,it'll be a mark
			{
				//currently at mark. So we can extend the space just before this.
				buffer[offset - 1] += timing;
				buffer[offset++] = timing;
			}
			else
			{
				buffer[offset++] = timing;
				buffer[offset++] = timing;
			}
		}
		else//logical 1
		{
			//mark and space
			if(offset % 2 == 1)//odd offset,it'll be a space
			{
				//currently at space. So we can extend the mark just before this.
				buffer[offset - 1] += timing;
				buffer[offset++] = timing;
			}
			else
			{
				buffer[offset++] = timing;
				buffer[offset++] = timing;
			}
		}
	}

	return offset;
}

uint16_t generate_rcmm(uint64_t code, uint32_t size, uint16_t *buffer)//12 bit mode
{
	uint16_t offset = 0;

	//First add the header.
	buffer[offset++] = RCMM_HEADER_MARK;
	buffer[offset++] = RCMM_HEADER_SPACE;

	uint64_t mask = 0x3;//we need to get two bits for the modes(0011)
	uint32_t slices = size / 2;
	int i;
	for(i = 0; i < slices; ++i)//tge size of the for loop is only half the code size because we take two bits at a time
	{
		uint64_t dataSlice = (code >> ((slices - i - 1) * 2));//to get two bits 
		dataSlice &= mask; //to mask every other bits other than the ones we need 

		switch(dataSlice)//four differrent cases for four diffeerent modes
		{
		case 0://00 : extended mode
			buffer[offset++] = RCMM_MARK;//6 cycles
			buffer[offset++] = RCMM_SPACE0;//10 cycles
			break;
		case 1://01 : mouse mode
			buffer[offset++] = RCMM_MARK;
			buffer[offset++] = RCMM_SPACE1;//16 cycles
			break;
		case 2:// 10 : keyboard mode 
			buffer[offset++] = RCMM_MARK;
			buffer[offset++] = RCMM_SPACE2;//22 cycles
			break;
		case 3:// 11 : game pad mode
			buffer[offset++] = RCMM_MARK;
			buffer[offset++] = RCMM_SPACE3;//28 cycles
			break;
		}
	}
	buffer[offset++] = RCMM_MARK;

	return offset;
}

uint16_t generate_xmp1(uint64_t code, uint32_t size, uint16_t *buffer)
{
	uint16_t offset = 0;

	//No header.

	return offset;uint64_t mask = 0xf;
	uint32_t slices = size / 4;
	int i;
	for(i = 0; i < slices; ++i)
	{
		if(i % 8 == 0 && i != 0)
		{
			buffer[offset++] = XMP1_MARK;
			buffer[offset++] = XMP1_GAP;
		}

		uint64_t dataSlice = (code >> ((slices - i - 1) * 4));
		dataSlice &= mask;
		
		uint16_t space = (uint16_t)std::floor((dataSlice * XMP1_DATA_MULTI) + 0.5f) + XMP1_PULSE_FIXED;
		buffer[offset++] = XMP1_MARK;
		buffer[offset++] = space;
	}
	buffer[offset++] = XMP1_MARK;

	return offset;
}

uint16_t generate_sony(uint64_t code, uint32_t size, uint16_t *buffer)
{
	uint16_t offset = 0;

	for(int c = 0; c < 3; ++c) 
	{
		//First add the header.
		buffer[offset++] = SONY_HEADER_MARK;
		buffer[offset++] = SONY_HEADER_SPACE;

		uint64_t mask = 0x1;
		for(int i = size - 1; i >= 0; --i)
		{
			if((code & (mask << i)) == 0)//logical 0
			{
				buffer[offset++] = SONY_LOW_MARK;
				buffer[offset++] = SONY_SPACE;
			}
			else//logical 1
			{
				buffer[offset++] = SONY_HIGH_MARK;
				buffer[offset++] = SONY_SPACE;
			}

		}
		//1000 pulses of spaces between 2 frames.
		if(c < 2) //the receiver must wait at least 10ms to make sure that no more pulses follow.
		{
			buffer[offset-1] = 1000+SONY_SPACE;
		}
	}

    offset -= 1;
    return offset;
}

uint16_t generate_pana(uint64_t code, uint32_t size, uint16_t *buffer)
{
	uint16_t offset = 0;

    //First add the header.
    buffer[offset++] = PANA_HEADER_MARK;
    buffer[offset++] = PANA_HEADER_SPACE;

    uint64_t mask = 0x1;
    for(int i = size - 1; i >= 0; --i)
    {
        if((code & (mask << i)) == 0)
        {
            buffer[offset++] = PANA_MARK;
            buffer[offset++] = PANA_LOW_SPACE;
        }
        else
        {
            buffer[offset++] = PANA_MARK;
            buffer[offset++] = PANA_HIGH_SPACE;
        }
    }

    buffer[offset++] = PANA_MARK;

    return offset;
}

uint16_t generate_jvc(uint64_t code, uint32_t size, uint16_t *buffer)
{
	uint16_t offset = 0;

    //First add the header.
    buffer[offset++] = JVC_HEADER_MARK;
    buffer[offset++] = JVC_HEADER_SPACE;

    uint64_t mask = 0x1;
    for(int i = size - 1; i >= 0; --i)
    {
        if((code & (mask << i)) == 0)
        {
            buffer[offset++] = JVC_MARK;
            buffer[offset++] = JVC_LOW_SPACE;
        }
        else
        {
            buffer[offset++] = JVC_MARK;
            buffer[offset++] = JVC_HIGH_SPACE;
        }
    }

    buffer[offset++] = JVC_MARK;

    return offset;
}

uint16_t generate_sharp(uint64_t code, uint32_t size, uint16_t *buffer)
{
	uint16_t offset = 0;

    uint64_t mask = 0x1;
    for(int i = size - 2; i >= 0; --i)
    {
        if (i == 15)
        {
            continue;
        }
        if((code & (mask << i)) == 0)
        {
            buffer[offset++] = SHARP_MARK;
            buffer[offset++] = SHARP_LOW_SPACE;
        }
        else
        {
            buffer[offset++] = SHARP_MARK;
            buffer[offset++] = SHARP_HIGH_SPACE;
        }
        if (i == 16)
        {
            buffer[offset++] = SHARP_MARK;
            buffer[offset++] = SHARP_GAP_SPACE;
        }
    }

    buffer[offset++] = SHARP_MARK;

    return offset;
}

uint16_t generate_rca_38(uint64_t code, uint32_t size, uint16_t *buffer)
{
	uint16_t offset = 0;

    //First add the header.
    buffer[offset++] = RCA38_HEADER_MARK;
    buffer[offset++] = RCA38_HEADER_SPACE;

    uint64_t mask = 0x1;
    for(int i = size - 1; i >= 0; --i)
    {
        if((code & (mask << i)) == 0)
        {
            buffer[offset++] = RCA38_MARK;
            buffer[offset++] = RCA38_LOW_SPACE;
        }
        else
        {
            buffer[offset++] = RCA38_MARK;
            buffer[offset++] = RCA38_HIGH_SPACE;
        }
    }

    buffer[offset++] = RCA38_MARK;

    return offset;
}

uint16_t generate_rca_57(uint64_t code, uint32_t size, uint16_t *buffer)
{
	uint16_t offset = 0;

    //First add the header.
    buffer[offset++] = RCA57_HEADER_MARK;
    buffer[offset++] = RCA57_HEADER_SPACE;

    uint64_t mask = 0x1;
    for(int i = size - 1; i >= 0; --i)
    {
        if((code & (mask << i)) == 0)
        {
            buffer[offset++] = RCA57_MARK;
            buffer[offset++] = RCA57_LOW_SPACE;
        }
        else
        {
            buffer[offset++] = RCA57_MARK;
            buffer[offset++] = RCA57_HIGH_SPACE;
        }
    }

    buffer[offset++] = RCA57_MARK;

    return offset;
}

uint16_t generate_mitsubishi(uint64_t code, uint32_t size, uint16_t *buffer)
{
    uint16_t offset = 0;

    //First add the header.
    buffer[offset++] = MITSUBISHI_HEADER_MARK;
    buffer[offset++] = MITSUBISHI_HEADER_SPACE;

    uint64_t mask = 0x1;
    for(int i = size - 1; i >= 0; --i)
    {
        if((code & (mask << i)) == 0)
        {
            buffer[offset++] = MITSUBISHI_MARK;
            buffer[offset++] = MITSUBISHI_LOW_SPACE;
        }
        else
        {
            buffer[offset++] = MITSUBISHI_MARK;
            buffer[offset++] = MITSUBISHI_HIGH_SPACE;
        }
    }

    buffer[offset++] = MITSUBISHI_MARK;

    return offset;
}

uint16_t generate_konka(uint64_t code, uint32_t size, uint16_t *buffer)
{
    uint16_t offset = 0;

    //First add the header.
    buffer[offset++] = KONKA_HEADER_MARK;
    buffer[offset++] = KONKA_HEADER_MARK;

    uint64_t mask = 0x1;
    for(int i = size - 1; i >= 0; --i)
    {
        if((code & (mask << i)) == 0)
        {
            buffer[offset++] = KONKA_MARK;
            buffer[offset++] = KONKA_LOW_SPACE;
        }
        else
        {
            buffer[offset++] = KONKA_MARK;
            buffer[offset++] = KONKA_HIGH_SPACE;
        }
    }

    buffer[offset++] = KONKA_MARK;
    buffer[offset++] = KONKA_TRAILER_SPACE;
    buffer[offset++] = KONKA_MARK;

    return offset;
}

uint16_t generate_test_seq(uint64_t code, uint32_t size, uint16_t *buffer)
{
	uint16_t offset = 0;

    buffer[offset++] = 65500; // continuous carrier for ~1.7s
    buffer[offset++] = 100; // brief space

	return offset;
}


